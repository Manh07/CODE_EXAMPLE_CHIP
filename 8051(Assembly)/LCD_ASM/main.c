#include <reg52.h>

// Define LCD control pins
sbit rs = P2^0;
sbit rw = P2^1;
sbit en = P2^2;

// Use P0 directly for LCD data (D4-D7)
#define LCD_DATA P0

// Function to provide delay (approx 1ms with 11.0592 MHz crystal)
void delay(unsigned int count)
{
    unsigned int i, j;
    for (i = 0; i < count; i++)
        for (j = 0; j < 112; j++);
}

// LCD16x2 command function
void LCD_Command(unsigned char cmnd)
{
    LCD_DATA = (LCD_DATA & 0x0F) | (cmnd & 0xF0); // Send upper nibble
    rs = 0;         // Command reg.
    rw = 0;         // Write operation
    en = 1;
    delay(1);
    en = 0;
    delay(2);

    LCD_DATA = (LCD_DATA & 0x0F) | (cmnd << 4); // Send lower nibble
    en = 1;         // Enable pulse
    delay(1);
    en = 0;
    delay(5);
}

// LCD data write function
void LCD_Char(unsigned char char_data)
{
    LCD_DATA = (LCD_DATA & 0x0F) | (char_data & 0xF0); // Send upper nibble
    rs = 1;         // Data reg.
    rw = 0;         // Write operation
    en = 1;
    delay(1);
    en = 0;
    delay(2);

    LCD_DATA = (LCD_DATA & 0x0F) | (char_data << 4); // Send lower nibble
    en = 1;         // Enable pulse
    delay(1);
    en = 0;
    delay(5);
}

// Send string to LCD function
void LCD_String(char *str)
{
    int i;
    for (i = 0; str[i] != 0; i++) // Send each char of string till the NULL
    {
        LCD_Char(str[i]); // Call LCD data write
    }
}

// Send string to LCD function
void LCD_String_xy(char row, char pos, char *str)
{
    if (row == 0)
        LCD_Command((pos & 0x0F) | 0x80);
    else if (row == 1)
        LCD_Command((pos & 0x0F) | 0xC0);
    LCD_String(str);     // Call LCD string function
}

// LCD Initialize function
void LCD_Init(void)
{
    delay(20);      // LCD Power ON Initialization time >15ms
    LCD_Command(0x02); // 4bit mode
    LCD_Command(0x28); // Initialization of 16X2 LCD in 4bit mode
    LCD_Command(0x0C); // Display ON Cursor OFF
    LCD_Command(0x06); // Auto Increment cursor
    LCD_Command(0x01); // clear display
    LCD_Command(0x80); // cursor at home position
}

void main()
{
    LCD_Init();     // Initialization of LCD
    LCD_String("Do Tri Manh"); // write string on 1st line of LCD
    LCD_Command(0xC0); // Go to 2nd line
    LCD_String_xy(1, 0, "Hello World"); // write string on 2nd line
    while (1);    // Infinite loop.
}