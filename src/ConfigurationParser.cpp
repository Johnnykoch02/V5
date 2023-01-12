
#include<iostream>
#include "../jsoncpp/include/json/json.h"
#include<fstream>

using namespace std;

int main() {
	/* Read the json file using ifstream */
	ifstream jsonfile("jsonfile.json");

	/* The root node of the json file*/
	Json::Value root;

	/* Read the json data from the file into the root node */
	jsonfile >> root;

	/* Get the value of the "name" property */
	string name = root.get("name", "").asString();

	/* Get the value of the "age" property */
	int age = root.get("age", 0).asInt();

	/* Print the parsed values */
	cout << "Name: " << name << endl;
	cout << "Age: " << age << endl;

	return 0;
}

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
                    this->pRoot.get("configurations", "ERROR: Variable not found.").getMemberNames();
            }
            ~ConfigurationParser();

        bool success() const {
            return this->errCode == NO_ERROR;
        }
    };

#endif