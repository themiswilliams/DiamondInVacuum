#ifndef EnergySpectrum_h
#define EnergySpectrum_h 1

#include"globals.hh"
#include <vector>

#include <stdlib.h>
#include <math.h>

#include"G4ParticleDefinition.hh"
#include"G4ParticleTable.hh"
#include"G4UnitsTable.hh"

class EnergySpectrum
{
public:
    EnergySpectrum();
    ~EnergySpectrum();

    static const G4int SPECTRUM_ARRAY_SIZE = 10000; // keV resolution
    G4int * spectrum_array;

    void initialize();
    void addValue(G4double val); // keV
    G4double round(G4double val);
};

#endif
