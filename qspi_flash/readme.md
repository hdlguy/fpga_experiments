# QSPI Flash
This project demonstrates access to the QSPI configuration flash of three different board with three different types of flash. The ultimate goal is to implement over-the-air FPGA bitstream updates using Multi-boot techniques.

## Method

A Microblaze CPU is used to access the flash through a Xilinx AXI QSPI Controller. C code runs the following steps:

    - JEDEC Flash ID detection
    - Flash erase on address range 0x40_0000 to 0x7F_FFFF.
    - Blank deted over above range.
    - Flash write with random numbers in above range.
    - Flash read and verify over above range.

## Files
    - arty - FPGA and C code for the Digilent Arty A7 (35T or 100T).
    - neso - same for the Numato Neso
    - custom - same for a custom FPGA module with A7-100T and Spansion flash

