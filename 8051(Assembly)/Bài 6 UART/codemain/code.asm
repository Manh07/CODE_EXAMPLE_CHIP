;=================================
; G.i 1 byte t. máy tính xu.ng vi ði.u khi.n, vi ði.u khi.n ð.c r.i g.i l.i byte týõng t. lên máy

;=================================
org 000h
ljmp begin
org 23h
ljmp serial_IT
;=================================
;/**
; * FUNCTION_PURPOSE: This file set up uart in mode 1 (8 bits uart) with
; * timer 1 in mode 2 (8 bits auto reload timer).
; * FUNCTION_INPUTS: void
; * FUNCTION_OUTPUTS: void
; */
;=================================
org 0100h
begin:
MOV SCON, #50h ; /* uart in mode 1 (8 bit), REN=1 */
ORL TMOD, #20h ; /* Timer 1 in mode 2 */
MOV TH1, #0F9h ; /* 9600 Bds at 11.059MHz */
MOV TL1, #0F9h ; /* 9600 Bds at 11.059MHz */
MOV A,PCON
SETB ACC.7
MOV PCON,A
SETB ES ; /* Enable serial interrupt*/
SETB EA ; /* Enable global interrupt */
SETB TR1 ; /* Timer 1 run */
JMP $ ; /* endless */
;/**
; * FUNCTION_PURPOSE: serial interrupt, echo received data.
; * FUNCTION_INPUTS: P3.0(RXD) serial input
; * FUNCTION_OUTPUTS: P3.1(TXD) serial output
; */
serial_IT:
JNB RI,EMIT_IT ; test if it is a reception
CLR RI ; clear reception flag for next reception
MOV A,SBUF ; read data from uart
MOV SBUF,A ; write same data to uart
LJMP END_IT
EMIT_IT:
CLR TI ; clear transmition flag for next transmition
END_IT:
RETI
end