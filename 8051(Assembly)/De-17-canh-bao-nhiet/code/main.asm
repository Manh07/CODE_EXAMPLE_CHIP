$MOD51
; Chỉ định sử dụng bộ vi điều khiển 8051.

ORG 0000H
; Đặt địa chỉ bắt đầu của chương trình tại 0x0000.

; ========== Định nghĩa ==========
TEMP_SET EQU 30H       ; Địa chỉ bộ nhớ lưu giá trị ngưỡng nhiệt độ.
TEMP_LM  EQU 31H       ; Địa chỉ bộ nhớ lưu giá trị nhiệt độ từ LM35.
U        EQU 32H       ; Thanh ghi tạm thời để lưu nibble cao.
L        EQU 33H       ; Thanh ghi tạm thời để lưu nibble thấp.
LED_PIN  EQU P3.5      ; Chân điều khiển LED.

PORT     EQU P0        ; Định nghĩa PORT là cổng P0.
RS       EQU P2.0      ; Định nghĩa chân RS của LCD là P2.0.
RW       EQU P2.1      ; Định nghĩa chân RW của LCD là P2.1.
EN       EQU P2.2      ; Định nghĩa chân EN của LCD là P2.2.

RD_PIN   EQU P3.2      ; Định nghĩa chân RD của ADC0804 là P3.2.
WR_PIN   EQU P3.3      ; Định nghĩa chân WR của ADC0804 là P3.3.
INTR     EQU P3.4      ; Định nghĩa chân INTR của ADC0804 là P3.4.

; ===== Dữ liệu LCD =====
TXT_TEMP:  DB 'Temp Set:', 0    ; Chuỗi hiển thị "Temp Set:" trên LCD.
TXT_LM35:  DB 'LM35: ', 0       ; Chuỗi hiển thị "LM35: " trên LCD.
TXT_ADC:   DB 'ADC: ', 0        ; Chuỗi hiển thị "ADC: " trên LCD.
TXT_DEG:   DB 'oC     ', 0      ; Chuỗi hiển thị "oC" trên LCD.

; ===== MACRO =====
SS MACRO LBL
    MOV R1, #0                 ; Đặt R1 = 0 (chỉ số bắt đầu của chuỗi).
    MOV DPTR, LBL              ; Đặt con trỏ dữ liệu (DPTR) trỏ đến nhãn chuỗi.
    LCALL lcd_puts             ; Gọi hàm in chuỗi ra LCD.
ENDM
; Macro để in chuỗi ra LCD.

; ===== Chương trình chính =====
START:
    MOV TEMP_SET, #25          ; Đặt ngưỡng nhiệt độ ban đầu là 25°C.
    CLR RW                     ; Đặt chân RW của LCD về mức thấp (chế độ ghi).
    MOV P1, #0FFH              ; Cấu hình cổng P1 làm đầu vào (đọc dữ liệu từ ADC0804).
    ACALL init_lcd             ; Gọi hàm khởi tạo LCD.

MAIN_LOOP:
    ACALL check_button         ; Gọi hàm kiểm tra nút nhấn tăng/giảm ngưỡng nhiệt độ.
    ACALL display_temp         ; Gọi hàm hiển thị ngưỡng nhiệt độ lên LCD.

    ACALL read_adc             ; Gọi hàm đọc giá trị ADC từ ADC0804.
    ACALL adc_to_temp          ; Gọi hàm chuyển đổi giá trị ADC sang nhiệt độ.
    ACALL control_led          ; Gọi hàm điều khiển LED dựa trên nhiệt độ.
    ACALL display_lm35         ; Gọi hàm hiển thị nhiệt độ LM35 lên LCD.

    SJMP MAIN_LOOP             ; Nhảy về đầu vòng lặp chính.

