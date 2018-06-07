/*! @file
********************************************************************************
<PRE>
模 块 名     : 铁电读取写入程序（MB85RC64）
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
// 电能记录按月分配
#define FRAM_Energy_SIZE	            591    // 最大值参数长度 31*19+2（校验位）

#define FRAM_JanuEnergy_sADDR      (0x00000436)      // 1月记录地址
#define FRAM_FebrEnergy_sADDR      (0x00000686)      // 2月记录地址
#define FRAM_MarcEnergy_sADDR      (0x000008D6)      // 3月记录地址
#define FRAM_ApriEnergy_sADDR      (0x00000B26)      // 4月记录地址
#define FRAM_MayyEnergy_sADDR      (0x00000D76)      // 5月记录地址
#define FRAM_JuneEnergy_sADDR      (0x0FC6)      // 6月记录地址
#define FRAM_JulyEnergy_sADDR      (0x00001216)      // 7月记录地址
#define FRAM_AuguEnergy_sADDR      (0x00001466)      // 8月记录地址
#define FRAM_SeptEnergy_sADDR      (0x000016B6)      // 9月记录地址
#define FRAM_OctoEnergy_sADDR      (0x00001906)      // 10月记录地址
#define FRAM_NoveEnergy_sADDR      (0x00001B56)      // 11月记录地址
#define FRAM_DeceEnergy_sADDR      (0x00001DA6)      // 12月记录地址



void MyI2C_Start(void);
void MyI2C_Stop(void);
u8   MyI2C_SendByte(u8 ByteData);
u8   MyI2C_ReceiveByte(u8 last_char);

void FRAM_WriteData(void);
void FRAM_RecordWrite(void);
void FRAM_IndexWrite(void);
void FRAM_DoRecordWrite(void);
void FRAM_MaxDemWrite(void);
void FRAM_EnergyRecordWrite(void);
void FRAM_EnergyRecordSeek(u8 RefKind,u8 RefMonth);
u8 FRAM_I2C_ReadData(u16 phyAddr, u8 *pReadData, u16 Length);
u16 FramEnergy_Crc(u8 *pData);


void FramInit(void);

//void FRAM_Erase_Sector(u16 Addr, u16 Num);
//void FRAM_Erase_Chip(void);

#endif /* _DRVFRAM_H */

//===========================================================================
// No more.
//===========================================================================

