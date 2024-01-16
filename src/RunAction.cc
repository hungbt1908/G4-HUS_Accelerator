#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "DetectorConstruction.hh"
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
    
    // get primary generator
    const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    // G4String particleName = particleGun->GetParticleDefinition()->GetParticleName();
    const G4double particleEnergy = generatorAction->GetGunEnergy();
    // G4cout << particleEnergy << G4endl;

    // open text file to write run information
    if (IsMaster())
    {
        // open text file to write run information
        std::ofstream file("Geant4_out.txt");
        //geant4OutputName.open("Geant4.out", std::ios::out);

        file << "                              ==> GEANT4 OUTPUT REPORT <==                            " << "\n";
        file << "  __________________________________ [ Job Title ] _________________________________  " << "\n";
        file << " |                                                                                  | " << "\n";
        file << " | Application for simulating Accelerator at HUS                                    | " << "\n";
        file << " | -------------------------------------------------------------------------------- | " << "\n";
        file << " | author: BUI Tien Hung                                                            | " << "\n";
        file << " | email : hungbt1908@gmail.com                                                     | " << "\n";
        file << " | adress: Institute for Nuclear Science and Technology (INST)                      | " << "\n";
        file << " |__________________________________________________________________________________| " << "\n";
        file << "                                                                                      " << "\n";
        file << "                         Starting = " << beginTime;
        file << "                         Ending   = " << endTime;
        file << "                                                                                      " << "\n";
        file << " >>>> Geometry Report                                                            <<<< " << "\n";
        file << "   + Target: " << "MatName = "    << detConstruction->logicTarget->GetMaterial()->GetName()                        << "; "
                                            << "Density = "    << detConstruction->logicTarget->GetMaterial()->GetDensity()/(g/cm3) << " g/cm3" << "; "
                                            << "Components = " << detConstruction->logicTarget->GetMaterial()->GetNumberOfElements()            << "; "
                         << "\n";
        file << "   + Baselayer: " << "MatName = "    << detConstruction->logicBackLayer->GetMaterial()->GetName()                        << "; "
                                               << "Density = "    << detConstruction->logicBackLayer->GetMaterial()->GetDensity()/(g/cm3) << " g/cm3" << "; "
                                               << "Components = " << detConstruction->logicBackLayer->GetMaterial()->GetNumberOfElements()            << "; "

             << "\n";
        file << "                                                                                      " << "\n";

        file << " >>>> Physics Report                                                             <<<< " << G4endl;

        file << " >>>> Source Report                                                              <<<< " << G4endl; 
        // file << particleEnergy << G4endl;

        file << " >>>> Simulation Report                                                          <<<< " << "\n";
        file << "  + number of worker threads: " << numberOfWorkerThreads << " threads" << "\n";
        file << "  + beam on                 : " << totalEvents << " events" << "\n";
        file << "  + total events            : " << nofEvents   << " events" << "\n";
        file << "  + elapsed time            : " << (double(diffclock(end, begin)/1000)) << " seconds    " << G4endl;
        file.close();
    }

    // print histogram statistics
    //
    auto analysisManager = G4AnalysisManager::Instance();

    // save histograms & ntuple
    //
    analysisManager->Write();
    analysisManager->CloseFile();
}

void RunAction::SetFilename(G4String fileName) 
{
    fFileName = G4String(fileName);
}