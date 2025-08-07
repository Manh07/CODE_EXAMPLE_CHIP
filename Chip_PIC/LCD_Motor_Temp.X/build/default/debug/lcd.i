# 1 "lcd.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 285 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v3.00\\pic\\include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "lcd.c" 2
# 72 "lcd.c"
typedef struct
{
   BOOLEAN enable;
   BOOLEAN rs;
   BOOLEAN rw;
   BOOLEAN unused;
   int data : 4;



} LCD_PIN_MAP;
# 178 "lcd.c"
BYTE const LCD_INIT_STRING[4] = {0x20 | (2 << 2), 0xc, 1, 6};



BYTE lcd_read_nibble(void);

BYTE lcd_read_byte(void)
{
   BYTE low,high;





   output_float(PIN_D4);
   output_float(PIN_D5);
   output_float(PIN_D6);
   output_float(PIN_D7);





   output_bit(PIN_E2, 1);
   delay_cycles(1);
   output_bit(PIN_E0, 1);
   delay_cycles(1);
   high = lcd_read_nibble();

   output_bit(PIN_E0, 0);
   delay_cycles(1);
   output_bit(PIN_E0, 1);
   delay_us(1);
   low = lcd_read_nibble();

   output_bit(PIN_E0, 0);





   output_drive(PIN_D4);
   output_drive(PIN_D5);
   output_drive(PIN_D6);
   output_drive(PIN_D7);





   return( (high<<4) | low);
}

BYTE lcd_read_nibble(void)
{

   BYTE n = 0x00;


   n |= input(PIN_D4);
   n |= input(PIN_D5) << 1;
   n |= input(PIN_D6) << 2;
   n |= input(PIN_D7) << 3;

   return(n);



}

void lcd_send_nibble(BYTE n)
{


   output_bit(PIN_D4, bit_test(n, 0));
   output_bit(PIN_D5, bit_test(n, 1));
   output_bit(PIN_D6, bit_test(n, 2));
   output_bit(PIN_D7, bit_test(n, 3));




   delay_cycles(1);
   output_bit(PIN_E0, 1);
   delay_us(2);
   output_bit(PIN_E0, 0);
}

void lcd_send_byte(BYTE address, BYTE n)
{
   output_bit(PIN_E1, 0);
   while ( bit_test(lcd_read_byte(),7) ) ;
   output_bit(PIN_E1, address);
   delay_cycles(1);
   output_bit(PIN_E2, 0);
   delay_cycles(1);
   output_bit(PIN_E0, 0);
   lcd_send_nibble(n >> 4);
   lcd_send_nibble(n & 0xf);
}

void lcd_init(void)
{
   BYTE i;





   output_drive(PIN_D4);
   output_drive(PIN_D5);
   output_drive(PIN_D6);
   output_drive(PIN_D7);



   output_drive(PIN_E0);
   output_drive(PIN_E1);
   output_drive(PIN_E2);


   output_bit(PIN_E1, 0);
   output_bit(PIN_E2, 0);
   output_bit(PIN_E0, 0);

   delay_ms(15);
   for(i=1;i<=3;++i)
   {
       lcd_send_nibble(3);
       delay_ms(5);
   }

   lcd_send_nibble(2);
   for(i=0;i<=3;++i)
      lcd_send_byte(0,LCD_INIT_STRING[i]);
}

void lcd_gotoxy(BYTE x, BYTE y)
{
   BYTE address;

   if(y!=1)
      address=0x40;
   else
      address=0;

   address+=x-1;
   lcd_send_byte(0,0x80|address);
}

void lcd_putc(char c)
{
   switch (c)
   {
      case '\f' : lcd_send_byte(0,1);
                     delay_ms(2);
                     break;

      case '\n' : lcd_gotoxy(1,2); break;

      case '\b' : lcd_send_byte(0,0x10); break;

      default : lcd_send_byte(1,c); break;
   }
}

char lcd_getc(BYTE x, BYTE y)
{
   char value;

   lcd_gotoxy(x,y);
   while ( bit_test(lcd_read_byte(),7) );
   output_bit(PIN_E1, 1);
   value = lcd_read_byte();
   output_bit(PIN_E1, 0);

   return(value);
}
