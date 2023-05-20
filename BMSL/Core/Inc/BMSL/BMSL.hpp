#include "BMSA/BMSA.hpp"
#include "BMSLStateMachine/BMSLStateMachine.hpp"
#include "BMSLLeds/BMSLLeds.hpp"

namespace BMSL {
    #define PinArray(size) array<reference_wrapper<Pin>, size>

    BMSA bms;
    HalfBridge dclv;
    BMSLStateMachine state_machine;
    uint8_t avionics_current_sensor;
    BMSLLeds leds;

    namespace Orders {
        void start_balancing();
        void stop_balancing();
    }
    
    void inscribe(SPI::Peripheral& bms_spi_pin, PinArray(5) dclv_pins, PinArray(7) led_pins, Pin& avionics_current_sensor);
    float get_avionics_current();

    void inscribe(SPI::Peripheral& bms_spi_pin, PinArray(5) dclv_pins, PinArray(7) led_pins, Pin& avionics_current_sensor_pin) {
        bms = BMSA(bms_spi_pin);
        dclv = HalfBridge(dclv_pins[0], dclv_pins[1], dclv_pins[2], dclv_pins[3], dclv_pins[4]);
        leds = BMSLLeds(led_pins[0], led_pins[1], led_pins[2], led_pins[3], led_pins[4], led_pins[5], led_pins[6]);
        state_machine = BMSLStateMachine();
        uint8_t opt_adc = ADC::inscribe(avionics_current_sensor_pin).value_or(69);
        
        if (opt_adc != 69) {
            avionics_current_sensor = opt_adc;
        }
        else {
            ErrorHandler("Could not inscribe ADC for avionics current sensor");
        }
    }


    float get_avionics_current() {
        return ADC::get_value(avionics_current_sensor);
    }
    
    namespace Orders {
        void start_balancing() {
            bms.start_balancing();
        }

        void stop_balancing() {
            bms.stop_balancing();
        }
    }
};