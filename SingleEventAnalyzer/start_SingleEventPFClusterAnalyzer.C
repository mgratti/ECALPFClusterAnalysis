// Specify input and output filenames
// Run with:
// root -l -q start_SingleEventPFClusterAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"

void start_SingleEventPFClusterAnalyzer(){

  std::vector<TString> inFileNames = {
    "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_shs1.0_y2023_T2_v1_t12_s1_n150.root",
    "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_shs2.0_y2023_T2_v1_t12_s2_n150.root",
    "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_shs0.5_y2023_T2_v1_t12_s0.5_n150.root",
    "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_shs0.01_y2023_T2_v1_t12_s0.01_n150.root",
    "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_shs100.0_y2023_T2_v1_t12_s100_n150.root",
    "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_shs10.0_y2023_T2_v1_t12_s10_n150.root",
    "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_shs0.1_y2023_T2_v1_t12_s0.1_n150.root",
  };

  std::vector<TString> outFileNames = {
   "./outputfiles/T2_v1_t12_s1.root", 
   "./outputfiles/T2_v1_t12_s2.root", 
   "./outputfiles/T2_v1_t12_s0.5.root", 
   "./outputfiles/T2_v1_t12_s0.01.root", 
   "./outputfiles/T2_v1_t12_s100.root", 
   "./outputfiles/T2_v1_t12_s10.root", 
   "./outputfiles/T2_v1_t12_s0.1.root", 
  };

  TString dir = "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/";

  std::vector<TString> inFileNames_maxSigmaDist = {
   dir + "photon_E1.0to200GeV_closeEcal_EEfar_wPU_pfrh1.0_seed1.0_thrXtalEBXtalEE_shs1.0_maxd1.0_y2023_T2_v6_t11_d1_n15.root", 
   dir + "photon_E1.0to200GeV_closeEcal_EEfar_wPU_pfrh1.0_seed1.0_thrXtalEBXtalEE_shs1.0_maxd5.0_y2023_T2_v6_t11_d5_n15.root", 
   dir + "photon_E1.0to200GeV_closeEcal_EEfar_wPU_pfrh1.0_seed1.0_thrXtalEBXtalEE_shs1.0_maxd10.0_y2023_T2_v6_t11_d10_n15.root", 
   dir + "photon_E1.0to200GeV_closeEcal_EEfar_wPU_pfrh1.0_seed1.0_thrXtalEBXtalEE_shs1.0_maxd20.0_y2023_T2_v6_t11_d20_n15.root", 
  };

  std::vector<TString> outFileNames_maxSigmaDist = {
   "./outputfiles/T2_v6_t11_d1_n15.root", 
   "./outputfiles/T2_v6_t11_d5_n15.root", 
   "./outputfiles/T2_v6_t11_d10_n15.root", 
   "./outputfiles/T2_v6_t11_d20_n15.root", 
  };

  //std::vector<TString> inFileNamesToUse = inFileNames_maxSigmaDist;
  //std::vector<TString> outFileNamesToUse = outFileNames_maxSigmaDist;
  std::vector<TString> inFileNamesToUse = inFileNames;
  std::vector<TString> outFileNamesToUse = outFileNames;

  if(inFileNamesToUse.size()!=outFileNamesToUse.size()) throw "different size of input and output names"; 

  for(unsigned int i=0; i<inFileNamesToUse.size();i++){

    TString inFileName = inFileNamesToUse.at(i);
    TString outFileName = outFileNamesToUse.at(i);
    
    TChain * c = new TChain("recosimdumper/caloTree");
    c->Add(inFileName);

    std::cout << "\n*************************************" << std::endl;
    std::cout << "SingleEventPFClusterAnalyzer" << std::endl;
    std::cout << "*************************************" << std::endl;
    std::cout << "\n Going to run analysis on file " << inFileName << "\n" << std::endl;
    
    c->Process("SingleEventPFClusterAnalyzer.C+", outFileName);
  }
}

  //TString inFileName = "/work/anlyon/dumpedFiles/photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_y2023_T2_v1_t12_n30000.root";
  //TString inFileName = "/work/anlyon/dumpedFiles/photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh2.0_seed3.0_thrXtalEBXtalEE_y2023_T2_v2_t23_n30000.root";
  //TString inFileName = "/work/anlyon/dumpedFiles/photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh1.0_seed2.0_thrXtalEBXtalEE_y2023_T2_v2_t12_n30000.root";
  //TString inFileName = "/work/anlyon/dumpedFiles/photon_E1.0to100GeV_closeEcal_EB_wPU_pfrhRef_seedRef_thrXtalEBXtalEE_y2023_T2_v4_t0_n30000.root";
 
  //TString inFileName = "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/T2_v1_t11_nj1.root";
  //TString inFileName = "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/T2_v1_t12_nj1.root";
  

  //TString outFileName = "./outputfiles/T2_v1_t12_EB.root";
  //TString outFileName = "./outputfiles/T2_v2_t23_EE_n30000.root";
  //TString outFileName = "./outputfiles/T2_v2_t12_EE.root";
  //TString outFileName = "./outputfiles/T2_v4_t0_wPU_EB.root";

  //TString outFileName = "./outputfiles/T2_v1_t11_nj1_AM.root";
  //TString outFileName = "./outputfiles/T2_v1_t12_nj1_AM.root";

