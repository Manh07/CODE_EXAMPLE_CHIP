#pragma debug scope 1 1
#line 14 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
__stm0ah EQU [02dH]
__stm0al EQU [02cH]
__stm0dh EQU [02bH]
__stm0dl EQU [02aH]
__stm0c1 EQU [029H]
__stm0c0 EQU [028H]
__pasr EQU [026H]
__rstc EQU [025H]
__sadc2 EQU [024H]
__sadc1 EQU [023H]
__sadc0 EQU [022H]
__sadoh EQU [021H]
__sadol EQU [020H]
__eed EQU [01fH]
__eea EQU [01eH]
__smod1 EQU [01cH]
__tbc EQU [01bH]
__wdtc EQU [019H]
__ifs0 EQU [018H]
__pawu EQU [017H]
__papu EQU [016H]
__pac EQU [015H]
__pa EQU [014H]
__mfi0 EQU [011H]
__intc1 EQU [0fH]
__intc0 EQU [0eH]
__integ EQU [0dH]
__smod EQU [0bH]
__status EQU [0aH]
__tblh EQU [08H]
__tblp EQU [07H]
__pcl EQU [06H]
__acc EQU [05H]
__bp EQU [04H]
__mp1 EQU [03H]
__iar1 EQU [02H]
__mp0 EQU [01H]
__iar0 EQU [00H]
@@TB_ISR .SECTION AT 08H 'CODE'
JMP _TB_ISR
@TB_ISR .SECTION 'CODE'
_TB_ISR PROC
MOV V2A,A
MOV A,[0AH]
MOV V2S,A
MOV A,[03H]
MOV V2MP1,A
MOV A,[04H]
MOV V2BP,A
#pragma debug scope 2 1 ; 16 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 16 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _g_sysTime
MOV [04H],A
MOV A,OFFSET _g_sysTime
MOV [03H],A
MOV A,01h
ADDM A,[02H]
MOV A,00h
INC [03H]
ADCM A,[02H]
MOV A,00h
INC [03H]
ADCM A,[02H]
MOV A,00h
INC [03H]
ADCM A,[02H]
#line 17 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _flag1ms
MOV [04H],A
MOV A,OFFSET _flag1ms
MOV [03H],A
MOV A,1
MOV [02H],A
#line 18 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
NOP
L5:
MOV A,V2MP1
MOV [03H],A
MOV A,V2BP
MOV [04H],A
MOV A,V2S
MOV [0AH],A
MOV A,V2A
RETI
_TB_ISR ENDP
RAMBANK 0 @TB_ISR
@TB_ISR .SECTION 'DATA'
#line 28 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
@UART_SoftWareSendByte .SECTION 'CODE'
PUBLIC _UART_SoftWareSendByte
_UART_SoftWareSendByte PROC
#pragma debug scope 3 1
#line 27 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC UART_SoftWareSendByte0
LOCAL UART_SoftWareSendByte0 DB ? ; TxData
#pragma debug variable 8 3 UART_SoftWareSendByte0 TxData
#pragma debug scope 3 1 ; 30 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 30 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [014H].0
#line 31 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,02fH
CALL __DELAYX1
#line 32 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR CR1
#line 32 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L10
L7:
#pragma debug scope 4 3 ; 35 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 35 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SNZ UART_SoftWareSendByte0.0
JMP L11
#line 35 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [014H].0
#line 35 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L12
L11:
#line 36 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [014H].0
L12:
#line 37 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,02fH
CALL __DELAYX1
#line 38 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [0AH].0
RRC UART_SoftWareSendByte0
L8:
#line 32 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#pragma debug scope 3 1 ; 32 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
INC CR1
L10:
#line 32 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,CR1
SUB A,08h
SNZ [0ah].0
JMP L7
#line 40 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [014H].0
#line 41 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,02fH
CALL __DELAYX1
L6:
RET
#line 29 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
LOCAL CR1 DB ? ; i
#pragma debug variable 8 3 CR1 i
_UART_SoftWareSendByte ENDP
#line 45 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
@EEPROM_Write_Byte .SECTION 'CODE'
PUBLIC _EEPROM_Write_Byte
_EEPROM_Write_Byte PROC
#pragma debug scope 5 1
#line 44 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC EEPROM_Write_Byte0
LOCAL EEPROM_Write_Byte0 DB ? ; Data
#pragma debug variable 8 5 EEPROM_Write_Byte0 Data
#line 44 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC EEPROM_Write_Byte1
LOCAL EEPROM_Write_Byte1 DB ? ; adr
#pragma debug variable 8 5 EEPROM_Write_Byte1 adr
#pragma debug scope 5 1 ; 47 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 47 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [04H].0
#line 48 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [0EH].0
#line 49 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,040h
MOV __mp1,A
#line 52 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,EEPROM_Write_Byte1
MOV __eea,A
#line 54 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,EEPROM_Write_Byte0
MOV __eed,A
#line 57 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET __iar1.3
#line 58 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET __iar1.2
L14:
L15:
#line 61 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SZ __iar1.2
JMP L14
#line 62 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR __iar1
#line 63 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [04H].0
#line 64 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [0EH].0
L13:
RET
_EEPROM_Write_Byte ENDP
#line 74 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
@EEPROM_Read_Byte .SECTION 'CODE'
PUBLIC _EEPROM_Read_Byte
_EEPROM_Read_Byte PROC
#pragma debug scope 6 1
#line 73 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC EEPROM_Read_Byte0
LOCAL EEPROM_Read_Byte0 DB ? ; adr
#pragma debug variable 8 6 EEPROM_Read_Byte0 adr
#pragma debug scope 6 1 ; 77 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 77 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [04H].0
#line 78 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,040h
MOV __mp1,A
#line 81 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,EEPROM_Read_Byte0
MOV __eea,A
#line 84 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET __iar1.1
#line 85 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET __iar1.0
L18:
L19:
#line 88 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SZ __iar1.0
JMP L18
#line 89 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR __iar1
#line 90 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [04H].0
#line 92 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,__eed
MOV CR2,A
#line 93 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,CR2
L17:
RET
#line 76 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
LOCAL CR2 DB ? ; EP_Read_Data
#pragma debug variable 8 6 CR2 EP_Read_Data
_EEPROM_Read_Byte ENDP
#line 110 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
@ADC_GetChannelValue .SECTION 'CODE'
PUBLIC _ADC_GetChannelValue
_ADC_GetChannelValue PROC
#pragma debug scope 7 1
#line 109 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC ADC_GetChannelValue0
LOCAL ADC_GetChannelValue0 DB ? ; ADC_Channel
#pragma debug variable 8 7 ADC_GetChannelValue0 ADC_Channel
#pragma debug scope 7 1 ; 112 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 112 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,01fh
ANDM A,__sadc1
#line 113 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,ADC_GetChannelValue0
SUB A,04h
SZ [0ah].0
JMP L23
#pragma debug scope 8 7 ; 115 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 115 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,0f0h
ANDM A,__sadc0
#line 116 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,ADC_GetChannelValue0
ORM A,__sadc0
#line 117 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,ADC_GetChannelValue0
MOV b0_5,A
SZ b0_5
JMP L34
JMP L27
L34:
MOV A,b0_5
XOR A,01h
SZ [0ah].2
JMP L28
MOV A,b0_5
XOR A,02h
SZ [0ah].2
JMP L29
MOV A,b0_5
XOR A,03h
SZ [0ah].2
JMP L30
#line 117 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L24
L27:
#pragma debug scope 9 8 ; 119 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 119 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [026H].1
#line 119 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [026H].0
#line 119 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L24
L28:
#line 120 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [026H].3
#line 120 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [026H].2
#line 120 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L24
L29:
#line 121 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [026H].5
#line 121 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [026H].4
#line 121 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L24
L30:
#line 122 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [026H].7
#line 122 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [026H].6
#line 125 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#pragma debug scope 7 1 ; 125 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L24
L23:
#line 126 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,ADC_GetChannelValue0
ORM A,__sadc1
L24:
#line 127 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [022H].7
#line 127 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [022H].7
#line 127 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [022H].7
L31:
L32:
#line 129 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SZ [022H].6
JMP L31
#line 130 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,__sadoh
MOV b0_5[1],A
CLR b0_5
MOV A,__sadol
MOV b1_5,A
CLR b1_5[1]
MOV A,b0_5
OR A,b1_5
MOV CR3,A
MOV A,b0_5[1]
OR A,b1_5[1]
MOV CR3[1],A
#line 131 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,CR3[1]
MOV RH,A
MOV A,CR3
L22:
RET
LOCAL b0_5 DB 2 DUP(?)
LOCAL b1_5 DB 2 DUP(?)
#line 111 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
LOCAL CR3 DB 2 DUP (?) ; m_adc
#pragma debug variable 12 7 CR3 m_adc
_ADC_GetChannelValue ENDP
#line 135 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
@MAIN .SECTION 'CODE'
_main PROC
begin:
#pragma debug scope 10 1 ; 136 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 136 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,0a8h
MOV __wdtc,A
#line 137 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [015H].5
#line 137 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [014H].5
#line 140 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [014H].0
CLR [015H].0
SET [016H].0
#line 141 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,0cch
MOV UART_SoftWareSendByte0,A
CALL _UART_SoftWareSendByte
#line 144 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _g_sysTime
MOV [04H],A
MOV A,OFFSET _g_sysTime
MOV [03H],A
CLR [02H]
INC [03H]
CLR [02H]
INC [03H]
CLR [02H]
INC [03H]
CLR [02H]
#line 145 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _flag1ms
MOV [04H],A
MOV A,OFFSET _flag1ms
MOV [03H],A
CLR [02H]
#line 146 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _cnt
MOV [04H],A
MOV A,OFFSET _cnt
MOV [03H],A
CLR [02H]
INC [03H]
CLR [02H]
#line 147 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _ntc_Adc
MOV [04H],A
MOV A,OFFSET _ntc_Adc
MOV [03H],A
CLR [02H]
INC [03H]
CLR [02H]
#line 150 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [023H].7
#line 150 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [023H].6
#line 150 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [023H].5
#line 151 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [023H].2
#line 151 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [023H].1
#line 151 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [023H].0
#line 152 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [022H].4
#line 153 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [024H].3
#line 153 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [024H].2
#line 153 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [024H].1
#line 153 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [024H].0
#line 154 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [022H].5
#line 157 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [01BH].6
#line 158 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [01BH].2
#line 158 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [01BH].1
#line 158 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [01BH].0
#line 159 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [01BH].7
#line 160 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
CLR [0EH].5
#line 161 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [0EH].2
#line 162 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
SET [0EH].0
#line 162 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L37
L36:
#pragma debug scope 11 10 ; 167 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 167 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,02h
MOV ADC_GetChannelValue0,A
CALL _ADC_GetChannelValue
MOV b0_6,A
MOV A,RH
MOV b0_6[1],A
MOV A,BANK _ntc_Adc
MOV [04H],A
MOV A,OFFSET _ntc_Adc
MOV [03H],A
MOV A,b0_6
MOV [02H],A
MOV A,b0_6[1]
INC [03H]
MOV [02H],A
#line 168 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _flag1ms
MOV [04H],A
MOV A,OFFSET _flag1ms
MOV [03H],A
MOV A,[02H]
XOR A,01h
SNZ [0AH].2
JMP L39
#pragma debug scope 12 11 ; 170 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 170 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _flag1ms
MOV [04H],A
MOV A,OFFSET _flag1ms
MOV [03H],A
CLR [02H]
#line 171 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _cnt
MOV [04H],A
MOV A,OFFSET _cnt
MOV [03H],A
MOV A,[02H]
MOV b0_6,A
INC [03H]
MOV A,[02H]
MOV b0_6[1],A
DEC [03H]
MOV A,1
ADDM A,[02H]
CLR [05H]
INC [03H]
ADCM A,[02H]
MOV A,03h
SUB A,b0_6[1]
SNZ [0AH].0
JMP L43
SNZ [0AH].2
JMP L41
MOV A,0e8h
SUB A,b0_6
SZ [0AH].0
JMP L41
L43:
#pragma debug scope 13 12 ; 174 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#line 174 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _cnt
MOV [04H],A
MOV A,OFFSET _cnt
MOV [03H],A
CLR [02H]
INC [03H]
CLR [02H]
L41:
L39:
#line 178 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#pragma debug scope 11 10 ; 178 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,0aah
MOV UART_SoftWareSendByte0,A
CALL _UART_SoftWareSendByte
#line 179 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,018H
CALL __DELAYX0
#line 180 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _ntc_Adc
MOV [04H],A
MOV A,OFFSET _ntc_Adc
MOV [03H],A
MOV A,[02H]
MOV b0_6,A
INC [03H]
MOV A,[02H]
MOV b0_6[1],A
MOV A,b0_6[1]
MOV b0_6,A
CLR b0_6[1]
MOV A,b0_6
MOV UART_SoftWareSendByte0,A
CALL _UART_SoftWareSendByte
#line 181 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,018H
CALL __DELAYX0
#line 182 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,BANK _ntc_Adc
MOV [04H],A
MOV A,OFFSET _ntc_Adc
MOV [03H],A
MOV A,[02H]
AND A,0ffh
MOV b0_6,A
CLR b0_6[1]
MOV A,b0_6
MOV UART_SoftWareSendByte0,A
CALL _UART_SoftWareSendByte
#line 184 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,0f3H
MOV RH,A
MOV A,0a9H
CALL __DELAYY1
#line 185 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
MOV A,0f3H
MOV RH,A
MOV A,0a9H
CALL __DELAYY1
L37:
#line 164 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
#pragma debug scope 10 1 ; 164 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP L36
L35:
#line 192 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
JMP $
LOCAL b0_6 DB 2 DUP(?)
_main ENDP
#line 133 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC _ntc_Adc
@ntc_Adc .SECTION 'DATA'
_ntc_Adc DB 2 DUP (?) ;ntc_Adc
#pragma debug variable 12 1 _ntc_Adc ntc_Adc
#line 11 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC _cnt
@cnt .SECTION 'DATA'
_cnt DB 2 DUP (?) ;cnt
#pragma debug variable 12 1 _cnt cnt
#line 10 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC _flag1ms
@flag1ms .SECTION 'DATA'
_flag1ms DB ? ; flag1ms
#pragma debug variable 8 1 _flag1ms flag1ms
#line 9 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
PUBLIC _g_sysTime
@g_sysTime .SECTION 'DATA'
_g_sysTime DB 4 DUP (?) ;g_sysTime
#pragma debug variable 14 1 _g_sysTime g_sysTime
#line 134 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
STARTSEC .SECTION  COM_L 'CODE'
STARTUP:
STARTSEC .SECTION  COM_E 'CODE'
RET
EXTERN RH:BYTE
EXTERN V2A:BYTE
EXTERN V2S:BYTE
EXTERN V2MP1:BYTE
EXTERN V2BP:BYTE
EXTERN __DELAYX0:NEAR
EXTERN __DELAYX1:NEAR
EXTERN __DELAYX2:NEAR
EXTERN __DELAYX3:NEAR
EXTERN __DELAYX4:NEAR
EXTERN __DELAYX5:NEAR
EXTERN __DELAYX6:NEAR
EXTERN __DELAYY0:NEAR
EXTERN __DELAYY1:NEAR
EXTERN __DELAYY2:NEAR
EXTERN __DELAYY3:NEAR
EXTERN __DELAYY4:NEAR
EXTERN __DELAYY5:NEAR
#line 134 "c:\users\thiet.dang\desktop\ntc binh nong_0025\NTC binh nong_0025.c"
@CODE .SECTION AT 0 'CODE'
CALL STARTUP
JMP begin
@DUMMY PROC
CALL _main
@DUMMY ENDP
