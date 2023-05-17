#include "main.h"
#include "lwip.h"

#include "BMSL/BMSL.hpp"
#include "Runes/Runes.hpp"



int main(void) {
	float avionics_current = 0;
	HeapPacket avionics_current_packet = HeapPacket(300, &avionics_current);
	
	array<reference_wrapper<Pin>, 5> dclv_pins = {DO_INV_PWM_H1, DO_INV_PWM_L1, DO_INV_PWM_H2, DO_INV_PWM_L2, GATE_DRIVERS_EN};
	array<reference_wrapper<Pin>, 7> led_pins = {LED_LOW_CHARGE, LED_FULL_CHARGE, LED_SLEEP, LED_FLASH, LED_CAN, LED_FAULT, LED_OPERATIONAL};
	BMSL bmsl(
		SPI::spi3,
		dclv_pins,
		led_pins,
		AVIONICSCURRENTSENSORFW
	);

	STLIB::start();



	while(1) {
		bmsl.leds.can.toggle();
		
		HAL_Delay(1000);
		STLIB::update();
	}
}

void Error_Handler(void)
{
	ErrorHandler("HAL error handler triggered");
}
