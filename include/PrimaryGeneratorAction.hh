#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "DetectorConstruction.hh"

#include<G4Event.hh>
#include<G4ParticleGun.hh>
#include<G4GeneralParticleSource.hh>
#include<G4ParticleTable.hh>
#include<G4ParticleDefinition.hh>
#include<globals.hh>

#include "ParticlesCount.hh"

#include <stdlib.h>

#include "G4VUserPrimaryGeneratorAction.hh"

class DetectorConstruction;
class G4ParticleGun;
class G4Event;

class BiasedEnergyHistogram
{
    public:
    BiasedEnergyHistogram();
    ~BiasedEnergyHistogram();

    void addEnergy(G4double ene, G4double weight);
    G4double produceEnergy();

    private:
    const static G4int SIZE = 10;
    G4double * energyList;
    G4double * weightList;
    G4int energyCounter;
    G4double weightSum;
};

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction(DetectorConstruction*);
    ~PrimaryGeneratorAction();

public:
    void GeneratePrimaries(G4Event*);
    void CountEnergy(G4String partName);

public:
    G4ParticleGun* particleGun;
    G4GeneralParticleSource* particleSource;

    BiasedEnergyHistogram * biasedEnergyHistogram;

    ParticlesCount * energyHistogramVerification;

private:
    DetectorConstruction* myDetector;
};


#endif


