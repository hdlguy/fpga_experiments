# run at linux command line 
#       xsct setup.tcl
#       vitis --classic --workspace ./workspace
#
file delete -force ./workspace

#set hw ../implement_arty/results/top.xsa
set hw ../implement_neso/results/top.xsa
set proc "microblaze_0"

setws ./workspace

#platform create -name "freertos_plat" -hw $hw -proc $proc -os freertos
#bsp config stdout "usb_uartlite"
#bsp config stdin  "usb_uartlite"

platform create -name "standalone_plat" -hw $hw -proc $proc -os standalone
bsp config stdout "usb_uartlite"
bsp config stdin  "usb_uartlite"

app create -name production -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
file link -symbolic ./workspace/production/src/test.c               ../../../src/production/test.c
file link -symbolic ./workspace/production/src/fpga.h               ../../../src/fpga.h

#app create -name fan_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/fan_test/src/test.c               ../../../src/fan_test/test.c
#file link -symbolic ./workspace/fan_test/src/fpga.h               ../../../src/fpga.h

#app create -name i2c_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/i2c_test/src/test.c               ../../../src/i2c_test/test.c
#file link -symbolic ./workspace/i2c_test/src/fpga.h               ../../../src/fpga.h

#app create -name direct_i2c_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/direct_i2c_test/src/test.c               ../../../src/direct_i2c_test/test.c
#file link -symbolic ./workspace/direct_i2c_test/src/fpga.h               ../../../src/fpga.h

#app create -name iic_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/iic_test/src/test.c               ../../../src/iic_test/test.c
#file link -symbolic ./workspace/iic_test/src/fpga.h               ../../../src/fpga.h

#app create -name amc7823_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/amc7823_test/src/test.c               ../../../src/amc7823_test/test.c
#file link -symbolic ./workspace/amc7823_test/src/fpga.h               ../../../src/fpga.h
#file link -symbolic ./workspace/amc7823_test/src/amc7823.h            ../../../src/amc7823/amc7823.h

#app create -name rick_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/rick_test/src/test.c               ../../../src/rick_test/test.c
#file link -symbolic ./workspace/rick_test/src/fpga.h               ../../../src/fpga.h

#app create -name auto_adc_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/auto_adc_test/src/test.c               ../../../src/auto_adc_test/test.c
#file link -symbolic ./workspace/auto_adc_test/src/fpga.h               ../../../src/fpga.h

#app create -name timer_intr_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/timer_intr_test/src/test.c        ../../../src/timer_intr_test/test.c
#file link -symbolic ./workspace/timer_intr_test/src/fpga.h        ../../../src/fpga.h

#app create -name iic_timer_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/iic_timer_test/src/test.c        ../../../src/iic_timer_test/test.c
#file link -symbolic ./workspace/iic_timer_test/src/fpga.h        ../../../src/fpga.h

#app create -name hello1 -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/hello1/src/test.c               ../../../src/hello1/test.c
#file link -symbolic ./workspace/hello1/src/platform.c           ../../../src/hello1/platform.c
#file link -symbolic ./workspace/hello1/src/platform.h           ../../../src/hello1/platform.h
#file link -symbolic ./workspace/hello1/src/platform_config.h    ../../../src/hello1/platform_config.h
#file link -symbolic ./workspace/hello1/src/fpga.h               ../../../src/fpga.h

#file delete -force  ./workspace/timer_intr_test/src/lscript.ld
#file link -symbolic ./workspace/timer_intr_test/src/lscript.ld   ../../../src/timer_intr_test/lscript.ld

#app create -name dac_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/dac_test/src/test.c               ../../../src/dac_test/test.c
#file link -symbolic ./workspace/dac_test/src/fpga.h               ../../../src/fpga.h
#file link -symbolic ./workspace/dac_test/src/amc7823.h          ../../../src/amc7823/amc7823.h
#file link -symbolic ./workspace/dac_test/src/amc7823.c          ../../../src/amc7823/amc7823.c

#app create -name dac_test -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/dac_test/src/test.c               ../../../src/dac_test/test.c
#file link -symbolic ./workspace/dac_test/src/amc7823.h            ../../../src/dac_test/amc7823.h

#app create -name tec_control -platform standalone_plat -domain standalone_domain -template "Empty Application(C)"
#file link -symbolic ./workspace/tec_control/src/test.c        ../../../src/tec_control/test.c
#file link -symbolic ./workspace/tec_control/src/mppc_therm_table.c        ../../../src/tec_control/mppc_therm_table.c
#file link -symbolic ./workspace/tec_control/src/pcb_therm_table.c        ../../../src/tec_control/pcb_therm_table.c
#file link -symbolic ./workspace/tec_control/src/fpga.h        ../../../src/fpga.h
#file link -symbolic ./workspace/tec_control/src/amc7823.h     ../../../src/amc7823/amc7823.h
#file link -symbolic ./workspace/tec_control/src/amc7823.c     ../../../src/amc7823/amc7823.c


app build all

