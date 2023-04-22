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
    class StateMachine {
    public:
        enum State {
            PEAK = 0,
            TROUGH = -1350,
            MID = -650,
        };

    private:
        State current_state;
        /* Transition Constraints */
        bool is_in_disk;


    public:
        StateMachine() : current_state(PEAK), is_in_disk(false) {}

        /* Returns true if a sequence has been completed. False Otherwise */
        bool update(float sensor_reading) {
            float index_diff[] = {float(int(PEAK)), float(int(TROUGH)), float(int(MID))};
            for (int i = 0; i < 3; i++) index_diff[i]= fabs(index_diff[i] - sensor_reading);
            int _currState = TerriBull::argmin(index_diff, 3);
            
            switch (current_state) {
                case PEAK:
                    if (_currState == 0) { return false; }
                    else if (_currState == 1) { this->current_state = TROUGH; is_in_disk = true; return false; }
                    else if (_currState == 2) { this->current_state = MID; is_in_disk = true; return false; }
                    break;
                case TROUGH:
                    if (_currState == 0) {
                         this->reset(); 
                         return true;
                        }
                    if (_currState == 1) { return false; }
                    if (_currState == 2) { this->current_state = MID; }
                    break;
                case MID:
                    if (_currState == 0) { return true; }
                    if (_currState == 1) { this->current_state = TROUGH; return false; }
                default:
                    break;
            }
            return false;
        }

        void reset() {
            current_state = PEAK;
            is_in_disk = false;
        }

        State getCurrentState() {
            return current_state;
        }
    };
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
    StateMachine incStateMachine;
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
        this->incStateMachine.reset();
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
                    this->pMagazineCnt++;
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
                    this->toggledDec = true;
                    this->pMagazineCnt--;
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
        // for (int i = 0; i < this->numIncSensors; i++) {
        //     // pros::lcd::set_text(i, to_string((incSensors[i]->get_value_calibrated() < incDefaults[i])));
        //     inc = inc & (incSensors[i]->get_value_calibrated() < incDefaults[i]); /* 1 - 0 */
        // }
        // this->__inc__(inc);
        if (this->incStateMachine.update(incSensors[0]->get_value_calibrated())) { pMagazineCnt++; }
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
