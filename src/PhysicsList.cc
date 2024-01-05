#include "PhysicsList.hh"

// gamma
#include "G4RayleighScattering.hh" //
#include "G4PenelopeRayleighModel.hh"
#include "G4LivermoreRayleighModel.hh"
#include "G4PhotoElectricEffect.hh" //
#include "G4PenelopePhotoElectricModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4ComptonScattering.hh" //
#include "G4PenelopeComptonModel.hh"
#include "G4LivermoreComptonModel.hh"
#include "G4KleinNishinaModel.hh"
#include "G4GammaConversion.hh" //
#include "G4PenelopeGammaConversionModel.hh"
#include "G4LivermoreGammaConversionModel.hh"
#include "G4BetheHeitler5DModel.hh"
#include "G4BetheHeitlerModel.hh" 
#include "G4PairProductionRelModel.hh"
#include "G4HadronInelasticProcess.hh" //
#include "G4GammaNuclearXS.hh"
#include "G4LowEGammaNuclearModel.hh"
#include "G4CascadeInterface.hh"

// e-
#include "G4CoulombScattering.hh" //
#include "G4eSingleCoulombScatteringModel.hh"
#include "G4eMultipleScattering.hh" //
#include "G4UrbanMscModel.hh"
#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4eIonisation.hh" //
#include "G4MollerBhabhaModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4eBremsstrahlung.hh" //
#include "G4SeltzerBergerModel.hh"
#include "G4LivermoreBremsstrahlungModel.hh"
#include "G4eplusAnnihilation.hh" //

// proton
#include "G4HadronInelasticProcess.hh" //
#include "G4BGGNucleonInelasticXS.hh"
#include "G4ParticleInelasticXS.hh"
//
#include "G4BGGNucleonElasticXS.hh"
#include "G4QGSModel.hh"
#include "G4FTFModel.hh"

// alpha
#include "G4hMultipleScattering.hh" //
#include "G4ionIonisation.hh" //
#include "G4NuclearStopping.hh" //
#include "G4BinaryLightIonReaction.hh"
#include "G4CrossSectionInelastic.hh"
#include "G4ComponentGGNuclNuclXsc.hh"
#include "G4HadronElastic.hh"

// He3
//#include "G4GGNuclNuclCrossSection.hh"

#include "G4HadronicParameters.hh"


