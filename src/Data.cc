#include "Data.hh"

#include <fstream>
#include <iomanip>
#include "G4UIcommand.hh"

HistogramEDEP::HistogramEDEP(G4String name, G4double scaling)
{
    this->scaling = scaling;

    for(G4int i = 0; i < SIZE; i++)
    {
        dataArray[i] = 0.0;
    }

    this->label = name;
}

HistogramEDEP::~HistogramEDEP() {}

void HistogramEDEP::initialize()
{
    for(G4int i = 0; i < SIZE; i++)
    {
        dataArray[i] = 0.0;
    }
}

void HistogramEDEP::addValue(G4double val)
{
    /// val in eV

    if(val < 1E-7)
        return;

    G4double bin = val / (this->scaling);

    if(bin < 0)
    {
        //G4cout << "HistogramEDEP " << label << " : ERROR! VAL EV == " << val << " < 0 !" << G4endl;
    }
    else if(bin > SIZE)
    {
        //G4cout << "HistogramEDEP " << label << " : ERROR! VAL EV == " << val << " > SIZE ! " << G4endl;
    }
    else dataArray[static_cast<G4int>(round(bin))]++;
}

void HistogramEDEP::printHisto()
{

    G4cout << "\n\nHistogramEDEP " << label << " (step 1 keV):\n" << G4endl;

    for(G4int i = 0; i < SIZE; i++)
    {
        if(dataArray[i] > 1E-6)
            G4cout << i << "  ..  " << dataArray[i] << G4endl;
    }
}

void HistogramEDEP::saveHisto(G4int runID)
{
    std::fstream output;
    G4String name;
    G4String s = G4UIcommand::ConvertToString(runID);

    G4String histoStepString = "EV";

    if(this->scaling > 1E4)
        histoStepString = "MEV";
    else if (this->scaling < 1E4 && this->scaling > 1E2)
        histoStepString = "KEV";

    name = prefixName;
    name += ".";
    name += runName;
    name += ".EDEP.1";
    name += histoStepString;
    name += ".HISTOGRAM.";
    name += this->label;
    name += ".RUN-";
    name += s;
    name += ".txt";

    G4cout << "\n SAVING .. " << name << G4endl;

    output.open(name, std::fstream::out);

    int numberOfHits = 0;

    for(G4int i = 0; i < SIZE; i++)
    {
        numberOfHits += dataArray[i];
    }

    output << numberOfHits << "\n";
    G4cout << this->label << " .. number of hits == " << numberOfHits << G4endl;

    for(G4int i = 0; i < SIZE; i++)
    {
        //if(dataArray[i] > 1E-3)
        output << i << "," << dataArray[i] << "\n";
    }

    output.flush();
    output.close();
}

void HistogramEDEP::setName(G4String name, G4String namePrefix)
{
    this->runName = name;
    this->prefixName = namePrefix;
}

double HistogramEDEP::getMean()
{
    double mean = 0.0, sum = 0.0;
    int counter = 0;

    for(G4int i = 0; i < SIZE; i++)
    {
        if(dataArray[i] > 1E-3)
        {
            mean += dataArray[i] * i;
            sum += dataArray[i];
            counter++;
        }
    }

    if(sum > 1E-6)
        mean /= sum;

    return mean;
}

double HistogramEDEP::getRMS()
{
    double mean = 0.0, meanSquare = 0.0, sum = 0.0, var = 0.0;
    int counter = 0;

    for(G4int i = 0; i < SIZE; i++)
    {
        if(dataArray[i] > 1E-3)
        {
            mean += dataArray[i] * i;
            sum += dataArray[i];
            counter++;
        }
    }

    if(sum > 1E-6)
        mean /= sum;

    sum = 0.0;
    counter = 0;

    for(G4int i = 0; i < SIZE; i++)
    {
        if(dataArray[i] > 1E-3)
        {
            meanSquare += dataArray[i] * i * i;
            sum += dataArray[i];
            counter++;
        }
    }

    if(sum > 1E-6)
        meanSquare /= sum;

    var = fabs(meanSquare - mean*mean);

    return sqrt(var);
}

Data::Data(G4double scaling)
{

    histoScaling = scaling;

    currentRunID = 0;

    histogramEDEP_DIAMOND_ALL = new HistogramEDEP("DiamondAll", histoScaling);
    histogramEDEP_DIAMOND_GAMMA = new HistogramEDEP("DiamondGamma", histoScaling);
    histogramEDEP_DIAMOND_ELECTRON = new HistogramEDEP("DiamondElectron", histoScaling);
    histogramInitial = new HistogramEDEP("Init", histoScaling);

    eDepInTracker[0] = 0.;

    vertexEnergy = 0.;

    numOfEventsWithSingleTrack = 0;
    numOfPrimariesInDiamond = 0;

    interactionInDiamondInEventNumber = 0;
    primaryGammaInteractionInDiamondInEventNumber = 0;
    primaryGammaInteractionInElectrodeInEventNumber = 0;

    primaryInDiamondInEvent = false;

    processChainOfEvent = "";

    proc_count = 0;
    nuc_count = 0;
    part_count = 0;
    vertices_count = 0;
    volumes_count = 0;
    gamma_origins_count = 0;
    processes_chain_count = 0;
    secondaries_spectra = 0;
}

