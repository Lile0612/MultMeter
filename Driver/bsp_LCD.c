/*! @file
********************************************************************************
<PRE>
模块名       : 驱动模块
文件名       : drvLED.c
相关文件     :
文件实现功能 : LED驱动程序
作者         : <--->
版本         : 1.0
--------------------------------------------------------------------------------
备注         :
--------------------------------------------------------------------------------
修改记录 :
日 期        版本   修改人         修改内容
2012/11/12   1.0    <xxxx>         创建
</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxx>, 保留所有权利

*******************************************************************************/
#ifdef _GUILCD_

/* Includes ------------------------------------------------------------------*/
//#include "drvI2C.h"
//#include "drvLCD.h"
#include "Include.h"


/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BIAS	0x30  // 内部RC时钟
#define SYSEN	0x02
#define LCDON	0x06
#define LCDOFF	0x04
#define TONE4K 0x80
#define TONE2K 0xA0

#define DISPNUM	92

enum emLCD_DISP_ADDR
{
	LCD_DISP_ADDR_COS = 0x0001,
	LCD_DISP_ADDR_COM = 0x0002,
	LCD_DISP_ADDR_DLY = 0x0004,
	LCD_DISP_ADDR_ONPF = 0x0008,
	LCD_DISP_ADDR_Q = 0x0101,
	LCD_DISP_ADDR_P = 0x0102,
	LCD_DISP_ADDR_I = 0x0104,
	LCD_DISP_ADDR_U = 0x0108,

};




// 第0行数字 16段  le
// 0 1 2 3 4 5 6 7 8 9
const unsigned char LCD_COM_Row0_Value[][4] =
            {{0xA0,0x30,0x90,0x30},{0x00,0x00,0x10,0x20},{0x90,0x30,0x90,0x90},{0x90,0x10,0x90,0xB0},// 0 1 2 3
            {0x30,0x00,0x10,0xA0},{0xB0,0x10,0x80,0xB0},{0xB0,0x30,0x80,0xB0},{0x80,0x00,0x90,0x20}, // 4 5 6 7
            {0xB0,0x30,0x90,0xB0},{0xB0,0x10,0x90,0xB0},{0xB0,0x20,0x90,0xA0},{0x30,0x30,0x00,0xB0}, // 8 9 a b
            {0xA0,0x30,0x80,0x10},{0x10,0x30,0x10,0xB0},{0xB0,0x30,0x80,0x90},{0xB0,0x20,0x80,0x80}, // c d e f
            {0xA0,0x30,0x80,0x30},{0x30,0x20,0x10,0xA0},{0x00,0x80,0x40,0x00},{0x00,0x10,0x90,0x30}, // g h i j
            {0x00,0x80,0x60,0x40},{0x20,0x30,0x00,0x10},{0x60,0x20,0x30,0x20},{0x10,0x20,0x00,0xA0}, // k l m n
            {0x10,0x30,0x00,0xB0},{0xB0,0x20,0x90,0x80},{0xB0,0x00,0x90,0xA0},{0x10,0x20,0x00,0x80}, // o p q r
            {0xB0,0x10,0x80,0xB0},{0x80,0x80,0xC0,0x00},{0x20,0x30,0x10,0x30},{0x30,0x10,0x10,0xB0}, // s t u y
            {0x80,0x50,0xA0,0x10},{0x10,0x80,0x40,0x80},{0x10,0x00,0x00,0x80},{0x00,0x00,0x00,0x00},
            {0xF0,0xF0,0xF0,0xF0},}; // NC

// 第1、2行数字  le
// 0 1 2 3 4 5 6 7 8 9
const unsigned char LCD_COM_Row12_Value[][2] =
					{{0x50,0xF0},{0x50,0x00},{0x30,0xD0},{0x70,0x90},{0x70,0x20}, // 0 1 2 3 4
					{0x60,0xB0},{0x60,0xF0},{0x50,0x10},{0x70,0xF0},{0x70,0xB0},  // 5 6 7 8 9
					{0x70,0x70},{0x60,0xE0},{0x00,0xF0},{0x70,0xC0},{0x20,0xF0},  // a b c d e
					{0x20,0x70},{0x40,0xF0},{0x70,0x60},{0x00,0x60},{0x00,0xE0},  // f g h i l
					{0x60,0x50},{0x60,0x40},{0x60,0xC0},{0x30,0x70},{0x70,0x30},  //m n o p q
					{0x20,0x40},{0x60,0xB0},{0x20,0xE0},{0x50,0xE0},{0x70,0xA0},  //r s t u y
					{0x20,0x00},{0x00,0x00},}; // - NC

// 第3行数字  le
// 0 1 2 3 4 5 6 7 8 9
const unsigned char LCD_COM_Row3_Value[][2] =
					{{0x70,0x70},{0x20,0x40},{0x50,0xD0},{0x60,0xD0},{0x20,0xE0},  //0 1 2 3 4
					{0x60,0xB0},{0x70,0xB0},{0x20,0x50},{0x70,0xF0},{0x60,0xF0},   //5 6 7 8 9
					{0x50,0x30},{0x70,0xC0},{0x50,0xB0},{0x10,0xB0},{0x30,0xE0},{0x30,0x80},{0x10,0xF0},   //C d E F h n P
					{0x20,0xF0},{0x60,0xB0},{0x50,0xA0},{0x00,0x80},{0x70,0x60},   //q s t - U
					{0x70,0x80},{0x50,0x20},{0x70,0x00},{0x10,0x80},}; //o l u r

// 小数点  le
const unsigned char LCD_COM_Row0_DotValue[] = {0x10,0x20,0x10};
const unsigned char LCD_COM_Row1_DotValue[] = {0x80,0x80,0x80};
const unsigned char LCD_COM_Row2_DotValue[] = {0x80,0x80,0x80};
const unsigned char LCD_COM_Row3_DotValue[] = {0x80,0x80,0x80};  //P15 P17 P18

// 冒号
const unsigned char LCD_COM_Row_ColonValue[] = {0x20,0x10};   //P13 P16

// Max Min Avg
const unsigned char LCD_COM_Row_Max[] = {0x20,0x40,0x80,0x80,0x80};   //Max Min Avg 通讯 报警
const unsigned char LCD_PIN_Max[] = {12,12,12,30,32};

// 负号 le
const unsigned char LCD_COM_NegaSign[] = {0x80,0x20,0x20,0x80};
const unsigned char LCD_PIN_NegaSign[] = {13,57,56,73};

// 第3行 求和符号∑
const unsigned char LCD_COM_Sum[] = {0x10,0x10,0x10,0x80};
const unsigned char LCD_PIN_Sum[] = {12,57,56,80};

// A相 B相 C相 le
const unsigned char LCD_COM_Phase[] = {0x20,0x40,0x40};
const unsigned char LCD_PIN_Phase[] = {13,57,56};

