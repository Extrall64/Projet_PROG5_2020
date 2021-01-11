.global main
.text

main:
	@ test passed if at the end 
		@ CPSR will have the value #0xF0000010
		@ r2, r3 will have the value #0xFFFFFFFF ! signed bit extended to the 31th via LDRSB, LDRSH

	@ extend byte sign
	mov r0, #0
	mov r1, #0x000000FF
	str r1, [r0]
	ldrsb r2, [r0, #3]
	
	@ extend halfword sign
	mov r0, #0
	mov r1, #0x000000FF
	mov r2, #0x0000FF00
	orr r1, r1, r2
	str r1, [r0]
	ldrsh r3, [r0, #2]

	@ test msr
	msr CPSR_f, #0xF0000000

fin: swi 0x123456
