ORG 0000H                   ; Đặt địa chỉ bắt đầu chương trình tại 0000H
SJMP START_PROGRAM          ; Nhảy tới chương trình chính tại nhãn START_PROGRAM

;-------------------------------------------------
; Định nghĩa bit đặc biệt
;-------------------------------------------------
RUNNING  BIT  32H           ; Đặt cờ RUNNING tại địa chỉ RAM bit 20H + 18H = 38H để theo dõi trạng thái đếm
LED      BIT  P3.6          ; Đặt nhãn LED cho chân P3.6 để điều khiển bật/tắt LED báo hiệu

;-------------------------------------------------
; Chương trình khởi tạo
;-------------------------------------------------
START_PROGRAM:
MOV  R0, #00H               ; R0 = phút ban đầu = 0
MOV  R1, #00H               ; R1 = giây ban đầu = 0

MOV  30H, #00H              ; Lưu phút ban đầu vào địa chỉ RAM 30H (phục hồi khi nhấn Reset)
MOV  31H, #00H              ; Lưu giây ban đầu vào 31H

CLR  RUNNING                ; Xóa cờ RUNNING (chưa chạy)
CLR  LED                    ; Tắt LED cảnh báo ban đầu

;-------------------------------------------------
; Vòng lặp chính
;-------------------------------------------------
MAIN_LOOP:
    ACALL CHECK_BUTTONS     ; Gọi hàm kiểm tra nút nhấn

    JB   RUNNING, RUNNING_MODE ; Nếu RUNNING = 1 thì vào chế độ đếm

NOT_RUNNING:
    ACALL DISPLAY_TIME      ; Hiển thị thời gian hiện tại khi không chạy
    SJMP MAIN_LOOP          ; Lặp lại chương trình chính

RUNNING_MODE:
    MOV  R7, #255           ; Đặt bộ đếm delay để hiển thị khoảng 1 giây
DISPLAY_LOOP:
    ACALL DISPLAY_TIME      ; Hiển thị thời gian trong khi đếm
    ACALL CHECK_BUTTONS     ; Kiểm tra nút nhấn khi đang chạy
    DJNZ R7, DISPLAY_LOOP   ; Giảm R7 và lặp lại nếu chưa hết

    ACALL COUNTDOWN         ; Giảm thời gian đi 1 giây
    SJMP MAIN_LOOP          ; Quay về vòng lặp chính

;-------------------------------------------------
; DISPLAY_TIME - hiển thị thời gian lên LED 7 đoạn
;-------------------------------------------------
DISPLAY_TIME:
    ; Chuyển phút thành chữ số hàng chục và đơn vị
    MOV  A, R0              ; A = phút
    MOV  B, #10             ; Chia cho 10
    DIV  AB                 ; A = hàng chục phút, B = hàng đơn vị
    MOV  R2, A              ; R2 = hàng chục phút
    MOV  R3, B              ; R3 = hàng đơn vị phút

    ; Chuyển giây thành chữ số hàng chục và đơn vị
    MOV  A, R1              ; A = giây
    MOV  B, #10
    DIV  AB
    MOV  R4, A              ; R4 = hàng chục giây
    MOV  R5, B              ; R5 = hàng đơn vị giây

    ; Hiển thị lên 4 LED 7 đoạn

    ; LED1: hàng chục phút
    MOV  P0, #0EH           ; Chọn LED 1 (thanh ghi điều khiển)
    MOV  DPTR, #SEGMENT_CODE ; Trỏ tới bảng mã LED
    MOV  A, R2
    MOVC A, @A+DPTR         ; Đọc mã LED tương ứng với số R2
    MOV  P1, A              ; Xuất mã LED ra cổng P1
    ACALL DELAY_2MS         ; Delay ngắn để ổn định hiển thị

    ; LED2: hàng đơn vị phút + dấu chấm
    MOV  P0, #0DH           ; Chọn LED 2
    MOV  DPTR, #SEGMENT_CODE
    MOV  A, R3
    MOVC A, @A+DPTR
    ORL  A, #080H           ; Thêm dấu chấm (bit 7 = 1)
    MOV  P1, A
    ACALL DELAY_2MS

    ; LED3: hàng chục giây
    MOV  P0, #0BH           ; Chọn LED 3
    MOV  DPTR, #SEGMENT_CODE
    MOV  A, R4
    MOVC A, @A+DPTR
    MOV  P1, A
    ACALL DELAY_2MS

    ; LED4: hàng đơn vị giây
    MOV  P0, #07H           ; Chọn LED 4
    MOV  DPTR, #SEGMENT_CODE
    MOV  A, R5
    MOVC A, @A+DPTR
    MOV  P1, A
    ACALL DELAY_2MS

    RET                     ; Quay về chương trình gọi

