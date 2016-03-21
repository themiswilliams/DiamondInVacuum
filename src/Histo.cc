#include "Histo.hh"

#include <fstream>
#include "G4UIcommand.hh"


Histo::Histo(G4int histoStep, G4String hostname)
{
    G4int i;

    this->numOfEvents = 0;

    this->firstHit = true;

    this->DetX = 500;
    this->DetY = 5000;
    this->DetZ = 5000;

    this->vertex = 0.;

    this->numOfDetPrimaries = 0;
    this->primaryEDep = 0.;
    this->primaryEDelta = 0.;
    this->primaryNIEL = 0.;
    this->totalEDep = 0.;

    this->numOfDetElectrons = 0; // e- born in Detector volume
    this->nonScatteredGamma = 0;

    this->histoStep = histoStep;

    this->hostname = hostname;

    for (i = 0; i < HISTO_SIZE; ++i)
    {
        this->eDepX[i] = 0.;
        this->eDepY[i] = 0.;
        this->eDepZ[i] = 0.;
        this->primaryDepX[i] = 0.;
        this->primaryDepY[i] = 0.;
        this->primaryDepZ[i] = 0.;
    }

    for (i = 0; i < E_HISTO_SIZE; ++i)
    {
        photoESpectra[i] = 0.;
        comptESpectra[i] = 0.;
        pairESpectra[i] = 0.;
    }

    this->LandauDb = new LandauDistribution();

    this->range = new RangeHistogram();

    this->runNumber = 0;
}

Histo::~Histo()
{
    G4cout << "Deleting Histo . . . " << G4endl;
}

void Histo::initialize()
{
    G4cout << "Initializing Histo . . . " << G4endl;
    G4int i;

    this->numOfEvents = 0;

    this->firstHit = true;
    this->vertex = 0.;

    this->numOfDetPrimaries = 0;
    this->primaryEDep = 0.;
    this->primaryEDelta = 0.;
    this->primaryNIEL = 0.;
    this->totalEDep = 0.;

    this->numOfDetElectrons = 0; // e- born in Detector volume
    this->nonScatteredGamma = 0;

    for (i = 0; i < HISTO_SIZE; ++i)
    {
        this->eDepX[i] = 0.;
        this->eDepY[i] = 0.;
        this->eDepZ[i] = 0.;
        this->primaryDepX[i] = 0.;
        this->primaryDepY[i] = 0.;
        this->primaryDepZ[i] = 0.;
    }


    for (i = 0; i < E_HISTO_SIZE; ++i)
    {
        photoESpectra[i] = 0.;
        comptESpectra[i] = 0.;
        pairESpectra[i] = 0.;
    }

    this->LandauDb = new LandauDistribution();
    this->range = new RangeHistogram();
}

void Histo::print()
{
    G4int i;

    G4cout << "Histogram in Run # " << this->runNumber << G4endl;
    G4cout << "Number of events: " << this->numOfEvents << G4endl;

    G4cout << "\nNumber of e- born in Detector: " << numOfDetElectrons << G4endl;

    G4cout << "\n -- Number of primary particles registered in Detector: " << numOfDetPrimaries << G4endl;

    if (numOfEvents != 0)
    {
        G4cout << "\n -- Average energy deposition (continuous losses) of primary particles in Detector: "
        << (primaryEDep/numOfEvents)/MeV << " MeV" << G4endl;
        G4cout << "\n -- Average total energy losses of primary particles in Detector: "
        << (primaryEDelta/numOfEvents)/MeV << " MeV" << G4endl;
        G4cout << "\n -- Average energy deposition (continuous losses) of all particles in Detector:"
        << (totalEDep/numOfEvents)/MeV << " MeV" << G4endl;
    }

    //G4cout << "\n\n Non-scattered primary gamma count: " << nonScatteredGamma << G4endl;
}

void Histo::saveLANDAU_MEAN_SIGMA_MPV()
{
    G4double meanValue;
    G4double sigmaValue;
    G4double MPVValue;

    meanValue = 0.;
    sigmaValue = 0.;
    MPVValue = 0.;

    get_Landau_Mean_Sigma(&meanValue, &sigmaValue);
    get_Landau_MPV(&MPVValue);

    std::ofstream output;
    G4String name;

    name = this->hostname;
    name += "-Landau-mean-sigma-mpv-vs-vertices-(ev-um).txt";
    output.open(name, std::ios::app);

    output << (this->vertex)/MeV << " "
    << meanValue << " "
    << sigmaValue << " "
    << MPVValue << "\n";

    output.flush();
    output.close();
}

