#include "Emm_V5.h"
#include "delay.h"


/**
  * @brief    ����ǰλ������
  * @param    addr  �������ַ
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Reset_CurPos_To_Zero(uint8_t addr)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x0A;                       // ������
  cmd[2] =  0x6D;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 4);
}

/**
  * @brief    �����ת����
  * @param    addr  �������ַ
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Reset_Clog_Pro(uint8_t addr)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x0E;                       // ������
  cmd[2] =  0x52;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 4);
}

/**
  * @brief    ��ȡϵͳ����
  * @param    addr  �������ַ
  * @param    s     ��ϵͳ��������
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Read_Sys_Params(uint8_t addr, SysParams_t s)
{
  uint8_t i = 0;
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[i] = addr; ++i;                   // ��ַ

  switch(s)                             // ������
  {
    case S_VER  : cmd[i] = 0x1F; ++i; break;
    case S_RL   : cmd[i] = 0x20; ++i; break;
    case S_PID  : cmd[i] = 0x21; ++i; break;
    case S_VBUS : cmd[i] = 0x24; ++i; break;
    case S_CPHA : cmd[i] = 0x27; ++i; break;
    case S_ENCL : cmd[i] = 0x31; ++i; break;
    case S_TPOS : cmd[i] = 0x33; ++i; break;
    case S_VEL  : cmd[i] = 0x35; ++i; break;
    case S_CPOS : cmd[i] = 0x36; ++i; break;
    case S_PERR : cmd[i] = 0x37; ++i; break;
    case S_FLAG : cmd[i] = 0x3A; ++i; break;
    case S_ORG  : cmd[i] = 0x3B; ++i; break;
    case S_Conf : cmd[i] = 0x42; ++i; cmd[i] = 0x6C; ++i; break;
    case S_State: cmd[i] = 0x43; ++i; cmd[i] = 0x7A; ++i; break;
    default: break;
  }

  cmd[i] = 0x6B; ++i;                   // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, i);
}

/**
  * @brief    �޸Ŀ���/�ջ�����ģʽ
  * @param    addr     �������ַ
  * @param    svF      ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    ctrl_mode������ģʽ����Ӧ��Ļ�ϵ�P_Pul�˵�����0�ǹر������������ţ�1�ǿ���ģʽ��2�Ǳջ�ģʽ��3����En�˿ڸ���Ϊ��Ȧ��λ�����������ţ�Dir�˿ڸ���Ϊ��λ����ߵ�ƽ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Modify_Ctrl_Mode(uint8_t addr, bool svF, uint8_t ctrl_mode)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x46;                       // ������
  cmd[2] =  0x69;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  ctrl_mode;                  // ����ģʽ����Ӧ��Ļ�ϵ�P_Pul�˵�����0�ǹر������������ţ�1�ǿ���ģʽ��2�Ǳջ�ģʽ��3����En�˿ڸ���Ϊ��Ȧ��λ�����������ţ�Dir�˿ڸ���Ϊ��λ����ߵ�ƽ����
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 6);
}

/**
  * @brief    ʹ���źſ���
  * @param    addr  �������ַ
  * @param    state ��ʹ��״̬     ��trueΪʹ�ܵ����falseΪ�رյ��
  * @param    snF   �����ͬ����־ ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_En_Control(uint8_t addr, bool state, bool snF)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xF3;                       // ������
  cmd[2] =  0xAB;                       // ������
  cmd[3] =  (uint8_t)state;             // ʹ��״̬
  cmd[4] =  snF;                        // ���ͬ���˶���־
  cmd[5] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 6);
}

/**
  * @brief    �ٶ�ģʽ
  * @param    addr�������ַ
  * @param    dir ������       ��0ΪCW������ֵΪCCW
  * @param    vel ���ٶ�       ����Χ0 - 5000RPM
  * @param    acc �����ٶ�     ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    snF �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF)
{
  uint8_t cmd[16] = {0};

  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xF6;                       // ������
  cmd[2] =  dir;                        // ����
  cmd[3] =  (uint8_t)(vel >> 8);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[4] =  (uint8_t)(vel >> 0);        // �ٶ�(RPM)��8λ�ֽ�
  cmd[5] =  acc;                        // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6] =  snF;                        // ���ͬ���˶���־
  cmd[7] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 8);
}

/**
  * @brief    λ��ģʽ
  * @param    addr�������ַ
  * @param    dir ������        ��0ΪCW������ֵΪCCW
  * @param    vel ���ٶ�(RPM)   ����Χ0 - 5000RPM
  * @param    acc �����ٶ�      ����Χ0 - 255��ע�⣺0��ֱ������
  * @param    clk ��������      ����Χ0- (2^32 - 1)��
  * @param    raF ����λ/���Ա�־��falseΪ����˶���trueΪ����ֵ�˶�
  * @param    snF �����ͬ����־ ��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF)
{
  uint8_t cmd[16] = {0};

  // װ������
  cmd[0]  =  addr;                      // ��ַ
  cmd[1]  =  0xFD;                      // ������
  cmd[2]  =  dir;                       // ����
  cmd[3]  =  (uint8_t)(vel >> 8);       // �ٶ�(RPM)��8λ�ֽ�
  cmd[4]  =  (uint8_t)(vel >> 0);       // �ٶ�(RPM)��8λ�ֽ� 
  cmd[5]  =  acc;                       // ���ٶȣ�ע�⣺0��ֱ������
  cmd[6]  =  (uint8_t)(clk >> 24);      // ������(bit24 - bit31)
  cmd[7]  =  (uint8_t)(clk >> 16);      // ������(bit16 - bit23)
  cmd[8]  =  (uint8_t)(clk >> 8);       // ������(bit8  - bit15)
  cmd[9]  =  (uint8_t)(clk >> 0);       // ������(bit0  - bit7 )
  cmd[10] =  raF;                       // ��λ/���Ա�־��falseΪ����˶���trueΪ����ֵ�˶�
  cmd[11] =  snF;                       // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[12] =  0x6B;                      // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 13);
}

/**
  * @brief    ����ֹͣ�����п���ģʽ��ͨ�ã�
  * @param    addr  �������ַ
  * @param    snF   �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Stop_Now(uint8_t addr, bool snF)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xFE;                       // ������
  cmd[2] =  0x98;                       // ������
  cmd[3] =  snF;                        // ���ͬ���˶���־
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 5);
}

/**
  * @brief    ���ͬ���˶�
  * @param    addr  �������ַ
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Synchronous_motion(uint8_t addr)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0xFF;                       // ������
  cmd[2] =  0x66;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 4);
}

/**
  * @brief    ���õ�Ȧ��������λ��
  * @param    addr  �������ַ
  * @param    svF   ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Set_O(uint8_t addr, bool svF)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x93;                       // ������
  cmd[2] =  0x88;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 5);
}

/**
  * @brief    �޸Ļ������
  * @param    addr  �������ַ
  * @param    svF   ���Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  * @param    o_mode ������ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ������㣬2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  * @param    o_dir  �����㷽��0ΪCW������ֵΪCCW
  * @param    o_vel  �������ٶȣ���λ��RPM��ת/���ӣ�
  * @param    o_tm   �����㳬ʱʱ�䣬��λ������
  * @param    sl_vel ������λ��ײ������ת�٣���λ��RPM��ת/���ӣ�
  * @param    sl_ma  ������λ��ײ�������������λ��Ma��������
  * @param    sl_ms  ������λ��ײ������ʱ�䣬��λ��Ms�����룩
  * @param    potF   ���ϵ��Զ��������㣬falseΪ��ʹ�ܣ�trueΪʹ��
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Modify_Params(uint8_t addr, bool svF, uint8_t o_mode, uint8_t o_dir, uint16_t o_vel, uint32_t o_tm, uint16_t sl_vel, uint16_t sl_ma, uint16_t sl_ms, bool potF)
{
  uint8_t cmd[32] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x4C;                       // ������
  cmd[2] =  0xAE;                       // ������
  cmd[3] =  svF;                        // �Ƿ�洢��־��falseΪ���洢��trueΪ�洢
  cmd[4] =  o_mode;                     // ����ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ������㣬2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  cmd[5] =  o_dir;                      // ���㷽��
  cmd[6]  =  (uint8_t)(o_vel >> 8);     // �����ٶ�(RPM)��8λ�ֽ�
  cmd[7]  =  (uint8_t)(o_vel >> 0);     // �����ٶ�(RPM)��8λ�ֽ� 
  cmd[8]  =  (uint8_t)(o_tm >> 24);     // ���㳬ʱʱ��(bit24 - bit31)
  cmd[9]  =  (uint8_t)(o_tm >> 16);     // ���㳬ʱʱ��(bit16 - bit23)
  cmd[10] =  (uint8_t)(o_tm >> 8);      // ���㳬ʱʱ��(bit8  - bit15)
  cmd[11] =  (uint8_t)(o_tm >> 0);      // ���㳬ʱʱ��(bit0  - bit7 )
  cmd[12] =  (uint8_t)(sl_vel >> 8);    // ����λ��ײ������ת��(RPM)��8λ�ֽ�
  cmd[13] =  (uint8_t)(sl_vel >> 0);    // ����λ��ײ������ת��(RPM)��8λ�ֽ� 
  cmd[14] =  (uint8_t)(sl_ma >> 8);     // ����λ��ײ���������(Ma)��8λ�ֽ�
  cmd[15] =  (uint8_t)(sl_ma >> 0);     // ����λ��ײ���������(Ma)��8λ�ֽ� 
  cmd[16] =  (uint8_t)(sl_ms >> 8);     // ����λ��ײ������ʱ��(Ms)��8λ�ֽ�
  cmd[17] =  (uint8_t)(sl_ms >> 0);     // ����λ��ײ������ʱ��(Ms)��8λ�ֽ�
  cmd[18] =  potF;                      // �ϵ��Զ��������㣬falseΪ��ʹ�ܣ�trueΪʹ��
  cmd[19] =  0x6B;                      // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 20);
}

/**
  * @brief    ��������
  * @param    addr   �������ַ
  * @param    o_mode ������ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ������㣬2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  * @param    snF   �����ͬ����־��falseΪ�����ã�trueΪ����
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Trigger_Return(uint8_t addr, uint8_t o_mode, bool snF)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x9A;                       // ������
  cmd[2] =  o_mode;                     // ����ģʽ��0Ϊ��Ȧ�ͽ����㣬1Ϊ��Ȧ������㣬2Ϊ��Ȧ����λ��ײ���㣬3Ϊ��Ȧ����λ���ػ���
  cmd[3] =  snF;                        // ���ͬ���˶���־��falseΪ�����ã�trueΪ����
  cmd[4] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 5);
}

/**
  * @brief    ǿ���жϲ��˳�����
  * @param    addr  �������ַ
  * @retval   ��ַ + ������ + ����״̬ + У���ֽ�
  */
