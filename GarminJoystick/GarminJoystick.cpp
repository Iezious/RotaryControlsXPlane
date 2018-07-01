#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_flash.h"
#include "usbd_customhid.h"
#include "stm32f1xx_hal_pcd.h"
#include "usbd_custom_hid_if.h"
#include "joylogic.h"
#ifdef __cplusplus
extern "C"
{
#endif
	#include <usbd_core.h>
	#include <usbd_desc.h>

	USBD_HandleTypeDef USBD_Device;
	void SysTick_Handler(void);
	void OTG_FS_IRQHandler(void);
	void OTG_HS_IRQHandler(void);
	void USB_LP_CAN1_RX0_IRQHandler(void);

	extern PCD_HandleTypeDef hpcd;
	
#ifdef __cplusplus
}
#endif

#if defined(STM32F102x6) || defined(STM32F102xB) || defined(STM32F103x6) || defined(STM32F103xB) || defined(STM32F103xG) || defined(STM32F103xE)

void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef clkinitstruct = { 0 };
	RCC_OscInitTypeDef oscinitstruct = { 0 };
	RCC_PeriphCLKInitTypeDef rccperiphclkinit = { 0 };
  
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState        = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
    
	oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  
	if (HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
	{
		asm("bkpt 255");
	}
  
	/* USB clock selection */
	rccperiphclkinit.PeriphClockSelection = RCC_PERIPHCLK_USB;
	rccperiphclkinit.UsbClockSelection = RCC_USBPLLCLK_DIV1_5;
	HAL_RCCEx_PeriphCLKConfig(&rccperiphclkinit);
  
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	clocks dividers */
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2) != HAL_OK)
	{
		asm("bkpt 255");
	}
}

#else

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  RCC_PeriphCLKInitTypeDef rccperiphclkinit = {0};
  
  /* Configure PLLs ------------------------------------------------------*/
  /* PLL2 configuration: PLL2CLK = (HSE / HSEPrediv2Value) * PLL2MUL = (25 / 5) * 8 = 40 MHz */
  /* PREDIV1 configuration: PREDIV1CLK = PLL2CLK / HSEPredivValue = 40 / 5 = 8 MHz */
  /* PLL configuration: PLLCLK = PREDIV1CLK * PLLMUL = 8 * 9 = 72 MHz */
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV5;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  oscinitstruct.Prediv1Source   = RCC_PREDIV1_SOURCE_PLL2;
  
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL2.PLL2State  = RCC_PLL2_ON;
  oscinitstruct.PLL2.HSEPrediv2Value = RCC_HSE_PREDIV2_DIV5;
  oscinitstruct.PLL2.PLL2MUL    = RCC_PLL2_MUL8;
  HAL_RCC_OscConfig(&oscinitstruct);
  
  /* USB clock selection */
  rccperiphclkinit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  rccperiphclkinit.UsbClockSelection = RCC_USBPLLCLK_DIV3;
  HAL_RCCEx_PeriphCLKConfig(&rccperiphclkinit);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
  clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2);
}
#endif

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd);
}
#elif defined(USE_USB_HS)
void OTG_HS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd);
}
#else
#error USB peripheral type not defined
#endif

/*
	This is a basic USB CDC device sample based on the ST USB library.
	To test it out:
		1. Install the ST VCP drivers (http://www.st.com/web/en/catalog/tools/PF257938)
		2. Connect to the virtual COM port using SmarTTY or any other terminal program
		3. Type some characters and observe the output
	Read more about the sample: http://visualgdb.com/tutorials/arm/stm32/usb/
*/

/**
* @brief This function handles USB low priority or CAN RX0 interrupts.
*/

     
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	/* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 0 */

	/* USER CODE END USB_LP_CAN1_RX0_IRQn 0 */
	HAL_PCD_IRQHandler(&hpcd);
	/* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 1 */

	/* USER CODE END USB_LP_CAN1_RX0_IRQn 1 */
}

int main(void)
{
	HAL_Init();



	SystemClock_Config();
	USBD_Init(&USBD_Device, &VCP_Desc, 0);

	USBD_RegisterClass(&USBD_Device, &USBD_CUSTOM_HID);
	USBD_CUSTOM_HID_RegisterInterface(&USBD_Device, &USBD_CustomHID_fops_FS);
	USBD_Start(&USBD_Device);

	for (;;)
	{
		JoystickCycle();
	}
}