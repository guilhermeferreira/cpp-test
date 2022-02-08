import sys
import time

from ctypes import cdll, c_int, c_uint, c_double, c_ubyte, byref

if sys.platform.startswith('win'):
    dwf = cdll.LoadLibrary('dwf.dll')
elif sys.platform.startswith('darwin'):
    dwf = cdll.LoadLibrary('/Library/Frameworks/dwf.framework/dwf')
else:
    dwf = cdll.LoadLibrary('libdwf.so')


class SpiMaster:
    """SpiMaster class."""

    # Device type of Analog Discovery 2.
    DEV_TYPE = 3

    def __init__(self, frequency: int, polarity: int, phase: int, word_bits: int, ss_pin: int,
                 cl_pin: int, mo_pin: int, mi_pin: int):
        """SpiMaster constructor.

        Args:
            frequency (int): SPI clock frequency, must be less than 50e6
            polarity (int): SPI clock polarity, either 0 or 1
            phase (int): SPI clock phase, either 0 or 1
            word_bits (int): bits to be transferred [8, 16, 24, 32]
            ss_pin (int): pin to use for SS signal [0-7]
            cl_pin (int): pin to use for CLK signal [0-7]
            mo_pin (int): pin to use for MO signal [0-7]
            mi_pin (int): pin to use for MI signal [0-7]
        """
        assert frequency < 50e6
        assert polarity == 0 or polarity == 1
        assert phase == 0 or phase == 1
        assert word_bits in [8, 16, 24, 32]
        assert ss_pin >= 0 and ss_pin < 8
        assert cl_pin >= 0 and cl_pin < 8
        assert mo_pin >= 0 and mo_pin < 8
        assert mi_pin >= 0 and mi_pin < 8

        self.frequency = frequency
        self.polarity = polarity
        self.phase = phase
        self.word_bits = word_bits
        self.ss_pin = ss_pin
        self.cl_pin = cl_pin
        self.mo_pin = mo_pin
        self.mi_pin = mi_pin

        self.dev_handle = c_int()

        self._setup_device()
        self._setup_digital_out()
        self._setup_digital_in()

    def __del__(self):
        time.sleep(0.1)
        dwf.FDwfDigitalOutReset(self.dev_handle)
        dwf.FDwfDigitalInReset(self.dev_handle)
        dwf.FDwfDeviceCloseAll()

    def transfer(self, tx_word: int):
        """Perform an SPI transfer.

        Args:
            tx_word (int): word to be sent (sent as MSB first)

        Returns:
            int: received word

        Raises:
            Exception: when some receive samples are lost or corrupted
        """

        # Make a list of bytes out of tx_word.
        data = []
        data_bytes = self.word_bits // 8
        for i in range(0, data_bytes):
            # Extract byte.
            byte = (tx_word & (0xFF << (i * 8))) >> (i * 8)
            # Data is sent out LSB first, thus we must flip the bits.
            data.insert(0, int('{:08b}'.format(byte)[::-1], 2))

        # Add an extra null byte at the end, to be sent out after the last clock pulse, and before
        # the slave select line goes inactive again.
        data_bytes += 1
        data += [0]

        # Fill in MOSI data buffer. Number of data bits: word_bits plus one null bit at the end.
        dwf.FDwfDigitalOutDataSet(self.dev_handle, c_int(self.mo_pin),
                                  byref((data_bytes * c_ubyte)(*data)),
                                  c_int(self.word_bits + 8))

        out_status = c_ubyte()
        in_status = c_ubyte()
        samples = (c_ubyte * self.word_bits)()

        # Start and trigger DigitalOut and DigitalIn instruments.
        dwf.FDwfDigitalOutConfigure(self.dev_handle, c_int(1))
        dwf.FDwfDigitalInConfigure(self.dev_handle, c_int(0), c_int(1))
        dwf.FDwfDeviceTriggerPC(self.dev_handle)

        available = c_int()
        lost = c_int()
        corrupted = c_int()
        received = 0

        # Receive samples from DigitalIn instrument.
        while received < self.word_bits:

            # Continue if sampling has not started yet.
            dwf.FDwfDigitalInStatus(self.dev_handle, c_int(1), byref(in_status))
            sv = in_status.value
            if received == 0 and (sv == 1 or sv == 4 or sv == 5):
                continue

            # Get number of available samples and number of lost/corrupted samples.
            dwf.FDwfDigitalInStatusRecord(self.dev_handle,
                                          byref(available), byref(lost), byref(corrupted))

            if lost.value:
                raise Exception(f'There were {lost.value} lost samples')

            if corrupted.value:
                raise Exception(f'There were {corrupted.value} corrupted samples')

            # Continue if no samples are available yet.
            if available.value == 0:
                continue

            # Do not receive more samples than wanted.
            if available.value > self.word_bits - received:
                available.value = self.word_bits - received

            # Actually receive samples.
            dwf.FDwfDigitalInStatusData(self.dev_handle, byref(samples, received), available)
            received += available.value

        # Wait for DigitalOut instrument to finish.
        dwf.FDwfDigitalOutStatus(self.dev_handle, byref(out_status))
        while out_status.value != 2:
            dwf.FDwfDigitalOutStatus(self.dev_handle, byref(out_status))

        # Extract MISO channel and build received word.
        mi_word = 0
        for s in samples:
            mi_bit = (s & (1 << self.mi_pin)) >> self.mi_pin
            mi_word = (mi_word << 1) | mi_bit

        return mi_word

    def _setup_device(self):
        dev_index = c_int()

        # List all connected devices.
        number_of_devices = c_int()
        dwf.FDwfEnum(c_int(0), byref(number_of_devices))

        # Loop through connected devices to find one whose type matches what we are looking for.
        dtype = c_int()
        drev = c_int()
        found_dev = False
        for dev in range(0, number_of_devices.value):
            dwf.FDwfEnumDeviceType(c_int(dev), byref(dtype), byref(drev))
            if dtype.value == self.DEV_TYPE:
                dev_index = c_int(dev)
                found_dev = True
                break

        if not found_dev:
            raise Exception(f'Could not find connected device with type {self.DEV_TYPE}')

        # Open device.
        dwf.FDwfDeviceOpen(dev_index, byref(self.dev_handle))
        if self.dev_handle.value == 0:
            raise Exception(f'Could not open device {dev_index.value}')

    def _setup_digital_out(self):
        clock_freq = c_double()

        dwf.FDwfDigitalOutReset(self.dev_handle)

        # Get clock frequency of DigitalOut instrument.
        dwf.FDwfDigitalOutInternalClockInfo(self.dev_handle, byref(clock_freq))

        # SPI slave select configuration.
        # --> enable pin
        dwf.FDwfDigitalOutEnableSet(self.dev_handle, c_int(self.ss_pin), c_int(1))
        # --> set pin to high-idle (low-active)
        dwf.FDwfDigitalOutIdleSet(self.dev_handle, c_int(self.ss_pin), c_int(2))
        # --> initial level low, initial counter 0
        dwf.FDwfDigitalOutCounterInitSet(self.dev_handle, c_int(self.ss_pin), c_uint(0), c_uint(0))
        # --> 0 pulses low, 0 pulses high, thus output constant while running (stays low during the
        #     whole transfer)
        dwf.FDwfDigitalOutCounterSet(self.dev_handle, c_int(self.ss_pin), c_uint(0), c_uint(0))

        # SPI clock configuration.
        # --> enable pin
        dwf.FDwfDigitalOutEnableSet(self.dev_handle, c_int(self.cl_pin), c_int(1))
        # --> set pin to low-idle (or high-idle for clock polarity 1)
        dwf.FDwfDigitalOutIdleSet(self.dev_handle, c_int(self.cl_pin), c_int(1 + self.polarity))
        # --> toggle data at 2 * SPI frequency
        dwf.FDwfDigitalOutDividerSet(self.dev_handle, c_int(self.cl_pin),
                                     c_uint(int(clock_freq.value / self.frequency / 2)))
        # --> custom output type (sample rate = internal frequency / divider)
        dwf.FDwfDigitalOutTypeSet(self.dev_handle, c_int(self.cl_pin), c_int(1))

        # SCLK output configuration. We want the SCLK line
        # A) to output a stream of leading and trailing edges, word_bits times
        # B) to remain inactive for half a clock period before the first leading edge (after the
        #    slave select line becomes active)
        # C) to remain inactive for one clock period after the last trailing edge (before the slave
        #    select line goes inactive again)
        #
        # Since we're treating the SCLK line as a data line, this translates to
        # A) one bit high (low for polarity 1), one bit low (high for polarity 1), word_bits times
        # B) one bit low (high for polarity 1) before the first bit of A)
        # C) two bits low (high for polarity 1) after the last bit of A)
        # for a total of (2 * word_bits + 1 + 2) bits
        #
        # To store (2 * word_bits) bits, we need (2 * word_bits / 8) bytes.
        data_bytes = 2 * self.word_bits // 8
        # A) high-low (or low-high) pulses, LSB first
        clock_byte = 0b01010101 if (self.polarity == 0) else 0b10101010
        # B) insert one low (or high) bit before the high-low (or low-high) pulses
        clock_byte = ((clock_byte << 1) & 0xFF) | self.polarity
        data = [clock_byte for i in range(0, data_bytes)]
        # C) append inactive bits after the high-low (or low-high) pulses
        data_bytes += 1
        data += [0x00 if (self.polarity == 0) else 0xFF]
        # Number of SCLK data bits: (2 * word_bits + 1 + 2)
        dwf.FDwfDigitalOutDataSet(self.dev_handle, c_int(self.cl_pin),
                                  byref((data_bytes * c_ubyte)(*data)),
                                  c_int(2 * self.word_bits + 1 + 2))

        # SPI MOSI configuration.
        # --> enable pin
        dwf.FDwfDigitalOutEnableSet(self.dev_handle, c_int(self.mo_pin), c_int(1))
        # --> set pin to low-idle
        dwf.FDwfDigitalOutIdleSet(self.dev_handle, c_int(self.mo_pin), c_int(1))
        # --> for clock phase 0, hold the first bit for 1 period (until the first trailing clock
        #     edge), for clock phase 1, hold the first bit for 1.5 periods (until the second leading
        #     clock edge)
        dwf.FDwfDigitalOutDividerInitSet(self.dev_handle, c_int(self.mo_pin),
                                         c_uint(int((1 + 0.5 * self.phase) *
                                                clock_freq.value / self.frequency)))
        # --> toggle data at SPI frequency
        dwf.FDwfDigitalOutDividerSet(self.dev_handle, c_int(self.mo_pin),
                                     c_uint(int(clock_freq.value / self.frequency)))
        # --> custom output type (sample rate = internal frequency / divider)
        dwf.FDwfDigitalOutTypeSet(self.dev_handle, c_int(self.mo_pin), c_int(1))

        # Set time to remain in the running state for (word_bits clock periods, plus half a period
        # before the first leading edge, plus one period before last trailing edge).
        dwf.FDwfDigitalOutRunSet(self.dev_handle,
                                 c_double((self.word_bits + 0.5 + 1) / self.frequency))

        # Set DigitalOut instrument to be triggered by PC.
        dwf.FDwfDigitalOutTriggerSourceSet(self.dev_handle, c_int(1))

    def _setup_digital_in(self):
        dwf.FDwfDigitalInReset(self.dev_handle)

        # Read 8 bits (channels) per sample.
        dwf.FDwfDigitalInSampleFormatSet(self.dev_handle, c_int(8))

        # Record (stream to PC) acquisition mode. Acquire word_bits samples.
        dwf.FDwfDigitalInAcquisitionModeSet(self.dev_handle, c_int(3))
        dwf.FDwfDigitalInTriggerPositionSet(self.dev_handle, c_int(self.word_bits))

        # Sync mode (divider = -1)
        dwf.FDwfDigitalInDividerSet(self.dev_handle, c_int(-1))

        # Sample on SPI clock edge. Falling edge if polarity is 0 and phase is 1, or if polarity is
        # 1 and phase is 0.
        trigger_mask = 1 << self.cl_pin
        rising_edge = 0 if (self.polarity ^ self.phase) else trigger_mask
        falling_edge = trigger_mask if (self.polarity ^ self.phase) else 0
        dwf.FDwfDigitalInTriggerSet(self.dev_handle, 0, 0, rising_edge, falling_edge)