PhysicsList::PhysicsList()
  : G4VUserPhysicsList()
{}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{
    G4BosonConstructor bosonConst;
    bosonConst.ConstructParticle();

    G4LeptonConstructor leptonConst;
    leptonConst.ConstructParticle();

    G4MesonConstructor mesonConst;
    mesonConst.ConstructParticle();

    G4BaryonConstructor baryonConst;
    baryonConst.ConstructParticle();

    G4IonConstructor ionConst;
    ionConst.ConstructParticle();

    G4ShortLivedConstructor shortLivedConst;
    shortLivedConst.ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
    ph->AddTransportation();
    auto particleIterator = GetParticleIterator();
    particleIterator->reset();

    while((*particleIterator)())
    {   
        // get the current particle definition
        G4ParticleDefinition* particle = particleIterator->value();
        G4String particleName = particle->GetParticleName();

        G4Decay* aDecay = new G4Decay();
        if (aDecay->IsApplicable(*particle)) ph->RegisterProcess(aDecay, particle);

        if (particleName == "gamma") 
        {
            // EM components
            // ----------------------------------------------------------------

            //
            G4RayleighScattering* raylProcess = new G4RayleighScattering(); // rayleigh
            //raylProcess->SetEmModel(new G4PenelopeRayleighModel());
            raylProcess->SetEmModel(new G4LivermoreRayleighModel());
            ph->RegisterProcess(raylProcess, particle);

            //
            G4PhotoElectricEffect* photProcess = new G4PhotoElectricEffect(); // photoelectric
            // photProcess->SetEmModel(new G4PenelopePhotoElectricModel());
            photProcess->SetEmModel(new G4LivermorePhotoElectricModel());
            ph->RegisterProcess(photProcess, particle);

            //
            G4ComptonScattering* comptProcess = new G4ComptonScattering(); // compton
            comptProcess->SetEmModel(new G4KleinNishinaModel());
            // comptProcess->SetEmModel(new G4PenelopeComptonModel());
            // comptProcess->SetEmModel(new G4LivermoreComptonModel());
            ph->RegisterProcess(comptProcess, particle);

            //
            G4GammaConversion* convProcess = new G4GammaConversion(); // conversion
            // convProcess->SetEmModel(new G4PenelopeGammaConversionModel()); // low models
            // convProcess->SetEmModel(new G4LivermoreGammaConversionModel());
            // convProcess-SetEmModel(new G4BetheHeitler5DModel());
            convProcess->SetEmModel(new G4BetheHeitlerModel()); // from Geant3, its for application below 100 GeV
            // convProcess->SetEmModel(new G4PairProductionRelModel()); // model for high energy application (above MeV), it take into account the Landau-Pomeranchuk-Migdal (LPM) effect
            ph->RegisterProcess(convProcess, particle);

            // Hadron components 
            // ----------------------------------------------------------------

            //
            G4HadronInelasticProcess* photoNuclProcess = new G4HadronInelasticProcess("photonNuclear", G4Gamma::Definition()); // photon-nuclear
            photoNuclProcess->AddDataSet(new G4GammaNuclearXS()); // data
            //
            // low gamma-nuclear model from 0 - 200 MeV
            G4LowEGammaNuclearModel* theLowGammaNuclear = new G4LowEGammaNuclearModel();
            theLowGammaNuclear->SetMaxEnergy(200*MeV);
            photoNuclProcess->RegisterMe(theLowGammaNuclear);
            //
            G4CascadeInterface* theCascade = new G4CascadeInterface();
            theCascade->SetMinEnergy(199*MeV);   
            theCascade->SetMaxEnergy(10*GeV);
            photoNuclProcess->RegisterMe(theCascade);

            G4ProcessManager* gammaProcessManager = G4Gamma::Gamma()->GetProcessManager();
            gammaProcessManager->AddDiscreteProcess(photoNuclProcess);
        } 
        else if (particleName == "e-") 
        {
            // EM components
            // ----------------------------------------------------------------

            //
            G4CoulombScattering* eCoulombScatProcess = new G4CoulombScattering();
            eCoulombScatProcess->SetEmModel(new G4eSingleCoulombScatteringModel());
            ph->RegisterProcess(eCoulombScatProcess, particle);

            //
            G4eMultipleScattering* mscProcess = new G4eMultipleScattering(); // msc
            // mscProcess->SetEmModel(new G4UrbanMscModel()); // used in the default EM Physics List
            mscProcess->SetEmModel(new G4GoudsmitSaundersonMscModel());
            // mscProcess->SetEmModel(new G4WentzelVIModel());
            ph->RegisterProcess(mscProcess, particle);

            //
            G4eIonisation* eIoniProcess = new G4eIonisation(); // ionisation
            eIoniProcess->SetEmModel(new G4MollerBhabhaModel()); // used in the default EM Physics List
            // eIoniProcess->SetEmModel(new G4PenelopeIonisationModel());
            // eIoniProcess->SetEmModel(new G4LivermoreIonisationModel());
            ph->RegisterProcess(eIoniProcess,  particle);

            //
            G4eBremsstrahlung* eBremProcess = new G4eBremsstrahlung();
            // eBremProcess->SetEmModel(new G4SeltzerBergerModel());
            // eBremProcess->SetEmModel(new G4LivermoreBremsstrahlungModel());
            ph->RegisterProcess(eBremProcess, particle);   
        } 
        else if (particleName == "e+") 
        {
            // EM components
            // ----------------------------------------------------------------

            //
            G4CoulombScattering* eCoulombScatProcess = new G4CoulombScattering();
            eCoulombScatProcess->SetEmModel(new G4eSingleCoulombScatteringModel());
            ph->RegisterProcess(eCoulombScatProcess, particle);

            //
            G4eMultipleScattering* eMscProcess = new G4eMultipleScattering();
            eMscProcess->SetEmModel(new G4UrbanMscModel());
            // eMscProcess->SetEmModel(new G4GoudsmitSaundersonMscModel());
            ph->RegisterProcess(eMscProcess, particle);

            //
            G4eIonisation* eIoniProcess = new G4eIonisation(); // ionisation
            eIoniProcess->SetEmModel(new G4MollerBhabhaModel()); // used in the default EM Physics List
            // eIoniProcess->SetEmModel(new G4PenelopeIonisationModel());
            // eIoniProcess->SetEmModel(new G4LivermoreIonisationModel());
            ph->RegisterProcess(eIoniProcess,  particle);

            //
            G4eBremsstrahlung* eBremProcess = new G4eBremsstrahlung();
            // eBremProcess->SetEmModel(new G4SeltzerBergerModel());
            // eBremProcess->SetEmModel(new G4LivermoreBremsstrahlungModel());
            ph->RegisterProcess(eBremProcess, particle); 

            //
            G4eplusAnnihilation* eAnnihilProcess = new G4eplusAnnihilation();
            ph->RegisterProcess(eAnnihilProcess, particle);      
        }
        else if (particleName == "proton")
        {
            // EM components
            // ----------------------------------------------------------------

            ph->RegisterProcess(new G4hMultipleScattering, particle); 
            ph->RegisterProcess(new G4hIonisation, particle);
            ph->RegisterProcess(new G4hBremsstrahlung, particle);
            ph->RegisterProcess(new G4hPairProduction, particle);
            ph->RegisterProcess(new G4CoulombScattering, particle);


            // Hadron components 
            // ----------------------------------------------------------------

            // inelastic
            G4HadronInelasticProcess* theProtonInelasticProcess = new G4HadronInelasticProcess("protonInelastic", G4Proton::Definition());
            // theProtonInelasticProcess->AddDataSet(new G4BGGNucleonInelasticXS(G4Proton::Definition())); // like FTFP-BERT, QGSP-BERT, QGSP-BIC
            theProtonInelasticProcess->AddDataSet(new G4ParticleInelasticXS(G4Proton::Definition())); // like QBBC

            // Bertini Intranuclear Cascade Model
            G4CascadeInterface* theBertiniCascadeModel = new G4CascadeInterface(); // like FTFP-BERT, QGSP-BERT, 
            theBertiniCascadeModel->SetMinEnergy(0.*GeV);
            theBertiniCascadeModel->SetMaxEnergy(6.*GeV);
            theProtonInelasticProcess->RegisterMe(theBertiniCascadeModel);

            // Binary Cascade Model
            // G4BinaryCascade* theBinaryCascadeModel = new G4BinaryCascade(); like QBBC
            // theBinaryCascadeModel->SetMinEnergy(0.*eV);   
            // theBinaryCascadeModel->SetMaxEnergy(1.5*GeV);
            // theProtonInelasticProcess->RegisterMe(theBinaryCascadeModel);

            // Quark-gluon String (QGS) and FTF models 
            // G4QGSModel* theQGSModel = new G4QGSModel(""); // like QGSP-BIC
            // theProtonInelasticProcess->RegisterMe(theQGSModel);
            // G4FTFModel* theFTFModel = new G4FTFModel();
            // theProtonInelasticProcess->RegisterMe(theFTFModel);

            // elastic 
            G4HadronElasticProcess * theProtonElasticProcess = new G4HadronElasticProcess("hadElastic");
            theProtonElasticProcess->AddDataSet(new G4BGGNucleonElasticXS(G4Proton::Definition())); // like FTFP-BERT, QBBC, QGSP-BERT, QGSP_BIC
            theProtonElasticProcess->RegisterMe(new G4ChipsElasticModel());

            G4ProcessManager* protonProcessManager = G4Proton::Proton()->GetProcessManager();
            protonProcessManager->AddDiscreteProcess(theProtonElasticProcess);
            protonProcessManager->AddDiscreteProcess(theProtonInelasticProcess);

        }
        else if (particleName == "alpha")
        {
            // EM components
            // ----------------------------------------------------------------

            // EM components
            ph->RegisterProcess(new G4hMultipleScattering, particle);
            ph->RegisterProcess(new G4ionIonisation, particle);
            ph->RegisterProcess(new G4NuclearStopping, particle);

            // Hadron components 
            // ----------------------------------------------------------------

            // inelastic
            G4HadronInelasticProcess* alphaInelasticProcess = new G4HadronInelasticProcess("AlphaInelastic", G4Alpha::Alpha());
            //alphaInelasticProcess->AddDataSet(new G4VCrossSectionDataSet("G4GlauberGribovCrossSection"));
            alphaInelasticProcess->AddDataSet(new G4CrossSectionInelastic(new G4ComponentGGNuclNuclXsc())); // like FTFP-BERT, QGSP-BERT
            // alphaInelasticProcess->AddDataSet(new G4ParticleInelasticXS(G4Alpha::Definition())); // like QBBC
            //alphaInelasticProcess->AddDataSet(new G4VCrossSectionDataSet("G4GGNuclNuclCrossSection"));
            G4BinaryLightIonReaction* theBinaryLightIonCascadeMode = new G4BinaryLightIonReaction();
            theBinaryLightIonCascadeMode->SetMinEnergy(0.*eV);
            theBinaryLightIonCascadeMode->SetMaxEnergy(6.*GeV);
            alphaInelasticProcess->RegisterMe(theBinaryLightIonCascadeMode);
            
            // elastic
            G4HadronElasticProcess* alphaElasticProcess = new G4HadronElasticProcess("hadElastic");
            alphaElasticProcess->AddDataSet(new G4CrossSectionInelastic(new G4ComponentGGNuclNuclXsc())); // like QBBC, FTFP-BERT, QGSP-BERT
            alphaElasticProcess->RegisterMe(new G4HadronElastic("hElasticLHEP")); // // like QBBC, FTFP-BERT, QGSP-BERT

            //
            G4ProcessManager* alphaProcessManager = G4Alpha::Alpha()->GetProcessManager();
            alphaProcessManager->AddDiscreteProcess(alphaElasticProcess);
            alphaProcessManager->AddDiscreteProcess(alphaInelasticProcess);
        }
        else if (particleName == "He3")
        {
            // inelastic
            G4HadronInelasticProcess* He3InelasticProcess = new G4HadronInelasticProcess("He3Inelastic", G4He3::He3()); // *** process ***
            G4CrossSectionInelastic* theHe3InelasticCrossSection = new G4CrossSectionInelastic(new G4ComponentGGNuclNuclXsc()); // *** cross section ***
            theHe3InelasticCrossSection->SetMinKinEnergy(0.*eV);
            theHe3InelasticCrossSection->SetMaxKinEnergy(25.6*PeV);
            He3InelasticProcess->AddDataSet(theHe3InelasticCrossSection); // like FTFP-BERT, QGSP-BERT
            G4BinaryLightIonReaction* theBinaryLightIonCascadeMode = new G4BinaryLightIonReaction(); // *** model ***
            theBinaryLightIonCascadeMode->SetMinEnergy(0.*eV);
            theBinaryLightIonCascadeMode->SetMaxEnergy(6.*GeV);
            He3InelasticProcess->RegisterMe(theBinaryLightIonCascadeMode);

            // elastic

            //
            G4ProcessManager* He3ProcessManager = G4He3::He3()->GetProcessManager();
            He3ProcessManager->AddDiscreteProcess(He3InelasticProcess);

        }
        else if (particleName == "GenericIon")
        {
            // EM proceses
            //
            ph->RegisterProcess(new G4hMultipleScattering, particle);

            //
            G4ionIonisation* ionIoni = new G4ionIonisation;
            ionIoni->SetEmModel(new G4IonParametrisedLossModel());
            ph->RegisterProcess(ionIoni, particle);

            //
            ph->RegisterProcess(new G4NuclearStopping(), particle);
        }
        else if((!particle->IsShortLived()) &&
               (particle->GetPDGCharge() != 0.0) && 
               (particle->GetParticleName() != "chargedgeantino"))
        {
            // all others charged particles except geantino
            ph->RegisterProcess(new G4hMultipleScattering(), particle);
            ph->RegisterProcess(new G4hIonisation(), particle);
            ph->RegisterProcess(new G4NuclearStopping(), particle);
        }

        // Scaling the nucleon cross sections
        G4HadronicParameters::Instance()->SetApplyFactorXS(true);
        G4HadronicParameters::Instance()->SetXSFactorNucleonElastic(0.85);
        G4HadronicParameters::Instance()->SetXSFactorNucleonInelastic(1.10);
    }
}