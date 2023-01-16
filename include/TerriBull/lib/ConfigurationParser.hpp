
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

    #include<iostream>

    // #ifdef __linux__
    #include "/usr/include/jsoncpp/json/json.h"
    // #elif _WIN32
    #include "../jsoncpp/include/json/json.h"
    #endif

    #include "../TerriBull.hpp"

    #include <string>
    #include<fstream>

    using namespace std;
    #include ""
    #include <stdio.h>
    class ConfigurationParser {
        private:
        string pFileLocation;
        /* Json Related Parsing Variables */
        ifstream * pFile;
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

            /* IMU Sensor*/
            Json::Value IMUConfig;

            /* CONTROLLER CONFIG VARIABLES */


        } ConfigVariables;

        ConfigVariables pConfigVariables;



        public:
            ConfigurationParser(string fileLocation, string configuration) : pFileLocation(fileLocation) {
                /* Preceed to load in Data from File*/
                this->pFile = new ifstream(pFileLocation.c_str());
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
                if (this->pConfigVariables.Config.asString() == "") {
                    this->errCode = VARIABLE_PARSE_ERROR;
                    return;
                }



                /* Load In Configuration Variables */
                this->pConfigVariables.DriveConfig = this->pConfigVariables.Config["mechanical_system"]["drive"]["config"];
                this->pConfigVariables.DriveMotorPorts = this->pConfigVariables.Config["mechanical_system"]["drive"]["motor_ports"];

                this->pConfigVariables.IMUConfig = this->pConfigVariables.Config["imu"];

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
                if (this->pDriveConfig.isNull() || this->pDriveMotorPorts.isNull()) {
                    this->errCode = VARIABLE_PARSE_ERROR;
                    return nullptr;
                }
                Json::String DriveType = this->pDriveConfig.asString();
                /* Drive Configurations that are Currently Supported */
                if (DriveType == "x_drive") {
                    return new TerriBull::X_Drive(this->pDriveMotorPorts[0].asInt(), this->pDriveMotorPorts[1].asInt(), this->pDriveMotorPorts[2].asInt(),this->pDriveMotorPorts[3].asInt());
                }

                else return nullptr;
    }

    TerriBull::MechanicalSystem* ConfigurationParser::getMechanicalSystemConfig() {
        if (this->pConfigVariables.Config.isNull() || this->pConfigVariables.IMUConfig.isNull()) {
            this->errCode = VARIABLE_PARSE_ERROR;
            return nullptr;
        }
        TerriBull::Drive* drive = this->getDriveConfig();
        if (drive == nullptr) {
            this->errCode = VARIABLE_PARSE_ERROR;
            return nullptr;
        }
        /* Construct a new base System */
        TerriBull::MechanicalSystem* system = new Terribull::MechanicalSystem(this->pConfigVariables.IMUConfig.asInt(), drive);
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

        return system;
    }




#endif
