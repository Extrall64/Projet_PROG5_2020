.global main
.text
a:
    add r2, r2, #1
    bal a1
b:
    add r3, r3, #1
    bal b1
c:
    add r4, r4, #1
    bal c1
d:
    add r5, r5, #1
    bal d1


main:
	@test passé si : les branchements A, B et C ne sont pas fait et D est fait
    mov r0, #0x7FFFFFFF
    mov r1, #0
    add r0, r0, #10
    beq a
a1:  
    bhs b
b1:
    bmi c
c1:
    bvc d
d1:

	@passé si le registre r1 atteint 0xFFFFFFFF sans rien modifier d'autre
    sub r1, r1, #1
   
    swi 0x123456
