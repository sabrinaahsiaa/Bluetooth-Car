# Bluetooth-Car

This repository contains resources related to the Bluetooth Car project. The Bluetooth Car project demonstrates a robot car driven by 2 DC motors that will be controlled through communications with a Bluetooth module.
Collaborated with Bryce Cheung

# Development Tools

* Keil uVision5 for tm4c123gh6pm microcontroller
* Tera Term for terminal emulator

# Contents

* `SetupBLT.c` : Main function to configure HC-05 Bluetooth module
* `UART0` : Initialization for UART 0
* `BLT` : Initialization for UART 1
* `GPIO` : Initialization for on-board LEDs
* `PWM` : Initialization for PWM 0
* `HW_PWM_Car` : Initialization for UART 1 communication between microcontroller and Bluetooth module
