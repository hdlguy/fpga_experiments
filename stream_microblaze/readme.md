# Stream Microblaze
This mini-project demonstrates how to use a Microblaze processor to send and receive frames of streaming data.  The AXI4-Stream FIFO is instantiated within an IPI block diagram and connected to a Microblaze processor for control. External to the IPI diagram is a Verilog block datagen.sv that generates AXI4 Streaming frames at one per second to be received by the Microblaze. The Microblaze then transmits a frame in response. An ILA debug core is instantiated to observe the traffic.

A vitis --classic project fifo_test provides a polled C application for the Microblaze. 


