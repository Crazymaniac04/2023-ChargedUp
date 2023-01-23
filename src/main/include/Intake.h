#pragma once

#include "Gearbox.h"
#include <frc/DigitalInput.h>
#include <frc/DoubleSolenoid.h>

#include "behaviour/HasBehaviour.h"

/**
 * The IntakeConfig describes all the resources and parameters for the Intake Subsystem.
 * This includes our actuators (motors), sensors, as well as configuration parameters
 * such as the voltage required to perform actions. 
 */
struct IntakeConfig {
  wom::Gearbox gearbox;
  frc::DoubleSolenoid *clamp;
  frc::DoubleSolenoid *stow;
};

/**
 * IntakeState defines the primary states of operation for the Intake:
 *  - kIdle: Not currently performing any actions - waiting to intake.
 *  - kIntaking: Intaking, but not yet with a gear.
 *  - kFull: We currently have a gear and are waiting to deliver
 *  - kOuttaking: Ejecting a currently held workpiece.
 */
enum class IntakeState {
  kIdle,
  kIntaking,
  kOuttaking,
  kStow
};

/**
 * The Intake Subsystem. This subsystem defines the operation of the Intake, 
 * including managing the transition between states and control of actuators.
 */
class Intake : public behaviour::HasBehaviour {
 public:
  Intake(IntakeConfig config);

  /**
   * The OnUpdate method is called periodically (50 times a second) to perform
   * the action of the intake. 
   */
  void OnUpdate(units::second_t dt);

  /**
   * Set the intake to "Intaking" mode.
   */
  void SetIntaking(units::volt_t v = 0_V);

  /**
   * Set the intake to "Outtaking" mode.
   */
  void SetOuttaking(units::volt_t v = 0_V);

  /**
   * Set the intake to stowed "Intake Up"
  */
  void SetStow();

  /**
   * Get the current intake state.
   */
  IntakeState GetState() const;
 private:
  IntakeConfig _config;
  IntakeState _state = IntakeState::kIdle;
  units::volt_t _voltage;
};