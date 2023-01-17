#include "Intake.h"

using namespace frc;
using namespace wom;

Intake::Intake(IntakeConfig config) : _config(config) { }

void Intake::OnUpdate(units::second_t dt) {
  units::volt_t voltage = 0_V;

  /* Depending on the state, we set a different voltage */
  switch (_state) {
  case IntakeState::kIdle:
    _config.solenoid->Set(frc::DoubleSolenoid::kOff);
    break;
  case IntakeState::kIntaking:
    _config.solenoid->Set(frc::DoubleSolenoid::kForward);
   break;
    
  case IntakeState::kOuttaking:
    _config.solenoid->Set(frc::DoubleSolenoid::kReverse);
    break;
    // _config.solenoid->Toggle();

  }

  _config.gearbox.transmission->SetVoltage(voltage);
}

void Intake::SetIntaking() {
  _state = IntakeState::kIntaking;
}

void Intake::SetOuttaking() {
  _state = IntakeState::kOuttaking;
}

IntakeState Intake::GetState() const {
  return _state;
}