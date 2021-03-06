
#ifndef _MEMDATAOPERATE_H
#define _MEMDATAOPERATE_H

/* Includes ------------------------------------------------------------------*/
#include "Macro.h"
#include "param.h"
/* Exported macro ------------------------------------------------------------*/

#define  ApplicationAddress     (0x8000000+0x0000)  //应用程序开始地址
#define  BootLoaderAddress      (0x8000000+0xD800)
#define CONFIG_STARTADDR        (0x0801FC00) // 第127k
#define CALIB_STARTADDR         (0x0801F800) // 第126k
#define FLASH_PARAM_SIZE	    256	            // 最大参数长度

#define MEM_JanEn_sADDR      (0x0801F000)      // 1月记录地址       124k
#define MEM_Energy_SIZE	            624    // 最大值参数长度 31*20+4（校验位）




// ===========================================================
void MEM_EnergyRecordWrite(void);
void MEM_EnergyRecordRead(void);
BOOL MemEnergy_Write_To_Flash(u32 Addr, u8 *pInData, u16 Count);

/* Exported functions ------------------------------------------------------- */
void MemData_Init(void);
void MemData_DefaultSettingParam(void);
// 外部调用,读取数据
BOOL MemData_WriteActNumToFlash(void);
void MemData_WriteParam(void);
void MemData_ReadParam(void);
void MemData_WriteCalib(u8 *pScrData, u16 Count);     //校表
void MemData_ReadCalib(u8 *pDstData, u16 Count);
void MemData_SetDefaultParam(PARAM_T *pSettingParam, BOOL bInit);

/* Exported variables ------------------------------------------------------- */

extern volatile BOOL vg_bLostFault;		// 掉电故障

//////////////////////////////////////////////////////////////////////////
//extern Device_ProtectParamStruct  vg_DeviceProtectParam; //设备保护参数
//
extern u16 g_CTRatio;
extern u8 g_SimTestFlag;			// 全局模拟测试变量
extern u8 g_ManualFlag;			    // 全局模拟测试变量

#endif /* _MEMDATADEF_H */

//===========================================================================
// No more.
//===========================================================================


