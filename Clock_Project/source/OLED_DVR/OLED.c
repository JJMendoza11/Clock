#include "Prototype.h"
#include "assert.h"
#include "stdlib.h"

#include "I2CDrive.h"
#include "OLED.h"
#include "OLED_def.h"


#define OLED_nSCREEN_SIZE		(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)
#define OLED_nMAX_BODY_LEN		(36U)
#define OLED_nSEND_INS			(0x00)
#define OLED_nSEND_Data			(0x40)

#define OLED_Msg_Len(body)		(body + 2)
#define OLED_Screen_Loc(x,y)	(x + (uint16)(y/8) * SSD1306_LCDWIDTH)
#define OLED_Bit_Loc(y)			((1 << (y & 7)))

typedef enum
{
	OLED_enAddr = 0,
	OLED_enCTRLByte,
	OLED_enWords,
	OLED_TotalCTRLBytes
}OLED_enMsgParts;


typedef struct
{
	const uint8 u8Address;
	const uint8 u8ControlByte;
	uint8 au8VirtualScreen[OLED_nSCREEN_SIZE];
}OLED_stScreenProp;

typedef struct
{
	const uint8 u8Address;
	uint8 u8ControlByte;
	uint8 au8Msg[OLED_nMAX_BODY_LEN];
}OLED_stBusMsg;

static OLED_stScreenProp stMyScreen = {SSD1306_I2C_ADDRESS,OLED_nSEND_Data,SSD1306_INIT_IMG};
static OLED_stBusMsg stMsg = {SSD1306_I2C_ADDRESS, OLED_nSEND_INS, SSD1306_INIT_CFG};
static uint16 xpos = 0;
static uint8 ypos = 0;
static uint8 Size = 1;

void _vDrawPixel(uint16 x, uint16 y, uint8 u8Color)
{
	if(u8Color == (uint8)True)
	{
		stMyScreen.au8VirtualScreen[(uint16)OLED_Screen_Loc(x,y)] |= (uint8)OLED_Bit_Loc(y);
	}
	else
	{
		stMyScreen.au8VirtualScreen[(uint16)OLED_Screen_Loc(x,y)] &= (uint8)~OLED_Bit_Loc(y);
	}
}

void _vDrawChar(uint8 c)
{
	_Bool _color;
	int i;
	int j;
	int line;

	if (c == ' ' && xpos == 0)
		return;
	if(c == '\a')
	{
		xpos = ypos = 0;
		return;
	}
	if( (c == '\b') && (xpos >= Size * 6) )
	{
		xpos -= Size * 6;
		return;
	}
	if(c == '\r')
	{
		xpos = 0;
		return;
	}
	if(c == '\n')
	{
		ypos += Size * 8;
		if((ypos + Size * 7) > SSD1306_LCDHEIGHT)
		{
			ypos = 0;
		}
		return;
	}
	if((c < ' ') || (c > '~'))
	{
		c = '?';
	}

	for(i = 0; i < 5; i++ ) {
		if(c < 'S')
		line = Font_Part_A[(c - ' ') * 5 + i];
		else
		line = Font_Part_B[(c - 'S') * 5 + i];

		for(j = 0; j < 7; j++, line >>= 1)
		{
			if(line & 0x01)
				_color = 1;
			else
				_color = 0;
			if(Size == 1) _vDrawPixel(xpos + i, ypos + j, _color);
			else OLED_vFillRect(xpos + (i * Size), ypos + (j * Size), Size, Size, _color);
		}
	}

	OLED_vFillRect(xpos + (5 * Size), ypos, Size, 7 * Size, 0);

	xpos += Size * 6;

	if( xpos > (SSD1306_LCDWIDTH + Size * 6) )
		xpos = SSD1306_LCDWIDTH;

	if (1 && (xpos + (Size * 5)) > SSD1306_LCDWIDTH)
	{
		xpos = 0;
		ypos += Size * 8;
		if((ypos + Size * 7) > SSD1306_LCDHEIGHT)
			ypos = 0;
	}
}

void OLED_vInit(void)
{
	uint8 u8Aux[] = SSD1306_INIT_CFG;
	uint16 u16Len = asizeof(u8Aux,uint8);
	uint8 i;
	for(i = 0; i < sizeof(u8Aux); i++)
	{
		stMsg.au8Msg[i] = u8Aux[i];
	}
	I2C_vfnSendData((uint8*)&stMsg, (uint16)OLED_Msg_Len(u16Len));
}