// AB相 BC相 CA相 le
const unsigned char LCD_COM_PhaseD[] = {0x60,0xC0,0xC0};
const unsigned char LCD_PIN_PhaseD[] = {13,57,56};

const unsigned char LCD_COM_ClearLeft[] = {0xE0,0xF0,0xF0,0xF0};
const unsigned char LCD_PIN_ClearLeft[] = {13,57,56,80};

// 显示单位 w kw mw kmw var kvar mvar kmvar va kva mva kmva oxff v kv mv A kA mA
const unsigned char LCD_COM_UNIT0[][4] =
                    {{0x00,0x00,0x10,0x00},{0x10,0x00,0x10,0x00},{0x00,0x80,0x10,0x00},{0x10,0x80,0x10,0x00},
                    {0x00,0x00,0x00,0x40},{0x00,0x40,0x00,0x40},{0x00,0x20,0x00,0x40},{0x00,0x60,0x00,0x40},
                    {0x00,0x00,0x60,0x00},{0x20,0x00,0x60,0x00},{0x40,0x00,0x60,0x00},{0x60,0x00,0x60,0x00},
                    {0x00,0x00,0x20,0x00},{0x20,0x00,0x20,0x00},{0x40,0x00,0x20,0x00},
                    {0x00,0x00,0x40,0x00},{0x20,0x00,0x40,0x00},{0x40,0x00,0x40,0x00},
                    {0x00,0x00,0x20,0x80},{0x00,0x00,0x40,0x80},{0x70,0xE0,0x70,0xC0},};
const unsigned char LCD_PIN_UNIT0[] = {30,31,32,33};

// 显示单位 w kw mw kmw var kvar mvar kmvar va kva mva kmva oxff v kv mv A kA mA
const unsigned char LCD_COM_UNIT1[][4] =
                    {{0x00,0x40,0x00,0x00},{0x00,0x40,0x40,0x00},{0x00,0x40,0x20,0x00},{0x00,0x40,0x60,0x00},
                    {0x00,0x80,0x00,0x00},{0x00,0x80,0x80,0x00},{0x00,0x80,0x00,0x80},{0x00,0x80,0x80,0x80},
                    {0x60,0x00,0x00,0x00},{0x60,0x20,0x00,0x00},{0x60,0x00,0x10,0x00},{0x60,0x20,0x10,0x00},
                    {0x20,0x00,0x00,0x00},{0x20,0x20,0x00,0x00},{0x20,0x00,0x10,0x00},
                    {0x40,0x00,0x00,0x00},{0x40,0x20,0x00,0x00},{0x40,0x00,0x10,0x00},
                    {0xA0,0x00,0x00,0x00},{0xC0,0x00,0x00,0x00},{0xE0,0xE0,0xF0,0x80},};
const unsigned char LCD_PIN_UNIT1[] = {35,37,39,41};

// 显示单位 w kw mw kmw var kvar mvar kmvar va kva mva kmva oxff v kv mv A kA mA
const unsigned char LCD_COM_UNIT2[][4] =
                    {{0x00,0x40,0x00,0x00},{0x00,0x40,0x40,0x00},{0x00,0x40,0x20,0x00},{0x00,0x40,0x60,0x00},
                    {0x00,0x80,0x00,0x00},{0x00,0x80,0x80,0x00},{0x00,0x80,0x00,0x80},{0x00,0x80,0x80,0x80},
                    {0x10,0x20,0x00,0x00},{0x10,0x30,0x00,0x00},{0x10,0x20,0x10,0x00},{0x10,0x30,0x10,0x00},
                    {0x00,0x20,0x00,0x00},{0x00,0x30,0x00,0x00},{0x00,0x20,0x10,0x00},
                    {0x10,0x00,0x00,0x00},{0x10,0x10,0x00,0x00},{0x10,0x00,0x10,0x00},
                    {0x20,0x20,0x00,0x00},{0x30,0x00,0x00,0x00},{0x30,0xF0,0xF0,0x80},};
const unsigned char LCD_PIN_UNIT2[] = {34,36,38,40};

// 显示单位 kwh kvar
const unsigned char LCD_COM_UNIT3[][3] =
                    {{0x60,0x20,0x00},{0x80,0xC0,0x00},{0xE0,0xF0,0x80}};
const unsigned char LCD_PIN_UNIT3[] = {58,60,62};

// 开关量框架1
const unsigned char LCD_COM_INS[] = {0x40,0x20,0x80,0x10,0x40,0x20,0x80,0x80};
const unsigned char LCD_PIN_INS[] = {89,88,86,87,91,90,90,87};

// 开关量框架2
const unsigned char LCD_COM_OUTS[] = {0X80,0X80,0X80,0X80};
const unsigned char LCD_PIN_OUTS[] = {84,83,82,85};

// 开关量状态1
const unsigned char LCD_COM_IN_On[] =   {0x20,0x10,0x80,0x20,0x20,0x10,0x40,0x10}; //闭合
const unsigned char LCD_COM_IN_Off[] =  {0x10,0x80,0x40,0x40,0x10,0x80,0x40,0x20}; //开断
const unsigned char LCD_PIN_IN_On[] =   {89,88,88,87,91,90,86,86};
const unsigned char LCD_PIN_IN_Off[] =  {89,89,88,87,91,91,90,86};

// 开关量状态2
const unsigned char LCD_COM_OUT_On[] =  {0X40,0X40,0X40,0X40};//闭合
const unsigned char LCD_COM_OUT_Off[] = {0X20,0X10,0X20,0X10};//开断
const unsigned char LCD_PIN_OUT_On[] =  {84,83,82,85};
const unsigned char LCD_PIN_OUT_Off[] = {84,82,82,84};

// 四象限显示
const unsigned char LCD_COM_FourQ[] = {0X20,0X20,0X10,0X10};
const unsigned char LCD_PIN_FourQ[] = {81,83,81,85};

// 时间点
const unsigned char LCD_COM_TimeDot[PH_TH][2] = {{0x10,0x80},{0x80,0x20},{0x20,0x80}};
const unsigned char LCD_PIN_TimeDot[PH_TH][2] = {{18,9},{8,18},{66,78}};

// 尖 峰 平 谷 脊 骨 上 上 月
const unsigned char LCD_COM_Record[] = {0x10,0x80,0x40,0x20,0x10,0x10,0x20,0x40,0x80};
const unsigned char LCD_PIN_Record[] = {58,59,59,59,59,61,61,61,61};


// 显存
unsigned char g_Display_Data[DISPNUM];
unsigned char sg_Display_Data_Back[DISPNUM];

const u8 cg_Disp[] = {	CHAR_0,/*0*/
						CHAR_1,/*1*/
						CHAR_2,/*2*/
						CHAR_3,/*3*/
						CHAR_4,/*4*/
						CHAR_5,/*5*/
						CHAR_6,/*6*/
						CHAR_7,/*7*/
						CHAR_8,/*8*/
						CHAR_9 /*9*/
			           };

