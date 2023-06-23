#include "SNneutrinosSimPrimaryGenerator.hh"

#include "g4root.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"

#include "Run.hh"

#include <fstream>
#include <random>
#include <set>

SNneutrinosSimPrimaryGenerator::SNneutrinosSimPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1);
}

SNneutrinosSimPrimaryGenerator::~SNneutrinosSimPrimaryGenerator()
{
    delete fParticleGun;
}

void SNneutrinosSimPrimaryGenerator::GeneratePrimaries(G4Event* anEvent)
{
    Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    run->AddToTotal();

    // set positrons as primary particle
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    fParticleGun->SetParticleDefinition(particleTable->FindParticle("e+")); //opticalphoton

    //
    // Simulate only inside the water tank.
    // The volume is divided into three subvolume: top, middle, bottom.
    // The middle one contains the outer cryostat
    //
    std::uniform_real_distribution<> rndm(0.0, 1.0);  // azimuth angle

    G4double WaterTankHeight = (650 + 0.8) * cm;
    G4double WaterTankRadius = (550 + 0.6) * cm;
    G4double CryostatHeight = 350 * cm;
    G4double CryostatRadius = 350 * cm; 
    //G4double Offset          = (200 - 100 - (850 - 650)) * cm;
  
    G4double Vol_top    = CLHEP::pi * WaterTankRadius * WaterTankRadius * (WaterTankHeight-CryostatHeight);
    G4double Vol_middle = CLHEP::pi * (WaterTankRadius - CryostatRadius) * (WaterTankRadius - CryostatRadius)  * (CryostatHeight);
    G4double Vol_bottom = Vol_top; 

    G4double Prob_middle = Vol_middle / (Vol_bottom + Vol_middle + Vol_top);
    G4double Prob_top = (1 - Prob_middle) / 2.;
    G4double Prob_bottom = Prob_top;
   
    std::discrete_distribution<> distribution_2({ Prob_middle, Prob_top, Prob_bottom });
    generator.seed(rd());

    G4int    where = distribution_2(generator);

    G4double px, py, pz, pos_x, pos_y, pos_z;
    G4double theta = CLHEP::twopi * rndm(generator);
    G4double phi   = 2 * CLHEP::twopi * rndm(generator);
    G4cout << where << G4endl;
    
  if(where == 0)
    {
      G4double pos_phi    = CLHEP::twopi * rndm(generator);

      pos_x = ( (WaterTankRadius - CryostatRadius) * rndm(generator) + CryostatRadius) * cos(pos_phi);
      pos_y = ( (WaterTankRadius - CryostatRadius) * rndm(generator) + CryostatRadius) * sin(pos_phi);
      pos_z = CryostatHeight * (1 - 2 * rndm(generator));
    }
  
    if(where > 0)
    {
      G4double pos_phi = CLHEP::twopi * rndm(generator);
      G4double pos_height = (WaterTankHeight - CryostatHeight) * rndm(generator) + CryostatHeight;
      G4double pos_rad = WaterTankRadius * rndm(generator);
      if(where == 2) //bottom side
        pos_height *=-1;

      pos_x = pos_rad * cos(pos_phi);
      pos_y = pos_rad * sin(pos_phi);
      pos_z = pos_height;
    }
    pos_x= 0.;
    pos_y= (500 ) * cm;
    pos_z= 0.;
    G4ThreeVector position(pos_x, pos_y, pos_z);
    fParticleGun->SetParticlePosition(position);
    G4cout << "r " << pos_x*pos_x+pos_y+pos_y << " " << 550*550*cm *cm << G4endl;
    G4cout << "h " << pos_z << " " << 650 *cm << G4endl;

    pz = -1 * std::cos(theta);
    px = std::sin(theta) * cos(phi);
    py = std::sin(theta) * sin(phi);
    G4ThreeVector momentumDir(px, py, pz);
    fParticleGun->SetParticleMomentumDirection(momentumDir);

    std::uniform_real_distribution<double> rndm_energy (1.3, 60.0);
    G4double energy = rndm_energy(generator) * 10e3 *keV;

    G4double theMass = particleTable->FindParticle("e+")->GetPDGMass();
    G4double totMomentum = std::sqrt(energy * energy + 2 * theMass * energy);
    fParticleGun->SetParticleEnergy(energy);


    
    // set optical photons random polarization --> 
    //SetOptPhotonPolar();
    
    fParticleGun->GeneratePrimaryVertex(anEvent);
    
    man->FillNtupleDColumn(0, position[0] / cm);
    man->FillNtupleDColumn(1, position[1] / cm);
    man->FillNtupleDColumn(2, position[2] / cm);
    man->FillNtupleDColumn(3, momentumDir[0]);
    man->FillNtupleDColumn(4, momentumDir[1]);
    man->FillNtupleDColumn(5, momentumDir[2]);
    //man->AddNtupleRow(0);
    
}

//It should be automatic for generated photons
/*
void SNneutrinosSimPrimaryGenerator::SetOptPhotonPolar()
{
    G4double angle = G4UniformRand() * 360.0*deg;
    SetOptPhotonPolar(angle);
}


void SNneutrinosSimPrimaryGenerator::SetOptPhotonPolar(G4double angle)
{
 if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
    {
    G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
                "the particleGun is not an opticalphoton" << G4endl;
    return;
    }

    G4ThreeVector normal (1., 0., 0.);
    G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
    G4ThreeVector product = normal.cross(kphoton);
    G4double modul2       = product*product;
    
    G4ThreeVector e_perpend (0., 0., 1.);
    if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
    G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
    
    G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
    fParticleGun->SetParticlePolarization(polar);
}
*/
