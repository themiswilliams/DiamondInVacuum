#include "RunAction.hh"
#include <iomanip>

RunAction::RunAction(DetectorConstruction* myDet, G4String info, PrimaryGeneratorAction* primGen, G4String name, G4String namePrefix, G4String histo, G4String calculator)
{
    G4double histogramScaling = 1.;

    if(calculator == "calc")
        this->calculator = true;
    else
        this->calculator = false;

    if(histo == "keV")
        histogramScaling = 1E3;
    else if(histo == "MeV")
        histogramScaling = 1E6;

    this->data = new Data(histogramScaling);
    this->myDet = myDet;
    this->myPrimGen = primGen;
    this->name = name;
    this->namePrefix = namePrefix;
    this->gcalc = new G4EmCalculator();

    this->eDepValues = new std::vector<G4double>;
}

RunAction::~RunAction()
{
    delete data;
    delete eDepValues;
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4int thisRunID = aRun->GetRunID();
    G4cout << "### Run " << thisRunID << " start." << G4endl;

    this->data->initialize();

    this->data->currentRunID = thisRunID;

    eDepValues->clear();
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    G4int thisRunID = aRun->GetRunID();
    G4double numOfEvents = 1.*(aRun->GetNumberOfEventToBeProcessed());

    G4cout << "EndOfRunAction: end of run # " << thisRunID << G4endl;
    G4cout << "Number of events: " << numOfEvents << G4endl;

    //G4double mean_edep = (data->eDepInTracker[0]) / (data->primaryGammaInteractionInDiamondInEventNumber);
    G4double mean_edep = (data->eDepInTracker[0]) / data->interactionInDiamondInEventNumber;
    G4double probability = data->interactionInDiamondInEventNumber / numOfEvents;
    G4double probabilityElectrode = data->primaryGammaInteractionInElectrodeInEventNumber / numOfEvents;

    if(!calculator)
    {
        saveEnergyDepositInTracker(thisRunID, data->vertexEnergy/eV, probability, mean_edep, probabilityElectrode, numOfEvents);

        data->setName(name, namePrefix);
        //data->saveHistograms(thisRunID);

        //saveStats(thisRunID);
    }
    //else
    //saveGCalcTables();
}

void  RunAction::saveEnergyDepositInTracker(G4int runID, G4double vertexEnergy,
        G4double probability, G4double eDep, G4double probabilityElectrode, G4int numOfEvents)
{
    std::ofstream output;
    G4String fname;

    fname = namePrefix;
    fname += ".";
    fname += name;
    fname += ".numOfVertices-vertexEnergy-Probability-Edep.txt";
    output.open(fname, std::ios::app);

    output << numOfEvents << " , " << vertexEnergy << " , " << probability << " , " << eDep << "\n";

    output.flush();
    output.close();
}


void  RunAction::saveStats(G4int runID)
{
    std::fstream myfile;

    G4String fileName;
    G4String s;

    fileName = namePrefix;
    fileName += ".";
    fileName += name;
    fileName += ".STATS.RUN-";
    s = G4UIcommand::ConvertToString(runID);
    fileName += s;
    fileName += ".txt";

    myfile.open(fileName, std::fstream::out);

    myfile << "\n ******************************************************" << "\n";
    myfile << "\nNumber of particles:" << "\n\n";
    for (size_t i = 0; i < data->part_count->size(); i++)
    {
        myfile << (*(data->part_count))[i]->GetName() << "  " << (*(data->part_count))[i]->GetCounter() << "\n";
    }

    myfile << "\n\n ******************************************************" << "\n";

    myfile << "Number of processes:" << "\n\n";
    for (size_t i = 0; i < data->proc_count->size(); i++)
    {
        myfile << (*(data->proc_count))[i]->GetName() << "  ...  " << (*(data->proc_count))[i]->GetCounter() << "\n";
    }

    myfile << "\n ******************************************************" << "\n";

    myfile << "NUCLEAR CHANNELS:" << "\n\n";
    for (size_t i = 0; i < data->nuc_count->size(); i++)
    {
        myfile << (*(data->nuc_count))[i]->GetName() << "  ...  " << (*(data->nuc_count))[i]->GetCounter() << "\n\n";
    }

    myfile << "\n ******************************************************" << "\n";

    myfile << "DIAMOND ALL EDEP HISTOGRAM (STEP " << this->data->histogramEDEP_DIAMOND_ALL->scaling << " eV) \n\n";
    myfile << "MEAN == " << this->data->histogramEDEP_DIAMOND_ALL->getMean() << "\n\n";
    myfile << "RMS == " << this->data->histogramEDEP_DIAMOND_ALL->getRMS() << "\n\n";


    myfile << "\n ******************************************************" << "\n";

    myfile << "GAMMA PROCESSES CHAIN:" << "\n\n";
    for (size_t i = 0; i < data->processes_chain_count->size(); i++)
    {
        myfile << (*(data->processes_chain_count))[i]->GetName() << "  ...  " << (*(data->processes_chain_count))[i]->GetCounter() << "\n\n";
    }

    myfile << "\n ******************************************************" << "\n";

    myfile.close();
}

