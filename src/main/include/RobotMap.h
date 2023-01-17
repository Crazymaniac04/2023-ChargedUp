#pragma once

#include "Intake.h"
#include  "Climber.h"
#include "VoltageController.h"
#include "DCMotor.h"
#include <frc/XboxController.h>
#include <ctre/Phoenix.h>
#include <frc/DoubleSolenoid.h>

struct RobotMap {
  struct Controllers {
    frc::XboxController driver{0};
    frc::XboxController coDriver{1};

    const double deadzone = 0.15;
  };
  Controllers controllers;

  /**
   * Resources and Paramters related to the Intake Subsystem
   */
  struct IntakeSystem {
    /* Create a new Voltage Controller */
    wom::MotorVoltageController controller{new WPI_TalonSRX(10)};
    frc::DoubleSolenoid solenoid{1, frc::PneumaticsModuleType::CTREPCM, 999,999};
 
    
    wom::Gearbox gearbox{
      &controller,
      nullptr,  /* nullptr for encoder means we have no encoder */
      wom::DCMotor::Bag(1).WithReduction(1) /* TODO: Update this reduction */
    };

   
   

    /* Create the intake config with the given resources */
    IntakeConfig config{
      gearbox,
      &solenoid
    };
  };
  IntakeSystem intake;
  
 struct climberSystem {
  wom::MotorVoltageController climberMotor{new WPI_TalonSRX(99)};

  wom::Gearbox climberGearbox {
    &climberMotor,
    nullptr,
    wom::DCMotor::CIM(1).WithReduction(1)
  };

  climberConfig config {
    climberGearbox
  };
 };
climberSystem climber;

};