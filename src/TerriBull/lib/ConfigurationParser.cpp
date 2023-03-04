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
#include "../../../include/MechanicalComponents/Drive/configurations/x_drive.hpp"

/* Intake */
#include "../../../include/MechanicalComponents/Intakes/Configurations/Intake_Uni.hpp"
#include "../../../include/MechanicalComponents/Intakes/Configurations/Intake_Duo.hpp"
/* Ctrls*/
#include "../../../include/Controllers/InputController/Configurations/AidanJoeShmo.hpp"
/*END INCLUDE*/

ConfigurationParser::~ConfigurationParser () {
    pFile->close();
    delete pFile;
}

TerriBull::Drive* ConfigurationParser::getDriveConfig() {
    /*return new X_Drive(1, 2, 3, 4);*/if (this->pConfigVariables.DriveConfig.isNull() || this->pConfigVariables.DriveMotorPorts.isNull()) {
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    Json::String DriveType = this->pConfigVariables.DriveConfig.asString();
    /* Drive Configurations that are Currently Supported */
    if (DriveType == "x_drive") {
        return new X_Drive(this->pConfigVariables.DriveMotorPorts[0].asInt(), this->pConfigVariables.DriveMotorPorts[1].asInt(), this->pConfigVariables.DriveMotorPorts[2].asInt(), this->pConfigVariables.DriveMotorPorts[3].asInt());
    }
    if (DriveType == "tank_drive_std") {

        return new Tank_Drive_Std(this->pConfigVariables.DriveMotorPorts[0].asInt(), this->pConfigVariables.DriveMotorReverse[0].asBool(), this->pConfigVariables.DriveMotorPorts[1].asInt(), this->pConfigVariables.DriveMotorReverse[1].asBool(), this->pConfigVariables.DriveMotorPorts[2].asInt(), this->pConfigVariables.DriveMotorReverse[2].asBool(), this->pConfigVariables.DriveMotorPorts[3].asInt(), this->pConfigVariables.DriveMotorReverse[3].asBool(), this->pConfigVariables.DriveMotorPorts[4].asInt(), this->pConfigVariables.DriveMotorReverse[4].asBool(), this->pConfigVariables.DriveMotorPorts[5].asInt(), this->pConfigVariables.DriveMotorReverse[5].asBool());
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

    return nullptr;
}

TerriBull::Intake* ConfigurationParser::getIntakeConfig() {
    /*return nullptr;*/Json::Value IntakeConfig = this->pConfigVariables.Config["mechanical_system"]["intake"];
    Json::String ConfigType = IntakeConfig["config"].asString();
    if (ConfigType == "") {
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    /* Controller Configurations that are Currently Supported */
    if (ConfigType == "IntakeUni") {
        return new Intake_Uni(IntakeConfig["motor_ports"][0].asInt(), IntakeConfig["max_speed"].asInt());
    }
    if (ConfigType == "IntakeDuo") {
        return new Intake_Duo(IntakeConfig["motor_ports"][0].asInt(),IntakeConfig["motor_ports"][1].asInt(), IntakeConfig["max_speed"].asInt());
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
        pros::lcd::set_text(3, "Null Drivebase : " + this->pConfigVariables.DriveConfig.asString());
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    if (this->pConfigVariables.StartingAngle.isNull() || this->pConfigVariables.StartingPos.isNull()) {
        pros::lcd::set_text(3, "Null Start Info : " + this->pConfigVariables.DriveConfig.asString());
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }



    /* Construct a new base System */
    TerriBull::MechanicalSystem* system = new TerriBull::MechanicalSystem(this->pConfigVariables.IMUConfig.asInt(), drive);
    system->setStartingAngle(this->pConfigVariables.StartingAngle.asFloat());
    system->setStartingPosition(this->pConfigVariables.StartingPos["x"].asFloat(), this->pConfigVariables.StartingPos["y"].asFloat());
    system->Init();
    return system;

    /* Iterate through Mechanical System Config Member Fields */
    for (auto ref : this->pConfigVariables.Config["mechanical_system"].getMemberNames()) {
        /* This Section is to be Continued */
        if (ref == "rollers") {

        }
        else if (ref == "intake") {
            TerriBull::Intake* intake = this->getIntakeConfig();
            if (intake != nullptr) {
                system->setIntake(intake);
            }
        }

        else if (ref == "lift") {

        }

        /* TODO: add other system components here */

    }
    pros::lcd::set_text(4, "Created System : ");
    return system;
}