
#include<iostream>
// #include <jsoncpp/json/json.h>
#include<fstream>

using namespace std;
#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

    #include <stdio.h>
    class ConfigurationParser {
        private:
        string pFileLocation;
        ifstream * pFile;
        Json::Value pRoot;
        typedef enum {NO_ERROR, FILE_NOT_FOUND, FILE_OPEN_ERROR, VARIABLE_PARSE_ERROR} Error;
        uint8_t errCode = 0;
        public:
            ConfigurationParser(string fileLocation, string configuration) : pFileLocation(fileLocation) {
                this->pFile = new ifstream(pFileLocation.c_str());
                if ( !this->pFile->is_open() ) {
                    if (this->pFile->good() ) {
                        this->errCode = FILE_OPEN_ERROR;
                    } else {
                        this->errCode = FILE_NOT_FOUND;
                    }
                    return;
                }

                	*(this->pFile) >> this->pRoot;      
                    auto vals = this->pRoot.get("configurations", "ERROR: Variable not found.").getMemberNames();
                    for (auto val : vals) 
                    {
                        cout << val << endl;
                    }

            }
            ~ConfigurationParser();

        bool success() const {
            return this->errCode == NO_ERROR;
        }
    };
    ConfigurationParser::~ConfigurationParser () {
        pFile->close();
        delete pFile;
    }

#endif

int main() {
	/* Read the json file using ifstream */
	ConfigurationParser parser("../configuration.json", "Shooter_Big_Bot");

	return 0;
}

