#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4VPhysicsConstructor;
class PhysicsListMessenger;

class PhysicsList: public G4VUserPhysicsList
{
public:
    PhysicsList();
    virtual ~PhysicsList();
    virtual void ConstructParticle();
    virtual void ConstructProcess();

private:
    PhysicsListMessenger* fPhysicsListMessenger;
    G4double elasticScaleFactor;
    G4double inelasticScaleFactor;

public:
    inline void SetElasticScaleFactor(G4double val) {elasticScaleFactor = val;}
    inline void SetInelasticScaleFactor(G4double val) {inelasticScaleFactor = val;}
};
#endif

