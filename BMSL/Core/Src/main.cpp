#include "main.h"
#include "lwip.h"

#include "BMSL/BMSL.hpp"
#include "Runes/Runes.hpp"

int main(void) {

	HeapOrder start_balancing_order = {
		800,
		BMSL::Orders::start_balancing,
		&BMSL::bms
	};

	HeapOrder stop_balancing_order = {
		801,
		BMSL::Orders::stop_balancing,
		&BMSL::bms
	};

	float avionics_current = 0;
	HeapPacket avionics_current_packet = HeapPacket(300, &avionics_current);
	
	array<reference_wrapper<Pin>, 5> dclv_pins = {DO_INV_PWM_H1, DO_INV_PWM_L1, DO_INV_PWM_H2, DO_INV_PWM_L2, GATE_DRIVERS_EN};
	array<reference_wrapper<Pin>, 7> led_pins = {LED_LOW_CHARGE, LED_FULL_CHARGE, LED_SLEEP, LED_FLASH, LED_CAN, LED_FAULT, LED_OPERATIONAL};
	BMSL::inscribe(
		SPI::spi3,
		dclv_pins,
		led_pins,
		AVIONICSCURRENTSENSORFW
	);

	// HeapOrder start_balancing_order = {
	// 	800,
	// 	start_balancing,
	// 	&bmsl
	// };

	uint8_t fdcan = FDCAN::inscribe(FDCAN::fdcan1).value();
	STLIB::start("192.168.1.8");

	DatagramSocket udp_socket = DatagramSocket( IPV4("192.168.1.8"), 50400, IPV4("192.168.0.9"), 50400);

	ADC::turn_on(0);

	array<uint8_t, 6> data = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
	while(1) {
		BMSL::leds.can.toggle();
		HAL_Delay(100);
		avionics_current = BMSL::get_avionics_current();
		udp_socket.send(avionics_current_packet);
		FDCAN::transmit(fdcan, 0x123, data, FDCAN::DLC::BYTES_6);
		STLIB::update();
	}
}

void Error_Handler(void)
{
	ErrorHandler("HAL error handler triggered");
}
