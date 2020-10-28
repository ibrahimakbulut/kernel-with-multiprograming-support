	.data

x:
	.word 175


str2: .asciiz "\n"

str3: .asciiz "-1\n"

str4: .asciiz "ooooooooooffffffffffffffffffff------------------------------------\n"
str5: .asciiz "Aranilan Sayinin Indeksi: "
str6: .asciiz "Aranilan Sayi Bulunamadi: "


list1: .word 10, 20, 30, 50, 60, 80, 100, 110, 170, 130
	
	.text
	.globl main

main:

	li $v0,4
	la $a0,str4


	lw $t0, x

	la $t1, list1
	li $t2, 9

	sll $t3,$t2,2
	add $t2,$t1,$t3

	li $t6, 0


loop2:	lw $t4, 0($t1) 
	beq $t0, $t4,exit2
	beq $t1,$t2,end
	addi $t1,$t1,4
	addi $t6,$t6,1
	j loop2

exit2:
	li $v0, 4
	la $a0, str2
	syscall

	la $a0, str5
	syscall

	li $v0, 1
	move $a0, $t6

	syscall

	li $v0, 4
	la $a0, str2
	syscall
	j all_end

end:

	li $v0, 4

	la $a0, str2
	syscall

	la $a0, str6
	syscall

	la $a0, str3
	syscall

all_end:


li $v0,21
syscall


loop10:

j loop10
	
	
