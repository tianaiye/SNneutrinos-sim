#ifndef DetectorMessenger_h
#define DetectorMessenger_h

#include "globals.hh"
#include "G4UImessenger.hh"

class SNneutrinosSimDetectorConstruction;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;

class DetectorMessenger: public G4UImessenger
{
    public:

        DetectorMessenger(SNneutrinosSimDetectorConstruction* );
        ~DetectorMessenger();

        virtual void SetNewValue(G4UIcommand*, G4String);

    private:

        SNneutrinosSimDetectorConstruction* fDetector;

        G4UIdirectory*  fSNneutrinosSimDir;

        G4UIcmdWithAString* fGeometryTypeCmd;
};

#endif