#include "usart.h"
#include "Emm_V5.h"

__IO bool rxFrameFlag = false;
__IO uint8_t rxCmd[FIFO_SIZE] = {0};
__IO uint8_t rxCount = 0;

__IO bool receiving = true;
__IO uint8_t rx_buffer[100];
__IO uint8_t rx_index = 0;

/**
	* @brief   USART1中断函数
	* @param   无
	* @retval  无
	*/
void USART1_IRQHandler(void)
{
	__IO uint16_t i = 0;

/**********************************************************
***	串口接收中断
**********************************************************/
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		// 未完成一帧数据接收，数据进入缓冲队列
		fifo_enQueue((uint8_t)USART1->DR);

		// 清除串口接收中断
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

/**********************************************************
***	串口空闲中断
**********************************************************/
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		// 先读SR再读DR，清除IDLE中断
		USART1->SR; USART1->DR;

		// 提取一帧数据命令
		rxCount = fifo_queueLength(); for(i=0; i < rxCount; i++) { rxCmd[i] = fifo_deQueue(); }

		// 一帧数据接收完成，置位帧标志位
		rxFrameFlag = true;
	}
}

void USART2_IRQHandler(void) {
  static uint16_t prev_rx_data = 0;  // 用于存储前一个接收的数据字节

  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
    uint8_t rx_data = USART_ReceiveData(USART2);  // 接收当前数据字节

    // 将前一个数据字节与当前数据字节组合成一个16位的值
    uint16_t combined_data = (prev_rx_data << 8) | rx_data;

    if (combined_data == 0xaaaa) {
      // 检测到起始标志0xaaaa，开始接收数据
      receiving = true;
      rx_index = 0;
    } else if (combined_data == 0xffff && receiving) {
      // 检测到结束标志0xffff，并且处于接收状态，处理接收到的数据
      if (rx_index > 0 && rx_buffer[rx_index - 1] == 0xff) {
        rx_index--;  // 删除缓冲区中最后一个字节0xff
      }
      process_received_data(rx_buffer, rx_index);
      receiving = false;  // 重置接收状态
    } else if (receiving) {
      // 将当前数据字节存入接收缓冲区
      rx_buffer[rx_index++] = rx_data;
      if (rx_index >= 100) {
        // 如果接收缓冲区溢出，重置接收状态
        receiving = false;
      }
    }

    // 更新前一个接收的数据字节
    prev_rx_data = rx_data;
  }
}


/**
	* @brief   USART发送多个字节
	* @param   无
	* @retval  无
	*/
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
	__IO uint8_t i = 0;
	
	for(i=0; i < len; i++) { usart_SendByte(cmd[i]); }
}

/**
	* @brief   USART发送一个字节
	* @param   无
	* @retval  无
	*/
void usart_SendByte(uint16_t data)
{
	__IO uint16_t t0 = 0;
	
	USART1->DR = (data & (uint16_t)0x01FF);

	while(!(USART1->SR & USART_FLAG_TXE))
	{
		++t0; if(t0 > 8000)	{	return; }
	}
}


