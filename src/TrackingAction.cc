#include "TrackingAction.hh"

TrackingAction::TrackingAction(RunAction* run_action, EventAction* event_action)
{
    this->run_action = run_action;
    this->event_action = event_action;

    this->lastStepX = 0.; //in mm
}

TrackingAction::~TrackingAction()
{
}

void TrackingAction::PreUserTrackingAction(const G4Track * aTrack)
{

}

void TrackingAction::PostUserTrackingAction(const G4Track * aTrack)
{
    //if(aTrack->GetLogicalVolumeAtVertex()->GetName() == "logDiamond")
        event_action->CountTracksInEvent();
}

