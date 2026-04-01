#  iic_mag_test
A project to access the MemSic MMC3416xPJ magnetometer over I2C.

A Microblaze design running on the Arty-35t board is used. A loop executes once per second which reads the Memsic and prints the results to the usb uart console.  The device ID reads correctly but the compass readings are not correct.

