#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"


int main()
{

    xil_printf("Hello World\n\r");
    
    // uint32_t *regptr = (uint32_t *)XPAR_M00_AXI_BASEADDR;
    //uint32_t *regptr = (uint32_t *)XPAR_AXI_REGFILE_CTRL_BASEADDR;
    //xil_printf("regptr = %p\n\r", regptr);

    uint32_t *tpgptr = (uint32_t *)XPAR_V_TPG_0_BASEADDR;
    xil_printf("tpgptr = %p\n\r", tpgptr);

    uint32_t rval;
    for (int i=0; i<55; i++) {
        rval = tpgptr[i];
        xil_printf("reg[%02d] = 0x%08x\n\r", i, rval);    
    }
    
    uint32_t whilecount=0;
	while(1){

        //xil_printf("%08d: FPGA_ID = 0x%08x, FPGA_VERSION = 0x%08x\n\r", whilecount, regptr[FPGA_ID], regptr[FPGA_VERSION]);
        xil_printf("%08d: hello\n\r", whilecount);

        for(int i=0; i<100000000; i++);
        whilecount++;
    }
    
    return 0;
}

