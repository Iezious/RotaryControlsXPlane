#include "stm32F1xx_it.h"



extern PCD_HandleTypeDef hpcd;


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	/* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 0 */

	/* USER CODE END USB_LP_CAN1_RX0_IRQn 0 */
	HAL_PCD_IRQHandler(&hpcd);
	/* USER CODE BEGIN USB_LP_CAN1_RX0_IRQn 1 */

	/* USER CODE END USB_LP_CAN1_RX0_IRQn 1 */
}


/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
	/* USER CODE BEGIN SVCall_IRQn 0 */

	/* USER CODE END SVCall_IRQn 0 */
	/* USER CODE BEGIN SVCall_IRQn 1 */

	/* USER CODE END SVCall_IRQn 1 */
}

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


void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)
{
	/* USER CODE BEGIN EXTI9_5_IRQn 0 */

	/* USER CODE END EXTI9_5_IRQn 0 */
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
	/* USER CODE BEGIN EXTI9_5_IRQn 1 */

	/* USER CODE END EXTI9_5_IRQn 1 */
}

/**
* @brief This function handles EXTI line[15:10] interrupts.
*/
void EXTI15_10_IRQHandler(void)
{
	/* USER CODE BEGIN EXTI15_10_IRQn 0 */

	/* USER CODE END EXTI15_10_IRQn 0 */
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
	/* USER CODE BEGIN EXTI15_10_IRQn 1 */

	/* USER CODE END EXTI15_10_IRQn 1 */
}