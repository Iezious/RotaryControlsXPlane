//
// Created by sergey-ki on 6/26/2018.
//

#include <mxconstants.h>
#include <sched.h>
//#include <stdbool.h>
#include <usbd_customhid.h>
#include "joylogic.h"
#include <stm32f407xx.h>
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal.h"

#define BUTTONS 17
#define LINES   5
#define ROTS    3

GPIO_TypeDef* mx_button_gpio[BUTTONS] = { 
	BUTTON_1_GPIO, BUTTON_2_GPIO, BUTTON_3_GPIO, BUTTON_4_GPIO, BUTTON_5_GPIO,
	BUTTON_6_GPIO, BUTTON_7_GPIO, BUTTON_8_GPIO, BUTTON_9_GPIO, BUTTON_10_GPIO,
	BUTTON_11_GPIO, BUTTON_12_GPIO, BUTTON_13_GPIO,	BUTTON_14_GPIO,	BUTTON_15_GPIO,
	BUTTON_16_GPIO,	BUTTON_17_GPIO
};

uint16_t mx_button_pins[BUTTONS] = { 
	BUTTON_1_PIN, BUTTON_2_PIN, BUTTON_3_PIN, BUTTON_4_PIN, BUTTON_5_PIN,
	BUTTON_6_PIN, BUTTON_7_PIN, BUTTON_8_PIN, BUTTON_9_PIN, BUTTON_10_PIN,
	BUTTON_11_PIN, BUTTON_12_PIN, BUTTON_13_PIN,	BUTTON_14_PIN,	BUTTON_15_PIN,
	BUTTON_16_PIN,	BUTTON_17_PIN
};

uint8_t mx_buttons[2][BUTTONS] = {
	{ 
		MX_BUTTON_0_0, MX_BUTTON_0_1, MX_BUTTON_0_2, MX_BUTTON_0_3, MX_BUTTON_0_4, MX_BUTTON_0_5, MX_BUTTON_0_6, MX_BUTTON_0_7, MX_BUTTON_0_8, 
		MX_BUTTON_0_9, MX_BUTTON_0_10, MX_BUTTON_0_11, MX_BUTTON_0_12, MX_BUTTON_0_13, MX_BUTTON_0_14, MX_BUTTON_0_15, MX_BUTTON_0_16 
	},
	{ 
		MX_BUTTON_1_0, MX_BUTTON_1_1, MX_BUTTON_1_2, MX_BUTTON_1_3, MX_BUTTON_1_4, MX_BUTTON_1_5, MX_BUTTON_1_6, MX_BUTTON_1_7, MX_BUTTON_1_8,
		MX_BUTTON_1_9, MX_BUTTON_1_10, MX_BUTTON_1_11, MX_BUTTON_1_12, MX_BUTTON_1_13, MX_BUTTON_1_14, MX_BUTTON_1_15, MX_BUTTON_1_16 
	},
};

#define LED_ON	HAL_GPIO_WritePin(LED_GPIO, LED_PIN, GPIO_PIN_RESET)
#define LED_OFF	HAL_GPIO_WritePin(LED_GPIO, LED_PIN, GPIO_PIN_SET)


GPIO_TypeDef* rot_sw_gpio[ROTS] = { ROT1_GPIO_C, ROT2_GPIO_C, ROT3_GPIO_C };
uint16_t rot_sw_pins[ROTS] = { ROT1_PIN_C, ROT2_PIN_C, ROT3_PIN_C };
uint8_t rot_sw_buttons[ROTS] = { ROT1_SW, ROT2_SW, ROT3_SW };

GPIO_TypeDef* rot_gpio_a[ROTS * 2] = { ROT1_GPIO_A1, ROT1_GPIO_A2, ROT2_GPIO_A1, ROT2_GPIO_A2, ROT3_GPIO_A1, ROT3_GPIO_A2 };
uint16_t rot_pins_a[ROTS * 2] = { ROT1_PIN_A1, ROT1_PIN_A2, ROT2_PIN_A1, ROT2_PIN_A2, ROT3_PIN_A1, ROT3_PIN_A2 };

