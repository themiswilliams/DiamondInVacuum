#include "SteppingAction.hh"


SteppingAction::SteppingAction(RunAction* run_action, EventAction* event_action)
{
    this->run_action = run_action;
    this->event_action = event_action;
}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
    /// COUNTERS OF PARTICLES, PROCESSES
    registerProcess(aStep);
    registerParticle(aStep);

    /// HISTOGRAM OF INITIAL ENERGIES OF INCIDENT PARTICLES
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    if ( aStep->GetTrack()->GetParentID() == 0
            && aStep->GetTrack()->GetCurrentStepNumber() == 1)
    {
        //G4cout << " VERTEX: " << aStep->GetTrack()->GetVertexKineticEnergy() << G4endl;
        this->run_action->data->histogramInitial->addValue(aStep->GetTrack()->GetVertexKineticEnergy());
        this->run_action->data->vertexEnergy = aStep->GetTrack()->GetVertexKineticEnergy();
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// INCIDENT PARTICLES
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    if ( aStep->GetTrack()->GetParentID() == 0
            && aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetName() == "logDiamond")

    {
        G4String processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

        if(processName == "phot" || processName == "compt" || processName == "conv")
        {
            //G4cout << "\n PROCESS: " << processName << G4endl;

            event_action->CountGammaProcessInEvent(processName);

            event_action->CountPrimaryGammaInteractionInDiamondInEvent();
        }

    }

    if ( aStep->GetTrack()->GetParentID() == 0
            && (aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetName() == "logGoldLayer1"
                || aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetName() == "logPlatinumLayer1"
                || aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetName() == "logTitaniumLayer1"))

    {
        G4String processName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

        if(processName == "phot" || processName == "compt" || processName == "conv")
        {
            //G4cout << "\n PROCESS: " << processName << G4endl;
            event_action->CountPrimaryGammaInteractionInElectrodeInEvent();
        }

    }

    /// ANY INTERACTIONS IN DIAMOND
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (aStep->GetTrack()->GetVolume()->GetLogicalVolume()->GetName() == "logDiamond")
    {
        const G4StepPoint* endPoint = aStep->GetPostStepPoint();
        const G4VProcess* process   = endPoint->GetProcessDefinedStep();
        // check that an real interaction occured (eg. not a transportation)
        G4StepStatus stepStatus = endPoint->GetStepStatus();
        G4bool transmit = (stepStatus==fGeomBoundary || stepStatus==fWorldBoundary);
        if (!transmit)
        {
            event_action->CountInteractionInDiamondInEvent();

            //G4cout << "\n\t---> INTERACTION in diamond: " << endPoint->GetProcessDefinedStep()->GetProcessName()
            //<< G4endl;

        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// COUNTER OF NUCLEAR CHANNELS FOR INCIDENT NEUTRONS
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (aStep->GetTrack()->GetDefinition()->GetParticleName() == "neutron"
            && aStep->GetTrack()->GetParentID() == 0)
    {
        const G4StepPoint* endPoint = aStep->GetPostStepPoint();
        const G4VProcess* process   = endPoint->GetProcessDefinedStep();

        // check that an real interaction occured (eg. not a transportation)
        G4StepStatus stepStatus = endPoint->GetStepStatus();
        G4bool transmit = (stepStatus==fGeomBoundary || stepStatus==fWorldBoundary);
        if (transmit) return;

        //energy-momentum balance initialisation
        //
        const G4StepPoint* prePoint = aStep->GetPreStepPoint();
        G4double Q             = - prePoint->GetKineticEnergy();
        G4ThreeVector Pbalance = - prePoint->GetMomentum();

        //initialisation of the nuclear channel identification
        //
        G4ParticleDefinition* particle = aStep->GetTrack()->GetDefinition();
        G4String partName = particle->GetParticleName();
        G4String nuclearChannel = "";
        G4HadronicProcess* hproc = (G4HadronicProcess*) process;
        const G4Isotope* target = hproc->GetTargetIsotope();
        G4String targetName = "...";
        if (target) targetName = target->GetName();

        nuclearChannel += aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();

        nuclearChannel += " : ";
        nuclearChannel += partName;
        nuclearChannel += " + " + targetName + " --> ";

        //scattered primary particle (if any)

        G4int ih = 1;

        if (aStep->GetTrack()->GetTrackStatus() == fAlive)
        {
            G4double energy = endPoint->GetKineticEnergy();
            G4ThreeVector momentum = endPoint->GetMomentum();
            Q        += energy;
            Pbalance += momentum;

            nuclearChannel += partName + " + ";
        }

        //secondaries
        //
        const G4TrackVector* secondary = fpSteppingManager->GetSecondary();

        for (size_t lp=0; lp<(*secondary).size(); lp++)
        {
            particle = (*secondary)[lp]->GetDefinition();
            G4String name   = particle->GetParticleName();
            G4String type   = particle->GetParticleType();
            G4double charge = particle->GetPDGCharge();
            G4double energy = (*secondary)[lp]->GetKineticEnergy();

            //energy spectrum
            if (charge > 3.)  ih = 2;

            else if (particle == G4Gamma::Gamma())       ih = 3;
            else if (particle == G4Neutron::Neutron())   ih = 4;
            else if (particle == G4Proton::Proton())     ih = 5;
            else if (particle == G4Deuteron::Deuteron()) ih = 6;
            else if (particle == G4Alpha::Alpha())       ih = 7;
            else if (type == "nucleus")                  ih = 8;
            else if (type == "meson")                    ih = 9;
            else if (type == "baryon")                   ih = 10;
            //energy-momentum balance
            G4ThreeVector momentum = (*secondary)[lp]->GetMomentum();
            Q        += energy;
            Pbalance += momentum;
            //particle flag
            fParticleFlag[particle]++;
        }

        //energy-momentum balance
        G4double Pbal = Pbalance.mag();
        ih = 11;
        ih = 12;

        // nuclear channel
        const G4int kMax = 16;
        const G4String conver[] = {"0","","2 ","3 ","4 ","5 ","6 ","7 ","8 ","9 ",
                                   "10 ","11 ","12 ","13 ","14 ","15 ","16 "
                                  };

        std::map<G4ParticleDefinition*,G4int>::iterator ip;

        for (ip = fParticleFlag.begin(); ip != fParticleFlag.end(); ip++)
        {
            particle = ip->first;
            G4String name = particle->GetParticleName();
            G4int nb = ip->second;
            if (nb > kMax) nb = kMax;
            G4String Nb = conver[nb];
            if (particle == G4Gamma::Gamma())
            {
                //fRunAction->CountGamma(nb);
                //Nb = "N ";
            }
            if (ip != fParticleFlag.begin()) nuclearChannel += " + ";
            nuclearChannel += Nb + name;
        }

        fParticleFlag.clear();

        registerNuclearChannel(nuclearChannel);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void SteppingAction::registerNuclearChannel(G4String nuclearChannel)
{
    this->run_action->CountNuclearChannels(nuclearChannel);
}

void SteppingAction::registerProcess(const G4Step* aStep)
{
    this->run_action->CountProcesses(aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName());
}

void SteppingAction::registerParticle(const G4Step* aStep)
{
    if (aStep->GetTrack()->GetCurrentStepNumber() == 1)
    {
        this->run_action->CountParticles(aStep->GetTrack()->GetDefinition()->GetParticleName());
    }
}





