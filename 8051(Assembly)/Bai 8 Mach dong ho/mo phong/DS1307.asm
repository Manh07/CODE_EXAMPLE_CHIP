;=====================================
$mod51
TEMP DATA 37H
XUNG_NHAY DATA 38H ; XUNG 100ms
BIEN_NHAY DATA 39H ;0 = SANG TAT CA DEN , 1 = NHAY led TUONG UNG KHI set
GIAY DATA 40H
PHUT DATA 41H
GIO DATA 42H
DONVI_GIAY DATA 47H
CHUC_GIAY DATA 48H
DONVI_PHUT DATA 49H
CHUC_PHUT DATA 4AH
DONVI_GIO DATA 4BH
CHUC_GIO DATA 4CH
PHAN_TRAM_GIAY DATA 4DH
FLAG_SET DATA 4EH ;0 = KHONG SET , 1 = SET PHUT , 2 = SET GIO
LED_GIAY BIT P2.0
LED_C_GIAY BIT P2.1
LED_PHUT BIT P2.2
LED_C_PHUT BIT P2.3
LED_GIO BIT P2.4
LED_C_GIO BIT P2.5
;--------I2C-------
SCL BIT P3.0
SDA BIT P3.1
SW_1 BIT P3.2
SW_2 BIT P3.3
SW_3 BIT P3.4
LED_DATA EQU P0
BYTE_W EQU 11010000B
BYTE_R EQU 11010001B
ADD_LOW EQU 62H
DATA_DS EQU 63H
;=====================================
;=====================================
ORG 00H
LJMP MAIN
;===========================
;===========================
ORG 0BH
LJMP NGAT_TIME
;===========================
;===========================
ORG 030H
MAIN: ;reset tat ca cac bien
MOV GIAY,#0
MOV PHUT,#0
MOV GIO,#0
MOV BIEN_NHAY,#0
MOV XUNG_NHAY,#0
MOV FLAG_SET,#0
MOV R0,#0
MOV IE,#10001010B
MOV TMOD,#11H
MOV TL0,#LOW(-9216)
MOV TH0,#HIGH(-9216)
SETB TR0
MOV A,#0FFH
MOV LED_DATA,A
MOV DPTR,#BANGSO
CLR SCL
CLR SDA
NOP
SETB SCL
SETB SDA
NOP
MOV ADD_LOW,#00H
MOV DATA_DS,#00H
LCALL WRITE_BYTE
;==========

;==========================================
LOOP_HIEN_THI: ; chuong trinh chinh chay tai day
;==========================================
MOV A,FLAG_SET
CJNE A,#0,L_HT
CALL INIT_PORT
L_HT:
LCALL HIEN_THI
LCALL SCAN_KEY
SJMP LOOP_HIEN_THI
;==========================================


