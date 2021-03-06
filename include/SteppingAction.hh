#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4StepPoint.hh"
#include "G4SteppingManager.hh"
#include "G4UserRunAction.hh"
#include "RunAction.hh"

#include "G4UserEventAction.hh"
#include "EventAction.hh"

#include "globals.hh"

#include "Histo.hh"

#include "G4ElectronIonPair.hh"
#include "G4HadronicProcess.hh"
#include "G4ParticleTypes.hh"
// #include "G4ParticleDefinition.hh"

class SteppingAction : public G4UserSteppingAction
{

public:
    SteppingAction(RunAction* run_action, EventAction* event_action);
    ~SteppingAction() {};

    RunAction* run_action;
    EventAction* event_action;

    std::map<G4ParticleDefinition*, G4int> fParticleFlag;

    void UserSteppingAction(const G4Step*);

    void registerParticle(const G4Step* aStep);
    void registerProcess(const G4Step* aStep);
    void registerNuclearChannel(G4String nuclearChannel);
};

#endif
