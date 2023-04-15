/**
 * @file ConfigurationParser.cpp
 * @author John Koch jkoch21@usf.edu
 * @brief JSON configuration parser designed to be paired with configuration.json to 
 * extract a current configuration of the BullBot.
 *     
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "../../../include/TerriBull/lib/ConfigurationParser.hpp"

/* Include All Items Needed for Development of System */
/* Drive */
#include "../../../include/MechanicalComponents/Drive/configurations/tank_drive_std.hpp"
#include "../../../include/MechanicalComponents/Drive/configurations/tank_drive_quad.hpp"
#include "../../../include/MechanicalComponents/Drive/configurations/x_drive.hpp"

/* Intake */
#include "../../../include/MechanicalComponents/Intakes/Configurations/Intake_Uni.hpp"
#include "../../../include/MechanicalComponents/Intakes/Configurations/Intake_Duo.hpp"

/* Roller */
#include "../../../include/MechanicalComponents/Rollers/Configurations/RollerUni.hpp"
#include "../../../include/MechanicalComponents/Rollers/Configurations/RollerDuo.hpp"

/* Shooter */
#include "../../../include/MechanicalComponents/Shooters/Configurations/Catapult/CatapultZolt.hpp"
#include "../../../include/MechanicalComponents/Shooters/Configurations/FlyWheel/FlyWheelSB.hpp"
#include "../../../include/MechanicalComponents/Shooters/Magazines/Magazine.hpp"

/* Ctrls*/
#include "../../../include/Controllers/InputController/Configurations/AidanJoeShmo.hpp"
#include "../../../include/Controllers/InputController/Configurations/DanTheMan.hpp"
#include "../../../include/Controllers/InputController/Configurations/NateIsTank.hpp"
#include "../../../include/Controllers/InputController/Configurations/RyanIsTank.hpp"

/*END INCLUDE*/

ConfigurationParser::~ConfigurationParser () {
    pFile->close();
    delete pFile;
}

