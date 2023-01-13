#pragma once

#include "Gearbox.h"
#include <frc/DigitalInput.h>

#include "behaviour/HasBehaviour.h"

struct climberConfig {
wom::Gearbox climberGearbox;

units::volt_t AscendingVoltage = 10_V;
units::volt_t DescendingVoltage = -5_V;
units::volt_t LockedVoltage = 6_V;
units::volt_t IdleVoltage = 0_V;

};

/**
 * idle is idle
 * locked is held out of way of intake
 * ascending is going up
 * descending is going down
*/

enum class climberState {
 kIdle = 0,
 kLocked,
 kAscending,
 kDescending
};

/**
 * 
*/

class Climber : public behaviour::HasBehaviour {
 public:
    Climber(climberConfig config);

    void OnUpdate(units::second_t dt);

    void SetAscending();

    void SetDescending();

    climberState getState() const;
 private:
    climberConfig _config;
    climberState _state = climberState::kIdle;
};