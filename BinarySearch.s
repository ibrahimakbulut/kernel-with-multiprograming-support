	.data


x: .word 110

list: .word 10, 20, 30, 50, 60, 80, 100, 110, 130, 170

str: .asciiz "\nAranilan Sayi Bulunamadi: -1\n"

str2: .asciiz "Aranilen Sayinin Indeksi: "


str4: .asciiz "\n"




	.text
	.globl main

main:

	la $t0, list
	li $t1, 0
	li $t2, 9


	lw $t3, x

	li $a0,-1
	li $a1,-1
	



loop2:

	sub $t4, $t2,$t1
	li $t5, 2
	div $t4,$t5
	mflo $t6	
	

	add $t6, $t6,$t1

	move $t4,$t6

	sll $t6,$t6,2
	add $t6,$t6,$t0
	lw $t6,0($t6)



	beq $t6,$t3,next

	addi $sp,$sp,-8
	sw $t3,4($sp)
	sw $t2,0($sp)
	
	slt $t2, $t6,$t3

	li $t3,1

	beq $t2,$t3,next2

	j else
	next2:

	lw $t3,4($sp)
	lw $t2,0($sp)
	addi $sp, $sp,8

	addi $t1,$t4,1

	j else2

	else:
	lw $t3,4($sp)
	lw $t2,0($sp)
	addi $sp, $sp,8

	addi $sp,$sp,-4

	sw $t5,0($sp)

	li $t5,-1

	
	else2:

	move $t6,$t4
	add $t2,$t6,$t5

	lw $t5, 0($sp)
	addi $sp,$sp,4
	
	
	slt $t7, $t2,$t1

	beq $t7,$zero, loop2

j next5


next:

	move $a0,$t4

next5:

	beq $a0,$a1,exit1
	j exit2



exit1:

	li $v0, 4
	la $a0, str4
	syscall

	li $v0, 4
	la $a0, str
	syscall

	li $v0, 4
	la $a0, str4
	syscall
	j end

exit2:

	
	move $t1,$a0

	li $v0, 4

	la $a0, str4
	syscall


	li $v0,4
	la $a0, str2
	syscall

	li $v0, 1
	move $a0, $t1
	syscall

	li $v0, 4

	la $a0, str4
	syscall

end:


li $v0,21
syscall

loop10:

j loop10
	
		

		









