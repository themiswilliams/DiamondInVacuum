#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4IonPhysics.hh"

#include "G4BaryonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4Neutron.hh"
#include "G4ProcessManager.hh"

#include "G4HadronElasticProcess.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4HadronFissionProcess.hh"
#include "G4HadronCaptureProcess.hh"

#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4NeutronHPThermalScattering.hh"
#include "G4NeutronHPFission.hh"
#include "G4ParaFissionModel.hh"
#include "G4NeutronHPCapture.hh"
#include "G4HadronElastic.hh"
#include "G4BinaryCascade.hh"
#include "G4CascadeInterface.hh"
#include "G4LFission.hh"
#include "G4LCapture.hh"
#include "G4TheoFSGenerator.hh"
#include "G4StringChipsParticleLevelInterface.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4QGSModel.hh"
#include "G4QGSParticipants.hh"

#include "G4NeutronHPElasticData.hh"
#include "G4HadronElasticDataSet.hh"
#include "G4BGGNucleonElasticXS.hh"
#include "G4NeutronHPJENDLHEElasticData.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4NeutronInelasticCrossSection.hh"
#include "G4BGGNucleonInelasticXS.hh"
#include "G4NeutronHPJENDLHEInelasticData.hh"
#include "G4NeutronHPThermalScatteringData.hh"
#include "G4NeutronHPFissionData.hh"
#include "G4HadronFissionDataSet.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4HadronCaptureDataSet.hh"

