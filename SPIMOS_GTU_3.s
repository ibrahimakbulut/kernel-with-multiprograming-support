	.data



str: .asciiz "BinarySearch"
str2: .asciiz "LinearSearch"
str3: .asciiz "Collatz"

str7: .asciiz "HAHAHAHAHAHAHA"

	.text
	.globl main

main:

	li $v0,4
	la $a0,str
	syscall


	li $v0,22
	li $a0,7
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



	li $v0,0
	syscall

	move $t3,$a0



	loop:
		syscall
		bne $t3,$a0,exit5
		j loop

	exit5:

	move $t4,$a0



	bne $t3,$t0,tolinear

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

	j all_end


tolinear:

	bne $t3,$t1,tocollactz

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


all_end:



	bne $t4,$t0,tolinear2

	li $v0,19
	li $a0,4
	la $a1,str
	syscall

	li $a0,5
	la $a1,str
	syscall

	li $a0,6
	la $a1,str
	syscall

	j all_end2


tolinear2:

	bne $t4,$t1,tocollactz2

	li $v0,19
	li $a0,4
	la $a1,str2
	syscall

	li $a0,5
	la $a1,str2
	syscall

	li $a0,6
	la $a1,str2
	syscall

	j all_end2

tocollactz2:

	li $v0,19
	li $a0,4
	la $a1,str3
	syscall

	li $a0,5
	la $a1,str3
	syscall

	li $a0,6
	la $a1,str3
	syscall

all_end2:


#system call to wait_pid and infinite loop until wait_pid call shout down all program.

li $v0,20

loop2:

	syscall

j loop2


#this system call is never used because wait_pid system call is finishing program.

li $v0,10 
syscall