void RunAction::saveInteractionRateInDiamond(G4int runID, G4double rate)
{
    std::ofstream output;
    G4String name;

    name = "interaction-rate-in-diamond.txt";
    output.open(name, std::ios::app);

    output << runID << " , " << rate << "\n";

    output.flush();
    output.close();

}

void RunAction::CountNuclearChannels(G4String nuclearChannel)
{
    size_t nbProc = data->nuc_count->size();

    size_t i = 0;

    while ( (i < nbProc) && ((*(data->nuc_count))[i]->GetName() != nuclearChannel) ) i++;

    if (i == nbProc)
        data->nuc_count->push_back(new OneProcessCount(nuclearChannel));

    (*(data->nuc_count))[i]->Count();
}

void RunAction::CountProcesses(G4String procName)
{
    size_t nbProc = data->proc_count->size();

    size_t i = 0;

    while ( (i < nbProc) && ((*(data->proc_count))[i]->GetName() != procName) ) i++;

    if (i == nbProc)
        data->proc_count->push_back(new OneProcessCount(procName));

    (*(data->proc_count))[i]->Count();
}

void RunAction::CountProcessesChain(G4String gammaProcessesChainInEvent)
{
    size_t nbPart = data->processes_chain_count->size();

    size_t i = 0;

    while ( (i < nbPart) && ((*(data->processes_chain_count))[i]->GetName() != gammaProcessesChainInEvent) ) i++;

    if (i == nbPart) data->processes_chain_count->push_back(new OneProcessCount(gammaProcessesChainInEvent));

    (*(data->processes_chain_count))[i]->Count();
}

void RunAction::CountParticles(G4String partName)
{
    size_t nbPart = data->part_count->size();

    size_t i = 0;

    while ( (i < nbPart) && ((*(data->part_count))[i]->GetName() != partName) ) i++;

    if (i == nbPart) data->part_count->push_back(new OneParticleCount(partName));

    (*(data->part_count))[i]->Count();
}

