#include "SNneutrinosSimSteppingAction.hh"
#include "Run.hh"

#include "G4OpBoundaryProcess.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SteppingManager.hh"
#include "G4RunManager.hh"
#include "G4ProcessManager.hh"

#include "G4SystemOfUnits.hh"

#include "g4root.hh"

SNneutrinosSimSteppingAction::SNneutrinosSimSteppingAction()
: G4UserSteppingAction()
{
    trackLength = 0.;
}

SNneutrinosSimSteppingAction::~SNneutrinosSimSteppingAction()
{}

void SNneutrinosSimSteppingAction::UserSteppingAction(const G4Step* step)
{
    // static G4ParticleDefinition* opticalphoton = 
    //             G4OpticalPhoton::OpticalPhotonDefinition();
    G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

    Run* run = static_cast<Run*>(
               G4RunManager::GetRunManager()->GetNonConstCurrentRun());

    G4Track* track = step->GetTrack();
    G4StepPoint* endPoint   = step->GetPostStepPoint();
    G4StepPoint* startPoint = step->GetPreStepPoint();

    G4double tmp_x = track->GetPosition().getX() / m;
    G4double tmp_y = track->GetPosition().getY() / m;
    G4double tmp_z = track->GetPosition().getZ() / m;
    //G4cout << tmp_x << " " << tmp_y << " " << tmp_z << G4endl;
    G4String particleName = track->GetDynamicParticle()->GetParticleDefinition()->GetParticleName();
    G4String startVolumeName = startPoint->GetTouchable()->GetVolume() -> GetLogicalVolume() -> GetName();
    G4String endVolumeName = endPoint->GetTouchable()->GetVolume() -> GetLogicalVolume() -> GetName();

    G4String processName = endPoint->GetProcessDefinedStep()->GetProcessName();
    

    //trackLength = trackLength + track->GetStepLength() / m;

   
    G4double pos_x = 0.;
    G4double pos_y = 0.;
    G4double pos_z = 0.;
    G4StepPoint* aStepPoint = 0;

    // G4cout << processName << G4endl;
    
    if (particleName=="opticalphoton" ){ 
        run->AddOpticalPhoton();
        aStepPoint = endPoint;
        G4ThreeVector pointIn = aStepPoint->GetPosition();
        pos_x = pointIn[0];
        pos_y = pointIn[1];
        pos_z = pointIn[2] ;

        G4cout << processName << G4endl;
        if (processName!="Transportation") G4cout << processName << G4endl;
        
        //G4cout << "step_r " << pos_x*pos_x+pos_y*pos_y << " " << 550*550*cm*cm <<G4endl;
        //G4cout << "step_h " << pos_z << " " << 650*cm << G4endl;

        if (startVolumeName == "Water_log"){

            if(endVolumeName == "Water_log"  || endVolumeName == "Cryostat_log" || endVolumeName == "Tank_log"){
                track->SetTrackStatus(fStopAndKill);
                run->AddWaterDetection();
                analysisMan->FillNtupleDColumn(6, pos_x);  //pos_x
                analysisMan->FillNtupleDColumn(7, pos_y);  //pos_y
                analysisMan->FillNtupleDColumn(8, pos_z);  //pos_z
                analysisMan->AddNtupleRow(0);
            }

            else if(endVolumeName == "PMT_log"){
                track->SetTrackStatus(fStopAndKill);
                run->AddPMTDetection();
                analysisMan->FillNtupleDColumn(6, pos_x);  //pos_x
                analysisMan->FillNtupleDColumn(7, pos_y);  //pos_y
                analysisMan->FillNtupleDColumn(8, pos_z);  //pos_z
                analysisMan->AddNtupleRow(0);
            }
            else G4cout << endVolumeName << G4endl;
        }
        else {
            G4cout << startVolumeName << G4endl;     
        }
    }
    return;
}

    // uncomment for Big Panel Design
    // if(endVolumeName == "Detector_log" && startVolumeName == "Panel_log")
    // {
    //     track->SetTrackStatus(fStopAndKill);
    //     run->AddDetection();
    //     analysisMan->FillNtupleIColumn(7, 1);
    //     analysisMan->FillNtupleDColumn(8, trackLength);
    //     trackLength = 0.*m;
    // }
    // // If photon inside LAr, kill track
    // if(endVolumeName == "World_log") 
    // {
    //     track->SetTrackStatus(fStopAndKill);
    //     trackLength = 0.*m;
    //     run->AddLArAbsorption();
    //     // if(startVolumeName == "Panel_log") run->AddPanelTowardLAr();
    //     // else if (startVolumeName == "frontPENLayer_log" ||
    //     //          startVolumeName == "backPENLayer_log"  ||
    //     //          startVolumeName == "topPENLayer_log"   ||
    //     //          startVolumeName == "botPENLayer_log"
    //     //         ) run->AddPENTowardLAr();
    //     // else if (startVolumeName == "Guide_log") run->AddLightGuideTowardLAr();
    // }


    /*
    // If photon inside LAr, kill track
    if(endVolumeName == "World_log") 
    {
        track->SetTrackStatus(fStopAndKill);
        trackLength = 0.;
        run->AddLArAbsorption();
        if(startVolumeName == "Panel_log") run->AddPanelTowardLAr();
        else if (startVolumeName == "frontPENLayer_log" ||
                 startVolumeName == "backPENLayer_log"  ||
                 startVolumeName == "topPENLayer_log"   ||
                 startVolumeName == "botPENLayer_log"
                ) run->AddPENTowardLAr();
        else if (startVolumeName == "Guide_log") run->AddLightGuideTowardLAr();

    }

    const G4VProcess* pds = endPoint->GetProcessDefinedStep(); 

    // if photon is absorbed, store volume information
    if(pds->GetProcessName() == "OpAbsorption")
    {
        analysisMan->FillNtupleIColumn(7, 0);
        trackLength = 0.;
        if(endVolumeName == "Panel_log") run->AddPanelAbsorption();
        else if(endVolumeName == "Guide_log") run->AddLightGuideAbsorption();
        else if (endVolumeName == "frontPENLayer_log" ||
                 endVolumeName == "backPENLayer_log"  ||
                 endVolumeName == "topPENLayer_log"   ||
                 endVolumeName == "botPENLayer_log"
                ) run->AddPenAbsorption();
        else if (endVolumeName == "frontOuterCladding_log" ||
                 endVolumeName == "backOuterCladding_log"  ||
                 endVolumeName == "topOuterCladding_log"   ||
                 endVolumeName == "botOuterCladding_log"
                ) run->AddOuterCladdingAbsorption();
        else if (endVolumeName == "frontInnerCladding_log" ||
                 endVolumeName == "backInnerCladding_log"  ||
                 endVolumeName == "topInnerCladding_log"   ||
                 endVolumeName == "botInnerCladding_log"
                ) run->AddInnerCladdingAbsorption();
    }
*/

