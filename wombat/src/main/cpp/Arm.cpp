#include "Arm.h"

using namespace frc;
using namespace wom;

Arm::Arm(ArmConfig config) : _config(config), _pid(config.path + "/pid", config.pidConfig) { }

void Arm::OnUpdate(units::second_t dt) {
  units::volt_t voltage = 0_V;
  switch (_state) {
    case ArmState::kIdle:
      break;
    case ArmState::kZeroing:
      voltage = -2_V;
      if (_config.lowerLimitSwitch->Get()) {
        _config.gearbox.encoder->ZeroEncoder();
        _state = ArmState::kIdle;
        /*when bottom limit switch is triggered, the encoder is zeroed and returns to the idle state*/
      }
      break;
    case ArmState::kAngle:
      {
        voltage = _pid.Calculate(_config.gearbox.encoder->GetEncoderPosition(),  dt);
        /*creates a pid controller*/
        // units::radian_t currentAngle = _config.gearbox.encoder->GetEncoderPosition();
        // units::radian_t error = _targetAngle - currentAngle;
        // voltage = 12_V / 20_deg * error;
      }
      break;
  }
  _config.gearbox.transmission->SetVoltage(voltage);
}

void Arm::SetIdle() {
  _state = ArmState::kIdle;
}

void Arm::SetZeroing() {
  _state = ArmState::kZeroing;
}

void Arm::SetAngle(units::radian_t angle) {
  _state = ArmState::kAngle;
  _pid.SetSetpoint(angle);
}

ArmConfig &Arm::GetConfig() {
  return _config;
}
/* SIMULATION */
#include <units/math.h>

::wom::sim::ArmSim::ArmSim(ArmConfig config) 
  : config(config),
    angle(config.initialAngle),
    encoder(config.gearbox.encoder->MakeSimEncoder()),
    lowerLimit(config.lowerLimitSwitch ? new frc::sim::DIOSim(*config.lowerLimitSwitch) : nullptr),
    upperLimit(config.upperLimitSwitch ? new frc::sim::DIOSim(*config.upperLimitSwitch) : nullptr),
    table(nt::NetworkTableInstance::GetDefault().GetTable(config.path + "/sim"))
  {}

units::ampere_t wom::sim::ArmSim::GetCurrent() const {
  return current;
}

void ::wom::sim::ArmSim::Update(units::second_t dt) {
  torque = (config.loadMass * config.armLength + config.armMass * config.armLength / 2.0) * 9.81_m / 1_s / 1_s * units::math::cos(config.angleOffset + angle) + additionalTorque;
  velocity = config.gearbox.motor.Speed(torque, config.gearbox.transmission->GetEstimatedRealVoltage());
  angle += velocity * dt;

  if (angle <= config.minAngle) {
    angle = config.minAngle;
    velocity = 0_rad / 1_s;
    if (lowerLimit) lowerLimit->SetValue(true);
  } else {
    if (lowerLimit) lowerLimit->SetValue(false);
  }

  if (angle >= config.maxAngle) {
    angle = config.maxAngle;
    velocity = 0_rad / 1_s;
    if (upperLimit) upperLimit->SetValue(true);
  } else {
    if (upperLimit) upperLimit->SetValue(false);
  }

  current = config.gearbox.motor.Current(velocity, config.gearbox.transmission->GetVoltage());

  if (encoder) encoder->SetEncoderTurns(angle);

  table->GetEntry("angle").SetDouble(angle.convert<units::degree>().value());
  table->GetEntry("current").SetDouble(current.value());
}
