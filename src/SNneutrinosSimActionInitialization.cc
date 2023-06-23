#include "SNneutrinosSimActionInitialization.hh"
#include "SNneutrinosSimRunAction.hh"
#include "SNneutrinosSimPrimaryGenerator.hh"
#include "SNneutrinosSimSteppingAction.hh"
#include "SNneutrinosSimTrackingAction.hh"

SNneutrinosSimActionInitialization::SNneutrinosSimActionInitialization()
{}

SNneutrinosSimActionInitialization::~SNneutrinosSimActionInitialization()
{}

void SNneutrinosSimActionInitialization::BuildForMaster() const
{
  SetUserAction(new SNneutrinosSimRunAction());
}

void SNneutrinosSimActionInitialization::Build() const
{
  SetUserAction(new SNneutrinosSimPrimaryGenerator());
  SetUserAction(new SNneutrinosSimRunAction());
  SetUserAction(new SNneutrinosSimSteppingAction());
  //SetUserAction(new SNneutrinosSimTrackingAction);
}