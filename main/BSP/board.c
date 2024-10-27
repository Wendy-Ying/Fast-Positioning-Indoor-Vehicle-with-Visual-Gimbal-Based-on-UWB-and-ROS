#include "board.h"


/**
	* @brief   ����NVIC������
	* @param   ��
	* @retval  ��
	*/
void nvic_init(void)
{	
	// 4bit��ռ���ȼ�λ
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// ���� USART1 �ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_Init(&NVIC_InitStructure);
	
	// ���� USART2 �ж�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/**
	*	@brief		����ʱ�ӳ�ʼ��
	*	@param		��
	*	@retval		��
	*/
void clock_init(void)
{
	// ʹ��GPIOA��AFIO����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	// ʹ��USART1����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
  // ʹ��USART2����ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// ����JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

/**
	* @brief   ��ʼ��USART
	* @param   ��
	* @retval  ��
	*/
void usart_init(void)
{
/**********************************************************
***	��ʼ��USART1����
**********************************************************/
	// PA9 - USART1_TX
	GPIO_InitTypeDef  GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_AF_PP;				/* ����������� */
	GPIO_Init(GPIOA, &GPIO_InitStructure1);
	// PA10 - USART1_RX
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;					/* �������� */
	GPIO_Init(GPIOA, &GPIO_InitStructure1);

/**********************************************************
***	��ʼ��USART1
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
***	���USART1�ж�
**********************************************************/
	USART1->SR; USART1->DR;
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);

/**********************************************************
***	ʹ��USART1�ж�
**********************************************************/	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

/**********************************************************
***	ʹ��USART1
**********************************************************/
	USART_Cmd(USART1, ENABLE);
	
/**********************************************************
***	��ʼ��USART2����
**********************************************************/
	// PA2 - USART2_TX
	GPIO_InitTypeDef  GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;				/* ����������� */
	GPIO_Init(GPIOA, &GPIO_InitStructure2);
	// PA3 - USART2_RX
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IPU;					/* �������� */
	GPIO_Init(GPIOA, &GPIO_InitStructure2);

/**********************************************************
***	��ʼ��USART2
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
***	���USART2�ж�
**********************************************************/
	USART2->SR; USART2->DR;
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);

/**********************************************************
***	ʹ��USART2�ж�
**********************************************************/	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

/**********************************************************
***	ʹ��USART2
**********************************************************/
	USART_Cmd(USART2, ENABLE);
	
}

/**
	*	@brief		���س�ʼ��
	*	@param		��
	*	@retval		��
	*/
void board_init(void)
{
	nvic_init();
	clock_init();
	usart_init();
}