;-------------------------------------------------
; COUNTDOWN - Giảm thời gian đi 1 giây
;-------------------------------------------------
COUNTDOWN:
    MOV  A, R1              ; Kiểm tra giây
    JZ   SEC_ZERO           ; Nếu giây = 0, xử lý tiếp
    DEC  R1                 ; Giảm giây
    CLR  LED                ; Tắt LED khi chưa về 0
    RET

SEC_ZERO:
    MOV  A, R0              ; Kiểm tra phút
    JZ   TIME_UP            ; Nếu phút cũng = 0 thì hết giờ
    DEC  R0                 ; Giảm phút
    MOV  R1, #00            ; Gán lại giây = 00
    SETB LED                ; Bật LED khi sang phút mới
    RET

TIME_UP:
    CLR  RUNNING            ; Hết giờ, dừng đếm
    CLR  LED                ; Tắt LED cảnh báo
    RET

;-------------------------------------------------
; CHECK_BUTTONS - xử lý các nút điều khiển
;-------------------------------------------------
CHECK_BUTTONS:
    ; Nút START tại P3.2
    JB   P3.2, SKIP_START   ; Nếu chưa nhấn thì bỏ qua
    ACALL DELAY_20MS        ; Delay chống dội phím
    JB   P3.2, SKIP_START
    SETB RUNNING            ; Bắt đầu chạy
WAIT_START_RELEASE:
    JNB  P3.2, WAIT_START_RELEASE ; Chờ nhả nút
SKIP_START:

    ; Nút RESET tại P3.3
    JB   P3.3, SKIP_RESET
    ACALL DELAY_20MS
    JB   P3.3, SKIP_RESET
    MOV  R0, 30H            ; Phục hồi phút từ bộ nhớ
    MOV  R1, 31H            ; Phục hồi giây
    CLR  RUNNING            ; Dừng đếm
    CLR  LED                ; Tắt LED cảnh báo
WAIT_RESET_RELEASE:
    JNB  P3.3, WAIT_RESET_RELEASE
SKIP_RESET:

    ; Nút tăng phút tại P3.0
    JB   P3.0, SKIP_INC_MIN
    ACALL DELAY_20MS
    JB   P3.0, SKIP_INC_MIN
    MOV  A, R0
    ADD  A, #5              ; Tăng 5 phút
    CJNE A, #100, SKIP_INC_MIN_WRAP ; Nếu < 100 thì tiếp tục
    MOV  A, #00             ; Nếu >= 100 thì quay về 0
SKIP_INC_MIN_WRAP:
    MOV  R0, A              ; Cập nhật R0
WAIT_INC_MIN_RELEASE:
    JNB  P3.0, WAIT_INC_MIN_RELEASE
SKIP_INC_MIN:

    ; Nút tăng giây tại P3.1
    JB   P3.1, SKIP_INC_SEC
    ACALL DELAY_20MS
    JB   P3.1, SKIP_INC_SEC
    MOV  A, R1
    ADD  A, #10             ; Tăng 10 giây
    CJNE A, #100, SKIP_INC_SEC_WRAP
    MOV  A, #00
SKIP_INC_SEC_WRAP:
    MOV  R1, A
WAIT_INC_SEC_RELEASE:
    JNB  P3.1, WAIT_INC_SEC_RELEASE
SKIP_INC_SEC:

    RET                     ; Quay về chương trình chính

;-------------------------------------------------
; DELAY 2ms - delay ngắn để ổn định LED
;-------------------------------------------------
DELAY_2MS:
    MOV  R6, #250
    DJNZ R6, $              ; Lặp đến khi R6 = 0
    RET

;-------------------------------------------------
; DELAY_20MS - delay chống dội phím
;-------------------------------------------------
DELAY_20MS:
    MOV  R6, #250
DELAY_20MS_LOOP1:
    MOV  R7, #250
DELAY_20MS_LOOP2:
    DJNZ R7, DELAY_20MS_LOOP2
    DJNZ R6, DELAY_20MS_LOOP1
    RET

;-------------------------------------------------
; SEGMENT_CODE - bảng mã LED 7 đoạn cho các số 0–9
;-------------------------------------------------
ORG 1000H
SEGMENT_CODE:
    DB 3FH,06H,5BH,4FH,66H,6DH,7DH,07H,7FH,6FH ; 0–9: LED mã BCD

;-------------------------------------------------
; Kết thúc chương trình
;-------------------------------------------------
END
