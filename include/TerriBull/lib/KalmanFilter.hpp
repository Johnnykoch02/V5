/**
 * @file KalmanFilter.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief 1 Dimensional Kalman Filter class
 *     
 * @version 0.1
 * @date 2023-04-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>
#include <vector>

class KalmanFilter1D {
public:
    KalmanFilter1D(float dt, float process_noise, float measurement_noise) {
        this->dt = dt;
        this->process_noise = process_noise;
        this->measurement_noise = measurement_noise;

        x.resize(2, 0.0);
        P.resize(2, std::vector<float>(2, 0.0));
        P[0][0] = 1.0;
        P[1][1] = 1.0;
    }

    void predict() {
        x[0] += x[1] * dt;
        P[0][0] += dt * (P[1][0] + dt * P[1][1]);
        P[0][1] += dt * P[1][1];
        P[1][0] += dt * P[1][1];
        P[1][1] += process_noise * dt * dt;
    }

    void update(float measured_angle, float dt) {
        this->dt = dt;
        float y = measured_angle - x[0];
        float S = P[0][0] + measurement_noise;
        float K0 = P[0][0] / S;
        float K1 = P[1][0] / S;
        x[0] += K0 * y;
        x[1] += K1 * y;
        P[0][0] -= K0 * P[0][0];
        P[0][1] -= K0 * P[0][1];
        P[1][0] -= K1 * P[0][0];
        P[1][1] -= K1 * P[0][1];
    }

    std::vector<float> getState() {
        return x;
    }

private:
    float dt, process_noise, measurement_noise;
    std::vector<float> x;
    std::vector<std::vector<float>> P;
};