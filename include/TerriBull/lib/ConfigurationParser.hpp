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

    // #ifdef _WIN32
    // #include "../../../jsoncpp/include/json/json.h"
    // #elif _WIN64
    // #include "../../../jsoncpp/include/json/json.h"
    // #else
    #include "../../../src/dist/json/json.h"
    // #endif

    #include "../TerriBull.hpp"

    /* Include All Items Needed for Development of System */
    #include "../../MechanicalComponents/Drive/configurations/x_drive.hpp"

    /*END INCLUDE*/

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

            /* IMU Sensor */
            Json::Value IMUConfig;
            Json::Value StartingAngle;
            Json::Value StartingPos;

            /* CONTROLLER CONFIG VARIABLES */


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
                pros::lcd::set_text(0,"Open Parse File Code: " + ::std::to_string(this->errCode));
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
                pros::lcd::set_text(1, "Parsed Config: " + this->pConfigVariables.Config["name"].asString());
                if (this->pConfigVariables.Config["name"].asString() == "") {
                    this->errCode = VARIABLE_PARSE_ERROR;
                    return;
                }



                /* Load In Configuration Variables */
                this->pConfigVariables.DriveConfig = this->pConfigVariables.Config["mechanical_system"]["drive"]["config"];
                this->pConfigVariables.DriveMotorPorts = this->pConfigVariables.Config["mechanical_system"]["drive"]["motor_ports"];
                this->pConfigVariables.IMUConfig = this->pConfigVariables.Config["mechanical_system"]["imu"];
                this->pConfigVariables.StartingAngle = this->pConfigVariables.Config["mechanical_system"]["starting_angle"];
                this->pConfigVariables.StartingPos = this->pConfigVariables.Config["mechanical_system"]["starting_position"];

                pros::lcd::set_text(2, "Parsed DriveType: " + this->pConfigVariables.DriveConfig.asString());
                // pros::lcd::set_text(3, "Parsed Start Pos: " + std::to_string(this->pConfigVariables.StartingPos["x"].asFloat()) + " " + std::to_string(this->pConfigVariables.StartingPos["y"].asFloat()));
                // pros::lcd::set_text(4, "Parsed Start Angle: " + std::to_string(this->pConfigVariables.StartingAngle.asFloat()));
            }
            ~ConfigurationParser();

            /*
            Functions to return Robot Configuration
            */
            TerriBull::Drive* getDriveConfig();
            TerriBull::MechanicalSystem* getMechanicalSystemConfig();

        bool success() const {
            return this->errCode == NO_ERROR;
        }
    };

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

#endif

