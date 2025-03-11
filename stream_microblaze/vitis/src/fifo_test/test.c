
#include <stdio.h>
#include <stdlib.h>
#include "xil_printf.h"
#include "xparameters.h"
#include "fpga.h"

#include "xllfifo.h"
#include "xstatus.h"

XLlFifo FifoInstance;

#define MAXLEN 1500
uint32_t TxBuffer[MAXLEN/4];
uint32_t RxBuffer[MAXLEN/4];

int TxSend(XLlFifo *InstancePtr, u32 *SourceAddr);
int RxReceive(XLlFifo *InstancePtr, u32 *DestinationAddr);


int main()
{

    xil_printf("Hello\n\r");

    xil_printf("TxBuffer = %p\n\r", TxBuffer);
    xil_printf("RxBuffer = %p\n\r", RxBuffer);
 
	XLlFifo_Config *Config;
    XLlFifo* InstancePtr = &FifoInstance;   

	Config = XLlFfio_LookupConfig(XPAR_AXI_FIFO_0_DEVICE_ID);

	XLlFifo_CfgInitialize(InstancePtr, Config, Config->BaseAddress);

	XLlFifo_Status(InstancePtr);
	XLlFifo_IntClear(InstancePtr,0xffffffff);
	XLlFifo_Status(InstancePtr);



//	// Transmit the Data Stream
//	for (uint32_t i=0; i<MAXLEN/4; i++) TxBuffer[i] = i;
//	for (uint32_t i=0; i<MAXLEN/4; i++) XLlFifo_TxPutWord(InstancePtr, TxBuffer[i]);
//	xil_printf("fifo transmission\n\r");
//	XLlFifo_iTxSetLen(InstancePtr, MAXLEN);
//	xil_printf("polling\n\r");
//	while( !(XLlFifo_IsTxDone(InstancePtr)) );


	uint32_t RxLength;
    uint32_t whilecount=0;
    int Status;
    while(1) {

        xil_printf("0x%08x\n\r", whilecount);
        
        // receive a frame
    	while(XLlFifo_IsRxEmpty(InstancePtr)){};
		while(XLlFifo_iRxOccupancy(InstancePtr)) {
			RxLength = XLlFifo_iRxGetLen(InstancePtr);
			for (int i=0; i < RxLength/4; i++) RxBuffer[i] = XLlFifo_RxGetWord(InstancePtr);			
		}
		Status = XLlFifo_IsRxDone(InstancePtr);
		if(Status != TRUE){
			xil_printf("Failing in receive complete ... \r\n");
		} else {
			xil_printf("RxLength = %d\n\r", RxLength);
		}
//		XLlFifo_RxReset(InstancePtr);  // ???????????
		

    	// Transmit a frame
		for (uint32_t i=0; i<MAXLEN/4; i++) TxBuffer[i] = i + (whilecount<<16);
    	for (uint32_t i=0; i<MAXLEN/4; i++) XLlFifo_TxPutWord(InstancePtr, TxBuffer[i]);
    	XLlFifo_iTxSetLen(InstancePtr, MAXLEN);
    	while( !(XLlFifo_IsTxDone(InstancePtr)) );

    	//for(int i=0; i<1000000; i++); // delay
    	whilecount++;

    }
    
    return 0;
}


//Status = RxReceive(InstancePtr, DestinationBuffer);
//if (Status != XST_SUCCESS){
//	xil_printf("Receiving data failed");
//	return XST_FAILURE;
//}


///*****************************************************************************/
///**
//*
//* TxSend routine, It will send the requested amount of data at the
//* specified addr.
//*
//* @param	InstancePtr is a pointer to the instance of the
//*		XLlFifo component.
//*
//* @param	SourceAddr is the address where the FIFO stars writing
//*
//* @return
//*		-XST_SUCCESS to indicate success
//*		-XST_FAILURE to indicate failure
//*
//* @note		None
//*
//******************************************************************************/
//int TxSend(XLlFifo *InstancePtr, u32  *SourceAddr)
//{
//
//	int i;
//	int j;
//	xil_printf(" Transmitting Data ... \r\n");
//
//	/* Fill the transmit buffer with incremental pattern */
//	for (i=0;i<MAX_DATA_BUFFER_SIZE;i++) *(SourceAddr + i) = i;
//
//	for(i=0 ; i < NO_OF_PACKETS ; i++){
//
//		/* Writing into the FIFO Transmit Port Buffer */
//		for (j=0 ; j < MAX_PACKET_LEN ; j++){
//			if( XLlFifo_iTxVacancy(InstancePtr) ){
//				XLlFifo_TxPutWord(InstancePtr, *(SourceAddr+(i*MAX_PACKET_LEN)+j));
//			}
//		}
//
//	}
//
//	/* Start Transmission by writing transmission length into the TLR */
//	XLlFifo_iTxSetLen(InstancePtr, (MAX_DATA_BUFFER_SIZE * WORD_SIZE));
//
//	/* Check for Transmission completion */
//	while( !(XLlFifo_IsTxDone(InstancePtr)) );
//
//	/* Transmission Complete */
//	return XST_SUCCESS;
//}
//
//
//
//
///*****************************************************************************/
///**
//*
//* RxReceive routine.It will receive the data from the FIFO.
//*
//* @param	InstancePtr is a pointer to the instance of the
//*		XLlFifo instance.
//*
//* @param	DestinationAddr is the address where to copy the received data.
//*
//* @return
//*		-XST_SUCCESS to indicate success
//*		-XST_FAILURE to indicate failure
//*
//* @note		None
//*
//******************************************************************************/
//int RxReceive (XLlFifo *InstancePtr, u32* DestinationAddr)
//{
//
//	int i;
//	int Status;
//	u32 RxWord;
//	static u32 ReceiveLength;
//
//	xil_printf(" Receiving data ....\n\r");
//
//	while(XLlFifo_iRxOccupancy(InstancePtr)) {
//		/* Read Receive Length */
//		ReceiveLength = (XLlFifo_iRxGetLen(InstancePtr))/WORD_SIZE;
//		for (i=0; i < ReceiveLength; i++) {
//			RxWord = XLlFifo_RxGetWord(InstancePtr);
//			*(DestinationBuffer+i) = RxWord;
//		}
//	}
//
//	Status = XLlFifo_IsRxDone(InstancePtr);
//	if(Status != TRUE){
//		xil_printf("Failing in receive complete ... \r\n");
//		return XST_FAILURE;
//	}
//
//	return XST_SUCCESS;
//}
