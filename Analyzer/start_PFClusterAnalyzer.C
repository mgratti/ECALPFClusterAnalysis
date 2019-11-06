// Run with
// root -l -q start_PFClusterAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"


void start_PFClusterAnalyzer(TString inFileName, TString inDirectory, string matchingMethod){


   //----------- USER'S DECISION BOARD --------------//

   // Information on the file to be processed
   //TString inDirectory = "/t3home/anlyon/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/dumpedFiles/";
   //TString inFileName = "dumped_photon_E0.1to200GeV_closeEcal_EE_wPU_pfrhRef_seedRef_V01_v64_n30000";

   cout << "I enter here" << endl;

   // Choose one caloParticle - cluster method 
   Bool_t doMatching_numberOfHits = false;
   Bool_t doMatching_simFraction  = false;
   Bool_t doMatching_deltaR       = false;

   if(matchingMethod == "numberOfHits"){
      doMatching_numberOfHits = true;
      cout << "number of hits" << endl;
   }
   else if(matchingMethod == "simFraction"){
      doMatching_simFraction = true;
      cout << "sim fraction" << endl;
   }
   else if(matchingMethod == "deltaR"){
      doMatching_deltaR = true;
      cout << "delta R" << endl;
   }

   //------------------------------------------------//

   //TString inFileName = infilename.c_str();

   TString inFile = inDirectory + inFileName + ".root";

   TChain * c = new TChain("recosimdumper/caloTree");
   c->Add(inFile);


   //create ouputFile
   TString outDirectory =  "./outputfiles/";
   inFileName.Remove(0,7);
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
   cout << "end" << endl;
}

