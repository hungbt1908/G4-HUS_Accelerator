# 1. Geometry

---
# 2. Physics
## 2.1 Hadronic components
* **Photon**
  * Photon Nuclear:
    * process: `G4HadronInelasticProcess("photonNuclear")`
    * cross section: `G4GammaNuclearXS()`
    * model: 
      * `G4LowEGammaNuclearModel` in range 0 - 200 MeV
      * `G4CascadeInterface` in range 199 MeV - 10 GeV
* **Proton**
  * Elastic: 
    * process: `G4HadronElasticProcess("hadElastic")`
    * cross section: `G4BGGNucleonElasticXS(G4Proton::Definition())`
    * model: `G4ChipsElasticModel`
  * Inelastic:
    * process: `G4HadronInelasticProcess("protonInelastic")`
    * cross section: `G4ParticleInelasticXS(G4Proton::Definition())`
    * model: `G4CascadeInterface` in range 0 - 6 GeV
* **Alpha**
  * Elastic:
    * process: `G4HadronElasticProcess("hadElastic")`
    * cross section: `G4CrossSectionElastic(new G4ComponentGGNuclNuclXsc())` in range 0 - 25.6 PeV
    * model: `G4HadronElastic("hElasticLHEP")` in range 0 - 6 GeV
  * Inelastic:
    * process: `G4HadronInelasticProcess("AlphaInelastic")`
    * cross section: `G4CrossSectionInelastic(new G4ComponentGGNuclNuclXsc())` in range 0 - 25.6 PeV
    * model: `G4BinaryLightIonReaction` in range 0 - 6 GeV
* **He3**
  * Elastic:
    * process: `G4HadronElasticProcess("hadElastic")`
    * cross section: `G4CrossSectionElastic(new G4ComponentGGNuclNuclXsc())` in range 0 - 25.6 PeV
    * model: `G4HadronElastic("hElasticLHEP")` in range 0 - 100 TeV
* **Neutron**
  * Elastic:
    * process: `G4HadronElasticProcess("hadElastic")`
    * cross section: `G4NeutronElasticXS`
    * model: `G4HadronElastic("hElasticCHIPS")`
  * Inelastic:
    * process: `G4HadronInelasticProcess("neutronInelastic")`
    * cross section: `G4NeutronInelasticXS()` in range 0 - 100 TeV
    * model:
      * `G4CascadeInterface()` in range 1 GeV - 6 GeV
      * `G4BinaryCascade()` in range 0 - 1.5 GeV
  * Capture:
    * process: `G4NeutronCaptureProcess`
    * cross section: `G4NeutronCaptureXS` in range 0 - 100 TeV
    * model: `G4NeutronRadCapture` in range 0 - 100 TeV
## 2.2 EM components
* **Photon**
  * Rayleigh Scattering: 
    * model: `G4LivermoreRayleighModel`
  * Photoelectric:
    * model: `G4LivermorePhotoElectricModel`
  * Compton Scattering:
    * model: `G4KleinNishinaModel`
  * Gamma Conversion:
    * model: `G4BetheHeitlerModel`
* **Electron**
  * Coulomb Scattering:
    * model: `G4eSingleCoulombScatteringModel`
  * Multiple Scattering:
    * model: `G4GoudsmitSaundersonMscModel`
  * Ionisation:
      * model: `G4MollerBhabhaModel`
  * Bremsstrahlung: (default)
* **Positron**
  * Coulomb Scattering:
    * model: `G4eSingleCoulombScatteringModel`
  * Multiple Scattering:
    * model: `G4UrbanMscModel`
  * Ionisation:
    * model: `G4MollerBhabhaModel`
  * Bremsstrahlung: (default)
  * Annihilation: (default)
* **Proton**
  * Multiple Scattering: (default)
  * Ionisation: (default)
  * Bremsstrahlung: (default)
  * Pair Production: (default)
  * Coulomb Scattering: (default)
* **Alpha**
  * Multiple Scattering: (default)
  * Ionisation: (default)
  * Nuclear Stopping: (default)
* **GenericIon**
  * Multiple Scattering: (default)