#include <vector>
using namespace std;

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  SetVerboseLevel(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle()
{
  G4BaryonConstructor *baryons=new G4BaryonConstructor();
  G4LeptonConstructor *leptons=new G4LeptonConstructor();
  G4BosonConstructor *bosons=new G4BosonConstructor();
  G4IonConstructor *ions=new G4IonConstructor();
  G4MesonConstructor *mesons=new G4MesonConstructor();
  G4ShortLivedConstructor *exotic=new G4ShortLivedConstructor();

  baryons->ConstructParticle();
  leptons->ConstructParticle();
  bosons->ConstructParticle();
  ions->ConstructParticle();
  mesons->ConstructParticle();
  exotic->ConstructParticle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructProcess()
{
  AddTransportation();
  GeneralPhysics();
  NeutronPhysics();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  G4VModularPhysicsList::SetCuts();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::GeneralPhysics()
{
  vector<G4VPhysicsConstructor*> general_physics;
  general_physics.push_back(new G4EmStandardPhysics());
  general_physics.push_back(new G4DecayPhysics());
  general_physics.push_back(new G4RadioactiveDecayPhysics());
  general_physics.push_back(new G4IonPhysics());
  for(G4int i=0; i<(G4int)general_physics.size(); i++) {
    general_physics[i]->ConstructProcess();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::NeutronPhysics()
{
  G4ParticleDefinition* neutron=G4Neutron::NeutronDefinition();
  G4ProcessManager *manager=neutron->GetProcessManager();

  //Elastic scattering
  G4HadronElasticProcess *theElasticProcess=new G4HadronElasticProcess();
  G4HadronElastic *theHEElasticModel=new G4HadronElastic();
  G4NeutronHPElastic *theElasticModel=new G4NeutronHPElastic();
  G4NeutronHPThermalScattering* theThermalElasticModel = new G4NeutronHPThermalScattering();
  //G4HadronElasticDataSet *theVHEElasticData=new G4HadronElasticDataSet(); //WARNING Alternative to G4BGGNucleonElasticXS
  G4BGGNucleonElasticXS *theVHEElasticData=new G4BGGNucleonElasticXS(G4Neutron::Neutron()); //WARNING Alternative to G4HadronElasticDataSet
  G4NeutronHPJENDLHEElasticData *theHEElasticData=new G4NeutronHPJENDLHEElasticData();
  G4NeutronHPElasticData *theElasticData=new G4NeutronHPElasticData();
  G4NeutronHPThermalScatteringData* theThermalElasticData = new G4NeutronHPThermalScatteringData();
  theElasticProcess->AddDataSet(theVHEElasticData); //Fisrt!
  theElasticProcess->AddDataSet(theHEElasticData); //Second!
  theElasticProcess->AddDataSet(theElasticData); //Third!
  theElasticProcess->AddDataSet(theThermalElasticData); //Fourth!
  theThermalElasticModel->SetMaxEnergy(4.*eV);
  theElasticModel->SetMinEnergy(4.*eV);
  theElasticModel->SetMaxEnergy(20.*MeV);
  theHEElasticModel->SetMinEnergy(20.*MeV);
  theElasticProcess->RegisterMe(theThermalElasticModel);
  theElasticProcess->RegisterMe(theElasticModel);
  theElasticProcess->RegisterMe(theHEElasticModel);
  manager->AddDiscreteProcess(theElasticProcess);

  //Inelastic scattering
  G4NeutronInelasticProcess *theInelasticProcess=new G4NeutronInelasticProcess();
    //Very hihg energy
    G4TheoFSGenerator *theVHEInelasticModel=new G4TheoFSGenerator();
    G4StringChipsParticleLevelInterface* theCascade=new G4StringChipsParticleLevelInterface();
    theVHEInelasticModel->SetTransport(theCascade);
    G4QGSMFragmentation* theFragmentation=new G4QGSMFragmentation();
    G4ExcitedStringDecay* theStringDecay=new G4ExcitedStringDecay(theFragmentation);
    G4QGSModel< G4QGSParticipants >* theStringModel=new G4QGSModel< G4QGSParticipants >;
    theStringModel->SetFragmentationModel(theStringDecay);
    theVHEInelasticModel->SetHighEnergyGenerator(theStringModel);
    //
  G4BinaryCascade *theHEInelasticModel=new G4BinaryCascade(); //WARNING Alternative to G4CascadeInterface (Bertini)
  //G4CascadeInterface *theHEInelasticModel=new G4CascadeInterface(); //WARNING Alternative to G4BinaryCascade
  //G4NeutronHPInelastic *theInelasticModel=new G4NeutronHPInelastic();
  G4NeutronHPThermalScattering* theThermalInelasticModel = new G4NeutronHPThermalScattering();
  //G4NeutronInelasticCrossSection *theVHEInelasticData=new G4NeutronInelasticCrossSection(); //WARNING Alternative to G4BGGNucleonInelasticXS
  G4BGGNucleonInelasticXS *theVHEInelasticData= new G4BGGNucleonInelasticXS(G4Neutron::Neutron()); //WARNING Alternative to G4NeutronInelasticCrossSection
  G4NeutronHPJENDLHEInelasticData *theHEInelasticData=new G4NeutronHPJENDLHEInelasticData();
  G4NeutronHPInelasticData *theInelasticData=new G4NeutronHPInelasticData();
  G4NeutronHPThermalScatteringData* theThermalInelasticData = new G4NeutronHPThermalScatteringData();
  theInelasticProcess->AddDataSet(theVHEInelasticData); //First!
  theInelasticProcess->AddDataSet(theHEInelasticData); //Second!
  theInelasticProcess->AddDataSet(theInelasticData); //Third!
  theInelasticProcess->AddDataSet(theThermalInelasticData); //Fourth!
  theThermalInelasticModel->SetMaxEnergy(0.01*eV);
  //theInelasticModel->SetMinEnergy(4.*eV);
  //theInelasticModel->SetMaxEnergy(20.*MeV);
  theHEInelasticModel->SetMinEnergy(0.01*eV);
  theHEInelasticModel->SetMaxEnergy(10.*GeV);
  theVHEInelasticModel->SetMinEnergy(10.*GeV);
  theInelasticProcess->RegisterMe(theThermalInelasticModel);
  //theInelasticProcess->RegisterMe(theInelasticModel);
  theInelasticProcess->RegisterMe(theHEInelasticModel);
  theInelasticProcess->RegisterMe(theVHEInelasticModel);
  manager->AddDiscreteProcess(theInelasticProcess);

  //Fission
  G4HadronFissionProcess *theFissionProcess=new G4HadronFissionProcess();
  G4LFission *theHEFissionModel=new G4LFission();
  //G4NeutronHPFission *theFissionModel=new G4NeutronHPFission(); //WARNING Does not create fragments; does create delayed neutrons!
  G4ParaFissionModel *theFissionModel=new G4ParaFissionModel(); //WARNING Does create fragments; does not create delayed neutrons!
  G4HadronFissionDataSet *theHEFissionData=new G4HadronFissionDataSet();
  G4NeutronHPFissionData *theFissionData=new G4NeutronHPFissionData();
  theFissionProcess->AddDataSet(theHEFissionData); //First!
  theFissionProcess->AddDataSet(theFissionData); //Second!
  theFissionModel->SetMaxEnergy(60.*MeV); //WARNING 60 MeV for G4ParaFissionModel; 20 MeV for G4NeutronHPFission
  theHEFissionModel->SetMinEnergy(60.*MeV);
  theFissionProcess->RegisterMe(theFissionModel);
  theFissionProcess->RegisterMe(theHEFissionModel);
  manager->AddDiscreteProcess(theFissionProcess);

  //Capture
  G4HadronCaptureProcess *theCaptureProcess=new G4HadronCaptureProcess();
  G4LCapture *theHECaptureModel=new G4LCapture();
  G4NeutronHPCapture *theCaptureModel=new G4NeutronHPCapture();
  G4HadronCaptureDataSet *theHECaptureData=new G4HadronCaptureDataSet();
  G4NeutronHPCaptureData *theCaptureData=new G4NeutronHPCaptureData();
  theCaptureProcess->AddDataSet(theHECaptureData); //First!
  theCaptureProcess->AddDataSet(theCaptureData); //Second!
  theCaptureModel->SetMaxEnergy(20.*MeV);
  theHECaptureModel->SetMinEnergy(20.*MeV);
  theCaptureProcess->RegisterMe(theCaptureModel);
  theCaptureProcess->RegisterMe(theHECaptureModel);
  manager->AddDiscreteProcess(theCaptureProcess);
}
