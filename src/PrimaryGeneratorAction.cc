#include "PrimaryGeneratorAction.hh"

BiasedEnergyHistogram::BiasedEnergyHistogram()
{
    energyList = new G4double[SIZE];
    weightList = new G4double[SIZE];

    for(int i = 0; i < SIZE; i++)
    {
        energyList[i] = 0.;
        weightList[i] = 0.;
    }

    energyCounter = 0;
    weightSum = 0.;
}

BiasedEnergyHistogram::~BiasedEnergyHistogram()
{
    delete [] energyList;
    delete [] weightList;

}

void BiasedEnergyHistogram::addEnergy(G4double ene, G4double weight)
{
    if(energyCounter >= SIZE)
    {
        G4cout << "BiasedEnergyHistogram: COUNTER == " << energyCounter << " !" << G4endl;
    }
    else
    {
        energyList[energyCounter] = ene;
        weightList[energyCounter] = weight;

        weightSum += weight;

        energyCounter++;
    }
}

G4double BiasedEnergyHistogram::produceEnergy()
{
    G4double result = 0.;
    G4double tmp = 0;

    G4int RAND_SIZE = 10000;

    G4double randValue = weightSum * (rand() % RAND_SIZE);
    G4double iter = 0;

    for(G4int k = 0; k < energyCounter; k++)
    {
        if(randValue >= iter && randValue < (weightList[k] * RAND_SIZE))
        {


            result = energyList[k];
            break;
        }
        else
            iter += weightList[k] * RAND_SIZE;
    }

    return result;
}

void PrimaryGeneratorAction::CountEnergy(G4String partName)
{
    size_t nbPart = energyHistogramVerification->size();

    size_t i = 0;

    while ( (i < nbPart) && ((*(energyHistogramVerification))[i]->GetName() != partName) ) i++;

    if (i == nbPart) energyHistogramVerification->push_back(new OneParticleCount(partName));

    (*(energyHistogramVerification))[i]->Count();
}

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* myDC)
    :myDetector(myDC)
{
    G4int n_particle = 1;

    particleSource = new G4GeneralParticleSource();

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* gamma = particleTable->FindParticle("gamma");
    G4ParticleDefinition* particle = gamma;

    //particleSource->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Mono");

    //particleGun->SetParticleDefinition(particle);
    //particleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
    //particleGun->SetParticlePosition(G4ThreeVector(-5*um, 0, 0));
    //particleGun->SetParticleEnergy(511*keV);

    //particleSource->SetParticleDefinition(particle);
    //particleSource->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
    //particleSource->SetParticlePosition(G4ThreeVector(-20*mm, 0, 0));
    //particleSource->SetParticleEnergy(0.012*eV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    //delete particleGun;
/*
    G4double total = 0;

    for (size_t i = 0; i < energyHistogramVerification->size(); i++)
    {
        total += (*(energyHistogramVerification))[i]->GetCounter();
    }

    G4cout << "\n\n --- \n\n PRIMARY ENERGIES:\n\n" << G4endl;
    for (size_t i = 0; i < energyHistogramVerification->size(); i++)
    {
        G4cout << (*(energyHistogramVerification))[i]->GetName() << "  " << 100. * (*(energyHistogramVerification))[i]->GetCounter()/total << " %" << G4endl;
    }

    delete energyHistogramVerification;

    */

    delete particleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    //particleGun->GeneratePrimaryVertex(anEvent);
    //G4double energy = biasedEnergyHistogram->produceEnergy();
    //CountEnergy(G4UIcommand::ConvertToString(energy));
    //G4cout << "\n energy == " << G4UIcommand::ConvertToString(energy) << G4endl;

    //particleSource->GetCurrentSource()->GetEneDist()->SetMonoEnergy(energy * keV);

    particleSource->GeneratePrimaryVertex(anEvent);
}
