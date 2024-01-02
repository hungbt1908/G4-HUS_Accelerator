#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* pDetectorConstruction)
    : G4UImessenger(),fDetectorConstruction(pDetectorConstruction)
{
    GeoDIR = new G4UIdirectory("/HUS/Geometry/");
    GeoDIR->SetGuidance("............................");

    TargetGeoDir = new G4UIdirectory("/HUS/Geometry/Target/");
    TargetGeoDir->SetGuidance("............................");

    DetectorGeoDir = new G4UIdirectory("/HUS/Geometry/Detector/");
    DetectorGeoDir->SetGuidance("............................");


    targetXYDimCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Target/SetTargetXYDim", this);
    targetXYDimCmd->SetGuidance("Set Target Size along X and Y axis.");
    targetXYDimCmd->SetParameterName("TargetXYDim", false);
    targetXYDimCmd->SetUnitCategory("Length");
    targetXYDimCmd->SetRange("TargetXYDim>0.0");
    targetXYDimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    targetZThickCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Target/SetTargetZThick", this);
    targetZThickCmd->SetGuidance("Set Target Size along Z axis.");
    targetZThickCmd->SetParameterName("TargetZThick", false);
    targetZThickCmd->SetUnitCategory("Length");
    targetZThickCmd->SetRange("TargetZThick>0.0");
    targetZThickCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    targetRotationAngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Target/SetTargetAngle", this);
    targetRotationAngleCmd->SetGuidance("Set Target Rotation Angle");
    targetRotationAngleCmd->SetParameterName("TargetAngle", false);
    targetRotationAngleCmd->SetUnitCategory("Angle");
    //targetRotationAngleCmd->SetRange("TargetAngle>0.0");
    targetRotationAngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    baseXYDimCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Target/SetBaseXYDim", this);
    baseXYDimCmd->SetGuidance("Set Base Size along X and Y axis.");
    baseXYDimCmd->SetParameterName("BaseXYDim", false);
    baseXYDimCmd->SetUnitCategory("Length");
    baseXYDimCmd->SetRange("BaseXYDim>0.0");
    baseXYDimCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    baseZThickCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Target/SetBaseZThick", this);
    baseZThickCmd->SetGuidance("Set Base Size along Z axis.");
    baseZThickCmd->SetParameterName("BaseZThick", false);
    baseZThickCmd->SetUnitCategory("Length");
    baseZThickCmd->SetRange("BaseZThick>0.0");
    baseZThickCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    targetMaterialCmd = new G4UIcmdWithAString("/HUS/Geometry/Target/SetTargetMaterial",this);
    targetMaterialCmd->SetGuidance("Select Material of the target.");
    targetMaterialCmd->SetParameterName("TargetMat", false);
    targetMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    baseMaterialCmd = new G4UIcmdWithAString("/HUS/Geometry/Target/SetBaseMaterial",this);
    baseMaterialCmd->SetGuidance("Select Material of the backlayer.");
    baseMaterialCmd->SetParameterName("BaseMat", false);
    baseMaterialCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    det0AngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Detector/SetDet0Angle", this);
    det0AngleCmd->SetGuidance("Set DET 0 Rotation Angle");
    det0AngleCmd->SetParameterName("Det0Angle", false);
    det0AngleCmd->SetUnitCategory("Angle");
    //det0AngleCmd->SetRange("Det0Angle>0.0");
    det0AngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    det1AngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Detector/SetDet1Angle", this);
    det1AngleCmd->SetGuidance("Set DET 1 Rotation Angle");
    det1AngleCmd->SetParameterName("Det1Angle", false);
    det1AngleCmd->SetUnitCategory("Angle");
    //det1AngleCmd->SetRange("Det1Angle>0.0");
    det1AngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    det2AngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Detector/SetDet2Angle", this);
    det2AngleCmd->SetGuidance("Set DET 2 Rotation Angle");
    det2AngleCmd->SetParameterName("Det2Angle", false);
    det2AngleCmd->SetUnitCategory("Angle");
    //det2AngleCmd->SetRange("Det2Angle>0.0");
    det2AngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    det3AngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Detector/SetDet3Angle", this);
    det3AngleCmd->SetGuidance("Set DET 3 Rotation Angle");
    det3AngleCmd->SetParameterName("Det3Angle", false);
    det3AngleCmd->SetUnitCategory("Angle");
    //det3AngleCmd->SetRange("Det3Angle>0.0");
    det3AngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    det4AngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Detector/SetDet4Angle", this);
    det4AngleCmd->SetGuidance("Set DET 4 Rotation Angle");
    det4AngleCmd->SetParameterName("Det4Angle", false);
    det4AngleCmd->SetUnitCategory("Angle");
    //det4AngleCmd->SetRange("Det4Angle>0.0");
    det4AngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    det5AngleCmd = new G4UIcmdWithADoubleAndUnit("/HUS/Geometry/Detector/SetDet5Angle", this);
    det5AngleCmd->SetGuidance("Set DET 5 Rotation Angle");
    det5AngleCmd->SetParameterName("Det5Angle", false);
    det5AngleCmd->SetUnitCategory("Angle");
    //det5AngleCmd->SetRange("Det5Angle>0.0");
    det5AngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);   
}

