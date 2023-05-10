#include "BMS-LIB.hpp"

class BMSLLeds {
private:
    DigitalOutput low_charge;
    DigitalOutput full_charge;
    DigitalOutput sleep;
    DigitalOutput flash;
    DigitalOutput can;
    DigitalOutput fault;
    DigitalOutput operational;
public:
    BMSLLeds() = default;
    BMSLLeds(
        DigitalOutput low_charge,
        DigitalOutput full_charge,
        DigitalOutput sleep,
        DigitalOutput flash,
        DigitalOutput can,
        DigitalOutput fault,
        DigitalOutput operational
    );
};

BMSLLeds::BMSLLeds(DigitalOutput low_charge, DigitalOutput full_charge, DigitalOutput sleep, DigitalOutput flash, DigitalOutput can, DigitalOutput fault, DigitalOutput operational) :
    low_charge(low_charge),
    full_charge(full_charge),
    sleep(sleep),
    flash(flash),
    can(can),
    fault(fault),
    operational(operational) {}