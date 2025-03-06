# run at linux command line 
#       xsct setup.tcl
#       vitis --classic --workspace ./workspace
#
file delete -force ./workspace

set hw ../implement/results/top.xsa
set proc "microblaze_2"

setws ./workspace

platform create -name "standalone_plat" -hw $hw -proc $proc -os standalone
#bsp config stdout "usb_uartlite"
#bsp config stdin  "usb_uartlite"

#app create -name production -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/production/src/test.c               ../../../src/production/test.c
#file link -symbolic ./workspace/production/src/fpga.h               ../../../src/fpga.h

#app create -name icap_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/icap_test/src/test.c               ../../../src/icap_test/test.c
#file link -symbolic ./workspace/icap_test/src/fpga.h               ../../../src/fpga.h

app create -name hello1 -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
file link -symbolic ./workspace/hello1/src/test.c               ../../../src/hello1/test.c
file link -symbolic ./workspace/hello1/src/fpga.h               ../../../src/fpga.h

app build all

