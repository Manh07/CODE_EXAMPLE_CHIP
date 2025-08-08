; 7seg.asm
; Countdown timer - 8051 Assembly

ORG 0000H           ; Dia chi bat dau chuong trinh
SJMP START_PROGRAM  ; Nhay toi START_PROGRAM

;-------------------------------------------------
; Dinh nghia bit
;-------------------------------------------------
RUNNING  BIT  32H   ; Bit danh dau dang chay (bit trong RAM)

;-------------------------------------------------
; Khoi tao ban dau
;-------------------------------------------------
START_PROGRAM:

MOV  R0, #01H       ; minutes = 1
MOV  R1, #00H       ; seconds = 0

MOV  30H, #01H      ; Luu gia tri khoi tao minutes vao dia chi 30H
MOV  31H, #00H      ; Luu gia tri khoi tao seconds vao dia chi 31H

CLR  RUNNING        ; Xoa bit RUNNING (dang dung)

;-------------------------------------------------
; Vong lap chinh
;-------------------------------------------------
MAIN_LOOP:
    ACALL CHECK_BUTTONS   ; Kiem tra phim bam

    JB   RUNNING, RUNNING_MODE ; Neu dang chay thi nhay toi RUNNING_MODE

NOT_RUNNING:
    ACALL DISPLAY_TIME    ; Hien thi thoi gian ra LED
    SJMP MAIN_LOOP        ; Quay lai MAIN_LOOP

RUNNING_MODE:
    MOV  R7, #255          ; Lap khoang gan 1 giay (255 lan)
DISPLAY_LOOP:
    ACALL DISPLAY_TIME     ; Hien thi thoi gian
    ACALL CHECK_BUTTONS    ; Kiem tra phim bam
    DJNZ R7, DISPLAY_LOOP  ; Giam R7, neu chua het thi quay lai

    ACALL COUNTDOWN        ; Giam thoi gian 1 giay
    SJMP MAIN_LOOP         ; Quay lai MAIN_LOOP

;-------------------------------------------------
; DISPLAY_TIME: Hien thi thoi gian ra LED 7 doan
;-------------------------------------------------
DISPLAY_TIME:
    ; digit[0] = minutes / 10
    MOV  A, R0
    MOV  B, #10
    DIV  AB
    MOV  R2, A             ; so hang chuc cua minutes

    ; digit[1] = minutes % 10
    MOV  R3, B             ; so don vi cua minutes

    ; digit[2] = seconds / 10
    MOV  A, R1
    MOV  B, #10
    DIV  AB
    MOV  R4, A             ; so hang chuc cua seconds

    ; digit[3] = seconds % 10
    MOV  R5, B             ; so don vi cua seconds

    ; Hien thi tung LED

    ; LED1 (hang chuc cua minutes)
    MOV  P0, #0EH          ; Chon LED1
    MOV  DPTR, #SEGMENT_CODE ; Tro toi bang ma LED
    MOV  A, R2
    MOVC A, @A+DPTR        ; Lay ma hien thi tu bang ma
    MOV  P1, A             ; Dua len P1
    ACALL DELAY_2MS        ; Delay nho de LED on dinh

    ; LED2 (hang don vi cua minutes, kem dau '.')
    MOV  P0, #0DH          ; Chon LED2
    MOV  DPTR, #SEGMENT_CODE
    MOV  A, R3
    MOVC A, @A+DPTR
    ORL  A, #080H          ; Them dau '.' vao
    MOV  P1, A
    ACALL DELAY_2MS

    ; LED3 (hang chuc cua seconds)
    MOV  P0, #0BH          ; Chon LED3
    MOV  DPTR, #SEGMENT_CODE
    MOV  A, R4
    MOVC A, @A+DPTR
    MOV  P1, A
    ACALL DELAY_2MS

    ; LED4 (hang don vi cua seconds)
    MOV  P0, #07H          ; Chon LED4
    MOV  DPTR, #SEGMENT_CODE
    MOV  A, R5
    MOVC A, @A+DPTR
    MOV  P1, A
    ACALL DELAY_2MS

    RET                    ; Quay lai

;-------------------------------------------------
; COUNTDOWN: Giam thoi gian
;-------------------------------------------------
COUNTDOWN:
    MOV  A, R1             ; Kiem tra seconds
    JZ   SEC_ZERO          ; Neu seconds = 0 thi xu ly giam minutes
    DEC  R1                ; Giam seconds
    RET

SEC_ZERO:
    MOV  A, R0             ; Kiem tra minutes
    JZ   TIME_UP           ; Neu minutes = 0 thi het gio
    DEC  R0                ; Giam minutes
    MOV  R1, #59           ; Reset seconds = 59
    RET

TIME_UP:
    CLR  RUNNING           ; Het gio, dung dem
    RET

;-------------------------------------------------
; CHECK_BUTTONS: Quet phim bam
;-------------------------------------------------
CHECK_BUTTONS:
    ; Nut START o P3.2
    JB   P3.2, SKIP_START  ; Neu chua bam thi bo qua
    ACALL DELAY_20MS       ; Chong doi phim
    JB   P3.2, SKIP_START
    SETB RUNNING           ; Bat bit RUNNING
WAIT_START_RELEASE:
    JNB  P3.2, WAIT_START_RELEASE ; Doi nha phim
SKIP_START:

    ; Nut RESET o P3.3
    JB   P3.3, SKIP_RESET
    ACALL DELAY_20MS
    JB   P3.3, SKIP_RESET
    MOV  R0, 30H           ; Reset minutes ve gia tri khoi tao
    MOV  R1, 31H           ; Reset seconds ve gia tri khoi tao
    CLR  RUNNING           ; Dung dem
WAIT_RESET_RELEASE:
    JNB  P3.3, WAIT_RESET_RELEASE
SKIP_RESET:

    ; Nut tang minutes o P3.0
    JB   P3.0, SKIP_INC_MIN
    ACALL DELAY_20MS
    JB   P3.0, SKIP_INC_MIN
    INC  R0                ; Tang minutes
    CJNE R0, #100, SKIP_INC_MIN_WRAP ; Gioi han 99
    MOV  R0, #00
SKIP_INC_MIN_WRAP:
WAIT_INC_MIN_RELEASE:
    JNB  P3.0, WAIT_INC_MIN_RELEASE
SKIP_INC_MIN:

    ; Nut tang seconds o P3.1
    JB   P3.1, SKIP_INC_SEC
    ACALL DELAY_20MS
    JB   P3.1, SKIP_INC_SEC
    INC  R1                ; Tang seconds
    CJNE R1, #60, SKIP_INC_SEC_WRAP  ; Gioi han 59
    MOV  R1, #00
SKIP_INC_SEC_WRAP:
WAIT_INC_SEC_RELEASE:
    JNB  P3.1, WAIT_INC_SEC_RELEASE
SKIP_INC_SEC:

    RET

;-------------------------------------------------
; DELAY nho
;-------------------------------------------------
DELAY_2MS:
    MOV  R6, #250          ; Delay ngan
    DJNZ R6, $             ; Lap cho den khi R6 = 0
    RET

DELAY_20MS:
    MOV  R6, #250          ; Delay dai hon
DELAY_20MS_LOOP1:
    MOV  R7, #250
DELAY_20MS_LOOP2:
    DJNZ R7, DELAY_20MS_LOOP2
    DJNZ R6, DELAY_20MS_LOOP1
    RET

;-------------------------------------------------
; Bang ma LED 7 doan
;-------------------------------------------------
ORG 1000H
SEGMENT_CODE:
    DB 3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH ; 0-9 ma hien thi

;-------------------------------------------------
END
