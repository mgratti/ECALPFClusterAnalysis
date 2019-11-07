// Run with
// root -l -q start_PFClusterAnalyzer.C+


#include "TChain.h"
#include <iostream>
#include "TProof.h"


void start_PFClusterAnalyzer(TString inFileName, TString inDirectory, string matchingMethod){

   Bool_t doMatching_numberOfHits = false;
   Bool_t doMatching_simFraction  = false;
   Bool_t doMatching_deltaR       = false;

   if(matchingMethod == "numberOfHits"){
      doMatching_numberOfHits = true;
   }
   else if(matchingMethod == "simFraction"){
      doMatching_simFraction = true;
   }
   else if(matchingMethod == "deltaR"){
      doMatching_deltaR = true;
   }


   TString inFile = inDirectory + inFileName + ".root";
   TFile* fileOpen = TFile::Open(inFile);
   if(!fileOpen){
      cout << "Aborting" << endl;
      exit(11);
   }

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