void Histo::saveToFile()
{
    saveEDepXToFile(this->runNumber, this->hostname);
    saveEDepYToFile(this->runNumber, this->hostname);
    saveEDepZToFile(this->runNumber, this->hostname);
    savePrimDepXToFile(this->runNumber, this->hostname);
    savePrimDepYToFile(this->runNumber, this->hostname);
    savePrimDepZToFile(this->runNumber, this->hostname);

    savePhotoESpectraToFile(this->runNumber, this->hostname);
    saveComptESpectraToFile(this->runNumber, this->hostname);
    savePairESpectraToFile(this->runNumber, this->hostname);

    calculatePrimLandau();
    saveLandauDb(this->runNumber, this->hostname);
    disposeLandauDb();

    saveRange(this->runNumber, this->hostname);
    disposeRangeHistogram();
}

void Histo::disposeRangeHistogram()
{
    delete range;
}

void Histo::savePhotoESpectraToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".photoE-Spectra(keV).txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    for (i = 0; i < E_HISTO_SIZE; ++i)
    {
        myfile << i << " " << photoESpectra[i] << "\n";
    }

    myfile.close();
}

void Histo::saveComptESpectraToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".comptE-Spectra(keV).txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    for (i = 0; i < E_HISTO_SIZE; ++i)
    {
        myfile << i << " " << comptESpectra[i] << "\n";
    }

    myfile.close();
}

void Histo::savePairESpectraToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".pairE-Spectra(MeV).txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    for (i = 0; i < E_HISTO_SIZE; ++i)
    {
        myfile << i << " " << pairESpectra[i] << "\n";
    }

    myfile.close();
}

void Histo::saveEDepXToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".total.X.txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    G4int printTo = (G4int)(floor((DetX/um)/histoStep));
    if (printTo > HISTO_SIZE) printTo = HISTO_SIZE;

    for (i = 0; i < printTo; ++i)
    {
        myfile << i << " " << eDepX[i]/numOfEvents << "\n";
    }

    myfile.close();
}

void Histo::saveEDepYToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".total.Y.txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    G4int printTo = (G4int)(floor((DetY/um)/histoStep));
    if (printTo > HISTO_SIZE) printTo = HISTO_SIZE;

    for (i = 0; i < printTo; ++i)
    {
        myfile << i << " " << eDepY[i]/numOfEvents << "\n";
    }

    myfile.close();
}

void Histo::saveEDepZToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".total.Z.txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    G4int printTo = (G4int)(floor((DetZ/um)/histoStep));
    if (printTo > HISTO_SIZE) printTo = HISTO_SIZE;

    for (i = 0; i < printTo; ++i)
    {
        myfile << i << " " << eDepZ[i]/numOfEvents << "\n";
    }

    myfile.close();
}

void Histo::savePrimDepXToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".primary.X.txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    G4int printTo = (G4int)(floor((DetX/um)/histoStep));
    if (printTo > HISTO_SIZE) printTo = HISTO_SIZE;

    for (i = 0; i < printTo; ++i)
    {
        myfile << i << " " << primaryDepX[i]/numOfEvents << "\n";
    }

    myfile.close();
}

void Histo::savePrimDepYToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".primary.Y.txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    G4int printTo = (G4int)(floor((DetY/um)/histoStep));
    if (printTo > HISTO_SIZE) printTo = HISTO_SIZE;

    for (i = 0; i < printTo; ++i)
    {
        myfile << i << " " << primaryDepY[i]/numOfEvents << "\n";
    }

    myfile.close();
}

void Histo::savePrimDepZToFile(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".primary.Z.txt";

    G4int i;

    myfile.open(name, std::fstream::out);

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    G4int printTo = (G4int)(floor((DetZ/um)/histoStep));
    if (printTo > HISTO_SIZE) printTo = HISTO_SIZE;

    for (i = 0; i < printTo; ++i)
    {
        myfile << i << " " << primaryDepZ[i]/numOfEvents << "\n";
    }

    myfile.close();
}

