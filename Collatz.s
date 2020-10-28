	.data


str: .asciiz "---- Collatz Sequence : "

str2: .asciiz " , "

str3: .asciiz "\n"

	.text
	.globl main

main:

	li $t0,1

	li $t1,25

	li $t3,1

	li $t4,2

	loop1:


	move $t2,$t0

	li $v0,1
	move $a0, $t2
	syscall	

	li $v0,4
	la $a0,str
	syscall


		loop2:
			
			beq $t2,$t3,exit1

			div $t2, $t4
			mfhi $t5
			bne $t5, $zero,else

			mflo $t7

			move $t2,$t7

			j else2


			else:
			add $t6,$t2,$t2
			add $t6,$t2,$t6
			move $t2,$t6
			addi $t2,$t2,1

			else2:
			li $v0,1
			move $a0, $t2
			syscall

			li $v0,4
			la $a0, str2
			syscall

		
		j loop2

	exit1:

	li $v0,4
	la $a0,str3
	syscall

	beq $t0,$t1,exit2

	addi $t0,$t0,1

	j loop1

	exit2:

	li $v0,21
	syscall



	loop10:

	j loop10
	

