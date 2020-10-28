	.data





str: .asciiz "BinarySearch"
str2: .asciiz "LinearSearch"
str3: .asciiz "Collatz"
str4: .asciiz "\nyyyyyyyyyy\n"
str5: .asciiz "\nPPPPPPPPPPPPPPPPPPPP\n"

str8: .asciiz "\nSSSSSSSSSSSSSSSSSSSSSSSSSSS\n"

str7: .asciiz "HAHAHAHAHAHAHA"

	.text
	.globl main

main:



	li $v0,22
	li $a0,4

	syscall


	li $v0,18
	syscall


	li $v0,18
	syscall

	li $v0,18
	syscall




	li $v0,19
	li $a0,1
	la $a1,str

	syscall

	li $v0,19
	li $a0,2
	la $a1,str2

	syscall



	li $v0,19
	li $a0,3
	la $a1,str3

	syscall



#system call to wait_pid and infinite loop until wait_pid call shout down all program.
	li $v0,20

	loop:

		syscall

	j loop


#this system call is never used because wait_pid system call is finishing program.
li $v0,10 
syscall
