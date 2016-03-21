#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Run.hh"

#include "RunAction.hh"

#include "G4UserEventAction.hh"

#include "GammaProcessesCount.hh"

class G4Event;



class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* run_action, G4String info);
    ~EventAction();

    RunAction *run_act;
    G4String mode;

    G4String gammaProcessesChainInEvent;

    G4int NUM_OF_DET;

    G4int tracksInEvent;
    void CountTracksInEvent();

    G4int interactionInDiamondInEvent;
    G4int primaryGammaInteractionInDiamondInEvent;
    G4int primaryGammaInteractionInElectrodeInEvent;
    void CountInteractionInDiamondInEvent();
    void CountPrimaryGammaInteractionInDiamondInEvent();
    void CountPrimaryGammaInteractionInElectrodeInEvent();

    void CountGammaProcessInEvent(G4String processName);

    G4int registeredElectronsInDiamond;
    G4int registeredProtonsInDiamond;

public:
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
};



#endif


