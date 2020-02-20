// Specify input and output filenames
// Run with:
// root -l -q start_SCAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"

void start_SCAnalyzer(){

  std::vector<TString> inFileNames = {
    "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/dumpedFiles/gjetEM_test_y2021_G1_v1_t33_n-1.root"
  };  
  std::vector<TString> outFileNames = {
    "./outputfiles/gjetEM_test_y2021_G1_v1_t33_n-1.root"
  };  

  std::vector<TString> inFileNamesToUse = inFileNames;
  std::vector<TString> outFileNamesToUse = outFileNames;

  if(inFileNamesToUse.size()!=outFileNamesToUse.size()) throw "different size of input and output names"; 

  for(unsigned int i=0; i<inFileNamesToUse.size();i++){

    TString inFileName = inFileNamesToUse.at(i);
    TString outFileName = outFileNamesToUse.at(i);
    
    TChain * c = new TChain("recosimdumper/caloTree");
    c->Add(inFileName);

    std::cout << "\n*************************************" << std::endl;
    std::cout << "SCAnalyzer" << std::endl;
    std::cout << "*************************************" << std::endl;
    std::cout << "\n Going to run analysis on file " << inFileName << "\n" << std::endl;
    
    c->Process("SCAnalyzer.C+", outFileName);
  }
}