void OLED_vRefresh(void)
{
	stMsg.u8ControlByte = (uint8)OLED_nSEND_INS;
	stMsg.au8Msg[0] = (uint8)SSD1306_PAGEADDR;
	stMsg.au8Msg[1] = (uint8)0;
	stMsg.au8Msg[2] = (uint8)3;
	stMsg.au8Msg[3] = (uint8)SSD1306_COLUMNADDR;
	stMsg.au8Msg[4] = (uint8)0;
	stMsg.au8Msg[5] = 128 - 1;

	I2C_vfnSendData((uint8*)&stMsg, (uint16)8);

	I2C_vfnSendData((uint8*)&stMyScreen, (uint16)sizeof(stMyScreen));
}

void OLED_vPartialRefresh(uint8 u8Page1, uint8 u8Page2, uint16 u16Pxl1, uint16 u16Pxl2)
{
	uint16 u16Index = 0;
	uint16 u16Aux = u16Pxl1;
	uint16 u16MsgLen = asizeof(stMsg.au8Msg,uint8);

	stMsg.u8ControlByte = (uint8)OLED_nSEND_INS;
	stMsg.au8Msg[0] = (uint8)SSD1306_PAGEADDR;
	stMsg.au8Msg[1] = u8Page1;
	stMsg.au8Msg[2] = u8Page2;
	stMsg.au8Msg[3] = (uint8)SSD1306_COLUMNADDR;
	stMsg.au8Msg[4] = u16Pxl1;
	stMsg.au8Msg[5] = u16Pxl2;

	I2C_vfnSendData((uint8*)&stMsg, (uint16)8);

	while((u8Page1 <= u8Page2) && (u16MsgLen != 0))
	{
		while((u16Pxl1 <= u16Pxl2) && (u16MsgLen != 0))
		{
			stMsg.au8Msg[u16Index] = stMyScreen.au8VirtualScreen[u16Pxl1 + u8Page1 * SSD1306_LCDWIDTH];

			u16Index++;
			u16Pxl1++;
			u16MsgLen--;
		}
		u16Pxl1 = u16Aux;
		u8Page1++;
	}
	stMsg.u8ControlByte = (uint8)OLED_nSEND_Data;
	I2C_vfnSendData ((uint8*)&stMsg, (uint16)OLED_Msg_Len(u16Index));
}

void OLED_vClearScreen(void)
{
	uint16 i;

	for(i = 0; i < OLED_nSCREEN_SIZE; i++)
	{
	stMyScreen.au8VirtualScreen[i] = 0;
	}
}

void OLED_vDrawRectangle(uint16 x, uint16 y, uint8 b, uint8 a)
{
	OLED_vDrawHLine(x, y, x + b, (uint8)True);
	OLED_vDrawVLine(x, y, a, (uint8)True);
	OLED_vDrawHLine(x, y + a - 1, x + b, (uint8)True);
	OLED_vDrawVLine(x + b - 1, y, a, (uint8)True);
}

void OLED_vFillRect(uint16 x, uint16 y, uint16 b, uint16 a, uint8 u8Color)
{
	uint16 i;
	uint16 j;

	for(j = y; j < (uint16)(y + a); j++)
	{
		for(i = x; i < (uint16)(x + b); i++)
		{
			_vDrawPixel(i,j, u8Color);
		}
	}
}

void OLED_vGotoxy(uint16 x, uint8 y)
{
	assert(x < SSD1306_LCDWIDTH);
	assert(y < SSD1306_LCDHEIGHT);

	xpos = x;
	ypos = y;
}

void OLED_vSetSize(uint8 u8Size)
{
	Size = u8Size;
}

void OLED_vDrawHLine(uint16 x, uint16 y, uint16 l, uint8 u8Color)
{
	for(; x <= (uint16)(l-1); x++)
	{
		_vDrawPixel(x, y, u8Color);
	}
}

void OLED_vDrawVLine(uint16 x, uint16 y, uint16 l, uint8 u8Color)
{
	for(; y <= (uint16)(l-1); y++)
	{
		_vDrawPixel(x, y, u8Color);
	}
}

void OLED_vInverse(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	for(; x0 <= x1; x0++){
		for(uint8_t j = y0; j <= y1; j++){
			stMyScreen.au8VirtualScreen[(uint16)OLED_Screen_Loc(x0,j)] ^= (uint8)OLED_Bit_Loc(j);
		}
	}
}

void OLED_vPrint(const uint8* C, uint16 u16Len)
{
	for (uint8 i = 0; i < u16Len; i++)
	{
		_vDrawChar(C[i]);
	}
}

void OLED_vInvDisplay(uint8 u8Invert)
{
	stMsg.u8ControlByte = (uint8)OLED_nSEND_INS;
	if(u8Invert == (uint8)False)
	{
		stMsg.au8Msg[0] = SSD1306_INVERTDISPLAY;
	}
	else
	{
		stMsg.au8Msg[0] = SSD1306_NORMALDISPLAY;
	}

	I2C_vfnSendData ((uint8*)&stMsg, (uint16)OLED_Msg_Len(1));
}