INIT_PORT:
;================================================= =READS SECONDS
;=================================================
READ_SEC:
MOV ADD_LOW,#00h
LCALL READ_BYTE
MOV A,DATA_DS
CALL BCD_HEX
MOV GIAY,A
LCALL I2C_STOP
;================================================= =READS MINUTES
MOV ADD_LOW,#01h
LCALL READ_BYTE
MOV A,DATA_DS
CALL BCD_HEX
MOV PHUT,A
LCALL I2C_STOP
;================================================= =READS HOURS
MOV ADD_LOW,#02h
LCALL READ_BYTE
MOV A,DATA_DS
CALL BCD_HEX
MOV GIO,A
LCALL I2C_STOP
RET
;================================================= =================================
;=====stop I2C communication
I2C_Stop:
CLR SDA
SETB SCL
NOP
SETB SDA
RET
;================================================= =================================
;************************************************* ****
;* WRITE DATA_DS TO DS1307 1 BYTE *
;* INPUT : ADD_LOW *
;* : DATA_DS *
;************************************************* ****
WRITE_BYTE:
CLR SDA ;start bit
CLR SCL
MOV A,#BYTE_W ;send control byte
LCALL LOOP_BYTE
SETB SDA
SETB SCL
JB SDA,WRITE_BYTE ;loop until busy
CLR SCL
MOV A,ADD_LOW ;send address low
LCALL LOOP_BYTE
SETB SDA
SETB SCL
JB SDA,WRITE_BYTE ;loop until busy
CLR SCL
MOV A,DATA_DS ;send DATA
LCALL LOOP_BYTE
SETB SDA
SETB SCL
JB SDA,WRITE_BYTE ;loop until busy
CLR SDA
CLR SCL
SETB SCL ;stop bit
SETB SDA
RET
;==========================================
BCD_HEX:
;==========================================
MOV B,#10H
DIV AB
MOV TEMP,B ;CAT HANG DON VI
MOV B,#10
MUL AB
ADD A,TEMP
ret
;==========================================
HEX_BCD:
;==========================================
MOV B,#10
DIV AB
MOV TEMP,B ;CAT HANG DON VI
MOV B,#10H
MUL AB
ADD A,TEMP
ret
;==========================================
;************************************************* *****
;* READ DATA FROM DS1307 1 BYTE *
;* INPUT : ADD_HIGH *
;* : ADD_LOW *
;* OUTPUT : DATA_DS *
;************************************************* *****
READ_BYTE:
CLR SDA ;start bit
CLR SCL
MOV A,#BYTE_W ;send control byte
LCALL LOOP_BYTE
SETB SDA
SETB SCL
JB SDA,READ_BYTE ;loop until busy
CLR SCL
MOV A,ADD_LOW ;send address low
LCALL LOOP_BYTE
SETB SDA
SETB SCL
JB SDA,READ_BYTE ;loop until busy
CLR SCL
SETB SCL
SETB SDA
CLR SDA ;start bit
CLR SCL
MOV A,#BYTE_R ;send control byte
LCALL LOOP_BYTE
SETB SDA
SETB SCL
JB SDA,READ_BYTE ;loop until busy
CLR SCL
LCALL LOOP_READ
SETB SDA
SETB SCL
CLR SCL
SETB SCL ;stop bit
SETB SDA
RET

