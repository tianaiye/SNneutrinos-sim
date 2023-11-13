#include "SNneutrinosSimDetectorConstruction.hh"
// #include "SNneutrinosSimDetectorMessenger.hh"

#include "G4Element.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh" 

SNneutrinosSimDetectorConstruction::SNneutrinosSimDetectorConstruction()
{
   waterMPT = new G4MaterialPropertiesTable();
   steelMPT = new G4MaterialPropertiesTable();
  //  SMPT = new G4MaterialPropertiesTable();
   CsMPT = new G4MaterialPropertiesTable();
   reflectorMPT = new G4MaterialPropertiesTable();

}

SNneutrinosSimDetectorConstruction::~SNneutrinosSimDetectorConstruction()
{
    // delete fDetectorMessenger;
}

G4VPhysicalVolume *SNneutrinosSimDetectorConstruction::Construct() 
{
    DefineMaterials();

    SetOpticalProperties();


    return SetupWTandCryostat();
}

void SNneutrinosSimDetectorConstruction::DefineMaterials()
{
  G4NistManager *nist = G4NistManager::Instance();
  nist->FindOrBuildMaterial("G4_Galactic");
  steelMat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  waterMat = nist->FindOrBuildMaterial("G4_WATER");
  // G4OpticalSurface* opTankSurface = new G4OpticalSurface("opTankSurface");
  // PMTMat = nist->FindOrBuildMaterial("G4_Cs");

  
  
}

void SNneutrinosSimDetectorConstruction::SetOpticalProperties()
{
    //Cerenkov photons are generated only in media where the user has provided an index of refraction

    // G4int nEntries = 2;
    // G4double energy[nEntries] = {1.77*eV, 3.26*eV}; 
    // // optical properties of materials 
    // G4double waterRIndex[nEntries] = {1.33, 1.33};  
    // G4double waterAbsorption[nEntries] = {1.60 *m, 87.95*m}; //1.60 *m, 87.95*m   // doi:10.1364/AO.36.008710
    // waterMPT->AddProperty("RINDEX", energy, waterRIndex, nEntries)->SetSpline(true);
    // waterMPT->AddProperty("ABSLENGTH", energy, waterAbsorption, nEntries)->SetSpline(true);
    // waterMat->SetMaterialPropertiesTable(waterMPT);
    
  
    // G4double steelRIndex[nEntries] = {2.76, 2.76};  
    // G4double steelAbsorption[nEntries] = {2 *m, 2*m};     //CHECK!!!
    //steelMPT->AddProperty("RINDEX", energy, steelRIndex, nEntries)->SetSpline(true);
    //steelMPT->AddProperty("ABSLENGTH", energy, steelAbsorption, nEntries)->SetSpline(true);
    //steelMat->SetMaterialPropertiesTable(steelMPT);
    
  

    // G4double PMTRIndex[nEntries] = {0., 0.};  
    // G4double PMTAbsorption[nEntries] = {0. *m, 0.*m};     //CHECK!!!
    // CsMPT->AddProperty("RINDEX", energy, PMTRIndex, nEntries)->SetSpline(true);
    // CsMPT->AddProperty("ABSLENGTH", energy, PMTAbsorption, nEntries)->SetSpline(true);
    // PMTMat->SetMaterialPropertiesTable(CsMPT);

}

