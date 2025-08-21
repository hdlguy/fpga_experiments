# QSPI Flash
This project demonstrates access to the QSPI configuration flash of three different board with three different types of flash. The ultimate goal is to implement over-the-air FPGA bitstream updates using Multi-boot techniques.

## Method

A Microblaze CPU is used to access the flash through a Xilinx AXI QSPI Controller. C code runs the following steps:

    - JEDEC Flash ID detection
    - Flash erase on address range 0x40_0000 to 0x7F_FFFF.
    - Blank detect over above range.
    - Flash write with random numbers in above range.
    - Flash read and verify over above range.

## Files
    - arty - FPGA and C code for the Digilent Arty A7 (35T or 100T).
    - neso - same for the Numato Neso
    - custom - same for a custom FPGA module with A7-100T and Spansion flash

## Conclusion
The only significant difference between the operation of these flash chips turned out to be the number of dummy bytes between the read command and the start of valid data.  This code reads the JEDEC ID register and adjusts the number of dummy bytes to make the same code work for Spansion, Micron and ISSI flash.

Console output: 
```
**** qspi_flash_test ****
FPGA_ID = 0x00010203, FPGA_VERSION = 0x00010203
** ID Bytes
FF 9D 60 18 9D 60 18 9D 60 18 9D 60 18 9D 60 18 9D 60 18 9D 60 18 9D 60
flash = ISSI
**** Erase ****
Sector 64/64
**** Blank Check ****
Page 16384/16384
**** Write ****
Page 16384/16384
** Verify
Page 16384/16384
********** Success! ***********
```
