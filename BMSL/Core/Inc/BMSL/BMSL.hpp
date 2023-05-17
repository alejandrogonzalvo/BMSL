#include "BMSA/BMSA.hpp"
#include "BMSLStateMachine/BMSLStateMachine.hpp"
#include "BMSLLeds/BMSLLeds.hpp"

class BMSL {
private:
    BMSA bms;
    HalfBridge dclv;
    BMSLStateMachine state_machine;
    uint8_t avionics_current_sensor;

public:
    BMSLLeds leds;

    BMSL(
        SPI::Peripheral& bms_spi_pin,
        array<reference_wrapper<Pin>, 5> dclv_pins,
        array<reference_wrapper<Pin>, 7> led_pins,
        Pin& avionics_current_sensor
        );

    float get_avionics_current();
};

BMSL::BMSL(SPI::Peripheral& bms_spi_pin, array<reference_wrapper<Pin>, 5> dclv_pins, array<reference_wrapper<Pin>, 7> led_pins, Pin& avionics_current_sensor) {
    bms = BMSA(bms_spi_pin);
    dclv = HalfBridge(dclv_pins[0], dclv_pins[1], dclv_pins[2], dclv_pins[3], dclv_pins[4]);
    leds = BMSLLeds(led_pins[0], led_pins[1], led_pins[2], led_pins[3], led_pins[4], led_pins[5], led_pins[6]);
    state_machine = BMSLStateMachine();
    this->avionics_current_sensor = ADC::inscribe(avionics_current_sensor).value();
}

float BMSL::get_avionics_current() {
    return ADC::get_value(avionics_current_sensor);
}