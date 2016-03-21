#ifndef Spectra_HH
#define Spectra_HH

#include "globals.hh"
#include <vector>

class OneItemCount
{
public:
    OneItemCount(G4String name) {Name=name; Counter=0.; numberOfCounts = 0;};
   ~OneItemCount() {};

public:
    G4String GetName()       {return Name;};
    G4double GetCounter()    {return Counter;};
    G4int GetNumberOfCounts()    {return numberOfCounts;};
    void Count(G4double value)         {Counter += value; numberOfCounts++;};

private:
    G4String Name;            // name
    G4double    Counter;         // counter
    G4int numberOfCounts;
};


typedef std::vector<OneItemCount*> Spectra;

#endif






