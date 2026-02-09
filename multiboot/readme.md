# Multiboot with Fallback and Barrier Images
This project hopes to demonstrate multiboot with fallback and barrier images on the Arty A7-100T. See XAPP1247.

## Purpose
These techniques will be used to provide field update capability to a device that contains a Xilinx Artix-7 device.  In that application an Ethernet connection is used to reprogram the fpga configurtion flash so that neither a USB JTAG nor Vivado software is needed to update the application.  This method burns a golden bitstream and a production bitstream into the configuration flash. The idea is that if the production bitstream is corrupted during the update process, the fpga reverts to the golden copied which can be used to burn the production bitstream again. The barrier images are very small bitstreams that set the configuration watchdog timer and are programmed at addresses below and above the production bitstreams.  The barrier images prevent bricking of the device in the unlikely event that the update process is interrupted between erasing the production bitstream and burning the new image.

## Method
The TCL script compile.tcl converts the single source, top.sv, into two bitstreams, top1.bit for the golden copy and top2.bit for the production copy.  Then barriers.tcl creates two small barrier images with the configuration watchdog timer enabled, timer1.bin and timer2.bin. The four images are combined into a single .mcs file with this command.

    write_cfgmem -force -format mcs -interface SPIx4 -size 128 -loadbit "up 0x000000  ./results/top1.bit up 0x400000 ./results/top2.bit" -loaddata "up 0x3F0000 ./timer1.bin up 0x7F0000 ./timer2.bin" ./results/top.mcs

## Testing

The production bitfile, top2.bit was modified with hexedit to cause a CRC error.  Then spi_program.tcl was used to burn the files into the configuration flash. The Hardware Manager was then used to check the boot status registers and the fallback bit was set.

Next the above write_cfgmen command was modified to exclude the top2.bit from the mcs file.  Again the fallback boot status bit was set.


