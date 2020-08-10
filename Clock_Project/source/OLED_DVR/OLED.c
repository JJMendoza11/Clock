#include <I2C_DVR/I2CDrive.h>
#include <OLED_DVR/OLED.h>
#include <OLED_DVR/OLED_def.h>
#include "Prototype.h"
#include "stdlib.h"




#define OLED_WRITE_INS			(0x00)
#define OLED_WRITE_DATA			(0x40)
#define OLED_DATA_LENGHT		(30U)
#define OLED_BUS_LENGHT(x)		(x+2)
#define OLED_Pixel_Num(x)		(x-1)
#define OLED_SCREEN_BUFFER		(SSD1306_LCDWIDTH*SSD1306_LCDHEIGHT/8)

#define Message_Leght 3
typedef struct
{
	uint8 u8Address;
	uint8 u8ControlByte;
	uint8 au8Instruction[OLED_DATA_LENGHT];
}OLED_stBusMsg;
typedef struct
{
	uint8 u8Address;
	uint8 u8Writebyte;
	uint8 u8SceenBuffer[512];
}OLED_stDrawMsg;

enum{
	enAddressByte = 0,
	enControlByte,
	enCommandByte,
	enCompleteMessage
};

static OLED_stBusMsg OLED_stSSD1306 = {0, 0, SSD1306_INIT_CFG};
static OLED_stDrawMsg OLED_stScreenProps = {SSD1306_I2C_ADDRESS, OLED_WRITE_DATA, SSD1306_INIT_IMG};
static uint8_t  x_pos = 0;
static uint8_t  y_pos = 0;
static uint8_t text_size = 0;

void OLED_vInit(void)
{
	uint8 au8AuxMsg[] = SSD1306_INIT_CFG;
	uint8 u8Len = asizeof(au8AuxMsg,uint8);
	uint8 u8Val = (uint8)OK;
	uint8 i;

	while((i<u8Len) && (u8Val == (uint8)OK))
	{
		u8Val = ((OLED_stSSD1306.au8Instruction[i] == au8AuxMsg[i]) ? u8Val : (uint8)False);
		i++;
	}
	if(u8Val == (uint8)OK)
	{
		OLED_stSSD1306.u8Address = (uint8)SSD1306_I2C_ADDRESS;
		OLED_stSSD1306.u8ControlByte = (uint8)0x00;
		I2C_vfnSendData ((uint8*)&OLED_stSSD1306, (uint16)OLED_BUS_LENGHT(u8Len));
	}
	else
	{
		/*Nothing to do. */
	}

}

void SSD1306_FullDisplay(void)
{
	uint16 u16Len =(uint16)sizeof(OLED_stScreenProps);

	OLED_stSSD1306.u8Address = (uint8)SSD1306_I2C_ADDRESS;
	OLED_stSSD1306.u8ControlByte = (uint8)OLED_WRITE_INS;
	OLED_stSSD1306.au8Instruction[0] = (uint8)SSD1306_PAGEADDR;
	OLED_stSSD1306.au8Instruction[1] = (uint8)OLED_enPage1;
	OLED_stSSD1306.au8Instruction[2] = (uint8)OLED_enPage4;
	OLED_stSSD1306.au8Instruction[3] = (uint8)SSD1306_COLUMNADDR;
	OLED_stSSD1306.au8Instruction[4] = 0x00;
	OLED_stSSD1306.au8Instruction[5] = (uint8)OLED_Pixel_Num(SSD1306_LCDWIDTH);

	I2C_vfnSendData ((uint8*)&OLED_stSSD1306, 8);

	OLED_stScreenProps.u8Address = (uint8)SSD1306_I2C_ADDRESS;
	OLED_stScreenProps.u8Writebyte = (uint8)0x40;

	I2C_vfnSendData ((uint8*)&OLED_stScreenProps, u16Len);
}

