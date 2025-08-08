ORG 0000H        ; Bắt đầu chương trình từ địa chỉ 0000H
SJMP START_PROGRAM ; Nhảy đến nhãn START_PROGRAM để bắt đầu thực thi chương trình chính

;-------------------------------------------------
; Start Program
;-------------------------------------------------
START_PROGRAM:
    ; Initialize traffic light pins (P3.0–P3.5)
    MOV P3, #00H        ; Thiết lập tất cả các chân của Port 3 ở mức thấp (tắt tất cả đèn giao thông ban đầu)

;-------------------------------------------------
; Main Loop
;-------------------------------------------------
MAIN_LOOP:
    ; Phase 1: NS Green (11s), EW Red (15s)
    MOV R0, #11         ; Gán giá trị 11 (giây) cho thanh ghi R0 để đếm ngược thời gian đèn xanh hướng Bắc-Nam
    MOV R1, #15         ; Gán giá trị 15 (giây) cho thanh ghi R1 để đếm ngược thời gian đèn đỏ hướng Đông-Tây
    MOV P3, #100001B    ; Thiết lập trạng thái đèn: Bắc-Nam Xanh (P3.2=1), Đông-Tây Đỏ (P3.3=1), các đèn khác tắt
    ACALL PHASE_NS_GREEN; Gọi chương trình con PHASE_NS_GREEN để thực hiện giai đoạn đèn xanh Bắc-Nam

    ; NS Yellow (4s), EW Red (4s remaining)
    MOV R0, #4          ; Gán giá trị 4 (giây) cho thanh ghi R0 để đếm ngược thời gian đèn vàng hướng Bắc-Nam
    MOV R1, #4          ; Gán giá trị 4 (giây) cho thanh ghi R1 để đếm ngược thời gian đèn đỏ hướng Đông-Tây (15 - 11 = 4 giây còn lại)
    MOV P3, #100010B    ; Thiết lập trạng thái đèn: Bắc-Nam Vàng (P3.1=1), Đông-Tây Đỏ (P3.3=1), các đèn khác tắt
    ACALL PHASE_NS_YELLOW; Gọi chương trình con PHASE_NS_YELLOW để thực hiện giai đoạn đèn vàng Bắc-Nam

    ; Phase 2: EW Green (11s), NS Red (15s)
    MOV R0, #15         ; Gán giá trị 15 (giây) cho thanh ghi R0 để đếm ngược thời gian đèn đỏ hướng Bắc-Nam
    MOV R1, #11         ; Gán giá trị 11 (giây) cho thanh ghi R1 để đếm ngược thời gian đèn xanh hướng Đông-Tây
    MOV P3, #001100B    ; Thiết lập trạng thái đèn: Đông-Tây Xanh (P3.5=1), Bắc-Nam Đỏ (P3.0=1), các đèn khác tắt
    ACALL PHASE_EW_GREEN; Gọi chương trình con PHASE_EW_GREEN để thực hiện giai đoạn đèn xanh Đông-Tây

    ; EW Yellow (4s), NS Red (4s remaining)
    MOV R0, #4          ; Gán giá trị 4 (giây) cho thanh ghi R0 để đếm ngược thời gian đèn đỏ hướng Bắc-Nam (15 - 11 = 4 giây còn lại)
    MOV R1, #4          ; Gán giá trị 4 (giây) cho thanh ghi R1 để đếm ngược thời gian đèn vàng hướng Đông-Tây
    MOV P3, #010100B    ; Thiết lập trạng thái đèn: Đông-Tây Vàng (P3.4=1), Bắc-Nam Đỏ (P3.0=1), các đèn khác tắt
    ACALL PHASE_EW_YELLOW; Gọi chương trình con PHASE_EW_YELLOW để thực hiện giai đoạn đèn vàng Đông-Tây

    SJMP MAIN_LOOP    ; Nhảy về đầu vòng lặp chính để tiếp tục chu trình

;-------------------------------------------------
; PHASE_NS_GREEN: NS Green + EW Red
;-------------------------------------------------
PHASE_NS_GREEN:
    MOV R7, #11         ; Gán giá trị 11 cho thanh ghi R7, đại diện cho 11 lần lặp (mỗi lần ~1 giây)
