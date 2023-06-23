#include "g4root.hh"
#include "SNneutrinosSimRunAction.hh"


SNneutrinosSimRunAction::SNneutrinosSimRunAction()
{
    fRun = nullptr;
}

SNneutrinosSimRunAction::~SNneutrinosSimRunAction()
{}

G4Run* SNneutrinosSimRunAction::GenerateRun()
{
  fRun = new Run();
  return fRun;
}

void SNneutrinosSimRunAction::BeginOfRunAction(const G4Run *aRun)
{
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->OpenFile("SNneutrinos.root"); 
    
    man->CreateNtuple("Score", "Score");
    man->CreateNtupleDColumn("xPositronVertex");
    man->CreateNtupleDColumn("yPositronVertex");
    man->CreateNtupleDColumn("zPositronVertex");
    man->CreateNtupleDColumn("xPositronDirection");
    man->CreateNtupleDColumn("yPositronDirection");
    man->CreateNtupleDColumn("zPositronDirection");
    man->CreateNtupleDColumn("xPhotonPosition");
    man->CreateNtupleDColumn("yPhotonPosition"); 
    man->CreateNtupleDColumn("zPhotonPosition");

    man->FinishNtuple(0);

    G4cout << "Ho creato la Ntupla" << G4endl;
}

void SNneutrinosSimRunAction::EndOfRunAction(const G4Run *aRun)
{
    if (isMaster) fRun->EndOfRun();

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    man->Write();
    man->CloseFile("SNneutrinos.root"); 
}