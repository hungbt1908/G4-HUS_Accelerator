#ifndef PrimaryGenerationAction_h
#define PrimaryGenerationAction_h 1


#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include "G4Timer.hh"
#include "Randomize.hh" 
#include "G4RunManager.hh"

#include "G4ParticleDefinition.hh"

class PrimaryGenerationActionMessenger;

class PrimaryGenerationAction : public G4VUserPrimaryGeneratorAction
{
public:
	PrimaryGenerationAction();
	~PrimaryGenerationAction();
	void GeneratePrimaries(G4Event *anEvent);

	inline void SetNOfParticle(G4double val){nOfParticle = val;}
	inline void SetGunEnergy(G4double val){energy = val;}
	inline void SetGunRadius(G4double val){GunRadius = val;}
	inline void SetGunXPosition(G4double val){x0 = val;}
	inline void SetGunYPosition(G4double val){y0 = val;}
	inline void SetGunZPosition(G4double val){z0 = val;}
	inline void SetMinSolidAngle(G4double val){minAlpha = val;}
	inline void SetMaxSolidAngle(G4double val){maxAlpha = val;}
	inline void SetThetaRotationAngle(G4double val){theta = val;}
	inline void SetPhiRotationAngle(G4double val){phi = val;}

private:
	G4double nOfParticle;
	G4double energy, GunRadius;
	G4double x0, y0, z0;
	G4ThreeVector dir, pos;
	G4double theta, phi;
	G4double minAlpha, maxAlpha;

	PrimaryGenerationActionMessenger* fPrimaryGenerationActionMessenger;
	G4ParticleGun *particleGun;
};

#endif
