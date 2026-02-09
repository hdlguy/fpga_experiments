#
bootgen -image bitstream.bif -arch zynq -o ./results/BOOT.bin -w

program_flash -fsbl ../zy_vitis/workspace/standalone_plat/zynq_fsbl/build/fsbl.elf -f ./results/BOOT.bin -flash_type qspi-x4-single -blank_check -verify



