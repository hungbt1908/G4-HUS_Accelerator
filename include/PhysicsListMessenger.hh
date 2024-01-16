#ifndef PhysicsListMessenger_h
#define PhysicsListMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class PhysicsList;
class G4UIdirectory;
class G4UIcmdWithADouble;

class PhysicsListMessenger: public G4UImessenger
{
public:
    PhysicsListMessenger(PhysicsList* p = 0);
    virtual ~PhysicsListMessenger();
    virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
    G4UIdirectory* PhysDIR;
    PhysicsList* fPhysicsList;
    G4UIcmdWithADouble* fElasticScaleFactorCmd;
    G4UIcmdWithADouble* fInelasticScaleFactorCmd;  
};
#endif

