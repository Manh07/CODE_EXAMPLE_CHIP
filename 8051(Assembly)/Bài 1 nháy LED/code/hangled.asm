 led_data equ p2
	org 00h
	main : 
	mov a,#0x00
	setb c
	loop :
	rlc a
	cpl a
	mov led_data,a
	cpl a
	lcall delay_500ms
	sjmp loop

	delay_500ms	:
	MOV 50H,#200
ll: MOV 51H,#250
	DJNZ 51H,$
	DJNZ 50H,ll

	RET
	end
  
