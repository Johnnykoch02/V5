/**
 * @file roller.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Spinnable device used for manipulating field items in a spin system.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef ROLLER_H
#define ROLLER_H

#include "../../TerriBull/TerriBull.hpp"


class TerriBull::Roller : public TerriBull::MechanicalComponent {
    public:
    Roller() : TerriBull::MechanicalComponent() {}

    virtual int Expand() {}
};     

#endif // ROLLER_H
