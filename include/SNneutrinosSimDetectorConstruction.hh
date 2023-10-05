#ifndef SNneutrinosSimDetectorConstruction_h
#define SNneutrinosSimDetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4LogicalVolume.hh"

class DetectorMessenger;

class SNneutrinosSimDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        SNneutrinosSimDetectorConstruction();
        ~SNneutrinosSimDetectorConstruction();

        virtual G4VPhysicalVolume *Construct(); 
        // virtual void ConstructSDandField();

        void SetGeometry(G4String name) {fGeometryName = name;};
    
    private:
        void DefineMaterials();
        void SetOpticalProperties();

        G4VPhysicalVolume* SetupWTandCryostat();
        
        G4String fGeometryName;

        DetectorMessenger *fDetectorMessenger;

        G4Material *waterMat;
        G4MaterialPropertiesTable *waterMPT;

        G4Material *steelMat;
        G4MaterialPropertiesTable *steelMPT;

        G4Material *PMTMat;
        G4MaterialPropertiesTable *CsMPT;
};


#endif