void Histo::calculatePrimLandau()
{
    G4int column;
    G4int i;

    if (this->numOfEvents == 0)
    {
        G4cout << "NUM OF EVENTS == 0" << G4endl;
        return;
    }

    G4int printTo = (G4int)(floor((DetX/um)/histoStep));
    if (printTo > HISTO_SIZE) printTo = HISTO_SIZE;

    for (i = 0; i < printTo; ++i)
    {
        column = (G4int)(floor((primaryDepX[i]/numOfEvents)*1000000));

        if (column > 0)
        {
            fillLandauDb(column);
        }
    }
}

void Histo::disposeLandauDb()
{
    delete LandauDb;
    LandauDb = 0;
}

void Histo::saveLandauDb(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".LandauDb.txt";

    myfile.open(name, std::fstream::out);

    size_t sz = LandauDb->size();

    size_t i = 0;

    for(i = 0; i < sz; i+=2)
    {
        myfile << (*LandauDb)[i] << " " << (*LandauDb)[i+1] << "\n";
    }

    myfile.close();
}

void Histo::fillLandauDb(G4int eLoss)
{
    G4bool filled = false;

    size_t sz = LandauDb->size();

    size_t i = 0;

    if(sz > 0)
    {
        for(i = 0; i < sz; i+=2)
        {
            if(((*LandauDb)[i]) == eLoss)
            {
                ((*LandauDb)[i+1])++;
                filled = true;
            }
        }

        if(!filled)
        {
            LandauDb->push_back(eLoss);
            LandauDb->push_back(1);
        }
    }
    else
    {
        LandauDb->push_back(eLoss);
        LandauDb->push_back(1);
    }
}

void Histo::fillRange(G4int rng)
{
    G4bool filled = false;

    size_t sz = range->size();

    size_t i = 0;

    if(sz > 0)
    {
        for(i = 0; i < sz; i+=2)
        {
            if(((*range)[i]) == rng)
            {
                ((*range)[i+1])++;
                filled = true;
            }
        }

        if(!filled)
        {
            range->push_back(rng);
            range->push_back(1);
        }
    }
    else
    {
        range->push_back(rng);
        range->push_back(1);
    }

}

void Histo::saveRange(G4int runNumber, G4String hostname)
{
    std::fstream myfile;

    G4String name = "run";
    G4String s = G4UIcommand::ConvertToString(runNumber);
    name += s;
    name += ".";
    name += hostname;
    name += ".Range.txt";

    myfile.open(name, std::fstream::out);

    size_t sz = range->size();

    size_t i = 0;

    for(i = 0; i < sz; i+=2)
    {
        myfile << (*range)[i] << " " << (*range)[i+1] << "\n";
    }

    myfile.close();
}

void Histo::get_Range_Mean_Sigma(G4double* mean, G4double* sigma)
{
    G4double sum = 0., deviation = 0.;
    G4int sumOfWeights = 0;
    G4double meanValue = 0., sigmaValue = 0.;

    size_t sz = range->size();

    size_t i = 0;

    if(sz == 0)
    {
        G4cout << "get-range-Mean-RMS: SIZE == 0 !" << G4endl;

        *mean = 0;
        *sigma = 0;

        return;
    }

    for(i = 0; i < sz; i+=2)
    {
        sumOfWeights += ((*range)[i+1]);
    }

    for(i = 0; i < sz; i+=2)
    {
        meanValue += ((*range)[i])*((((*range)[i+1])*1.)/sumOfWeights);
    }

    for(i = 0; i < sz; i+=2)
    {
        deviation += ((meanValue - ((*range)[i]))*(meanValue - ((*range)[i])))*((((*range)[i+1])*1.)/sumOfWeights);
    }

    sigmaValue = sqrt(deviation);

    *mean = meanValue;
    *sigma = sigmaValue;
}