GPIO_TypeDef* rot_gpio_b[ROTS * 2] = { ROT1_GPIO_B1, ROT1_GPIO_B2, ROT2_GPIO_B1, ROT2_GPIO_B2, ROT3_GPIO_B1, ROT3_GPIO_B2 };
uint16_t rot_pins_b[ROTS * 2] = { ROT1_PIN_B1, ROT1_PIN_B2, ROT2_PIN_B1, ROT2_PIN_B2, ROT3_PIN_B1, ROT3_PIN_B2 };

uint8_t rot_bits_up[ROTS * 2] = { ROT1_L1_UP, ROT1_L2_UP, ROT2_L1_UP, ROT2_L2_UP, ROT3_L1_UP, ROT3_L2_UP };
uint8_t rot_bits_dn[ROTS * 2] = { ROT1_L1_DN, ROT1_L2_DN, ROT2_L1_DN, ROT2_L2_DN, ROT3_L1_DN, ROT3_L2_DN };



typedef union
{
	uint32_t bits[2];
	uint8_t buffer[8];
} TJoystickReport;

TJoystickReport JoystickReport;

inline static void SpinWait(uint32_t mks)
{
	volatile uint32_t w = mks * 36;
	while (--w) ;
}

static inline void CleanReport()
{
	JoystickReport.bits[0] = 0;
	JoystickReport.bits[1] = 0;
}

static inline void SetBit(uint8_t bit)
{
	uint32_t idx = bit < 32 ? 0 : 1;
	uint32_t offset = bit < 32 ? bit : bit - 32;
	uint32_t mask = 1 << offset;

	JoystickReport.bits[idx] |= mask;
}

static inline void ResetBit(uint16_t bit)
{
	uint32_t idx = bit < 32 ? 0 : 1;
	uint32_t offset = bit < 32 ? bit : bit - 32;
	uint32_t mask = ~(1 << offset);

	JoystickReport.bits[idx] &= mask;
}

void ScanButtons()
{
	uint8_t* buttonIndexes = HAL_GPIO_ReadPin(SELECTOR_GPIO, SELECTOR_PIN) == GPIO_PIN_SET
		? mx_buttons[0] : mx_buttons[1];

	for (uint8_t i = 0; i < BUTTONS; i++)
	{
		if (HAL_GPIO_ReadPin(mx_button_gpio[i], mx_button_pins[i]) == GPIO_PIN_SET)
			SetBit(buttonIndexes[i]);
	}
}

void ScanSwitches()
{
	for (uint8_t sw = 0; sw < ROTS; sw++)
	{
		if (HAL_GPIO_ReadPin(rot_sw_gpio[sw], rot_sw_pins[sw]) == GPIO_PIN_RESET)
			SetBit(rot_sw_buttons[sw]);
	}
}

void ScanRotaries()
{
	static int32_t rotaryDelays[6] = { 0, 0, 0, 0, 0, 0 };
	static uint8_t rotaryStates[6] = { 0, 0, 0, 0, 0, 0 };
	
	//	static int8_t rotary[3] = { 0, 0, 0 };

	uint32_t time = HAL_GetTick();

	for (uint8_t i = 0; i < ROTS * 2; i++)
	{
		uint8_t a_state = HAL_GPIO_ReadPin(rot_gpio_a[i], rot_pins_a[i]);
		uint8_t b_state = HAL_GPIO_ReadPin(rot_gpio_b[i], rot_pins_b[i]);

		if (rotaryDelays[i] && time - rotaryDelays[i] > 2)
		{
			if (a_state != rotaryStates[0])
			{
				if (a_state == b_state)
				{
					SetBit(rot_bits_up[i]);
				}
				else
				{
					SetBit(rot_bits_dn[i]);
				}
			}

			rotaryDelays[i] = 0;
			rotaryStates[i] = a_state;
		}

		if (rotaryDelays[i] == 0 && a_state != rotaryStates[i])
		{
			rotaryDelays[i] = time;
		}
	}
}

/*
static void StoreReport()
{
	prevReport[0] = JoystickReport.bits[0];
	prevReport[1] = JoystickReport.bits[1];
}

int IsReportChanged()
{
	return JoystickReport.bits[0] != prevReport[0] || JoystickReport.bits[1] != prevReport[1];
}
*/

void CleanRotators()
{

}

