; ============================
; Chuong trinh LCD 4-bit - hien thi chu chay ngang (scroll text)
; ============================

$mod51                 ; Khai bao chip 8051

U equ 31              ; Bien luu nibble cao
L equ 21              ; Bien luu nibble thap
Port equ P0           ; Ket noi LCD vao port 0
RS equ P2.0           ; Chan RS
RW equ P2.1           ; Chan RW
EN equ P2.2           ; Chan EN

; ============================
; Ket noi LCD:
; Port.4 = DB4
; Port.5 = DB5
; Port.6 = DB6
; Port.7 = DB7
; ============================

; Macro hien thi chuoi
SS MACRO L1
    MOV R1, #0
    MOV DPTR, L1
    LCALL lcd_puts
ENDM

; ============================
ORG 0000h

CLR RW
ACALL init           ; Khoi tao LCD

MAIN_LOOP:
    SS #STRING1      ; Hien thi dong 1
    MOV A, #0C0H     ; Di chuyen xuong dong 2
    ACALL lcd_cmd
    SS #STRING2      ; Hien thi dong 2

    MOV R5, #20      ; So buoc scroll
SCROLL:
    MOV A, #18H      ; Lenh dich trai
    ACALL lcd_cmd
    ACALL delay_scroll
    DJNZ R5, SCROLL

    MOV A, #01H      ; Xoa man hinh
    ACALL lcd_cmd
    ACALL delay
    SJMP MAIN_LOOP   ; Lap lai

; ============================
separator:
    MOV U, A
    ANL U, #0F0H
    SWAP A
    ANL A, #0F0H
    MOV L, A
    RET

; ============================
move_to_port:
    MOV C, Acc.4
    MOV port.4, C
    MOV C, Acc.5
    MOV port.5, C
    MOV C, Acc.6
    MOV port.6, C
    MOV C, Acc.7
    MOV port.7, C

    SETB EN
    ACALL delay
    CLR EN
    ACALL delay
    RET

; ============================
lcd_cmd:
    CLR RS
    ACALL separator
    MOV A, U
    ACALL move_to_port
    MOV A, L
    ACALL move_to_port
    RET

; ============================
lcd_data:
    SETB RS
    ACALL separator
    MOV A, U
    ACALL move_to_port
    MOV A, L
    ACALL move_to_port
    RET

; ============================
init:
    ACALL delay
    ACALL delay

    CLR port.4
    SETB port.5
    CLR port.6
    CLR port.7

    CLR RS
    SETB EN
    ACALL delay
    CLR EN

    MOV A, #28H
    ACALL lcd_cmd
    MOV A, #0CH
    ACALL lcd_cmd
    MOV A, #06H
    ACALL lcd_cmd
    MOV A, #01H
    ACALL lcd_cmd
    RET

; ============================
lcd_puts:
    MOV A, R1
    MOVC A, @A+DPTR
    LCALL lcd_data
    INC R1
    CJNE R1, #15, lcd_puts
    RET

; ============================
delay:
    MOV R6, #5FH
L2: MOV R7,#3FH
L1: DJNZ R7, L1
    DJNZ R6, L2
    RET

; ============================
; Delay dài hon dùng khi scroll
delay_scroll:
    MOV R6, #0FFH
DL2: MOV R7, #0FFH
DL1: DJNZ R7, DL1
     DJNZ R6, DL2
     RET

; ============================
STRING1: DB 'Ky thuat VXL    '
STRING2: DB 'Nhom 8          '

END
