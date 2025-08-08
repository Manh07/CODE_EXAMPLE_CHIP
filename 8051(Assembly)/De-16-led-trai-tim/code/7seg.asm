; Định nghĩa các biến và hằng số
ROW         EQU P1          ; Gán tên ROW cho Port 1, port này sẽ điều khiển các hàng của ma trận LED
COL         EQU P2          ; Gán tên COL cho Port 2, port này sẽ điều khiển các cột của ma trận LED
DELAY_COUNT EQU 30          ; Định nghĩa hằng số DELAY_COUNT với giá trị 30, được sử dụng trong hàm delay để tạo độ trễ khoảng 500ms

ORG 0000H                   ; Chỉ thị cho trình biên dịch đặt đoạn mã sau tại địa chỉ 0000H (điểm bắt đầu thực thi của vi điều khiển)
    SJMP MAIN               ; Lệnh nhảy không điều kiện đến nhãn MAIN, bỏ qua các phần mã khác có thể nằm trước

; Chương trình chính
MAIN:

LOOP:
    MOV COL, #01000000B      ; Gửi giá trị nhị phân 01000000B (chỉ bit 6 là 1) ra Port 2 (điều khiển cột), kích hoạt cột tương ứng
    MOV ROW, #11101111B      ; Gửi giá trị nhị phân 11101111B ra Port 1 (điều khiển hàng), kích hoạt hàng tương ứng (bit 3 là 0)
    ACALL DELAY_MS          ; Gọi chương trình con DELAY_MS để tạo một khoảng trễ ngắn, giúp mắt người nhìn thấy LED sáng

    MOV COL, #01000000B
    MOV ROW, #11110111B
    ACALL DELAY_MS

    MOV COL, #00100000B      ; Kích hoạt cột có bit 5 là 1
    MOV ROW, #11111011B      ; Kích hoạt hàng có bit 2 là 0
    ACALL DELAY_MS

    MOV COL, #00010000B      ; Kích hoạt cột có bit 4 là 1
    MOV ROW, #11111101B      ; Kích hoạt hàng có bit 1 là 0
    ACALL DELAY_MS

    MOV COL, #00001000B      ; Kích hoạt cột có bit 3 là 1
    MOV ROW, #11111110B      ; Kích hoạt hàng có bit 0 là 0
    ACALL DELAY_MS

    MOV COL, #00000100B      ; Kích hoạt cột có bit 2 là 1
    MOV ROW, #11111110B
    ACALL DELAY_MS

    MOV COL, #00000010B      ; Kích hoạt cột có bit 1 là 1
    MOV ROW, #11111101B
    ACALL DELAY_MS

    MOV COL, #00000010B
    MOV ROW, #11111011B
    ACALL DELAY_MS

    MOV COL, #00000100B
    MOV ROW, #11110111B
    ACALL DELAY_MS

    MOV COL, #00000100B
    MOV ROW, #11101111B
    ACALL DELAY_MS

    MOV COL, #00000010B
    MOV ROW, #11011111B
    ACALL DELAY_MS

    MOV COL, #00000010B
    MOV ROW, #10111111B
    ACALL DELAY_MS

    MOV COL, #00000100B
    MOV ROW, #01111111B
    ACALL DELAY_MS

    MOV COL, #00001000B
    MOV ROW, #01111111B
    ACALL DELAY_MS

    MOV COL, #00010000B
    MOV ROW, #10111111B
    ACALL DELAY_MS

    MOV COL, #00100000B
    MOV ROW, #11011111B
    ACALL DELAY_MS

    ACALL LOOP              ; Nhảy không điều kiện về đầu nhãn LOOP, tạo thành một vòng lặp vô hạn

; Hàm delay ~500ms
DELAY_MS:
    MOV R2, #DELAY_COUNT    ; Gán giá trị của hằng số DELAY_COUNT (30) vào thanh ghi R2, R2 sẽ được dùng làm bộ đếm vòng lặp ngoài
DELAY_LOOP:
    MOV R3, #10             ; Gán giá trị 10 vào thanh ghi R3, R3 sẽ được dùng làm bộ đếm vòng lặp trung gian (~1ms)
DELAY_INNER:
    MOV R4, #10             ; Gán giá trị 10 vào thanh ghi R4, R4 sẽ được dùng làm bộ đếm vòng lặp trong (~0.1ms)
    DJNZ R4, $              ; Giảm giá trị của R4 đi 1 và nhảy về địa chỉ hiện tại ($) nếu R4 khác 0 (tạo delay nhỏ)
    DJNZ R3, DELAY_INNER    ; Giảm giá trị của R3 đi 1 và nhảy về nhãn DELAY_INNER nếu R3 khác 0 (lặp lại vòng lặp trong)
    DJNZ R2, DELAY_LOOP     ; Giảm giá trị của R2 đi 1 và nhảy về nhãn DELAY_LOOP nếu R2 khác 0 (lặp lại vòng lặp trung gian)
    RET                     ; Lệnh trả về chương trình gọi (trong trường hợp này là MAIN)

END                         ; Chỉ thị kết thúc chương trình Assembly