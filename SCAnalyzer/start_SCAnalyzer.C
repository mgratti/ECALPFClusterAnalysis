// Specify input and output filenames
// Run with:
// root -l -q start_SCAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"

void start_SCAnalyzer(){

  TString inPath = "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/dumpedFiles/";
  //TString inPath = "root://t3dcachedb.psi.ch:1094//pnfs/psi.ch/cms/trivcat/store/user/mratti/old_dumperOutput/";
  TString outPath = "./outputfiles/";

  std::vector<TString> inFileNames = {
    
     //"gjetEM_Et40toInfGeV_wTk_all_wPU_noiseCond2021_pfrhRef_seedRef_wMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_y2021_newDumper_n150_njd0.root",
     //"test_1000.root"
     "gjetEM_Et40toInfGeV_wTk_all_wPU_noiseCond2021_pfrhRef_seedRef_wMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_y2021_fixedDumper_n150_njd0.root"

  };  

  std::vector<TString> inFileNamesToUse = inFileNames;
  TString outFileNameToUse = "test_fixedDumper.root";

  
  TChain * c = new TChain("recosimdumper/caloTree");

  for(unsigned int i=0; i<inFileNamesToUse.size();i++){

    TString inFileNameToUse = inFileNamesToUse.at(i);
    
    c->Add(inPath+"/"+inFileNameToUse);

    std::cout << "\n*************************************" << std::endl;
    std::cout << "SCAnalyzer" << std::endl;
    std::cout << "*************************************" << std::endl;
    std::cout << "\n Going to run analysis on file " << inPath+"/"+inFileNameToUse << "\n" << std::endl;
    
  }

  c->Process("SCAnalyzer.C+", outPath + "/" + outFileNameToUse);


}

