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
    char *infile, *outfile;
    if (argc != 3) {
        printf("USAGE: ./bit2bin <input filename> <output filename>\n");
        printf("Example: ./bit2bin bitfile.bit binfile.bin\n");
        return(1);
    } else {
        infile  = argv[1] ;
        outfile = argv[2] ;
    }
    
    // read the bit file
    FILE *fp;
    printf("opening %s\n", infile);
    struct stat file_info;
    if (stat(infile, &file_info) != 0) { printf("ERROR: problem opening %s\n", infile); return(1); }   
    size_t streamsize = file_info.st_size;
    printf("%s file size = %lu\n", infile, (unsigned long)streamsize);
    fp = fopen(infile, "rb");
    if (fp == NULL) { printf("ERROR: could not open %s\n", infile); return(1); }
    uint8_t *bitstream = (uint8_t *)malloc(streamsize);
    if (bitstream == NULL) { printf("ERROR: malloc failure\n"); return(1); }   
    fread(bitstream, streamsize, 1, fp);
    fclose(fp);


    // copy bitstream to binstream omitting header.
    const int header_length = 119;
    uint8_t *binstream = (uint8_t *)malloc(streamsize);
    for (int i=header_length; i<streamsize; i++) {
        binstream[i-header_length] = bitstream[i];
    }



    fp = fopen(outfile, "wb");
    fwrite(binstream, streamsize-header_length, 1, fp);
    fclose(fp);

    return(0);

}

/*

*/
