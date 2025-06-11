

updatemem \
    -meminfo /home/pedro/github/fpga_experiments/vitis2025/implement/results/top.mmi \
    -data /home/pedro/github/fpga_experiments/vitis2025/vitis/workspace/hello_world/build/hello_world.elf \
    -proc system_i/microblaze_0 \
    -bit /home/pedro/github/fpga_experiments/vitis2025/vitis/workspace/hello_world/_ide/bitstream/download.bit \
    -out /home/pedro/github/fpga_experiments/vitis2025/vitis/workspace/hello_world/_ide/bitstream/download.bit \
    -force


program_flash \
    -f /home/pedro/github/fpga_experiments/vitis2025/implement/results/top.bit \
    -offset 0 \
    -flash_type s25fl128sxxxxxx0-spi-x1_x2_x4 \
    -blank_check \
    -verify \
    -url TCP:127.0.0.1:3121

