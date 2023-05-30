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
	
	BMSL::inscribe();

	uint8_t fdcan = FDCAN::inscribe(FDCAN::fdcan1);
	STLIB::start("192.168.1.8");


	ADC::turn_on(0);

	array<uint8_t, 6> data = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
	while(1) {
		BMSL::Leds::can.toggle();
		HAL_Delay(100);
		avionics_current = BMSL::get_avionics_current();
		BMSL::Communications::udp_socket.send(avionics_current_packet);
		FDCAN::transmit(fdcan, 0x123, data, FDCAN::DLC::BYTES_6);
		STLIB::update();
		BMSL::update();
	}
}

void Error_Handler(void)
{
	ErrorHandler("HAL error handler triggered");
}
