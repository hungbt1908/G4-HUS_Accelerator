
# ⭐ About this project
This project was implemented to simulate a proton experiment at HUS.
## 1. Geometry
* This simulation included 2 main geometrical components:
    * Target and base layer
    * 6 detectors
* This application allows you to set-up geometry of target and base layer by using below commands:
    ```
    /HUS/Geometry/Target/SetTargetXYDim 1. cm
    /HUS/Geometry/Target/SetTargetZThick 200. nm 
    /HUS/Geometry/Target/SetTargetAngle 60. deg
    /HUS/Geometry/Target/SetBaseXYDim 1. cm
    /HUS/Geometry/Target/SetBaseZThick 150. nm
    /HUS/Geometry/Target/SetTargetMaterial B10
    /HUS/Geometry/Target/SetBaseMaterial Al
    ```
* You can set position of 6 detector by using following commands in macro file:
    ```
    /HUS/Geometry/Detector/SetDet0Angle  40. deg
    /HUS/Geometry/Detector/SetDet1Angle  70. deg
    /HUS/Geometry/Detector/SetDet2Angle  150. deg
    /HUS/Geometry/Detector/SetDet3Angle  233. deg
    /HUS/Geometry/Detector/SetDet4Angle  270 deg
    /HUS/Geometry/Detector/SetDet5Angle  330. deg
    ```
* The target's material and base layer's material can be chose by using commands:

  ```bash
  # targetaMat = [B, B10, B11, Au197, Al-N]
  /HUS/Geometry/Target/SetTargetMaterial B10

  # baseMat = [Al, Ti, C3H6N6, C3H6O2]
  /HUS/Geometry/Target/SetBaseMaterial Al
  ```

* Geometry image:

![](./images/Capture.PNG)

## 2. Physics
There are 2 main physic components:
### Hadronic components
#### **Photon**
  * Photon Nuclear:
    * process: `G4HadronInelasticProcess("photonNuclear")`
    * cross section: `G4GammaNuclearXS()`
    * model: 
      * `G4LowEGammaNuclearModel` in range 0 - 200 MeV
      * `G4CascadeInterface` in range 199 MeV - 10 GeV
#### **Proton**
  * Elastic: 
    * process: `G4HadronElasticProcess("hadElastic")`
    * cross section: `G4BGGNucleonElasticXS(G4Proton::Definition())`
    * model: `G4ChipsElasticModel`
  * Inelastic:
    * process: `G4HadronInelasticProcess("protonInelastic")`
    * cross section: `G4ParticleInelasticXS(G4Proton::Definition())`
    * model: `G4CascadeInterface` in range 0 - 6 GeV
#### **Alpha**
  * Elastic:
    * process: `G4HadronElasticProcess("hadElastic")`
    * cross section: `G4CrossSectionElastic(new G4ComponentGGNuclNuclXsc())` in range 0 - 25.6 PeV
    * model: `G4HadronElastic("hElasticLHEP")` in range 0 - 6 GeV
  * Inelastic:
    * process: `G4HadronInelasticProcess("AlphaInelastic")`
    * cross section: `G4CrossSectionInelastic(new G4ComponentGGNuclNuclXsc())` in range 0 - 25.6 PeV
    * model: `G4BinaryLightIonReaction` in range 0 - 6 GeV
#### **He3**
  * Elastic:
    * process: `G4HadronElasticProcess("hadElastic")`
    * cross section: `G4CrossSectionElastic(new G4ComponentGGNuclNuclXsc())` in range 0 - 25.6 PeV
    * model: `G4HadronElastic("hElasticLHEP")` in range 0 - 100 TeV
#### **Neutron**
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
### EM components
#### **Photon**
  * Rayleigh Scattering: 
    * model: `G4LivermoreRayleighModel`
  * Photoelectric:
    * model: `G4LivermorePhotoElectricModel`
  * Compton Scattering:
    * model: `G4KleinNishinaModel`
  * Gamma Conversion:
    * model: `G4BetheHeitlerModel`
#### **Electron**
  * Coulomb Scattering:
    * model: `G4eSingleCoulombScatteringModel`
  * Multiple Scattering:
    * model: `G4GoudsmitSaundersonMscModel`
  * Ionisation:
      * model: `G4MollerBhabhaModel`
  * Bremsstrahlung: (default)
#### **Positron**
  * Coulomb Scattering:
    * model: `G4eSingleCoulombScatteringModel`
  * Multiple Scattering:
    * model: `G4UrbanMscModel`
  * Ionisation:
    * model: `G4MollerBhabhaModel`
  * Bremsstrahlung: (default)
  * Annihilation: (default)
#### **Proton**
  * Multiple Scattering: (default)
  * Ionisation: (default)
  * Bremsstrahlung: (default)
  * Pair Production: (default)
  * Coulomb Scattering: (default)
#### **Alpha**
  * Multiple Scattering: (default)
  * Ionisation: (default)
  * Nuclear Stopping: (default)
#### **GenericIon**
  * Multiple Scattering: (default)

To decrease uncertainties of simulation process, we added the scaling cross section function for nucleons (proton and neutron) in the PhysicsList class:
  ```c++
  // Scaling the nucleon cross sections
  G4HadronicParameters::Instance()->SetApplyFactorXS(true);
  G4HadronicParameters::Instance()->SetXSFactorNucleonElastic(1.50);
  G4HadronicParameters::Instance()->SetXSFactorNucleonInelastic(1.50);
```

# 🔧 Requirements
  * Ubuntu 20.04/Window 10
  * Geant4.11.1.2
  * Root data analysis framework

# 🏃‍♂️ How to run
* Run example in interactive mode
    ```c++
    ./sim
    ```
* Run example in batch mode
    ```c++
    ./sim run.mac
    ```
# 🚀 About Me
**Bùi Tiến Hưng** - *hungbt1908@gmail.com*
1. Nuclear Engineering Lab, Hanoi University of Science and Technology (HUST).
2. Vietnam Atomic Energy Institute (VINATOM).

**Interestet Fields:**
* Medical physics, 
* Monte-Carlo simulation, 
* Machine learning.