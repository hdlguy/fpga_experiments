
#include <stdio.h>
#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"

int main()
{

    uint32_t* regptr = (uint32_t *) XPAR_REGFILE_CTRL_BASEADDR;

    uint32_t* ramptr = (uint32_t *) XPAR_AXI_BRAM_CTRL_0_BASEADDR;
    uint32_t ramsize = (uint32_t)XPAR_AXI_BRAM_CTRL_0_HIGHADDR - (uint32_t)XPAR_AXI_BRAM_CTRL_0_BASEADDR + 1;
    xil_printf("ramsize = 0x%08x\n\r", ramsize);
    xil_printf("XPAR_AXI_BRAM_CTRL_0_BASEADDR = 0x%08x\n\r", XPAR_AXI_BRAM_CTRL_0_BASEADDR);
    xil_printf("XPAR_AXI_BRAM_CTRL_0_HIGHADDR = 0x%08x\n\r", XPAR_AXI_BRAM_CTRL_0_HIGHADDR);
    

    xil_printf("Hello World\n\r");
    uint32_t whilecount=0;
    while(1) {

        xil_printf("%06u: FPGA_VERSION = 0x%08x, FPGA_ID = 0x%08x\n\r", whilecount, regptr[FPGA_VERSION], regptr[FPGA_ID]);
        regptr[FPGA_LED_CONTROL] = whilecount;

        // fill ram with random numbers
        srand(whilecount);
        for (uint32_t i=0; i<ramsize/4; i++){
            ramptr[i] = rand();
        }

        // check ram contents
        int errors = 0;
        srand(whilecount);
        for (uint32_t i=0; i<ramsize/4; i++){
            uint32_t rval = rand();
            if (rval != ramptr[i]) {
                errors++;
                //xil_printf("%08u: %08x  %08x\n\r", i, rval, ramptr[i]);
            }
        }
        xil_printf("ram errors = %d\n\r", errors);


        for(int i=0; i<2000000; i++);
        whilecount++;
    }

    return(0);
    
}

