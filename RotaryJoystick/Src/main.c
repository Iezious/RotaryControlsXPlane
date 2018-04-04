/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usb_device.h"
#include "usbd_hid.h"
#include "mxconstants.h"

#define ROWS    5
#define LINES   5
#define ROTS    3

GPIO_TypeDef* mx_line_gpio[LINES] = {MX_LINE_0_GPIO, MX_LINE_1_GPIO, MX_LINE_2_GPIO, MX_LINE_3_GPIO, MX_LINE_4_GPIO};
uint16_t mx_line_pins[LINES] = {MX_LINE_0_PIN, MX_LINE_1_PIN, MX_LINE_2_PIN, MX_LINE_3_PIN, MX_LINE_4_PIN};

GPIO_TypeDef* mx_row_gpio[ROWS] = {MX_ROW_0_GPIO, MX_ROW_1_GPIO, MX_ROW_2_GPIO, MX_ROW_3_GPIO, MX_ROW_4_GPIO};
uint16_t mx_row_pins[ROWS] = {MX_LINE_0_PIN, MX_LINE_1_PIN, MX_LINE_2_PIN, MX_LINE_3_PIN, MX_LINE_4_PIN};

GPIO_TypeDef* rot_sw_gpio[ROTS] = {ROT1_GPIO_C, ROT2_GPIO_C, ROT3_GPIO_C};
uint16_t rot_sw_pins[ROTS] = {ROT1_PIN_C, ROT2_PIN_C, ROT3_PIN_C};
uint8_t rot_sw_buttons[ROTS] = {ROT1_SW, ROT2_SW, ROT3_SW};

uint8_t mx_buttons[LINES*ROWS] = {
    MX_BUTTON_0_0, MX_BUTTON_0_1, MX_BUTTON_0_2, MX_BUTTON_0_3, MX_BUTTON_0_4,
    MX_BUTTON_1_0, MX_BUTTON_1_1, MX_BUTTON_1_2, MX_BUTTON_1_3, MX_BUTTON_1_4,
    MX_BUTTON_2_0, MX_BUTTON_2_1, MX_BUTTON_2_2, MX_BUTTON_2_3, MX_BUTTON_2_4,
    MX_BUTTON_3_0, MX_BUTTON_3_1, MX_BUTTON_3_2, MX_BUTTON_3_3, MX_BUTTON_3_4,
    MX_BUTTON_4_0, MX_BUTTON_4_1, MX_BUTTON_4_2, MX_BUTTON_4_3, MX_BUTTON_4_4,
};

void SystemClock_Config(void);
void Error_Handler(void);
void MX_GPIO_Init(void);
void MX_NVIC_Init(void);

union
{
    uint32_t bits[2];
	uint8_t buffer[8];
} JoystickReport;

uint32_t prevReport[2] = {0, 0};


inline static void SpinWait(uint32_t mks)
{
    volatile uint32_t w = mks * 36;
    while(--w);
}

static inline void CleanReport()
{
    JoystickReport.bits[0] = 0;
    JoystickReport.bits[1] = 0;
}

static inline void SetBit(uint8_t bit)
{
    uint16_t idx = bit < 32 ? 0 : 1;
    uint16_t offset = bit < 32 ? bit : bit - 32;
    uint32_t mask = 1 << offset;

    JoystickReport.bits[idx] |= mask;
}

static inline void ResetBit(uint16_t bit)
{
    uint16_t idx = bit < 32 ? 0 : 1;
    uint16_t offset = bit < 32 ? bit : bit - 32;
    uint32_t mask = ~(1 << offset);

    JoystickReport.bits[idx] &= mask;
}

static void ScanButtons()
{
    for(uint8_t line=0; line < LINES; line++)
    {
        HAL_GPIO_WritePin(mx_line_gpio[line], mx_line_pins[line], GPIO_PIN_SET);
        SpinWait(5);

        for(uint8_t row=0; row < ROWS; row++)
        {
            if(HAL_GPIO_ReadPin(mx_row_gpio[row], mx_row_pins[row]) == GPIO_PIN_SET)
                SetBit(mx_buttons[line*ROWS + row]);
        }

        HAL_GPIO_WritePin(mx_line_gpio[line], mx_line_pins[line], GPIO_PIN_RESET);
        SpinWait(5);
    }
}

static void ScanSwitches()
{
    for(uint8_t sw=0; sw < ROTS; sw++)
    {
        if(HAL_GPIO_ReadPin(rot_sw_gpio[sw], rot_sw_pins[sw]) == GPIO_PIN_RESET)
            SetBit(rot_sw_buttons[sw]);
    }
}

static bool IsReportChanged()
{
    return JoystickReport.bits[0] != prevReport[0] || JoystickReport.bits[1] != prevReport[1];
}

static void StoreReport()
{
    prevReport[0] = JoystickReport.bits[0];
    prevReport[1] = JoystickReport.bits[1];
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USB_DEVICE_Init();
    MX_NVIC_Init();

    HAL_Delay(100);

    while (1)
    {
        uint32_t start = HAL_GetTick();

        while(HAL_GetTick() - start < 10)
        {
          SpinWait(50);
          ScanButtons();
          ScanSwitches();
        }

        if(IsReportChanged())
            USBD_HID_SendReport(&hUsbDeviceFS, JoystickReport.buffer, 5);

        StoreReport();
        CleanReport();
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch(GPIO_Pin)
    {
        case ROT1_PIN_A1:
            if(HAL_GPIO_ReadPin(ROT1_GPIO_B1, ROT1_PIN_B1) == GPIO_PIN_SET)
                SetBit(ROT1_L1_UP);
            else
                SetBit(ROT1_L1_DN);
            break;

        case ROT1_PIN_A2:
            if(HAL_GPIO_ReadPin(ROT1_GPIO_B2, ROT1_PIN_B2) == GPIO_PIN_SET)
                SetBit(ROT1_L2_UP);
            else
                SetBit(ROT1_L2_DN);

        case ROT2_PIN_A1:
            if(HAL_GPIO_ReadPin(ROT2_GPIO_B1, ROT2_PIN_B1) == GPIO_PIN_SET)
                SetBit(ROT2_L1_UP);
            else
                SetBit(ROT2_L1_DN);
            break;

        case ROT2_PIN_A2:
            if(HAL_GPIO_ReadPin(ROT2_GPIO_B2, ROT2_PIN_B2) == GPIO_PIN_SET)
                SetBit(ROT2_L2_UP);
            else
                SetBit(ROT2_L2_DN);

        case ROT3_PIN_A1:
            if(HAL_GPIO_ReadPin(ROT3_GPIO_B1, ROT3_PIN_B1) == GPIO_PIN_SET)
                SetBit(ROT3_L1_UP);
            else
                SetBit(ROT3_L1_DN);
            break;

        case ROT3_PIN_A2:
            if(HAL_GPIO_ReadPin(ROT3_GPIO_B2, ROT3_PIN_B2) == GPIO_PIN_SET)
                SetBit(ROT3_L2_UP);
            else
                SetBit(ROT3_L2_DN);
            break;
    }
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1)  {  }
  /* USER CODE END Error_Handler */
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */

/**
  * @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
