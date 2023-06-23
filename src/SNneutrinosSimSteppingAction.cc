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
    

    //trackLength = trackLength + track->GetStepLength() / m;

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


   
    G4double pos_x = 0.;
    G4double pos_y = 0.;
    G4double pos_z = 0.;
    G4StepPoint* aStepPoint = 0;
    //if photons ???
    if (particleName=="opticalphotons" || particleName=="gamma"){ 
        G4cout << "photon" << G4endl;  
        //if the post-step point is “fGeomBoundary”, the step end on a volume boundary
        // OUT
        //if(endPoint->GetStepStatus() == fGeomBoundary && startVolumeName == "Water_log" && endVolumeName=="Tank_log")
            aStepPoint = endPoint;
        //IN
        //if(startPoint->GetStepStatus() == fGeomBoundary && endVolumeName == "Tank_log" && startVolumeName == "Water_log")
        //    aStepPoint = startPoint;
        //NOT CROSSING VOLUME
        //else return;
        //G4cout << particleName << " " << startVolumeName << " " << endVolumeName << G4endl;
        G4ThreeVector pointInSurf = aStepPoint->GetPosition();
        pos_x = pointInSurf[0];
        pos_y = pointInSurf[1];
        pos_z = pointInSurf[2] ;
        G4cout << "step_r " << pos_x*pos_x+pos_y*pos_y << " " << 550*550*cm*cm <<G4endl;
        G4cout << "step_h " << pos_z << " " << 650*cm << G4endl;
        
        if (pos_z<((650.0-0.8))*cm){   //no PMT on the top surface
            track->SetTrackStatus(fStopAndKill);
            run->AddSurfaceDetection();
            analysisMan->FillNtupleDColumn(6, pos_x);  //pos_x
            analysisMan->FillNtupleDColumn(7, pointInSurf[1]);  //pos_y
            analysisMan->FillNtupleDColumn(8, pointInSurf[2]);  //pos_z
            analysisMan->AddNtupleRow(0);
        
        }
        G4cout << " fine" << G4endl;
    }
    else{ 
    G4cout << "ciao" << G4endl;
     aStepPoint = endPoint;
       G4ThreeVector pointInSurf = aStepPoint->GetPosition();
        pos_x = pointInSurf[0];
        pos_y = pointInSurf[1];
        pos_z = pointInSurf[2] ;
        G4cout << "NO step_r " << pos_x*pos_x+pos_y*pos_y << " " << 550*550*cm*cm <<G4endl;
        G4cout << "NO step_h " << pos_z << " " << 650*cm << G4endl;}
    


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
    return;

}
