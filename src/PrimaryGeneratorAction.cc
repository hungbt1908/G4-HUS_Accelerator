#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4PhysicalConstants.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int nofParticles = 100;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic
  //
  auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("proton");
  fParticleGun->SetParticleDefinition(particleDefinition);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Set gun position
  //position
  G4double posZ = -1.*cm, posX = 0.*cm, posY = 0.*cm;
  G4double a = 2.*pi*G4UniformRand(); //angle
  G4double rc = 0.5*mm;//radius
  G4double r = (rc*std::sqrt(G4UniformRand()));
  G4double Xo = 0.*mm, Yo = 0.*mm; //origin
  posX = r*std::cos(a) + Xo;
  posY = r*std::sin(a) + Yo;
  fParticleGun->SetParticlePosition(G4ThreeVector(posX, posY, posZ));

  // Energy
  fParticleGun->SetParticleEnergy(1.5*MeV);

  // Direction
  G4double ux = 0.;
  G4double uy = 0.;
  G4double uz = 1.;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux, uy, uz));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
