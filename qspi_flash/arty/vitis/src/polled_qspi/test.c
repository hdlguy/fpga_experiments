
#include "xparameters.h"	/* XPAR parameters */
#include "xspi.h"		/* SPI device driver */
#include "xspi_l.h"
#include "xil_printf.h"

#define BUFFER_SIZE		12

typedef u8 DataBuffer[BUFFER_SIZE];

int SpiPolledExample(XSpi *SpiInstancePtr, UINTPTR BaseAddress);

static XSpi  SpiInstance;	 /* The instance of the SPI device */

u8 ReadBuffer[BUFFER_SIZE];
u8 WriteBuffer[BUFFER_SIZE];

int main(void)
{
	int Status;

	Status = SpiPolledExample(&SpiInstance, XPAR_XSPI_0_BASEADDR);

	if (Status != XST_SUCCESS) {
		xil_printf("Spi polled Example Failed\r\n");
		return XST_FAILURE;
	}

	xil_printf("Successfully ran Spi polled Example\r\n");
	return XST_SUCCESS;
}


int SpiPolledExample(XSpi *SpiInstancePtr, UINTPTR BaseAddress)
{
	int Status;
	u8 Test;
	XSpi_Config *ConfigPtr;	/* Pointer to Configuration data */


	ConfigPtr = XSpi_LookupConfig(BaseAddress);
	if (ConfigPtr == NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	Status = XSpi_CfgInitialize(SpiInstancePtr, ConfigPtr, ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		xil_printf("XSpi_CfgInitialize error\n\r");
		return XST_FAILURE;
	}

	Status = XSpi_SelfTest(SpiInstancePtr);
	if (Status != XST_SUCCESS) {
		xil_printf("XSpi_SelfTest error\n\r");
		return XST_FAILURE;
	}

	// if (SpiInstancePtr->SpiMode != XSP_STANDARD_MODE) {
	// 	return XST_SUCCESS;
	// }

	Status = XSpi_SetOptions(SpiInstancePtr, XSP_MASTER_OPTION);
	if (Status != XST_SUCCESS) {
		xil_printf("XSpi_SetOptions error\n\r");
		return XST_FAILURE;
	}


	XSpi_Start(SpiInstancePtr);

	// XSpi_IntrGlobalDisable(SpiInstancePtr);

	Test = 0x10;
	for (int Count = 0; Count < BUFFER_SIZE; Count++) {
		WriteBuffer[Count] = (u8)(Count + Test);
		ReadBuffer[Count] = 0;
	}


	XSpi_Transfer(SpiInstancePtr, WriteBuffer, ReadBuffer, BUFFER_SIZE);

	for (int Count = 0; Count < BUFFER_SIZE; Count++) {
		if (WriteBuffer[Count] != ReadBuffer[Count]) {
			xil_printf("XSpi_Transfer error\n\r");
			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}
