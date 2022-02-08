# How to Test Using Analog Discovery 2

This guide assumes QNodeOS runs on the [MicroZed board][microzed-start], but
most of the instructions are generic.

## Required Hardware

- [MicroZed board][microzed-start]
- JTAG programmer
- Analog Discovery 2 (AD2)
- Additional UART connection for interface to host

## Set-Up the AD2

Check the [Analog Discovery 2 - Getting Started Guide][ad2-start] for how to
download Waveforms, which contains the SDK to use the AD2.

Then, connect the AD2 to the SPI interface on the MicroZed, as indicated in the
following table:

| Signal   | Channel |
|:---------|:--------|
| SPI SS   | 0       |
| SPI SCLK | 1       |
| SPI MOSI | 2       |
| SPI MISO | 3       |
| SPI GND  | Ground  |

## Run Test

1. Program the FPGA if needed [^1] and flash the `*.elf` executable
2. Start the host Python program
3. From within this `test` directory, launch the AD2 script `test.py` and
   observe the output

To run the script, just do
```sh
python test.py
```

To start from a QDevice cycle other than the default `0`, do
```sh
python test.py -c <cycle>
```

To run the script in interactive mode (one QDevice transaction at a time,
triggered by pressing ENTER), do
```sh
python test.py -i
```

For other command-line options, do
```sh
python test.py -h
```

[^1]: Programming the FPGA should only be necessary once after powering on the
      MicroZed board. If the received SPI messages look weird and get stuck into
      garbage data, reprogramming the FPGA might prove useful.

[microzed-start]: ../../../../platforms/microzed/README.md
[ad2-start]: https://reference.digilentinc.com/reference/instrumentation/analog-discovery-2/getting-started-guide
[example-py]: ../../../../host/py/example.py