PHASE_NS_GREEN_LOOP:
    PUSH 07H            ; Lưu giá trị của thanh ghi R7 vào stack
    MOV R7, #255        ; Gán giá trị 255 cho thanh ghi R7 để tạo vòng lặp delay (chậm hơn)
INNER_NS_GREEN:
    ACALL DISPLAY_TIME  ; Gọi chương trình con DISPLAY_TIME để hiển thị thời gian đếm ngược
    DJNZ R7, INNER_NS_GREEN; Giảm R7 và nhảy về INNER_NS_GREEN nếu R7 chưa bằng 0 (tạo delay)
    POP 07H             ; Khôi phục giá trị của thanh ghi R7 từ stack
    ACALL COUNTDOWN     ; Gọi chương trình con COUNTDOWN để giảm giá trị đếm ngược (R0 và R1)
    DJNZ R7, PHASE_NS_GREEN_LOOP; Giảm R7 và nhảy về PHASE_NS_GREEN_LOOP nếu R7 chưa bằng 0 (duy trì thời gian pha)
    RET                 ; Trở về chương trình gọi

;-------------------------------------------------
; PHASE_NS_YELLOW: NS Yellow + EW Red
;-------------------------------------------------
PHASE_NS_YELLOW:
    MOV R7, #4          ; Gán giá trị 4 cho thanh ghi R7, đại diện cho 4 lần lặp (~4 giây)
PHASE_NS_YELLOW_LOOP:
    PUSH 07H
    MOV R7, #255        ; ~1.5s delay loop (chậm hơn)
INNER_NS_YELLOW:
    ACALL DISPLAY_TIME
    DJNZ R7, INNER_NS_YELLOW
    POP 07H
    ACALL COUNTDOWN
    DJNZ R7, PHASE_NS_YELLOW_LOOP
    RET

;-------------------------------------------------
; PHASE_EW_GREEN: EW Green + NS Red
;-------------------------------------------------
PHASE_EW_GREEN:
    MOV R7, #11         ; 11 seconds for EW green
PHASE_EW_GREEN_LOOP:
    PUSH 07H
    MOV R7, #255        ; ~1.5s delay loop (chậm hơn)
INNER_EW_GREEN:
    ACALL DISPLAY_TIME
    DJNZ R7, INNER_EW_GREEN
    POP 07H
    ACALL COUNTDOWN
    DJNZ R7, PHASE_EW_GREEN_LOOP
    RET

;-------------------------------------------------
; PHASE_EW_YELLOW: EW Yellow + NS Red
;-------------------------------------------------
PHASE_EW_YELLOW:
    MOV R7, #4          ; 4 seconds for EW yellow
PHASE_EW_YELLOW_LOOP:
    PUSH 07H
    MOV R7, #255        ; ~1.5s delay loop (chậm hơn)
INNER_EW_YELLOW:
    ACALL DISPLAY_TIME
    DJNZ R7, INNER_EW_YELLOW
    POP 07H
    ACALL COUNTDOWN
    DJNZ R7, PHASE_EW_YELLOW_LOOP
    RET

