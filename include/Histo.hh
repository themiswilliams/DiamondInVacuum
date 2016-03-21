#ifndef Histo_h
#define Histo_h 1

#include"globals.hh"
#include <vector>

#include <stdlib.h>
#include <math.h>

class Histo
{
public:
    Histo(G4int histoStep, G4String hostname);
    ~Histo();

    G4String hostname;

    const static G4int HISTO_SIZE = 10000;
        const static G4int E_HISTO_SIZE = 1000;

    typedef std::vector<G4int> RangeHistogram;
    RangeHistogram * range;

    void disposeRangeHistogram();
    void fillRange(G4int rng); //in um
    void saveRange(G4int runNumber, G4String hostname);
    void get_Range_Mean_Sigma(G4double* mean, G4double* sigma);
    void saveRangeMeanSigma();

    typedef std::vector<G4int> LandauDistribution;
    LandauDistribution * LandauDb;

    void calculatePrimLandau();

    void disposeLandauDb();
    void saveLandauDb(G4int runNumber, G4String hostname);
    void fillLandauDb(G4int eLoss);
    void get_Landau_Mean_Sigma(G4double* mean, G4double* sigma);
    void get_Landau_MPV(G4double* MPV);
    void saveLANDAU_MEAN_SIGMA_MPV();

    G4double eDepX[HISTO_SIZE]; //average total energy deposit along OX
    G4double eDepY[HISTO_SIZE];
    G4double eDepZ[HISTO_SIZE];

    G4double primaryDepX[HISTO_SIZE]; //average primary particle energy deposit along OX
    G4double primaryDepY[HISTO_SIZE];
    G4double primaryDepZ[HISTO_SIZE];

    G4double photoESpectra[E_HISTO_SIZE];
    G4double comptESpectra[E_HISTO_SIZE];
    G4double pairESpectra[E_HISTO_SIZE];

    void get_Mean_Sigma_Averaged(G4double array[], G4double numOfEvents, G4int size, G4double* mean, G4double* sigma);
    void get_Mean_Sigma(G4double array[], G4int size, G4double* mean, G4double* sigma);

    G4bool firstHit;

    G4double DetX; // um
    G4double DetY;
    G4double DetZ;

    G4double vertex;

    G4int histoStep; // um

    G4int numOfEvents;

    G4int numOfDetPrimaries; //primary particles that have reached the Detector

    G4double primaryEDep; //primary particles total energy deposition (continuous losses) in Detector
    G4double primaryEDelta; //primary particles total energy loss in Detector
    G4double primaryNIEL;

    G4double totalEDep;

    G4int numOfDetElectrons; // e- born in Detector volume
    G4int nonScatteredGamma;

    G4int runNumber;

    void initialize();

    void saveToFile();
    void saveEDepXToFile(G4int runNumber, G4String hostname);
    void saveEDepYToFile(G4int runNumber, G4String hostname);
    void saveEDepZToFile(G4int runNumber, G4String hostname);
    void savePrimDepXToFile(G4int runNumber, G4String hostname);
    void savePrimDepYToFile(G4int runNumber, G4String hostname);
    void savePrimDepZToFile(G4int runNumber, G4String hostname);

    void savePhotoESpectraToFile(G4int runNumber, G4String hostname);
    void saveComptESpectraToFile(G4int runNumber, G4String hostname);
    void savePairESpectraToFile(G4int runNumber, G4String hostname);

    void setDetSize(G4double X, G4double Y, G4double Z);

    void print();
};

#endif
