#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "xstatus.h"
// #include "xwdttb_l.h"
#include "xwdttb_hw.h"





int main()
{

    uint32_t* wdt_ptr = (uint32_t *) XPAR_XWDTTB_0_BASEADDR;

    xil_printf("\n\rHello World\n\r");

    wdt_ptr[XWT_TWCSR1_OFFSET/4] = 1; // enable the wdt 

    uint32_t whilecount=0;
    while(1) {

      uint32_t rval = wdt_ptr[XWT_TWCSR0_OFFSET];
      uint32_t reset_status = (rval >> 3) & 0x01;
      uint32_t timer_state  = (rval >> 2) & 0x01;

		  xil_printf("0x%08x: ", whilecount);
      xil_printf("reset_status = %d, timer_state = %d\n\r", reset_status, timer_state);

      wdt_ptr[XWT_TWCSR0_OFFSET] = 0x0c; // clear the reset status and timer state.

      // xil_printf("XWT_TWCSR0_OFFSET = 0x%08x\n\r", wdt_ptr[XWT_TWCSR0_OFFSET/4]);
      // xil_printf("XWT_TBR_OFFSET    = 0x%08x\n\r", wdt_ptr[XWT_TBR_OFFSET/4]);

    	for(int i=0; i<6000000; i++); // delay
    	whilecount++;

    }
    
    return 0;
}
