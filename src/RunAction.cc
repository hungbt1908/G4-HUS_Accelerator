#include "RunAction.hh"
#include "RunActionMessenger.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

double RunAction::diffclock(clock_t clock1, clock_t clock2)
{
	double diffticks = clock1-clock2;
	double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
	return diffms;
}

RunAction::RunAction()
{
  fRunActionMessenger = new RunActionMessenger(this);

  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Create analysis manager
  // The choice of the output format is done via the specified
  // file extension.
  auto analysisManager = G4AnalysisManager::Instance();

  // Create directories
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  //analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);
    // Note: merging ntuples is available only with Root output

  // Book histograms, ntuple
  //
  analysisManager->CreateH1("EDep","Edep in Det0", 200, 0., 2.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det1", 200, 0., 2.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det2", 200, 0., 2.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det3", 200, 0., 2.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det4", 200, 0., 2.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det5", 200, 0., 2.0*MeV);

  // Creating ntuple
  for (G4int NofDet=0; NofDet<6; NofDet++)
  {
    G4String name = "DetResults" + std::to_string(NofDet);
    analysisManager->CreateNtuple(name.c_str(),"Energy deposition in detector");
    analysisManager->CreateNtupleDColumn(NofDet, "Esum");
    analysisManager->CreateNtupleDColumn(NofDet, "Eproton");
    analysisManager->CreateNtupleDColumn(NofDet, "Ealpha");
    analysisManager->CreateNtupleDColumn(NofDet, "EBe");
    analysisManager->CreateNtupleDColumn(NofDet, "Egamma");
    analysisManager->CreateNtupleDColumn(NofDet, "EB11");
    analysisManager->CreateNtupleDColumn(NofDet, "EB10");
    analysisManager->CreateNtupleDColumn(NofDet, "Eother");
    analysisManager->FinishNtuple(NofDet);
  }
}

RunAction::~RunAction()
{
  delete fRunActionMessenger;
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
  begin = clock();

  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  if(fFileName.size()) // or whatever check is necessesary :)
    analysisManager->OpenFile(fFileName);
  else
    analysisManager->OpenFile("defaultFilename.root");

  // G4cout << "Using " << analysisManager->GetType() << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  end = clock();

  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();

  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();

  int numberOfWorkerThreads = G4Threading::GetNumberOfRunningWorkerThreads();

    if (IsMaster()) 
    {
        G4cout << "ELAPSED TIME IN SECONDS: "
               << (double(diffclock(end, begin)/1000))/(double)numberOfWorkerThreads 
               << " seconds" << G4endl;
    }
}

void RunAction::SetFilename(G4String fileName) 
{
  fFileName = G4String(fileName);
}