#ifndef TrackerSD_h
#define TrackerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "TrackerHit.hh"
#include "Histo.hh"

#include "G4StepPoint.hh"
#include "G4SteppingManager.hh"
#include "G4VProcess.hh"

class G4Step;

class TrackerSD : public G4VSensitiveDetector
{
public:
    TrackerSD(G4String);
    ~TrackerSD();

    G4String name;

    double eDepInEvent;
    double eDepInEvent_GAMMA;
    double eDepInEvent_ELECTRON;

    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void clearEDEP();
    void printEDEP(G4int numberOfEvent);
    //Histo *hist;
};


#endif

