#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4VPhysicsConstructor;

class PhysicsList: public G4VUserPhysicsList
{
public:

  PhysicsList();
  virtual ~PhysicsList();

  virtual void ConstructParticle();
  virtual void ConstructProcess();
};
#endif

