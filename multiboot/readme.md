# Multiboot with Fallback and Barrier Images
This project hopes to demonstrate multiboot with fallback and barrier images on the Arty A7-100T. See XAPP1247.

## Method
The TCL script compile.tcl converts the single source, top.sv, into two bitstreams, top1.bit for the golden copy and top2.bit for the production copy.  Then barriers.tcl creates two small barrier images with the configuration watchdog timer enabled, timer1.bin and timer2.bin. The four images are combined into a single .mcs file with this command.

    write_cfgmem -force -format mcs -interface SPIx4 -size 128 -loadbit "up 0x000000  ./results/top1.bit up 0x400000 ./results/top2.bit" -loaddata "up 0x3F0000 ./timer1.bin up 0x7F0000 ./timer2.bin" ./results/top.mcs

## Testing

The production bitfile, top2.bit was modified with hexedit to cause a CRC error.  Then spi_program.tcl was used to burn the files into the configuration flash. The Hardware Manager was then used to check the boot status registers and the fallback bit was set.

Next the above writ_cfgmen command was modified to exclude the top2.bit from the mcs file.  Again the fallback boot status bit was set.


