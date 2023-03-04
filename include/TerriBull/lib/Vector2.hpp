/**
 * @file Vector2.hpp
 * @author John Koch jkoch21@usf.edu
 * @brief Basic 2D Vector Class designed to give easy access to many of the 
 * necessary calculations on Vector2 Instance creation.
 *     
 * @version 0.1
 * @date 2023-01-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef VECTOR2_H 
#define VECTOR2_H

#include "TerriBull/TerriBull.hpp"

class TerriBull::Vector2 {
    private:

    public:
        float x;
        float y;
        float r;
        float theta;

        Vector2() {}
        ~Vector2() {}
        Vector2(const TerriBull::Vector2& v) : x(v.x), y(v.y), r(v.r), theta(v.theta) {}
        Vector2(const TerriBull::Vector2*  v) : x(v->x), y(v->y), r(v->r), theta(v->theta) {}
        ::TerriBull::Vector2* operator *(const float scale) {
            Vector2* v = new Vector2();
            v->x = scale * this->x;
            v->y = scale * this->y;
            v->r = ::std::sqrt(v->x*v->x+v->y*v->y);
            v->theta = this->theta;
            return v;
        }

        float operator *(Vector2 const & that) {
            return this->x * that.x + this->y * that.y + this->y;
        } 

        TerriBull::Vector2 operator +(const TerriBull::Vector2& that) {
           float x = this->x + that.x;
           float y = this->y + that.y;
           return TerriBull::Vector2::cartesianToVector2( x, y);
        }
        
        TerriBull::Vector2 operator -(const TerriBull::Vector2& that) {
           float x = this->x - that.x;
           float y = this->y - that.y;
           return TerriBull::Vector2::cartesianToVector2( x, y);
        }

        bool operator ==(TerriBull::Vector2 const & that) const {
            return this->x == that.x && this->y == that.y;
        }
        bool operator!=(TerriBull::Vector2 const & that) const {
            return this->x!= that.x || this->y!= that.y;
        }

        static TerriBull::Vector2* cartesianToVector2(float x, float y) {
            Vector2 *v = new Vector2();
            v->x = x;
            v->y = y;
            v->r = ::std::sqrt(x*x+y*y);
            v->theta = ::std::fmod(RAD2DEG(atan2(y, x)), 360.0);
            return v;
        }
        static TerriBull::Vector2* polarToVector2(float r, float theta) {
            Vector2* v = new Vector2();
            v->x = r*cos(theta);
            v->y = r*sin(theta);
            v->r = r;
            v->theta = theta;
            return v;
        }
        TerriBull::Vector2* unit() {
            Vector2* v = new Vector2();
            v->theta = this->theta;
            v->r = 1;
            v->x = ::std::cos(theta);
            v->y = ::std::sin(theta);
            return v;
        }
        bool sameDirection(Vector2 const & other) {
            return this->theta == other.theta;
        }
};

#endif