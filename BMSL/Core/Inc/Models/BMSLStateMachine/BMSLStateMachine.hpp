#include "BMS-LIB.hpp"

class BMSLStateMachine {
private:
    enum GENERAL_STATES {
        CONNECTING,
        OPERATIONAL,
        FAULT
    };

    enum OPERATIONAL_STATES {
        WAITING,
        CHARGING
    };

    enum CHARGING_STATES {
        PRECHARGE,
        CONSTANT_CURRENT,
        CONSTANT_VOLTAGE
    };

    StateMachine general;
    StateMachine operational;
    StateMachine charging;

public:
    BMSLStateMachine();
};

BMSLStateMachine::BMSLStateMachine() {
	general = StateMachine(CONNECTING);
	general.add_state(OPERATIONAL);
	general.add_state(FAULT);

	operational = StateMachine(WAITING);
	operational.add_state(CHARGING);

	charging = StateMachine(PRECHARGE);
	charging.add_state(CONSTANT_CURRENT);
	charging.add_state(CONSTANT_VOLTAGE);

	operational.add_state_machine(charging, CHARGING);
	general.add_state_machine(operational, OPERATIONAL);
}