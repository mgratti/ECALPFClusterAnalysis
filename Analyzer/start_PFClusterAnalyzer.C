// Run with
// root -l -q start_PFClusterAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"

void start_PFClusterAnalyzer(){

  TString inFileName = "/t3home/anlyon/CMSSW_10_6_0/src/RecoSimStudies/Dumpers/test/outputfiles/dumped_photon_Et1to100GeV_closeEcal_EB_noPU_pfrh2.0_seed3.0_V00_V02_n150000_withOldDumper.root";
  
  TString outFileName = "./outputfiles/histo_photon_Et1to100GeV_closeEcal_EB_noPU_pfrh2.0_seed3.0_V00_V02_n150000_withOldDumper.root";

  TChain * c = new TChain("recosimdumper/caloTree");
  c->Add(inFileName);

  //TProof::Open("");
  //c->SetProof();

  c->Process("PFClusterAnalyzer.C+", outFileName);

}

