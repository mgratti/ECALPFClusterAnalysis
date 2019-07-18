// Run with
// root -l -q start_PFClusterAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"

void start_PFClusterAnalyzer(){

  TString inFileName = "/work/mratti/cmssw_workarea/Generation/CMSSW_10_6_0/src/RecoSimStudies/Dumpers/test/outputfiles/test_5000_updated4_EB_doubleGamma.root";
  TString outFileName = "./outputfiles/test_5000_updated5_EB_doubleGamma.root";

  TChain * c = new TChain("recosimdumper/caloTree");
  c->Add(inFileName);

  //TProof::Open("");
  //c->SetProof();

  c->Process("PFClusterAnalyzer.C+", outFileName);

}