void SSD1306_ShrtDisplay(uint8 u8P1, uint8 u8P2, uint8 u8Px1, uint8 u8Px2)
{
//	uint16 u16Len =(uint16)OLED_BUS_LENGHT(u8Px2 - u8Px1);
	uint16 i = 0;
	uint8 u8Aux = u8Px1;

	OLED_stSSD1306.u8Address = (uint8)SSD1306_I2C_ADDRESS;
	OLED_stSSD1306.u8ControlByte = (uint8)OLED_WRITE_INS;
	OLED_stSSD1306.au8Instruction[0] = (uint8)SSD1306_PAGEADDR;
	OLED_stSSD1306.au8Instruction[1] = u8P1;
	OLED_stSSD1306.au8Instruction[2] = u8P2;
	OLED_stSSD1306.au8Instruction[3] = (uint8)SSD1306_COLUMNADDR;
	OLED_stSSD1306.au8Instruction[4] = u8Px1;
	OLED_stSSD1306.au8Instruction[5] = u8Px2;

	I2C_vfnSendData ((uint8*)&OLED_stSSD1306, 8);

	OLED_stSSD1306.u8ControlByte = (uint8)OLED_WRITE_DATA;
	do
	{
		for(; u8Px1 <= u8Px2; u8Px1++)
		{
			OLED_stSSD1306.au8Instruction[i] = OLED_stScreenProps.u8SceenBuffer[u8Px1 + (uint16)(SSD1306_LCDWIDTH*u8P1)];
			i++;
		}
		u8Px1 = u8Aux;
		u8P1++;
	}while(u8P1 <= u8P2);
	I2C_vfnSendData ((uint8*)&OLED_stSSD1306, (uint16)OLED_BUS_LENGHT(i));
}

void SSD1306_ClearDisplay(void)
{
	uint16 i;
	for (i = 0; i < (uint16)OLED_SCREEN_BUFFER; i++){
		OLED_stScreenProps.u8SceenBuffer[i] = 0;
	}
}

void SSD1306_DrawPixel(uint8 x, uint8 y, _Bool color)
{
	  if (color)
	    OLED_stScreenProps.u8SceenBuffer[x + (uint16)(y / 8) * SSD1306_LCDWIDTH] |=  (1 << (y & 7));
	  else
		  OLED_stScreenProps.u8SceenBuffer[x + (uint16)(y / 8) * SSD1306_LCDWIDTH] &=  ~(1 << (y & 7));
}

void SSD1306_DrawLine(uint8 x0, uint8 y0, uint8 x1, uint8 y1, _Bool color)
{
	  _Bool steep;
	  char ystep;
	  uint8 dx, dy;
	  uint16 err;
	  steep = abs(y1 - y0) > abs(x1 - x0);
	  if (steep) {
	    ssd1306_swap(x0, y0);
	    ssd1306_swap(x1, y1);
	  }
	  if (x0 > x1) {
	    ssd1306_swap(x0, x1);
	    ssd1306_swap(y0, y1);
	  }
	  dx = x1 - x0;
	  dy = abs(y1 - y0);

	  err = dx / 2;
	  if (y0 < y1)
	    ystep = 1;
	  else
	    ystep = -1;

	  for (; x0 <= x1; x0++)
	  {
	    if (steep)
	    {
	      if(color) SSD1306_DrawPixel(y0, x0, 1);
	      else      SSD1306_DrawPixel(y0, x0, 0);
	    }
	    else
	    {
	      if(color) SSD1306_DrawPixel(x0, y0, 1);
	      else      SSD1306_DrawPixel(x0, y0, 0);
	    }
	    err -= dy;
	    if (err < 0) {
	      y0  += ystep;
	      err += dx;
	    }
	  }
}

void SSD1306_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, _Bool color)
{
   SSD1306_DrawLine(x, y, x + w - 1, y, color);
}

void SSD1306_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, _Bool color)
{
  SSD1306_DrawLine(x, y, x, y + h - 1, color);
}


void SSD1306_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, _Bool color)
{
  for (int16_t i = x; i < x + w; i++)
    SSD1306_DrawFastVLine(i, y, h, color);
}

void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r)
{
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  SSD1306_DrawPixel(x0  , y0 + r, 1);
  SSD1306_DrawPixel(x0  , y0 - r, 1);
  SSD1306_DrawPixel(x0 + r, y0, 1);
  SSD1306_DrawPixel(x0 - r, y0, 1);

  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    SSD1306_DrawPixel(x0 + x, y0 + y, 1);
    SSD1306_DrawPixel(x0 - x, y0 + y, 1);
    SSD1306_DrawPixel(x0 + x, y0 - y, 1);
    SSD1306_DrawPixel(x0 - x, y0 - y, 1);
    SSD1306_DrawPixel(x0 + y, y0 + x, 1);
    SSD1306_DrawPixel(x0 - y, y0 + x, 1);
    SSD1306_DrawPixel(x0 + y, y0 - x, 1);
    SSD1306_DrawPixel(x0 - y, y0 - x, 1);
  }

}

