#ifndef ParticlesCount_HH
#define ParticlesCount_HH

#include "globals.hh"
#include <vector>

class OneParticleCount
{
public:
    OneParticleCount(G4String name) {Name=name; Counter=0;};
   ~OneParticleCount() {};

public:
    G4String GetName()       {return Name;};
    G4int GetCounter()    {return Counter;};
    void Count()         {Counter++;};

private:
    G4String Name;            // name
    G4int    Counter;         // counter
};


typedef std::vector<OneParticleCount*> ParticlesCount;

#endif






