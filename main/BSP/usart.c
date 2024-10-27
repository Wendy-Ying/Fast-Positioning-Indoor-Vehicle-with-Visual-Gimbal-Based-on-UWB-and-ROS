#include "usart.h"
#include "Emm_V5.h"

__IO bool rxFrameFlag = false;
__IO uint8_t rxCmd[FIFO_SIZE] = {0};
__IO uint8_t rxCount = 0;

__IO bool receiving = true;
__IO uint8_t rx_buffer[100];
__IO uint8_t rx_index = 0;

/**
	* @brief   USART1�жϺ���
	* @param   ��
	* @retval  ��
	*/
void USART1_IRQHandler(void)
{
	__IO uint16_t i = 0;

/**********************************************************
***	���ڽ����ж�
**********************************************************/
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		// δ���һ֡���ݽ��գ����ݽ��뻺�����
		fifo_enQueue((uint8_t)USART1->DR);

		// ������ڽ����ж�
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}

/**********************************************************
***	���ڿ����ж�
**********************************************************/
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		// �ȶ�SR�ٶ�DR�����IDLE�ж�
		USART1->SR; USART1->DR;

		// ��ȡһ֡��������
		rxCount = fifo_queueLength(); for(i=0; i < rxCount; i++) { rxCmd[i] = fifo_deQueue(); }

		// һ֡���ݽ�����ɣ���λ֡��־λ
		rxFrameFlag = true;
	}
}

void USART2_IRQHandler(void) {
  static uint16_t prev_rx_data = 0;  // ���ڴ洢ǰһ�����յ������ֽ�

  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
    uint8_t rx_data = USART_ReceiveData(USART2);  // ���յ�ǰ�����ֽ�

    // ��ǰһ�������ֽ��뵱ǰ�����ֽ���ϳ�һ��16λ��ֵ
    uint16_t combined_data = (prev_rx_data << 8) | rx_data;

    if (combined_data == 0xaaaa) {
      // ��⵽��ʼ��־0xaaaa����ʼ��������
      receiving = true;
      rx_index = 0;
    } else if (combined_data == 0xffff && receiving) {
      // ��⵽������־0xffff�����Ҵ��ڽ���״̬��������յ�������
      if (rx_index > 0 && rx_buffer[rx_index - 1] == 0xff) {
        rx_index--;  // ɾ�������������һ���ֽ�0xff
      }
      process_received_data(rx_buffer, rx_index);
      receiving = false;  // ���ý���״̬
    } else if (receiving) {
      // ����ǰ�����ֽڴ�����ջ�����
      rx_buffer[rx_index++] = rx_data;
      if (rx_index >= 100) {
        // ������ջ�������������ý���״̬
        receiving = false;
      }
    }

    // ����ǰһ�����յ������ֽ�
    prev_rx_data = rx_data;
  }
}


/**
	* @brief   USART���Ͷ���ֽ�
	* @param   ��
	* @retval  ��
	*/
void usart_SendCmd(__IO uint8_t *cmd, uint8_t len)
{
	__IO uint8_t i = 0;
	
	for(i=0; i < len; i++) { usart_SendByte(cmd[i]); }
}

/**
	* @brief   USART����һ���ֽ�
	* @param   ��
	* @retval  ��
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


