#ifndef Data_h
#define Data_h 1

#include"globals.hh"
#include <vector>

#include <stdlib.h>
#include <math.h>

#include "ProcessesCount.hh"
#include "ParticlesCount.hh"
#include "Spectra.hh"
#include "EnergySpectrum.hh"

#include"G4ParticleDefinition.hh"
#include"G4ParticleTable.hh"
#include"G4UnitsTable.hh"

class HistogramEDEP
{
public:
    HistogramEDEP(G4String, G4double);
    ~HistogramEDEP();

    G4String label;

    G4double scaling;

    G4String runName;
    G4String prefixName;

    static const G4int SIZE = 300000; // 1 eV/keV/MeV step size, 100 keV/MeV/GeV maximum

    G4double dataArray[SIZE];

    void initialize();
    void addValue(G4double val);
    void printHisto();
    void saveHisto(G4int runID);
    void setName(G4String name, G4String namePrefix);

    double getMean();
    double getRMS();
};

class Data
{
public:
    Data(G4double);
    ~Data();

    G4int currentRunID;

    G4String name;
    G4String namePrefix;

    G4int interactionInDiamondInEventNumber;
    G4int primaryGammaInteractionInDiamondInEventNumber;
    G4int primaryGammaInteractionInElectrodeInEventNumber;

    HistogramEDEP* histogramEDEP_DIAMOND_ALL;
    HistogramEDEP* histogramEDEP_DIAMOND_GAMMA;
    HistogramEDEP* histogramEDEP_DIAMOND_ELECTRON;

    HistogramEDEP* histogramInitial;

    G4double eDepInTracker[3];

    G4double vertexEnergy;

    G4int numOfEventsWithSingleTrack;
    G4int numOfPrimariesInDiamond;

    G4bool primaryInDiamondInEvent;

    G4double histoScaling;

    G4String processChainOfEvent;

    ProcessesCount* proc_count;
    ProcessesCount* nuc_count;
    ParticlesCount* part_count;
    ParticlesCount* vertices_count;
    ParticlesCount* volumes_count;
    ParticlesCount* gamma_origins_count;
    ProcessesCount* processes_chain_count;
    Spectra* secondaries_spectra;

    void initialize();

    void initializeProcessCounter();
    void initializeNuclearChannelCounter();
    void initializePSData();
    void initializeParticleCounter();
    void initializeVerticesCounter();
    void initializeVolumesCounter();
    void initializeGammaOriginsCounter();
    void initializeProcessesChainCounter();
    void initializeSecondariesSpectra();

    void deleteProcessCounter();
    void deleteNuclearChannelCounter();
    void deleteParticleCounter();
    void deleteVerticesCounter();
    void deleteVolumesCounter();
    void deleteGammaOriginsCounter();
    void deleteProcessesChainCounter();
    void deleteSecondariesSpectra();

    void setName(G4String name, G4String namePrefix);
    void saveHistograms(G4int runID);
};

#endif
