#include "ActionInitialization.hh"
#include "PrimaryGenerationAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction)
 : fDetConstruction(detConstruction)
{}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGenerationAction);
    SetUserAction(new RunAction);
    auto eventAction = new EventAction;
    SetUserAction(eventAction);
    SetUserAction(new SteppingAction(fDetConstruction, eventAction));
}