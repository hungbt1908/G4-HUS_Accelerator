#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <fstream>
#include <iostream>
#include <ctime>
using namespace std;

class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction();

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;

    void SetFilename(G4String fileName);

  private:
    G4String fFileName;
    ofstream geant4OutputName;
    struct std::tm* beginTime;
    struct std::tm* endTime;
    clock_t begin;
    clock_t end;
    double diffclock(clock_t clock1, clock_t clock2);

    RunActionMessenger* fRunActionMessenger;
};

#endif

