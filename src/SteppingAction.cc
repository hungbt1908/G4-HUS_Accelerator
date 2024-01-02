#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction(const DetectorConstruction* detConstruction,
                               EventAction* eventAction)
  : fDetConstruction(detConstruction),
    fEventAction(eventAction)
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Check sensitive detector
  if(step->GetPreStepPoint()->GetPhysicalVolume()->GetName()=="physDetector")
  {
    auto touchable = (step->GetPreStepPoint()->GetTouchable());
    G4int detID = touchable->GetReplicaNumber(0);

    // Get Edep
	  G4double edep = step->GetTotalEnergyDeposit();

    // Get Track
    G4Track* track = dynamic_cast<G4Track*>(step->GetTrack());
    G4ParticleDefinition* particleDef = track->GetDefinition();
    G4String projname = particleDef->GetParticleName();

    if(edep > 0.)
    {
      // Store Total edep
      fEventAction->AddEdepTotal(edep, detID);

      // Store edep by particle name
      if(projname == "proton")
        fEventAction->AddEdepProton(edep, detID);
      else if(projname == "alpha") 
        fEventAction->AddEdepAlpha(edep, detID);
      else if(projname == "Be7")
        fEventAction->AddEdepBe(edep, detID);
      else if(projname == "B11")
        fEventAction->AddEdepB11(edep, detID);
      else if(projname == "B10")
        fEventAction->AddEdepB10(edep, detID);
      else if(projname == "gamma")
        fEventAction->AddEdepGamma(edep, detID);
      else
        fEventAction->AddEdepOther(edep, detID);
    }
  }
}
