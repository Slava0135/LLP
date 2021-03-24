#main.s
.text
main:
.globl main
  
  la a0, array_out       # <указатель выходного массива>
  la a1, array1          # <указатель массива 1>  
  la a2, array2          # <указатель массива 2>
  lw a3, array1_length   # <длина массива 1>
  lw a4, array2_length   # <длина массива 2>
  
  addi sp, sp, -16  
  sw ra, 12(sp)
  
  call join
  
  lw ra, 12(sp)
  addi sp, sp, 16
  
  li a0, 0
  ret
  
.rodata # секция неизменяемых данных
array1_length:
  .word 4
array2_length:
  .word 7
array1:
  .word 2, 5, 6, 8
array2:
  .word 0, 1, 3, 4, 7, 9, 10
  
.data # секция изменяемых данных
array_out:
  .word 255