void HT_I2C_delay(void)
{
#ifndef _DEBUG
	int i = 60; //这里可以优化速度
#else
	int i = 30; //这里可以优化速度
#endif
   while(i)
   {
     i--;
   }
}

void HT_Send_Delay(void)
{
#ifndef _DEBUG
	int i = 12; //这里可以优化速度
#else
	int i = 6; //这里可以优化速度
#endif
   while(i)
   {
     i--;
   }
}

// 写一个字节数据
void HT_SendBit(unsigned char data, unsigned char cnt)
{
    unsigned char i;
    for(i = 0; i < cnt; i++)  // 输出8位数据
    {
        if(data&0x80)
        {
        	LCD_SDA_H;
        }
        else
        {
        	LCD_SDA_L;
        }
        LCD_SCL_L;
        data<<=1;
		    nus_delay(5);
        LCD_SCL_H;
    }
}

void HT_SendCmd(unsigned char Command)
{

	nus_delay(5);
	LCD_CS_L;
	HT_SendBit(0x80, 4);
	HT_SendBit(Command, 8);
	nus_delay(5);
    LCD_CS_H;

}

void HT_WriteData(unsigned char addr,unsigned char data)
{
	LCD_CS_L;
	HT_SendBit(0xa0, 3);
	HT_SendBit(addr, 7);
	HT_SendBit(data, 4);
	LCD_CS_H;
}

// 液晶背光
void HT_LedStatus(BOOL bStatus)
{
	if (bStatus)
	{
		LCD_BG_H;
	}
	else
	{
		LCD_BG_L;
	}
}

u8 HT_Data_Index(unsigned char data)
{
    return cg_Disp[data];
}

void HT_Init(void)      //液晶屏幕初始化
{
	int i = 0;
	LCD_CS_H;
	LCD_SCL_H;
	LCD_SDA_H;
	LCD_CS_L;
	HT_LedStatus(TRUE);

	HT_SendCmd(BIAS);
	HT_SendCmd(SYSEN);
	HT_SendCmd(LCDON);
    HT_SendCmd(TONE4K);
	LCD_CS_H;
    HT_I2C_delay();
	for (i = 0; i < DISPNUM; i++)
	{
		g_Display_Data[i] = 0x00;
		sg_Display_Data_Back[i] = 0xff;
	}
	Lcd_Update_screen();
}

/**
  * @brief  写三排数字
  * @param  unsigned char row:行(0-2)
  *			unsigned char col:列(0-4)
  *			unsigned char Value:数值(0-3位为值，有小数点时，第4位置1)
  * @retval None
  */
void HT_WriteValue(unsigned char row,unsigned char col,unsigned char Value)
{
	if (row == 0)
	{
		if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[14+4*col], LCD_COM_Row0_Value[36][0]);
			MODIFY_BIT(g_Display_Data[15+4*col], LCD_COM_Row0_Value[36][1]);
			MODIFY_BIT(g_Display_Data[16+4*col], LCD_COM_Row0_Value[36][2]);
			MODIFY_BIT(g_Display_Data[17+4*col], LCD_COM_Row0_Value[36][3]);
		}
		else
		{
			if (Value <= DISP1_ALL)
			{
				MODIFY_BIT(g_Display_Data[14+4*col], LCD_COM_Row0_Value[36][0]);
			    MODIFY_BIT(g_Display_Data[15+4*col], LCD_COM_Row0_Value[36][1]);
			    MODIFY_BIT(g_Display_Data[16+4*col], LCD_COM_Row0_Value[36][2]);
			    MODIFY_BIT(g_Display_Data[17+4*col], LCD_COM_Row0_Value[36][3]);
				g_Display_Data[14+4*col] |= LCD_COM_Row0_Value[Value][0];
				g_Display_Data[15+4*col] |= LCD_COM_Row0_Value[Value][1];
				g_Display_Data[16+4*col] |= LCD_COM_Row0_Value[Value][2];
				g_Display_Data[17+4*col] |= LCD_COM_Row0_Value[Value][3];
			}
		}
	}
	else if (row == 1)
	{
		if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[53-4*col], LCD_COM_Row12_Value[8][0]);
			MODIFY_BIT(g_Display_Data[55-4*col], LCD_COM_Row12_Value[8][1]);
		}
		else
		{
			if (Value <= DISP_ALL)
			{
				MODIFY_BIT(g_Display_Data[53-4*col], LCD_COM_Row12_Value[8][0]);
			    MODIFY_BIT(g_Display_Data[55-4*col], LCD_COM_Row12_Value[8][1]);
				g_Display_Data[53-4*col] |= LCD_COM_Row12_Value[Value][0];
				g_Display_Data[55-4*col] |= LCD_COM_Row12_Value[Value][1];
			}
		}
	}
	else if (row == 2)
	{
		if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[52-4*col], LCD_COM_Row12_Value[8][0]);
			MODIFY_BIT(g_Display_Data[54-4*col], LCD_COM_Row12_Value[8][1]);
		}
		else
		{
			if (Value <= DISP_ALL)
			{
				MODIFY_BIT(g_Display_Data[52-4*col], LCD_COM_Row12_Value[8][0]);
			    MODIFY_BIT(g_Display_Data[54-4*col], LCD_COM_Row12_Value[8][1]);
				g_Display_Data[52-4*col] |= LCD_COM_Row12_Value[Value][0];
				g_Display_Data[54-4*col] |= LCD_COM_Row12_Value[Value][1];
			}
		}
	}
	else if (row == 3)
	{
        if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[78-2*col], LCD_COM_Row3_Value[8][0]);
			MODIFY_BIT(g_Display_Data[79-2*col], LCD_COM_Row3_Value[8][1]);
		}
		else
		{
			if (Value <= CHAR_NUM)
			{
				MODIFY_BIT(g_Display_Data[78-2*col], LCD_COM_Row3_Value[8][0]);
			    MODIFY_BIT(g_Display_Data[79-2*col], LCD_COM_Row3_Value[8][1]);
				g_Display_Data[78-2*col] |= LCD_COM_Row3_Value[Value][0];
				g_Display_Data[79-2*col] |= LCD_COM_Row3_Value[Value][1];
			}
		}
	}
	else if (row == 4)
	{
        if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[68-2*col], LCD_COM_Row3_Value[8][0]);
			MODIFY_BIT(g_Display_Data[69-2*col], LCD_COM_Row3_Value[8][1]);
		}
		else
		{
			if (Value <= CHAR_NUM)
			{
				MODIFY_BIT(g_Display_Data[68-2*col], LCD_COM_Row3_Value[8][0]);
			    MODIFY_BIT(g_Display_Data[69-2*col], LCD_COM_Row3_Value[8][1]);
				g_Display_Data[68-2*col] |= LCD_COM_Row3_Value[Value][0];
				g_Display_Data[69-2*col] |= LCD_COM_Row3_Value[Value][1];
			}
		}
	}
	else if (row == 5)
	{
		if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[0+4*col], LCD_COM_Row0_Value[36][0]);
			MODIFY_BIT(g_Display_Data[1+4*col], LCD_COM_Row0_Value[36][1]);
			MODIFY_BIT(g_Display_Data[2+4*col], LCD_COM_Row0_Value[36][2]);
			MODIFY_BIT(g_Display_Data[3+4*col], LCD_COM_Row0_Value[36][3]);
		}
		else
		{
			if (Value <= DISP1_ALL)
			{
				MODIFY_BIT(g_Display_Data[0+4*col], LCD_COM_Row0_Value[36][0]);
			    MODIFY_BIT(g_Display_Data[1+4*col], LCD_COM_Row0_Value[36][1]);
			    MODIFY_BIT(g_Display_Data[2+4*col], LCD_COM_Row0_Value[36][2]);
			    MODIFY_BIT(g_Display_Data[3+4*col], LCD_COM_Row0_Value[36][3]);
				g_Display_Data[0+4*col] |= LCD_COM_Row0_Value[Value][0];
				g_Display_Data[1+4*col] |= LCD_COM_Row0_Value[Value][1];
				g_Display_Data[2+4*col] |= LCD_COM_Row0_Value[Value][2];
				g_Display_Data[3+4*col] |= LCD_COM_Row0_Value[Value][3];
			}
		}
	}

}


