#ifndef SNneutrinosSimRunAction_h
#define SNneutrinosSimRunAction_h

#include "G4UserRunAction.hh"

#include "Run.hh"

class SNneutrinosSimRunAction : public G4UserRunAction
{
    public:
        SNneutrinosSimRunAction();
        virtual ~SNneutrinosSimRunAction();

        virtual void BeginOfRunAction(const G4Run *aRun);
        virtual void EndOfRunAction(const G4Run *aRun);
        G4Run* GenerateRun();

    private:
        Run* fRun;
};

#endif