; ===== Khởi tạo LCD =====
init_lcd:
    ACALL delay                ; Gọi hàm delay để đảm bảo LCD ổn định.
    ACALL delay                ; Gọi thêm một lần delay.

    CLR PORT.4                 ; Đặt chân P0.4 về mức thấp.
    SETB PORT.5                ; Đặt chân P0.5 về mức cao.
    CLR PORT.6                 ; Đặt chân P0.6 về mức thấp.
    CLR PORT.7                 ; Đặt chân P0.7 về mức thấp.
    CLR RS                     ; Đặt chân RS về mức thấp (chế độ lệnh).
    SETB EN                    ; Đặt chân EN về mức cao (kích hoạt LCD).
    ACALL delay                ; Gọi hàm delay.
    CLR EN                     ; Đặt chân EN về mức thấp.

    MOV A, #28H                ; Gửi lệnh cấu hình LCD (2 dòng, 5x8 điểm ảnh).
    ACALL lcd_cmd              ; Gọi hàm gửi lệnh đến LCD.
    MOV A, #0CH                ; Gửi lệnh bật màn hình LCD, tắt con trỏ.
    ACALL lcd_cmd              ; Gọi hàm gửi lệnh đến LCD.
    MOV A, #06H                ; Gửi lệnh tự động tăng địa chỉ con trỏ LCD.
    ACALL lcd_cmd              ; Gọi hàm gửi lệnh đến LCD.
    MOV A, #01H                ; Gửi lệnh xóa màn hình LCD.
    ACALL lcd_cmd              ; Gọi hàm gửi lệnh đến LCD.
    RET                        ; Trả về.

; ===== In chuỗi LCD =====
lcd_puts:
    MOV A, R1                  ; Lấy chỉ số hiện tại của chuỗi.
    MOVC A, @A+DPTR            ; Lấy ký tự từ chuỗi tại địa chỉ DPTR + R1.
    JZ lcd_puts_end            ; Nếu ký tự là 0 (kết thúc chuỗi), nhảy đến lcd_puts_end.
    LCALL lcd_data             ; Gửi ký tự đến LCD.
    INC R1                     ; Tăng chỉ số chuỗi.
    SJMP lcd_puts              ; Nhảy về đầu vòng lặp để in ký tự tiếp theo.
lcd_puts_end:
    RET                        ; Trả về.

; ===== Hiển thị Temp Set =====
display_temp:
    MOV A, #80H                ; Đặt con trỏ LCD tại dòng 1.
    ACALL lcd_cmd              ; Gọi hàm gửi lệnh đến LCD.
    MOV DPTR, #TXT_TEMP        ; Hiển thị chuỗi "Temp Set:".
    MOV R1, #0
    LCALL lcd_puts

    MOV A, TEMP_SET            ; Lấy giá trị ngưỡng nhiệt độ.
    ACALL convert_2digit       ; Chuyển đổi sang 2 chữ số ASCII.
    SETB RS                    ; Đặt RS về mức cao (chế độ dữ liệu).
    MOV A, R2                  ; Hiển thị chữ số hàng chục.
    ACALL lcd_data
    MOV A, R3                  ; Hiển thị chữ số hàng đơn vị.
    ACALL lcd_data
    CLR RS                     ; Đặt RS về mức thấp (chế độ lệnh).
    RET                        ; Trả về.

; ===== Hiển thị LM35 =====
display_lm35:
    MOV A, #0C0H               ; Đặt con trỏ LCD tại dòng 2.
    ACALL lcd_cmd              ; Gọi hàm gửi lệnh đến LCD.
    MOV DPTR, #TXT_LM35        ; Hiển thị chuỗi "LM35: ".
    MOV R1, #0
    LCALL lcd_puts

    MOV A, TEMP_LM             ; Lấy giá trị nhiệt độ từ TEMP_LM.
    ACALL convert_2digit       ; Chuyển đổi sang 2 chữ số ASCII.
    SETB RS                    ; Đặt RS về mức cao (chế độ dữ liệu).
    MOV A, R2                  ; Hiển thị chữ số hàng chục.
    ACALL lcd_data
    MOV A, R3                  ; Hiển thị chữ số hàng đơn vị.
    ACALL lcd_data
    CLR RS                     ; Đặt RS về mức thấp (chế độ lệnh).
    RET                        ; Trả về.

; ===== Đọc dữ liệu từ ADC0804 =====
read_adc:
    CLR WR_PIN                 ; Kích hoạt chuyển đổi ADC.
    ACALL delay                ; Gọi hàm delay.
    SETB WR_PIN                ; Kết thúc tín hiệu ghi.

