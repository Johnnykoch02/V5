/**
 * @file expansion.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Expandable component of the robot drive base.
 *
 * @version 0.1
 * @date 2023-03-01
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef EXPANSION_H
#define EXPANSION_H

#include "../../TerriBull/TerriBull.hpp"


class TerriBull::Expansion : public TerriBull::MechanicalComponent {
    public:
    Expansion() : TerriBull::MechanicalComponent() {}

    virtual int Expand() {}
};     

#endif // EXPANSION_H
