#include "PrimaryGenerationActionMessenger.hh"
#include "PrimaryGenerationAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

PrimaryGenerationActionMessenger::PrimaryGenerationActionMessenger(PrimaryGenerationAction* pPrimaryGenerationAction) 
	: fPrimaryGenerationAction(pPrimaryGenerationAction)
{
	BeamDIR = new G4UIdirectory("/HUS/Beam/");
    BeamDIR->SetGuidance("............................");

	SetNOfParticleCmd = new G4UIcmdWithAnInteger("/HUS/Beam/SetNOfParticle", this);
	SetNOfParticleCmd->SetGuidance("Set number of particles per event");
	SetNOfParticleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	SetBeamEnergyCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetBeamEnergy", this);
	SetBeamEnergyCmd->SetDefaultUnit("MeV");
	SetBeamEnergyCmd->SetDefaultValue(1.);
	SetBeamEnergyCmd->SetGuidance("Energy of the beam");

	SetBeamRadiusCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetBeamRadius", this);
	SetBeamRadiusCmd->SetDefaultUnit("mm");
	SetBeamRadiusCmd->SetDefaultValue(10.);
	SetBeamRadiusCmd->SetGuidance("radius of the beam");

	SetBeamXPosCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetBeamXPos", this);
	SetBeamXPosCmd->SetGuidance("Set X position of proton beam.");
    SetBeamXPosCmd->SetParameterName("XPos", false);
    SetBeamXPosCmd->SetUnitCategory("Length");
	SetBeamXPosCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	SetBeamYPosCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetBeamYPos", this);
	SetBeamYPosCmd->SetGuidance("Set Y position of proton beam.");
    SetBeamYPosCmd->SetParameterName("YPos", false);
    SetBeamYPosCmd->SetUnitCategory("Length");
	SetBeamYPosCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	SetBeamZPosCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetBeamZPos", this);
	SetBeamZPosCmd->SetGuidance("Set Z position of proton beam.");
    SetBeamZPosCmd->SetParameterName("ZPos", false);
    SetBeamZPosCmd->SetUnitCategory("Length");
	SetBeamZPosCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	SetMinSolidAngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetMinSolidAngle", this);
	SetMinSolidAngleCmd->SetGuidance("Set min solid angle of proton beam.");
	SetMinSolidAngleCmd->SetParameterName("minAlpha", false);
	SetMinSolidAngleCmd->SetUnitCategory("Angle");
	SetMinSolidAngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	SetMaxSolidAngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetMaxSolidAngle", this);
	SetMaxSolidAngleCmd->SetGuidance("Set max solid angle of proton beam.");
	SetMaxSolidAngleCmd->SetParameterName("maxAlpha", false);
	SetMaxSolidAngleCmd->SetUnitCategory("Angle");
	SetMaxSolidAngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	SetThetaRotationAngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetThetaRotationAngle", this);
	SetThetaRotationAngleCmd->SetGuidance("Set theta rotation angle of proton beam.");
	SetThetaRotationAngleCmd->SetParameterName("theta", false);
	SetThetaRotationAngleCmd->SetUnitCategory("Angle");
	SetThetaRotationAngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	SetPhiRotationAngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Beam/SetPhiRotationAngle", this);
	SetPhiRotationAngleCmd->SetGuidance("Set phi rotation angle of proton beam.");
	SetPhiRotationAngleCmd->SetParameterName("phi", false);
	SetPhiRotationAngleCmd->SetUnitCategory("Angle");
	SetPhiRotationAngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

PrimaryGenerationActionMessenger::~PrimaryGenerationActionMessenger(void)
{
}
void PrimaryGenerationActionMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
	if(cmd == SetNOfParticleCmd)
	{
		fPrimaryGenerationAction->SetNOfParticle(SetNOfParticleCmd->GetNewIntValue(newValue));
	}

	if(cmd == SetBeamEnergyCmd)
	{
		SetBeamEnergyCmd->GetNewUnitValue(newValue);
		fPrimaryGenerationAction->SetGunEnergy(SetBeamEnergyCmd->GetNewDoubleValue(newValue));
	}

	if(cmd == SetBeamRadiusCmd)
	{
		fPrimaryGenerationAction->SetGunRadius(SetBeamRadiusCmd->GetNewDoubleValue(newValue));
	}

	if(cmd == SetBeamXPosCmd)
	{
		fPrimaryGenerationAction->SetGunXPosition(SetBeamXPosCmd->GetNewDoubleValue(newValue));
	}

	if(cmd == SetBeamYPosCmd)
	{
		fPrimaryGenerationAction->SetGunYPosition(SetBeamYPosCmd->GetNewDoubleValue(newValue));
	}

	if(cmd == SetBeamZPosCmd)
	{
		fPrimaryGenerationAction->SetGunZPosition(SetBeamZPosCmd->GetNewDoubleValue(newValue));
	}

	if(cmd == SetMinSolidAngleCmd)
	{
		fPrimaryGenerationAction->SetMinSolidAngle(SetMinSolidAngleCmd->GetNewDoubleValue(newValue));
	}

	if(cmd == SetMaxSolidAngleCmd)
	{
		fPrimaryGenerationAction->SetMaxSolidAngle(SetMaxSolidAngleCmd->GetNewDoubleValue(newValue));
	}

	if(cmd == SetThetaRotationAngleCmd)
	{
		fPrimaryGenerationAction->SetThetaRotationAngle(SetThetaRotationAngleCmd->GetNewDoubleValue(newValue));
	}

	if(cmd == SetPhiRotationAngleCmd)
	{
		fPrimaryGenerationAction->SetPhiRotationAngle(SetPhiRotationAngleCmd->GetNewDoubleValue(newValue));
	}

}
