// #ifdef G4MULTITHREADED
// #include "G4MTRunManager.hh"
// #else
#include "G4RunManager.hh"
// #endif

#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "G4UImanager.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics.hh"

#include "SNneutrinosSimDetectorConstruction.hh"
#include "SNneutrinosSimActionInitialization.hh"

#include "G4Cerenkov.hh"



int main(int argc,char** argv)
{
    //detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    if (argc == 1) ui = new G4UIExecutive(argc,argv);

    // Construct the default run manager
    //
    // #ifdef G4MULTITHREADED
    //     G4MTRunManager * runManager = new G4MTRunManager;
    // #else
    //     G4RunManager * runManager = new G4RunManager;
    // #endif

    // #ifdef G4MULTITHREADED
    // G4MTRunManager * runManager = new G4MTRunManager;
    // // G4int nThreads = std::min(G4Threading::G4GetNumberOfCores(), 4);
    // // runManager->SetNumberOfThreads(nThreads);
    // // G4cout << "===== OpNovice2 is started with "
    // //         <<  runManager->GetNumberOfThreads() << " threads =====" << G4endl;
    // #else
    G4RunManager * runManager = new G4RunManager;
    // #endif

    // Set mandatory initialization classes
    //
    // Detector construction
    runManager-> SetUserInitialization(new SNneutrinosSimDetectorConstruction());
    
    // Physics list
    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->ReplacePhysics(new G4EmStandardPhysics());
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    // opticalPhysics->SetCerenkovTrackSecondariesFirst(true);
    //opticalPhysics->SetMaxNumPhotonsPerStep(10); 
    //default 100 -->Set the step size to limit the number of photons produced (on average) to a given value (an integer N)
    physicsList->RegisterPhysics(opticalPhysics);
  
    runManager->SetUserInitialization(physicsList);

    // User action initialization
    runManager->SetUserInitialization(new SNneutrinosSimActionInitialization());

   //initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    //get the pointer to the User Interface manager 
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    if (ui)  {
        //interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else  {
        //batch mode  
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command+fileName);
    }

    // job termination
    delete visManager;
    delete runManager;
    return 0;
}