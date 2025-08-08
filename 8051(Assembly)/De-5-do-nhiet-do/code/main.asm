$MOD51
; Chỉ định sử dụng bộ vi điều khiển 8051.

ORG 0000H
; Đặt địa chỉ bắt đầu của chương trình tại 0x0000.

; ===== Định nghĩa =====
TEMP_LM  EQU 31H       ; Địa chỉ bộ nhớ lưu giá trị nhiệt độ sau khi đã nhân đôi.
RD_PIN   EQU P3.2      ; Chân điều khiển đọc dữ liệu (Read) của chip ADC0804.
WR_PIN   EQU P3.3      ; Chân điều khiển ghi dữ liệu (Write) / bắt đầu chuyển đổi của chip ADC0804.
INTR     EQU P3.4      ; Chân báo ngắt (Interrupt) của chip ADC0804, báo hiệu quá trình chuyển đổi hoàn tất.

; ===== Chương trình chính =====
START:
    MOV P2, #00H       ; Thiết lập giá trị ban đầu cho Port 2 là 00H, tắt tất cả các đoạn của LED 7 đoạn (với LED cathode chung, mức 0 tích cực).
    MOV P0, #0FFH      ; Thiết lập giá trị ban đầu cho Port 0 là 0FFH, tắt tất cả các LED (với LED được chọn bằng mức thấp, mức 1 không chọn).

MAIN_LOOP:
    ACALL read_adc         ; Gọi chương trình con (Subroutine Call) để đọc giá trị từ ADC0804.
    ACALL adc_to_temp      ; Gọi chương trình con để chuyển đổi giá trị ADC sang giá trị nhiệt độ (đã nhân đôi).
    ACALL extract_digits   ; Gọi chương trình con để tách giá trị nhiệt độ thành các chữ số hàng trăm, chục, đơn vị.
    ACALL display_numbers  ; Gọi chương trình con để hiển thị các chữ số lên 4 LED 7 đoạn.
    SJMP MAIN_LOOP         ; Nhảy không điều kiện (Short Jump) về đầu vòng lặp chính để thực hiện lại các bước.

;-------------------------------------------------
; Đọc dữ liệu từ ADC0804
;-------------------------------------------------
read_adc:
    CLR WR_PIN             ; Xóa bit (Clear bit) của chân WR_PIN xuống mức thấp, bắt đầu quá trình chuyển đổi ADC.
    ACALL delay            ; Gọi chương trình con delay để tạo khoảng thời gian cần thiết cho quá trình bắt đầu chuyển đổi.
    SETB WR_PIN            ; Đặt bit (Set bit) của chân WR_PIN lên mức cao, kết thúc xung kích hoạt chuyển đổi.

wait_intr:
    JB INTR, wait_intr     ; Nhảy nếu bit (Jump if Bit) INTR bằng 1 về lại chính nó, đợi cho chân INTR xuống mức 0 (báo hiệu chuyển đổi hoàn tất).
    CLR RD_PIN             ; Xóa bit của chân RD_PIN xuống mức thấp để bắt đầu quá trình đọc dữ liệu từ ADC.
    ACALL delay            ; Gọi chương trình con delay để tạo khoảng thời gian cần thiết cho quá trình đọc dữ liệu.
    MOV A, P1              ; Di chuyển (Move) giá trị từ Port 1 (nơi dữ liệu ADC được đưa ra) vào thanh ghi A (Accumulator).
    MOV TEMP_LM, A         ; Di chuyển giá trị từ thanh ghi A vào vùng nhớ TEMP_LM để lưu trữ giá trị ADC.
    SETB RD_PIN            ; Đặt bit của chân RD_PIN lên mức cao, kết thúc quá trình đọc dữ liệu từ ADC.
    RET                    ; Trở về (Return) từ chương trình con.

;-------------------------------------------------
; Chuyển đổi giá trị ADC sang nhiệt độ
; (giả sử 255 = 127.5°C, xấp xỉ nhân 2)
;-------------------------------------------------
adc_to_temp:
    MOV A, TEMP_LM         ; Di chuyển giá trị ADC từ TEMP_LM vào thanh ghi A.
    RL A                   ; Dịch trái thanh ghi A một bit (Rotate Left), tương đương với nhân giá trị với 2.
    MOV TEMP_LM, A         ; Di chuyển giá trị đã nhân đôi từ thanh ghi A trở lại vùng nhớ TEMP_LM.
    RET                    ; Trở về từ chương trình con.

;-------------------------------------------------
; Tách nhiệt độ thành 3 chữ số: R1 (trăm), R2 (chục), R3 (đơn vị)
;-------------------------------------------------
extract_digits:
    MOV A, TEMP_LM         ; Di chuyển giá trị nhiệt độ (đã nhân đôi) từ TEMP_LM vào thanh ghi A.
    MOV B, #100            ; Di chuyển giá trị 100 vào thanh ghi B để thực hiện phép chia.
    DIV AB                 ; Chia (Divide) giá trị trong A cho giá trị trong B. Kết quả: A chứa thương (hàng trăm), B chứa số dư.
    MOV R1, A              ; Di chuyển thương (hàng trăm) từ thanh ghi A vào thanh ghi R1.
    MOV A, B               ; Di chuyển số dư (phần còn lại sau khi chia cho 100) từ thanh ghi B vào thanh ghi A.
    MOV B, #10              ; Di chuyển giá trị 10 vào thanh ghi B để tiếp tục phép chia.
    DIV AB                 ; Chia giá trị trong A (số dư trước đó) cho 10. Kết quả: A chứa thương (hàng chục), B chứa số dư (hàng đơn vị).
    MOV R2, A              ; Di chuyển thương (hàng chục) từ thanh ghi A vào thanh ghi R2.
    MOV R3, B              ; Di chuyển số dư (hàng đơn vị) từ thanh ghi B vào thanh ghi R3.
    RET                    ; Trở về từ chương trình con.

