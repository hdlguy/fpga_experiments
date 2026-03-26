# generic IO 
This subproject studies a way to handle using the same source code for two boards with slightly different IO and logic.

## Approach
The IO superset approach will be used in a common top-level Systemverilog file. 
-   top.sv will have all the IO needed in board A or in board B.  
-   A separate build folder will be maintained for each board, implement_0, implement_1.
-   A generic will be passed using "set_property generic {BOARD_VERSION=0} [current_fileset]" in the setup.tcl script.  
-   Different XDC files will be created for each board. 
-   Top.sv will use generate-if statements to control generated code.

## Conclusion
In this example, a common top.sv can be used on two different boards. The top level IO is a superset that includes ports needed for both or either boards.

