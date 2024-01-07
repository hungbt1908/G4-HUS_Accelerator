#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

class G4VisAttributes;
class G4Material;
class G4LogicalVolume;
class G4VPhysicalVolume;
class PhantomSD;
class PhantomROGeometry;
class DetectorMessenger;
class G4Box;

class DetectorConstruction : public G4VUserDetectorConstruction 
{
public:
    // methods
    DetectorConstruction();
    ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void DefineMaterial();
    void DefineAttribute();
    G4VPhysicalVolume* ConstructVolume();
    void ConstructTarget();
    void ConstructDetector();
    
public:
    // attributes

    G4bool checkOverlap = false;

    // materials 
    G4Material* Air;
    G4Material* Vacuum;
    G4Material* Si;
    G4Material* targetMat;
    G4Material* baseMat;

    // visualization
    G4VisAttributes* worldAtt;
    G4VisAttributes* phantomAtt;
    G4VisAttributes* voxelAtt;

    // volumes
    G4LogicalVolume* logicWorld;
    G4VPhysicalVolume* physWorld;
    G4Box* solidTargetFrame;
    G4LogicalVolume* logicTarget;
    G4LogicalVolume* logicBackLayer;
    G4VPhysicalVolume* physDetector;

    G4double targetXYDim, targetZThick;
    G4double baseXYDim, baseZThick;
    G4double targetAngle;
    G4double thetaMat[6] = {0.*deg, 0.*deg, 0.*deg, 0.*deg, 0.*deg, 0.*deg};
    inline const G4VPhysicalVolume* GetPhysDetector() const {return physDetector;};

public:
    // set methods
    void SetTargetXYDim(G4double);
    void SetTargetZThick(G4double);
    void SetBaseXYDim(G4double);
    void SetBaseZThick(G4double);
    void SetTargetAngleRotation(G4double);
    void SetTargetMaterial(const G4String& mat);
    void SetBaseMaterial(const G4String& mat);
    void SetDetAngle(int detID, G4double angle);
    void PrintInformation();

public:
    DetectorMessenger* pDetectorMessenger;

};

#endif
