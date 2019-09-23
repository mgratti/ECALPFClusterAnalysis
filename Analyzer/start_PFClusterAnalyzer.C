// Run with
// root -l -q start_PFClusterAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"

void start_PFClusterAnalyzer(){

  TString inFileName = "/t3home/anlyon/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/dumpedFiles/dumped_singlePhoton_5k_EB_test.root";
  TString outFileName = "./outputfiles/histo_singlePhoton_5k_EB.root";

  TChain * c = new TChain("recosimdumper/caloTree");
  c->Add(inFileName);

  //TProof::Open("");
  //c->SetProof();

  c->Process("PFClusterAnalyzer.C+", outFileName);

}

