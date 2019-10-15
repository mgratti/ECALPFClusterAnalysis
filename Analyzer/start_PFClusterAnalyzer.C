// Run with
// root -l -q start_PFClusterAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"

void start_PFClusterAnalyzer(){

  TString inFileName = "/t3home/anlyon/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/dumpedFiles/dumped_photon_Et1to100GeV_closeEcal_EB_noPU_pfrh1.0_seed3.0_V01_v31_n15000.root";
  
  TString outFileName = "./outputfiles/histo_photon_Et1to100GeV_closeEcal_EB_noPU_pfrh1.0_seed3.0_V01_v31_n15000_EB.root";

  TChain * c = new TChain("recosimdumper/caloTree");
  c->Add(inFileName);

  //TProof::Open("");
  //c->SetProof();

  c->Process("PFClusterAnalyzer.C+", outFileName);

}

