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


// This little program reads a xilinx .bit file and writes a xilinx .bin file.
// This is done by searching for the sync pattern then removing all the bytes 
// up to the beginning of the preamble.
//
int main(int argc, char * argv[])
{

    const uint32_t sync_pat = 0xAA995566;
    const int preamble_length = 48;

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
    struct stat file_info;
    stat(infile, &file_info);
    size_t streamsize = file_info.st_size;
    uint8_t *bitstream = (uint8_t *)malloc(streamsize);
    fp = fopen(infile, "rb");
    fread(bitstream, streamsize, 1, fp);
    fclose(fp);


    // find the sync pattern
    uint32_t search_pat;
    int sync_loc=-1;
    for (int i=0; i<streamsize-4; i++) {
        search_pat = (bitstream[i+0]<<24) | (bitstream[i+1]<<16) | (bitstream[i+2]<< 8) | (bitstream[i+3]<< 0);
        if (search_pat == sync_pat) {
            sync_loc = i;
        }
    }
    printf("sync location = %d\n", sync_loc);


    // check for validity
    if (sync_loc<preamble_length) {
        printf("sync pattern not found\n");
        return(1);
    } 


    // copy bitstream to binstream omitting header.
    size_t binsize = streamsize - sync_loc + preamble_length;
    uint8_t *binstream = (uint8_t *)malloc(binsize);
    for (int i=0; i<binsize; i++) {
        binstream[i] = bitstream[i+sync_loc-preamble_length];
    }


    // write bin file
    fp = fopen(outfile, "wb");
    fwrite(binstream, binsize, 1, fp);
    fclose(fp);

    return(0);

}

