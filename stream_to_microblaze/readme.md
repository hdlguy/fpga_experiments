# Dual Microblaze
This mini-project demonstrates techniques to run two independent Microblaze processors in a single Artix-7 FPGA.

## Description
A Digilent Arty A7-100T development board was used.  A single IP Integrator block diagram contains both processors microblaze_0 and microblaze_1. Two Vitis --classic projects were created, vitis0 and vitis1, for the two processors.  TCL scripts are provided to compile the C code into the FPGA and burn the configuration flash. Scripts are provided in each of the Vitis projects to setup debugging of the individual processors.

## Conclusion
Dual Microblaze processors are much more difficult to manage than a single one.  Putting both processors in a single IPI block diagram lets Vivado see all the connections so it can write a correct and complete XSA file.  In the end, the debugging environment is not perfect. I did not succeed to have both processors running during a debug session.