auto SNneutrinosSimDetectorConstruction::SetupWTandCryostat() -> G4VPhysicalVolume*
{


// Get materials
  auto* worldMaterial = G4Material::GetMaterial("G4_Galactic");
  //auto* waterMat = G4Material::GetMaterial("G4_WATER");
  //auto* steelMat      = G4Material::GetMaterial("G4_STAINLESS-STEEL");

  /****** Set material properties ******/
  G4int nEntries = 2;
  G4double energy[nEntries] = {1.77*eV, 3.26*eV}; 
  
  // Steel
  G4double steelRIndex[nEntries] = {2.76, 2.76};  
  G4double steelAbsorption[nEntries] = {2 *m, 2*m}; 
  steelMPT->AddProperty("RINDEX", energy, steelRIndex, nEntries)->SetSpline(true);
  steelMPT->AddProperty("ABSLENGTH", energy, steelAbsorption, nEntries)->SetSpline(true);
  steelMat->SetMaterialPropertiesTable(steelMPT); 

  // Water
  G4double waterRIndex[nEntries] = {1.33, 1.33};  
  G4double waterAbsorption[nEntries] = {1.60 *m, 87.95*m}; //1.60 *m, 87.95*m   // doi:10.1364/AO.36.008710
  waterMPT->AddProperty("RINDEX", energy, waterRIndex, nEntries)->SetSpline(true);
  waterMPT->AddProperty("ABSLENGTH", energy, waterAbsorption, nEntries)->SetSpline(true);
  G4OpticalSurface* waterOP = new G4OpticalSurface("WaterSurface", glisur, polished, dielectric_dielectric);
  waterOP->SetMaterialPropertiesTable(waterMPT);
  waterMat->SetMaterialPropertiesTable(waterMPT);

  // Reflective surface (properties TBD)
  G4double refRIndex[nEntries] = {1.5, 1.5};
  G4double refAbsorption[nEntries] = {2 *m, 2*m};
  G4double refReflectivity[nEntries] = {0.9, 0.9};  
  reflectorMPT->AddProperty("RINDEX", energy, refRIndex, nEntries);
  reflectorMPT->AddProperty("ABSLENGTH", energy, refAbsorption, nEntries);
  reflectorMPT->AddProperty("REFLECTIVITY", energy, refReflectivity, nEntries);
  G4OpticalSurface *reflectorMat = new G4OpticalSurface("Reflector",
      glisur,
      polished,
      dielectric_metal);

  reflectorMat->SetMaterialPropertiesTable(reflectorMPT);

  /******* Build volumes ********/

  G4double stone       = 500.0;  // Hall wall thickness 5 m
  G4double hallrad     = 600.0;  // Hall cylinder diam 12 m
  G4double hallhheight = 850.0;  // Hall cylinder height 17 m
  G4double offset = 200.0;  // shift cavern floor to keep detector centre at origin
 
  // water tank
  G4double tankwalltop = 0.6;  // water tank thickness at top 6 mm
  G4double tankwallbot = 0.8;  // water tank thickness at bottom 8 mm
  G4double tankrad     = 550;  // water tank diam 11 m
  G4double tankheight = 650;  // water tank height 13 m
  // cryostat
  G4double cryowall   = 3.0;                   // cryostat wall thickness from GERDA
  G4double vacgap     = 1.0;                   // vacuum gap between walls
  G4double cryrad     = 350.0; //fCryostatOuterRadius;  // 350.0;  // cryostat diam 7 m
  G4double cryhheight = 350.0; //fCryostatHeight;       // 350.0;  // cryostat height 7 m
  // PMT
  G4double PMTrad     = 10.2;   // diameter 8 inch
  G4double PMTheight  = 2.0;    //random value

 //
  // World (hall)
  //
  auto* worldSolid =
    new G4Tubs("World", 0.0 * cm, (hallrad + stone + 0.1) * cm,
               (hallhheight + stone + offset + 0.1) * cm, 0.0, CLHEP::twopi);
  auto* fWorldLogical  = new G4LogicalVolume(worldSolid, worldMaterial, "World_log");
  auto* fWorldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fWorldLogical,
                                           "World_phys", nullptr, false, 0);

  //
  // Tank
  //
  auto* tankSolid =
    new G4Cons("Tank", 0.0 * cm, (tankrad + tankwallbot) * cm, 0.0 * cm,
               (tankrad + tankwalltop) * cm, tankheight * cm, 0.0, CLHEP::twopi);
  auto* fTankLogical  = new G4LogicalVolume(tankSolid, steelMat, "fTankLogical");
  auto* fTankPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fTankLogical,
                                          "fTankPhysical", fWorldLogical, false, 0, true);

  //
  // Water
  //
  auto* waterSolid     = new G4Tubs("Water", 0.0 * cm, tankrad * cm,
                                    (tankheight - tankwallbot) * cm, 0.0, CLHEP::twopi);
  auto* fWaterLogical  = new G4LogicalVolume(waterSolid, waterMat, "fWaterLogical");
  auto* fWaterPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fWaterLogical,
                                           "fWaterPhysical", fTankLogical, false, 0, true);

  //
  // outer cryostat
  //
  auto* cryostatSolid =
    new G4Tubs("Cryostat", 0.0 * cm, cryrad * cm, cryhheight * cm, 0.0, CLHEP::twopi);
  auto* fcryostatLogical  = new G4LogicalVolume(cryostatSolid, steelMat, "fcryostatLogical");
  auto* fcryostatPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), fcryostatLogical,
                                          "fcryostatPhysical", fWaterLogical, false, 0, true);

  G4LogicalBorderSurface *tankSurface = new G4LogicalBorderSurface("tankSurface", fWaterPhysical, fTankPhysical, reflectorMat);
  G4LogicalBorderSurface *cryoSurface = new G4LogicalBorderSurface("cryoSurface", fcryostatPhysical, fWaterPhysical, reflectorMat);

  G4OpticalSurface* opticalSurface = dynamic_cast<G4OpticalSurface*>(
  tankSurface->GetSurface(fWaterPhysical, fTankPhysical)->GetSurfaceProperty(),
  cryoSurface->GetSurface(fcryostatPhysical, fWaterPhysical)->GetSurfaceProperty());
  G4cout << "******  opticalSurface->DumpInfo:" << G4endl;
  if(opticalSurface)
  {
    opticalSurface->DumpInfo();
  }
  G4cout << "******  end of opticalSurface->DumpInfo" << G4endl;


  // /* ------------- Surface -------------- */
  // /* Try LogicalSkinSurface */

  // G4OpticalSurface* opTankSurface = new G4OpticalSurface("TankSurface");
  // G4double rindex[nEntries] = {1.35, 1.40};
  // G4double reflectivity[nEntries] = {0.9, 0.9};
  // // G4double efficiency[nEntries] = {0.8, 0.1};
  // opTankSurface->SetType(dielectric_metal); //dielectric_dielectric
  // opTankSurface->SetFinish(polished);
  // opTankSurface->SetModel(glisur);
  // SMPT->AddProperty("REFLECTIVITY", energy, reflectivity, nEntries);
  // // SMPT->AddProperty("EFFICIENCY", energy, efficiency, nEntries);
  // opTankSurface->SetMaterialPropertiesTable(SMPT);
  // new G4LogicalSkinSurface("TankSurface", fTankLogical, opTankSurface);
 
 
  // /* Try LogicalBorderSurface */
  
  // G4int nEntries = 2;
  // G4double energy[nEntries] = {1.77*eV, 3.26*eV}; 
  // G4double reflectivity[nEntries] = {1, 1};
  // G4double transmittance[nEntries] = {0, 0};
  // G4double efficiency[nEntries] = {1, 1};
  // G4double rindex[nEntries] = {1.4, 1.5};
  // G4double slc[nEntries] = {0.1, 0.1};
  // G4double ssc[nEntries] = {0.1, 0.1};
  // G4double bsc[nEntries] = {0.1, 0.1};

  // SMPT->AddProperty("REFLECTIVITY", energy, reflectivity, nEntries);
  // SMPT->AddProperty("TRANSMITTANCE", energy, transmittance, nEntries);
  // SMPT->AddProperty("EFFICIENCY", energy, efficiency, nEntries);
  // SMPT->AddProperty("RINDEX", energy, rindex, nEntries);
  // SMPT->AddProperty("SPECULARLOBECONSTANT", energy, slc, nEntries);
  // SMPT->AddProperty("SPECULARSPIKECONSTANT", energy, ssc, nEntries);
  // SMPT->AddProperty("BACKSCATTERCONSTANT", energy, bsc, nEntries);


  // G4OpticalSurface* surface = new G4OpticalSurface("Surface");
  // surface->SetType(dielectric_dielectric); //https://geant4.kek.jp/lxr/source/examples/extended/optical/OpNovice2/boundary.mac
  // surface->SetFinish(polishedfrontpainted);
  // surface->SetModel(unified);
  // surface->SetMaterialPropertiesTable(SMPT);
  // surface->SetSigmaAlpha(0.);
  // G4LogicalBorderSurface *refSurface1 = new G4LogicalBorderSurface("Surface1", fWaterPhysical, fTankPhysical, surface);
  // G4LogicalBorderSurface *refSurface2 = new G4LogicalBorderSurface("Surface2", fcryostatPhysical, fWaterPhysical, surface);

  // G4OpticalSurface* opticalSurface = dynamic_cast<G4OpticalSurface*>(
  // refSurface1->GetSurface(fWaterPhysical, fTankPhysical)->GetSurfaceProperty(),
  // refSurface2->GetSurface(fcryostatPhysical, fWaterPhysical)->GetSurfaceProperty());
  // G4cout << "******  opticalSurface->DumpInfo:" << G4endl;
  // if(opticalSurface)
  // {
  //   opticalSurface->DumpInfo();
  // }
  // G4cout << "******  end of opticalSurface->DumpInfo" << G4endl;




