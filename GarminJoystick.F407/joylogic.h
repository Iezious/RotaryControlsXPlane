#pragma once
//
// Created by sergey-ki on 6/26/2018.
//

#ifndef GARMINJOYSTIK_JOYLOGIC_H
#define GARMINJOYSTIK_JOYLOGIC_H


#ifdef __cplusplus
extern "C" {
#endif
	
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
	void JoystickCycle();
	void JoystickInit();
	

#ifdef __cplusplus
}
#endif

#endif //GARMINJOYSTIK_JOYLOGIC_H
