#include "BMSA/BMSA.hpp"
#include "BMSLStateMachine/BMSLStateMachine.hpp"
#include "BMSLLeds/BMSLLeds.hpp"

class BMSL {
private:
    BMSA bms;
    HalfBridge dclv;
    BMSLStateMachine state_machine;

public:
    BMSLLeds leds;

    BMSL(
        SPI::Peripheral& bms_spi_pin,
        array<reference_wrapper<Pin>, 5> dclv_pins,
        array<reference_wrapper<Pin>, 7> led_pins
        );
};

BMSL::BMSL(SPI::Peripheral& bms_spi_pin, array<reference_wrapper<Pin>, 5> dclv_pins, array<reference_wrapper<Pin>, 7> led_pins) {
    bms = BMSA(bms_spi_pin);
    dclv = HalfBridge(dclv_pins[0], dclv_pins[1], dclv_pins[2], dclv_pins[3], dclv_pins[4]);
    leds = BMSLLeds(led_pins[0], led_pins[1], led_pins[2], led_pins[3], led_pins[4], led_pins[5], led_pins[6]);
    state_machine = BMSLStateMachine();
}
