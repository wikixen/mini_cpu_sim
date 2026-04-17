.section .text
.align 2
.globl setupGPIO

#include "GPIO.inc"

# Setup pins for use
setupGPIO:
  addi sp, sp, -16
  sw ra, 12(sp)

  li t0, GPIO_BASE_ADDR #Load GPIO base address into t0
  
