#include "StepLimiterBuilder.hh"
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"
#include "G4ProcessManager.hh"
#include "StepMax.hh"

StepLimiterBuilder::StepLimiterBuilder(G4int ver, const G4String& name) :
        G4VPhysicsConstructor(name), verbose(ver)
{
}

StepLimiterBuilder::~StepLimiterBuilder(){ }

void StepLimiterBuilder::ConstructParticle(){ }

void StepLimiterBuilder::ConstructProcess()
{
    // Step limitation seen as a process
    G4StepLimiter* stepLimiter = new G4StepLimiter();
    G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();
    StepMax * sm = new StepMax();


    theParticleIterator->reset();
    while ((*theParticleIterator)())
    {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();

        if (particle->GetPDGCharge() != 0.0 && !(particle->IsShortLived()))
        {
            if (verbose > 0)
            {
                G4cout << "### " << GetPhysicsName() << " instantiates for "
                << particle->GetParticleName() << G4endl;
            }

            pmanager ->AddDiscreteProcess(stepLimiter);
            //pmanager ->AddDiscreteProcess(sm);
            pmanager ->AddDiscreteProcess(userCuts);
        }
    }
}
