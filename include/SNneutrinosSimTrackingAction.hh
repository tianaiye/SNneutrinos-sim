#ifndef SNneutrinosSimTrackingAction_h
#define SNneutrinosSimTrackingAction_h 

#include "G4UserTrackingAction.hh"

class SNneutrinosSimTrackingAction : public G4UserTrackingAction 
{
public:
  SNneutrinosSimTrackingAction();
  virtual ~SNneutrinosSimTrackingAction(){};
   
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

private:
    double track_length_g4;
  
};

#endif