/**
  * @brief  写一排数 4个
  * @param  unsigned char row:行(0-2)
  *		    u8 1 2 3 4
  * @retval None
  */
void HT_Write4Value(u8 row,u8 tmp1,u8 tmp2,u8 tmp3,u8 tmp4)
{
    HT_WriteValue(row,0,tmp1);
    HT_WriteValue(row,1,tmp2);
    HT_WriteValue(row,2,tmp3);
    HT_WriteValue(row,3,tmp4);
}

/**
  * @brief  写三排小数点
  * @param  unsigned char row:行(0-2)
  *			unsigned char col:列(0-4)
  * @retval None
  */
void HT_WriteDot(unsigned char row,unsigned char col,unsigned char Value)
{
	if (row == 0)
	{
		if (Value == CHAR_NONE)
		{
			if (col == 0)
			{
				MODIFY_BIT(g_Display_Data[33], LCD_COM_Row0_DotValue[col]);
			}
			else if(col == 1)
			{
				MODIFY_BIT(g_Display_Data[33], LCD_COM_Row0_DotValue[col]);
			}
			else if(col == 2)
			{
                MODIFY_BIT(g_Display_Data[31], LCD_COM_Row0_DotValue[col]);
			}
		}
		else
		{
			if (col == 0)
			{
				g_Display_Data[33] |= LCD_COM_Row0_DotValue[col];
			}
			else if(col == 1)
			{
				g_Display_Data[33] |= LCD_COM_Row0_DotValue[col];
			}
			else if(col == 2)
			{
				g_Display_Data[31] |= LCD_COM_Row0_DotValue[col];
			}
		}
	}
	else if (row == 1)
	{
		if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[53-4*col], LCD_COM_Row1_DotValue[col]);
		}
		else
		{
			g_Display_Data[53-4*col] |= LCD_COM_Row1_DotValue[col];
		}
	}
	else if (row == 2)
	{
		if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[52-4*col], LCD_COM_Row2_DotValue[col]);
		}
		else
		{
			g_Display_Data[52-4*col] |= LCD_COM_Row2_DotValue[col];
		}
	}
	else if (row == 4)
	{
		if (Value == CHAR_NONE)
		{
			MODIFY_BIT(g_Display_Data[68-2*col], LCD_COM_Row3_DotValue[col]);
		}
		else
		{
			g_Display_Data[68-2*col] |= LCD_COM_Row3_DotValue[col];
		}
	}
}

/**
  * @brief  写冒号
  * @param  unsigned char row:行(0-2)
  *
  * @retval None
  */
void HT_WriteColon(unsigned char index,unsigned char Value)
{
    if (Value == CHAR_NONE)
	{
		if (index == 0)
		{
			MODIFY_BIT(g_Display_Data[70], LCD_COM_Row_ColonValue[index]);
		}
		else if(index == 1)
		{
			MODIFY_BIT(g_Display_Data[70], LCD_COM_Row_ColonValue[index]);
		}
	}
	else
	{
		if (index == 0)
		{
			g_Display_Data[70] |= LCD_COM_Row_ColonValue[index];
		}
		else if(index == 1)
		{
			g_Display_Data[70] |= LCD_COM_Row_ColonValue[index];
		}
	}

}

/**
  * @brief  Max Min Avr S1 S2
  * @param  unsigned char row:行(0-2)
  *
  * @retval None
  */
void HT_WriteMax(unsigned char index,unsigned char Value)
{
    if (Value == CHAR_NONE)
	{
        switch (index)
        {
            case IndexMax:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Max[IndexMax]], LCD_COM_Row_Max[IndexMax]);
                break;
            case IndexMin:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Max[IndexMin]], LCD_COM_Row_Max[IndexMin]);
                break;
            case IndexAvg:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Max[IndexAvg]], LCD_COM_Row_Max[IndexAvg]);
                break;
            case IndexCom:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Max[IndexCom]], LCD_COM_Row_Max[IndexCom]);
                break;
            case IndexWarm:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Max[IndexWarm]], LCD_COM_Row_Max[IndexWarm]);
                break;
            default:
                break;
        }
	}
	else
	{
        switch (index)
        {
            case IndexMax:
                g_Display_Data[LCD_PIN_Max[IndexMax]] |= LCD_COM_Row_Max[IndexMax];
                break;
            case IndexMin:
                g_Display_Data[LCD_PIN_Max[IndexMin]] |= LCD_COM_Row_Max[IndexMin];
                break;
            case IndexAvg:
                g_Display_Data[LCD_PIN_Max[IndexAvg]] |= LCD_COM_Row_Max[IndexAvg];
                break;
            case IndexCom:
                g_Display_Data[LCD_PIN_Max[IndexCom]] |= LCD_COM_Row_Max[IndexCom];
                break;
            case IndexWarm:
                g_Display_Data[LCD_PIN_Max[IndexWarm]] |= LCD_COM_Row_Max[IndexWarm];
                break;
            default:
                break;
        }
	}
}

/**
  * @brief  写负号
  * @param  unsigned char row:行(0-2)
  *
  * @retval None
  */
void HT_WriteNegative(unsigned char row,unsigned char Value)
{
	if (Value == CHAR_NONE)
	{
	    MODIFY_BIT(g_Display_Data[LCD_PIN_NegaSign[row]], LCD_COM_NegaSign[row]);
	}
	else
	{
		g_Display_Data[LCD_PIN_NegaSign[row]] |= LCD_COM_NegaSign[row];
	}
}

