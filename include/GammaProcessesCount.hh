#ifndef GammaProcessesCount_HH
#define GammaProcessesCount_HH

#include "globals.hh"
#include <vector>

class OneGammaCount
{
public:
    OneGammaCount(G4int ID)
    {
        myID = ID;

        processesChainIter = 0;

        for(G4int i = 0; i < processesChainSize; ++i)
        {
            processesChain[i]="na";
        }
    };

    ~OneGammaCount() {};

public:
    G4int GetID()
    {
        return myID;
    };


    G4int GetIter()
    {
        return processesChainIter;
    };

    G4String GetProcess(G4int i)
    {
        return processesChain[i];
    };

    void Count(G4String process)
    {
        processesChain[processesChainIter] = process;
        processesChainIter++;
    };

private:
    const static G4int processesChainSize = 16;
    G4int myID;            // ID

    G4String processesChain[processesChainSize];
    G4int processesChainIter;
};


typedef std::vector<OneGammaCount*> GammaProcessesCount;

#endif






