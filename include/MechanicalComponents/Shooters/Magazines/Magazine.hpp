/**
 * @file magazine.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Magazine mechanism for Robots to count the number of contained disks.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 * CHANGELOG:
 * - Converted Digital to Analog Logic 04-18-2023 
 * 
*/
#ifndef MAGAZINE_H
#define MAGAZINE_H

#include "../../TerriBull/TerriBull.hpp"


class TerriBull::Magazine : public TerriBull::MechanicalComponent {
    protected:
    bool toggledInc;
    bool toggledDec;
    pros::ADIAnalogIn **incSensors;
    uint8_t numIncSensors, numDecSensors;
    int8_t pMagazineCnt; /* Maintain Number of Disks in the Magazine */
    pros::ADIAnalogIn **decSensors;
    int8_t incCntr, decCntr;
    bool previousIncState, previousDecState;
    float* incDefaults,* decDefaults;
    public: /* TODO:
     - some of our sensors might be in state high as default as opposed to low. 
     - if a state is high as default, then we need to multiply that sensor readings like this:
        1. add to config file a way to modify the magazines default state.
    
     */
    Magazine(std::string incSensorPorts, float* inc_defaults, std::string decSensorPorts, float* dec_defaults) : TerriBull::MechanicalComponent(0), numIncSensors(incSensorPorts.size()), numDecSensors(decSensorPorts.size()), incDefaults(inc_defaults), decDefaults(dec_defaults) {
        incSensors = new pros::ADIAnalogIn*[incSensorPorts.size()];
        decSensors = new pros::ADIAnalogIn*[decSensorPorts.size()];
        for (int i = 0; i < incSensorPorts.size(); i++) {
            incSensors[i] = new pros::ADIAnalogIn(incSensorPorts[i]);
            incSensors[i]->calibrate();
        }
        for (int i = 0; i < decSensorPorts.size(); i++) {
            decSensors[i] = new pros::ADIAnalogIn(decSensorPorts[i]);
            decSensors[i]->calibrate();
        }
        pMagazineCnt = 0;
        previousDecState = false;        
        incCntr = 0;
        decCntr = 0;
    }
    ~Magazine() {
        /* Delete the Sensors and Data Pointers */
        for (int i = 0; i < numIncSensors; i++) {
            delete incSensors[i];
        }
        for (int i = 0; i < numDecSensors; i++) {
            delete decSensors[i];
        }
        delete[] incSensors;
        delete[] decSensors;
    }

    void __inc__(bool inc) {
        if (! this->toggledInc) {
            if (inc) {
                this->incCntr++;
                if (this->incCntr > 6) {
                    this->toggledInc = true;
                }
            }
            else {
                this->incCntr = 0;
            }
        } 
    }

    void __dec__(bool dec) {
        if (! this->toggledDec) {
            if (dec) {
                this->decCntr++;
                if (this->decCntr > 6) {
                    this->toggleddec = true;
                }
            }
            else {
                this->decCntr = 0;
            }
        }
        /* Clamp our Dec to 0*/
        if (this->pMagazineCnt < 0) this->pMagazineCnt = 0;
    }

    int update(float delta) {
        bool inc = true;
        for (int i = 0; i < this->numIncSensors; i++) {
            // pros::lcd::set_text(i, to_string((incSensors[i]->get_value_calibrated() < incDefaults[i])));
            inc = inc & (incSensors[i]->get_value_calibrated() < incDefaults[i]); /* 1 - 0 */
        }
        this->__inc__(inc);
        int dec = true;
        for (int i = 0; i < this->numDecSensors; i++) {
            // pros::lcd::set_text(this->numIncSensors+i, to_string((incSensors[i]->get_value_calibrated() < incDefaults[i])));
            dec = dec & (decSensors[i]->get_value_calibrated() < decDefaults[i]); 
        }
        this->__dec__(dec);
        return 0;
    }

    int reset() {
        this->toggledInc = false;
        this->toggledDec = false;
        this->incCntr = 0;
        this->decCntr = 0;
        return 0;
    }

    int8_t getMagazineCount() {
        return this->pMagazineCnt;
    }
    bool getIncToggle() {
        return this->toggledInc;
    }
    bool getDecToggle() {
        return this->toggledDec;
    }

};     

#endif // SHOOTER_H
