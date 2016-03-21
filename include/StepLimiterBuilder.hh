#ifndef StepLimiterBuilder_hh
#define StepLimiterBuilder_hh 1

#include "G4VPhysicsConstructor.hh"

class StepLimiterBuilder : public G4VPhysicsConstructor
{
public:
    StepLimiterBuilder(G4int ver=1, const G4String& name = "StepLimitBuilder");
    virtual ~StepLimiterBuilder();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

private:
    G4int verbose;
};

#endif
