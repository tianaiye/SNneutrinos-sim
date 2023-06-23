#include "SNneutrinosSimTrackingAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
SNneutrinosSimTrackingAction::SNneutrinosSimTrackingAction()
:G4UserTrackingAction()
{;}

void SNneutrinosSimTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    track_length_g4 = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void SNneutrinosSimTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

    track_length_g4 = aTrack->GetTrackLength() / m;

    //analysisMan->FillNtupleDColumn(9, track_length_g4);
}