void Histo::saveRangeMeanSigma()
{
    G4double meanValue;
    G4double sigmaValue;

    meanValue = 0.;
    sigmaValue = 0.;

    get_Range_Mean_Sigma(&meanValue, &sigmaValue);

    std::ofstream output;
    G4String name;

    name = this->hostname;
    name += "-Range-mean-sigma-vs-vertices-(um).txt";
    output.open(name, std::ios::app);

    output << (this->vertex)/MeV << " "
    << meanValue << " "
    << sigmaValue << "\n";

    output.flush();
    output.close();
}

void Histo::get_Landau_Mean_Sigma(G4double* mean, G4double* sigma)
{
    G4double sum = 0., deviation = 0.;
    G4int sumOfWeights = 0;
    G4double meanValue = 0., sigmaValue = 0.;

    size_t sz = LandauDb->size();

    size_t i = 0;

    if(sz == 0)
    {
        G4cout << "get-Landau-Mean-Sigma: SIZE == 0 !" << G4endl;
        exit(1);
    }

    for(i = 0; i < sz; i+=2)
    {
        sumOfWeights += ((*LandauDb)[i+1]);
    }

    for(i = 0; i < sz; i+=2)
    {
        meanValue += ((*LandauDb)[i])*((((*LandauDb)[i+1])*1.)/sumOfWeights);
    }

    for(i = 0; i < sz; i+=2)
    {
        deviation += ((meanValue - ((*LandauDb)[i]))*(meanValue - ((*LandauDb)[i])))*((((*LandauDb)[i+1])*1.)/sumOfWeights);
    }

    sigmaValue = sqrt(deviation);

    *mean = meanValue;
    *sigma = sigmaValue;
}

void Histo::get_Landau_MPV(G4double* MPV)
{
    G4int max = 0, numOfMax = 0;
    G4double sum = 0.;

    size_t sz = LandauDb->size();

    size_t i = 0;
    size_t k = 0;

    if(sz == 0)
    {
        G4cout << "get-Landau-MPV: SIZE == 0 !" << G4endl;
        exit(1);
    }

    for(i = 0; i < sz; i+=2)
    {
        if(((*LandauDb)[i+1]) > max)
            max = ((*LandauDb)[i+1]);
    }

    for(k = 0; k < sz; k+=2)
    {
        if(((*LandauDb)[k+1]) == max)
        {
            sum += 1.*((*LandauDb)[k]);
            numOfMax++;
        }
    }

    if(numOfMax == 0)
    {
        G4cout << "get-Landau-MPV: numOfMax == 0 !" << G4endl;
        exit(1);
    }

    *MPV = sum/numOfMax;
}

void Histo::get_Mean_Sigma_Averaged(G4double array[], G4double numOfEvents, G4int size, G4double* mean, G4double* sigma)
{
    G4int i = 0;

    G4double sum = 0., deviation = 0.;
    G4double meanValue = 0., sigmaValue = 0.;

    if(size == 0)
    {
        G4cout << "get-Mean-Sigma-Averaged: SIZE == 0 !" << G4endl;
        exit(1);
    }

    for(i = 0; i < size; ++i)
    {
        sum += array[i]/numOfEvents;
    }

    meanValue = sum/size;

    for(i = 0; i < size; ++i)
    {
        deviation += (meanValue - array[i]/numOfEvents)*(meanValue - array[i]/numOfEvents);
    }

    deviation = deviation/size;
    sigmaValue = sqrt(deviation);

    *mean = meanValue;
    *sigma = sigmaValue;
}

void Histo::get_Mean_Sigma(G4double array[], G4int size, G4double* mean, G4double* sigma)
{
    G4int i = 0;

    G4double sum = 0., deviation = 0.;
    G4double meanValue = 0., sigmaValue = 0.;

    if(size == 0)
    {
        G4cout << "get-Mean-Sigma: SIZE == 0 !" << G4endl;
        exit(1);
    }

    for(i = 0; i < size; ++i)
    {
        sum += array[i];
    }

    meanValue = sum/size;

    for(i = 0; i < size; ++i)
    {
        deviation += (meanValue - array[i])*(meanValue - array[i]);
    }

    deviation = deviation/size;
    sigmaValue = sqrt(deviation);

    *mean = meanValue;
    *sigma = sigmaValue;
}

void Histo::setDetSize(G4double X, G4double Y, G4double Z)
{
    this->DetX = X;
    this->DetY = Y;
    this->DetZ = Z;
}