/**
  * @brief  写∑
  * @param  unsigned char row:行(3)
  *
  * @retval None
  */
void HT_WriteSum(unsigned char row,unsigned char Value)
{
	if (Value == CHAR_NONE)
	{
	    MODIFY_BIT(g_Display_Data[LCD_PIN_Sum[row]], LCD_COM_Sum[row]);
	}
	else
	{
		g_Display_Data[LCD_PIN_Sum[row]] |= LCD_COM_Sum[row];
	}
}

/**
  * @brief  写 时间 :
  * @param  unsigned char Row_Value:第几行(0-2)
  * @retval None
  */
void HT_WriteTimeDot(BOOL bDisp, unsigned char Row_Value)
{
	if (bDisp)
	{
		g_Display_Data[LCD_PIN_TimeDot[Row_Value][0]] |= LCD_COM_TimeDot[Row_Value][0];
		g_Display_Data[LCD_PIN_TimeDot[Row_Value][1]] |= LCD_COM_TimeDot[Row_Value][1];
	}
	else
	{
		MODIFY_BIT(g_Display_Data[LCD_PIN_TimeDot[Row_Value][0]], LCD_COM_TimeDot[Row_Value][0]);
		MODIFY_BIT(g_Display_Data[LCD_PIN_TimeDot[Row_Value][1]], LCD_COM_TimeDot[Row_Value][1]);
	}
}

/**
  * @brief  写A相 B相 C相
  * @param  unsigned char Phase_Value:0-2
  * @retval None
  */
void HT_WritePhase(BOOL bDisp, unsigned char Phase_Value)       //显示A相B相C相
{
	u8 Index = 0;
	if (bDisp)
	{
        if (Phase_Value == PH_TH)       //显示三相
		{
			for (Index = PH_A; Index < PH_TH; Index++)
			{
				g_Display_Data[LCD_PIN_Phase[Index]] |= LCD_COM_Phase[Index];
			}
		}
		else                            //显示相应单相
		{
			g_Display_Data[LCD_PIN_Phase[Phase_Value]] |= LCD_COM_Phase[Phase_Value];
		}
	}
	else
	{
		if (Phase_Value == PH_TH)       //显示三相
		{
			for (Index = PH_A; Index < PH_TH; Index++)
			{
				MODIFY_BIT(g_Display_Data[LCD_PIN_Phase[Index]], LCD_COM_Phase[Index]);
			}
		}
		else                            //显示相应单相
		{
			MODIFY_BIT(g_Display_Data[LCD_PIN_Phase[Phase_Value]], LCD_COM_Phase[Phase_Value]);
		}
	}
}

/**
  * @brief  写A相 B相 C相
  * @param  unsigned char Phase_Value:0-2
  * @retval None
  */
void HT_WritePhaseDouble(BOOL bDisp, unsigned char Phase_Value)       //显示A相B相C相
{
	u8 Index = 0;
	if (bDisp)
	{
		if (Phase_Value == PH_TH)       //显示三相
		{
			for (Index = PH_A; Index < PH_TH; Index++)
			{
				g_Display_Data[LCD_PIN_PhaseD[Index]] |= LCD_COM_PhaseD[Index];
			}
		}
		else                            //显示相应单相
		{
			g_Display_Data[LCD_PIN_PhaseD[Phase_Value]] |= LCD_COM_PhaseD[Phase_Value];
		}
	}
	else
	{
		if (Phase_Value == PH_TH)       //显示三相
		{
			for (Index = PH_A; Index < PH_TH; Index++)
			{
				MODIFY_BIT(g_Display_Data[LCD_PIN_PhaseD[Index]], LCD_COM_PhaseD[Index]);
			}
		}
		else                            //显示相应单相
		{
			MODIFY_BIT(g_Display_Data[LCD_PIN_PhaseD[Phase_Value]], LCD_COM_PhaseD[Phase_Value]);
		}
	}
}

/**
  * @brief  写右侧单位函数
  * @param  unsigned char Phase_Value:0-2
  * @retval None
  */
void HT_WriteUnit(BOOL bDisp, u8 Phase_Value, u8 Disp_Unit)
{
	u8 Index = 0;
	if (bDisp)
	{
        if(Phase_Value > PH_TH)
        {
            for (Index = PH_A; Index < PH_TH; Index++)
			{
				g_Display_Data[LCD_PIN_UNIT3[Index]] |=  LCD_COM_UNIT3[Disp_Unit][Index];
			}
        }
		if (Phase_Value == PH_TH)       //显示三相
		{
			for (Index = PH_A; Index < PH_ALL; Index++)
			{
				g_Display_Data[LCD_PIN_UNIT0[Index]] |=  LCD_COM_UNIT0[Disp_Unit][Index];
				g_Display_Data[LCD_PIN_UNIT1[Index]] |=  LCD_COM_UNIT1[Disp_Unit][Index];
				g_Display_Data[LCD_PIN_UNIT2[Index]] |=  LCD_COM_UNIT2[Disp_Unit][Index];

			}
		}
		else                            //显示相应单相
		{
			switch(Phase_Value)
		    {
                case PH_A:
                    for (Index = PH_A; Index < PH_ALL; Index++)
        			{
        				g_Display_Data[LCD_PIN_UNIT0[Index]] |=  LCD_COM_UNIT0[Disp_Unit][Index];
        			}
                    break;
                case PH_B:
                    for (Index = PH_A; Index < PH_ALL; Index++)
        			{
        				g_Display_Data[LCD_PIN_UNIT1[Index]] |=  LCD_COM_UNIT1[Disp_Unit][Index];
        			}
                    break;
                case PH_C:
                    for (Index = PH_A; Index < PH_ALL; Index++)
        			{
        				g_Display_Data[LCD_PIN_UNIT2[Index]] |=  LCD_COM_UNIT2[Disp_Unit][Index];
        			}
        			break;
        	    default:
        	        break;
		    }
		}
	}
	else
	{
	    if(Phase_Value > PH_TH)
        {
            for (Index = PH_A; Index < PH_TH; Index++)
			{
				MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT3[Index]], LCD_COM_UNIT3[2][Index]);
			}
        }
		if (Phase_Value == PH_TH)       //!显示三相
		{
			for (Index = PH_A; Index < PH_ALL; Index++)
			{
				MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT0[Index]], LCD_COM_UNIT0[DISP_UNIT_NONE][Index]);
				MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT1[Index]], LCD_COM_UNIT1[DISP_UNIT_NONE][Index]);
				MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT2[Index]], LCD_COM_UNIT2[DISP_UNIT_NONE][Index]);
			}
		}
		else                            //!显示相应单相
		{
		    switch(Phase_Value)
		    {
                case PH_A:
                    for (Index = PH_A; Index < PH_ALL; Index++)
        			{
        				MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT0[Phase_Value]], LCD_COM_UNIT0[DISP_UNIT_NONE][Index]);
        			}
                    break;
                case PH_B:
                    for (Index = PH_A; Index < PH_ALL; Index++)
        			{
        				MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT1[Phase_Value]], LCD_COM_UNIT1[DISP_UNIT_NONE][Index]);
        			}
                    break;
                case PH_C:
                    for (Index = PH_A; Index < PH_ALL; Index++)
        			{
        				MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT2[Phase_Value]], LCD_COM_UNIT2[DISP_UNIT_NONE][Index]);
        			}
        			break;
        	    default:
        	        break;
		    }
		}
	}
}

