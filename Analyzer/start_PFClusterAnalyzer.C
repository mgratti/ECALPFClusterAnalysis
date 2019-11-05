// Run with
// root -l -q start_PFClusterAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"


void start_PFClusterAnalyzer(){


   //----------- USER'S DECISION BOARD --------------//

   // Information on the file to be processed
   //TString inDirectory = "/t3home/anlyon/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/dumpedFiles/";
   TString inDirectory = "/work/anlyon/CMSSW_10_6_1_patch1_old/RecoSimStudies/Dumpers/test/outputfiles/dumpedFiles/";
   TString inFileName = "photon_E1.0to100GeV_closeEcal_EB_wPU_pfrhRef_seedRef_V03_v02_n30000";


   // Choose one caloParticle - cluster method 
   Bool_t doMatching_numberOfHits = false;
   Bool_t doMatching_simFraction  = false;
   Bool_t doMatching_deltaR       = true;

   //------------------------------------------------//


   TString inFile = inDirectory + inFileName + ".root";

   TChain * c = new TChain("recosimdumper/caloTree");
   c->Add(inFile);


   //create ouputFile
   TString outDirectory =  "./outputfiles/";
   //inFileName.Remove(0,7);
   TString outFileName = "histo_" + inFileName;
   if(doMatching_numberOfHits){
      outFileName += "_numberOfHits";
   }
   else if(doMatching_simFraction){
      outFileName += "_simFraction";
   }
   else if(doMatching_deltaR){
      outFileName += "_deltaR";
   }

   if(inFileName.Contains("EB")){
      outFileName += "_EB";
   }
   else if(inFileName.Contains("EE")){
      outFileName += "_EE";
   }
   TString outFile = outDirectory + outFileName + ".root";


   c->Process("PFClusterAnalyzer.C+", outFile);

}

