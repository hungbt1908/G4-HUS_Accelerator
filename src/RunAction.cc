#include "RunAction.hh"
#include "RunActionMessenger.hh"
#include "DetectorConstruction.hh"

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
    // get time
    time_t now = time(NULL);
    struct std::tm* beginTime = localtime(&now);

    begin = clock();

    // open text file to write run information
    geant4OutputName.open("Geant4.out", std::ios::out);

    geant4OutputName << "                              ==> GEANT4 OUTPUT REPORT <==                            " << "\n";
    geant4OutputName << "  __________________________________ [ Job Title ] _________________________________  " << "\n";
    geant4OutputName << " |                                                                                  | " << "\n";
    geant4OutputName << " | Application for simulating Accelerator at HUS                                    | " << "\n";
    geant4OutputName << " | -------------------------------------------------------------------------------- | " << "\n";
    geant4OutputName << " | author: BUI Tien Hung                                                            | " << "\n";
    geant4OutputName << " | email : hungbt1908@gmail.com                                                     | " << "\n";
    geant4OutputName << " | adress: Institute for Nuclear Science and Technology (INST)                      | " << "\n";
    geant4OutputName << " |__________________________________________________________________________________| " << "\n";
    geant4OutputName << "                                                                                      " << "\n";
    geant4OutputName << "                         Starting = " << asctime(beginTime);
    geant4OutputName.close();
    

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
    end = clock();
    // get time
    time_t now = time(0);
    struct std::tm* endTime = localtime(&now);

    // get the number of threads used
    int numberOfWorkerThreads = G4Threading::GetNumberOfRunningWorkerThreads();

    //
    G4RunManager* runManager = G4RunManager::GetRunManager();
    int totalEvents = runManager->GetNumberOfEventsToBeProcessed();
    G4int nofEvents = run->GetNumberOfEvent();

    const auto detConstruction = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    // open text file to write run information
    if (IsMaster())
    {
        geant4OutputName.open("Geant4.out", ios::app);
        geant4OutputName << "                         Ending   = " << asctime(endTime);
        geant4OutputName << "                                                                                      " << "\n";
        geant4OutputName << " >>>> Geometry Report                                                            <<<< " << "\n";
        geant4OutputName << "   + Target: " << "MatName = "    << detConstruction->logicTarget->GetMaterial()->GetName()                        << "; "
                                            << "Density = "    << detConstruction->logicTarget->GetMaterial()->GetDensity()/(g/cm3) << " g/cm3" << "; "
                                            << "Components = " << detConstruction->logicTarget->GetMaterial()->GetNumberOfElements()            << "; "
                         << "\n";
        geant4OutputName << "   + Baselayer: " << "MatName = "    << detConstruction->logicBackLayer->GetMaterial()->GetName()                        << "; "
                                               << "Density = "    << detConstruction->logicBackLayer->GetMaterial()->GetDensity()/(g/cm3) << " g/cm3" << "; "
                                               << "Components = " << detConstruction->logicBackLayer->GetMaterial()->GetNumberOfElements()            << "; "

                         << "\n";
        geant4OutputName << "                                                                                      " << "\n";

        geant4OutputName << " >>>> Physics Report                                                             <<<< " << "\n";
        geant4OutputName << "                                                                                      " << "\n";

        geant4OutputName << " >>>> Source Report                                                              <<<< " << "\n";
        geant4OutputName << "                                                                                      " << "\n";

        geant4OutputName << " >>>> Simulation Report                                                          <<<< " << "\n";
        geant4OutputName << "  + number of worker threads: " << numberOfWorkerThreads << " threads" << "\n";
        geant4OutputName << "  + beam on                 : " << totalEvents << " events" << "\n";
        geant4OutputName << "  + total events            : " << nofEvents   << " events" << "\n";
        geant4OutputName << "  + elapsed time            : " << (double(diffclock(end, begin)/1000))/numberOfWorkerThreads << " seconds    " << "\n";
        geant4OutputName.close();
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