;-------------------------------------------------
; Hiển thị 3 chữ số nhiệt độ trên 4 LED 7 đoạn
;-------------------------------------------------
display_numbers:
    ; LED 1 - hàng trăm
    MOV P0, #0FEH          ; Chọn LED thứ nhất (P0.0 = 0, các chân còn lại = 1, active low).
    MOV DPTR, #SEGMENT_CODE ; Đặt con trỏ dữ liệu (Data Pointer) trỏ đến địa chỉ của bảng mã LED 7 đoạn.
    MOV A, R1              ; Di chuyển giá trị hàng trăm từ thanh ghi R1 vào thanh ghi A.
    MOVC A, @A+DPTR        ; Di chuyển mã 7 đoạn tương ứng với giá trị trong A từ bộ nhớ chương trình vào thanh ghi A.
    MOV P2, A              ; Xuất mã 7 đoạn từ thanh ghi A ra Port 2 để hiển thị lên LED.
    ACALL DELAY_2MS        ; Gọi chương trình con delay để LED hiển thị trong một khoảng thời gian.

    ; LED 2 - hàng chục
    MOV P0, #0FDH          ; Chọn LED thứ hai (P0.1 = 0, các chân còn lại = 1).
    MOV DPTR, #SEGMENT_CODE ; Đặt lại con trỏ dữ liệu trỏ đến bảng mã LED.
    MOV A, R2              ; Di chuyển giá trị hàng chục từ thanh ghi R2 vào thanh ghi A.
    MOVC A, @A+DPTR        ; Lấy mã 7 đoạn cho hàng chục.
    MOV P2, A              ; Xuất mã 7 đoạn ra Port 2.
    ACALL DELAY_2MS        ; Gọi delay.

    ; LED 3 - hàng đơn vị
    MOV P0, #0FBH          ; Chọn LED thứ ba (P0.2 = 0, các chân còn lại = 1).
    MOV DPTR, #SEGMENT_CODE ; Đặt lại con trỏ dữ liệu.
    MOV A, R3              ; Di chuyển giá trị hàng đơn vị từ thanh ghi R3 vào thanh ghi A.
    MOVC A, @A+DPTR        ; Lấy mã 7 đoạn cho hàng đơn vị.
    MOV P2, A              ; Xuất mã 7 đoạn ra Port 2.
    ACALL DELAY_2MS        ; Gọi delay.

    ; LED 4 - không dùng (hoặc có thể hiện 'C')
    MOV P0, #0F7H          ; Chọn LED thứ tư (P0.3 = 0, các chân còn lại = 1).
    MOV P2, #0FFH          ; Tắt tất cả các đoạn của LED thứ tư (hiển thị trống).
    ACALL DELAY_2MS        ; Gọi delay.

    RET                    ; Trở về từ chương trình con.

;-------------------------------------------------
; Delay ngắn cho ADC
;-------------------------------------------------
delay:
    MOV R6, #100           ; Thiết lập giá trị đếm cho vòng lặp delay ngoài.
D1: MOV R7, #1FH            ; Thiết lập giá trị đếm cho vòng lặp delay trong.
D2: DJNZ R7, D2            ; Giảm giá trị R7 và nhảy về D2 nếu R7 khác 0 (vòng lặp trong).
    DJNZ R6, D1            ; Giảm giá trị R6 và nhảy về D1 nếu R6 khác 0 (vòng lặp ngoài).
    RET                    ; Trở về từ chương trình con delay.

;-------------------------------------------------
; Delay để hiển thị LED (khoảng ~2ms)
;-------------------------------------------------
DELAY_2MS:
    MOV R6, #250           ; Thiết lập giá trị đếm cho vòng lặp delay.
DELAY_LOOP:
    DJNZ R6, DELAY_LOOP    ; Giảm giá trị R6 và nhảy về DELAY_LOOP nếu R6 khác 0.
    RET                    ; Trở về từ chương trình con delay.

;-------------------------------------------------
; Bảng mã LED 7 đoạn (Cathode chung)
; SEGMENT_CODE[0-9] = mã từ 0 đến 9
;-------------------------------------------------
ORG 1000H
SEGMENT_CODE:
    DB 3FH ; 0             ; Mã 7 đoạn để hiển thị số 0.
    DB 06H ; 1             ; Mã 7 đoạn để hiển thị số 1.
    DB 5BH ; 2             ; Mã 7 đoạn để hiển thị số 2.
    DB 4FH ; 3             ; Mã 7 đoạn để hiển thị số 3.
    DB 66H ; 4             ; Mã 7 đoạn để hiển thị số 4.
    DB 6DH ; 5             ; Mã 7 đoạn để hiển thị số 5.
    DB 7DH ; 6             ; Mã 7 đoạn để hiển thị số 6.
    DB 07H ; 7             ; Mã 7 đoạn để hiển thị số 7.
    DB 7FH ; 8             ; Mã 7 đoạn để hiển thị số 8.
    DB 6FH ; 9             ; Mã 7 đoạn để hiển thị số 9.

;-------------------------------------------------
END