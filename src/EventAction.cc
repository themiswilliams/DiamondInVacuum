#include "EventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"


EventAction::EventAction(RunAction* run_action, G4String info)
{
    this->run_act = run_action;
    this->mode = info;
}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
    NUM_OF_DET = 1;

    gammaProcessesChainInEvent = "";

    this->tracksInEvent = 0;
    this->interactionInDiamondInEvent = 0;
    this->primaryGammaInteractionInDiamondInEvent = 0;
    this->primaryGammaInteractionInElectrodeInEvent = 0;

    registeredElectronsInDiamond = 0;

    this->run_act->myDet->diamondTrackerSD[0]->clearEDEP();
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
    if(evt->GetEventID() % 100000 == 0)
    {
        G4cout << " .. EVENT .." << evt->GetEventID() << G4endl;
    }

    if(gammaProcessesChainInEvent != "")
    {
        run_act->CountProcessesChain(gammaProcessesChainInEvent);
    }
    if(tracksInEvent == 1)
        run_act->data->numOfEventsWithSingleTrack++;
    else
    {
        //if(this->interactionInDiamondInEvent > 0)
        //this->run_act->data->interactionInDiamondInEventNumber++;

        if(this->primaryGammaInteractionInDiamondInEvent > 0)
            this->run_act->data->primaryGammaInteractionInDiamondInEventNumber++;

        if(this->primaryGammaInteractionInElectrodeInEvent > 0)
            this->run_act->data->primaryGammaInteractionInElectrodeInEventNumber++;

        this->run_act->data->eDepInTracker[0] += this->run_act->myDet->diamondTrackerSD[0]->eDepInEvent;

        if(this->run_act->myDet->diamondTrackerSD[0]->eDepInEvent >= 1E-6)
        {
            this->run_act->data->interactionInDiamondInEventNumber++;
            this->run_act->data->histogramEDEP_DIAMOND_ALL->addValue(this->run_act->myDet->diamondTrackerSD[0]->eDepInEvent);
            this->run_act->data->histogramEDEP_DIAMOND_GAMMA->addValue(this->run_act->myDet->diamondTrackerSD[0]->eDepInEvent_GAMMA);
            this->run_act->data->histogramEDEP_DIAMOND_ELECTRON->addValue(this->run_act->myDet->diamondTrackerSD[0]->eDepInEvent_ELECTRON);
        }
    }
}

void EventAction::CountTracksInEvent()
{
    this->tracksInEvent++;
}
void EventAction::CountInteractionInDiamondInEvent()
{
    this->interactionInDiamondInEvent++;
}
void EventAction::CountPrimaryGammaInteractionInDiamondInEvent()
{
    this->primaryGammaInteractionInDiamondInEvent++;
}
void EventAction::CountPrimaryGammaInteractionInElectrodeInEvent()
{
    this->primaryGammaInteractionInElectrodeInEvent++;
}

void EventAction::CountGammaProcessInEvent(G4String processName)
{
    gammaProcessesChainInEvent += "-";
    gammaProcessesChainInEvent += processName;
}
