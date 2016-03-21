#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <stdlib.h>

#include "RunAction.hh"
#include "EventAction.hh"

#include "G4UserTrackingAction.hh"
#include "G4ThreeVector.hh"

#include "G4TrackingManager.hh"

#include "G4Track.hh"

class G4Track;

typedef std::vector< G4Track* > G4TrackVector;

class TrackingAction : public G4UserTrackingAction
{
  public:
    TrackingAction(RunAction*, EventAction*);
    ~TrackingAction();

  public:
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

    public:
    RunAction* run_action;
    EventAction* event_action;

    G4double lastStepX;
};

#endif