TerriBull::Drive* ConfigurationParser::getDriveConfig() {
    if (this->pConfigVariables.DriveConfig.Config.isNull() || this->pConfigVariables.DriveConfig.MotorPorts.isNull() || this->pConfigVariables.DriveConfig.MotorGearset.isNull() || this->pConfigVariables.DriveConfig.WheelRadius.isNull() || this->pConfigVariables.DriveConfig.WheelRadius.isNull() || this->pConfigVariables.DriveConfig.ConversionFactor.isNull()) {
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    Json::String DriveType = this->pConfigVariables.DriveConfig.Config.asString();
    Json::String gearSet = this->pConfigVariables.DriveConfig.MotorGearset.asString();
    
    /* Drive Configurations that are Currently Supported */
    if (DriveType == "x_drive") {
        return new X_Drive(this->pConfigVariables.DriveConfig.MotorPorts[0].asInt(), this->pConfigVariables.DriveConfig.MotorPorts[1].asInt(), this->pConfigVariables.DriveConfig.MotorPorts[2].asInt(), this->pConfigVariables.DriveConfig.MotorPorts[3].asInt(), GEAR_ENCODER[gearSet], this->pConfigVariables.DriveConfig.ConversionFactor.asFloat(), this->pConfigVariables.DriveConfig.WheelRadius.asFloat(), this->pConfigVariables.DriveConfig.WheelBase.asFloat(), this->pConfigVariables.DriveConfig.KPos[0].asFloat(), this->pConfigVariables.DriveConfig.KPos[1].asFloat(), this->pConfigVariables.DriveConfig.KPos[2].asFloat(), this->pConfigVariables.DriveConfig.KTheta[0].asFloat(), this->pConfigVariables.DriveConfig.KTheta[1].asFloat(), this->pConfigVariables.DriveConfig.KTheta[2].asFloat());
    }
    if (DriveType == "tank_drive_std") {
        return new Tank_Drive_Std(this->pConfigVariables.DriveConfig.MotorPorts[0].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[0].asBool(), this->pConfigVariables.DriveConfig.MotorPorts[1].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[1].asBool(), this->pConfigVariables.DriveConfig.MotorPorts[2].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[2].asBool(), this->pConfigVariables.DriveConfig.MotorPorts[3].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[3].asBool(), this->pConfigVariables.DriveConfig.MotorPorts[4].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[4].asBool(), this->pConfigVariables.DriveConfig.MotorPorts[5].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[5].asBool(), GEAR_ENCODER[gearSet], this->pConfigVariables.DriveConfig.ConversionFactor.asFloat(), this->pConfigVariables.DriveConfig.WheelRadius.asFloat(), this->pConfigVariables.DriveConfig.WheelBase.asFloat(), this->pConfigVariables.DriveConfig.KPos[0].asFloat(), this->pConfigVariables.DriveConfig.KPos[2].asFloat(), this->pConfigVariables.DriveConfig.KPos[2].asFloat(), this->pConfigVariables.DriveConfig.KTheta[0].asFloat(), this->pConfigVariables.DriveConfig.KTheta[1].asFloat(), this->pConfigVariables.DriveConfig.KTheta[2].asFloat());
    }
    if (DriveType == "tank_drive_quad") {
        return new Tank_Drive_Quad(this->pConfigVariables.DriveConfig.MotorPorts[0].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[0].asBool(), this->pConfigVariables.DriveConfig.MotorPorts[1].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[1].asBool(), this->pConfigVariables.DriveConfig.MotorPorts[2].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[2].asBool(), this->pConfigVariables.DriveConfig.MotorPorts[3].asInt(), this->pConfigVariables.DriveConfig.MotorReverse[3].asBool(), GEAR_ENCODER[gearSet], this->pConfigVariables.DriveConfig.ConversionFactor.asFloat(), this->pConfigVariables.DriveConfig.WheelRadius.asFloat(), this->pConfigVariables.DriveConfig.WheelBase.asFloat(), this->pConfigVariables.DriveConfig.KPos[0].asFloat(), this->pConfigVariables.DriveConfig.KPos[2].asFloat(), this->pConfigVariables.DriveConfig.KPos[2].asFloat(), this->pConfigVariables.DriveConfig.KTheta[0].asFloat(), this->pConfigVariables.DriveConfig.KTheta[1].asFloat(), this->pConfigVariables.DriveConfig.KTheta[2].asFloat());
    }
    else return nullptr;
}

TerriBull::InputController* ConfigurationParser::getInputControllerConfig(RoboController* roboController) {
    // return new AidanJoeShmo(roboController, 5);
    if (this->pConfigVariables.ControllerConfig.isNull()) {
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    Json::String ConfigType = this->pConfigVariables.ControllerConfig.asString();
    int deadzone = this->pConfigVariables.ControllerDeadzone.asInt();

    /* Controller Configurations that are Currently Supported */
    if (ConfigType == "AidanJoeShmo") {
        return new AidanJoeShmo(roboController, deadzone);
    }
    if (ConfigType == "DanTheMan") {
        return new DanTheMan(roboController, deadzone);
    }
    if (ConfigType == "NateIsTank") {
        return new NateIsTank(roboController, deadzone);
    }
    if (ConfigType == "RyanIsTank") {
        return new RyanIsTank(roboController, deadzone);
    }
    return nullptr;
}

TerriBull::Intake* ConfigurationParser::getIntakeConfig() {
    /*return nullptr;*/Json::Value IntakeConfig = this->pConfigVariables.Config["mechanical_system"]["intake"];
    if (IntakeConfig.isNull()) {
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    Json::String ConfigType = IntakeConfig["config"].asString();
    Json::String gearSet = IntakeConfig["gear_ratio"].asString();
    /* Controller Configurations that are Currently Supported */
    if (ConfigType == "IntakeUni") {
        return new Intake_Uni(IntakeConfig["motor_ports"][0].asInt(), IntakeConfig["reverse_motors"][0].asBool(), IntakeConfig["max_speed"].asInt(), GEAR_ENCODER[gearSet]);
    }
    if (ConfigType == "IntakeDuo") {
        return new Intake_Duo(IntakeConfig["motor_ports"][0].asInt(),IntakeConfig["reverse_motors"][0].asBool(), IntakeConfig["motor_ports"][1].asInt(), IntakeConfig["reverse_motors"][1].asBool(), IntakeConfig["max_speed"].asInt(), GEAR_ENCODER[gearSet]);
    }

    return nullptr;
}

TerriBull::Roller* ConfigurationParser::getRollerConfig() {
    Json::Value RollerConfig = this->pConfigVariables.Config["mechanical_system"]["roller"];
    if (RollerConfig.isNull()) {
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    Json::String ConfigType = RollerConfig["config"].asString();
    Json::String gearSet = RollerConfig["gear_ratio"].asString();
    /* Controller Configurations that are Currently Supported */
    if (ConfigType == "RollerUni") {
        return new Roller_Uni(RollerConfig["motor_ports"][0].asInt(), RollerConfig["reverse_motors"][0].asBool(), RollerConfig["max_speed"].asInt(), GEAR_ENCODER[gearSet]);
    }
    if (ConfigType == "RollerDuo") {
        return new Roller_Duo(RollerConfig["motor_ports"][0].asInt(), RollerConfig["reverse_motors"][0].asBool(), RollerConfig["motor_ports"][1].asInt(), RollerConfig["reverse_motors"][1].asBool(), RollerConfig["max_speed"].asInt(), GEAR_ENCODER[gearSet]);
    }
    return nullptr;
}

TerriBull::Shooter* ConfigurationParser::getShooterConfig(TerriBull::MechanicalSystem* _system) {
    Json::Value ShooterConfig = this->pConfigVariables.Config["mechanical_system"]["shooter"];
    if (ShooterConfig.isNull()) {
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    Json::String ConfigType = ShooterConfig["config"].asString();
    Json::String gearSet = ShooterConfig["gear_ratio"].asString();
    /* Controller Configurations that are Currently Supported */
    if (ConfigType == "CatapultZolt") {
        return new CatapultZolt(ShooterConfig["motor_ports"][0].asInt(), ShooterConfig["reverse_motors"][0].asBool(), ShooterConfig["motor_ports"][1].asInt(), ShooterConfig["reverse_motors"][1].asBool(), ShooterConfig["limit_port"].asString()[0], GEAR_ENCODER[gearSet]);
    }
    else if (ConfigType == "ShooterFlyWheelSB") {
        /* Parse for the Magazine as well*/
        Json::Value MagazineConfig = this->pConfigVariables.Config["mechanical_system"]["magazine"];
        bool* increment_bools = new bool[2];
        bool* decrement_bools = new bool[2];
        increment_bools[0] = MagazineConfig["inc_default"][0].asBool(); increment_bools[1] = MagazineConfig["inc_default"][1].asBool();
        decrement_bools[0] = MagazineConfig["dec_default"][0].asBool(); decrement_bools[1] = MagazineConfig["dec_default"][1].asBool();
        Magazine* mag = new Magazine(MagazineConfig["incPorts"].asString(), increment_bools, MagazineConfig["decPorts"].asString(), decrement_bools);
        return new FlyWheelSB(ShooterConfig["motor_ports"][0].asInt(), ShooterConfig["reverse_motors"][0].asBool(), ShooterConfig["motor_ports"][1].asInt(), ShooterConfig["reverse_motors"][1].asBool(), mag, GEAR_ENCODER[gearSet], _system);
    }
    return nullptr;
}

TerriBull::MechanicalSystem* ConfigurationParser::getMechanicalSystemConfig() {
    if (this->pConfigVariables.Config.isNull() || this->pConfigVariables.IMUConfig.isNull()) {
        pros::lcd::set_text(3, "Null IMU : " + this->pConfigVariables.IMUConfig.asString());
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    TerriBull::Drive* drive = this->getDriveConfig();
    if (drive == nullptr) {
        pros::lcd::set_text(3, "Null Drivebase : " + this->pConfigVariables.DriveConfig.Config.asString());
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    if (this->pConfigVariables.StartingAngle.isNull() || this->pConfigVariables.StartingPos.isNull()) {
        pros::lcd::set_text(3, "Null Start Info : " + this->pConfigVariables.DriveConfig.Config.asString());
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }

    /* Construct a new base System */
    TerriBull::MechanicalSystem* system = new TerriBull::MechanicalSystem(this->pConfigVariables.IMUConfig.asInt(), drive);
    system->setStartingAngle(this->pConfigVariables.StartingAngle.asFloat());
    system->setStartingPosition(this->pConfigVariables.StartingPos["x"].asFloat(), this->pConfigVariables.StartingPos["y"].asFloat());
    system->Init();

    /* Iterate through Mechanical System Config Member Fields */
    for (auto ref : this->pConfigVariables.Config["mechanical_system"].getMemberNames()) {
        /* This Section is to be Continued */
        if (ref == "roller") {
            TerriBull::Roller* roller = this->getRollerConfig();
            if(roller != nullptr) {
                system->setRoller(roller);
            }
        }
        else if (ref == "intake") {
            TerriBull::Intake* intake = this->getIntakeConfig();
            if (intake != nullptr) {
                system->setIntake(intake);
            }
        }

        else if (ref == "shooter") {
            TerriBull::Shooter* shooter = this->getShooterConfig(system);
            if (shooter!= nullptr) {
                system->setShooter(shooter);
            }
        }

        /* TODO: add other system components here */

    }
    // pros::lcd::set_text(4, "Created System : ");
    return system;
}