;-------------------------------------------------
; DISPLAY_TIME: Display on 4 LEDs (NS and EW)
;-------------------------------------------------
DISPLAY_TIME:
    ; NS countdown (R0) → LED1 (tens), LED2 (units)
    MOV A, R0           ; Chuyển giá trị của R0 (thời gian NS còn lại) vào thanh ghi A
    MOV B, #10          ; Gán giá trị 10 vào thanh ghi B để thực hiện phép chia
    DIV AB              ; Chia A cho B. Thương (hàng chục) được lưu trong A, số dư (hàng đơn vị) được lưu trong B
    MOV R2, A           ; Lưu thương (hàng chục của thời gian NS) vào thanh ghi R2
    MOV R3, B           ; Lưu số dư (hàng đơn vị của thời gian NS) vào thanh ghi R3

    ; EW countdown (R1) → LED3 (tens), LED4 (units)
    MOV A, R1           ; Chuyển giá trị của R1 (thời gian EW còn lại) vào thanh ghi A
    MOV B, #10          ; Gán giá trị 10 vào thanh ghi B
    DIV AB              ; Chia A cho B. Thương (hàng chục) vào A, số dư (hàng đơn vị) vào B
    MOV R4, A           ; Lưu thương (hàng chục của thời gian EW) vào thanh ghi R4
    MOV R5, B           ; Lưu số dư (hàng đơn vị của thời gian EW) vào thanh ghi R5

    ; LED1 (NS tens, R2)
    MOV P0, #0EH        ; Chọn LED 1 (giả sử P0 điều khiển chân chọn LED)
    MOV DPTR, #SEGMENT_CODE; Nạp địa chỉ của bảng mã LED 7 đoạn vào thanh ghi con trỏ dữ liệu DPTR
    MOV A, R2           ; Chuyển giá trị hàng chục của thời gian NS (R2) vào thanh ghi A
    MOVC A, @A+DPTR     ; Đọc mã 7 đoạn tương ứng từ bảng SEGMENT_CODE dựa trên giá trị trong A
    MOV P1, A           ; Xuất mã 7 đoạn ra Port 1 để hiển thị trên LED 1
    ACALL DELAY_2MS     ; Gọi chương trình con DELAY_2MS để tạo khoảng trễ ngắn

    ; LED2 (NS units, R3)
    MOV P0, #0DH        ; Chọn LED 2
    MOV DPTR, #SEGMENT_CODE
    MOV A, R3           ; Chuyển giá trị hàng đơn vị của thời gian NS (R3) vào A
    MOVC A, @A+DPTR
    MOV P1, A
    ACALL DELAY_2MS

    ; LED3 (EW tens, R4)
    MOV P0, #0BH        ; Chọn LED 3
    MOV DPTR, #SEGMENT_CODE
    MOV A, R4           ; Chuyển giá trị hàng chục của thời gian EW (R4) vào A
    MOVC A, @A+DPTR
    MOV P1, A
    ACALL DELAY_2MS

    ; LED4 (EW units, R5)
    MOV P0, #07H        ; Chọn LED 4
    MOV DPTR, #SEGMENT_CODE
    MOV A, R5           ; Chuyển giá trị hàng đơn vị của thời gian EW (R5) vào A
    MOVC A, @A+DPTR
    MOV P1, A
    ACALL DELAY_2MS

    RET                 ; Trở về chương trình gọi

;-------------------------------------------------
; COUNTDOWN: Decrease R0 and R1 if non-zero
;-------------------------------------------------
COUNTDOWN:
    ; NS countdown
    MOV A, R0           ; Chuyển giá trị của R0 vào A
    JZ SKIP_R0          ; Nếu A bằng 0 (R0 đã về 0), nhảy đến SKIP_R0
    DEC R0              ; Giảm giá trị của R0 đi 1
SKIP_R0:
    ; EW countdown
    MOV A, R1           ; Chuyển giá trị của R1 vào A
    JZ SKIP_R1          ; Nếu A bằng 0 (R1 đã về 0), nhảy đến SKIP_R1
    DEC R1              ; Giảm giá trị của R1 đi 1
SKIP_R1:
    RET                 ; Trở về chương trình gọi

;-------------------------------------------------
; DELAY ~2ms
;-------------------------------------------------
DELAY_2MS:
    MOV R6, #255        ; Gán giá trị 255 cho R6 để tạo vòng lặp delay
    DJNZ R6, $          ; Giảm R6 và nhảy về địa chỉ hiện tại ($) nếu R6 chưa bằng 0 (tạo delay)
    RET                 ; Trở về chương trình gọi

;-------------------------------------------------
; 7-Segment Code Table
;-------------------------------------------------
ORG 1000H               ; Đặt địa chỉ bắt đầu cho bảng mã tại 1000H trong bộ nhớ chương trình
SEGMENT_CODE:
    DB 3FH, 06H, 5BH, 4FH, 66H, 6DH, 7DH, 07H, 7FH, 6FH ; Mã 7 đoạn cho các số từ 0 đến 9 (anode chung)
END                     ; Kết thúc chương trình Assembly