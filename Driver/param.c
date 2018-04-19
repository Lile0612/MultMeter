/*
*********************************************************************************************************
*
*	模块名称 : 应用程序参数模块
*	文件名称 : param.c
*	版     本 : V1.0
*	说     明 : 读取和保存应用程序的参数
*	修改记录 :
*	            版本号      日期            作者        说明
*	            V1.0    2017-10-27       l、     正式发布
*
*	Copyright (C), 2015-2020   www.yidek.com
*
*********************************************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "Include.h"

/* Public variables ---------------------------------------------------------*/

float RtuPrimaryData[36];       // 一次侧数据
u16 RtuPrimaryDataTmp[72];      // 一次侧数据转换
s16 RtuSecondaryData[192];      // 二次侧数据
s16 RtuTHDData[102];            // 谐波数据
s16 RtuCTRLData[64];            // 设置参数
s16 RtuCTRLDataTemp[64];        // 设置参数
s16 RtuOUTData[4];              // 继电器状态
s16 RtuINData[4];               // 开关量状态

Energy_Memory ElectricEnergy;
PARAM_T g_tParam;
SX_PARAM DispCtrlParam;
u8 FourQuadtand;
u8 IbEnbleFlag = FALSE;
/* Private functions ---------------------------------------------------------*/

/*
*********************************************************************************************************
*	函 数 名: InitCtrlParam
*	功能说明: 控制参数初始化
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void InitCtrlParam(void)
{
    g_tParam.CtrlParam.INIT = 0x55; //初始化标识位
    g_tParam.CtrlParam.Code = 1;
    g_tParam.CtrlParam.NetMode = N34;
    g_tParam.CtrlParam.PTNum = 1;
    g_tParam.CtrlParam.CTNum = 1;
    g_tParam.CtrlParam.Addr485 = 0x01;
    g_tParam.CtrlParam.Baud485 = Baud48;
    g_tParam.CtrlParam.DataFormat = n81;
    g_tParam.CtrlParam.DataProtocol = calib;
    g_tParam.CtrlParam.Cycle = SaveNO;
#ifdef _GUILCD_
    g_tParam.CtrlParam.BlackTime = 60;
#else
    g_tParam.CtrlParam.BlackTime = BrightLevel_1;
#endif
    g_tParam.CtrlParam.Alarm = 120;
    g_tParam.CtrlParam.DefaultDisp = DISP_UINT_U;

    g_tParam.CtrlParam.DO1_Mode = DO_MODE_OFF;
    g_tParam.CtrlParam.DO1_Item = DO_ITEM_Ua_H;
    g_tParam.CtrlParam.DO1_Time = 0;
    g_tParam.CtrlParam.DO1_UAL = 0;
    g_tParam.CtrlParam.DO1_HYS = 0;
    g_tParam.CtrlParam.DO1_Delay = 0;

    g_tParam.CtrlParam.DO2_Mode = DO_MODE_OFF;
    g_tParam.CtrlParam.DO2_Item = DO_ITEM_Ua_H;
    g_tParam.CtrlParam.DO2_Time = 0;
    g_tParam.CtrlParam.DO2_UAL = 0;
    g_tParam.CtrlParam.DO2_HYS = 0;
    g_tParam.CtrlParam.DO2_Delay = 0;

    g_tParam.CtrlParam.DO3_Mode = DO_MODE_OFF;
    g_tParam.CtrlParam.DO3_Item = DO_ITEM_Ua_H;
    g_tParam.CtrlParam.DO3_Time = 0;
    g_tParam.CtrlParam.DO3_UAL = 0;
    g_tParam.CtrlParam.DO3_HYS = 0;
    g_tParam.CtrlParam.DO3_Delay = 0;

    g_tParam.CtrlParam.DO4_Mode = DO_MODE_OFF;
    g_tParam.CtrlParam.DO4_Item = DO_ITEM_Ua_H;
    g_tParam.CtrlParam.DO4_Time = 0;
    g_tParam.CtrlParam.DO4_UAL = 0;
    g_tParam.CtrlParam.DO4_HYS = 0;
    g_tParam.CtrlParam.DO4_Delay = 0;

    g_tParam.CtrlParam.AO1_Mode = AO_MODE_OFF;
    g_tParam.CtrlParam.AO1_Item = AO_ITEM_UA;
    g_tParam.CtrlParam.AO1_DS = 0;
    g_tParam.CtrlParam.AO1_FS = 0;

    g_tParam.CtrlParam.AO2_Mode = AO_MODE_OFF;
    g_tParam.CtrlParam.AO2_Item = AO_ITEM_UA;
    g_tParam.CtrlParam.AO2_DS = 0;
    g_tParam.CtrlParam.AO2_FS = 0;

    g_tParam.CtrlParam.AdjustFinishFlag = FALSE;
    g_tParam.CtrlParam.AnaCalibFlag = FALSE;
}

void InitRNParam_Phase(u8 Phase) //单相初始化
{
    g_tParam.RN_Coefficients.HFConst1 = RNPARAM_HFCONST1;

    if (Phase < 3)
    {
        g_tParam.RN_Coefficients.GSU[Phase] = 0x2000;// 10210;
        g_tParam.RN_Coefficients.GSI[Phase] = 0xC000;// 49152;
        g_tParam.RN_Coefficients.PHSI[Phase] = 0x808080;// 65505;
        g_tParam.RN_Coefficients.P_PHSL[Phase] = 0x00;
        g_tParam.RN_Coefficients.P_PHSM[Phase] = 0x00;
        g_tParam.RN_Coefficients.P_PHSH[Phase] = 0x00;
    }
}

void InitRNParam_PhaseToZero(u8 Phase)
{
    g_tParam.RN_Coefficients.HFConst1 = 0x1000;

    if (Phase < 3)
    {
        g_tParam.RN_Coefficients.GSU[Phase] = 0;// 10210;
        g_tParam.RN_Coefficients.GSI[Phase] = 0;// 59652;
        g_tParam.RN_Coefficients.PHSI[Phase] = 0x808080;// 65505;  //电流默认值
        g_tParam.RN_Coefficients.P_PHSL[Phase] = 0x00;
        g_tParam.RN_Coefficients.P_PHSM[Phase] = 0x00;
        g_tParam.RN_Coefficients.P_PHSH[Phase] = 0x00;
    }
}

void InitRNParam(void) //三相初始化
{
    int ph;

    for (ph = 0; ph < 3; ph++)
    {
        InitRNParam_Phase(ph);
    }
}

void InitRNParamToZero(void)
{
    int ph;
    for (ph = 0; ph < 3; ph++)
    {
        InitRNParam_PhaseToZero(ph);
    }
}
void WriteRNParam(u8 *pWriteData) //校准参数格式转换
{
    g_tParam.RN_Coefficients.HFConst1 = FLIPDW(pWriteData);

    g_tParam.RN_Coefficients.GSU[0] = FLIPDW(pWriteData+4);
    g_tParam.RN_Coefficients.GSI[0] = FLIPDW(pWriteData+8);
    g_tParam.RN_Coefficients.PHSI[0] = FLIPDW(pWriteData+12);

    g_tParam.RN_Coefficients.GSU[1] = FLIPDW(pWriteData+16);
    g_tParam.RN_Coefficients.GSI[1] = FLIPDW(pWriteData+20);
    g_tParam.RN_Coefficients.PHSI[1] = FLIPDW(pWriteData+24);

    g_tParam.RN_Coefficients.GSU[2] = FLIPDW(pWriteData+28);
    g_tParam.RN_Coefficients.GSI[2] = FLIPDW(pWriteData+32);
    g_tParam.RN_Coefficients.PHSI[2] = FLIPDW(pWriteData+36);
}



/*
*********************************************************************************************************
*	函 数 名: LoadParam
*	功能说明: 从Flash读参数到g_tParam
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void LoadParam(void)
{

#ifdef PARAM_SAVE_TO_FLASH
	/* 读取CPU Flash中的参数 */
    MemData_ReadParam();
#endif

#ifdef PARAM_SAVE_TO_EEPROM
	/* 读取EEPROM中的参数 */
	ee_ReadBytes((uint8_t *)&g_tParam, PARAM_ADDR, sizeof(PARAM_T));
#endif

}

/*
*********************************************************************************************************
*	函 数 名: SaveParam
*	功能说明: 将全局变量g_tParam 写入到CPU内部Flash
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void SaveParam(void)
{
#ifdef PARAM_SAVE_TO_FLASH
	/* 将全局的参数变量保存到 CPU Flash */
	MemData_WriteParam();
#endif

#ifdef PARAM_SAVE_TO_EEPROM
	/* 将全局的参数变量保存到EEPROM */
	ee_WriteBytes((uint8_t *)&g_tParam, PARAM_ADDR, sizeof(PARAM_T));
#endif
}