;************************************************* ***
;* WRITE *
;* INPUT: ACC *
;************************************************* ***
LOOP_BYTE:
PUSH 02H
MOV R2,#08H
LOOP_SEND:
RLC A
MOV SDA,C
SETB SCL
CLR SCL
DJNZ R2,LOOP_SEND
POP 02H
RET
;************************************************* ****
;* READ *
;* OUTPUT: ACC *
;************************************************* ****
LOOP_READ:
PUSH 02H
MOV R2,#08H
LOOP_READ1:
SETB SCL
MOV C,SDA
CLR SCL
RLC A
DJNZ R2,LOOP_READ1
MOV DATA_DS,A
POP 02H
RET
;==========================================
TACHSO: ; tach rieng hang chuc va hang don vi bang cach chia cho 10
;==========================================
MOV A,GIAY ;Lan luot chia cac Bien: Giay, Phut, Gio cho 10
MOV B,#10 ;de tach phan Don Vi va Hang Chuc ra, de cat rieng vao cac Bien tuong ung.
DIV AB ;PHAN NGUYEN trong A, PHAN DU trong B
MOV CHUC_GIAY,A ;Luu lai HANG CHUC Giay
MOV DONVI_GIAY,B ;luu lai DON VI Giay
;==========
MOV A,PHUT
MOV B,#10
DIV AB
MOV CHUC_PHUT,A
MOV DONVI_PHUT,B
;==========
MOV A,GIO
MOV B,#10
DIV AB
MOV CHUC_GIO,A
MOV DONVI_GIO,B
RET
;========================================
HIEN_THI: ; HIEN THI LED 7 DOAN
;========================================
MOV A,FLAG_SET
CJNE A,#0,CHOP_NHAY
LCALL HIENTHI
AJMP THOAT_HIENTHI
;========================================
CHOP_NHAY: ; KIEM TRA BIEN NHAY VA FLAG_SET DE TAO HIEU UNG NHAY LED DANG SETING
;=================================
MOV A,BIEN_NHAY
CJNE A,#0,CHOP_NHAY1
LCALL HIENTHI
AJMP THOAT_HIENTHI
CHOP_NHAY1:
LCALL NHAY
JMP CHOP_NHAY
THOAT_HIENTHI:
RET
;=================================
HIENTHI:
;=================================
LCALL HIENTHI_S
LCALL HIENTHI_P
LCALL HIENTHI_G
RET
;=================================
NHAY:
;=================================
MOV A,FLAG_SET
CJNE A,#1,KT1
LCALL HIENTHI_S
LCALL HIENTHI_G
KT1:
MOV A,FLAG_SET
CJNE A,#2,THOAT_N
LCALL HIENTHI_S
LCALL HIENTHI_P
THOAT_N:
RET
;=================================
HIENTHI_S:
;=================================
;hien thi hang don vi cua Giay
MOV A,DONVI_GIAY
MOVC A,@A+DPTR
MOV LED_DATA,A
CLR LED_GIAY
LCALL DL
SETB LED_GIAY
;==========
MOV A,CHUC_GIAY ;hien thi hang chuc cua Giay
MOVC A,@A+DPTR
MOV LED_DATA,A
CLR LED_C_GIAY
LCALL DL
SETB LED_C_GIAY
RET
;=================================
HIENTHI_P:
;=================================
MOV A,DONVI_PHUT ;hien thi hang don vi cua Phut
MOVC A,@A+DPTR
MOV LED_DATA,A
CLR LED_PHUT
LCALL DL
SETB LED_PHUT
;============
MOV A,CHUC_PHUT ;hien thi hang chuc cua Phut
MOVC A,@A+DPTR
MOV LED_DATA,A
CLR LED_C_PHUT
LCALL DL
SETB LED_C_PHUT
RET
;=================================
HIENTHI_G:
;=================================
MOV A,DONVI_GIO ;hien thi hang don vi cua gio
MOVC A,@A+DPTR
MOV LED_DATA,A
CLR LED_GIO
LCALL DL
SETB LED_GIO
;============
MOV A,CHUC_GIO ;hien thi hang chuc cua Gio
MOVC A,@A+DPTR
MOV LED_DATA,A
CLR LED_C_GIO
LCALL DL
SETB LED_C_GIO
RET
;=================================
NGAT_TIME:
;=================================
INC XUNG_NHAY
INC PHAN_TRAM_GIAY ;DAT TIMER CHAY 1/100 GIAY
MOV TL0,#LOW(-9216)
MOV TH0,#HIGH(-9216)
SETB TR0
;===============
PUSH ACC
PUSH PSW ;Thanh ghi trang th�i chuong tr�nh
;==========
MOV A,XUNG_NHAY ;TAO XUNG NHAP NHAY = 1/4 GIAY
CJNE A,#25,TIME1
MOV XUNG_NHAY,#0
INC BIEN_NHAY
MOV A,BIEN_NHAY
CJNE A,#3,TIME1
MOV BIEN_NHAY,#0
;==========
TIME1:
MOV A,PHAN_TRAM_GIAY ;Kiem tra bien PHAN_TRAM_GIAY - Thoat khoi ngat Time0 neu khong =
CJNE A,#100,THOAT_NGAT_TIME
MOV PHAN_TRAM_GIAY,#0 ;Neu = 100 th� set bien nay = 0
;===============
THOAT_NGAT_TIME:
LCALL TACHSO
POP PSW
POP ACC
RETI
;==================================
SCAN_KEY: ;KIEM TRA PHIM NHAN
;==================================
SW1: ;SET TIME
JB SW_1,SW2
INC FLAG_SET
MOV A,FLAG_SET
CJNE A,#3,L_SW1
;==========
MOV A,PHUT
CALL HEX_BCD
MOV DATA_DS,A
MOV ADD_LOW,#01H
LCALL WRITE_BYTE
;==========
MOV A,GIO
CALL HEX_BCD
MOV DATA_DS,A
MOV ADD_LOW,#02H
LCALL WRITE_BYTE

