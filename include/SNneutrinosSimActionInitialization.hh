#ifndef SNneutrinosSimActionInitialization_h
#define SNneutrinosSimActionInitialization_h

#include "G4VUserActionInitialization.hh"

class SNneutrinosSimActionInitialization : public G4VUserActionInitialization
{
    public:
        SNneutrinosSimActionInitialization();
        ~SNneutrinosSimActionInitialization();

        virtual void BuildForMaster() const;
        virtual void Build() const;
};

#endif