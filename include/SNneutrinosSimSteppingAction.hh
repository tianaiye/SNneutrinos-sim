#ifndef SNneutrinosSimSteppingAction_h
#define SNneutrinosSimSteppingAction_h 

#include "G4UserSteppingAction.hh"
#include "G4Types.hh"
#include "G4SystemOfUnits.hh"


class SNneutrinosSimSteppingAction : public G4UserSteppingAction
{
  public:
    SNneutrinosSimSteppingAction();
    virtual ~SNneutrinosSimSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

  private:
    G4double trackLength;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif