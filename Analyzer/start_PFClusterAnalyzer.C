// Run with
// root -l -q start_PFClusterAnalyzer.C+
// root -l -q start_PFClusterAnalyzer.C+\(\"2017_SR\"\)


#include "TChain.h"
#include <iostream>
#include "TProof.h"

void start_PFClusterAnalyzer(){

  TChain * c = new TChain("recosimdumper/caloTree");

  //c->Add("/work/mratti/cmssw_workarea/Generation/CMSSW_10_6_0/src/RecoSimStudies/Dumpers/RecoSimDumper_V0.root");
  c->Add("/work/mratti/cmssw_workarea/Generation/CMSSW_10_6_0/src/RecoSimStudies/Dumpers/RecoSimDumper.root");

  // to run parallel with PROOF, currently not working
  //TProof::Open("");
  //c->SetProof();

  c->Process("PFClusterAnalyzer.C+");
}

