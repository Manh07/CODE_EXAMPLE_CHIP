; M?c tiêu: Nh?p nháy dèn LED k?t n?i v?i chân P2.7 c?a 8051

; Khai báo h?ng s? (tùy ch?n, cho d? d?c)
LED_PIN EQU P2.7

; Khai báo các thanh ghi d?c bi?t (SFR)
DLY1MS EQU 3000 ; Giá tr? delay x?p x? 1ms (c?n di?u ch?nh theo t?n s? th?ch anh)

ORG 0000H       ; Ð?a ch? b?t d?u chuong trình

MAIN:
    MOV P2, #0FFH   ; Kh?i t?o Port 2 ? m?c cao (t?t LED n?u c?c duong chung)

LOOP:
    SETB LED_PIN    ; B?t LED (gi? s? LED n?i v?i P2.7 và c?c âm chung)
    LCALL DELAY     ; G?i hàm delay
    CLR LED_PIN     ; T?t LED
    LCALL DELAY     ; G?i hàm delay
    SJMP LOOP       ; Nh?y v? d?u vòng l?p

; Hàm delay (x?p x? 1 giây)
DELAY:
    MOV R7, #200    ; Vòng l?p ngoài (di?u ch?nh giá tr? này d? thay d?i th?i gian delay)
DELAY_OUTER:
    MOV R6, #250    ; Vòng l?p gi?a
DELAY_MIDDLE:
    MOV R5, #200    ; Vòng l?p trong
DELAY_INNER:
    DJNZ R5, DELAY_INNER
    DJNZ R6, DELAY_MIDDLE
    DJNZ R7, DELAY_OUTER
    RET             ; Tr? v? t? hàm delay

END             ; K?t thúc chuong trình