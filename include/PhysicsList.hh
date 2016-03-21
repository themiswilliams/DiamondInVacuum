#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class PhysicsList: public G4VModularPhysicsList
{
public:
  PhysicsList();
  ~PhysicsList();

  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

private:
  void GeneralPhysics();
  void NeutronPhysics();
};

#endif

