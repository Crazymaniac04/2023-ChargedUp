#include "Robot.h"
#include <frc/smartdashboard/SmartDashboard.h>

#include "behaviour/BehaviourScheduler.h"
#include "behaviour/Behaviour.h"

using namespace frc;
using namespace behaviour;

void Robot::RobotInit() {
  /* Create a new intake */
  intake = new Intake(map.intake.config);
}
void Robot::RobotPeriodic() {
  /* Update the intake */
  intake->OnUpdate(20_ms);

  BehaviourScheduler::GetInstance()->Tick();
}

void Robot::AutonomousInit() { }
void Robot::AutonomousPeriodic() {}

void Robot::TeleopInit() { }

void Robot::TeleopPeriodic() {

  // Intake motor power
  double motorPower = 0;
  if (map.controllers.driver.GetRightTriggerAxis() > map.controllers.deadzone) {
    motorPower = map.controllers.driver.GetRightTriggerAxis();
  } else if (map.controllers.driver.GetLeftTriggerAxis() > map.controllers.deadzone) {
    motorPower = -map.controllers.driver.GetLeftTriggerAxis();
  } else {
    motorPower = 0.0;
  }

  // Intake solenoids
  if (map.controllers.driver.GetAButton()) {
    intake->SetIntaking(motorPower * 12.0_V);
  } else if (map.controllers.driver.GetBButton()) {
    intake->SetOuttaking(motorPower * 12.0_V);
  } else if (map.controllers.driver.GetYButton()) {
    intake->SetStow();
  }
}

void Robot::DisabledInit() {}
void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}
void Robot::TestPeriodic() {}