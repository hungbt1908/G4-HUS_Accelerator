#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
// /#include "G4BestUnit.hh"
#include "G4RunManager.hh"


DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(), 
    phantomXDim(0.), phantomYDim(0.), phantomZDim(0.)
{
  	pDetectorMessenger = new DetectorMessenger(this); 

	G4NistManager* nistManager = G4NistManager::Instance();
    initTargetMaterial = nistManager->FindOrBuildMaterial("G4_B");
	initBaseMaterial = nistManager->FindOrBuildMaterial("G4_Ti");
}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  DefineMaterial();
  DefineAttribute();
  return ConstructVolume();
}

void DetectorConstruction::DefineMaterial()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  Air = nistManager->FindOrBuildMaterial("G4_AIR");
  Vacuum = nistManager-> FindOrBuildMaterial("G4_Galactic");
  B10 = new G4Material("B10", 5., 10.0129369*g/mole, 2.37*g/cm3);
  Ti = nistManager->FindOrBuildMaterial("G4_Ti");
  Si = nistManager->FindOrBuildMaterial("G4_Si");
//   Water = nistManager-> FindOrBuildMaterial("G4_WATER");

//   Mylar = nistManager->FindOrBuildMaterial("G4_MYLAR");

// 	G4Element* elW = nistManager->FindOrBuildElement("W");
// 	G4Element* elNi = nistManager->FindOrBuildElement("Ni");
// 	G4Element* elFe = nistManager->FindOrBuildElement("Fe");
// 	G4Element* elCu = nistManager->FindOrBuildElement("Cu");

//   Steel = new G4Material("Steel", 18*g/cm3, 3);
//   Steel->AddElement(elW, 95*perCent);
//   Steel->AddElement(elNi, 3.75*perCent);
//   Steel->AddElement(elFe, 1.25*perCent);

// 	JawMat = new G4Material("JawMat", 18.768*g/cm3, 4); 
// 	JawMat->AddElement(elW, 95*perCent);
// 	JawMat->AddElement(elNi, 2.8*perCent);
// 	JawMat->AddElement(elFe, 1.2*perCent);
// 	JawMat->AddElement(elCu, 1.0*perCent);

// 	G4cout << *(G4Material::GetMaterialTable());
}

void DetectorConstruction::DefineAttribute()
{
  worldAtt = new G4VisAttributes(G4Color(1.,1.,1.));
  worldAtt->SetVisibility(false);

  phantomAtt = new G4VisAttributes(G4Color(0.,0.,1.));
  phantomAtt->SetVisibility(true);

  voxelAtt = new G4VisAttributes(G4Color(0.,0.,1.));
  voxelAtt-> SetVisibility(true);
}

G4VPhysicalVolume* DetectorConstruction::ConstructVolume()
{

	// world volume
	G4Box* solidworld =  new G4Box("solidworld", 500.*cm, 500.*cm, 500.*cm);
	logicWorld =  new G4LogicalVolume(solidworld, Vacuum, "logicWorld");
	physWorld =  new G4PVPlacement(0, G4ThreeVector(), "physWorld", logicWorld, 0, false, 0);
	logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

	DefineMaterial();
	ConstructTarget();
	ConstructDetector();
	
	PrintInformation();

	return physWorld;
}

void DetectorConstruction::ConstructTarget()
{
	// frame = target + base layer
	G4double frameXYDim, frameZThick;

	if(targetXYDim >= baseXYDim) frameXYDim = targetXYDim;
	else frameXYDim = baseXYDim;

	frameZThick = targetZThick + baseZThick;

	solidTargetFrame = new G4Box("solidTargetFrame", 0.5*frameXYDim, 0.5*frameXYDim, 0.5*frameZThick);
	G4LogicalVolume* logicTargetFrame = new G4LogicalVolume(solidTargetFrame, Vacuum, "logicTargetFrame");
	G4RotationMatrix rotm = G4RotationMatrix();
	rotm.rotateY(90*deg - targetAngle);
	G4ThreeVector targetFramePos = G4ThreeVector(0,0,0);
	G4Transform3D targetFrameTrans = G4Transform3D(rotm, targetFramePos);
	new G4PVPlacement(targetFrameTrans, logicTargetFrame, "TargetFrame", logicWorld, false, 0, checkOverlap);

	// target
	G4Box* solidTarget =  new G4Box("Target", 0.5*targetXYDim, 0.5*targetXYDim, 0.5*targetZThick);
	logicTarget =  new G4LogicalVolume(solidTarget, initTargetMaterial, "Target");
  	G4double targetZPos = (-0.5*frameZThick + 0.5*targetZThick);
    new G4PVPlacement(0, G4ThreeVector(0, 0, targetZPos), logicTarget, "Target", logicTargetFrame, false, 0, checkOverlap);

	G4VisAttributes * targetColor = new G4VisAttributes(G4Colour(0.3 ,0.3 ,0.3));
	targetColor->SetVisibility(true);
	targetColor->SetForceSolid(true);
	logicTarget->SetVisAttributes(targetColor);

	// base layer
	G4Box* solidBackLayer =  new G4Box("solidBackLayer", 0.5*baseXYDim,  0.5*baseXYDim, 0.5*baseZThick);
	logicBackLayer = new G4LogicalVolume(solidBackLayer, initBaseMaterial, "logicBackLayer");
	G4double backLayerZPos = 0.5*frameZThick - 0.5*baseZThick;
	new G4PVPlacement(0, G4ThreeVector(0, 0, backLayerZPos), logicBackLayer, "BackLayer",  logicTargetFrame, false, 0, checkOverlap);

	G4VisAttributes * backLayerColor = new G4VisAttributes(G4Colour(1, 0., 0.));
	backLayerColor->SetVisibility(true);
	backLayerColor->SetForceSolid(true);
	logicBackLayer->SetVisAttributes(backLayerColor);

}

