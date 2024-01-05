#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

#include "G4PhysicsListHelper.hh"

#include "G4SystemOfUnits.hh"

//
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

//particle
#include "G4Gamma.hh"
#include "G4Proton.hh"
#include "G4Alpha.hh"
#include "G4He3.hh"

#include "G4ProcessManager.hh"

// ****************************************************************************
// EM process
// ****************************************************************************
// --- gamma ---
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh" // model
#include "G4ComptonScattering.hh"
#include "G4KleinNishinaModel.hh" // model
#include "G4GammaConversion.hh"
#include "G4BetheHeitlerModel.hh" // model
#include "G4RayleighScattering.hh"
#include "G4LivermoreRayleighModel.hh" // model
// --- e- & e+ ---
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
// --- proton & anti-proton ---
#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"
#include "G4CoulombScattering.hh"

/// --- ions ---
#include "G4hMultipleScattering.hh"
#include "G4IonParametrisedLossModel.hh"

// ****************************************************************************
// Hadronic process
// ****************************************************************************
// --- gamma ---
#include "G4HadronInelasticProcess.hh" // "photonNuclear"
#include "G4GammaNuclearXS.hh"
#include "G4PhotoNuclearCrossSection.hh" // datasets
#include "G4LowEGammaNuclearModel.hh" // model
#include "G4CascadeInterface.hh" // model
// --- proton & anti-proton ---
#include "G4HadronElasticProcess.hh" // "hadElastic"
#include "G4BGGNucleonElasticXS.hh" // datasets
#include "G4ChipsElasticModel.hh" // model
#include "G4HadronInelasticProcess.hh" // "protonInelastic" process
#include "G4BGGNucleonInelasticXS.hh" // datasets
#include "G4BinaryCascade.hh" // model
#include "G4CascadeInterface.hh" // model
// --- alpha & He3 ---
#include "G4HadronElasticProcess.hh" // "AlphaElastic" process
#include "G4VCrossSectionDataSet.hh" // "G4GlauberGribovCrossSection" datasets
#include "G4ChipsElasticModel.hh" // model
#include "G4HadronInelasticProcess.hh" // "AlphaInelastic" process
#include "G4VCrossSectionDataSet.hh" // "GheishaInelastic" datasets
#include "G4HadronicInteraction.hh" // "G4LEAlphaInelastic" model

// ****************************************************************************
// Decay process
// ****************************************************************************
#include "G4Decay.hh"

class G4VPhysicsConstructor;

class PhysicsList: public G4VUserPhysicsList
{
public:

  PhysicsList();
  virtual ~PhysicsList();

  virtual void ConstructParticle();
  virtual void ConstructProcess();
};
#endif

