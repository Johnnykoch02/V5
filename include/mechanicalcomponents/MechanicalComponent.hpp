#ifndef MECHANICAL_COMPONENT
#define MECHANICAL_COMPONENT

#include "../TeriBull/TerriBull.hpp"

class TerriBull::MechanicalComponent  {
    private:
    TerriBull::string pType;
    TerriBull::Str2SizeMap pPorts;    
    public:
    MechanicalComponent();
    MechanicalComponent(const string type, const TerriBull::Str2SizeMap  ports) : pType(type), pPorts(ports) {}
    virtual float* dError();
    virtual TerriBull::string getType() const = 0;
};


#endif