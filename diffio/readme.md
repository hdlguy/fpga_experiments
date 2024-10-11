# diffio sub-project
This project explores sourcing external differential clocks from inside the fpga.  Input and output IO buffers are connected to the same pin so that a design that contains differential clocks can be emulated without touching its source code.

This project also uses an extra xdc file to insert ILA cores without touching the source code.

