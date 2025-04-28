# mem_regfile
This is a project to make a little register file core that connects to memory interface of the Xilinx AXI Block Ram Controller.

## Motivation
Currently I use an axi register file block that has a native AXI4 Lite interface. The register file side of the block cannot go inside the IPI because it uses arrays of 32 bit words.  Since it is external to IPI the XSA file does not get the base address for it. Also, since it is AXI4 Lite it cannot accept burst transactions.

It is hoped that a register file that connects to the bram controller will fix all these limitations.

