#setup.s
.text
start:
.globl start
  call main
  
finish:
  li a0, 10
  ecall