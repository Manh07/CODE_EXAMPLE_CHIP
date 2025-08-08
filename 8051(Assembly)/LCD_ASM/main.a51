ORG 0000H

; Ð?nh nghia chân LCD
RS  EQU P2.0
RW  EQU P2.1
EN  EQU P2.2

; ========== DELAY ==========
DELAY:
    MOV R2, #250
D1: MOV R1, #250
D2: DJNZ R1, D2
    DJNZ R2, D1
    RET

; ======== G?i Nibble t?i LCD =========
SEND_HIGH_NIBBLE:
    ANL A, #0F0H         ; L?y nibble cao
    ORL P0, A            ; G?i nibble cao
    ACALL PULSE_ENABLE
    RET

SEND_LOW_NIBBLE:
    SWAP A               ; Ð?i nibble th?p lên cao
    ANL A, #0F0H
    ORL P0, A
    ACALL PULSE_ENABLE
    RET

; ====== Kích xung EN =======
PULSE_ENABLE:
    SETB EN
    ACALL DELAY
    CLR EN
    ACALL DELAY
    RET

; ====== G?i L?nh LCD =======
LCD_CMD:
    CLR RS
    CLR RW
    MOV C, ACC.7         ; Luu A t?m th?i
    ACALL SEND_HIGH_NIBBLE
    MOV A, C
    ACALL SEND_LOW_NIBBLE
    RET

; ====== G?i D? li?u LCD =====
LCD_DATA:
    SETB RS
    CLR RW
    MOV C, ACC.7
    ACALL SEND_HIGH_NIBBLE
    MOV A, C
    ACALL SEND_LOW_NIBBLE
    RET

; ====== Kh?i t?o LCD ======
LCD_INIT:
    MOV A, #03H
    ACALL SEND_HIGH_NIBBLE
    ACALL DELAY
    ACALL DELAY

    MOV A, #02H         ; Ch? d? 4-bit
    ACALL SEND_HIGH_NIBBLE
    ACALL DELAY

    MOV A, #28H         ; 2 dòng, font 5x8
    ACALL LCD_CMD
