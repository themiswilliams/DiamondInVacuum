#include "EnergySpectrum.hh"

#include <fstream>
#include "G4UIcommand.hh"


EnergySpectrum::EnergySpectrum()
{
    spectrum_array = new G4int[SPECTRUM_ARRAY_SIZE];

    for (G4int i = 0; i < SPECTRUM_ARRAY_SIZE; i++)
    {
        spectrum_array[i] = 0;
    }
}

EnergySpectrum::~EnergySpectrum()
{
    delete spectrum_array;
}

void EnergySpectrum::initialize()
{
    for (G4int i = 0; i < SPECTRUM_ARRAY_SIZE; i++)
    {
        spectrum_array[i] = 0;
    }
}

void EnergySpectrum::addValue(G4double val)
{
    G4int index = static_cast<G4int>(round(val));

    if(index >= 0 && index < SPECTRUM_ARRAY_SIZE)
        spectrum_array[index]++;
}

G4double EnergySpectrum::round(G4double val)
{
    return (val > 0.0) ? floor(val + 0.5) : ceil(val - 0.5);
}