/**
  * @brief  写开关量框架
  * @param  unsigned char Phase_Value:0-2
  * @retval None
  */
void HT_Write_IO_Frame(BOOL bDisp)
{
    u8 i,j;
    g_Display_Data[13] |= 0x10;
    g_Display_Data[85] |= 0x20;
	if (bDisp)
	{
		for (i = 0; i < DIN_NUM; i++)
		{
			g_Display_Data[LCD_PIN_INS[i]] |= LCD_COM_INS[i];
		}
		for (j = 0; j < OUT_NUM; j++)
		{
			g_Display_Data[LCD_PIN_OUTS[j]] |= LCD_COM_OUTS[j];
		}
	}
	else
	{
		for (i = 0; i < DIN_NUM; i++)
		{
			MODIFY_BIT(g_Display_Data[LCD_PIN_INS[i]],LCD_COM_INS[i]);
		}
		for (j = 0; j < OUT_NUM; j++)
		{
			MODIFY_BIT(g_Display_Data[LCD_PIN_OUTS[j]],LCD_COM_OUTS[j]);
		}
	}
}

/**
  * @brief  写四象限框架
  * @param  unsigned char Phase_Value:0-2
  * @retval None
  */
void HT_Write_Four_Quadrant(BOOL bDisp)
{
    u8 i;
    g_Display_Data[83] |= 0x10;
	if (bDisp)
	{
        for (i = 0; i < 4; i++)
		{
			MODIFY_BIT(g_Display_Data[LCD_PIN_FourQ[i]],LCD_COM_FourQ[i]);
		}
		switch(FourQuadtand)
		{
            case 0:
                g_Display_Data[LCD_PIN_FourQ[1]] |= LCD_COM_FourQ[1];  // 1象限 感性
                break;
            case 1:
                g_Display_Data[LCD_PIN_FourQ[0]] |= LCD_COM_FourQ[0];  // 2象限 容性
                break;
            case 2:
                g_Display_Data[LCD_PIN_FourQ[3]] |= LCD_COM_FourQ[3];  // 3象限 感性
                break;
            case 3:
                g_Display_Data[LCD_PIN_FourQ[2]] |= LCD_COM_FourQ[2];  // 4象限 容性
                break;
            default:
                break;
		}
	}
	else
	{
		for (i = 0; i < 4; i++)
		{
			MODIFY_BIT(g_Display_Data[LCD_PIN_FourQ[i]],LCD_COM_FourQ[i]);
		}
	}
}


/**
  * @brief  写开关量状态
  * @param  unsigned char Phase_Value:0-
  * @retval None
  * TRUE 闭合  FALSE 开断
  */
void HT_Write_IO_Status(BOOL bDisp,u8 Kind,u8 Index)
{
	if (bDisp)
	{
	    if(Kind == 0x00)
	    {
	        MODIFY_BIT(g_Display_Data[LCD_PIN_IN_Off[Index]],LCD_COM_IN_Off[Index]);
    		g_Display_Data[LCD_PIN_IN_On[Index]] |= LCD_COM_IN_On[Index];
	    }
		else if(Kind == 0x01)
		{
		    MODIFY_BIT(g_Display_Data[LCD_PIN_OUT_Off[Index]],LCD_COM_OUT_Off[Index]);
    		g_Display_Data[LCD_PIN_OUT_On[Index]] |= LCD_COM_OUT_On[Index];
		}
	}
	else
	{
		if(Kind == 0x00)
	    {
	        MODIFY_BIT(g_Display_Data[LCD_PIN_IN_On[Index]],LCD_COM_IN_On[Index]);
    		g_Display_Data[LCD_PIN_IN_Off[Index]] |= LCD_COM_IN_Off[Index];
	    }
		else if(Kind == 0x01)
		{
		    MODIFY_BIT(g_Display_Data[LCD_PIN_OUT_On[Index]],LCD_COM_OUT_On[Index]);
    		g_Display_Data[LCD_PIN_OUT_Off[Index]] |= LCD_COM_OUT_Off[Index];
		}
	}
}

/**
  * @brief  清空左侧标记位
  * @param  unsigned char Phase_Value:0-2
  * @retval None
  */
void HT_ClearLeftSign(BOOL bClear)
{
	u8 Index = 0;
	if (!bClear)
	{
		return;
	}
	else
	{
		for (Index = PH_A; Index < PH_ALL; Index++)
		{
			MODIFY_BIT(g_Display_Data[LCD_PIN_ClearLeft[Index]], LCD_COM_ClearLeft[Index]);
		}
	}
}

/**
  * @brief  清空右侧标记位
  * @param  unsigned char Phase_Value:0-2
  * @retval None
  */
void HT_ClearRightSign(BOOL bClear)
{
	u8 Index = 0;
	if (!bClear)
	{
		return;
	}
	else
	{
		for (Index = PH_A; Index < PH_ALL; Index++)
		{
			MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT0[Index]], LCD_COM_UNIT0[DISP_UNIT_NONE][Index]);
			MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT1[Index]], LCD_COM_UNIT1[DISP_UNIT_NONE][Index]);
			MODIFY_BIT(g_Display_Data[LCD_PIN_UNIT2[Index]], LCD_COM_UNIT2[DISP_UNIT_NONE][Index]);
		}
	}
}

