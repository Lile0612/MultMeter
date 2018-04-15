/* Includes ------------------------------------------------------------------*/
#include "Include.h"

DMA_InitTypeDef  USART_DMA_InitStructure;

void RCC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2
                        | RCC_APB1Periph_TIM2
                        | RCC_APB1Periph_TIM3
                        | RCC_APB1Periph_TIM4
						,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA
                        | RCC_APB2Periph_GPIOB
                        | RCC_APB2Periph_GPIOC
                        | RCC_APB2Periph_GPIOD
                        | RCC_APB2Periph_GPIOF
                        | RCC_APB2Periph_TIM1
                        | RCC_APB2Periph_AFIO
                        | RCC_APB2Periph_USART1
                        ,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1
                        | RCC_AHBPeriph_DMA2
						,ENABLE);


}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
	// USART1
    /* Configure USART1 Rx (PA.10) as input floating */   //第1路485
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
	// Key
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_InitStructure.GPIO_Pin = PIN_KEY_ONE;      //PB6/5/4/3
	GPIO_Init(PORT_KEY_ONE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_KEY_TWO;
	GPIO_Init(PORT_KEY_TWO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_KEY_THREE;
	GPIO_Init(PORT_KEY_THREE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(PORT_KEY_FOUR, &GPIO_InitStructure);

#ifdef _GUILED_
    //-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	// 显示1640
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Pin = PIN_DIN;
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CLK;
	GPIO_Init(PORT_CLK, &GPIO_InitStructure);
#endif
    //-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//RN8302
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_InitStructure.GPIO_Pin = PIN_SPI_MOSI;
	GPIO_Init(PORT_SPI_MOSI, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PIN_SPI_SCK;
	GPIO_Init(PORT_SPI_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_SPI_CSN;
	GPIO_Init(PORT_SPI_CSN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_SPI_RST;
	GPIO_Init(PORT_SPI_RST, &GPIO_InitStructure);

	RN_SPI_CSN_H;
	RN_SPI_RST_H;
	//RN_SPI_SCK_H;
	//RN_SPI_MOSI_H;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	GPIO_InitStructure.GPIO_Pin = PIN_SPI_INTN;
	GPIO_Init(PORT_SPI_INTN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RN_PIN_MISO;
	GPIO_Init(RN_PORT_MISO, &GPIO_InitStructure);

	//Fram
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;

    GPIO_InitStructure.GPIO_Pin =  PIN_EE_SCL;
    GPIO_Init(PORT_EE_SCL, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  PIN_EE_SDA;
    GPIO_Init(PORT_EE_SDA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  PIN_EE_WP;
    GPIO_Init(PORT_EE_WP, &GPIO_InitStructure);

    //PWM1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin =  PIN_PWM1;
    GPIO_Init(PORT_PWM1, &GPIO_InitStructure);

    //PWM2
    GPIO_InitStructure.GPIO_Pin =  PIN_PWM2;
    GPIO_Init(PORT_PWM2, &GPIO_InitStructure);
    PWM1_H;
    PWM2_H;

    //DOUT
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin =  PIN_CTRL1;
    GPIO_Init(PORT_CTRL1, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  PIN_CTRL2;
    GPIO_Init(PORT_CTRL2, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  PIN_CTRL3;
    GPIO_Init(PORT_CTRL3, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin =  PIN_CTRL4;
    GPIO_Init(PORT_CTRL4, &GPIO_InitStructure);

    CTRL1_DisEnble;
    CTRL2_DisEnble;
    CTRL3_DisEnble;
    CTRL4_DisEnble;

    //DIN
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_InitStructure.GPIO_Pin = PIN_DIN_STATUS1;
	GPIO_Init(PORT_DIN_STATUS1, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN_STATUS2;
	GPIO_Init(PORT_DIN_STATUS2, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN_STATUS3;
	GPIO_Init(PORT_DIN_STATUS3, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN_STATUS4;
	GPIO_Init(PORT_DIN_STATUS4, &GPIO_InitStructure);

	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	// LCD显示
#ifdef _GUILCD_
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;


	GPIO_InitStructure.GPIO_Pin = PIN_LCD_BG;
	GPIO_Init(PORT_LCD_BG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_CS;
	GPIO_Init(PORT_LCD_CS, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_WR;
	GPIO_Init(PORT_LCD_WR, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_LCD_DATA;
	GPIO_Init(PORT_LCD_DATA, &GPIO_InitStructure);
#endif
}

void USART_DMAReConfig(u8* pDataBuf, u16 BufSize) //串口1发送数据
{
    DMA_DeInit(USARTx_TX_DMA_CHANNEL);
    USART_DMA_InitStructure.DMA_PeripheralBaseAddr = USARTx_TDR_ADDRESS;
    USART_DMA_InitStructure.DMA_BufferSize = (u32)BufSize;
    USART_DMA_InitStructure.DMA_MemoryBaseAddr = (u32)pDataBuf;
    USART_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    USART_DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_Init(USARTx_TX_DMA_CHANNEL, &USART_DMA_InitStructure);

	/* Clear the TC bit in the SR register by writing 0 to it */
	USART_ClearFlag(USART1, USART_FLAG_TC);
	/* Enable DMA1 USART Tx Channel */
	DMA_Cmd(USARTx_TX_DMA_CHANNEL, ENABLE);
}

void USART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStructure;
    // 串口1配置参数
    switch(g_tParam.CtrlParam.Baud485)
    {
        case Baud12:
            USART_InitStructure.USART_BaudRate = 1200;
            break;
        case Baud24:
            USART_InitStructure.USART_BaudRate = 2400;
            break;
        case Baud48:
            USART_InitStructure.USART_BaudRate = 4800;
            break;
        case Baud96:
            USART_InitStructure.USART_BaudRate = 9600;
            break;
        case Baud192:
            USART_InitStructure.USART_BaudRate = 19200;
            break;
        default:
            break;
    }

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	if(g_tParam.CtrlParam.DataFormat == n81)
	{
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}
	else if(g_tParam.CtrlParam.DataFormat == e81)
	{
        USART_InitStructure.USART_Parity = USART_Parity_Even;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}
	else if(g_tParam.CtrlParam.DataFormat == o81)
	{
        USART_InitStructure.USART_Parity = USART_Parity_Odd;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}
	else if(g_tParam.CtrlParam.DataFormat == n82)
	{
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_StopBits = USART_StopBits_2;
	}
	else
	{
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
	}

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // 配置串口1
    USART_Init(USART1, &USART_InitStructure);

    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    // 配置串口同步参数
    USART_ClockInit(USART1, &USART_ClockInitStructure);

    /* 串口1发送接收中断使能 */
    //USART_ITConfig(USART1,USART_IT_TC,DISABLE);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    //USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);

    //DMA1通道5配置
    DMA_DeInit(USARTx_RX_DMA_CHANNEL);
    //外设地址
    USART_DMA_InitStructure.DMA_PeripheralBaseAddr = DMA_PeripheralDataSize_HalfWord;
    //内存地址
    USART_DMA_InitStructure.DMA_MemoryBaseAddr = DMA_PeripheralDataSize_HalfWord;
    //dma传输方向单向
    USART_DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //设置DMA在传输时缓冲区的长度
    USART_DMA_InitStructure.DMA_BufferSize = 128;
    //设置DMA的外设递增模式，一个外设
    USART_DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //设置DMA的内存递增模式
    USART_DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据字长
    USART_DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    //内存数据字长
    USART_DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    //设置DMA的传输模式
    USART_DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //设置DMA的优先级别
    USART_DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //设置DMA的2个memory中的变量互相访问
    USART_DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(USARTx_RX_DMA_CHANNEL,&USART_DMA_InitStructure);

    //使能通道
    DMA_Cmd(USARTx_RX_DMA_CHANNEL,ENABLE);

     //采用DMA方式接收
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

    // 使能串口1
    USART_Cmd(USART1, ENABLE);

}



/*******************************************************************************
* Function Name  : TIM17_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;

u16 TimerPeriod = 0;
u16 Channel1Pulse = 0;
u16 Channel2Pulse = 0;

void TIM_PWM_Configuration(void)  //电流配置
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //PWM1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin =  PIN_PWM1;
    GPIO_Init(PORT_PWM1, &GPIO_InitStructure);

    //PWM2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin =  PIN_PWM2;
    GPIO_Init(PORT_PWM2, &GPIO_InitStructure);

    TimerPeriod = (SystemCoreClock / 1200 ) - 1;
    Channel1Pulse = (u16) (100 * (TimerPeriod - 1) / 10000);
    Channel2Pulse = (u16) (100 * (TimerPeriod - 1) / 10000);

    /*Time 定时基础设置*/
    TIM_TimeBaseStructure.TIM_Prescaler = 11;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /*Time 定时设置为上升沿计算模式*/
    TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
    //TIM_TimeBaseStructure.TIM_Period = 0x9C3F;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /*PWM模式设置*/
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;//使能
    //TIM_OCInitStructure.TIM_Pulse = 745;//使能18.54
    TIM_OC4Init(TIM4, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

    /*PWM模式设置*/
    //TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;//使能
    //TIM_OCInitStructure.TIM_Pulse = 15105;//使能
    TIM_OC3Init(TIM4, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);
    /*TIME17 计算器使能*/
    TIM_Cmd(TIM4, ENABLE);

    /*TIM17 主输出使能*/
    TIM_CtrlPWMOutputs(TIM4,ENABLE);
}


void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);

    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/*******************************************************************************
* Function Name  : EXTI_Configuration
* Description    : 外部中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI_Configuration(void)
{

}

void SysTick_Configuration(void)
{
    /* config systick to generate fixed time unit */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    /* SystemFrequency / 1000    1ms中断一次
     * SystemFrequency / 100000  10us中断一次
     * SystemFrequency / 1000000 1us中断一次
     */
    if (SysTick_Config(SystemCoreClock / 1000))
    {
        /* Capture error */
        while (1);
    }
}

void IWDG_Configuration(void)
{
    /* Check if the system has resumed from IWDG reset */
    if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
    {
        /* Clear reset flags */
        RCC_ClearFlag();
    }
    /* 写入0x5555,用于允许狗狗寄存器写入功能 */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

    /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
    IWDG_SetPrescaler(IWDG_Prescaler_32);

    /* Set counter reload value to 3000  2.4秒 */
    IWDG_SetReload(3000);

    /* 喂狗*/
    IWDG_ReloadCounter();

    /* 使能狗狗*/
    IWDG_Enable();
}

void Bsp_Init(void)
{
	SystemInit();
    //SystemCoreClockUpdate();
	RCC_Configuration();

	GPIO_Configuration();

	//USART_Configuration();

	EXTI_Configuration();

	NVIC_Configuration();

	SysTick_Configuration();
#ifndef  IWDG_DEBUG
	IWDG_Configuration();
#endif
    Init8025();
}


/******************* (C) EEPW.com.cn*****END OF FILE****/