/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case ROT1_PIN_A1:
		if (HAL_GPIO_ReadPin(ROT1_GPIO_B1, ROT1_PIN_B1) == GPIO_PIN_SET)
			SetBit(ROT1_L1_UP);
		else
			SetBit(ROT1_L1_DN);
		break;

	case ROT1_PIN_A2:
		if (HAL_GPIO_ReadPin(ROT1_GPIO_B2, ROT1_PIN_B2) == GPIO_PIN_SET)
			SetBit(ROT1_L2_UP);
		else
			SetBit(ROT1_L2_DN);
		break;

	case ROT2_PIN_A1:
		if (HAL_GPIO_ReadPin(ROT2_GPIO_B1, ROT2_PIN_B1) == GPIO_PIN_SET)
			SetBit(ROT2_L1_UP);
		else
			SetBit(ROT2_L1_DN);
		break;

	case ROT2_PIN_A2:
		if (HAL_GPIO_ReadPin(ROT2_GPIO_B2, ROT2_PIN_B2) == GPIO_PIN_SET)
			SetBit(ROT2_L2_UP);
		else
			SetBit(ROT2_L2_DN);
		break;

	case ROT3_PIN_A1:
		if (HAL_GPIO_ReadPin(ROT3_GPIO_B1, ROT3_PIN_B1) == GPIO_PIN_SET)
			SetBit(ROT3_L1_UP);
		else
			SetBit(ROT3_L1_DN);
		break;

	case ROT3_PIN_A2:
		if (HAL_GPIO_ReadPin(ROT3_GPIO_B2, ROT3_PIN_B2) == GPIO_PIN_SET)
			SetBit(ROT3_L2_UP);
		else
			SetBit(ROT3_L2_DN);
		break;
	}
}
*/

static void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */

	__HAL_RCC_GPIOH_CLK_ENABLE();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	
	GPIO_InitStruct.Pin = SELECTOR_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(SELECTOR_GPIO, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LED_GPIO, LED_PIN, GPIO_PIN_RESET);

	for (uint8_t i = 0; i < BUTTONS; i++)
	{
		GPIO_InitStruct.Pin = mx_button_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		HAL_GPIO_Init(mx_button_gpio[i], &GPIO_InitStruct);
	}

	for (uint8_t i = 0; i < ROTS; i++)
	{
		GPIO_InitStruct.Pin = rot_sw_pins[i];
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(rot_sw_gpio[i], &GPIO_InitStruct);

		HAL_GPIO_WritePin(rot_sw_gpio[i], rot_sw_pins[i], GPIO_PIN_SET);
	}

	for (uint8_t i = 0; i < ROTS * 2; i++)
	{
		GPIO_InitStruct.Pin = rot_pins_a[i];
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(rot_gpio_a[i], &GPIO_InitStruct);

		GPIO_InitStruct.Pin = rot_pins_b[i];
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(rot_gpio_b[i], &GPIO_InitStruct);

		HAL_GPIO_WritePin(rot_gpio_a[i], rot_pins_a[i], GPIO_PIN_SET);
		HAL_GPIO_WritePin(rot_gpio_b[i], rot_pins_b[i], GPIO_PIN_SET);
	}


	/*Set rotator dir to ON */
	//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_3 | GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_SET);
	//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10 | GPIO_PIN_8 | GPIO_PIN_15, GPIO_PIN_SET);

		/* EXTI interrupt init*/
		/*
		HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI4_IRQn);

		HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

		HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
		*/
}


extern USBD_HandleTypeDef USBD_Device;

void JoystickCycle()
{
	uint32_t lastUsbSent = 0;
	uint32_t lastLed = 0;
	uint8_t ledon = 0;

	TJoystickReport sendBuffer;

	for (;;)	
	{
		SpinWait(50);
		ScanRotaries();
		ScanButtons();
		ScanSwitches();

		if (HAL_GetTick() - lastUsbSent > 10)
		{
			sendBuffer.bits[0] = JoystickReport.bits[0];
			sendBuffer.bits[1] = JoystickReport.bits[1];

			if (USBD_CUSTOM_HID_SendReport(&USBD_Device, sendBuffer.buffer, 8) == USBD_OK)
			{
				CleanRotators();
				CleanReport();

				lastUsbSent = HAL_GetTick();
			}
		}

		if (HAL_GetTick() - lastLed > 1000)
		{
			if (ledon)
				LED_ON;
			else
				LED_OFF;

			ledon = !ledon;
			lastLed = HAL_GetTick();
		}

		SpinWait(168);
	}
}


void JoystickInit()
{
	MX_GPIO_Init();
}