#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include<globals.hh>

#include<G4VUserDetectorConstruction.hh>
#include<G4VSolid.hh>
#include<G4LogicalVolume.hh>
#include<G4VPhysicalVolume.hh>
#include<G4Material.hh>
#include<G4Element.hh>
#include<G4SubtractionSolid.hh>
#include<G4RotationMatrix.hh>

#include<G4UserLimits.hh>

#include<G4Box.hh>
#include<G4Tubs.hh>
#include<G4PVPlacement.hh>
#include<G4Transform3D.hh>
#include<G4PVReplica.hh>
#include<G4NistManager.hh>
#include<G4RunManager.hh>
#include<G4PVPlacement.hh>
#include<G4SDManager.hh>
#include<G4VisAttributes.hh>

#include<G4Mag_UsualEqRhs.hh>
#include<G4UniformMagField.hh>
#include<G4MagIntegratorStepper.hh>
#include<G4ChordFinder.hh>
#include<G4FieldManager.hh>
#include<G4ClassicalRK4.hh>

#include "TrackerSD.hh"
#include "Histo.hh"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <math.h>
#include <string>
#include <iomanip>

#include "G4UIcommand.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction(G4String, G4int, G4bool);
    ~DetectorConstruction();

    G4VPhysicalVolume* Construct();

    G4double detMaxStep;

    G4double worldLength;

    G4bool IsInVacuum;
    G4bool IsCompleteModel;
    G4bool addPCBAndElectrode;

    G4double detY;
    G4double detZ;

    G4double plateSize;

    G4double shiftOX;
    G4double positionCenterOX;

    G4double thickness_solPCBLayer1;
    G4double thickness_solGoldLayer1;
    G4double thickness_solPlatinumLayer1;
    G4double thickness_solTitaniumLayer1;
    G4double thickness_solDiamond;

    G4double diamondDensity;
    G4double diamondA;

    G4double PCBDensity;
    G4double Al2O3Density;

    G4Material* diamond;
    G4Material* vacuum;
    G4Material* air;
    G4Material* lead;
    G4Material* boron;
    G4Material* graphite;
    G4Material* aluminum;
    G4Material* water;
    G4Material* silicon;
    G4Material* copper;
    G4Material* PCB;
    G4Material* oxygen;
    G4Material* carbon;
    G4Material* hydrogen;
    G4Material* bromine;
    G4Material* gold;
    G4Material* platinum;
    G4Material* titanium;
    G4Material* Al2O3;
    G4Material* nickel;
    G4Material* beryllium;
    G4Material* CdTe;
    G4Material* GaAs;
    G4Material* iron;
    G4Material* steel;
    G4Material* kapton;
    G4Material* paraffin;
    G4Material* lithium_fluoride;
    G4Material* boron_carbide;
    G4Material* poly;
    G4Material* BerylliumSpring;
    G4Material* palladium;
    G4Material* polypropylene;
    G4Material* PET;

    G4Element* O;
    G4Element* Al;
    G4Element* C;
    G4Element* H;

    G4Material* detMat;
    G4Material* sampleMat;

    G4Box*             solWorld;
    G4LogicalVolume*   logWorld;
    G4VPhysicalVolume* physWorld;


    static const int NUM_OF_DET = 1; // a single detector in simulation

    ///////////////// DETECTOR A /////////////////

    G4Box *solPCBLayer1[NUM_OF_DET];
    G4LogicalVolume * logPCBLayer1[NUM_OF_DET];
    G4PVPlacement * physPCBLayer1[NUM_OF_DET];

    G4Box *solGoldLayer1[NUM_OF_DET];
    G4LogicalVolume * logGoldLayer1[NUM_OF_DET];
    G4PVPlacement * physGoldLayer1[NUM_OF_DET];

    G4Box *solPlatinumLayer1[NUM_OF_DET];
    G4LogicalVolume * logPlatinumLayer1[NUM_OF_DET];
    G4PVPlacement * physPlatinumLayer1[NUM_OF_DET];

    G4Box *solTitaniumLayer1[NUM_OF_DET];
    G4LogicalVolume * logTitaniumLayer1[NUM_OF_DET];
    G4PVPlacement * physTitaniumLayer1[NUM_OF_DET];

    G4Box *solDiamond[NUM_OF_DET];
    G4LogicalVolume * logDiamond[NUM_OF_DET];
    G4PVPlacement * physDiamond[NUM_OF_DET];

////////////////////////////////////////////////////

    G4VisAttributes* getColourAttribute(G4String colour);

    void buildMaterials();
    void buildDiamondModel();
    G4SDManager* SDman;
    TrackerSD* diamondTrackerSD[1];
};

#endif