/*
  //
  // PMT
  //
  G4double pos_x = 0.;
  G4double pos_y = 0.;
  G4double pos_z = 0.;
  G4double pos_r = 0.;
  G4double pos_theta = 0.;

  auto* PMTSolid =
    new G4Tubs("PMT", 0.0 * cm, PMTrad * cm, PMTheight *  cm, 0.0, CLHEP::twopi);
  auto* fPMTLogical  = new G4LogicalVolume(PMTSolid, PMTMat, "PMT_log");
 
 for (int j=1; j<=2; j++){ //100 PMT at the bottom
    for (int i=1;i<=50; i++) {
      pos_r = G4double((tankrad - cryrad) * j /3.) + cryrad;
      pos_theta = G4double(CLHEP::twopi*i/50);
      pos_x = pos_r * cos(pos_theta);
      pos_y = pos_r * sin(pos_theta);
      pos_z = -tankheight;
      new G4PVPlacement(nullptr, G4ThreeVector(pos_x*cm, pos_y*cm, pos_z*cm), fPMTLogical, "PMT_phys", fWaterLogical, false, 0);
      }
  }  
  
  for (int j=1; j<=3; j++) { //300 PMT at the later surface 80cm from the cryostat
    for (int i=0;i<100; i++) {
      pos_theta = double(CLHEP::twopi)*i/100;
      G4RotateY3D rotTheta; //(0);
      G4cout << pos_theta << G4endl;
      G4int quad=i/25;
      G4cout << "quad " << quad << G4endl;
      G4double pos_theta_mod =pos_theta-quad*CLHEP::pi/2.;
      G4cout << "pos_theta_mod " << pos_theta_mod << G4endl;
      if (quad==0 || quad==2){
        rotTheta= G4RotateY3D(CLHEP::pi/2.+pos_theta_mod);}
      else{
        rotTheta= G4RotateY3D(pos_theta_mod);}
      G4RotateX3D rotPhi(CLHEP::pi/2.);
      pos_r = 80.0 + cryrad;
      pos_x = pos_r * cos(pos_theta);
      pos_y = pos_r * sin(pos_theta);
      G4Translate3D shift(pos_x*cm, pos_y*cm,  (2*tankheight * j/4.-tankheight) *cm);
      auto transform = shift*rotPhi*rotTheta; 
      new G4PVPlacement(transform, fPMTLogical, "PMT_phys", fWaterLogical, false, 0);
    } 
  }
   
*/

  //
  // Visualization
  //
  G4Color testColor_water(170 / 255., 191 / 225., 219 / 225.); //170, 191, 219
  auto*   testVisAtt_water = new G4VisAttributes(testColor_water);
  testVisAtt_water->SetVisibility(true);
  auto* greyVisAtt = new G4VisAttributes(G4Colour::Grey());
  auto* greenVisAtt = new G4VisAttributes(G4Colour::Green());
  greyVisAtt->SetVisibility(true);

  fTankLogical->SetVisAttributes(greyVisAtt);
  fWaterLogical->SetVisAttributes(testVisAtt_water);
  fcryostatLogical->SetVisAttributes(greyVisAtt);
  // fPMTLogical->SetVisAttributes(greenVisAtt);
  

    return fWorldPhysical;
}
