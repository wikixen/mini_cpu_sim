; Address Setup for GPIO addresses; Pins are decclared in pins.S
.equ GPIO_CTRL_ADDR, 0x10012000
.equ UART_CTRL_ADDR, 0x10013000
.equ MTIME, 0x0200BFF8
.equ MTIME_FREQUENCY, 33

.equ GPIO_OUTPUT_EN, 0x008 # Enable Output to selected pins
.equ GPIO_OUTPUT_VAL, 0x00C # Set Output Value
.equ GPIO_OUTPUT_XOR, 0x040 # inverse logic on selected pins
.equ GPIO_INPUT_EN, 0x04 # Set Input enable for selected pins
.equ GPIO_INPUT_VAL, 0x00 # Read Input value of selected pins
.equ GPIO_INPUT_PULUP, 0x10 # enable pull up

; Inputs
.equ PIN_2, 0x40000
.equ PIN_3, 0x80000
.equ PIN_4, 0x100000
.equ PIN_7, 0x800000 ;This pin is saved for UART if it's ever required for debugging

; Outputs
.equ PIN_9, 0x1
.equ PIN_10, 0x2
.equ PIN_11, 0x4
.equ PIN_12, 0x8
.equ PIN_13, 0x20
.equ PIN_15, 0x200
.equ PIN_16, 0x400
.equ PIN_17, 0x800

; I2C for LCD
.equ PIN_18, 0x1000
.equ PIN_19, 0x2000

; Assign pins to input & output
.equ INPUT, PIN_2 | PIN_3 | PIN_4 | PIN_7
.equ OUTPUT, PIN_9 | PIN_10 | PIN_11 | PIN_12 | PIN_13 | PIN_15 | PIN_16 | PIN_17 | PIN_18 | PIN_19