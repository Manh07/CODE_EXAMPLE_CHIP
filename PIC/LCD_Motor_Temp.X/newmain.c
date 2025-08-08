#pragma config FOSC = HS   // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF  // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON  // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON    // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF   // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF    // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 20000000
#include <xc.h>
#include <stdio.h>

// ??nh ngh?a ch�n LCD
#define RS RB0
#define EN RB1
#define D4 RB2
#define D5 RB3
#define D6 RB4
#define D7 RB5
#define IN1 RB6
#define IN2 RB7
#define Button_UP RD0
#define Button_DOWN RD1
#define Button_MODE RD2
#define TMR2PRESCALE 4

long PWM_freq = 5000;
unsigned int speed_percent = 100; // Tốc độ mặc định 50%
float pwm_value = 1023;           // Giá trị PWM tương ứng 50% (10 bit)

void LCD_Command(unsigned char cmd);
void LCD_Char(unsigned char data);
void LCD_Init(void);
void LCD_String(const char *str);
void LCD_Enable(void);
void LCD_Clear();
unsigned int ADC_Read(unsigned char channel);
void ADC_Initialize(void);
void PWM_Initialize();
void PWM_Duty(unsigned int duty);

void main(void)
{
    TRISB = 0x00;     // LCD output
    TRISC = 0x00;     // RC2 output (PWM)
    TRISA = 0xFF;     // PORTA input (cho ADC)
    TRISD = 0xFF;     // RD0, RD1, RD2 input (nút nhấn)
    ADC_Initialize(); // Khởi tạo ADC
    PWM_Initialize(); // Khởi tạo PWM
    LCD_Init();
    IN1 = 1;
    IN2 = 0; // Quay thuận

    LCD_String("START");
    __delay_ms(100);
    LCD_Clear();

    unsigned char last_speed_percent = 255;
    float last_temp = -1000;
    unsigned char mode = 0;      // 0: nút nhấn, 1: theo nhiệt độ
    unsigned char last_mode = 2; // để đảm bảo cập nhật LCD lần đầu

    while (1)
    {
        // Đọc nhiệt độ từ LM35 (giả sử kênh AN4)
        unsigned int adc_value = ADC_Read(4);
        float temp = adc_value * 5.0 / 1023 * 100; // LM35: 10mV/°C, Vref=5V

        // Chuyển mode khi nhấn RD2
        if (Button_MODE == 0)
        {
            __delay_ms(20); // chống dội phím
            if (Button_MODE == 0)
            {
                mode = !mode;
                while (Button_MODE == 0)
                    ; // chờ nhả nút
            }
        }

        if (mode == 0)
        {
            // Điều khiển bằng nút nhấn
            if (Button_UP == 0)
            {
                if (speed_percent < 100)
                    speed_percent += 20;
                if (speed_percent > 100)
                    speed_percent = 100;
                __delay_ms(50);
            }
            if (Button_DOWN == 0)
            {
                if (speed_percent > 0)
                    speed_percent -= 20;
                if (speed_percent < 0)
                    speed_percent = 0;
                __delay_ms(50);
            }
        }
        else
        {
            // Điều khiển theo nhiệt độ
            if (temp < 25)
                speed_percent = 20;
            else if (temp < 30)
                speed_percent = 40;
            else if (temp < 35)
                speed_percent = 60;
            else if (temp < 40)
                speed_percent = 80;
            else
                speed_percent = 100;
        }

        pwm_value = (speed_percent / 100.0) * 1023;
        PWM_Duty((int)pwm_value);

        // Chỉ cập nhật LCD khi speed_percent, temp hoặc mode thay đổi
        if ((last_speed_percent != speed_percent) || (temp - last_temp > 0.5f) || (last_temp - temp > 0.5f) || (last_mode != mode))
        {
            last_speed_percent = speed_percent;
            last_temp = temp;
            last_mode = mode;

            LCD_Clear();
            if (mode == 0)
                LCD_String("Mode: Manual");
            else
                LCD_String("Mode: Temp");

            LCD_Command(0xC0); // Xuống dòng 2
            LCD_String("Spd:");
            char buf[8];
            sprintf(buf, "%d%%", speed_percent);
            LCD_String(buf);

            LCD_String(" T:");
            char tbuf[8];
            sprintf(tbuf, "%dC", (int)temp);
            LCD_String(tbuf);
        }
    }
}

// H�m t?o xung cho ch�n Enable
void LCD_Enable(void)
{
    EN = 1;
    __delay_ms(1);
    EN = 0;
    __delay_ms(1);
}

// G?i l?nh ??n LCD
void LCD_Command(unsigned char cmd)
{
    RS = 0; // Ch? ?? l?nh

    // G?i 4 bit cao
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    LCD_Enable();

    // G?i 4 bit th?p
    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    LCD_Enable();
}

// G?i k� t? ??n LCD
void LCD_Char(unsigned char data)
{
    RS = 1; // Ch? ?? d? li?u

    // G?i 4 bit cao
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    LCD_Enable();

    // G?i 4 bit th?p
    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    LCD_Enable();
}

// Hi?n th? chu?i
void LCD_String(const char *str)
{
    while (*str)
    {
        LCD_Char(*str++);
    }
}

// X�a m�n h�nh
void LCD_Clear()
{
    LCD_Command(0x01); // Clear display
    __delay_ms(1);
}

// Kh?i t?o LCD
void LCD_Init(void)
{
    __delay_ms(1); // Ch? LCD kh?i ??ng

    LCD_Command(0x02); // Tr? v? ch? ?? home
    LCD_Command(0x28); // Giao ti?p 4 bit, 2 d�ng, font 5x8
    LCD_Command(0x0C); // B?t LCD, t?t con tr?
    LCD_Command(0x06); // T?ng ??a ch? DDRAM, kh�ng d?ch m�n h�nh
    LCD_Command(0x01); // X�a m�n h�nh
    __delay_ms(1);
}

void ADC_Initialize(void)
{
    ADCON0 = 0b00010001; // Chọn kênh AN4, ADC ON, Fosc/8
    ADCON1 = 0b11000000; // Chọn điện áp tham chiếu trong, AN4 là analog
}

unsigned int ADC_Read(unsigned char channel)
{
    ADCON0 &= 0x11000101;     // Xóa bit chọn kênh
    ADCON0 |= (channel << 3); // Chọn kênh
    __delay_ms(1);            // Thời gian nạp tụ
    GO_nDONE = 1;
    while (GO_nDONE)
        ;
    return ((ADRESH << 8) + ADRESL);
}

void PWM_Initialize()
{
    PR2 = (_XTAL_FREQ / (PWM_freq * 4 * TMR2PRESCALE)) - 1; // Setting the PR2 formulae using Datasheet // Makes the PWM work in 5KHZ
    CCP1M3 = 1;
    CCP1M2 = 1; // Configure the CCP1 module
    T2CKPS0 = 1;
    T2CKPS1 = 0;
    TMR2ON = 1; // Configure the Timer module
    TRISC2 = 0; // make port pin on C as output
}

void PWM_Duty(unsigned int duty)
{
    if (duty <= 1023)
    {
        duty = ((float)duty / 1023) * (_XTAL_FREQ / (PWM_freq * TMR2PRESCALE)); // On reducing //duty = (((float)duty/1023)*(1/PWM_freq)) / ((1/_XTAL_FREQ) * TMR2PRESCALE);
        CCP1X = duty & 1;                                                       // Store the 1st bit
        CCP1Y = duty & 2;                                                       // Store the 0th bit
        CCPR1L = duty >> 2;                                                     // Store the remining 8 bit
    }
}