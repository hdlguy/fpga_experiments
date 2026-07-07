#include "xparameters.h"
#include "xstatus.h"
#include "xintc_l.h"
#include "xintc.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xinterrupt_wrap.h"


#define INTC_BASEADDR		XPAR_XINTC_0_BASEADDR
// #define INTC_DEVICE_INTR_ID	0x0U
// #define INTC_DEVICE_INT_MASK	0x1U
#define INTC_DEVICE_INTR_ID		0x1U
#define INTC_DEVICE_INT_MASK	0x2U


int IntcLowLevelExample(u32 IntcBaseAddress);
void SetupInterruptSystem();
void DeviceDriverHandler(void *CallbackRef);

/*
 * Create a shared variable to be used by the main thread of processing and
 * the interrupt processing
 */
static volatile int InterruptProcessed = FALSE;
static XIntc_Config *CfgPtr;

#if defined(XPAR_SCUGIC)
XScuGic InterruptController;
static XScuGic_Config *GicConfig;
#endif

int main(void)
{
	int Status;

	/*
	 * Run the low level example of Interrupt Controller, specify the Base
	 * Address generated in xparameters.h.
	 */
	Status = IntcLowLevelExample(INTC_BASEADDR);
	if (Status != XST_SUCCESS) {
		xil_printf("Intc lowlevel Example Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran Intc lowlevel Example\r\n");
	return XST_SUCCESS;

}

/*****************************************************************************/
/**
*
* This function is an example of how to use the interrupt controller driver
* component (XIntc) and the hardware device.  This function is designed to
* work without any hardware devices to cause interrupts. It may not return
* if the interrupt controller is not properly connected to the processor in
* either software or hardware.
*
* This function relies on the fact that the interrupt controller hardware
* has come out of the reset state such that it will allow interrupts to be
* simulated by the software.
*
* @param	IntcBaseAddress is Base Address of the the Interrupt Controller
*		Device.
*
* @return	XST_SUCCESS to indicate success, otherwise XST_FAILURE.
*
* @note		None.
*
******************************************************************************/
int IntcLowLevelExample(u32 IntcBaseAddress)
{
	UINTPTR vector_base;
	u8 Id;

	xil_printf("\n\r*** running fit_intc_test ***\n\r");

	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device.
	 */
	XIntc_RegisterHandler(IntcBaseAddress, INTC_DEVICE_INTR_ID, (XInterruptHandler)DeviceDriverHandler, (void *)0);

	/*
	 * Enable interrupts for all devices that cause interrupts, and enable
	 * the INTC master enable bit.
	 */
	XIntc_EnableIntr(IntcBaseAddress, INTC_DEVICE_INT_MASK);


	/* Set the master enable bit. Note that we do not enable hardware
	 * interrupts yet since we want to simulate an interrupt from software
	 * down below. 	 */
	// XIntc_Out32(IntcBaseAddress + XIN_MER_OFFSET, XIN_INT_MASTER_ENABLE_MASK);
	XIntc_Out32(IntcBaseAddress + XIN_MER_OFFSET, XIN_INT_MASTER_ENABLE_MASK|XIN_INT_HARDWARE_ENABLE_MASK);




    vector_base = 0x10;


	CfgPtr = LookupConfigByBaseAddress(IntcBaseAddress);
	if (CfgPtr->VectorAddrWidth > XINTC_STANDARD_VECTOR_ADDRESS_WIDTH) {
		for (Id = 0; Id < 32 ; Id++) {
			XIntc_Out64(IntcBaseAddress + XIN_IVEAR_OFFSET + (Id * 8), vector_base);
		}
	} else {
		for (Id = 0; Id < 32 ; Id++) {
			XIntc_Out32(IntcBaseAddress + XIN_IVAR_OFFSET + (Id * 4), vector_base);
		}
	}


	/*
	 * This step is processor specific, connect the handler for the
	 * interrupt controller to the interrupt source for the processor.
	 */
	SetupInterruptSystem();

	// /* Cause (simulate) an interrupt so the handler will be called. This is
	//  * done by writing a 1 to the interrupt status bit for the device
	//  * interrupt. */
	// XIntc_Out32(IntcBaseAddress + XIN_ISR_OFFSET, INTC_DEVICE_INT_MASK);


	/*
	 * Wait for the interrupt to be processed, if the interrupt does not
	 * occur this loop will wait forever.
	 */
	uint32_t intcount=0;
	while (1) {
		/*
		 * If the interrupt occurred which is indicated by the global
		 * variable which is set in the device driver handler, then
		 * stop waiting.
		 */
		if (InterruptProcessed) {
			InterruptProcessed = 0;
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

	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();


	// * Register the interrupt controller handler with the exception table.
	if (XGet_IntcType(CfgPtr->IntrParent) == XINTC_TYPE_IS_INTC) {
		Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler)XIntc_DeviceInterruptHandler, (void *)INTC_BASEADDR);
	} else {
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
void DeviceDriverHandler(void *CallbackRef)
{
	(void)CallbackRef;
	InterruptProcessed = TRUE;
}
