.global main
.text

etq:
	mov r0, #0
	b suite

main:
	@ test passed if at the end thus registers will have the same values as its index
	mov r0, #0
	mov r1, #1
	mov r2, #2 
	mov r3, #3
	mov r4, #4 
	mov r5, #5
	mov r6, #6 
	mov r7, #7
	mov r8, #8

	@ test processing data instructions
	orr r0, r0, r7
	eor r0, r0, r7
	
	mvn r3, #0
	add r3, r3, #5
	sbc r3, r3, #0

	and r6, r6, r6
	
	add r0, r0, #1
	sub r0, r0, #1

	cmp r0, r1
	beq etq
	blt etq
	
	bic r5, r5, #1
	add r5, r5, #1
	
	mvn r8, #0
	asr r8, #4
	adds r8, r8, #1
	adc r8, r8, #0
	ror r8, r8, #31
	lsl r8, r8, #2


	rsbs r0, r7, r0
	bmi etq
	
suite:
	mov r1, r8, lsr #3
	teq r1, r1
	beq fin
	
	@ must be skiped
	tst r1, r1
	mov r0, #9
	
	bne fin
	
fin: swi 0x123456
