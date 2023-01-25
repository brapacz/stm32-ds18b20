#include "ds18b20.h"



// function to print a device address
void DS18B20_printAddress(CurrentDeviceAddress deviceAddress) {
	for (uint8_t i = 0; i < 8; i++) {
		printf("0x%02X ", deviceAddress[i]);
	}
}

float DS18B20_ReadTemperature(DS18B20_HandleTypeDef* handle) {

	// call sensors.requestTemperatures() to issue a global temperature
	// request to all devices on the bus
	printf("[%8lu] 1-line: Requesting temperatures...", HAL_GetTick());
	DT_RequestTemperatures(&handle->hdt); // Send the command to get temperatures
	printf("\r\n[%8lu] 1-line: DONE\r\n", HAL_GetTick());
	float temp;
	for (int i = 0; i < handle->numDevOneLine; ++i) {
		// After we got the temperatures, we can print them here.
		// We use the function ByIndex, and as an example get the temperature from the first sensor only.
		temp = DT_GetTempCByIndex(&handle->hdt, i);
		printf(
				"[%8lu] 1-line: Temperature for the device %d (index %d) is: %i.%iC \r\n",
				HAL_GetTick(), i, i, (int) temp,
				(int) (temp * 10) % 10);
	}

	HAL_Delay(DT_MillisToWaitForConversion(DT_GetAllResolution(&handle->hdt)));
	return temp;
}

void DS18B20_Init(DS18B20_HandleTypeDef* handle, DS18B20_InitTypeDef* config) {
	handle->Init = config;
	printf("[%8lu] Debug UART1 is OK!\r\n", HAL_GetTick());
}

void DS18B20_Start(DS18B20_HandleTypeDef* handle) {
	OW_Begin(&handle->how, handle->Init->huart_debug);
	if (OW_Reset(&handle->how) == OW_OK) {
		printf("[%8lu] OneWire 1 line devices are present :)\r\n",
				HAL_GetTick());
	} else {
		printf("[%8lu] OneWire 1 line no devices :(\r\n", HAL_GetTick());
	}

	DT_SetOneWire(&handle->hdt, &handle->how);
	// arrays to hold device address
	CurrentDeviceAddress thermometer;

	// locate devices on the bus
	printf("[%8lu] 1-line Locating devices...\r\n", HAL_GetTick());
	DT_Begin(&handle->hdt);
	handle->numDevOneLine = DT_GetDeviceCount(&handle->hdt);
	printf("[%8lu] 1-line Found %d devices.\r\n", HAL_GetTick(), handle->numDevOneLine);

	for (int i = 0; i < handle->numDevOneLine; ++i) {
		if (!DT_GetAddress(&handle->hdt, thermometer, i))
			printf("[%8lu] 1-line: Unable to find address for Device %d\r\n",
					HAL_GetTick(), i);
		printf("[%8lu] 1-line: Device %d Address: ", HAL_GetTick(), i);
		DS18B20_printAddress(thermometer);
		printf("\r\n");
		// set the resolution to 12 bit (Each Dallas/Maxim device is capable of several different resolutions)
		DT_SetResolution(&handle->hdt, thermometer, 12, true);
		printf("[%8lu] 1-line: Device %d Resolution: %d\r\n", HAL_GetTick(), i,
				DT_GetResolution(&handle->hdt, thermometer));
	}
}
