#include"DetectorConstruction.hh"

#include"G4MultiFunctionalDetector.hh"
#include"G4VPrimitiveScorer.hh"
#include"G4PSEnergyDeposit.hh"

#include <fstream>

using namespace std;

DetectorConstruction::DetectorConstruction(G4String apertureMaterial, G4int thickness, G4bool addPCBAndElectrode)
{
    buildMaterials();

    G4cout << "\n\n ADD PCB+ELECTRODE == " << addPCBAndElectrode << G4endl;

    this->addPCBAndElectrode = addPCBAndElectrode;

    if(apertureMaterial == "vacuum")
        IsInVacuum = true;
    else
        IsInVacuum = false;

    this->thickness_solDiamond = thickness*um;
}

DetectorConstruction::~DetectorConstruction()
{}

void DetectorConstruction::buildMaterials()
{
    diamondDensity = 3.5*g/cm3;
    diamondA = 12*g/mole;

    diamond = new G4Material("diamond", 6., diamondA, diamondDensity);
    vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    air = vacuum;

    if(!(IsInVacuum))
        air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

    lead = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
    aluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
    silicon = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
    oxygen = G4NistManager::Instance()->FindOrBuildMaterial("G4_O");
    carbon = G4NistManager::Instance()->FindOrBuildMaterial("G4_C");
    hydrogen = G4NistManager::Instance()->FindOrBuildMaterial("G4_H");
    bromine = G4NistManager::Instance()->FindOrBuildMaterial("G4_Br");
    copper = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
    gold = G4NistManager::Instance()->FindOrBuildMaterial("G4_Au");
    platinum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pt");
    titanium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ti");
    nickel = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ni");
    beryllium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Be");
    palladium = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pd");
    steel = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    polypropylene = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYPROPYLENE");
    lithium_fluoride = G4NistManager::Instance()->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");
    boron_carbide = G4NistManager::Instance()->FindOrBuildMaterial("G4_BORON_CARBIDE");

    O  = new G4Element("Oxygen" ,   "O",  8.,  16.00*g/mole);
    Al = new G4Element("Aluminum" , "Al", 13., 26.98*g/mole);
    H = new G4Element("Hydrogen" ,   "O",  1.,  1.00*g/mole);
    C = new G4Element("Carbon" ,   "O",  6.,  12.00*g/mole);

    Al2O3Density = 4.00*g/cm3;
    Al2O3 = new G4Material("Al2O3", Al2O3Density, 2);
    Al2O3->AddElement(Al, (G4int)2);
    Al2O3->AddElement(O, (G4int)3);

    PCBDensity = 1.7 * g/cm3;
    PCB = new G4Material("PCB", PCBDensity, 5);
    PCB->AddMaterial(silicon, 18.0774 * perCent);
    PCB->AddMaterial(oxygen, 40.5633 * perCent);
    PCB->AddMaterial(carbon, 27.8042 * perCent);
    PCB->AddMaterial(hydrogen, 6.84428 * perCent);
    PCB->AddMaterial(bromine, 6.71091 * perCent);
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    this->detMat = diamond;

    G4cout << "\n\n::: IsInVacuum == " << IsInVacuum << G4endl;
    G4cout << "\n\n::: IsCompleteModel == " << IsCompleteModel << G4endl;

    buildDiamondModel();

    logWorld->SetVisAttributes(G4VisAttributes::Invisible);

    return physWorld;
}

G4VisAttributes* DetectorConstruction::getColourAttribute(G4String colour)
{
    if(colour == "gray")
        return new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    else if(colour == "black")
        return new G4VisAttributes(G4Colour(0.0, 0.0, 0.0));
    else if(colour == "brown")
        return new G4VisAttributes(G4Colour(0.45,0.25,0.0));
    else if(colour == "red")
        return new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    else if(colour == "green")
        return new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
    else if(colour == "blue")
        return new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
    else if(colour == "cyan")
        return new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
    else if(colour == "magenta")
        return new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
    else if(colour == "yellow")
        return new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
    else if(colour == "PCB")
        return new G4VisAttributes(G4Colour(153.0, 51.0, 0.0));

    //att = new G4VisAttributes(col);
    //att->SetVisibility(true);
    //att->SetForceSolid(true);

    return new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
}

