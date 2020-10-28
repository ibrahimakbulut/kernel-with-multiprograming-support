	.data



str: .asciiz "BinarySearch"
str2: .asciiz "LinearSearch"
str3: .asciiz "Collatz"


	.text
	.globl main

main:

	li $v0,22
	li $a0,11
	syscall

	li $v0,0
	syscall

	li $t0,0
	li $t1,1
	li $t2,2

	li $v0,18

	syscall
	syscall
	syscall
	syscall
	syscall
	syscall
	syscall
	syscall
	syscall
	syscall

	bne $a0,$t0,tolinear

	li $v0,19
	li $a0,1
	la $a1,str
	syscall

	li $a0,2
	la $a1,str
	syscall

	li $a0,3
	la $a1,str
	syscall

	li $a0,4
	la $a1,str
	syscall

	li $a0,5
	la $a1,str
	syscall

	li $a0,6
	la $a1,str
	syscall

	li $a0,7
	la $a1,str
	syscall

	li $a0,8
	la $a1,str
	syscall

	li $a0,9
	la $a1,str
	syscall

	li $a0,10
	la $a1,str
	syscall

	j all_end



tolinear:
	
	bne $a0,$t1,tocollactz

	li $v0,19
	li $a0,1
	la $a1,str2
	syscall

	li $a0,2
	la $a1,str2
	syscall

	li $a0,3
	la $a1,str2
	syscall

	li $a0,4
	la $a1,str2
	syscall

	li $a0,5
	la $a1,str2
	syscall

	li $a0,6
	la $a1,str2
	syscall


	li $a0,7
	la $a1,str2
	syscall

	li $a0,8
	la $a1,str2
	syscall

	li $a0,9
	la $a1,str2
	syscall

	li $a0,10
	la $a1,str2
	syscall


	j all_end




tocollactz:


	li $v0,19
	li $a0,1
	la $a1,str3
	syscall

	li $a0,2
	la $a1,str3
	syscall

	li $a0,3
	la $a1,str3
	syscall

	li $a0,4
	la $a1,str3
	syscall

	li $a0,5
	la $a1,str3
	syscall

	li $a0,6
	la $a1,str3
	syscall

	li $a0,7
	la $a1,str3
	syscall

	li $a0,8
	la $a1,str3
	syscall

	li $a0,9
	la $a1,str3
	syscall


	li $a0,10
	la $a1,str3
	syscall


all_end:


#system call to wait_pid and infinite loop until wait_pid call shout down all program.
li $v0,20
loop:

	syscall

j loop


#this system call is never used because wait_pid system call is finishing program.
li $v0,10 
syscall
