/**
 * @file VoltageRegulator.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Abstract Data structure for implementing game objects on the field
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef VOLTAGEREGULATOR_H
#define VOLTAGEREGULATOR_H

#include "../../TerriBull.hpp"

#include <string>


class TerriBull::VoltageRegulator {
    private:
    float* pCurrentVoltages;
    float* pPreviousVoltages;
    float* pGradientVoltages;
    uint16_t pLength;
    float pMaxGradient;

    bool allZeros(float* ptr, uint16_t length) {
        for (uint16_t i = 0; i < length; i++) {
            if (ptr[i]!= 0) {
                return false;
            }
        }
        return true;
    }
    public:   
    VoltageRegulator(uint16_t _lengthVoltages, float _maxGradient) : pLength(_lengthVoltages), pMaxGradient(_maxGradient) {
        this->pCurrentVoltages = (float*)malloc(sizeof(float) * _lengthVoltages);
        this->pPreviousVoltages = (float*)malloc(sizeof(float) * _lengthVoltages);
        this->pGradientVoltages = (float*)malloc(sizeof(float) * _lengthVoltages);
        
    }
    
    ~VoltageRegulator() {

    }
    
    float* getRegulatedVoltages(float* targetVoltages) {
        /* Check to see if it is a Hard Reset */
        if(allZeros(targetVoltages, pLength) ) {
                this->ResetGradients();
                return targetVoltages;
            }
        /* Copy our Old voltages into our previous container */
        for (uint16_t i = 0; i < this->pLength; i++) this->pPreviousVoltages[i] = this->pCurrentVoltages[i];
        /* Go through Each index and clamp the Gradient to the Max Gradient */
        for (uint16_t i = 0; i < this->pLength; i++) {
            /* Handle Case where current voltages are zero */
            if (this->pPreviousVoltages[i] == 0) {
                this->pCurrentVoltages[i] = (this->pMaxGradient * this->pMaxGradient ) * targetVoltages[i];
            }
            else {
                /* Compute the Current index Gradient*/
                this->pGradientVoltages[i] = (targetVoltages[i] - this->pPreviousVoltages[i]);
                int dir = this->pGradientVoltages[i]/ fabs(this->pGradientVoltages[i]);
                /* Gradient at each index should be the clamped version of the Graident w.r.t the current value */
                this->pGradientVoltages[i] = dir * min(fabs(this->pMaxGradient*this->pPreviousVoltages[i]), fabs(this->pGradientVoltages[i]));
                /* Then we add that Gradient to our Current Voltages */
                this->pCurrentVoltages[i] += this->pGradientVoltages[i];
            }
        }
        return this->pCurrentVoltages;
    }

    void ResetGradients() {
        for (uint16_t i = 0; i < this->pLength; i++) { /* Initialize to Zero*/
            this->pCurrentVoltages[i] = 0;
            this->pPreviousVoltages[i] = 0;
            this->pGradientVoltages[i] = 0;
        }
    }



};

#endif