ORG 0000H              ; Dat dia chi bat dau chuong trinh tai 0000H
SJMP START             ; Nhay khong dieu kien den START

;------------------------------------------------------------
; Khoi dong he thong
;------------------------------------------------------------
START:
    MOV R1, #00H        ; Gan R1 = 0, dung de luu bien dem san pham

;------------------------------------------------------------
; Vong lap chinh
;------------------------------------------------------------
MAIN_LOOP:
    ACALL DISPLAY_TIME  ; Goi ham hien thi so dem len LED 7 doan
    ACALL CHECK_BUTTON  ; Goi ham kiem tra cam bien IR
    SJMP MAIN_LOOP      ; Lap lai vong lap chinh

;------------------------------------------------------------
; DISPLAY_TIME - Hien thi so dem ra 2 LED 7 doan (LED3 va LED4)
;------------------------------------------------------------
DISPLAY_TIME:
    MOV A, R1           ; Lay gia tri bien dem vao thanh ghi A
    MOV B, #10          ; B = 10 de chia lay hang chuc va hang don vi
    DIV AB              ; Chia A cho B ? A = hang chuc, B = hang don vi
    MOV R4, A           ; Luu hang chuc vao R4
    MOV R5, B           ; Luu hang don vi vao R5

    MOV P0, #0BH        ; Chon LED3 (hien thi hang chuc)
    MOV DPTR, #SEGMENT_CODE ; Tro DPTR den bang ma LED
    MOV A, R4           ; Lay gia tri hang chuc
    MOVC A, @A+DPTR     ; Lay ma LED tu bang ma
    MOV P1, A           ; Dua ma LED ra cong P1
    ACALL DELAY_2MS     ; Delay ngan de LED on dinh

    MOV P0, #07H        ; Chon LED4 (hien thi hang don vi)
    MOV DPTR, #SEGMENT_CODE ; Tro lai bang ma LED
    MOV A, R5           ; Lay gia tri hang don vi
    MOVC A, @A+DPTR     ; Lay ma LED tu bang
    MOV P1, A           ; Dua ma LED ra cong P1
    ACALL DELAY_2MS     ; Delay ngan de LED on dinh

    RET                 ; Ket thuc ham hien thi

;------------------------------------------------------------
; CHECK_BUTTON - Kiem tra cam bien IR (ket noi o P3.1)
;------------------------------------------------------------
CHECK_BUTTON:
    JB P3.1, SKIP_BTN   ; Neu P3.1 = 1 (khong co vat can) thi bo qua
    ACALL DELAY_20MS    ; Delay ngan chong doi tin hieu
    JB P3.1, SKIP_BTN   ; Kiem tra lai, neu P3.1 van = 1 thi bo qua

    INC R1              ; Tang bien dem len 1
    CJNE R1, #60, SKIP_WRAP ; Neu R1 != 60 thi khong lam gi
    MOV R1, #00         ; Neu R1 = 60 thi reset ve 0
SKIP_WRAP:
WAIT_RELEASE:
    JNB P3.1, WAIT_RELEASE ; Doi den khi cam bien khong bi che (P3.1 = 1)
SKIP_BTN:
    RET                 ; Ket thuc ham kiem tra cam bien

;------------------------------------------------------------
; DELAY_2MS - Delay ngan khoang 2ms
;------------------------------------------------------------
DELAY_2MS:
    MOV R6, #250        ; Gan R6 = 250
    DJNZ R6, $          ; Giam R6 den khi = 0
    RET                 ; Ket thuc delay

;------------------------------------------------------------
; DELAY_20MS - Delay dai hon ~20ms de chong doi
;------------------------------------------------------------
DELAY_20MS:
    MOV R6, #250        ; Vong lap ngoai
D20_LOOP1:
    MOV R7, #250        ; Vong lap trong
D20_LOOP2:
    DJNZ R7, D20_LOOP2  ; Lap den khi R7 = 0
    DJNZ R6, D20_LOOP1  ; Lap den khi R6 = 0
    RET                 ; Ket thuc delay

;------------------------------------------------------------
; SEGMENT_CODE - Bang ma LED 7 doan cho so 0 den 9
;------------------------------------------------------------
ORG 1000H              ; Dat bang ma tai dia chi 1000H
SEGMENT_CODE:
    DB 3FH              ; So 0
    DB 06H              ; So 1
    DB 5BH              ; So 2
    DB 4FH              ; So 3
    DB 66H              ; So 4
    DB 6DH              ; So 5
    DB 7DH              ; So 6
    DB 07H              ; So 7
    DB 7FH              ; So 8
    DB 6FH              ; So 9

END                    ; Ket thuc chuong trinh
