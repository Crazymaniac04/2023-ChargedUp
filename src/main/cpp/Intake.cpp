#include "Intake.h"

using namespace frc;
using namespace wom;

Intake::Intake(IntakeConfig config) : _config(config) { }

void Intake::OnUpdate(units::second_t dt) {

  /* Depending on the state, we set a different voltage */
  switch (_state) {
  case IntakeState::kIdle:
    _config.clamp->Set(frc::DoubleSolenoid::kOff);
    break;
  case IntakeState::kIntaking:
    _config.clamp->Set(frc::DoubleSolenoid::kForward);
    _config.stow->Set(frc::DoubleSolenoid::kForward);
   break;
    
  case IntakeState::kOuttaking:
    _config.clamp->Set(frc::DoubleSolenoid::kReverse);
    _config.stow->Set(frc::DoubleSolenoid::kForward);
    break;

  case IntakeState::kStow:
    _config.stow->Set(frc::DoubleSolenoid::kReverse);
    _config.clamp->Set(frc::DoubleSolenoid::kForward);
    break;
    // _config.solenoid->Toggle();

  }

  _config.gearbox.transmission->SetVoltage(_voltage);
}

void Intake::SetIntaking(units::volt_t v) {
  _voltage = v;
  _state = IntakeState::kIntaking;
}

void Intake::SetOuttaking(units::volt_t v) {
  _voltage = v;
  _state = IntakeState::kOuttaking;
}

void Intake::SetStow() {
  _voltage = 0_V;
  _state = IntakeState::kStow;
}

IntakeState Intake::GetState() const {
  return _state;
}