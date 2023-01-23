#include "Robot.h"

#include "behaviour/BehaviourScheduler.h"
#include "behaviour/Behaviour.h"
#include "behaviour/SwerveBaseBehaviour.h"

#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/event/BooleanEvent.h>

using namespace frc;
using namespace behaviour;

static units::second_t lastPeriodic;

void Robot::RobotInit() {
  lastPeriodic = wom::now();

  // armavator = new Armavator(map.armavator.config);
  // BehaviourScheduler::GetInstance()->Register(armavator);

  swerve = new wom::SwerveDrive(map.swerveBase.config, frc::Pose2d());
  // map.swerveBase.moduleConfigs[0].turnMotor.transmission->SetInverted(true);
  // map.swerveBase.moduleConfigs[2].turnMotor.transmission->SetInverted(true);
  BehaviourScheduler::GetInstance()->Register(swerve);
  swerve->SetDefaultBehaviour([this]() {
    return make<ManualDrivebase>(swerve, &map.controllers.driver);
  });
}

void Robot::RobotPeriodic() {
  auto dt = wom::now() - lastPeriodic;
  lastPeriodic = wom::now();
  
  loop.Poll();
  BehaviourScheduler::GetInstance()->Tick();

  // map.swerveBase.turnMotors[0]->Set(map.controllers.driver.GetRightX());
  // map.swerveBase.driveMotors[0]->Set(map.controllers.driver.GetLeftY());

  std::shared_ptr<nt::NetworkTable> _table = nt::NetworkTableInstance::GetDefault().GetTable("encoderValues");
  _table->GetEntry("speed").SetDouble(map.swerveBase.moduleConfigs[0].driveMotor.encoder->GetEncoderPosition().convert<units::degree>().value());
  _table->GetEntry("angle").SetDouble(map.swerveBase.moduleConfigs[0].turnMotor.encoder->GetEncoderPosition().convert<units::degree>().value());
  map.swerveBase.moduleConfigs[0].WriteNT(_table->GetSubTable("encoderValues"));



  // armavator->OnUpdate(dt);
  swerve->OnUpdate(dt);
}

void Robot::AutonomousInit() { }
void Robot::AutonomousPeriodic() { }

void Robot::TeleopInit() {
  loop.Clear();
  BehaviourScheduler *sched = BehaviourScheduler::GetInstance();
  
  // map.controllers.driver.A(&loop).Rising().IfHigh([sched, this]() {
  //   sched->Schedule(make<ArmavatorGoToPositionBehaviour>(armavator, ArmavatorPosition{0.2_m, 0_deg}));
  // });

  // map.controllers.driver.B(&loop).Rising().IfHigh([sched, this]() {
  //   sched->Schedule(make<ArmavatorGoToPositionBehaviour>(armavator, ArmavatorPosition{1.2_m, -75_deg}));
  // });

  // map.controllers.driver.X(&loop).Rising().IfHigh([sched, this]() {
  //   sched->Schedule(make<ArmavatorGoToPositionBehaviour>(armavator, ArmavatorPosition{1.0_m, 240_deg}));
  // });

  // map.controllers.driver.Y(&loop).Rising().IfHigh([sched, this]() {
  //   sched->Schedule(make<ArmavatorGoToPositionBehaviour>(armavator, ArmavatorPosition{0_m, 0_deg}));
  // });



  //map.controllers.driver.POV(0, &loop).Rising().IfHigh([sched, this]() { // up dpad
  //  sched->Schedule(make<DrivebasePoseBehaviour>(swerve, frc::Pose2d(1_m, 1_m, 0_rad)));
  //});
  //swerve->OnStart();


  // Swervedrivebase poses
  

  map.controllers.driver.POV(0, &loop).Rising().IfHigh([sched, this]() { // up dpad
    if (map.controllers.driver.GetAButton()) {
      if (map.controllers.driver.GetXButton()){
        sched->Schedule(make<DrivebasePoseBehaviour>(swerve, map.swerveGridPoses.centreGrid2)); // central grid
      } else {
        sched->Schedule(make<DrivebasePoseBehaviour>(swerve, map.swerveGridPoses.outerGrid3)); // Outer Grid 3 (furthest from centre)
      }
    } else {
      sched->Schedule(make<DrivebasePoseBehaviour>(swerve,map.swerveGridPoses.innerGrid1)); // Inner Grid 1 (furthest from centre)
    }
  });
  map.controllers.driver.POV(90, &loop).Rising().IfHigh([sched, this]() { // right dpad
    if (map.controllers.driver.GetAButton()) {
      sched->Schedule(make<DrivebasePoseBehaviour>(swerve, map.swerveGridPoses.outerGrid2)); // Outer Grid 2
    } else {
      sched->Schedule(make<DrivebasePoseBehaviour>(swerve, map.swerveGridPoses.innerGrid2)); // Inner Grid 2
    }
  });
  map.controllers.driver.POV(180, &loop).Rising().IfHigh([sched, this]() { // down dpad
    if (map.controllers.driver.GetAButton()) {
      sched->Schedule(make<DrivebasePoseBehaviour>(swerve, map.swerveGridPoses.outerGrid1)); // Outer Grid 1 (closest to centre)
    } else{
      sched->Schedule(make<DrivebasePoseBehaviour>(swerve, map.swerveGridPoses.innerGrid3)); // Inner Grid 3 (closest to centre)
    }
  });
  map.controllers.driver.POV(270, &loop).Rising().IfHigh([sched, this]() { // left dpad
    if (map.controllers.driver.GetAButton()) {
      sched->Schedule(make<DrivebasePoseBehaviour>(swerve, map.swerveGridPoses.centreGrid3)); // Community Grid 3 (outer grid side)
    } else {
      sched->Schedule(make<DrivebasePoseBehaviour>(swerve, map.swerveGridPoses.centreGrid1)); // Community Grid 1 (inner grid side)
    }
  });
  swerve->OnStart();

}

void Robot::TeleopPeriodic() { }

void Robot::DisabledInit() { }
void Robot::DisabledPeriodic() { }

void Robot::TestInit() { }
void Robot::TestPeriodic() { }

/* SIMULATION */

// #include <frc/simulation/BatterySim.h>
// #include <frc/simulation/RoboRioSim.h>
// #include <networktables/NetworkTableInstance.h>
// #include "ControlUtil.h"

// static units::second_t lastSimPeriodic{0};
// static auto simTable = nt::NetworkTableInstance::GetDefault().GetTable("/sim");

// struct SimConfig {
//   ::sim::ArmavatorSim arm;
//   wom::sim::SwerveDriveSim swerveSim; 
// };
// SimConfig *simConfig;

// void Robot::SimulationInit() {
//   simConfig = new SimConfig{
//     ::sim::ArmavatorSim(map.armavator.config),
//     wom::sim::SwerveDriveSim(map.swerveBase.config, 0.5 * 6_lb * 7.5_in * 7.5_in)
//   };

//   lastSimPeriodic = wom::now();
// }

// void Robot::SimulationPeriodic() {
//   auto dt = wom::now() - lastSimPeriodic;

//   simConfig->arm.OnUpdate(dt);
//   simConfig->swerveSim.Update(dt);

//   auto batteryVoltage = units::math::min(units::math::max(frc::sim::BatterySim::Calculate({
//     simConfig->arm.GetCurrent(),
//   }), 0_V), 12_V);
//   frc::sim::RoboRioSim::SetVInVoltage(batteryVoltage);
//   simTable->GetEntry("batteryVoltage").SetDouble(batteryVoltage.value()); 

//   lastSimPeriodic = wom::now();
// }