MOV FLAG_SET,#0
L_SW1:
LCALL DL1
LCALL DL1
LCALL DL1
LCALL DL1
LJMP NOKEY
;===============
SW2: ;SET_MIN
JB SW_2,SW3
MOV A,FLAG_SET
CJNE A,#0,SW20
LJMP NOKEY
SW20:
MOV A,FLAG_SET
CJNE A,#1,TANG_GIO ;
JB SW_2,SW3
;===============
TANG_PHUT:
INC PHUT ;Roi tang Bien phut them 1
MOV A,PHUT
CJNE A,#60,L_SW2 ;
MOV PHUT,#0 ;Neu = 60 th� set bien nay = 0
L_SW2:
LCALL DL1
LCALL DL1
LJMP SW2
;===============
TANG_GIO: ;SET HOUR
JB SW_2,SW3
MOV A,FLAG_SET
CJNE A,#2,SW3
JB SW_2,SW3
INC GIO ;Roi tang Bien Gio them 1
MOV A,GIO
CJNE A,#24,L_TANG_GIO ;Bien gio = 60? - Thoat khoi ngat Time0 neu khong =
MOV GIO,#0
L_TANG_GIO:
LCALL DL1
LCALL DL1
LJMP TANG_GIO
;==============================
SW3: ;DANG NHAN SW3?
JB SW_3,NOKEY ;KHONG NHAN SW2? KIEM TRA SW3
MOV A,FLAG_SET
CJNE A,#0,SW30
LJMP NOKEY
SW30:
MOV A,FLAG_SET ;DANG NHAN SW2. KIEM TRA CHE DO CHINH GIO HAY CHINH PHUT.
CJNE A,#1,GIAM_GIO ;
JB SW_3,NOKEY
;===============
GIAM_PHUT:
DEC PHUT ;Roi tang Bien phut them 1
MOV A,PHUT
CJNE A,#-1,L_SW3 ;Bien Phut = -1? - Thoat khoi ngat Time0 neu khong =
MOV PHUT,#59 ;Neu = -1 th� set bien nay = 60
L_SW3:
LCALL DL1
LCALL DL1
LJMP SW3
;===============
GIAM_GIO:
;===============
JB SW_3,NOKEY
MOV A,FLAG_SET
CJNE A,#2,NOKEY

LCALL HIENTHI
JB SW_3,NOKEY
DEC GIO ;Roi Giam Bien Gio them 1
MOV A,GIO
CJNE A,#-1,L_GIAM_GIO ;Bien gio = -1? - Thoat khoi ngat Time0 neu khong =
MOV GIO,#23
L_GIAM_GIO:
LCALL DL1
LCALL DL1
LJMP GIAM_GIO
;===============
NOKEY:
RET
;=================================
DL:
;=================================
MOV R7,#200
DJNZ R7,$
RET
;==================
DL1:
;==================
PUSH 00H
PUSH 01H
MOV R1,#20O
DEL:
LCALL HIEN_THI
LCALL HIEN_THI
LCALL HIEN_THI
MOV R0,#250
DJNZ R0,$
DJNZ R1,DEL
POP 01H
POP 00H
RET
;===================
DELAY_1MS:
;===================
MOV R7,#200
DL_1MS_1:
MOV R6,#200
DJNZ R6,$
DJNZ R7,DL_1MS_1
RET
;====================
DELAY:
;====================
NOP
NOP
NOP
NOP
NOP
NOP
NOP
NOP
RET
;=================================
BANGSO:
;=================================
DB 0C0H,0F9H,0A4H,0B0H,99H,92H,82H,0F8H,80H,90H
RET
END 