void DetectorConstruction::ConstructDetector()
{
	G4double detXDim = 10.*mm;
	G4double detYDim = 10.*mm;
	G4double detZThick = 0.30*mm;

	G4Box* solidDet = new G4Box("solidDet", 0.5*detXDim, 0.5*detYDim, 0.5*detZThick);
	G4LogicalVolume* logicDet = new G4LogicalVolume(solidDet, Si, "logicDet");

  	for(int copyNo=0; copyNo<6; copyNo++)
  	{
		G4double theta = 0.;
		G4double phi = 0.;
		G4ThreeVector uz;

		G4double detThick = 0.3*mm;
		G4double detTarDis = 6.2*cm; //Khoang cach det-target

		theta = thetaMat[copyNo];
		uz.setX(std::cos(phi)*std::sin(theta));
		uz.setY(std::sin(phi)*std::sin(theta));
		uz.setZ(std::cos(theta));

		G4ThreeVector detPos = (detTarDis + 0.5*detThick)*uz;
		G4RotationMatrix rotm = G4RotationMatrix();
		rotm.rotateY(theta);
		G4Transform3D trans = G4Transform3D(rotm, detPos);
		physDetector = new G4PVPlacement(trans, logicDet, "physDetector", 
										logicWorld, false, copyNo, checkOverlap);
  	}
}

void DetectorConstruction::SetTargetXYDim(G4double pTargetXYDim)
{
	this->targetXYDim = pTargetXYDim;
}

void DetectorConstruction::SetTargetZThick(G4double pTargetZThick)
{
	this->targetZThick = pTargetZThick;
}

void DetectorConstruction::SetBaseXYDim(G4double pBaseXYDim)
{
	this->baseXYDim = pBaseXYDim;
}

void DetectorConstruction::SetBaseZThick(G4double pBaseZThick)
{
	this->baseZThick = pBaseZThick;
}

void DetectorConstruction::SetTargetAngleRotation(G4double pTargetAngle)
{
	this->targetAngle = pTargetAngle;
}

void DetectorConstruction::SetTargetMaterial(const G4String mat)
{
	G4NistManager* nistManager = G4NistManager::Instance();
	G4Material* pttoMaterial;
	if(mat == "B10") pttoMaterial = new G4Material("B10", 5., 10.0129369*g/mole, 2.37*g/cm3);
	else if(mat == "Al-N")
	{
		G4Element* ElAl = nistManager->FindOrBuildElement(13);
		G4Element* ElN = nistManager->FindOrBuildElement(7);
		G4Material* pttoMaterial = new G4Material("AlN", 3.255 *g/cm3, 2);
    	pttoMaterial->AddElement(ElAl,1);
    	pttoMaterial->AddElement(ElN,1);
	} 
	else
	{
		G4cout << "Cannot found " << mat << " material !!!" << G4endl;
	}

	if(pttoMaterial)
	{
		if(pttoMaterial != initTargetMaterial)
		{
			initTargetMaterial = pttoMaterial;
			if(logicTarget)
			{
				logicTarget->SetMaterial(initTargetMaterial);
				G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			}
		}
	}
}

void DetectorConstruction::SetBaseMaterial(const G4String mat)
{
	G4NistManager* nistManager = G4NistManager::Instance();
	G4Material* pttoMaterial;
	if(mat == "Al")
	{
		pttoMaterial = nistManager->FindOrBuildMaterial("G4_Al");
	}
	else if(mat == "Ti")
	{
		pttoMaterial = nistManager->FindOrBuildMaterial("G4_Ti");
	} 
	else
	{
		G4cout << "Cannot found " << mat << " material !!!" << G4endl;
	}

	if(pttoMaterial)
	{
		if(pttoMaterial != initBaseMaterial)
		{
			initBaseMaterial = pttoMaterial;
			if(logicBackLayer)
			{
				logicBackLayer->SetMaterial(initBaseMaterial);
				G4RunManager::GetRunManager()->PhysicsHasBeenModified();
			}
		}
	}
}

void DetectorConstruction::SetDetAngle(int detID, G4double angle)
{
	this->thetaMat[detID] = angle;
}

void DetectorConstruction::PrintInformation()
{
	G4cout << "\n---------------------------------------------------------\n";
	G4cout << "---> The Target thickness is    " 
		   << 2.0*solidTargetFrame->GetZHalfLength()
		   << G4endl;
}

void DetectorConstruction::SetNumberOfVoxelsAlongX(int _nOfVoxelAlongX)
{
  this->nOfVoxelAlongX = _nOfVoxelAlongX;
}

void DetectorConstruction::SetNumberOfVoxelsAlongY(int _nOfVoxelAlongY)
{
  this->nOfVoxelAlongY = _nOfVoxelAlongY;
}

void DetectorConstruction::SetNumberOfVoxelsAlongZ(int _nOfVoxelAlongZ)
{
  this->nOfVoxelAlongZ = _nOfVoxelAlongZ;
}

void DetectorConstruction::SetPhantomSizeX(G4double _phantomXDim)
{
  this->phantomXDim = _phantomXDim;
}

void DetectorConstruction::SetPhantomSizeY(G4double _phantomYDim)
{
  this->phantomYDim = _phantomYDim;
}

void DetectorConstruction::SetPhantomSizeZ(G4double _phantomZDim)
{
  this->phantomZDim = _phantomZDim;
}