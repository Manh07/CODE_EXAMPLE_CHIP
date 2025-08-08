;=================================
; giai ma ban phim 4x4
;================================

;=================================
; Reset Vector
org 0000h
jmp Start
ORG 0003H
LJMP INT0_ISR
;=================================
; CODE SEGMENT
;=================================
org 0100h
Start:
SETB EA ; Enable Interrupt
SETB EX0 ; Enable INT0
;=================================
; SETB IT0 ; INT0 NGAT THEO SUON XUONG
;=================================
MOV TMOD, #20H
MOV TH1, #0B0H
MOV P2, #0FH
MOV P1, #0
MOV R1, #0 ; COUNT
CLR A
SJMP $
;=================================
INT0_ISR:
CALL DELAY
JB P3.2, EXIT
MOV P2, #11111110B
MOV A, P2
ANL A, #0F0H
CJNE A, #0F0H, ROW0
MOV P2, #11111101B
MOV A, P2
MOV R1, #4
ANL A, #0F0H
CJNE A, #0F0H, ROW1
MOV P2, #11111011B
MOV A, P2
MOV R1, #8
ANL A, #0F0H
CJNE A, #0F0H, ROW2
MOV P2, #11110111B
MOV A, P2
MOV R1, #12
ANL A, #0F0H
CJNE A, #0F0H, ROW3
ROW0:
RLC A
JNC MATCH
INC R1
SJMP ROW0
ROW1:
RLC A
JNC MATCH
INC R1
SJMP ROW1
ROW2:
RLC A
JNC MATCH
INC R1
SJMP ROW2
ROW3:
RLC A
JNC MATCH
INC R1
SJMP ROW3
MATCH:
MOV A, #0
MOV A, R1
MOV P1, A
EXIT:
MOV R1, #0
MOV P2, #0F0H
RETI
;=================================
DELAY:
SETB TR1
JNB TF1, $
CLR TF1
CLR TR1
RET
;=================================
END