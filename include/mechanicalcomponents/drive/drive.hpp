#include "../../TeriBull/TerriBull.hpp"

class TerriBull::Drive {
    protected:
    float pCurrXVoltage, pCurrYVoltage;

    public:
    virtual void drive(float x, float y) = 0;
    virtual void resultant_vector() = 0;
    virtual void tare_encoders() = 0;
}