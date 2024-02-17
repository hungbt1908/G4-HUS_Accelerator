#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
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
  analysisManager->CreateH1("EDep","Edep in Det0", 8000, 0., 8.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det1", 8000, 0., 8.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det2", 8000, 0., 8.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det3", 8000, 0., 8.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det4", 8000, 0., 8.0*MeV);
  analysisManager->CreateH1("EDep","Edep in Det5", 8000, 0., 8.0*MeV);

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
    // get time
    time_t beginnow = time(0);
    beginTime = asctime(localtime(&beginnow));
    begin = clock();
    //G4cout << "Begin: " << beginTime << G4endl;

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

void RunAction::EndOfRunAction(const G4Run* run)
{
    // get time
    time_t endnow = time(0);
    endTime = asctime(localtime(&endnow));
    end = clock();
    //G4cout << "Finish: " << endTime << G4endl;

    // get the number of threads used
    int numberOfWorkerThreads = G4Threading::GetNumberOfRunningWorkerThreads();

    //
    G4RunManager* runManager = G4RunManager::GetRunManager();
    int totalEvents = runManager->GetNumberOfEventsToBeProcessed();
    G4int nofEvents = run->GetNumberOfEvent();

    // get user detector
    const auto detConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    
    // get user physics list
    const auto physicsList = static_cast<const PhysicsList*>(G4RunManager::GetRunManager()->GetUserPhysicsList());


    // get primary generator
    const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    // G4String particleName = particleGun->GetParticleDefinition()->GetParticleName();
    const G4double particleEnergy = generatorAction->GetGunEnergy();
    // G4cout << particleEnergy << G4endl;

    // save histograms & ntuple
    //
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    // Print
    if(IsMaster())
    {
      G4cout << "                                                                                      " << G4endl;
      G4cout << "                                                                                      " << G4endl;
      G4cout << "                              ==> GEANT4 OUTPUT REPORT <==                            " << G4endl;
      G4cout << "  __________________________________ [ Job Title ] _________________________________  " << G4endl;
      G4cout << " |                                                                                  | " << G4endl;
      G4cout << " | Application for simulating Accelerator at HUS                                    | " << G4endl;
      G4cout << " | -------------------------------------------------------------------------------- | " << G4endl;
      G4cout << " | author: BUI Tien Hung                                                            | " << G4endl;
      G4cout << " | email : hungbt1908@gmail.com                                                     | " << G4endl;
      G4cout << " | adress: Institute for Nuclear Science and Technology (INST)                      | " << G4endl;
      G4cout << " |__________________________________________________________________________________| " << G4endl;
      G4cout << "                                                                                      " << G4endl;
      G4cout << "                         Starting = " << beginTime                                      << G4endl;
      G4cout << "                         Ending   = " << endTime                                        << G4endl;
      G4cout << "                                                                                      " << G4endl;
      G4cout << " # Geometry setups:                                                                   " << G4endl;
      G4cout << "   Target MatName         : " << detConstruction->logicTarget->GetMaterial()->GetName() << G4endl;
      G4cout << "   Base Layer MatName     : " << detConstruction->logicBackLayer->GetMaterial()->GetName()<< G4endl;
      G4cout << "   Target Angle:                                                                      " << G4endl;
      G4cout << "   Detectors:                                                                         " << G4endl;
      G4cout << " # Physics setups:                                                                    " << G4endl;
      G4cout << "   Elastic Scale Factor   : " << physicsList->elasticScaleFactor                        << G4endl; 
      G4cout << "   Inelastic Scale Factor : " << physicsList->inelasticScaleFactor                      << G4endl;
      G4cout << " # Primary sources:                                                                   " << G4endl;  
      G4cout << " ------------------------------------------------------------------------------------ " << G4endl;
      G4cout << " # Number of worker threads        : " << numberOfWorkerThreads << " threads"           << G4endl;
      G4cout << " # Number of primary events        : " << nofEvents             << " events"            << G4endl;
      G4cout << " # Number of total events (scaled) : " << 250*nofEvents         << " events"            << G4endl;
      G4cout << " # Elapsed time                    : " << (double(diffclock(end, begin)/1000)) <<  " s" << G4endl;
      G4cout << "                                                                                      " << G4endl;
      G4cout << "                                                                                      " << G4endl;
    }
    else
    {}
}

void RunAction::SetFilename(G4String fileName) 
{
    fFileName = G4String(fileName);
}