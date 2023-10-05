#include "Run.hh"

Run::Run() : G4Run()
{
  fTotal = 0;
  //fDetection = 0;
  fSurfaceDetection = 0;
  fWaterDetection = 0;
  fPMTDetection = 0;
  fOpticalPhoton = 0;

}
Run::~Run()
{}

void Run::Merge(const G4Run* run)
{
  const Run* localRun = static_cast<const Run*>(run);

  // pass information about primary particle
  fTotal = localRun->fTotal;   //fix it! We want the t
  //fDetection = localRun->fDetection;
  fSurfaceDetection = localRun->fSurfaceDetection;
  fWaterDetection = localRun->fWaterDetection;
  fPMTDetection = localRun->fPMTDetection;
  fOpticalPhoton = localRun->fOpticalPhoton;
   
  G4Run::Merge(run);
}

void Run::EndOfRun()
{
  G4cout << "\n   Summary\n";
  G4cout <<   "---------------------------------\n";
  G4cout << "  # of Generated Positrons:                " << std::setw(8) << fTotal << G4endl;
  G4cout << "  # of Generated Optical photons:          " << std::setw(8) << fOpticalPhoton << G4endl;
  G4cout << "  Photons Ended in the water tank:         " << std::setw(8) << fWaterDetection << G4endl;
  G4cout << "  Photons Ended in the PMT:                " << std::setw(8) << fPMTDetection <<  G4endl;
  G4cout << "  Efficiency of Photons in PMT :           " << std::setw(8) <<  double(fPMTDetection)/double(fOpticalPhoton)*100 << " %" << G4endl;
  /*G4cout << "  Photons Detected:                 " << std::setw(8) << double(fDetection)/double(fTotal)*100 << " %" << G4endl;
  G4cout << "  Photons Absorbed in PEN:          " << std::setw(8) << double(fPenAbsorption)/double(fTotal)*100 << " %" << G4endl;
  G4cout << "  Photons Absorbed in PMMA guide:   " << std::setw(8) << double(fLightGuideAbsorption)/double(fTotal)*100 << " %" << G4endl;
  G4cout << "  Photons Absorbed in PMMA panel:   " << std::setw(8) << double(fPanelAbsorption)/double(fTotal)*100 << " %" << G4endl;
  G4cout << "  Photons Absorbed in LAr:          " << std::setw(8) << double(fLArAbsorption)/double(fTotal)*100 << " %" << G4endl;
  G4cout << "  Photons Absorbed in Outer Cladding: " << std::setw(8) << double(fOuterCladdingAbsorption)/double(fTotal)*100 << " %" << G4endl;
  G4cout << "  Photons Absorbed in Inner Cladding: " << std::setw(8) << double(fInnerCladdingAbsorption)/double(fTotal)*100 << " %" << G4endl; 
  G4cout << "  TOTAL:          " << std::setw(8) << double(fDetection+fPenAbsorption+fLightGuideAbsorption+fPanelAbsorption+fLArAbsorption+fOuterCladdingAbsorption+fInnerCladdingAbsorption)/double(fTotal)*100 << " %" << G4endl;
  G4cout <<   "---------------------------------\n";

  G4cout << "\n   Where are photon before going into LAr?\n";
  G4cout <<   "---------------------------------\n";
  G4cout << "  Photons coming from Panel        " << std::setw(8) << double(fPanelTowardLAr)/double(fLArAbsorption) * 100 << " %" << G4endl;
  G4cout << "  Photons coming from PEN          " << std::setw(8) << double(fPENTowardLAr)/double(fLArAbsorption) * 100 << " %" << G4endl;
  G4cout << "  Photons coming from PMMA guide   " << std::setw(8) << double(fLightGuideTowardLAr)/double(fLArAbsorption) * 100 << " %" << G4endl;
  G4cout << "  TOTAL:          " << std::setw(8) << double(fPanelTowardLAr+fPENTowardLAr+fLightGuideTowardLAr)/double(fLArAbsorption)*100 << " %" << G4endl;
  G4cout << "\n";
  */
}