DetectorMessenger::~DetectorMessenger()
{
    delete GeoDIR;
    delete DetectorGeoDir;
    delete targetXYDimCmd;
    delete targetZThickCmd;
    delete targetRotationAngleCmd;
    delete baseXYDimCmd;
    delete baseZThickCmd;
    delete targetMaterialCmd;
    delete baseMaterialCmd;
    delete det0AngleCmd;
    delete det1AngleCmd;
    delete det2AngleCmd;
    delete det3AngleCmd;
    delete det4AngleCmd;
    delete det5AngleCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{       
    if(command == targetXYDimCmd)
    {
        fDetectorConstruction->SetTargetXYDim(targetXYDimCmd->GetNewDoubleValue(newValue));
    }

    else if(command == targetZThickCmd)
    {
        fDetectorConstruction->SetTargetZThick(targetZThickCmd->GetNewDoubleValue(newValue));
    }

    else if (command == targetRotationAngleCmd)
    {
        fDetectorConstruction->SetTargetAngleRotation(targetRotationAngleCmd->GetNewDoubleValue(newValue));
    }
    
    else if(command == baseXYDimCmd)
    {
        fDetectorConstruction->SetBaseXYDim(baseXYDimCmd->GetNewDoubleValue(newValue));
    }

    else if(command == baseZThickCmd)
    {
        fDetectorConstruction->SetBaseZThick(baseZThickCmd->GetNewDoubleValue(newValue));
    }

    else if (command == targetMaterialCmd)
    {
        fDetectorConstruction->SetTargetMaterial(newValue);
    }

    else if(command == baseMaterialCmd)
    {
        fDetectorConstruction->SetBaseMaterial(newValue);
    }

    else if(command == det0AngleCmd)
    {
        fDetectorConstruction->SetDetAngle(0, det0AngleCmd->GetNewDoubleValue(newValue));
    }

    else if(command == det1AngleCmd)
    {
        fDetectorConstruction->SetDetAngle(1, det1AngleCmd->GetNewDoubleValue(newValue));
    }

    else if(command == det2AngleCmd)
    {
        fDetectorConstruction->SetDetAngle(2, det2AngleCmd->GetNewDoubleValue(newValue));
    }

    else if(command == det3AngleCmd)
    {
        fDetectorConstruction->SetDetAngle(3, det3AngleCmd->GetNewDoubleValue(newValue));
    }

    else if(command == det4AngleCmd)
    {
        fDetectorConstruction->SetDetAngle(4, det4AngleCmd->GetNewDoubleValue(newValue));
    } 

    else if(command == det5AngleCmd)
    {
        fDetectorConstruction->SetDetAngle(5, det5AngleCmd->GetNewDoubleValue(newValue));
    }  
}