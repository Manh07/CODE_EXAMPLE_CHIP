; M?c ti�u: Nh?p nh�y d�n LED k?t n?i v?i ch�n P2.7 c?a 8051

; Khai b�o h?ng s? (t�y ch?n, cho d? d?c)
LED_PIN EQU P2.7

; Khai b�o c�c thanh ghi d?c bi?t (SFR)
DLY1MS EQU 3000 ; Gi� tr? delay x?p x? 1ms (c?n di?u ch?nh theo t?n s? th?ch anh)

ORG 0000H       ; �?a ch? b?t d?u chuong tr�nh

MAIN:
    MOV P2, #0FFH   ; Kh?i t?o Port 2 ? m?c cao (t?t LED n?u c?c duong chung)

LOOP:
    SETB LED_PIN    ; B?t LED (gi? s? LED n?i v?i P2.7 v� c?c �m chung)
    LCALL DELAY     ; G?i h�m delay
    CLR LED_PIN     ; T?t LED
    LCALL DELAY     ; G?i h�m delay
    SJMP LOOP       ; Nh?y v? d?u v�ng l?p

; H�m delay (x?p x? 1 gi�y)
DELAY:
    MOV R7, #200    ; V�ng l?p ngo�i (di?u ch?nh gi� tr? n�y d? thay d?i th?i gian delay)
DELAY_OUTER:
    MOV R6, #250    ; V�ng l?p gi?a
DELAY_MIDDLE:
    MOV R5, #200    ; V�ng l?p trong
DELAY_INNER:
    DJNZ R5, DELAY_INNER
    DJNZ R6, DELAY_MIDDLE
    DJNZ R7, DELAY_OUTER
    RET             ; Tr? v? t? h�m delay

END             ; K?t th�c chuong tr�nh