void RunAction::saveGCalcTables()
{
    std::ofstream output;

    G4String name;
    G4double energy;

    //G4ParticleDefinition* part = G4Gamma::ProtonDefinition();

    G4ParticleDefinition* part = myPrimGen->particleSource->GetParticleDefinition();

    //G4ParticleDefinition* part = G4Gamma::ProtonDefinition();
    //G4ParticleDefinition* part = G4Alpha::AlphaDefinition();
    //G4ParticleDefinition* part = G4Deuteron::DeuteronDefinition();
    //G4ParticleDefinition* part = G4Triton::TritonDefinition();
    //G4ParticleDefinition* part = G4He3::He3Definition();

    G4Material* mat = myDet->sampleMat;

    name = part->GetParticleName();
    name += "-";
    name += mat->GetName();
    name += ".txt";


    G4cout << "\n\n\n ******* EM CALCULATOR ********* \n\n"
    << myPrimGen->particleSource->GetParticleDefinition()->GetParticleName()
    << "\n PDG Charge == " << myPrimGen->particleSource->GetParticleDefinition()->GetPDGCharge()
    << "\n Atomic Mass == " << myPrimGen->particleSource->GetParticleDefinition()->GetAtomicMass()
    << "\n Atomic Number == " << myPrimGen->particleSource->GetParticleDefinition()->GetAtomicNumber()
    << "\n\nMATERIAL: " << mat->GetName()
    << "\n\n\n *********************** \n\n" << G4endl;

    output.open(name, std::ios::out);

    G4double DEDX = 0.;
    G4double RANGE = 0.;

    G4String procName1 = "phot";
    G4String procName2 = "compt";
    G4String procName3 = "conv";

    G4double matDensity = mat->GetDensity();

    G4double coeff = 0.;

    G4double massSigma = 0.;

    output << "\n";

    G4ProcessVector* plist = part->GetProcessManager()->GetProcessList();
    G4String procName;
    std::vector<G4String> emName;
    size_t length = plist->size();
    for (size_t j=0; j<length; j++)
    {
        procName = (*plist)[j]->GetProcessName();
        if (((*plist)[j]->GetProcessType() == fElectromagnetic) &&
                (procName != "msc"))
        {
            emName.push_back(procName);
        }
    }

    // print list of processes
    G4cout << "\n  processes :                ";
    for (size_t j=0; j<emName.size(); j++)
        G4cout << "\t" << std::setw(13) << emName[j] << "\t";
    G4cout << "\t" << std::setw(13) <<"total";

    if(part->GetPDGCharge() > 1e-6 || part->GetPDGCharge() < -1e-6 )
    {
        G4cout << "Creating Ionisation Model . . . " << G4endl;
        G4VEmModel * ioni = new G4IonParametrisedLossModel(part);

        G4cout << "Creating Ionisation Model . . . " << G4endl;
        G4VEmModel * bb = new G4BetheBlochModel(part);

        G4cout << "Creating Ionisation Model . . . " << G4endl;
        G4VEmModel * bragg = new G4BraggModel(part);

        G4cout << "Creating Ionisation Model . . . " << G4endl;
        G4VEmModel * braggion = new G4BraggIonModel(part);

        G4double Ecut = 100*m;

        G4cout << "Starting a loop . . . " << G4endl;

        for(G4int i = 9; i < 10; i=i+3)
        {
            coeff = std::pow((double)10, (double)i);

            G4int start = 1;
            G4int end = i > 9 ? 99 : 999;

            for (G4int k = start; k <= end; ++k)
            {
                energy = coeff*k*eV;


                //G4cout << "ComputeDEDXPerVolume: " << mat->GetName()
                //<< " , " << myPrimGen->particleSource->GetParticleDefinition()->GetParticleName()
                //<< G4endl;

                //DEDX = ioni->ComputeDEDXPerVolume(mat, part, energy, Ecut);
                RANGE = gcalc->GetRange(energy, part, mat);

                output << "\n ---" << "\n";
                output << energy/eV << "\n";

                DEDX = gcalc->GetDEDX(energy, part, mat);
                output << DEDX << "\n" ;
                DEDX = gcalc->ComputeDEDX(energy, part, "hIoni", mat, Ecut);
                output << DEDX << "\n" ;
                DEDX = gcalc->ComputeElectronicDEDX(energy, part, mat, Ecut);
                output << DEDX << "\n" ;
                DEDX = gcalc->ComputeNuclearDEDX(energy, part, mat);
                output << DEDX << "\n" ;
                DEDX = gcalc->ComputeTotalDEDX(energy, part, mat, Ecut);
                output << DEDX << "\n" ;

                DEDX = ioni->ComputeDEDXPerVolume(mat, part, energy, Ecut);
                output << DEDX << "\n" ;

                DEDX = bb->ComputeDEDXPerVolume(mat, part, energy, Ecut);
                output << DEDX << "\n" ;

                DEDX = bragg->ComputeDEDXPerVolume(mat, part, energy, Ecut);
                output << DEDX << "\n" ;

                DEDX = braggion->ComputeDEDXPerVolume(mat, part, energy, Ecut);
                output << DEDX << "\n" ;


                output << RANGE << "\n";
            }
        }
    }
    else
    {
        /*
                for(G4int i = 3; i < 13; i=i+3)
                {
                    coeff = std::pow((double)10, (double)i);

                    G4int start = 1;
                    G4int end = i > 9 ? 99 : 999;

                    for (G4int k = start; k <= end; ++k)
                    {
                        energy = coeff*k*eV;

                        output << energy/eV << "\n";
                        massSigma = gcalc->ComputeCrossSectionPerVolume(energy, part, procName1, mat);
                        //output << G4BestUnit(massSigma/matDensity, "Surface/Mass") << "\n";
                        output << (massSigma/matDensity)/(cm2/g) << "\n";
                        massSigma = gcalc->ComputeCrossSectionPerVolume(energy, part, procName2, mat);
                        //output << G4BestUnit(massSigma/matDensity, "Surface/Mass") << "\n";
                        output << (massSigma/matDensity)/(cm2/g) << "\n";
                        massSigma = gcalc->ComputeCrossSectionPerVolume(energy, part, procName3, mat);
                        //output << G4BestUnit(massSigma/matDensity, "Surface/Mass") << "\n";
                        output << (massSigma/matDensity)/(cm2/g) << "\n";;


                    }
                }
        */

        for(G4int i = 1; i <= 1000; i++)
        {
            energy = i*1000*eV;

            output << energy/eV << ", ";

            massSigma = gcalc->ComputeCrossSectionPerVolume(energy, part, procName1, mat);
            //output << G4BestUnit(massSigma/matDensity, "Surface/Mass") << "\n";
            output << (massSigma/matDensity)/(cm2/g) << ", ";
            massSigma = gcalc->ComputeCrossSectionPerVolume(energy, part, procName2, mat);
            //output << G4BestUnit(massSigma/matDensity, "Surface/Mass") << "\n";
            output << (massSigma/matDensity)/(cm2/g) << ", ";
            massSigma = gcalc->ComputeCrossSectionPerVolume(energy, part, procName3, mat);
            //output << G4BestUnit(massSigma/matDensity, "Surface/Mass") << "\n";
            output << (massSigma/matDensity)/(cm2/g) << "\n";
        }

    }


    output.flush();
    output.close();
}





