#include "BMS-LIB.hpp"

#define ACTIVE_DISCHARGE PE2
#define LAN_INT PC13
#define LSE_IN PC14
#define LSE_OUT PC15
#define BUFFER_EN PF4
#define MDC PC1
#define AVIONICSCURRENTSENSORFW PA0
#define MDIO PA2
#define TEMPINVERTERFW PA3
#define TEMPCAPACITORFW PA4
#define TEMPTRANSFORMERFW PA5
#define TEMPRECTIFIERFW PA6
#define CRS_DV PA7
#define RXD0 PC4
#define RXD1 PC5
#define INPUTVOLTAGEFW PB0
#define INPUTCURRENTFW PB1
#define OUTPUTVOLTAGEFW PF11
#define OUTPUTCURRENTFW PF12
#define PHY_RST PG0
#define DO_INV_PWM_L1 PE8
#define DO_INV_PWM_H1 PE9
#define DO_INV_PWM_L2 PE12
#define DO_INV_PWM_H2 PE13
#define GATE_DRIVERS_EN PE15
#define TXD1 PB13
#define LED_LOW_CHARGE PG2
#define LED_FULL_CHARGE PG3
#define LED_SLEEP PG4
#define LED_FLASH PG5
#define LED_CAN PG6
#define LED_FAULT PG7
#define LED_OPERATIONAL PG8
#define SLNT PA8
#define PGOOD_12V PA11
#define PGOOD_16V PA12
#define SWDIO PA13
#define SWCLK PA14
#define JTDI PA15
#define SPI_CLK PC10
#define SPI_MISO PC11
#define SPI_MOSI PC12
#define CAN1_RX PD0
#define CAN1_TX PD1
#define SPI_CS PD3
#define UART_TX PD5
#define UART_RX PD6
#define TXEN PG11
#define TXD0 PG13
#define SWO PB3
#define HW_FAULT PE0
#define RX_ER PE1

namespace BMSL {

    BMSA bms;
    HalfBridge dclv;
    uint8_t avionics_current_sensor;

    namespace States {
        enum General {
            CONNECTING,
            OPERATIONAL,
            FAULT
        };

        enum Operational {
            WAITING,
            CHARGING
        };

        enum Charging {
            PRECHARGE,
            CONSTANT_CURRENT,
            CONSTANT_VOLTAGE
        };
    }

    namespace StateMachines {
        StateMachine general;
        StateMachine operational;
        StateMachine charging;

        void start() {
            general = StateMachine(States::CONNECTING);
            general.add_state(States::OPERATIONAL);
            general.add_state(FAULT);

            operational = StateMachine(States::WAITING);
            operational.add_state(States::CHARGING);

            charging = StateMachine(States::PRECHARGE);
            charging.add_state(States::CONSTANT_CURRENT);
            charging.add_state(States::CONSTANT_VOLTAGE);

            operational.add_state_machine(charging, States::CHARGING);
            general.add_state_machine(operational, States::OPERATIONAL);
        }
    }
    namespace Leds {
        DigitalOutput low_charge;
        DigitalOutput full_charge;
        DigitalOutput sleep;
        DigitalOutput flash;
        DigitalOutput can;
        DigitalOutput fault;
        DigitalOutput operational;
    }


    namespace Orders {
        void start_balancing();
        void stop_balancing();
    }

    namespace Communications {
        DatagramSocket udp_socket;
    }
    
    void inscribe();
    void update();
    float get_avionics_current();

    void inscribe() {
        bms = BMSA(SPI::spi3);
        dclv = HalfBridge(DO_INV_PWM_H1, DO_INV_PWM_L1, DO_INV_PWM_H2, DO_INV_PWM_L2, GATE_DRIVERS_EN);

        Leds::can = DigitalOutput(LED_CAN);
        Leds::fault = DigitalOutput(LED_FAULT);
        Leds::flash = DigitalOutput(LED_FLASH);
        Leds::full_charge = DigitalOutput(LED_FULL_CHARGE);
        Leds::low_charge = DigitalOutput(LED_LOW_CHARGE);
        Leds::operational = DigitalOutput(LED_OPERATIONAL);
        Leds::sleep = DigitalOutput(LED_SLEEP);

        avionics_current_sensor = ADC::inscribe(AVIONICSCURRENTSENSORFW);
    }

    void start() {
        STLIB::start();
        StateMachines::start();
        
    }

    void update() {
        STLIB::update();
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