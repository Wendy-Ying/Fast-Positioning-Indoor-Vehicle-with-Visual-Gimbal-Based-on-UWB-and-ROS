#include "board.h"


/**
	* @brief   配置NVIC控制器
	* @param   无
	* @retval  无
	*/
void nvic_init(void)
{	
	// 4bit抢占优先级位
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 配置 USART1 中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	// 配置 USART2 中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/**
	*	@brief		外设时钟初始化
	*	@param		无
	*	@retval		无
	*/
void clock_init(void)
{
	// 使能GPIOA、AFIO外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	// 使能USART1外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  // 使能USART2外设时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// 禁用JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

/**
	* @brief   初始化USART
	* @param   无
	* @retval  无
	*/
void usart_init(void)
{
/**********************************************************
***	初始化USART1引脚
**********************************************************/
	// PA9 - USART1_TX
	GPIO_InitTypeDef  GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;				/* 复用推挽输出 */
	GPIO_Init(GPIOA, &GPIO_InitStructure1);
	// PA10 - USART1_RX
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;					/* 浮空输入 */
	GPIO_Init(GPIOA, &GPIO_InitStructure1);

/**********************************************************
***	初始化USART1
**********************************************************/
	USART_InitTypeDef USART_InitStructure1;
	USART_InitStructure1.USART_BaudRate = 115200;
	USART_InitStructure1.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure1.USART_StopBits = USART_StopBits_1;
	USART_InitStructure1.USART_Parity = USART_Parity_No;
	USART_InitStructure1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure1.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART1, &USART_InitStructure1);

/**********************************************************
***	清除USART1中断
**********************************************************/
	USART1->SR; USART1->DR;
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);

/**********************************************************
***	使能USART1中断
**********************************************************/	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

/**********************************************************
***	使能USART1
**********************************************************/
	USART_Cmd(USART1, ENABLE);
	
/**********************************************************
***	初始化USART2引脚
**********************************************************/
	// PA2 - USART2_TX
	GPIO_InitTypeDef  GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;				/* 复用推挽输出 */
	GPIO_Init(GPIOA, &GPIO_InitStructure2);
	// PA3 - USART2_RX
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPU;					/* 浮空输入 */
	GPIO_Init(GPIOA, &GPIO_InitStructure2);

/**********************************************************
***	初始化USART2
**********************************************************/
	USART_InitTypeDef USART_InitStructure2;
	USART_InitStructure2.USART_BaudRate = 115200;
	USART_InitStructure2.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure2.USART_StopBits = USART_StopBits_1;
	USART_InitStructure2.USART_Parity = USART_Parity_No;
	USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure2.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART2, &USART_InitStructure2);

/**********************************************************
***	清除USART2中断
**********************************************************/
	USART2->SR; USART2->DR;
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);

/**********************************************************
***	使能USART2中断
**********************************************************/	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

/**********************************************************
***	使能USART2
**********************************************************/
	USART_Cmd(USART2, ENABLE);
	
}

/**
	*	@brief		板载初始化
	*	@param		无
	*	@retval		无
	*/
void board_init(void)
{
	nvic_init();
	clock_init();
	usart_init();
}
