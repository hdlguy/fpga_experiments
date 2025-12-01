// this little Vitis app demonstrates basic operation of the AXI Watchdog Timer.
// The while(1) loop runs fast enough to prevent the WDT asserting reset on the CPU.
// Pressing one of the buttons on the board suspends the while loop so that a CPU 
// reset is triggered by the WDT.
#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "xstatus.h"
#include "xwdttb_hw.h"
#include "xgpio_l.h"

int main()
{
    uint32_t* wdt_ptr = (uint32_t *) XPAR_XWDTTB_0_BASEADDR;
    uint32_t* gpio_ptr = (uint32_t *) XPAR_XGPIO_0_BASEADDR;

    xil_printf("\n\rHello World\n\r");

    // enable the wdt 
    wdt_ptr[XWT_TWCSR1_OFFSET/4] = 1;

    uint32_t whilecount=0;
    while(1) {

        uint32_t rval = wdt_ptr[XWT_TWCSR0_OFFSET/4];
        uint32_t reset_status = (rval >> 3) & 0x01;
        uint32_t timer_state  = (rval >> 2) & 0x01;

        xil_printf("0x%08x: ", whilecount);
        xil_printf("reset_status = %d, timer_state = %d\n\r", reset_status, timer_state);

        wdt_ptr[XWT_TWCSR0_OFFSET/4] = 0x0c; // clear the reset status and timer state.
        gpio_ptr[XGPIO_DATA_OFFSET/4] = 0x00ff & whilecount;

        // xil_printf("XWT_TWCSR0_OFFSET = 0x%08x\n\r", wdt_ptr[XWT_TWCSR0_OFFSET/4]);
        // xil_printf("XWT_TBR_OFFSET    = 0x%08x\n\r", wdt_ptr[XWT_TBR_OFFSET/4]);

        while (gpio_ptr[XGPIO_DATA2_OFFSET/4] != 0) {}; // simulate a fault by waiting while a button is pressed

        for(int i=0; i<4000000; i++); // delay
        whilecount++;
    }
    
    return 0;
}
