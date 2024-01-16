#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"
#include "G4UIcmdWithADouble.hh"

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhysicsList)
    : G4UImessenger(), fPhysicsList(pPhysicsList)
{   
    PhysDIR = new G4UIdirectory("/HUS/Physics/");
    PhysDIR->SetGuidance("............................");

    fElasticScaleFactorCmd = new G4UIcmdWithADouble("/HUS/Physics/SetElasticScaleFactor",this);
    fElasticScaleFactorCmd->SetGuidance("Set cross section scale factor for elastic process of protons.");
    fElasticScaleFactorCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    fInelasticScaleFactorCmd = new G4UIcmdWithADouble("/HUS/Physics/SetInelasticScaleFactor",this);
    fInelasticScaleFactorCmd->SetGuidance("Set cross section scale factor for inelastic process of protons.");
    fInelasticScaleFactorCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PhysicsListMessenger::~PhysicsListMessenger()
{
    delete PhysDIR;
    delete fElasticScaleFactorCmd;
    delete fInelasticScaleFactorCmd;
}

void PhysicsListMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if(command == fElasticScaleFactorCmd)
    {
        fPhysicsList->SetElasticScaleFactor(fElasticScaleFactorCmd->GetNewDoubleValue(newValue));
    }

    if (command == fInelasticScaleFactorCmd)
    {
        fPhysicsList->SetInelasticScaleFactor(fInelasticScaleFactorCmd->GetNewDoubleValue(newValue));
    }
}