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
ConfigurationParser::~ConfigurationParser () {
    pFile->close();
    delete pFile;
}

TerriBull::Drive* ConfigurationParser::getDriveConfig() {
    if (this->pConfigVariables.DriveConfig.isNull() || this->pConfigVariables.DriveMotorPorts.isNull()) {
        this->errCode = VARIABLE_PARSE_ERROR;
        return nullptr;
    }
    Json::String DriveType = this->pConfigVariables.DriveConfig.asString();
    /* Drive Configurations that are Currently Supported */
    if (DriveType == "x_drive") {
        return new X_Drive(this->pConfigVariables.DriveMotorPorts[0].asInt(), this->pConfigVariables.DriveMotorPorts[1].asInt(), this->pConfigVariables.DriveMotorPorts[2].asInt(), this->pConfigVariables.DriveMotorPorts[3].asInt());
    }

    else return nullptr;
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
    
    /* Iterate through Mechanical System Config Member Fields */
    for (auto ref : this->pConfigVariables.Config["mechanical_system"].getMemberNames()) {
        /* This Section is to be Continued */
        if (ref == "rollers") {

        }
        else if (ref == "intake") {

        }
        else if (ref == "lift") {

        }

        /* TODO: add other system components here */

    }
    pros::lcd::set_text(4, "Created System : ");
    return system;
}