Data::~Data()
{
    delete histogramEDEP_DIAMOND_ALL;
    delete histogramEDEP_DIAMOND_GAMMA;
    delete histogramEDEP_DIAMOND_ELECTRON;
    delete histogramInitial;

    deleteProcessCounter();
    deleteNuclearChannelCounter();
    deleteParticleCounter();
    //deleteVerticesCounter();
    //deleteVolumesCounter();
    //deleteGammaOriginsCounter();
    //deleteProcessesChainCounter();
    //deleteSecondariesSpectra();

    G4cout << "Deleting Data object . . . " << G4endl;
}

void Data::initialize()
{
    histogramEDEP_DIAMOND_ALL->initialize();
    histogramEDEP_DIAMOND_GAMMA->initialize();
    histogramEDEP_DIAMOND_ELECTRON->initialize();
    histogramInitial->initialize();

    vertexEnergy = 0.;

    eDepInTracker[0] = 0.;

    numOfEventsWithSingleTrack = 0;
    numOfPrimariesInDiamond = 0;

    interactionInDiamondInEventNumber = 0;
    primaryGammaInteractionInDiamondInEventNumber = 0;
    primaryGammaInteractionInElectrodeInEventNumber = 0;

    primaryInDiamondInEvent = false;
    processChainOfEvent = "";

    initializeProcessCounter();
    initializeNuclearChannelCounter();
    initializeParticleCounter();
    //initializeVerticesCounter();
    //initializeVolumesCounter();
    //initializeGammaOriginsCounter();
    initializeProcessesChainCounter();
    //initializeSecondariesSpectra();
}

void Data::initializeProcessCounter()
{
    if(proc_count != 0)
        deleteProcessCounter();

    proc_count = new ProcessesCount();
}

void Data::initializeNuclearChannelCounter()
{
    if(nuc_count != 0)
        deleteNuclearChannelCounter();

    nuc_count = new ProcessesCount();
}

void Data::initializeParticleCounter()
{
    if(part_count != 0)
        deleteParticleCounter();

    part_count = new ParticlesCount();
}

void Data::initializeVerticesCounter()
{
    if(vertices_count != 0)
        deleteVerticesCounter();

    vertices_count = new ParticlesCount();
}

void Data::initializeVolumesCounter()
{
    if(volumes_count != 0)
        deleteVolumesCounter();

    volumes_count = new ParticlesCount();
}

void Data::initializeGammaOriginsCounter()
{
    if(gamma_origins_count != 0)
        deleteGammaOriginsCounter();

    gamma_origins_count = new ParticlesCount();
}

void Data::initializeProcessesChainCounter()
{
    if(processes_chain_count != 0)
        deleteProcessesChainCounter();

    processes_chain_count = new ProcessesCount();
}

void Data::initializeSecondariesSpectra()
{
    if(secondaries_spectra != 0)
        deleteSecondariesSpectra();

    secondaries_spectra = new Spectra();
}

void Data::deleteProcessCounter()
{
    if(proc_count != 0)
        delete proc_count;

    proc_count = 0;
}

void Data::deleteNuclearChannelCounter()
{
    if(nuc_count != 0)
        delete nuc_count;

    nuc_count = 0;
}

void Data::deleteParticleCounter()
{
    if(part_count != 0)
        delete part_count;

    part_count = 0;
}

void Data::deleteVerticesCounter()
{
    if(vertices_count != 0)
        delete vertices_count;

    vertices_count = 0;
}

void Data::deleteVolumesCounter()
{
    if(volumes_count != 0)
        delete volumes_count;

    volumes_count = 0;
}

void Data::deleteGammaOriginsCounter()
{
    if(gamma_origins_count != 0)
        delete gamma_origins_count;

    gamma_origins_count = 0;
}

void Data::deleteProcessesChainCounter()
{
    if(processes_chain_count != 0)
        delete processes_chain_count;

    processes_chain_count = 0;
}

void Data::deleteSecondariesSpectra()
{
    if(secondaries_spectra != 0)
        delete secondaries_spectra;

    secondaries_spectra = 0;
}

void Data::setName(G4String name, G4String namePrefix)
{
    this->name = name;

    this->histogramEDEP_DIAMOND_ALL->setName(name, namePrefix);
    this->histogramEDEP_DIAMOND_GAMMA->setName(name, namePrefix);
    this->histogramEDEP_DIAMOND_ELECTRON->setName(name, namePrefix);
    this->histogramInitial->setName(name, namePrefix);

}

void Data::saveHistograms(G4int runID)
{
    this->histogramEDEP_DIAMOND_ALL->saveHisto(runID);
    this->histogramEDEP_DIAMOND_GAMMA->saveHisto(runID);
    this->histogramEDEP_DIAMOND_ELECTRON->saveHisto(runID);
    this->histogramInitial->saveHisto(runID);
}

