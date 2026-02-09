# Zynq-Microblaze
This little project shows one way to incorporate a Microblaze processor into the PL of a Zynq 7000 device on a MicroZed development board using Vitis/Vivado 2025.2.

Two Vitis projects are provided, one for the Arm PS (./zy_vitis) and the other for the Microblaze (./mb_vitis).  The FPGA compile scripts are setup so that the Microblaze compiled C file (.elf) is automatically included in the bit file.

The two processors interact through a shared dual port block ram.  The Microblaze writes 0xffffffff, once per second, to location zero of the bram.  The Zynq Arm continuously polls the bram location and clears it when it sees it go to a non-zero value.

The Zynq Arm processor prints messages to its standard serial port. The Microblaze prints messages to a JTAG uart enabled in the debug module.




