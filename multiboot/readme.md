# multiboot
This project hopes to demonstrate multiboot with fallback on the Arty A7-100T. See XAPP1247.

The TCL script compile.tcl converts single source, top.sv, into two bitstreams, top1.bit for the golden copy at 0x000000 and top2.bit for the production copy at 0x400000.

The TCL script spi_program.tcl combines the two bitstreams into a single .mcs file and burns it into the configuration flash of the Arty.