wait_intr:
    JB INTR, wait_intr         ; Chờ INTR xuống thấp (chuyển đổi hoàn tất).
    CLR RD_PIN                 ; Kích hoạt đọc dữ liệu.
    ACALL delay                ; Gọi hàm delay.
    MOV A, P1                  ; Đọc dữ liệu từ cổng P1.
    MOV TEMP_LM, A             ; Lưu giá trị vào TEMP_LM.
    SETB RD_PIN                ; Kết thúc tín hiệu đọc.
    RET                        ; Trả về.

; ===== Chuyển đổi giá trị ADC sang nhiệt độ =====
adc_to_temp:
    MOV A, TEMP_LM             ; Lấy giá trị ADC từ TEMP_LM.
    RL A                       ; Dịch trái 1 bit (nhân giá trị ADC với 2).
    MOV TEMP_LM, A             ; Lưu kết quả vào TEMP_LM.
    RET                        ; Trả về.

; ===== Điều khiển LED =====
control_led:
    MOV A, TEMP_LM             ; Lấy giá trị nhiệt độ.
    SUBB A, TEMP_SET           ; So sánh với ngưỡng đặt.
    JC led_off                 ; Nếu nhỏ hơn ngưỡng, tắt LED.
    SETB LED_PIN               ; Bật LED.
    RET                        ; Trả về.

led_off:
    CLR LED_PIN                ; Tắt LED.
    RET                        ; Trả về.

; ===== Nút nhấn tăng/giảm =====
check_button:
    JB P3.0, check_decrease    ; Kiểm tra nút nhấn giảm.
    ACALL delay
    INC TEMP_SET               ; Tăng ngưỡng nhiệt độ.
check_decrease:
    JB P3.1, check_done        ; Kiểm tra nút nhấn tăng.
    ACALL delay
    MOV R0, #TEMP_SET
    MOV A, @R0
    CJNE A, #0, dec_temp       ; Nếu TEMP_SET > 0, giảm ngưỡng.
    SJMP check_done
dec_temp:
    DEC TEMP_SET               ; Giảm ngưỡng nhiệt độ.
check_done:
    RET                        ; Trả về.

; ===== Chuyển đổi 2 chữ số ASCII =====
convert_2digit:
    MOV B, #10                 ; Chia lấy hàng chục và đơn vị.
    DIV AB
    ADD A, #30H                ; Chuyển sang mã ASCII.
    MOV R2, A
    MOV A, B
    ADD A, #30H
    MOV R3, A
    RET                        ; Trả về.

; ===== Lệnh gửi đến LCD (command) =====
lcd_cmd:
    CLR RS                     ; Gửi lệnh.
    ACALL send_nibble
    RET                        ; Trả về.

; ===== Lệnh gửi đến LCD (data) =====
lcd_data:
    SETB RS                    ; Gửi dữ liệu.
    ACALL send_nibble
    RET                        ; Trả về.

; ===== Gửi 2 nibble tới LCD =====
send_nibble:
    MOV U, A
    ANL U, #0F0H               ; Lấy 4 bit cao.
    SWAP A
    ANL A, #0F0H               ; Lấy 4 bit thấp.
    MOV L, A
    MOV A, U
    ACALL move_to_port         ; Gửi 4 bit cao.
    MOV A, L
    ACALL move_to_port         ; Gửi 4 bit thấp.
    RET                        ; Trả về.

; ===== Xuất dữ liệu ra P0.4–P0.7 =====
move_to_port:
    MOV C, ACC.4
    MOV PORT.4, C
    MOV C, ACC.5
    MOV PORT.5, C
    MOV C, ACC.6
    MOV PORT.6, C
    MOV C, ACC.7
    MOV PORT.7, C
    SETB EN                    ; Kích hoạt LCD.
    ACALL delay
    CLR EN                     ; Tắt kích hoạt LCD.
    ACALL delay
    RET                        ; Trả về.

; ===== Delay dài =====
delay:
    MOV R6, #5FH
D1: MOV R7, #3FH
D2: DJNZ R7, D2
    DJNZ R6, D1
    RET                        ; Trả về.

END
; Kết thúc chương trình.