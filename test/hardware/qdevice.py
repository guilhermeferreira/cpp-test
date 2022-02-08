import enum
from spi import SpiMaster

####################################################################################################
# Utils
####################################################################################################

crc_table = [
        0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
        0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
        0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
        0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
        0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
        0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
        0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
        0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
        0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
        0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
        0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
        0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
        0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
        0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
        0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
        0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
        0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
        0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
        0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
        0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
        0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
        0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
        0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
        0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
        0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
        0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
        0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
        0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
        0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
        0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
        0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
        0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3,
]


def crc_8(byte_list):
    crc = 0
    for byte in byte_list:
        crc = crc_table[byte ^ crc]
    return crc


####################################################################################################
# QDevice interface
####################################################################################################


class QDevice:
    """QDevice class."""

    class Status(enum.IntEnum):
        READY = 0x00
        OUTCOME_READY = 0x01
        BUSY_CMD = 0x02
        BUSY_LINK = 0x04

    class Outcome(enum.IntEnum):
        ERR_UNKNOWN = 0x00
        OK = 0x08

    class EntState(enum.IntEnum):
        PHI_PLUS = 0x00
        PHI_MINUS = 0x01
        PSI_PLUS = 0x02
        PSI_MINUS = 0x03

    class RotAxis(enum.IntEnum):
        X = 0x00
        Y = 0x01
        Z = 0x02

    class MsrBasis(enum.IntEnum):
        Z = 0x00
        ARBITRARY = 0x01

    class PmgAxis(enum.IntEnum):
        XYX = 0x00
        YZY = 0x01
        ZXZ = 0x02

    class CmdType(enum.IntEnum):
        NOP = 0x00
        INI = 0x01
        SQG = 0x02
        TQG = 0x03
        MSR = 0x04
        ENT = 0x05
        ENM = 0x06
        PMG = 0x07
        MOV = 0x08
        SWP = 0x09
        ESW = 0x0A
        INV = 0xFF  # invalid operation

    def __init__(self):
        """QDevice constructor."""

        self.spi = SpiMaster(
                frequency=int(12.5e6),
                polarity=0,
                phase=1,
                word_bits=32,
                ss_pin=0,
                cl_pin=1,
                mo_pin=2,
                mi_pin=3
                )

        self.cycle_count = 0
        self.status = self.Status.READY
        self.outcome = self.Outcome.OK

        self.lp_data = [0] * 32
        self.lp_data[0] = 0xDEADBEEF

        self.last_sent_msg = {}
        self.last_rcvd_msg = {}

        self.stats = {'rcvd_instructions': {}}
        for cmd_type in self.CmdType:
            self.stats['rcvd_instructions'][cmd_type.value] = 0

    def send_recv_msg(self):
        """Perform a QDevice transaction.

        Returns:
            Tuple[Dict, Dict]: sent message, received message

        Raises:
            Exception: when a CRC error on the received message occurs

        Sent message is a dictionary containing:
            cycle_count (int): least-significant byte of the cycle count
            status (QDevice.Status): QDevice status
            outcome (QDevice.Outcome): QDevice outcome
            raw (int): raw message

        Received message is a dictionary containing:
            operation (QDevice.CmdType): QDevice operation, can be invalid (QDevice.CmdType.INV)
            raw (int): raw message
        """

        # Update low-priority data table when LSB of cycle_count is 0.
        if (self.cycle_count & 0xFF) == 0:
            self.lp_data[2] = self.cycle_count & 0xFFFFFFFF
            self.lp_data[3] = (self.cycle_count & (0xFFFFFFFF << 32)) >> 32

        send = self._encode_qdevice_msg()
        recv = self.spi.transfer(send)

        # Compute CRC of received message.
        octet_1 = (recv & (0xFF << 24)) >> 24
        octet_2 = (recv & (0xFF << 16)) >> 16
        octet_3 = (recv & (0xFF << 8)) >> 8
        octet_4 = recv & 0xFF
        crc = crc_8([octet_1, octet_2, octet_3]) & 0xFF
        if crc != octet_4:
            raise Exception('CRC error on received message(computed: 0x{:02X}, expected: 0x{:02X},'
                            'received message: 0x{:08X})'.format(crc, octet_4, recv))

        try:
            operation = self.CmdType((recv & 0xFF000000) >> 24)
        except ValueError:
            operation = self.CmdType.INV

        sent_msg = {
                'cycle_count': self.cycle_count,
                'status': self.status,
                'outcome': self.outcome,
                'raw': send
                }

        rcvd_msg = {
                'operation': operation,
                'raw': recv
                }

        self.last_sent_msg = sent_msg
        self.last_rcvd_msg = rcvd_msg

        self.stats['rcvd_instructions'][operation] += 1

        self.cycle_count += 1
        return sent_msg, rcvd_msg

    def print_last_msg(self):
        """Print last received and sent message."""

        try:
            status = self.last_sent_msg['status'].name
        except AttributeError:
            status = '0x{:02X}'.format(self.last_sent_msg['status'])

        try:
            outcome = self.last_sent_msg['outcome'].name
        except AttributeError:
            outcome = '0x{:02X}'.format(self.last_sent_msg['outcome'])

        print('Rcvd: OP = {:3s} (0x{:08X}) | Sent: CC = {:6d}, ST = {:13s}, OC = {:11s} (0x{:08X})'
              .format(
                  self.last_rcvd_msg['operation'].name, self.last_rcvd_msg['raw'],
                  self.last_sent_msg['cycle_count'], status, outcome, self.last_sent_msg['raw']))

    def print_stats(self):
        """Print QDevice statistics."""

        total_instructions = 0

        for cmd_type in self.CmdType:
            instructions = self.stats['rcvd_instructions'][cmd_type.value]
            total_instructions += instructions
            print('Total received {} instructions: {}'.format(
                  cmd_type.name, instructions))

        if total_instructions:
            nops_to_instructions_ratio = (self.stats['rcvd_instructions'][self.CmdType.NOP] /
                                          total_instructions)
            print('NOPs-to-all-instructions ratio: {}'.format(nops_to_instructions_ratio))

    def reset_stats(self):
        """Reset QDevice statistics."""

        for cmd_type in self.CmdType:
            self.stats['rcvd_instructions'][cmd_type.value] = 0

    def _encode_qdevice_msg(self):
        # |   4th octet   |   3rd octet   |   2nd octet   |   1st octet   |
        # |7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|
        # +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        # |      CRC      |   0   |   LP  |   OC  |   ST  |  CYCLE_COUNT  |
        # +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        # See full doc:
        # https://3.basecamp.com/3321769/buckets/14171081/documents/2430858045

        # An lp-data nibble is composed of 4 bits, while an lp-data word is composed of 32 bits.
        nibbles_per_word = 32 // 4
        # We index into the lp-data table.
        lp_data_word = self.lp_data[(self.cycle_count & 0xFF) // nibbles_per_word]
        # Then we extract the 4-bit nibble from the lp-data word.
        nibble_index = self.cycle_count & (nibbles_per_word - 1)
        nibble_offset = nibble_index * 4
        lp_data = (lp_data_word & (0x0F << nibble_offset)) >> nibble_offset

        octet_1 = (self.cycle_count & 0xFF)
        octet_2 = (self.status & 0x0F) | (self.outcome & 0x0F) << 4
        octet_3 = (lp_data & 0xFF)
        octet_4 = crc_8([octet_1, octet_2, octet_3]) & 0xFF
        msg = octet_1 << 24 | octet_2 << 16 | octet_3 << 8 | octet_4
        return msg
