#include "main.h"
#include "lwip.h"

#include "BMSL/BMSL.hpp"
#include "Runes/Runes.hpp"

int main(void) {

	array<Pin, 5> dclv_pins = {DO_INV_PWM_H1, DO_INV_PWM_L1, DO_INV_PWM_H2, DO_INV_PWM_L2, GATE_DRIVERS_EN};
	array<Pin, 7> led_pins = {LED_LOW_CHARGE, LED_FULL_CHARGE, LED_SLEEP, LED_FLASH, LED_CAN, LED_FAULT, LED_OPERATIONAL};
	BMSL bmsl(
		SPI::spi3,
		dclv_pins,
		led_pins
	);

	STLIB::start();

	while(1) {
		STLIB::update();
	}
}

void Error_Handler(void)
{
	ErrorHandler("HAL error handler triggered");
}