void DetectorConstruction::buildDiamondModel()
{
    int detectorNumber = 0;

    worldLength = 10000*mm;
    plateSize = 10*mm;
    detY = 4.5*mm;
    detZ = 4.5*mm;
    thickness_solPCBLayer1 = 1600*um;
    thickness_solGoldLayer1 = 250*nm;
    thickness_solPlatinumLayer1 = 120*nm;
    thickness_solTitaniumLayer1 = 100*nm;
    thickness_solDiamond = 500*um;

    this->detMaxStep = 100*nm;

    G4VisAttributes *att = new G4VisAttributes(G4Colour(0.0,0.5,0.5,1.0));

//----------------------------- WORLD ------------------------------------------------------
    solWorld = new G4Box("world", worldLength/2, worldLength/2, worldLength/2);
    logWorld = new G4LogicalVolume(solWorld, vacuum, "logWorld", 0, 0, 0);
    physWorld = new G4PVPlacement(0, G4ThreeVector(), logWorld, "physWorld", 0, false, 0);
//------------------------------------------------------------------------------------------

    shiftOX = 1*mm;

    G4cout << "\n\n BUILDING DETECTOR." << G4endl;

    G4RotationMatrix rm;
    rm.rotateY(90*deg);

    positionCenterOX = shiftOX;

    if(addPCBAndElectrode)
    {
//----------------------------- PCB LAYER 1 -----------------------------------------------

        positionCenterOX += thickness_solPCBLayer1/2;

        G4cout << "PCB LAYER 1, positionCenterOX = " << positionCenterOX/nm << " nm, location == [ "
        << (positionCenterOX - thickness_solPCBLayer1/2)/nm << "  ..  "
        << (positionCenterOX + thickness_solPCBLayer1/2)/nm << " ] nm" << G4endl;
        solPCBLayer1[detectorNumber] = new G4Box("solPCBLayer1", plateSize/2, plateSize/2, thickness_solPCBLayer1/2);
        logPCBLayer1[detectorNumber] = new G4LogicalVolume(solPCBLayer1[detectorNumber], PCB, "logPCBLayer1");
        physPCBLayer1[detectorNumber] = new G4PVPlacement(G4Transform3D(rm, G4ThreeVector(positionCenterOX, 0, 0)),
                logPCBLayer1[detectorNumber], "physPCBLayer1", logWorld, false, 0);

        logPCBLayer1[detectorNumber]->SetVisAttributes(getColourAttribute("brown"));

        positionCenterOX += thickness_solPCBLayer1/2;

//------------------------------------------------------------------------------------------


//----------------------------- GOLD LAYER 1 -----------------------------------------------

        positionCenterOX += thickness_solGoldLayer1/2;

        G4cout << "GOLD LAYER 3, positionCenterOX = " << positionCenterOX/nm << " nm, location == [ "
        << (positionCenterOX - thickness_solGoldLayer1/2)/nm << "  ..  "
        << (positionCenterOX + thickness_solGoldLayer1/2)/nm << " ] nm" << G4endl;
        solGoldLayer1[detectorNumber] = new G4Box("solGoldLayer1", plateSize/2, plateSize/2, thickness_solGoldLayer1/2);
        logGoldLayer1[detectorNumber] = new G4LogicalVolume(solGoldLayer1[detectorNumber], gold, "logGoldLayer1");
        physGoldLayer1[detectorNumber] = new G4PVPlacement(G4Transform3D(rm, G4ThreeVector(positionCenterOX, 0, 0)),
                logGoldLayer1[detectorNumber], "physGoldLayer1", logWorld, false, 0);

        logGoldLayer1[detectorNumber]->SetVisAttributes(getColourAttribute("yellow"));

        positionCenterOX += thickness_solGoldLayer1/2;

//------------------------------------------------------------------------------------------

//----------------------------- PLATINUM LAYER 1 -----------------------------------------------

        positionCenterOX += thickness_solPlatinumLayer1/2;

        G4cout << "PLATINUM LAYER 1, positionCenterOX = " << positionCenterOX/nm << " nm, location == [ "
        << (positionCenterOX - thickness_solPlatinumLayer1/2)/nm << "  ..  "
        << (positionCenterOX + thickness_solPlatinumLayer1/2)/nm << " ] nm" << G4endl;
        solPlatinumLayer1[detectorNumber] = new G4Box("solPlatinumLayer1", plateSize/2, plateSize/2, thickness_solPlatinumLayer1/2);
        logPlatinumLayer1[detectorNumber] = new G4LogicalVolume(solPlatinumLayer1[detectorNumber], platinum, "logPlatinumLayer1");
        physPlatinumLayer1[detectorNumber] = new G4PVPlacement(G4Transform3D(rm, G4ThreeVector(positionCenterOX, 0, 0)),
                logPlatinumLayer1[detectorNumber], "physPlatinumLayer1", logWorld, false, 0);

        logPlatinumLayer1[detectorNumber]->SetVisAttributes(getColourAttribute("yellow"));

        positionCenterOX += thickness_solPlatinumLayer1/2;

//------------------------------------------------------------------------------------------

//----------------------------- TITANIUM LAYER 1 -----------------------------------------------

        positionCenterOX += thickness_solTitaniumLayer1/2;

        G4cout << "TITANIUM LAYER 1, positionCenterOX = " << positionCenterOX/nm << " nm, location == [ "
        << (positionCenterOX - thickness_solTitaniumLayer1/2)/nm << "  ..  "
        << (positionCenterOX + thickness_solTitaniumLayer1/2)/nm << " ] nm" << G4endl;
        solTitaniumLayer1[detectorNumber] = new G4Box("solTitaniumLayer1", plateSize/2, plateSize/2, thickness_solTitaniumLayer1/2);
        logTitaniumLayer1[detectorNumber] = new G4LogicalVolume(solTitaniumLayer1[detectorNumber], titanium, "logTitaniumLayer1");
        physTitaniumLayer1[detectorNumber] = new G4PVPlacement(G4Transform3D(rm, G4ThreeVector(positionCenterOX, 0, 0)),
                logTitaniumLayer1[detectorNumber], "physTitaniumLayer1", logWorld, false, 0);

        logTitaniumLayer1[detectorNumber]->SetVisAttributes(getColourAttribute("yellow"));

        positionCenterOX += thickness_solTitaniumLayer1/2;

//------------------------------------------------------------------------------------------
    }

//----------------------------- DIAMOND -----------------------------------------------

    positionCenterOX += thickness_solDiamond/2;

    G4cout << "SENSOR, material = " << detMat->GetName() << G4endl;

    G4cout << "SENSOR, positionCenterOX = " << positionCenterOX/nm << " nm, location == [ "
    << (positionCenterOX - thickness_solDiamond/2)/nm << "  ..  "
    << (positionCenterOX + thickness_solDiamond/2)/nm << " ] nm" << G4endl;

    solDiamond[detectorNumber] = new G4Box("solDiamond", detY/2, detZ/2, thickness_solDiamond/2);
    logDiamond[detectorNumber] = new G4LogicalVolume(solDiamond[detectorNumber], detMat, "logDiamond");
    physDiamond[detectorNumber] = new G4PVPlacement(G4Transform3D(rm, G4ThreeVector(positionCenterOX, 0, 0)),
            logDiamond[detectorNumber], "physDiamond", logWorld, false, 0);

    logDiamond[detectorNumber]->SetVisAttributes(getColourAttribute("white"));

    SDman = G4SDManager::GetSDMpointer();
    diamondTrackerSD[0] = new TrackerSD("diamondTrackerSD");
    SDman->AddNewDetector(diamondTrackerSD[0]);
    logDiamond[0]->SetSensitiveDetector(diamondTrackerSD[0]);

    positionCenterOX += thickness_solDiamond/2;

//------------------------------------------------------------------------------------------
}
