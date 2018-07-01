//
// Created by SAK on 2018-06-28.
//

/*
	This file contains the entry point (Reset_Handler) of your firmware project.
	The reset handled initializes the RAM and calls system library initializers as well as
	the platform-specific initializer and the main() function.
*/

extern void *_estack;

#define NULL ((void *)0)

void Reset_Handler();
void Default_Handler();

#ifdef DEBUG_DEFAULT_INTERRUPT_HANDLERS
void __attribute__ ((weak)) NMI_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void NMI_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) HardFault_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void HardFault_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) MemManage_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void MemManage_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) BusFault_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void BusFault_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) UsageFault_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void UsageFault_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) SVC_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void SVC_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) DebugMon_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void DebugMon_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) PendSV_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void PendSV_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) SysTick_Handler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void SysTick_Handler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) WWDG_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void WWDG_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) PVD_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void PVD_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) TAMPER_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void TAMPER_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) RTC_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void RTC_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) FLASH_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void FLASH_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) RCC_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void RCC_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) EXTI0_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI0_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) EXTI1_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI1_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) EXTI2_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI2_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) EXTI3_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI3_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) EXTI4_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI4_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) DMA1_Channel1_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Channel1_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) DMA1_Channel2_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Channel2_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) DMA1_Channel3_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Channel3_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) DMA1_Channel4_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Channel4_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) DMA1_Channel5_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Channel5_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) DMA1_Channel6_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Channel6_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) DMA1_Channel7_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void DMA1_Channel7_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) ADC1_2_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void ADC1_2_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) USB_HP_CAN1_TX_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void USB_HP_CAN1_TX_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) USB_LP_CAN1_RX0_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void USB_LP_CAN1_RX0_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) CAN1_RX1_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void CAN1_RX1_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) CAN1_SCE_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void CAN1_SCE_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) EXTI9_5_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI9_5_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) TIM1_BRK_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void TIM1_BRK_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) TIM1_UP_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void TIM1_UP_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) TIM1_TRG_COM_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void TIM1_TRG_COM_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) TIM1_CC_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void TIM1_CC_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) TIM2_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void TIM2_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) TIM3_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void TIM3_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) TIM4_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void TIM4_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) I2C1_EV_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void I2C1_EV_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) I2C1_ER_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void I2C1_ER_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) I2C2_EV_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void I2C2_EV_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) I2C2_ER_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void I2C2_ER_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) SPI1_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void SPI1_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) SPI2_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void SPI2_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) USART1_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void USART1_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) USART2_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void USART2_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) USART3_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void USART3_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) EXTI15_10_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void EXTI15_10_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) RTC_Alarm_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void RTC_Alarm_IRQHandler();
	asm("bkpt 255");
};

void __attribute__ ((weak)) USBWakeUp_IRQHandler()
{
	//If you hit the breakpoint below, one of the interrupts was unhandled in your code.
	//Define the following function in your code to handle it:
	//	extern "C" void USBWakeUp_IRQHandler();
	asm("bkpt 255");
};

#else
void NMI_Handler()                 __attribute__ ((weak, alias ("Default_Handler")));
void HardFault_Handler()           __attribute__ ((weak, alias ("Default_Handler")));
void MemManage_Handler()           __attribute__ ((weak, alias ("Default_Handler")));
void BusFault_Handler()            __attribute__ ((weak, alias ("Default_Handler")));
void UsageFault_Handler()          __attribute__ ((weak, alias ("Default_Handler")));
void SVC_Handler()                 __attribute__ ((weak, alias ("Default_Handler")));
void DebugMon_Handler()            __attribute__ ((weak, alias ("Default_Handler")));
void PendSV_Handler()              __attribute__ ((weak, alias ("Default_Handler")));
void SysTick_Handler()             __attribute__ ((weak, alias ("Default_Handler")));
void WWDG_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void PVD_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void TAMPER_IRQHandler()           __attribute__ ((weak, alias ("Default_Handler")));
void RTC_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void FLASH_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void RCC_IRQHandler()              __attribute__ ((weak, alias ("Default_Handler")));
void EXTI0_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void EXTI1_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void EXTI2_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void EXTI3_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void EXTI4_IRQHandler()            __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Channel1_IRQHandler()    __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Channel2_IRQHandler()    __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Channel3_IRQHandler()    __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Channel4_IRQHandler()    __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Channel5_IRQHandler()    __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Channel6_IRQHandler()    __attribute__ ((weak, alias ("Default_Handler")));
void DMA1_Channel7_IRQHandler()    __attribute__ ((weak, alias ("Default_Handler")));
void ADC1_2_IRQHandler()           __attribute__ ((weak, alias ("Default_Handler")));
void USB_HP_CAN1_TX_IRQHandler()   __attribute__ ((weak, alias ("Default_Handler")));
void USB_LP_CAN1_RX0_IRQHandler()  __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_RX1_IRQHandler()         __attribute__ ((weak, alias ("Default_Handler")));
void CAN1_SCE_IRQHandler()         __attribute__ ((weak, alias ("Default_Handler")));
void EXTI9_5_IRQHandler()          __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_BRK_IRQHandler()         __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_UP_IRQHandler()          __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_TRG_COM_IRQHandler()     __attribute__ ((weak, alias ("Default_Handler")));
void TIM1_CC_IRQHandler()          __attribute__ ((weak, alias ("Default_Handler")));
void TIM2_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void TIM3_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void TIM4_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void I2C1_EV_IRQHandler()          __attribute__ ((weak, alias ("Default_Handler")));
void I2C1_ER_IRQHandler()          __attribute__ ((weak, alias ("Default_Handler")));
void I2C2_EV_IRQHandler()          __attribute__ ((weak, alias ("Default_Handler")));
void I2C2_ER_IRQHandler()          __attribute__ ((weak, alias ("Default_Handler")));
void SPI1_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void SPI2_IRQHandler()             __attribute__ ((weak, alias ("Default_Handler")));
void USART1_IRQHandler()           __attribute__ ((weak, alias ("Default_Handler")));
void USART2_IRQHandler()           __attribute__ ((weak, alias ("Default_Handler")));
void USART3_IRQHandler()           __attribute__ ((weak, alias ("Default_Handler")));
void EXTI15_10_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void RTC_Alarm_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
void USBWakeUp_IRQHandler()        __attribute__ ((weak, alias ("Default_Handler")));
#endif

