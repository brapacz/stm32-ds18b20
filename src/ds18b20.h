/*
 * status_led.h
 *
 *  Created on: 19.07.2022
 *      Author: bartek
 */

#include "main.h"
#include "OneWire.h"
#include "DallasTemperature.h"

#ifndef DS18B20_H_
#define DS18B20_H_

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	UART_HandleTypeDef * huart_debug;
} DS18B20_InitTypeDef;

typedef struct {
	DS18B20_InitTypeDef *Init;
	uint8_t numDevOneLine;
	OneWire_HandleTypeDef how;
	DallasTemperature_HandleTypeDef hdt;
} DS18B20_HandleTypeDef;


void DS18B20_Init(DS18B20_HandleTypeDef* handle, DS18B20_InitTypeDef* config);
void DS18B20_Start(DS18B20_HandleTypeDef* handle);
float DS18B20_ReadTemperature(DS18B20_HandleTypeDef* handle);


#ifdef __cplusplus
}
#endif

#endif /* DS18B20_H_ */