void Emm_V5_Origin_Interrupt(uint8_t addr)
{
  uint8_t cmd[16] = {0};
  
  // װ������
  cmd[0] =  addr;                       // ��ַ
  cmd[1] =  0x9C;                       // ������
  cmd[2] =  0x48;                       // ������
  cmd[3] =  0x6B;                       // У���ֽ�
  
  // ��������
  usart_SendCmd(cmd, 4);
}

void Motion_Control(uint16_t Left_Velocity, uint16_t Right_Velocity, uint32_t Left_Distance, uint32_t Right_Distance)
{
/**********************************************************
***	���������ͬʱת���������ٶȣ������ٶȣ�����Ȧ��*3200������Ȧ��*3200
**********************************************************/	
	
	// ��������������顢�������ݳ���
  uint8_t rxCmd[128] = {0}; uint8_t rxCount = 0;
	uint16_t timeout = 0;

	//λ��ģʽ��1�ŵ��������CW���ٶ�1000RPM�����ٶ�1000RPM/S2��������3200��16ϸ���·���3200��������תһȦ��������˶������ͬ���˶�ģʽ
  Emm_V5_Pos_Control(1, 0, Left_Velocity, 236, Left_Distance, 0, 1);
	
	//�ȴ���������������ݻ���������rxCmd�ϣ�����ΪrxCount
	while(rxFrameFlag == false)
	{
		// �ȴ�100���뻹δ��λ�����ж�Ϊû�����ݷ��أ���ʱ�˳�
		delay_ms(1); ++timeout; if(timeout > 100) { rxCount = 0; break; }
	}
	rxFrameFlag = false;
	delay_ms(10);

	//λ��ģʽ��2�ŵ��������CCW���ٶ�1000RPM�����ٶ�1000RPM/S2��������3200��16ϸ���·���3200��������תһȦ��������˶������ͬ���˶�ģʽ
  Emm_V5_Pos_Control(2, 1, Right_Velocity, 236, Right_Distance, 0, 1);
	
	//�ȴ���������������ݻ���������rxCmd�ϣ�����ΪrxCount
	while(rxFrameFlag == false)
	{
		// �ȴ�100���뻹δ��λ�����ж�Ϊû�����ݷ��أ���ʱ�˳�
		delay_ms(1); ++timeout; if(timeout > 100) { rxCount = 0; break; }
	}
	rxFrameFlag = false;
	delay_ms(10);

	//�ù㲥��ַ0������������ͬ����ʼ�˶������ַ1�͵�ַ2���Ҫͬʱ��ʼ�˶������������ᶯ
  Emm_V5_Synchronous_motion(0);
	
	//�ȴ���������������ݻ���������rxCmd�ϣ�����ΪrxCount
	while(rxFrameFlag == false)
	{
		// �ȴ�100���뻹δ��λ�����ж�Ϊû�����ݷ��أ���ʱ�˳�
		delay_ms(1); ++timeout; if(timeout > 100) { rxCount = 0; break; }
	}
	rxFrameFlag = false;
	delay_ms(10);
	
	// ��֤У���ֽ�
  if(rxCmd[rxCount - 1] == 0x6B) {  } else {  }
	
	//�ȴ���ת
	uint32_t time1 = 75*Left_Distance/Left_Velocity/4;
	uint32_t time2 = 75*Right_Distance/Right_Velocity/4;
	delay_ms(time1 > time2 ? time1 : time2);
	delay_ms(500); //���ӵȴ�����ֹ����
}