void HT_WriteRecordSign(unsigned char index,unsigned char Value)
{
    if (Value == CHAR_NONE)
	{
        switch (index)
        {
            case IndexS_jian:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_jian]] , LCD_COM_Record[IndexS_jian]);
                break;
            case IndexS_feng:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_feng]] , LCD_COM_Record[IndexS_feng]);
                break;
            case IndexS_ping:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_ping]] , LCD_COM_Record[IndexS_ping]);
                break;
            case IndexS_gu1:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_gu1]] , LCD_COM_Record[IndexS_gu1]);
                break;
            case IndexS_ji:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_ji]] , LCD_COM_Record[IndexS_ji]);
                break;
            case IndexS_gu2:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_gu2]] , LCD_COM_Record[IndexS_gu2]);
                break;
            case IndexS_shang1:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_shang1]] , LCD_COM_Record[IndexS_shang1]);
                break;
            case IndexS_shang2:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_shang1]] , LCD_COM_Record[IndexS_shang2]);
                break;
            case IndexS_yue:
                MODIFY_BIT(g_Display_Data[LCD_PIN_Record[IndexS_yue]] , LCD_COM_Record[IndexS_yue]);
                break;
            default:
                break;
        }
	}
	else
	{
        switch (index)
        {
            case IndexS_jian:
                g_Display_Data[LCD_PIN_Record[IndexS_jian]] |= LCD_COM_Record[IndexS_jian];
                break;
            case IndexS_feng:
                g_Display_Data[LCD_PIN_Record[IndexS_feng]] |= LCD_COM_Record[IndexS_feng];
                break;
            case IndexS_ping:
                g_Display_Data[LCD_PIN_Record[IndexS_ping]] |= LCD_COM_Record[IndexS_ping];
                break;
            case IndexS_gu1:
                g_Display_Data[LCD_PIN_Record[IndexS_gu1]] |= LCD_COM_Record[IndexS_gu1];
                break;
            case IndexS_ji:
                g_Display_Data[LCD_PIN_Record[IndexS_ji]] |= LCD_COM_Record[IndexS_ji];
                break;
            case IndexS_gu2:
                g_Display_Data[LCD_PIN_Record[IndexS_gu2]] |= LCD_COM_Record[IndexS_gu2];
                break;
            case IndexS_shang1:
                g_Display_Data[LCD_PIN_Record[IndexS_shang1]] |= LCD_COM_Record[IndexS_shang1];
                break;
            case IndexS_shang2:
                g_Display_Data[LCD_PIN_Record[IndexS_shang2]] |= LCD_COM_Record[IndexS_shang2];
                break;
            case IndexS_yue:
                g_Display_Data[LCD_PIN_Record[IndexS_yue]] |= LCD_COM_Record[IndexS_yue];
                break;
            default:
                break;
        }
	}
}


/*test*/
void HT_TEST(void)
{
   g_Display_Data[33] |= 0x80;
}

/**
  * @brief  每段逐个显示
  * @param  unsigned char Com_Value:0x10,0x20,0x40,0x80,
  *			unsigned char Pin_Value:0-83
  * @retval None
  */
void HT_WriteEveryComPin(BOOL bDisp, unsigned char Com_Value, unsigned char Pin_Value)
{
	if (bDisp)
	{
		g_Display_Data[Pin_Value] |= Com_Value;
	}
	else
	{
		MODIFY_BIT(g_Display_Data[Pin_Value], Com_Value);
	}
}

/**
  * @brief  清屏幕 不显示任何信息
  * @param  None
  * @retval None
  */
/**********************************************************************
 * 函 数 名: Clear_Screen
 * 功能描述: 全显测试有没有坏字段
 * 输    入: 无
 * 返    回: 无
 ***********************************************************************/
void Clear_Screen(void)
{
	int i = 0;
	for (i = 0; i < DISPNUM; i++)
	{
		g_Display_Data[i] = 0xff;
		sg_Display_Data_Back[i] = 0x00;
	}
}

/**
  * @brief  更新屏幕显示信息
  * @param  None
  * @retval None
  */
void Lcd_Update_screen(void)
{
	int i = 0;
	nus_delay(10);
	LCD_CS_L;
	nus_delay(10);

	for (i = 0; i < DISPNUM>>1; i++)
	{
		if (sg_Display_Data_Back[2*i] != g_Display_Data[2*i])
		{
			HT_WriteData(i*4,g_Display_Data[2*i]);
		}
		if (sg_Display_Data_Back[2*i+1] != g_Display_Data[2*i+1])
		{
			HT_WriteData(i*4+2,g_Display_Data[2*i+1]);
		}
		sg_Display_Data_Back[2*i] = g_Display_Data[2*i];
		sg_Display_Data_Back[2*i+1] = g_Display_Data[2*i+1];
	}
	LCD_CS_H;
	LCD_CS_H;
}

/**
  * @brief  更新屏幕显示信息(全屏 or 全清)
  * @param  None
  * @retval None
  */
void LCD_Update_screen_T(BOOL Flag)
{
	int i = 0;

	if (Flag)
	{
		for (i = 0; i < DISPNUM>>1; i++)
		{
			HT_WriteData(i*4,0xff);
			HT_WriteData(i*4+2,0xff);
		}
	}
	else
	{
		for (i = 0; i < DISPNUM>>1; i++)
		{
			HT_WriteData(i*4,0);
			HT_WriteData(i*4+2,0);
		}
	}
//	PT_TimerDelay(10);
}

/**
  * @brief  清屏幕 不显示任何信息
  * @param  None
  * @retval None
  */
/**********************************************************************
 * 函 数 名: Clear_Screen
 * 功能描述: 全显测试有没有坏字段
 * 输    入: 无
 * 返    回: 无
 ***********************************************************************/
