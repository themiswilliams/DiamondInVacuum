#include "TrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "math.h"


TrackerSD::TrackerSD(G4String name)
    :G4VSensitiveDetector(name)
{
    //this->hist = hist;
    this->name = name;
    clearEDEP();
}

TrackerSD::~TrackerSD() { }

void TrackerSD::clearEDEP()
{
    eDepInEvent = 0.0;
    eDepInEvent_ELECTRON = 0.0;
    eDepInEvent_GAMMA = 0.0;
}

void TrackerSD::printEDEP(G4int numberOfEvent)
{
    G4cout << " .. EVENT " << numberOfEvent << ", eDep == " << eDepInEvent/eV << " eV" << G4endl;
}

G4bool TrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    eDepInEvent += aStep->GetTotalEnergyDeposit()/eV;

    if (aStep->GetTrack()->GetDefinition()->GetParticleName() == "e-")
    {
        eDepInEvent_ELECTRON += aStep->GetTotalEnergyDeposit()/eV;
    }

    if (aStep->GetTrack()->GetDefinition()->GetParticleName() == "gamma")
    {
        eDepInEvent_GAMMA += aStep->GetTotalEnergyDeposit()/eV;
    }

    return true;
}