void Straight_Forward(uint16_t velocity, uint32_t distance)
{
/**********************************************************
***	velocity				��ʻ�ٶȣ���λ��RPM
*** distance				��ʻ���룬��λ��cm
**********************************************************/	
	uint8_t perimeter = 204; //����һȦ��·�̣���λ������
	uint32_t loop = 32000*distance/perimeter;
	Motion_Control(velocity,velocity,loop,loop);
}


void Turn(uint8_t direction, uint8_t angle, uint16_t velocity)
{
/**********************************************************
***	direction		ת�䷽��0Ϊ��ת��1Ϊ��ת
*** angle				ת��Ƕȣ��Ƕ�ֵ��0-360�ȣ�
*** velocity		ת���ٶȣ���λ��RPM
**********************************************************/	
	uint8_t radius_wheel = 65; //����ֱ������λ������
	uint8_t width = 169; //������ȣ���λ������
	uint32_t loop = 3200*angle*width/360/radius_wheel; //������
	
	// ��������������顢�������ݳ���
  uint8_t rxCmd[128] = {0}; uint8_t rxCount = 0;
	uint16_t timeout = 0;

	//λ��ģʽ��1�ŵ��������CW���ٶ�1000RPM�����ٶ�1000RPM/S2��������3200��16ϸ���·���3200��������תһȦ��������˶������ͬ���˶�ģʽ
  Emm_V5_Pos_Control(1, direction, velocity, 236, loop, 0, 1);
	
	//�ȴ���������������ݻ���������rxCmd�ϣ�����ΪrxCount
	while(rxFrameFlag == false)
	{
		// �ȴ�100���뻹δ��λ�����ж�Ϊû�����ݷ��أ���ʱ�˳�
		delay_ms(1); ++timeout; if(timeout > 100) { rxCount = 0; break; }
	}
	rxFrameFlag = false;
	delay_ms(10);

	//λ��ģʽ��2�ŵ��������CCW���ٶ�1000RPM�����ٶ�1000RPM/S2��������3200��16ϸ���·���3200��������תһȦ��������˶������ͬ���˶�ģʽ
  Emm_V5_Pos_Control(2, direction, velocity, 236, loop, 0, 1);
	
	//�ȴ���������������ݻ���������rxCmd�ϣ�����ΪrxCount
	while(rxFrameFlag == false)
	{
		// �ȴ�100���뻹δ��λ�����ж�Ϊû�����ݷ��أ���ʱ�˳�
		delay_ms(1); ++timeout; if(timeout > 100) { rxCount = 0; break; }
	}
	rxFrameFlag = false;
	delay_ms(10);

	//�ù㲥��ַ0������������ͬ����ʼ�˶������ַ1�͵�ַ2���Ҫͬʱ��ʼ�˶������������ᶯ
  Emm_V5_Synchronous_motion(0);
	
	//�ȴ���������������ݻ���������rxCmd�ϣ�����ΪrxCount
	while(rxFrameFlag == false)
	{
		// �ȴ�100���뻹δ��λ�����ж�Ϊû�����ݷ��أ���ʱ�˳�
		delay_ms(1); ++timeout; if(timeout > 100) { rxCount = 0; break; }
	}
	rxFrameFlag = false;
	delay_ms(10);
	
	// ��֤У���ֽ�
  if(rxCmd[rxCount - 1] == 0x6B) {  } else {  }
	
	//�ȴ���ת
	delay_ms(75*loop/velocity/4);
	delay_ms(1000); //���ӵȴ�����ֹ����
}

void process_received_data(uint8_t *data, uint8_t length) {
	uint8_t i = 0;
	for(i = 0; i < length; i = i + 2) {
		if(data[i] == 0x10) {
			Turn(1,data[i+1],50);
		}	else if(data[i] == 0x20) {
			Turn(0,data[i+1],50);
		} else if((data[i] & 0xF0) == 0x00) {
			Straight_Forward(50,(data[i] << 8) | data[i+1]);
		}
	}
}