void Reset_Screen(void)
{
	int i = 0;
	for (i = 0; i < DISPNUM; i++)
	{
		g_Display_Data[i] = 0x00;
		sg_Display_Data_Back[i] = 0xff;
	}
}
/*
void Test_LCDValue(void)
{
	int i = 0;
	int j = 0;
	HT_WriteValue(0, 0, 0);
	Update_screen();
	HT_WriteValue(0, 0, 1);
	Update_screen();
	HT_WriteValue(0, 0, 2);
	Update_screen();
	HT_WriteValue(0, 0, 3);
	Update_screen();
	HT_WriteValue(0, 0, 4);
	Update_screen();
	HT_WriteValue(0, 0, 5);
	Update_screen();
	HT_WriteValue(0, 0, 6);
	Update_screen();
	HT_WriteValue(0, 0, 7);
	Update_screen();
	HT_WriteValue(0, 0, 8);
	Update_screen();
	HT_WriteValue(0, 0, 9);
	Update_screen();
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 4; j++)

		{
			HT_WriteValue(0, j, i);
			Update_screen();
			HT_WriteDot(0, j, CHAR_NONE);
			Update_screen();
			HT_WriteDot(0, j, CHAR_DOT);
			Update_screen();
			HT_WriteValue(1, j, i);
			Update_screen();
			HT_WriteDot(1, j, CHAR_NONE);
			Update_screen();
			HT_WriteDot(1, j, CHAR_DOT);
			Update_screen();
			HT_WriteValue(2, j, i);
			Update_screen();
			HT_WriteDot(2, j, CHAR_NONE);
			Update_screen();
			HT_WriteDot(2, j, CHAR_DOT);
			Update_screen();
		}
	}
//		for (i = 0; i < 380; i++)
//		{
//			HT_WriteData(i,0);
//		}
//		for (i = 0; i < 48; i++)
//		{
//			HT_WriteData(i*4,0x00);
//			HT_WriteData(i*4+2,0x00);
//		}
//		for (i = 0; i < 48; i++)
//		{
//
//			HT_WriteData(i*4,0x10);
//			HT_WriteData(i*4,0x20);
//			HT_WriteData(i*4,0x40);
//			HT_WriteData(i*4,0x80);
//			HT_WriteData(i*4,0x00);
//			HT_WriteData(i*4+2,0x10);
//			HT_WriteData(i*4+2,0x20);
//			HT_WriteData(i*4+2,0x40);
//			HT_WriteData(i*4+2,0x80);
//			HT_WriteData(i*4+2,0x00);
//		}
}

void Test_LCDLED(void)
{
	int i = 0;
	for (i = 0; i < MAX_STAR_CAP_NUM; i++)
	{
		HT_WriteCapLED(CAPTYPE_STAR, i, PH_A, FHKG_OFFTOON);
		Update_screen();
		HT_WriteCapLED(CAPTYPE_STAR, i, PH_A, FHKG_ONTOOFF);
		Update_screen();
	}
	for (i = 0; i < MAX_STAR_CAP_NUM; i++)
	{
		HT_WriteCapLED(CAPTYPE_STAR, i, PH_B, FHKG_OFFTOON);
		Update_screen();
		HT_WriteCapLED(CAPTYPE_STAR, i, PH_B, FHKG_ONTOOFF);
		Update_screen();
	}
	for (i = 0; i < MAX_STAR_CAP_NUM; i++)
	{
		HT_WriteCapLED(CAPTYPE_STAR, i, PH_C, FHKG_OFFTOON);
		Update_screen();
		HT_WriteCapLED(CAPTYPE_STAR, i, PH_C, FHKG_ONTOOFF);
		Update_screen();
	}
	for (i = 0; i < MAX_DELTA_CAP_NUM-10; i++)
	{
		HT_WriteCapLED(CAPTYPE_DELTA, i, PH_A, FHKG_OFFTOON);
		Update_screen();
		HT_WriteCapLED(CAPTYPE_DELTA, i, PH_A, FHKG_ONTOOFF);
		Update_screen();
	}
	for (i = 0; i < MAX_DELTA_CAP_NUM-10; i++)
	{
		HT_WriteCapLED(CAPTYPE_DELTA, i, PH_C, FHKG_OFFTOON);
		Update_screen();
		HT_WriteCapLED(CAPTYPE_DELTA, i, PH_C, FHKG_ONTOOFF);
		Update_screen();
	}
}

void Test_LCDNorm(void)
{
	int i = 0;
	for (i = DISP_NORM_S1; i <= DISP_NORM_T; i++)
	{
		HT_WriteNorm(TRUE, i);
		Update_screen();
		HT_WriteNorm(FALSE, i);
		Update_screen();
	}
}

void Test_LCDSet(void)
{
	int i = 0;
	for (i = DISP_SET_CT; i <= DISP_SET_TP; i++)
	{
		HT_WriteSet(TRUE, TRUE, i);
		Update_screen();
		HT_WriteSet(FALSE, TRUE, i);
		Update_screen();
	}
}

void Test_LCDPrt(void)
{
	int i = 0;
	for (i = DISP_PRT_TEMP; i <= DISP_PRT_LP; i++)
	{
		HT_WritePrt(TRUE, i);
		Update_screen();
		HT_WritePrt(FALSE, i);
		Update_screen();
	}
}

void Test_LCDUIHL12(void)
{
//		int i = 0;
//		for (i = DISP_UIHL12_U; i <= DISP_UIHL12_2; i++)
//		{
//			HT_WriteUIHL12(TRUE, i);
//			Update_screen();
//			HT_WriteUIHL12(FALSE, i);
//			Update_screen();
//		}
}

void Test_LCDPhase(void)
{
	int i = 0;
	for (i = PH_A; i <= PH_C; i++)
	{
		HT_WritePhase(TRUE, i);
		Update_screen();
		HT_WritePhase(FALSE, i);
		Update_screen();
	}
}

void Test_LCDLedPhase(void)
{
	int i = 0;
	for (i = PH_A; i <= PH_C; i++)
	{
		HT_WriteLedPhase(TRUE, i);
		Update_screen();
		HT_WriteLedPhase(FALSE, i);
		Update_screen();
	}
}

void Test_LCDLedAddr(void)
{
	int i = 0;
	for (i = 0; i < MAX_DELTA_CAP_NUM; i++)
	{
		HT_WriteLedAddr(TRUE, i);
		Update_screen();
		HT_WriteLedAddr(FALSE, i);
		Update_screen();
	}
}

void Test_LCDCtrl(void)
{
	int i = 0;
	for (i = DISP_CTRL_SET; i <= DISP_CTRL_TEST; i++)
	{
		HT_WriteCtrl(TRUE, i);
		Update_screen();
		HT_WriteCtrl(FALSE, i);
		Update_screen();
	}
}

void Test_LCDSys(void)
{
	int i = 0;
	for (i = DISP_SYS_INDUCTOR; i <= DISP_SYS_CAP; i++)
	{
		HT_WriteSys(TRUE, PH_A, i);
		Update_screen();
		HT_WriteSys(FALSE, PH_A, i);
		Update_screen();
		HT_WriteSys(TRUE, PH_B, i);
		Update_screen();
		HT_WriteSys(FALSE, PH_B, i);
		Update_screen();
		HT_WriteSys(TRUE, PH_C, i);
		Update_screen();
		HT_WriteSys(FALSE, PH_C, i);
		Update_screen();
	}
}

void Test_LCDCapStatus(void)
{
	HT_WriteCapStatus(TRUE);
	Update_screen();
	HT_WriteCapStatus(FALSE);
	Update_screen();
}

void Test_LCDTimeDot(void)
{
	HT_WriteTimeDot(TRUE, 0);
	Update_screen();
	HT_WriteTimeDot(FALSE, 0);
	Update_screen();
	HT_WriteTimeDot(TRUE, 1);
	Update_screen();
	HT_WriteTimeDot(FALSE, 1);
	Update_screen();
	HT_WriteTimeDot(TRUE, 2);
	Update_screen();
	HT_WriteTimeDot(FALSE, 2);
	Update_screen();
}

void Test_LCDCapType(void)
{
	int i = 0;
	for (i = DISP_CAPTYPE_ID; i <= DISP_CAPTYPE_STAR; i++)
	{
		HT_WriteCapType(TRUE, i);
		Update_screen();
		HT_WriteCapType(FALSE, i);
		Update_screen();
	}
}

void Test_LCDEveryComPin(void)
{
	int com_Value = 0;
	int pin_Value = 0;
	unsigned char Value[] = {0x10,0x20,0x40,0x80};

	for (com_Value = 0; com_Value <= 3; com_Value++)
	{
		for (pin_Value = 0; pin_Value <= 83; pin_Value++)
		{
			HT_WriteEveryComPin(TRUE, Value[com_Value],pin_Value);
			Update_screen();
			HT_WriteEveryComPin(FALSE, Value[com_Value],pin_Value);
			Update_screen();
		}
	}
}
*/
#endif // #ifdef _GUILCD_



