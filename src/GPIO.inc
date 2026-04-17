; Address Setup for GPIO addresses; Pins are decclared in pins.S
.equ GPIO_BASE_ADDR, 0x10012000
.equ UART_CTRL_ADDR, 0x10013000
.equ MTIME, 0x0200BFF8
.equ MTIME_FREQUENCY, 33

.equ GPIO_OUTPUT_EN, 0x008 # Enable Output to selected pins
.equ GPIO_OUTPUT_VAL, 0x00C # Set Output Value
.equ GPIO_OUTPUT_XOR, 0x040 # inverse logic on selected pins
.equ GPIO_INPUT_EN, 0x04 # Set Input enable for selected pins
.equ GPIO_INPUT_VAL, 0x00 # Read Input value of selected pins
.equ GPIO_INPUT_PULUP, 0x10 # enable pull up

.equ PIN_2, 0x40000
.equ PIN_7, 0x800000
.equ PIN_8, 0x1
.equ PIN_9, 0x00002

.equ PIN_17, 0x800
.equ PIN_18, 0x1000
.equ PIN_19, 0x2000


.equ INPUT, PIN_2 | PIN_7 | PIN_8 | PIN_9
.equ OUTPUT, PIN_17 | PIN_18 | PIN_19