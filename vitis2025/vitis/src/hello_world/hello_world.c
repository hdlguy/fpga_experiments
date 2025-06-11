
#include <stdio.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"

int main()
{

    uint32_t* regptr = (uint32_t *) XPAR_REGFILE_CTRL_BASEADDR;

    print("Hello World\n\r");
    uint32_t whilecount=0;
    while(1) {
        xil_printf("%06u: FPGA_VERSION = 0x%08x, FPGA_ID = 0x%08x\n\r", whilecount, regptr[FPGA_VERSION], regptr[FPGA_ID]);
        regptr[FPGA_LED_CONTROL] = whilecount;
        for(int i=0; i<2000000; i++);
        whilecount++;
    }

    return(0);
    
}

