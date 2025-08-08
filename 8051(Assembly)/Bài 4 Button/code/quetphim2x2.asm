  ;================================= 
  ; Mot phim bat tat mot led 
  ;================================= 

 
 
 			



ORG 00H
MAIN:

JNB P1.4,PHIM1
JMP MAIN
PHIM1:
CLR P2.0
acall delay_500ms
SETB P2.0
RET
delay_500ms:
MOV 50H,#200
l1: MOV 51H,#250
DJNZ 51H,$
DJNZ 50H,l1
RET
END