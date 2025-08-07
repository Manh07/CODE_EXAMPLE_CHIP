#include "HT66F0025.h"
#include "HT8_Type.h"
#define DEBUG
#define RL_OUPUT (_pac5 = 0)
#define RL_ON	(_pa5 = 1)
#define RL_OFF (_pa5 = 0)

#pragma vector TB_ISR @ 0x08
u32 g_sysTime;
u8 flag1ms; 
u16 cnt;

void TB_ISR(void)
{
    /* user define */
    g_sysTime++;
    flag1ms = 1;    
    _nop();
}

#ifdef DEBUG
	#define 	TXD		_pa0			//TX pin mapping on PA7	
	#define 	TXC		_pac0			//TX pin input/output control
	#define 	TXPU	_papu0			//TX pin pull-high control
	#define		SIMULATION_UART_INIT()	TXD=1,TXC=0,TXPU=1	
	#define BAUDRATE	193		// baud rate 9600,191~195
void UART_SoftWareSendByte(u8 TxData)
{
	u8 i;
	TXD = 0;					//start bit
	_delay(BAUDRATE);
	for(i=0;i<8;i++)			//send 8-bit data
	{	
	
		if(TxData & 0x01)TXD = 1;
		else TXD = 0;
		_delay(BAUDRATE);
		TxData >>= 1;
	}
	TXD = 1;					//stop bit
	_delay(BAUDRATE);
}
#endif
void EEPROM_Write_Byte(u8 Data,u8 adr)
{
	/*config EEPROM init*/
	_dmbp0 = 1;			
	_emi = 0;		//disable global interrupt
	_mp1 = 0x40;
	
	/*config EEPROM address*/
	_eea = adr;	
	/*Write data*/
	_eed = Data;	
		
	/*config EEPROM Write mode*/
	_iar1 |= 0x08;
	_iar1 |= 0x04;
		
	/* waitting for EEPROM write finishing */
	while(_iar1 & 0x04);
	_iar1 = 0;
	_dmbp0 = 0;
	_emi = 1;		//enable global interrupt			
	return;
}
/**
  * @brief EEPROM read function.
  * @param[in] Specifies EEPROM address that you want to read.
  * It can be 0x00~0x1f.
  * @retval EEPROM data.
  */
u8 EEPROM_Read_Byte(u8 adr)
{
	/* config EEPROM init */
	u8 EP_Read_Data;
	_dmbp0 = 1;
	_mp1 = 0x40;
	
	/*config EEPROM address*/
	_eea = adr;
	
	/*config EEPROM Read mode*/
	_iar1 |= 0x02;
	_iar1 |= 0x01;
	
	/* waitting for EEPROM Read finishing */
	while(_iar1 & 0x01);
	_iar1 = 0;
	_dmbp0 = 0;
		
	EP_Read_Data = _eed;		//Read data
	return EP_Read_Data;		
}
typedef enum 
{
	ADC_CH0  = (u8)0x00, /**< Analog channel 0 */
	ADC_CH1  = (u8)0x01, /**< Analog channel 1 */
	ADC_CH2  = (u8)0x02, /**< Analog channel 2 */
	ADC_CH3  = (u8)0x03, /**< Analog channel 3 */
	ADC_CH_INTERNAL_VDD		 = (u8)0x20,
	ADC_CH_INTERNAL_VDD_DIV2 = (u8)0x40,
	ADC_CH_INTERNAL_VDD_DIV4 = (u8)0x60,
	ADC_CH_INTERNAL_VR		 = (u8)0xa0,
	ADC_CH_INTERNAL_VR_DIV2	 = (u8)0xc0,
	ADC_CH_INTERNAL_VR_DIV4	 = (u8)0xe0,   
} ADC_Channel_TypeDef;

u16 ADC_GetChannelValue(u8 ADC_Channel)
{
	u16 m_adc;
	_sadc1 &= 0b00011111;
	if(ADC_Channel < 4)
	{
		_sadc0 &= 0b11110000; /* clear ADC channel */
		_sadc0 |= ADC_Channel; /* selection external ADC channel */
		switch(ADC_Channel) /* select IO in AD input */
		{
			case ADC_CH0 : _pas1 = 1; _pas0 = 1; break;
			case ADC_CH1 : _pas3 = 1; _pas2 = 1; break;
			case ADC_CH2 : _pas5 = 1; _pas4 = 1; break;
			case ADC_CH3 : _pas7 = 1; _pas6 = 1; break;	
			default : break;
		}		
	}
	else _sadc1 |= ADC_Channel;		/* selection internal ADC channel */
	_start = 0; _start = 1; _start = 0; /* start AD convert */
	/* waitting AD conversion finish */	
	while(1 == _adbz);	
	m_adc = ((u16)_sadoh << 8) | _sadol;
	return m_adc;
}
u16 ntc_Adc;		//AD conversion value   -->adc VR2
void main()
{
	_wdtc = 0b10101000;		//disable watchdog timer
    RL_OUPUT; 
    RL_OFF;
#ifdef	DEBUG
/* Debug mode can be enabled by uncommenting "DEBUG" Macro in HT8_MCU_IP_SEL.h */
	SIMULATION_UART_INIT();
	UART_SoftWareSendByte(0xcc);
#endif

	g_sysTime = 0;
	flag1ms = 0;
	cnt = 0;
	ntc_Adc = 0;

	//adc	
	_sains2 = 0; _sains1 = 0; _sains0 = 0;
	_sack2 = 0; _sack1 = 1; _sack0 = 1;
	_adrfs = 1;
    _savrs3 = 0; _savrs2 = 0; _savrs1 = 0; _savrs0 = 0;      
	_enadc = 1;
	
	//timebase
	_tbck = 1;
	_tb02 = 0;	_tb01 = 1;	_tb00 = 1;
	_tbon = 1;
	_tb0f = 0;
	_tb0e = 1;
	_emi = 1;

	while (1)
	{
		//g_nAdc_VR2 = ADC_GetChannelValue(ADC_CH1);	//PA2	//v_load-VR2
		ntc_Adc  = ADC_GetChannelValue(ADC_CH2);	//PA7	//v_in  -VR1
		if(flag1ms == 1)
		{
			flag1ms = 0;
			if(cnt++ > 1000)
			{
				cnt = 0;
			}	
		}
	#ifdef DEBUG	
		UART_SoftWareSendByte(0xaa);						//send head code to PC
		_delay(100);
		UART_SoftWareSendByte(ntc_Adc>>8);		//send AD conversion value high byte to PC
		_delay(100);
		UART_SoftWareSendByte(ntc_Adc&0xff);		//send AD conversion value low byte to PC

		_delay(250000);
		_delay(250000);
	#endif



	}
	
}