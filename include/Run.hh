#ifndef Run_h
#define Run_h

#include "G4Run.hh"

class Run : public G4Run
{
    public:
        Run();
        ~Run();

        void AddToTotal(void) {fTotal += 1;}
        void AddSurfaceDetection(void) {fSurfaceDetection += 1;}
        //void AddDetection(void) {fDetection += 1;}
        
        virtual void Merge(const G4Run*);

        void EndOfRun();

    private:
        G4int fTotal;
        G4int fSurfaceDetection;
        //G4int fDetection;
        
};

#endif 