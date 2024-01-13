#ifndef ML2PrimaryGenerationActionMessenger_h
#define ML2PrimaryGenerationActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class PrimaryGenerationAction;
class G4UImessenger;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;

class PrimaryGenerationActionMessenger : public G4UImessenger 
{
public:
	PrimaryGenerationActionMessenger(PrimaryGenerationAction* pPrimaryGenerationAction);
	~PrimaryGenerationActionMessenger(void);
	void SetNewValue(G4UIcommand* cmd, G4String newValue);

private:
	PrimaryGenerationAction* fPrimaryGenerationAction;
	G4UIdirectory* BeamDIR;
	G4UIcmdWithAnInteger* SetNOfParticleCmd;
	G4UIcmdWithADoubleAndUnit* SetBeamEnergyCmd;
	G4UIcmdWithADoubleAndUnit* SetBeamRadiusCmd;
	G4UIcmdWithADoubleAndUnit* SetBeamXPosCmd, * SetBeamYPosCmd, * SetBeamZPosCmd;
	G4UIcmdWithADoubleAndUnit* SetMinSolidAngleCmd, * SetMaxSolidAngleCmd;
	G4UIcmdWithADoubleAndUnit* SetPhiRotationAngleCmd, * SetThetaRotationAngleCmd;

};

#endif

