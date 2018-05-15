/*! @file
********************************************************************************
<PRE>
模 块 名     : 铁电读取写入程序（MB85RC16/04）
文 件 名     : drvFram.h
相关文件      :
文件实现功能 :
作者         : < 612 >
版本         : 1.0
--------------------------------------------------------------------------------
备注         : 数显表
--------------------------------------------------------------------------------
修改记录 :
  日 期        版本      修改人         修改内容
2018/05/22   1.0    < 612 >        创建

</PRE>
********************************************************************************

  * 版权所有(c) YYYY, <xxxx>, 保留所有权利

*******************************************************************************/

#ifndef _DRVFRAM_H
#define _DRVFRAM_H

/* Includes ------------------------------------------------------------------*/
//#include "TimerDef.h"

/* Exported types ------------------------------------------------------------*/
#define IIC_ERR_NONE			0
#define IIC_ERR_NO_ACK			1
/* Exported constants --------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
/* Exported macro ------------------------------------------------------------*/

void FRAM_WriteData(void);
void FRAM_ReadData(void);
void FRAM_RecordWrite(void);
void FRAM_RecordRead(void);
void FRAM_IndexWrite(void);
void FRAM_IndexRead(void);
void FRAM_DoRecordWrite(void);
void FRAM_DoRecordRead(void);
void FRAM_MaxDemWrite(void);
void FRAM_MaxDemRead(void);

void MyI2C_Start(void);
void MyI2C_Stop(void);
u8   MyI2C_SendByte(u8 ByteData);
u8   MyI2C_ReceiveByte(u8 last_char);

//void FRAM_Erase_Sector(u16 Addr, u16 Num);
//void FRAM_Erase_Chip(void);

#endif /* _DRVFRAM_H */

//===========================================================================
// No more.
//===========================================================================

