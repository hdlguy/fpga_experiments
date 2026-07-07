#include "xparameters.h"
#include "xstatus.h"
#include "xintc_l.h"
#include "xintc.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xinterrupt_wrap.h"
#include "fpga.h"


#define INTC_BASEADDR		XPAR_XINTC_0_BASEADDR
// interrupt 1 is the FIT
#define FIT_INTR_ID		0x1U
#define FIT_INT_MASK	0x2U
// interrupt 0 is the uart
#define UART_INTR_ID 			XPAR_FABRIC_AXI_UARTLITE_0_INTR
#define UART_INT_MASK			0x01U


void SetupInterruptSystem();
void FitIntrHandler(void *CallbackRef);
void UartIntrHandler(void *CallbackRef);

// Create a shared variable to be used by the main thread of processing and the interrupt processing
static volatile int InterruptProcessed = FALSE;
static XIntc_Config *CfgPtr;


int main(void)
{
	UINTPTR vector_base;
	u32 IntcBaseAddress = INTC_BASEADDR;
	uint32_t *intcptr = (uint32_t *)INTC_BASEADDR;
	uint32_t *regptr  = (uint32_t *)XPAR_REGFILE_CTRL_BASEADDR;

	xil_printf("\n\r*** running fit_intc_test ***\n\r");
	xil_printf("FPGA_ID = 0x%08x, FPGA_VERSION = 0x%08x\n\r", regptr[FPGA_ID], regptr[FPGA_VERSION]);

	// Connect a device driver handler 
	XIntc_RegisterHandler(IntcBaseAddress, FIT_INTR_ID, (XInterruptHandler)FitIntrHandler, (void *)0);
	XIntc_RegisterHandler(IntcBaseAddress, UART_INTR_ID, (XInterruptHandler)UartIntrHandler, (void *)0);

	// Enable interrupts for all devices that cause interrupts, and enable the INTC master enable bit.
	XIntc_EnableIntr(IntcBaseAddress, FIT_INT_MASK);

	// Set the master enable bit and enable hardware interrupts
	XIntc_Out32(IntcBaseAddress + XIN_MER_OFFSET, XIN_INT_MASTER_ENABLE_MASK|XIN_INT_HARDWARE_ENABLE_MASK);

    vector_base = 0x10;
	//vector_base = XIL_EXCEPTION_ID_INT;

	// write the interrupt vector table, all entries point to vector_base.
	CfgPtr = LookupConfigByBaseAddress(IntcBaseAddress);
	for (u8 Id = 0; Id < 32 ; Id++) {
		intcptr[XIN_IVAR_OFFSET/4+Id] = vector_base;
	}

	/* This step is processor specific, connect the handler for the interrupt controller to the interrupt source for the processor.*/
	SetupInterruptSystem();

	uint32_t intcount=0;
	while (1) {

		if (InterruptProcessed) {
			InterruptProcessed = FALSE;
			xil_printf("intcount = %u\n\r", intcount);
			intcount++;
		}
	}

	return XST_SUCCESS;

}
/*****************************************************************************/
/**
*
* This function connects the interrupt handler of the interrupt controller to
* the processor. This function is separate to allow it to be customized for
* each application. Each processor or RTOS may require unique processing to
* connect the interrupt handler.
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void SetupInterruptSystem()
{

	// Initialize the exception table.
	Xil_ExceptionInit();

	// Register the interrupt controller handler with the exception table.
	if (XGet_IntcType(CfgPtr->IntrParent) == XINTC_TYPE_IS_INTC) {
		Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_DeviceInterruptHandler, (void *)INTC_BASEADDR);
	}

	Xil_ExceptionEnable();
}



/*****************************************************************************/
/**
*
* This function is designed to look like an interrupt handler in a device
* driver. This is typically a 2nd level handler that is called from the
* interrupt controller interrupt handler.  This handler would typically
* perform device specific processing such as reading and writing the registers
* of the device to clear the interrupt condition and pass any data to an
* application using the device driver.
*
* @param	CallbackRef is passed back to the device driver's interrupt
*		handler by the XIntc driver.  It was given to the XIntc driver
*		in the XIntc_Connect() function call. It is typically a pointer
*		to the device driver instance variable if using the Xilinx Level
*		1 device drivers. In this example, we do not care about the
*		callback reference, so we passed it a 0 when connecting the
*		handler to the XIntc driver and we make no use of it here.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void FitIntrHandler(void *CallbackRef)
{
	(void)CallbackRef;
	InterruptProcessed = TRUE;

	uint32_t *regptr = (uint32_t *)XPAR_REGFILE_CTRL_BASEADDR;
	regptr[FPGA_LED_CONTROL]++;
}

void UartIntrHandler(void *CallbackRef)
{
	(void)CallbackRef;
}