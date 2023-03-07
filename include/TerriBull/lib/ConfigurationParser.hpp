/**
 * @file ConfigurationParser.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief JSON configuration parser designed to be paired with configuration.json to 
 * extract a current configuration of the BullBot.
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include<iostream>
#include "../../../src/dist/json/json.h"
#include "../TerriBull.hpp"
#include <string>
#include <fstream>
#include <stdio.h>
class ConfigurationParser {
    private:
    ::std::string pFileLocation;
    /* Json Related Parsing Variables */
    ::std::ifstream * pFile;

    Json::Value pRoot;
    typedef enum {NO_ERROR, FILE_NOT_FOUND, FILE_OPEN_ERROR, VARIABLE_PARSE_ERROR} Error;
    uint8_t errCode = 0;
    /* MECHANICAL SYSTEM VARIABLES */
  
    typedef struct {
        Json::Value Config; /* User Selected Config */
        /*
        Drive Variables
        */
        Json::Value DriveConfig;
        Json::Value DriveMotorPorts;
        Json::Value DriveMotorReverse;
        Json::Value DriveMotorGearset;
        Json::Value DriveConversionFactor;
        Json::Value DriveWheelRadius;
        /* IMU Sensor */
        Json::Value IMUConfig;
        Json::Value StartingAngle;
        Json::Value StartingPos;
        /* CONTROLLER CONFIG VARIABLES */
        Json::Value ControllerConfig;
        Json::Value ControllerDeadzone;
    } ConfigVariables;
    ConfigVariables pConfigVariables;
    public:
        uint8_t getErrCode() {
            return this->errCode;
        }
        ConfigurationParser(::std::string fileLocation, ::std::string configuration) : pFileLocation(fileLocation) {
            /* Preceed to load in Data from File*/
            this->errCode = NO_ERROR;
            this->pFile = new ::std::ifstream(pFileLocation.c_str());
            if ( !this->pFile->is_open() ) {
                if (this->pFile->good() ) {
                    this->errCode = FILE_OPEN_ERROR;
                } else {
                    this->errCode = FILE_NOT_FOUND;
                }
                return;
            }
            /* Convert to JSON Value */
            *(this->pFile) >> this->pRoot;
            /*
            Variable Parsing and Extraction
            */
            for (int i = 0; i < this->pRoot["configurations"].size(); ++i) {
                if (this->pRoot["configurations"][i]["name"].asString() == configuration) {
                    this->pConfigVariables.Config = this->pRoot["configurations"][i];
                }
            }
            pros::lcd::set_text(0, "Parsed Config: " + this->pConfigVariables.Config["name"].asString());
            if (this->pConfigVariables.Config["name"].asString() == "") {
                this->errCode = VARIABLE_PARSE_ERROR;
                return;
            }
            /* Load In Configuration Variables */
            this->pConfigVariables.ControllerConfig = this->pConfigVariables.Config["controller_config"];
            this->pConfigVariables.ControllerDeadzone = this->pConfigVariables.Config["controller_deadzone"];
            this->pConfigVariables.DriveConfig = this->pConfigVariables.Config["mechanical_system"]["drive"]["config"];
            this->pConfigVariables.DriveMotorPorts = this->pConfigVariables.Config["mechanical_system"]["drive"]["motor_ports"];
            this->pConfigVariables.DriveMotorReverse = this->pConfigVariables.Config["mechanical_system"]["drive"]["reverse_motors"];
            this->pConfigVariables.DriveMotorGearset = this->pConfigVariables.Config["mechanical_system"]["drive"]["gear_ratio"];
            this->pConfigVariables.DriveConversionFactor = this->pConfigVariables.Config["mechanical_system"]["drive"]["cf"];
            this->pConfigVariables.DriveWheelRadius = this->pConfigVariables.Config["mechanical_system"]["drive"]["radius"];

            this->pConfigVariables.IMUConfig = this->pConfigVariables.Config["mechanical_system"]["imu"];
            this->pConfigVariables.StartingAngle = this->pConfigVariables.Config["mechanical_system"]["starting_angle"];
            this->pConfigVariables.StartingPos = this->pConfigVariables.Config["mechanical_system"]["starting_position"];
            pros::lcd::set_text(1, "Parsed DriveType: " + this->pConfigVariables.DriveConfig.asString());
            // pros::lcd::set_text(3, "Parsed Start Pos: " + std::to_string(this->pConfigVariables.StartingPos["x"].asFloat()) + " " + std::to_string(this->pConfigVariables.StartingPos["y"].asFloat()));
            // pros::lcd::set_text(4, "Parsed Start Angle: " + std::to_string(this->pConfigVariables.StartingAngle.asFloat()));
        }
        ~ConfigurationParser();
        /*
        Functions to return Robot Configuration
        */
        TerriBull::Drive* getDriveConfig();
        TerriBull::Intake* getIntakeConfig();
        TerriBull::Roller* getRollerConfig();
        TerriBull::Shooter* getShooterConfig();
        TerriBull::MechanicalSystem* getMechanicalSystemConfig();
        TerriBull::InputController* getInputControllerConfig(TerriBull::RoboController* roboController);

    bool success() const {
        return this->errCode == NO_ERROR;
    }
};


#endif