void * g_pfnVectors[0x43] __attribute__ ((section (".isr_vector"))) =
        {
                &_estack,
                &Reset_Handler,
                &NMI_Handler,
                &HardFault_Handler,
                &MemManage_Handler,
                &BusFault_Handler,
                &UsageFault_Handler,
                NULL,
                NULL,
                NULL,
                NULL,
                &SVC_Handler,
                &DebugMon_Handler,
                NULL,
                &PendSV_Handler,
                &SysTick_Handler,
                &WWDG_IRQHandler,
                &PVD_IRQHandler,
                &TAMPER_IRQHandler,
                &RTC_IRQHandler,
                &FLASH_IRQHandler,
                &RCC_IRQHandler,
                &EXTI0_IRQHandler,
                &EXTI1_IRQHandler,
                &EXTI2_IRQHandler,
                &EXTI3_IRQHandler,
                &EXTI4_IRQHandler,
                &DMA1_Channel1_IRQHandler,
                &DMA1_Channel2_IRQHandler,
                &DMA1_Channel3_IRQHandler,
                &DMA1_Channel4_IRQHandler,
                &DMA1_Channel5_IRQHandler,
                &DMA1_Channel6_IRQHandler,
                &DMA1_Channel7_IRQHandler,
                &ADC1_2_IRQHandler,
                &USB_HP_CAN1_TX_IRQHandler,
                &USB_LP_CAN1_RX0_IRQHandler,
                &CAN1_RX1_IRQHandler,
                &CAN1_SCE_IRQHandler,
                &EXTI9_5_IRQHandler,
                &TIM1_BRK_IRQHandler,
                &TIM1_UP_IRQHandler,
                &TIM1_TRG_COM_IRQHandler,
                &TIM1_CC_IRQHandler,
                &TIM2_IRQHandler,
                &TIM3_IRQHandler,
                &TIM4_IRQHandler,
                &I2C1_EV_IRQHandler,
                &I2C1_ER_IRQHandler,
                &I2C2_EV_IRQHandler,
                &I2C2_ER_IRQHandler,
                &SPI1_IRQHandler,
                &SPI2_IRQHandler,
                &USART1_IRQHandler,
                &USART2_IRQHandler,
                &USART3_IRQHandler,
                &EXTI15_10_IRQHandler,
                &RTC_Alarm_IRQHandler,
                &USBWakeUp_IRQHandler,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                (void *)0xF108F85F /* BootRAM */,
        };

void SystemInit();
void __libc_init_array();
int main();

extern void *_sidata, *_sdata, *_edata;
extern void *_sbss, *_ebss;

void __attribute__((naked, noreturn)) Reset_Handler()
{
    //Normally the CPU should will setup the based on the value from the first entry in the vector table.
    //If you encounter problems with accessing stack variables during initialization, ensure
    //asm ("ldr sp, =_estack");

    void **pSource, **pDest;
    for (pSource = &_sidata, pDest = &_sdata; pDest != &_edata; pSource++, pDest++)
        *pDest = *pSource;

    for (pDest = &_sbss; pDest != &_ebss; pDest++)
        *pDest = 0;

    SystemInit();
    __libc_init_array();
    (void)main();
    for (;;) ;
}

void __attribute__((naked, noreturn)) Default_Handler()
{
    //If you get stuck here, your code is missing a handler for some interrupt.
    //Define a 'DEBUG_DEFAULT_INTERRUPT_HANDLERS' macro via VisualGDB Project Properties and rebuild your project.
    //This will pinpoint a specific missing vector.
    for (;;) ;
}
