#include "EventAction.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"

#include "G4AnalysisManager.hh"

#include "Randomize.hh"
#include<math.h>

EventAction::EventAction()
	: G4UserEventAction()
{
	eDep = new eDepStruct[6];
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction( const G4Event* evt)
{ 
	for(int i=0; i<6; i++)
	{
		eDep[i].TotalEDep = 0.;
		eDep[i].ProtonEDep = 0.;
		eDep[i].AlphaEDep = 0.;
		eDep[i].BeEDep = 0.;
		eDep[i].B11EDep = 0.;
		eDep[i].B10EDep = 0.;
		eDep[i].GammaEDep = 0.;
		eDep[i].OtherEDep = 0.;
	}

}

void EventAction::EndOfEventAction(const G4Event*)
{
	// get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	G4double sigma = 0.;
		
	for(G4int i=0; i<6; i++)
	{
		
		if(eDep[i].TotalEDep>0.)
		{
			sigma = 0.5/100;
			G4double resol = G4RandGauss::shoot(1, sigma);
			//
			analysisManager->FillH1(i, eDep[i].TotalEDep*resol);

			// Fill NTuple
			analysisManager->FillNtupleDColumn(i, 0, eDep[i].TotalEDep);
			analysisManager->FillNtupleDColumn(i, 1, eDep[i].ProtonEDep);
			analysisManager->FillNtupleDColumn(i, 2, eDep[i].AlphaEDep);
			analysisManager->FillNtupleDColumn(i, 3, eDep[i].BeEDep);
			analysisManager->FillNtupleDColumn(i, 4, eDep[i].B11EDep);
			analysisManager->FillNtupleDColumn(i, 5, eDep[i].B10EDep);
			analysisManager->FillNtupleDColumn(i, 6, eDep[i].GammaEDep);
			analysisManager->FillNtupleDColumn(i, 7, eDep[i].OtherEDep);
			analysisManager->AddNtupleRow(i);
		}
	}

}
