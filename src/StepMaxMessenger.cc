
#include "StepMaxMessenger.hh"

#include "StepMax.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "globals.hh"

StepMaxMessenger::StepMaxMessenger(StepMax* stepM)
:stepMax(stepM)
{
  StepMaxCmd = new G4UIcmdWithADoubleAndUnit("/mult/stepMax",this);
  StepMaxCmd->SetGuidance("Set max allowed step length");
  StepMaxCmd->SetParameterName("mxStep",false);
  StepMaxCmd->SetRange("mxStep>0.");
  StepMaxCmd->SetUnitCategory("Length");
  StepMaxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

StepMaxMessenger::~StepMaxMessenger()
{
  delete StepMaxCmd;
}

void StepMaxMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if (command == StepMaxCmd)
    { stepMax->SetMaxStep(StepMaxCmd->GetNewDoubleValue(newValue));}
}
