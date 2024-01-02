#ifndef DetecorMessenger_h
#define DetecorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class  G4UIcmdWithAString;
class G4UIcmdWithABool;

class DetectorMessenger: public G4UImessenger
{
public:
    DetectorMessenger(DetectorConstruction* );
    ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);
    
private:
    DetectorConstruction* fDetectorConstruction;

    G4UIdirectory* GeoDIR;
    G4UIdirectory* TargetGeoDir;
    G4UIdirectory* DetectorGeoDir;

    G4UIcmdWithADoubleAndUnit* targetXYDimCmd;
    G4UIcmdWithADoubleAndUnit* targetZThickCmd;
    G4UIcmdWithADoubleAndUnit* targetRotationAngleCmd;
    G4UIcmdWithADoubleAndUnit* baseXYDimCmd;
    G4UIcmdWithADoubleAndUnit* baseZThickCmd;

    G4UIcmdWithAString* targetMaterialCmd;
    G4UIcmdWithAString* baseMaterialCmd;

    G4UIcmdWithADoubleAndUnit* det0AngleCmd;
    G4UIcmdWithADoubleAndUnit* det1AngleCmd;
    G4UIcmdWithADoubleAndUnit* det2AngleCmd;
    G4UIcmdWithADoubleAndUnit* det3AngleCmd;
    G4UIcmdWithADoubleAndUnit* det4AngleCmd;
    G4UIcmdWithADoubleAndUnit* det5AngleCmd;

    G4UIcmdWithAString* UpdateGeometryCmd;
};



#endif