void SSD1306_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h)
{
  SSD1306_DrawFastHLine(x, y, w, 1);
  SSD1306_DrawFastHLine(x, y + h - 1, w, 1);
  SSD1306_DrawFastVLine(x, y, h, 1);
  SSD1306_DrawFastVLine(x + w - 1, y, h, 1);
}

void SSD1306_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  SSD1306_DrawLine(x0, y0, x1, y1, 1);
  SSD1306_DrawLine(x1, y1, x2, y2, 1);
  SSD1306_DrawLine(x2, y2, x0, y0, 1);
}

void SSD1306_GotoXY(uint8_t x, uint8_t y)
{
  if((x >= SSD1306_LCDWIDTH) || (y >= SSD1306_LCDHEIGHT))
    return;
  x_pos = x;
  y_pos = y;
}

void SSD1306_TextSize(uint8_t t_size)
{
  if(t_size < 1)
    t_size = 1;
  text_size = t_size;
}

void SSD1306_Print(uint8_t c)
{
  _Bool _color;
  uint8_t i, j, line;

  if (c == ' ' && x_pos == 0)
    return;
  if(c == '\a') {
    x_pos = y_pos = 0;
    return;
  }
  if( (c == '\b') && (x_pos >= text_size * 6) ) {
    x_pos -= text_size * 6;
    return;
  }
  if(c == '\r') {
    x_pos = 0;
    return;
  }
  if(c == '\n') {
    y_pos += text_size * 8;
    if((y_pos + text_size * 7) > SSD1306_LCDHEIGHT)
      y_pos = 0;
    return;
  }

  if((c < ' ') || (c > '~'))
    c = '?';

  for(i = 0; i < 5; i++ ) {
    if(c < 'S')
      line = Font_Part_A[(c - ' ') * 5 + i];
    else
      line = Font_Part_B[(c - 'S') * 5 + i];

    for(j = 0; j < 7; j++, line >>= 1) {
      if(line & 0x01)
        _color = 1;
      else
        _color = 0;
      if(text_size == 1) SSD1306_DrawPixel(x_pos + i, y_pos + j, _color);
      else               SSD1306_FillRect(x_pos + (i * text_size), y_pos + (j * text_size), text_size, text_size, _color);
    }
  }

  SSD1306_FillRect(x_pos + (5 * text_size), y_pos, text_size, 7 * text_size, 0);

  x_pos += text_size * 6;

  if( x_pos > (SSD1306_LCDWIDTH + text_size * 6) )
    x_pos = SSD1306_LCDWIDTH;

  if (1 && (x_pos + (text_size * 5)) > SSD1306_LCDWIDTH)
  {
    x_pos = 0;
    y_pos += text_size * 8;
    if((y_pos + text_size * 7) > SSD1306_LCDHEIGHT)
      y_pos = 0;
  }
}

void SSD1306_DrawText(uint8_t x, uint8_t y, char *_text, uint8_t size)
{
  SSD1306_GotoXY(x, y);
  SSD1306_TextSize(size);
  while(*_text != '\0')
    SSD1306_Print(*_text++);

}

#define pgm_read_byte(addr) \
 (*(const unsigned char *)(addr))

void SSD1306_ROMBMP(uint8_t x, uint8_t y, uint8_t *bitmap, uint8_t w, uint8_t h)
{
	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	for(int16_t j=0; j<h; j++, y++) {
	    for(int16_t i=0; i<w; i++) {
	        if(i & 7) byte <<= 1;
	        else      byte   = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
	        if(byte & 0x80) SSD1306_DrawPixel(x+i, y, 1);
	    }
	}
}

void SSD1306_INVERSE(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0){
	for(; x <= x0; x++){
		for(uint8_t j = y; j <= y0; j++){
			OLED_stScreenProps.u8SceenBuffer[x + (j/8)*SSD1306_LCDWIDTH ] ^=  (1 << (j&7));
		}
	}
}

void SSD1306_StopScroll(void)
{
	OLED_stSSD1306.u8Address = (uint8)SSD1306_I2C_ADDRESS;
	OLED_stSSD1306.u8ControlByte = (uint8)OLED_WRITE_INS;
	OLED_stSSD1306.au8Instruction[0] = (uint8)SSD1306_DEACTIVATE_SCROLL;
	I2C_vfnSendData ((uint8*)&OLED_stSSD1306, 3);
}


