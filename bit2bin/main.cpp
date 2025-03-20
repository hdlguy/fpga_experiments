// This app is used to test flash interface for field update
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include "fpga.h"

#include <sys/stat.h>

#define PORT    1234

#define DATA_SIZE       1024
#define BUF_SIZE        (DATA_SIZE+4+4)

#define FLASH_OP_WRITE  1
#define FLASH_OP_READ   2
#define FLASH_OP_ERASE  3
#define FLASH_OP_ECHO   4
#define FLASH_OP_REBOOT 5


int main(int argc, char * argv[])
{

    // get command line arguments
    char *fname;
    if (argc != 2) {
        printf("USAGE: ./test <filename.bit>\n");
        printf("Example: ./test ../../implement_arty/results/top2.bit\n");
        return(1);
    } else {
        fname = argv[1] ;
    }
    
    // read the bit file
    printf("opening %s\n", fname);
    struct stat file_info;
    if (stat(fname, &file_info) != 0) { printf("ERROR: problem opening %s\n", fname); return(1); }   
    size_t streamsize = file_info.st_size;
    printf("FILE SIZE: %lu\n", (unsigned long)streamsize);
    FILE *fp = fopen(fname, "rb");
    if (fp == NULL) { printf("ERROR: could not open %s\n", fname); return(1); }
    uint8_t *bitstream = (uint8_t *)malloc(streamsize);
    if (bitstream == NULL) { printf("ERROR: malloc failure\n"); return(1); }   
    fread(bitstream, streamsize, 1, fp);
    fclose(fp);


    const int MAXLINE = 2048;
    uint8_t rxbuf[MAXLINE];
    ssize_t rxlength;

    const int OneKB = 1024;
    const int SectorSize = 64*OneKB;
    const int RegionStart = 0x400000;
    //const int RegionStart = 0x000000;
    const int RegionSize = 0x3F0000;

    int stream_1KB = (int)(ceil((double)streamsize/(double)OneKB));

    // UDP Socket Tx
    int clientSocket;
    uint8_t txbuf[BUF_SIZE];
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("16.0.0.128");
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));  
    addr_size = sizeof(serverAddr); 

    // UDP Socket Rx
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    } else {
        printf("socket creation passed\n");
    }
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family    = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    socklen_t len;
    len = sizeof(cliaddr);

    ssize_t nBytes;
    uint32_t flash_address;
    uint8_t flash_op;
    uint32_t errors;

    // *********** read and print Version and ID codes
    txbuf[0] = 0xaa; txbuf[1] = 0xbb; txbuf[2] = 0xcc; txbuf[3] = UDP_STAT_REQ;
    nBytes = 4;
    sendto(clientSocket, txbuf, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size); 
    rxlength = recvfrom(sockfd, (char *)rxbuf, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
    uint32_t fpga_version = ((uint32_t *)rxbuf)[1];
    uint32_t fpga_id      = ((uint32_t *)rxbuf)[2];
    printf("fpga_id = 0x%08x, fpga_version = 0x%08x\n", fpga_id, fpga_version);


    // first four bytes of DMT header with UDP_FLASH destination code
    txbuf[0] = 0xaa; txbuf[1] = 0xbb; txbuf[2] = 0xcc; txbuf[3] = UDP_FLASH;


    // ************ Erase Sectors
    printf("ERASE\n");
    for (int sector=0; sector<(RegionSize/SectorSize); sector++){

        nBytes = 8;
        flash_address = RegionStart + SectorSize*sector;
        flash_op = FLASH_OP_ERASE;

        ((uint32_t *)txbuf)[1] = (flash_address&0xffffff00) | (flash_op);
        sendto(clientSocket, txbuf, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size); 

        //usleep(1000);

        rxlength = recvfrom(sockfd, (char *)rxbuf, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        if(rxlength<0) { printf("error in reading recvfrom function\n"); } 

        printf("ERASE: address = 0x%08x\r", flash_address);

    }
    printf("ERASE: address = 0x%08x\n", flash_address);



    // *********** blank check the flash from 0x400000 to 0x7effff, 1KB at a time.
    printf("BLANK_CHECK\n");
    errors = 0;
    for (int i=0; i<(RegionSize/OneKB); i++) {


        nBytes = 8;
        flash_address = RegionStart + OneKB*i;
        flash_op = FLASH_OP_READ;

        // send READ packet
        ((uint32_t *)txbuf)[1] = (flash_address&0xffffff00) | (flash_op);
        sendto(clientSocket, txbuf, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size); 

        // receive READ response packet
        rxlength = recvfrom(sockfd, (char *)rxbuf, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        if(rxlength<0) { printf("error in reading recvfrom function\n"); }

        for (int i=8; i<rxlength; i++) {
            if (rxbuf[i] != 0xff) { errors++; }
        }

        printf("BLANK_CHECK: address = 0x%08x, errors = %d\r", flash_address, errors);

    }
    printf("BLANK_CHECK: address = 0x%08x, errors = %d\n", flash_address, errors);




    // *********** write the configuration data to flash
    printf("WRITE\n");
    for (int i=0; i<stream_1KB; i++) {

        nBytes = BUF_SIZE;
        flash_address = RegionStart + OneKB*i;
        flash_op = FLASH_OP_WRITE;

        // fill the the tx buffer
        ((uint32_t *)txbuf)[1] = (flash_address&0xffffff00) | (flash_op);
        for (int j=0; j<OneKB; j++) {
            if ((OneKB*i+j) < streamsize){
                txbuf[j+4+4] = bitstream[j+OneKB*i];
            } else {
                txbuf[j+4+4] = 0xff;
            }
        }

        // send WRITE packet
        ((uint32_t *)txbuf)[1] = (flash_address&0xffffff00) | (flash_op);
        sendto(clientSocket, txbuf, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size); 

        //usleep(1000);

        // receive WRITE response packet
        rxlength = recvfrom(sockfd, (char *)rxbuf, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        if(rxlength<0) { printf("error in reading recvfrom function\n"); }

        printf("WRITE: address = 0x%08x\r", flash_address);

    }
    printf("WRITE: address = 0x%08x\n", flash_address);




    // *********** read and verify flash contents
    printf("VERIFY\n");
    errors = 0;
    uint8_t compare_buf[OneKB];
    for (int i=0; i<stream_1KB; i++) {


        nBytes = 8;
        flash_address = RegionStart + OneKB*i;
        flash_op = FLASH_OP_READ;

        // send READ packet
        ((uint32_t *)txbuf)[1] = (flash_address&0xffffff00) | (flash_op);
        sendto(clientSocket, txbuf, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size); 

        // receive READ response packet
        rxlength = recvfrom(sockfd, (char *)rxbuf, MAXLINE, MSG_WAITALL, ( struct sockaddr *) &cliaddr, &len);
        if(rxlength<0) { printf("error in reading recvfrom function\n"); }

        // verify data
        for (int j=0; j<OneKB; j++) {
            if ((OneKB*i+j) < streamsize){
                compare_buf[j] = bitstream[j+OneKB*i];
            } else {
                compare_buf[j] = 0xff;
            }
        }
        for (int i=8; i<rxlength; i++) {
            if (rxbuf[i] != compare_buf[i-8]) { errors++; }
        }

        printf("VERIFY: address = 0x%08x, errors = %d\r", flash_address, errors);

    }
    printf("VERIFY: address = 0x%08x, errors = %d\n", flash_address, errors);




    // *********** reboot fpga
    nBytes = 16;//BUF_SIZE;
    flash_address = RegionStart;
    flash_op = FLASH_OP_REBOOT;

    printf("REBOOT\n");
    ((uint32_t *)txbuf)[1] = (flash_address&0xffffff00) | (flash_op);
    sendto(clientSocket, txbuf, nBytes, 0, (struct sockaddr *)&serverAddr, addr_size); 

    return 0;
}

/*

*/
