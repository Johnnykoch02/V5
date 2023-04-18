/**
 * @file magazine.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Magazine mechanism for Robots to count the number of contained disks.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
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
        }
        for (int i = 0; i < decSensorPorts.size(); i++) {
            decSensors[i] = new pros::ADIAnalogIn(decSensorPorts[i]);
        }
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
        if (inc != this->previousIncState) {
            if (inc) {
                this->pMagazineCnt++;
                this->toggledInc = true;
            }
        }
        this->previousIncState = inc;
    }

    void __dec__(bool dec) {
        if (dec != this->previousDecState) {
            if (dec) {
                this->pMagazineCnt--;
                this->toggledDec = true;
            }
        }
        this->previousDecState = dec;
        /* Clamp our Dec to 0*/
        if (this->pMagazineCnt < 0) this->pMagazineCnt = 0;
    }

    int update(float delta) {
        int inc = 1;
        for (int i = 0; i < this->numIncSensors; i++) {
            pros::lcd::set_text(i, to_string(incSensors[i]->get_value()));
            inc *= int(incSensors[i]->get_value() < incDefaults[i]); /* 1 - 0 */
        }
        this->__inc__(inc);
        int dec = 1;
        for (int i = 0; i < this->numDecSensors; i++) {
            pros::lcd::set_text(this->numIncSensors+i, to_string(decSensors[i]->get_value()));
            dec *= int(decSensors[i]->get_value() < decDefaults[i]); 
        }
        this->__dec__(dec);
        return 0;
    }

    int reset() {
        this->toggledInc = false;
        this->toggledDec = false;
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
