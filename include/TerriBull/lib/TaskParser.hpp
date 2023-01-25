/**
 * @file TaskParser.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief JSON configuration parser designed to be paired with configuration.json to 
 * extract a current configuration of the BullBot.
 *     
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

    #include<iostream>

    #ifdef __linux__
        #include "/usr/include/jsoncpp/json/json.h"
    #elif _WIN32
        #include "../../../jsoncpp/json/json.h"
    #else
        #include "../../../jsoncpp/json/json.h"
    #endif

    #include "../TerriBull.hpp"
    #include <string>
    #include<fstream>
    #include <map>
    #include <stream>
    #include <iostream>
    #include <cmath>

    using namespace std;
    #include <stdio.h>

    class TaskParser {
        private:
        string pFileLocation;
        /* Json Related Parsing Variables */
        ifstream * pFile;
        Json::Value pRoot;
        typedef enum {NO_ERROR, FILE_NOT_FOUND, FILE_OPEN_ERROR, VARIABLE_PARSE_ERROR} Error;
        uint8_t errCode = 0;

        public:
        TaskParser(string fileLocation) : pFileLocation(fileLocation) {
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

            if ( !this->pFile->is_open() ) {
                if (this->pFile->good() ) {
                    this->errCode = FILE_OPEN_ERROR;
                } else {
                    this->errCode = FILE_NOT_FOUND;
                }
                return;
            }

            /*
            Variable Parsing and Extraction
            */
            /*  */
            for (int i = 0; i < this->pRoot["tasks"].size(); ++i) {
                   /*
                        Grab Each Task type out and Parse for Init Variables
                   */
                  (for auto ref : this->pRoot["tasks"][i].getMemberNames()) { 
                    /* Tasks inside each Task Set */
                    if (ref == "drive") {
                        Json::Value drive = this->pRoot["tasks"][i]["drive"];
                        if (!drive.isNull()) {
                            
                        } else {
                            logger.logError("Error While Parsing Taskfile: Value is Null,\n Index" + ::std::to_string(i) + ": on Drive Parse.");
                        } 
                    }

                  }
            }
        }


map<string, float> mMacroList;

float findMacroValue(string macroName)
{
    if (mMacroList.find(macroName) != mMacroList.end())
    {
        return mMacroList[macroName];
    }
    else
    {
        return 0.0f;
    }
}

    Expression parseExpression(string expression)
    {
        // Creates a stringstream to extract data from the expression
        stringstream ss(expression);
        char op;
        float val1, val2, result;
        // Extracting the first value
        ss >> val1;
        // If the first value is a Macro, finding the Macro value
        if (val1 == '{{') {
            string macroName;
            // Read in the macro name
            getline(ss, macroName, '}');
            macroName.erase(0,2); // Removing the {{
            val1 = findMacroValue(macroName);
        }
        // Performing operation on the next value until the end of the expression
        while (ss >> op >> val2){
            // If the value is a Macro, finding the Macro value
            if (val2 == '{{') {
                string macroName;
                // Read in the macro name
                getline(ss, macroName, '}');
                macroName.erase(0,2); // Removing the {{
                val2 = findMacroValue(macroName);
            }
            switch (op) {
                case '+':
                    result = val1 + val2;
                    break;
                case '-':
                    result = val1 - val2;
                    break;
                case '*':
                    result = val1 * val2;
                    break;
                case '/':
                    result = val1 / val2;
                    break;
                case '^':
                    result = pow(val1, val2);
                    break;
            }
            val1 = result;
        }

        return result;
    }
    };


#endif