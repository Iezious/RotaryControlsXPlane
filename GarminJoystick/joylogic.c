//
// Created by sergey-ki on 6/26/2018.
//

#include <mxconstants.h>
#include <sched.h>
#include <stm32f103xb.h>
#include "stm32f1xx_hal.h"
//#include <stdbool.h>
#include <usbd_customhid.h>
#include "joylogic.h"

#define ROWS    5
#define LINES   5
#define ROTS    3

GPIO_TypeDef* mx_line_gpio[LINES] = { MX_LINE_0_GPIO, MX_LINE_1_GPIO, MX_LINE_2_GPIO, MX_LINE_3_GPIO, MX_LINE_4_GPIO };
uint16_t mx_line_pins[LINES] = { MX_LINE_0_PIN, MX_LINE_1_PIN, MX_LINE_2_PIN, MX_LINE_3_PIN, MX_LINE_4_PIN };

GPIO_TypeDef* mx_row_gpio[ROWS] = { MX_ROW_0_GPIO, MX_ROW_1_GPIO, MX_ROW_2_GPIO, MX_ROW_3_GPIO, MX_ROW_4_GPIO };
uint16_t mx_row_pins[ROWS] = { MX_ROW_0_PIN, MX_ROW_1_PIN, MX_ROW_2_PIN, MX_ROW_3_PIN, MX_ROW_4_PIN };

GPIO_TypeDef* rot_sw_gpio[ROTS] = { ROT1_GPIO_C, ROT2_GPIO_C, ROT3_GPIO_C };
uint16_t rot_sw_pins[ROTS] = { ROT1_PIN_C, ROT2_PIN_C, ROT3_PIN_C };
uint8_t rot_sw_buttons[ROTS] = { ROT1_SW, ROT2_SW, ROT3_SW };

GPIO_TypeDef* rot_gpio_a[ROTS*2] = { ROT1_GPIO_A1, ROT1_GPIO_A2, ROT2_GPIO_A1, ROT2_GPIO_A2, ROT3_GPIO_A1, ROT3_GPIO_A2};
uint16_t rot_pins_a[ROTS * 2] = { ROT1_PIN_A1, ROT1_PIN_A2, ROT2_PIN_A1, ROT2_PIN_A2, ROT3_PIN_A1, ROT3_PIN_A2 };

GPIO_TypeDef* rot_gpio_b[ROTS*2] = { ROT1_GPIO_B1, ROT1_GPIO_B2, ROT2_GPIO_B1, ROT2_GPIO_B2, ROT3_GPIO_B1, ROT3_GPIO_B2};
uint16_t rot_pins_b[ROTS * 2] = { ROT1_PIN_B1, ROT1_PIN_B2, ROT2_PIN_B1, ROT2_PIN_B2, ROT3_PIN_B1, ROT3_PIN_B2 };

uint8_t rot_bits_up[ROTS*2] = { ROT1_L1_UP, ROT1_L2_UP, ROT2_L1_UP, ROT2_L2_UP, ROT3_L1_UP, ROT3_L2_UP};
uint8_t rot_bits_dn[ROTS*2] = { ROT1_L1_DN, ROT1_L2_DN, ROT2_L1_DN, ROT2_L2_DN, ROT3_L1_DN, ROT3_L2_DN};


uint8_t mx_buttons[LINES*ROWS] = {
	MX_BUTTON_0_0,
	MX_BUTTON_0_1,
	MX_BUTTON_0_2,
	MX_BUTTON_0_3,
	MX_BUTTON_0_4,
	MX_BUTTON_1_0,
	MX_BUTTON_1_1,
	MX_BUTTON_1_2,
	MX_BUTTON_1_3,
	MX_BUTTON_1_4,
	MX_BUTTON_2_0,
	MX_BUTTON_2_1,
	MX_BUTTON_2_2,
	MX_BUTTON_2_3,
	MX_BUTTON_2_4,
	MX_BUTTON_3_0,
	MX_BUTTON_3_1,
	MX_BUTTON_3_2,
	MX_BUTTON_3_3,
	MX_BUTTON_3_4,
	MX_BUTTON_4_0,
	MX_BUTTON_4_1,
	MX_BUTTON_4_2,
	MX_BUTTON_4_3,
	MX_BUTTON_4_4,
};

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
	for (uint8_t line = 0; line < LINES; line++)
	{
		HAL_GPIO_WritePin(mx_line_gpio[line], mx_line_pins[line], GPIO_PIN_SET);
		SpinWait(5);

		for (uint8_t row = 0; row < ROWS; row++)
		{
			if (HAL_GPIO_ReadPin(mx_row_gpio[row], mx_row_pins[row]) == GPIO_PIN_SET)
				SetBit(mx_buttons[line*ROWS + row]);
		}

		HAL_GPIO_WritePin(mx_line_gpio[line], mx_line_pins[line], GPIO_PIN_RESET);
		SpinWait(5);
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

	for (uint8_t i = 0; i < ROTS*2; i++)
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
//	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pins : PC13 PC14 PC15 */
	GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PA0 PA1 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure BUTTON pins : PA2 PA3 PA4 PA5 PA6 PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure BUTTON pins : PB0 PB1 PB2 PB10 PB11 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure ROT SW pins : PB5 PB12 */
	GPIO_InitStruct.Pin =  GPIO_PIN_5 | GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure ROT SW pins : PA9 */
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure ROT DIR/SW pins : PB3 PB6 PB8 PB13 PB15  */
	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_8 | GPIO_PIN_13 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure ROT DIR/SW pins : PA10 */
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure ROT INT pins : PB4 PB7 PB9 PB14*/
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure ROT INT pins : PA8 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Set rotator switch to ON */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5 | GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);

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
	TJoystickReport sendBuffer;

	for(;;)
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

		SpinWait(168);
	}
}


void JoystickInit()
{
	MX_GPIO_Init();
}