#!/bin/bash
# bash script to burn the configuration flash with the new C code in the bitfile.

## ./flashboard.sh  <name of app folder under src>
## Example: ./flashboard.sh hello_world

appname=$1

echo "appname = $appname"

results='../implement/results/'

# merge the elf file of the C application into the bitfile
updatemem \
    -meminfo $results/top.mmi \
    -data ./workspace/$appname/build/$appname.elf \
    -proc system_i/microblaze_0 \
    -bit $results/top.bit \
    -out ./download.bit \
    -force

# program the configuration flash
program_flash \
    -f ./download.bit \
    -offset 0 \
    -flash_type s25fl128sxxxxxx0-spi-x1_x2_x4 \
    -blank_check \
    -verify \
    -url TCP:127.0.0.1:3121

