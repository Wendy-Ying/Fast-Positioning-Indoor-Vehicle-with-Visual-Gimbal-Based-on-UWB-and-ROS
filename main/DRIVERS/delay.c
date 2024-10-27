#include "delay.h"

/**
	*	@brief		毫秒级延时
	*	@param		int32_t u32Cnt
	*	@retval		无
	*/
void delay_ms(int32_t i32Cnt)
{
	__IO int32_t i32end = 0;

	SysTick->LOAD = 0xFFFFFF;
	SysTick->VAL  = 0;
	SysTick->CTRL = (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);

	while(i32Cnt > 0)
	{
		SysTick->VAL = 0;
		i32end = 0x1000000 - (SystemCoreClock / 1000);
		while(SysTick->VAL > i32end);
		--i32Cnt;
	}

	SysTick->CTRL = (SysTick->CTRL & (~SysTick_CTRL_ENABLE_Msk));
}

/**
	*	@brief		软件延时
	*	@param		int32_t u32Cnt
	*	@retval		无
	*/
void delay_cnt(int32_t i32Cnt)
{
	while(i32Cnt > 0) { i32Cnt--; }
}
