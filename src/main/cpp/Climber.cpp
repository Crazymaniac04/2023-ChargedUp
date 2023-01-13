#include "Climber.h"

using namespace frc;
using namespace wom;

Climber::Climber(climberConfig config) : _config(config){

} 


void Climber::OnUpdate(units::second_t dt){
  units::volt_t voltage = 0_V;

switch (_state) {
  case climberState::kIdle:
    voltage = _config.IdleVoltage;
    break;
  case climberState::kAscending:
    voltage = _config.AscendingVoltage;

    break;
  case climberState::kDescending:
    voltage = _config.DescendingVoltage;
    break;
  case climberState::kLocked:
    voltage = _config.LockedVoltage;
    break;
  }

  
}
