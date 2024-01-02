#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

struct eDepStruct
{
    G4double TotalEDep;
    G4double ProtonEDep;
    G4double AlphaEDep;
    G4double BeEDep;
    G4double B11EDep;
    G4double B10EDep;
    G4double GammaEDep;
    G4double OtherEDep;
};

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    ~EventAction();

    void  BeginOfEventAction(const G4Event* event) override;
    void    EndOfEventAction(const G4Event* event) override;

    void AddEdepTotal(G4double tde, int id) {eDep[id].TotalEDep += tde;}
    void AddEdepProton(G4double tde, int id) {eDep[id].ProtonEDep += tde;}
    void AddEdepAlpha(G4double tde, int id) {eDep[id].AlphaEDep += tde;}
    void AddEdepBe(G4double tde, int id) {eDep[id].BeEDep += tde;}
    void AddEdepB11(G4double tde, int id) {eDep[id].B11EDep += tde;}
    void AddEdepB10(G4double tde, int id) {eDep[id].B10EDep += tde;}
    void AddEdepGamma(G4double tde, int id) {eDep[id].GammaEDep += tde;}
    void AddEdepOther(G4double tde, int id) {eDep[id].OtherEDep += tde;}

  private:
    eDepStruct* eDep;
};
#endif


