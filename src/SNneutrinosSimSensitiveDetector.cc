#include "SNneutrinosSimSensitiveDetector.hh"

#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

SensitiveDetector::SensitiveDetector(G4String name)  : G4VSensitiveDetector(name)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROHist)
{
    G4Track *aTrack = aStep->GetTrack();

    aTrack->SetTrackStatus(fStopAndKill); 

    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector vertexPosition = aTrack->GetVertexPosition();
    G4ThreeVector momDirection = aTrack->GetMomentumDirection();

    G4AnalysisManager *man = G4AnalysisManager::Instance();
    
    man->FillNtupleDColumn(0, vertexPosition[0] / m);
    man->FillNtupleDColumn(1, vertexPosition[1] / m);
    man->FillNtupleDColumn(2, vertexPosition[2] / m);
    man->FillNtupleDColumn(3, momDirection[0]);
    man->FillNtupleDColumn(4, momDirection[1]);
    man->FillNtupleDColumn(5, momDirection[2]);
    man->AddNtupleRow(0);
    
    return true;
}