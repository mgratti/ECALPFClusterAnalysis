#define PFClusterAnalyzer_cxx
// The class definition in PFClusterAnalyzer.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.


// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("PFClusterAnalyzer.C")
// root> T->Process("PFClusterAnalyzer.C","some options")
// root> T->Process("PFClusterAnalyzer.C+")
//

#include "PFClusterAnalyzer.h"
#include <TStyle.h>
#include <TMath.h>
#include <cmath>
#include <bits/stdc++.h>
#include <stdexcept>
#include<bits/stdc++.h> 

using namespace std;


bool sortbysecdesc(const pair<float,float> &a, const pair<float,float> &b){ 
   return a.second>b.second; 
} 

//struct MatchingMap{
//   unsigned int CaloIndex;
//   unsigned int PFIndex;
//   float Score;
//};


void PFClusterAnalyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   Info("Begin", "Called with option %s", option.Data());

   TH1::SetDefaultSumw2();
   TH1::StatOverflows(kTRUE); // force overflows and underflows in the stats computation

   fout = 0;

}

void PFClusterAnalyzer::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   // here you could handle more complicated options
   TString foutName = option;


   //----------- USER'S DECISION BOARD --------------//

   //Turn to true this flag in case you want to save only one PFCluster per caloParticle
   flag_keepOnlyOnePFCluster = true;

   //------------------------------------------------//


   flag_doEB = false;
   flag_doEE = false;
   //turns flag_doEB/flag_doEE on depending on the outputFile
   std::string fileName = option.Data();
   if(fileName.find("EB") != std::string::npos){
      flag_doEB = true;
   }
   else if(fileName.find("EE") != std::string::npos){
      flag_doEE = true;
   }


   flag_doMatching_numberOfHits = false;
   flag_doMatching_score        = false;
   flag_doMatching_deltaR       = false;

   if(fileName.find("numberOfHits") != std::string::npos){
      flag_doMatching_numberOfHits = true;
   }
   else if(fileName.find("simFraction") != std::string::npos){
      flag_doMatching_score = true;
   }
   else if(fileName.find("deltaR") != std::string::npos){
      flag_doMatching_deltaR = true;
   }


   // output
   fout = new TFile(foutName, "RECREATE"); 
   if(!fout->IsOpen()) throw std::runtime_error("Output file could not be created");
   Info("SlaveBegin", "Writing on %s", foutName.Data());

   fout->mkdir("PFCluster_caloMatched");
   fout->mkdir("caloParticle");
   fout->mkdir("EtEta_binned");
   fout->mkdir("SuperCluster");
   fout->mkdir("perEvent");

   Et_keys.push_back("0_5");
   Et_keys.push_back("5_10");
   Et_keys.push_back("10_15");
   Et_keys.push_back("15_20");

   Et_keys.push_back("1_20");
   Et_keys.push_back("20_40");
   Et_keys.push_back("40_60");
   Et_keys.push_back("60_80");
   Et_keys.push_back("80_100");

   Et_edges["0_5"].first = 0.;
   Et_edges["0_5"].second = 5.;
   Et_edges["5_10"].first = 5.;
   Et_edges["5_10"].second = 10.;
   Et_edges["10_15"].first = 10.;
   Et_edges["10_15"].second = 15.;
   Et_edges["15_20"].first = 15.;
   Et_edges["15_20"].second = 20.;

   Et_edges["1_20"].first = 1.;
   Et_edges["1_20"].second = 20.;
   Et_edges["20_40"].first = 20.;
   Et_edges["20_40"].second = 40.;
   Et_edges["40_60"].first = 40.;
   Et_edges["40_60"].second = 60.;
   Et_edges["60_80"].first = 60.;
   Et_edges["60_80"].second = 80.;
   Et_edges["80_100"].first = 80.;
   Et_edges["80_100"].second = 100.;

   if(flag_doEB){
      Eta_keys.push_back("0p00_0p50");
      Eta_keys.push_back("0p00_0p20");
      Eta_keys.push_back("0p20_0p40");
      Eta_keys.push_back("0p40_0p60");
      Eta_keys.push_back("0p60_0p80");
      Eta_keys.push_back("0p80_1p00");
      Eta_keys.push_back("1p00_1p20");
      Eta_keys.push_back("1p20_1p44");
      Eta_keys.push_back("0p50_1p00");
      Eta_keys.push_back("1p00_1p44");
      Eta_keys.push_back("1p44_1p48");
      Eta_keys.push_back("1p00_1p48");
      Eta_edges["0p00_0p50"].first = 0.;
      Eta_edges["0p00_0p50"].second = 0.5;
      Eta_edges["0p00_0p20"].first = 0.;
      Eta_edges["0p00_0p20"].second = 0.2;
      Eta_edges["0p20_0p40"].first = 0.2;
      Eta_edges["0p20_0p40"].second = 0.4;
      Eta_edges["0p40_0p60"].first = 0.4;
      Eta_edges["0p40_0p60"].second = 0.6;
      Eta_edges["0p60_0p80"].first = 0.6;
      Eta_edges["0p60_0p80"].second = 0.8;
      Eta_edges["0p80_1p00"].first = 0.8;
      Eta_edges["0p80_1p00"].second = 1.;
      Eta_edges["1p00_1p20"].first = 1.;
      Eta_edges["1p00_1p20"].second = 1.2;
      Eta_edges["1p20_1p44"].first = 1.2;
      Eta_edges["1p20_1p44"].second = 1.44;
      Eta_edges["0p50_1p00"].first = 0.5;
      Eta_edges["0p50_1p00"].second = 1.0;
      Eta_edges["1p00_1p44"].first = 1.0;
      Eta_edges["1p00_1p44"].second = 1.44;
      Eta_edges["1p44_1p48"].first = 1.44;
      Eta_edges["1p44_1p48"].second = 1.479;
      Eta_edges["1p00_1p48"].first = 1.0;
      Eta_edges["1p00_1p48"].second = 1.479;
   }
   if(flag_doEE){
      Eta_keys.push_back("1p48_2p00");
      Eta_keys.push_back("2p00_2p50");
      Eta_keys.push_back("2p50_3p00");
      Eta_edges["1p48_2p00"].first = 1.479;
      Eta_edges["1p48_2p00"].second = 2.0;
      Eta_edges["2p00_2p50"].first = 2.0;
      Eta_edges["2p00_2p50"].second = 2.5;
      Eta_edges["2p50_3p00"].first = 2.5;
      Eta_edges["2p50_3p00"].second = 3.0;
   }


   fout->cd("PFCluster_caloMatched");

   int nBins_energy = 240;
   int rangeMin_energy = -5;
   int rangeMax_energy = 300;

   // initialize histograms
   if(flag_doEE){
      h_PFClusters_caloMatched_size_EE    = new TH1F("h_PFClusters_caloMatched_size_EE","h_PFClusters_caloMatched_size_EE",50,0.,50.);
      h_PFClusters_caloMatched_nRecHit_EE = new TH1F("h_PFClusters_caloMatched_nRecHit_EE","h_PFClusters_caloMatched_nRecHit_EE",50,0.,50.);
      h_PFClusters_caloMatched_nXtals_EE  = new TH1F("h_PFClusters_caloMatched_nXtals_EE","h_PFClusters_caloMatched_nXtals_EE",50,0.,50.);
      h_PFClusters_caloMatched_energy_EE  = new TH1F("h_PFClusters_caloMatched_energy_EE","h_PFClusters_caloMatched_energy_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_et_EE      = new TH1F("h_PFClusters_caloMatched_et_EE","h_PFClusters_caloMatched_et_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_eta_EE     = new TH1F("h_PFClusters_caloMatched_eta_EE","h_PFClusters_caloMatched_eta_EE",300,-3.,3.);
      h_PFClusters_caloMatched_phi_EE     = new TH1F("h_PFClusters_caloMatched_phi_EE","h_PFClusters_caloMatched_phi_EE",128,-3.2,3.2);
      h_PFClusters_caloMatched_eOverEtrue_EE = new TH1F("h_PFClusters_caloMatched_eOverEtrue_EE","h_PFClusters_caloMatched_eOverEtrue_EE",100,0.,2.);
      h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE = new TH1F("h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE","h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE",100,0.,2.);
      h_PFClusters_caloMatched_nXtals_vs_xtalEnergy_EE = new TH2F("h_PFClusters_caloMatched_nXtals_vs_xtalEnergy_EE", "h_PFClusters_caloMatched_nXtals_vs_xtalEnergy_EE", 50, 0., 50., 50., 0., 50.);
      h_PFClusters_caloMatched_nXtals_vs_energy_EE = new TH2F("h_PFClusters_caloMatched_nXtals_vs_energy_EE", "h_PFClusters_caloMatched_nXtals_vs_energy_EE", 50, 0., 50., 100., 0., 100.);
      h_PFClusters_caloMatched_nRecHit_vs_energy_EE = new TH2F("h_PFClusters_caloMatched_nRecHit_vs_energy_EE", "h_PFClusters_caloMatched_nRecHit_vs_energy_EE", 30, 0., 30., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_energy_EE = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_energy_EE", "h_PFClusters_caloMatched_nPFCLusters_vs_energy_EE", 8, 0., 8., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EE = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EE", "h_PFClusters_caloMatched_nPFCLusters_vs_caloEnergy_EE", 8, 0., 8., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_eta_EE = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_eta_EE", "h_PFClusters_caloMatched_nPFCLusters_vs_eta_EE", 270, -3., 3., 8, 0., 8.);

      //EEM
      h_PFClusters_caloMatched_EEM_eta     = new TH1F("h_PFClusters_caloMatched_EEM_eta","h_PFClusters_caloMatched_EEM_eta",300,-3.,3.);
      h_PFClusters_caloMatched_EEM_size    = new TH1F("h_PFClusters_caloMatched_EEM_size","h_PFClusters_caloMatched_EEM_size",50,0.,50.);
      h_PFClusters_caloMatched_EEM_nXtals  = new TH1F("h_PFClusters_caloMatched_EEM_nXtals","h_PFClusters_caloMatched_EEM_nXtals",50,0.,50.);
      h_PFClusters_caloMatched_EEM_energy  = new TH1F("h_PFClusters_caloMatched_EEM_energy","h_PFClusters_caloMatched_EEM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EEM_et      = new TH1F("h_PFClusters_caloMatched_EEM_et","h_PFClusters_caloMatched_EEM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EEM_phi     = new TH1F("h_PFClusters_caloMatched_EEM_phi","h_PFClusters_caloMatched_EEM_phi",128,-3.2,3.2);
      h_PFClusters_caloMatched_EEM_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EEM_eOverEtrue","h_PFClusters_caloMatched_EEM_eOverEtrue",100,0.,2.);

      //EEP
      h_PFClusters_caloMatched_EEP_eta     = new TH1F("h_PFClusters_caloMatched_EEP_eta","h_PFClusters_caloMatched_EEP_eta",300,-3.,3.);
      h_PFClusters_caloMatched_EEP_size    = new TH1F("h_PFClusters_caloMatched_EEP_size","h_PFClusters_caloMatched_EEP_size",50,0.,50.);
      h_PFClusters_caloMatched_EEP_nXtals  = new TH1F("h_PFClusters_caloMatched_EEP_nXtals","h_PFClusters_caloMatched_EEP_nXtals",50,0.,50.);
      h_PFClusters_caloMatched_EEP_energy  = new TH1F("h_PFClusters_caloMatched_EEP_energy","h_PFClusters_caloMatched_EEP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EEP_et      = new TH1F("h_PFClusters_caloMatched_EEP_et","h_PFClusters_caloMatched_EEP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EEP_phi     = new TH1F("h_PFClusters_caloMatched_EEP_phi","h_PFClusters_caloMatched_EEP_phi",128,-3.2,3.2);
      h_PFClusters_caloMatched_EEP_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EEP_eOverEtrue","h_PFClusters_caloMatched_EEP_eOverEtrue",100,0.,2.);

   }

   if(flag_doEB){
      h_PFClusters_caloMatched_size_EB    = new TH1F("h_PFClusters_caloMatched_size_EB","h_PFClusters_caloMatched_size_EB",50,0.,50.);
      h_PFClusters_caloMatched_nRecHit_EB = new TH1F("h_PFClusters_caloMatched_nRecHit_EB","h_PFClusters_caloMatched_nRecHit_EB",50,0.,50.);
      h_PFClusters_caloMatched_nXtals_EB  = new TH1F("h_PFClusters_caloMatched_nXtals_EB","h_PFClusters_caloMatched_nXtals_EB",50,0.,50.);
      h_PFClusters_caloMatched_energy_EB  = new TH1F("h_PFClusters_caloMatched_energy_EB","h_PFClusters_caloMatched_energy_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_et_EB      = new TH1F("h_PFClusters_caloMatched_et_EB","h_PFClusters_caloMatched_et_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_eta_EB     = new TH1F("h_PFClusters_caloMatched_eta_EB","h_PFClusters_caloMatched_eta_EB",300,-3.,3.);
      h_PFClusters_caloMatched_phi_EB     = new TH1F("h_PFClusters_caloMatched_phi_EB","h_PFClusters_caloMatched_phi_EB",128,-3.2,3.2);
      h_PFClusters_caloMatched_eOverEtrue_EB = new TH1F("h_PFClusters_caloMatched_eOverEtrue_EB","h_PFClusters_caloMatched_eOverEtrue_EB",100,0.,2.);
      h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB = new TH1F("h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB","h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB",100,0.,2.);
      h_PFClusters_caloMatched_nXtals_vs_xtalEnergy_EB = new TH2F("h_PFClusters_caloMatched_nXtals_vs_xtalEnergy_EB", "h_PFClusters_caloMatched_nXtals_vs_xtalEnergy_EB", 50, 0., 50., 50., 0., 50.);
      h_PFClusters_caloMatched_nXtals_vs_energy_EB = new TH2F("h_PFClusters_caloMatched_nXtals_vs_energy_EB", "h_PFClusters_caloMatched_nXtals_vs_energy_EB", 50, 0., 50., 100., 0., 100.);
      h_PFClusters_caloMatched_nRecHit_vs_energy_EB = new TH2F("h_PFClusters_caloMatched_nRecHit_vs_energy_EB", "h_PFClusters_caloMatched_nRecHit_vs_energy_EB", 30, 0., 30., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_energy_EB = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_energy_EB", "h_PFClusters_caloMatched_nPFCLusters_vs_energy_EB", 8, 0., 8., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EB = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EB", "h_PFClusters_caloMatched_nPFCLusters_vs_caloEnergy_EB", 8, 0., 8., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_eta_EB = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_eta_EB", "h_PFClusters_caloMatched_nPFCLusters_vs_eta_EB", 270, -3., 3., 8, 0., 8.);
      h_PFClusters_caloMatched_deltaR_EB    = new TH1F("h_PFClusters_caloMatched_deltaR_EB","h_PFClusters_caloMatched_deltaR_EB",300,0.,7.);
      h_PFClusters_caloMatched_EoverEtrue_iEta_iPhi_EB = new TH2F("h_PFClusters_caloMatched_EoverEtrue_iEta_iPhi_EB", "h_PFClusters_caloMatched_EoverEtrue_iEta_iPhi_EB", 172, -86., 86., 360, 1., 361.);
      h_PFClusters_caloMatched_et_iEta_iPhi_EB = new TH2F("h_PFClusters_caloMatched_et_iEta_iPhi_EB", "h_PFClusters_caloMatched_et_iEta_iPhi_EB", 172, -86., 86., 360, 1., 361.);


      //EBM
      h_PFClusters_caloMatched_EBM_eta     = new TH1F("h_PFClusters_caloMatched_EBM_eta","h_PFClusters_caloMatched_EBM_eta",300,-3.,3.);
      h_PFClusters_caloMatched_EBM_size    = new TH1F("h_PFClusters_caloMatched_EBM_size","h_PFClusters_caloMatched_EBM_size",50,0.,50.);
      h_PFClusters_caloMatched_EBM_nXtals  = new TH1F("h_PFClusters_caloMatched_EBM_nXtals","h_PFClusters_caloMatched_EBM_nXtals",50,0.,50.);
      h_PFClusters_caloMatched_EBM_energy  = new TH1F("h_PFClusters_caloMatched_EBM_energy","h_PFClusters_caloMatched_EBM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EBM_et      = new TH1F("h_PFClusters_caloMatched_EBM_et","h_PFClusters_caloMatched_EBM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EBM_phi     = new TH1F("h_PFClusters_caloMatched_EBM_phi","h_PFClusters_caloMatched_EBM_phi",128,-3.2,3.2);
      h_PFClusters_caloMatched_EBM_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EBM_eOverEtrue","h_PFClusters_caloMatched_EBM_eOverEtrue",100,0.,2.);


      //EBP
      h_PFClusters_caloMatched_EBP_eta     = new TH1F("h_PFClusters_caloMatched_EBP_eta","h_PFClusters_caloMatched_EBP_eta",300,-3.,3.);
      h_PFClusters_caloMatched_EBP_size    = new TH1F("h_PFClusters_caloMatched_EBP_size","h_PFClusters_caloMatched_EBP_size",50,0.,50.);
      h_PFClusters_caloMatched_EBP_nXtals  = new TH1F("h_PFClusters_caloMatched_EBP_nXtals","h_PFClusters_caloMatched_EBP_nXtals",50,0.,50.);
      h_PFClusters_caloMatched_EBP_energy  = new TH1F("h_PFClusters_caloMatched_EBP_energy","h_PFClusters_caloMatched_EBP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EBP_et      = new TH1F("h_PFClusters_caloMatched_EBP_et","h_PFClusters_caloMatched_EBP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EBP_phi     = new TH1F("h_PFClusters_caloMatched_EBP_phi","h_PFClusters_caloMatched_EBP_phi",128,-3.2,3.2);
      h_PFClusters_caloMatched_EBP_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EBP_eOverEtrue","h_PFClusters_caloMatched_EBP_eOverEtrue",100,0.,2.);
   }


   fout->cd("caloParticle");
   if(flag_doEE){
      h_caloParticle_size_EE       = new TH1F("h_caloParticle_size_EE","h_caloParticle_size_EE",50,0.,50.);
      h_caloParticle_energy_EE     = new TH1F("h_caloParticle_energy_EE","h_caloParticle_energy_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_simEnergy_EE  = new TH1F("h_caloParticle_simEnergy_EE","h_caloParticle_simEnergy_EE", nBins_energy, rangeMin_energy, rangeMax_energy);
      h_caloParticle_et_EE      = new TH1F("h_caloParticle_et_EE","h_caloParticle_et_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_eta_EE     = new TH1F("h_caloParticle_eta_EE","h_caloParticle_eta_EE",300,-3.,3.);
      h_caloParticle_phi_EE     = new TH1F("h_caloParticle_phi_EE","h_caloParticle_phi_EE",128,-3.2,3.2);

      h_caloParticle_EEM_size       = new TH1F("h_caloParticle_EEM_size","h_caloParticle_EEM_size",50,0.,50.);
      h_caloParticle_EEM_energy     = new TH1F("h_caloParticle_EEM_energy","h_caloParticle_EEM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEM_simEnergy  = new TH1F("h_caloParticle_EEM_simEnergy","h_caloParticle_EEM_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEM_et      = new TH1F("h_caloParticle_EEM_et","h_caloParticle_EEM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEM_eta     = new TH1F("h_caloParticle_EEM_eta","h_caloParticle_EEM_eta",300,-3.,3.);
      h_caloParticle_EEM_phi     = new TH1F("h_caloParticle_EEM_phi","h_caloParticle_EEM_phi",128,-3.2,3.2);

      h_caloParticle_EEP_size       = new TH1F("h_caloParticle_EEP_size","h_caloParticle_EEP_size",50,0.,50.);
      h_caloParticle_EEP_energy     = new TH1F("h_caloParticle_EEP_energy","h_caloParticle_EEP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEP_simEnergy  = new TH1F("h_caloParticle_EEP_simEnergy","h_caloParticle_EEP_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEP_et      = new TH1F("h_caloParticle_EEP_et","h_caloParticle_EEP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEP_eta     = new TH1F("h_caloParticle_EEP_eta","h_caloParticle_EEP_eta",300,-3.,3.);
      h_caloParticle_EEP_phi     = new TH1F("h_caloParticle_EEP_phi","h_caloParticle_EEP_phi",128,-3.2,3.2);
   }

   if(flag_doEB){
      h_caloParticle_size_EB       = new TH1F("h_caloParticle_size_EB","h_caloParticle_size_EB",50,0.,50.);
      h_caloParticle_energy_EB     = new TH1F("h_caloParticle_energy_EB","h_caloParticle_energy_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_simEnergy_EB  = new TH1F("h_caloParticle_simEnergy_EB","h_caloParticle_simEnergy_EB", nBins_energy, rangeMin_energy, rangeMax_energy);
      h_caloParticle_et_EB      = new TH1F("h_caloParticle_et_EB","h_caloParticle_et_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_eta_EB     = new TH1F("h_caloParticle_eta_EB","h_caloParticle_eta_EB",300,-3.,3.);
      h_caloParticle_phi_EB     = new TH1F("h_caloParticle_phi_EB","h_caloParticle_phi_EB",128,-3.2,3.2);

      h_caloParticle_phi_vs_eta_ifNoPFCluster_EB = new TH2F("h_caloParticle_phi_vs_eta_ifNoPFCluster_EB", "h_caloParticle_phi_vs_eta_ifNoPFCluster_EB", 50, -3.14, 3.14, 50., -1.479, 1.479);


      h_caloParticle_EBM_size       = new TH1F("h_caloParticle_EBM_size","h_caloParticle_EBM_size",50,0.,50.);
      h_caloParticle_EBM_energy     = new TH1F("h_caloParticle_EBM_energy","h_caloParticle_EBM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBM_simEnergy  = new TH1F("h_caloParticle_EBM_simEnergy","h_caloParticle_EBM_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBM_et      = new TH1F("h_caloParticle_EBM_et","h_caloParticle_EBM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBM_eta     = new TH1F("h_caloParticle_EBM_eta","h_caloParticle_EBM_eta",300,-3.,3.);
      h_caloParticle_EBM_phi     = new TH1F("h_caloParticle_EBM_phi","h_caloParticle_EBM_phi",128,-3.2,3.2);

      h_caloParticle_EBP_size       = new TH1F("h_caloParticle_EBP_size","h_caloParticle_EBP_size",50,0.,50.);
      h_caloParticle_EBP_energy     = new TH1F("h_caloParticle_EBP_energy","h_caloParticle_EBP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBP_simEnergy  = new TH1F("h_caloParticle_EBP_simEnergy","h_caloParticle_EBP_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBP_et      = new TH1F("h_caloParticle_EBP_et","h_caloParticle_EBP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBP_eta     = new TH1F("h_caloParticle_EBP_eta","h_caloParticle_EBP_eta",300,-3.,3.);
      h_caloParticle_EBP_phi     = new TH1F("h_caloParticle_EBP_phi","h_caloParticle_EBP_phi",128,-3.2,3.2);
   }

   fout->cd("SuperCluster");

   if(flag_doEB){
      h_superCluster_energy_EB = new TH1F("h_superCluster_energy_EB","h_superCluster_energy_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_superCluster_eta_EB    = new TH1F("h_superCluster_eta_EB","h_superCluster_eta_EB", 300,-3, 3);
      h_superCluster_phi_EB    = new TH1F("h_superCluster_phi_EB","h_superCluster_phi_EB", 128, -3.2, 3.2);
      h_superCluster_r9_EB     = new TH1F("h_superCluster_r9_EB","h_superCluster_r9_EB",500, 0, 1.2);
      h_superCluster_sigmaIetaIeta_EB     = new TH1F("h_superCluster_sigmaIetaIeta_EB","h_superCluster_sigmaIetaIeta_EB",40, 0, 0.04);
      h_superCluster_sigmaIetaIphi_EB     = new TH1F("h_superCluster_sigmaIetaIphi_EB","h_superCluster_sigmaIetaIphi_EB",50, -0.005, 0.005);
      h_superCluster_sigmaIphiIphi_EB     = new TH1F("h_superCluster_sigmaIphiIphi_EB","h_superCluster_sigmaIphiIphi_EB",40, 0, 0.04);
      h_superCluster_full5x5_r9_EB     = new TH1F("h_superCluster_full5x5_r9_EB","h_superCluster_full5x5_r9_EB",500, 0, 1.2);
      h_superCluster_full5x5_sigmaIetaIeta_EB     = new TH1F("h_superCluster_full5x5_sigmaIetaIeta_EB","h_superCluster_full5x5_sigmaIetaIeta_EB",40, 0, 0.04);
      h_superCluster_full5x5_sigmaIetaIphi_EB     = new TH1F("h_superCluster_full5x5_sigmaIetaIphi_EB","h_superCluster_full5x5_sigmaIetaIphi_EB",50, -0.005, 0.005);
      h_superCluster_full5x5_sigmaIphiIphi_EB     = new TH1F("h_superCluster_full5x5_sigmaIphiIphi_EB","h_superCluster_full5x5_sigmaIphiIphi_EB",40, 0, 0.04);


      h_superCluster_caloMatched_energy_EB = new TH1F("h_superCluster_caloMatched_energy_EB","h_superCluster_caloMatched_energy_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_superCluster_caloMatched_eta_EB    = new TH1F("h_superCluster_caloMatched_eta_EB","h_superCluster_caloMatched_eta_EB", 300,-3, 3);
      h_superCluster_caloMatched_phi_EB    = new TH1F("h_superCluster_caloMatched_phi_EB","h_superCluster_caloMatched_phi_EB", 128, -3.2, 3.2);
      h_superCluster_caloMatched_r9_EB     = new TH1F("h_superCluster_caloMatched_r9_EB","h_superCluster_caloMatched_r9_EB",500, 0, 1.2);
      h_superCluster_caloMatched_sigmaIetaIeta_EB     = new TH1F("h_superCluster_caloMatched_sigmaIetaIeta_EB","h_superCluster_caloMatched_sigmaIetaIeta_EB",40, 0, 0.04);
      h_superCluster_caloMatched_sigmaIetaIphi_EB     = new TH1F("h_superCluster_caloMatched_sigmaIetaIphi_EB","h_superCluster_caloMatched_sigmaIetaIphi_EB",50, -0.005, 0.005);
      h_superCluster_caloMatched_sigmaIphiIphi_EB     = new TH1F("h_superCluster_caloMatched_sigmaIphiIphi_EB","h_superCluster_caloMatched_sigmaIphiIphi_EB",40, 0, 0.04);
      h_superCluster_caloMatched_full5x5_r9_EB     = new TH1F("h_superCluster_caloMatched_full5x5_r9_EB","h_superCluster_caloMatched_full5x5_r9_EB",500, 0, 1.2);
      h_superCluster_caloMatched_full5x5_sigmaIetaIeta_EB     = new TH1F("h_superCluster_caloMatched_full5x5_sigmaIetaIeta_EB","h_superCluster_caloMatched_full5x5_sigmaIetaIeta_EB",40, 0, 0.04);
      h_superCluster_caloMatched_full5x5_sigmaIetaIphi_EB     = new TH1F("h_superCluster_caloMatched_full5x5_sigmaIetaIphi_EB","h_superCluster_caloMatched_full5x5_sigmaIetaIphi_EB",50, -0.005, 0.005);
      h_superCluster_caloMatched_full5x5_sigmaIphiIphi_EB     = new TH1F("h_superCluster_caloMatched_full5x5_sigmaIphiIphi_EB","h_superCluster_caloMatched_full5x5_sigmaIphiIphi_EB",40, 0, 0.04);
   }


   if(flag_doEE){
      h_superCluster_energy_EE = new TH1F("h_superCluster_energy_EE","h_superCluster_energy_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_superCluster_eta_EE    = new TH1F("h_superCluster_eta_EE","h_superCluster_eta_EE", 300, -3, 3);
      h_superCluster_phi_EE    = new TH1F("h_superCluster_phi_EE","h_superCluster_phi_EE", 128, -3.2, 3.2);
      h_superCluster_r9_EE     = new TH1F("h_superCluster_r9_EE","h_superCluster_r9_EE",500, 0, 1.2);
      h_superCluster_sigmaIetaIeta_EE     = new TH1F("h_superCluster_sigmaIetaIeta_EE","h_superCluster_sigmaIetaIeta_EE",40, 0, 0.04);
      h_superCluster_sigmaIetaIphi_EE     = new TH1F("h_superCluster_sigmaIetaIphi_EE","h_superCluster_sigmaIetaIphi_EE",50, -0.005, 0.005);
      h_superCluster_sigmaIphiIphi_EE     = new TH1F("h_superCluster_sigmaIphiIphi_EE","h_superCluster_sigmaIphiIphi_EE",40, 0, 0.04);
      h_superCluster_full5x5_r9_EE     = new TH1F("h_superCluster_full5x5_r9_EE","h_superCluster_full5x5_r9_EE",500, 0, 1.2);
      h_superCluster_full5x5_sigmaIetaIeta_EE     = new TH1F("h_superCluster_full5x5_sigmaIetaIeta_EE","h_superCluster_full5x5_sigmaIetaIeta_EE",40, 0, 0.04);
      h_superCluster_full5x5_sigmaIetaIphi_EE     = new TH1F("h_superCluster_full5x5_sigmaIetaIphi_EE","h_superCluster_full5x5_sigmaIetaIphi_EE",50, -0.005, 0.005);
      h_superCluster_full5x5_sigmaIphiIphi_EE     = new TH1F("h_superCluster_full5x5_sigmaIphiIphi_EE","h_superCluster_full5x5_sigmaIphiIphi_EE",40, 0, 0.04);

      h_superCluster_caloMatched_energy_EE = new TH1F("h_superCluster_caloMatched_energy_EE","h_superCluster_caloMatched_energy_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_superCluster_caloMatched_eta_EE    = new TH1F("h_superCluster_caloMatched_eta_EE","h_superCluster_caloMatched_eta_EE", 300, -3, 3);
      h_superCluster_caloMatched_phi_EE    = new TH1F("h_superCluster_caloMatched_phi_EE","h_superCluster_caloMatched_phi_EE", 128, -3.2, 3.2);
      h_superCluster_caloMatched_r9_EE     = new TH1F("h_superCluster_caloMatched_r9_EE","h_superCluster_caloMatched_r9_EE",500, 0, 1.2);
      h_superCluster_caloMatched_sigmaIetaIeta_EE     = new TH1F("h_superCluster_caloMatched_sigmaIetaIeta_EE","h_superCluster_caloMatched_sigmaIetaIeta_EE",40, 0, 0.04);
      h_superCluster_caloMatched_sigmaIetaIphi_EE     = new TH1F("h_superCluster_caloMatched_sigmaIetaIphi_EE","h_superCluster_caloMatched_sigmaIetaIphi_EE",50, -0.005, 0.005);
      h_superCluster_caloMatched_sigmaIphiIphi_EE     = new TH1F("h_superCluster_caloMatched_sigmaIphiIphi_EE","h_superCluster_caloMatched_sigmaIphiIphi_EE",40, 0, 0.04);
      h_superCluster_caloMatched_full5x5_r9_EE     = new TH1F("h_superCluster_caloMatched_full5x5_r9_EE","h_superCluster_caloMatched_full5x5_r9_EE",500, 0, 1.2);
      h_superCluster_caloMatched_full5x5_sigmaIetaIeta_EE     = new TH1F("h_superCluster_caloMatched_full5x5_sigmaIetaIeta_EE","h_superCluster_caloMatched_full5x5_sigmaIetaIeta_EE",40, 0, 0.04);
      h_superCluster_caloMatched_full5x5_sigmaIetaIphi_EE     = new TH1F("h_superCluster_caloMatched_full5x5_sigmaIetaIphi_EE","h_superCluster_caloMatched_full5x5_sigmaIetaIphi_EE",50, -0.005, 0.005);
      h_superCluster_caloMatched_full5x5_sigmaIphiIphi_EE     = new TH1F("h_superCluster_caloMatched_full5x5_sigmaIphiIphi_EE","h_superCluster_caloMatched_full5x5_sigmaIphiIphi_EE",40, 0, 0.04);
   }


   fout->cd("EtEta_binned");
   int nBins_binned = 250;
   float rangeMin_binned = 0.;
   float rangeMax_binned = 2.;
   for (TString Et_key : Et_keys){
      for (TString Eta_key: Eta_keys){
         TString histo_name            = "h_PFclusters_caloMatched_eOverEtrue_Eta" + Eta_key + "_Et" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name,histo_name,nBins_binned,rangeMin_binned,rangeMax_binned);
         TString histo_name_n          = "h_PFclusters_caloMatched_eOverEtrue_Eta" + Eta_key + "_En" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_EtaEnBinned[Eta_key][Et_key] = new TH1F(histo_name_n,histo_name_n,nBins_binned,rangeMin_binned,rangeMax_binned);

         TString histo_name_simEnergy   = "h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + Eta_key + "_Et" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name_simEnergy,histo_name_simEnergy,nBins_binned,rangeMin_binned,rangeMax_binned);
         TString histo_name_simEnergy_n = "h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + Eta_key + "_En" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEnBinned[Eta_key][Et_key] = new TH1F(histo_name_simEnergy_n,histo_name_simEnergy_n,nBins_binned,rangeMin_binned,rangeMax_binned);


         TString histo_name_efficiency   = "h_PFclusters_caloMatched_size_Eta" + Eta_key + "_Et" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiency,histo_name_efficiency,nBins_binned,rangeMin_binned,rangeMax_binned);
         TString histo_name_efficiency_n = "h_PFclusters_caloMatched_size_Eta" + Eta_key + "_En" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEnBinned_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiency_n,histo_name_efficiency_n,nBins_binned,rangeMin_binned,rangeMax_binned);

         TString histo_name_efficiencySim_n = "h_PFclusters_caloMatched_size_simEnergy_Eta" + Eta_key + "_En" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEnBinned_simEnergy_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiencySim_n,histo_name_efficiencySim_n,nBins_binned,rangeMin_binned,rangeMax_binned);
         TString histo_name_efficiencySim = "h_PFclusters_caloMatched_size_simEnergy_Eta" + Eta_key + "_Et" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEtBinned_simEnergy_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiencySim,histo_name_efficiencySim,nBins_binned,rangeMin_binned,rangeMax_binned);


         TString histo_name_size             = "h_caloParticle_size_Eta" + Eta_key + "_Et" + Et_key;
         h_caloParticle_size_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name_size,histo_name_size,nBins_binned,rangeMin_binned,rangeMax_binned);
         TString histo_name_size_simEnergy   = "h_caloParticle_size_simEnergy_Eta" + Eta_key + "_Et" + Et_key;
         h_caloParticle_size_EtaEtBinned_simEnergy[Eta_key][Et_key] = new TH1F(histo_name_size_simEnergy,histo_name_size_simEnergy,nBins_binned,rangeMin_binned,rangeMax_binned);
         TString histo_name_size_n           = "h_caloParticle_size_Eta" + Eta_key + "_En" + Et_key;
         h_caloParticle_size_EtaEnBinned[Eta_key][Et_key] = new TH1F(histo_name_size_n,histo_name_size_n,nBins_binned,rangeMin_binned,rangeMax_binned);
         TString histo_name_size_n_simEnergy = "h_caloParticle_size_simEnergy_Eta" + Eta_key + "_En" + Et_key;
         h_caloParticle_size_EtaEnBinned_simEnergy[Eta_key][Et_key] = new TH1F(histo_name_size_n_simEnergy,histo_name_size_n_simEnergy,nBins_binned,rangeMin_binned,rangeMax_binned);
      }
   }


   fout->cd("perEvent");
   for (int i=1; i<N_perEvent_plots+1; i++){
      TString histo_name = "h_caloParticle_EB_ietaiphi_" + TString::Format("%d", i);
      h_caloParticle_EB_ietaiphi.push_back(new TH2F(histo_name, histo_name, 172,-86.,86.,360,1.,361.));
      histo_name = "h_caloParticle_EEP_ixiy_" + TString::Format("%d", i);
      h_caloParticle_EEP_ixiy.push_back(new TH2F(histo_name, histo_name, 100,0.,100.,100,0.,100.));
      histo_name = "h_caloParticle_EEM_ixiy_" + TString::Format("%d", i);
      h_caloParticle_EEM_ixiy.push_back(new TH2F(histo_name, histo_name, 100,0.,100.,100,0.,100.));
      histo_name = "h_PFClusterHits_caloMatched_EB_ietaiphi_" + TString::Format("%d", i);
      h_PFClusterHits_caloMatched_EB_ietaiphi.push_back(new TH2F(histo_name, histo_name, 172,-86.,86.,360,1.,361.));
      histo_name = "h_PFClusterHits_caloMatched_EEP_ixiy_" + TString::Format("%d", i);
      h_PFClusterHits_caloMatched_EEP_ixiy.push_back(new TH2F(histo_name, histo_name, 100,0.,100.,100,0.,100.));
      histo_name = "h_PFClusterHits_caloMatched_EEM_ixiy_" + TString::Format("%d", i);
      h_PFClusterHits_caloMatched_EEM_ixiy.push_back(new TH2F(histo_name, histo_name, 100,0.,100.,100,0.,100.));
      histo_name = "h_PFClusterHits_all_EB_ietaiphi_" + TString::Format("%d", i);
      h_PFClusterHits_all_EB_ietaiphi.push_back(new TH2F(histo_name, histo_name, 172,-86.,86.,360,1.,361.));
      histo_name = "h_PFClusterHits_all_EEP_ixiy_" + TString::Format("%d", i);
      h_PFClusterHits_all_EEP_ixiy.push_back(new TH2F(histo_name, histo_name, 100,0.,100.,100,0.,100.));
      histo_name = "h_PFClusterHits_all_EEM_ixiy_" + TString::Format("%d", i);
      h_PFClusterHits_all_EEM_ixiy.push_back(new TH2F(histo_name, histo_name, 100,0.,100.,100,0.,100.));
   }



   Info("Begin", "Booked Histograms");

}

Bool_t PFClusterAnalyzer::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // When processing keyed objects with PROOF, the object is already loaded
   // and is available via the fObject pointer.
   //
   // This function should contain the \"body\" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   //fReader.SetLocalEntry(entry); // read the event 
   fReader.SetLocalEntry(entry);
   if (entry % 1000 == 0) Info("Process", "processing event %d", (Int_t)entry);

   //if(entry>1000){ throw std::invalid_argument("aborting");}

   // loop over genParticles
   //for (unsigned int igP=0; igP<genParticle_energy.GetSize(); igP++){
   //std::cout << "igP=" << igP << " energy=" << genParticle_energy[igP] << " eta=" << genParticle_eta[igP] << " phi=" << genParticle_phi[igP] << std::endl;
   //}


   //we loop on the (reco) superCluster
   for(unsigned int iSC(0); iSC<superCluster_energy.GetSize(); ++iSC){
      if(flag_doEB){
         if(superCluster_eta[iSC]>=-1.479 && superCluster_eta[iSC]<=1.479){
            h_superCluster_energy_EB->Fill(superCluster_energy[iSC]);
            h_superCluster_eta_EB->Fill(superCluster_eta[iSC]);
            h_superCluster_phi_EB->Fill(superCluster_phi[iSC]);
            h_superCluster_r9_EB->Fill(superCluster_r9[iSC]);
            h_superCluster_sigmaIetaIeta_EB->Fill(superCluster_sigmaIetaIeta[iSC]);
            h_superCluster_sigmaIetaIphi_EB->Fill(superCluster_sigmaIetaIphi[iSC]);
            h_superCluster_sigmaIphiIphi_EB->Fill(superCluster_sigmaIphiIphi[iSC]);
            h_superCluster_full5x5_r9_EB->Fill(superCluster_full5x5_r9[iSC]);
            h_superCluster_full5x5_sigmaIetaIeta_EB->Fill(superCluster_full5x5_sigmaIetaIeta[iSC]);
            h_superCluster_full5x5_sigmaIetaIphi_EB->Fill(superCluster_full5x5_sigmaIetaIphi[iSC]);
            h_superCluster_full5x5_sigmaIphiIphi_EB->Fill(superCluster_full5x5_sigmaIphiIphi[iSC]);
         }
      }
      else if(flag_doEE){
         if(superCluster_eta[iSC]<-1.479 || superCluster_eta[iSC]>1.479){
            h_superCluster_energy_EE->Fill(superCluster_energy[iSC]);
            h_superCluster_eta_EE->Fill(superCluster_eta[iSC]);
            h_superCluster_phi_EE->Fill(superCluster_phi[iSC]);
            h_superCluster_r9_EE->Fill(superCluster_r9[iSC]);
            h_superCluster_sigmaIetaIeta_EE->Fill(superCluster_sigmaIetaIeta[iSC]);
            h_superCluster_sigmaIetaIphi_EE->Fill(superCluster_sigmaIetaIphi[iSC]);
            h_superCluster_sigmaIphiIphi_EE->Fill(superCluster_sigmaIphiIphi[iSC]);
            h_superCluster_full5x5_r9_EE->Fill(superCluster_full5x5_r9[iSC]);
            h_superCluster_full5x5_sigmaIetaIeta_EE->Fill(superCluster_full5x5_sigmaIetaIeta[iSC]);
            h_superCluster_full5x5_sigmaIetaIphi_EE->Fill(superCluster_full5x5_sigmaIetaIphi[iSC]);
            h_superCluster_full5x5_sigmaIphiIphi_EE->Fill(superCluster_full5x5_sigmaIphiIphi[iSC]);
         }
      }
   }

   //count indices needed to retrieve the size
   int N_pfCl = 0;
   int N_pfCl_EEM = 0;
   int N_pfCl_EBM = 0;
   int N_pfCl_EBP = 0;
   int N_plCl_EEP = 0;

   int N_Cl = 0;
   int N_Cl_EEM = 0;
   int N_Cl_EBM = 0;
   int N_Cl_EBP = 0;
   int N_Cl_EEP = 0;

   vector<MatchingMap> matchingMap_caloParticle_PFCluster;
   if(flag_doMatching_score){
      //to match a PFCluster to a caloParticle, we first build a map that stores the index of the caloParticle, the index of the Cluster, and the score
      matchingMap_caloParticle_PFCluster = PFClusterAnalyzer::getMapCaloParticleCluster(pfCluster_energy, caloParticle_energy, caloParticle_simEnergy, simHit_energy, pfClusterHit_energy);
   }


   for (unsigned int icP=0; icP<caloParticle_energy.GetSize(); icP++){
      //counts the number of crystals per caloParticle
      int N_pfClH=0;

      //counts the number of superClusterHit per caloParticle
      int N_spClH=0;

      //this vector will contain the indices of the PFClusters that match with a given PFClusterHit.
      //vector<int> vector_matched_indices{-1};
      //vector<int> vector_matched_indices_single{-1};

      //same than above but for superCluster - superClusterHits
      vector<int> vector_spCl_matched_indices{-1};
      vector<int> vector_spCl_matched_indices_single{-1};


      N_Cl++;


      //---caloParticle---

      if(flag_doEB){
         if(TMath::Abs(caloParticle_eta[icP])<1.479){
            h_caloParticle_energy_EB->Fill(caloParticle_energy[icP]);
            h_caloParticle_simEnergy_EB->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_et_EB->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
            h_caloParticle_phi_EB->Fill(caloParticle_phi[icP]);
            h_caloParticle_eta_EB->Fill(caloParticle_eta[icP]);
         }

         if(caloParticle_eta[icP]>=-1.479 && caloParticle_eta[icP]<0){
            N_Cl_EBM++;
            h_caloParticle_EBM_energy->Fill(caloParticle_energy[icP]);
            h_caloParticle_EBM_simEnergy->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_EBM_et->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
            h_caloParticle_EBM_phi->Fill(caloParticle_phi[icP]);
            h_caloParticle_EBM_eta->Fill(caloParticle_eta[icP]);
         }

         if(caloParticle_eta[icP]>=0 && caloParticle_eta[icP]<1.479){
            N_Cl_EBP++;
            h_caloParticle_EBP_energy->Fill(caloParticle_energy[icP]);
            h_caloParticle_EBP_simEnergy->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_EBP_et->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
            h_caloParticle_EBP_phi->Fill(caloParticle_phi[icP]);
            h_caloParticle_EBP_eta->Fill(caloParticle_eta[icP]);
         }
      }

      if(flag_doEE){
         if(TMath::Abs(caloParticle_eta[icP])>1.479){
            h_caloParticle_energy_EE->Fill(caloParticle_energy[icP]);
            h_caloParticle_simEnergy_EE->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_et_EE->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
            h_caloParticle_phi_EE->Fill(caloParticle_phi[icP]);
            h_caloParticle_eta_EE->Fill(caloParticle_eta[icP]);
         }

         if(caloParticle_eta[icP]<-1.479){
            N_Cl_EEM++;
            h_caloParticle_EEM_energy->Fill(caloParticle_energy[icP]);
            h_caloParticle_EEM_simEnergy->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_EEM_et->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
            h_caloParticle_EEM_phi->Fill(caloParticle_phi[icP]);
            h_caloParticle_EEM_eta->Fill(caloParticle_eta[icP]);
         }

         if(caloParticle_eta[icP]>=1.479){
            N_Cl_EEP++;
            h_caloParticle_EEP_energy->Fill(caloParticle_energy[icP]);
            h_caloParticle_EEP_simEnergy->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_EEP_et->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
            h_caloParticle_EEP_phi->Fill(caloParticle_phi[icP]);
            h_caloParticle_EEP_eta->Fill(caloParticle_eta[icP]);
         }
      }

      //h_caloParticle_EEM_size->Fill(N_Cl_EEM); 
      //h_caloParticle_EBM_size->Fill(N_Cl_EBM);
      //h_caloParticle_EBP_size->Fill(N_Cl_EBP);
      //h_caloParticle_EEP_size->Fill(N_Cl_EEP);

      //eta ET binned quantity that stores the number of events (equal to the number of entries of the histogram)
      for(TString Eta_key: Eta_keys){
         for(TString Et_key: Et_keys){
            double caloParticle_et = caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP])));
            if(caloParticle_et>=Et_edges[Et_key].first && caloParticle_et<Et_edges[Et_key].second 
                  && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEtBinned[Eta_key][Et_key]->Fill(1.);
            }
            double caloParticle_et_sim = caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP])));
            if(caloParticle_et_sim>=Et_edges[Et_key].first && caloParticle_et_sim<Et_edges[Et_key].second 
                  && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEtBinned_simEnergy[Eta_key][Et_key]->Fill(1.);
            }
         }
      }

      //eta energy binned quantity that stores the number of events (equal to the number of entries of the histogram)
      for(TString Eta_key: Eta_keys){
         for(TString Et_key: Et_keys){
            if(caloParticle_energy[icP]>=Et_edges[Et_key].first && caloParticle_energy[icP]<Et_edges[Et_key].second 
                  && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEnBinned[Eta_key][Et_key]->Fill(1.);
            }
            if(caloParticle_simEnergy[icP]>=Et_edges[Et_key].first && caloParticle_simEnergy[icP]<Et_edges[Et_key].second 
                  && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEnBinned_simEnergy[Eta_key][Et_key]->Fill(1.);
            }
         }
      }

      // per-event 2D maps
      if(entry<N_perEvent_plots){
         if(fabs(caloParticle_eta[icP])<1.479){
            h_caloParticle_EB_ietaiphi.at((Int_t)entry)->Fill(caloParticle_ieta[icP],caloParticle_iphi[icP],caloParticle_energy[icP]);
         } else { // end-cap
            if(caloParticle_eta[icP]>0){ 
               h_caloParticle_EEP_ixiy.at((Int_t)entry)->Fill(caloParticle_ieta[icP],caloParticle_iphi[icP],caloParticle_energy[icP]);
            } else {
               h_caloParticle_EEM_ixiy.at((Int_t)entry)->Fill(caloParticle_ieta[icP],caloParticle_iphi[icP],caloParticle_energy[icP]);
            }
         }
      }

      // caloMatched superCluster
      // Step 1: we get the indices of the superCluster associated to a caloParticle
      for(unsigned int ispCl=0; ispCl<superClusterHit_energy[icP].size(); ispCl++){

         //for each superClusterHit, we save a map that lists all the superClusters to which the hit is associated and the energy deposited in the crystal
         map<int, float>  map_superClusters = superClusterHit_energy[icP][ispCl];

         //if the hit is not matched to a superCluster, the size of the map is 0
         if(map_superClusters.size()!=0){
            //we get the pfCluster index out of the map and store the all the indices (with repetition)
            for (auto itr = map_superClusters.begin(); itr != map_superClusters.end(); ++itr) { 
               vector_spCl_matched_indices.push_back(itr->first);
            }

            //same as above, but this time the index is saved only once
            for (auto itr = map_superClusters.begin(); itr != map_superClusters.end(); ++itr) { 
               if(std::find(vector_spCl_matched_indices_single.begin(), vector_spCl_matched_indices_single.end(), itr->first)==vector_spCl_matched_indices_single.end()){
                  vector_spCl_matched_indices_single.push_back(itr->first);
               }
            }
         }//end match superClusterHit - superCluster
      }//end of loop on superClusterHits

      // Step 2: we fill the histograms with the selected superClusters
      //it has been checked that at most one SuperCluster was matched with the caloParticle
      for(unsigned int iSC(0); iSC < vector_spCl_matched_indices_single.size(); ++iSC){
         int matched_index;
         if(vector_spCl_matched_indices_single[iSC] != -1){
            matched_index = vector_spCl_matched_indices_single[iSC];
            if(flag_doEB){
               if(caloParticle_eta[icP]>=-1.479 && caloParticle_eta[icP]<=1.479){
                  h_superCluster_caloMatched_energy_EB->Fill(superCluster_energy[matched_index]); 
                  h_superCluster_caloMatched_eta_EB->Fill(superCluster_eta[matched_index]); 
                  h_superCluster_caloMatched_phi_EB->Fill(superCluster_phi[matched_index]); 
                  h_superCluster_caloMatched_r9_EB->Fill(superCluster_r9[matched_index]); 
                  h_superCluster_caloMatched_sigmaIetaIeta_EB->Fill(superCluster_sigmaIetaIeta[iSC]);
                  h_superCluster_caloMatched_sigmaIetaIphi_EB->Fill(superCluster_sigmaIetaIphi[iSC]);
                  h_superCluster_caloMatched_sigmaIphiIphi_EB->Fill(superCluster_sigmaIphiIphi[iSC]);
                  h_superCluster_caloMatched_full5x5_r9_EB->Fill(superCluster_full5x5_r9[iSC]);
                  h_superCluster_caloMatched_full5x5_sigmaIetaIeta_EB->Fill(superCluster_full5x5_sigmaIetaIeta[iSC]);
                  h_superCluster_caloMatched_full5x5_sigmaIetaIphi_EB->Fill(superCluster_full5x5_sigmaIetaIphi[iSC]);
                  h_superCluster_caloMatched_full5x5_sigmaIphiIphi_EB->Fill(superCluster_full5x5_sigmaIphiIphi[iSC]);
               }
            }
            else if(flag_doEE){
               if(caloParticle_eta[icP]<-1.479 || caloParticle_eta[icP]>1.479){
                  h_superCluster_caloMatched_energy_EE->Fill(superCluster_energy[matched_index]); 
                  h_superCluster_caloMatched_eta_EE->Fill(superCluster_eta[matched_index]); 
                  h_superCluster_caloMatched_phi_EE->Fill(superCluster_phi[matched_index]); 
                  h_superCluster_caloMatched_r9_EE->Fill(superCluster_r9[matched_index]); 
                  h_superCluster_caloMatched_sigmaIetaIeta_EE->Fill(superCluster_sigmaIetaIeta[iSC]);
                  h_superCluster_caloMatched_sigmaIetaIphi_EE->Fill(superCluster_sigmaIetaIphi[iSC]);
                  h_superCluster_caloMatched_sigmaIphiIphi_EE->Fill(superCluster_sigmaIphiIphi[iSC]);
                  h_superCluster_caloMatched_full5x5_r9_EE->Fill(superCluster_full5x5_r9[iSC]);
                  h_superCluster_caloMatched_full5x5_sigmaIetaIeta_EE->Fill(superCluster_full5x5_sigmaIetaIeta[iSC]);
                  h_superCluster_caloMatched_full5x5_sigmaIetaIphi_EE->Fill(superCluster_full5x5_sigmaIetaIphi[iSC]);
                  h_superCluster_caloMatched_full5x5_sigmaIphiIphi_EE->Fill(superCluster_full5x5_sigmaIphiIphi[iSC]);
               }
            }
         }
      }
      // end of caloMatched superCluster
/*
      // loop over pfClusterHits associated to calo particle
      for(unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){

         //for each pfClusterHit, we save a map that lists all the pfClusters to which the hit is associated and the energy deposited in the crystal
         //A given hit can belong to more than one pfCluster, in case of an overlap
         //map<int, float>  map_pfClusters = pfClusterHit_energy[icP][ipfClH];
         //float pfClH_filling_energy = 0;

         //if the hit is not matched to a PFCluster, the size of the map is 0
         //if(map_pfClusters.size()!=0){
         //N_pfClH++;

         //we get the pfCluster index out of the map and store the all the indices (with repetition)
         // for (auto itr = map_pfClusters.begin(); itr != map_pfClusters.end(); ++itr) { 
         // vector_matched_indices.push_back(itr->first);
         // pfClH_filling_energy += itr->second;
         // }

         //same as above, but this time the index is saved only once
         //for (auto itr = map_pfClusters.begin(); itr != map_pfClusters.end(); ++itr) { 
         //if(std::find(vector_matched_indices_single.begin(), vector_matched_indices_single.end(), itr->first)==vector_matched_indices_single.end()){
         //vector_matched_indices_single.push_back(itr->first);
         // }
         // }
         // }//end match pfClusterHit - pfCluster



         if(entry<N_perEvent_plots){
            if(fabs(pfClusterHit_eta[icP][ipfClH])<1.479){
               h_PFClusterHits_caloMatched_EB_ietaiphi.at((Int_t)entry)->Fill(pfClusterHit_ieta[icP][ipfClH],pfClusterHit_iphi[icP][ipfClH],pfClH_filling_energy);
               h_PFClusterHits_all_EB_ietaiphi.at((Int_t)entry)->Fill(pfClusterHit_ieta[icP][ipfClH],pfClusterHit_iphi[icP][ipfClH],pfClH_filling_energy);
            } else {
               if(pfClusterHit_eta[icP][ipfClH]>0){
                  h_PFClusterHits_caloMatched_EEP_ixiy.at((Int_t)entry)->Fill(pfClusterHit_ieta[icP][ipfClH],pfClusterHit_iphi[icP][ipfClH],pfClH_filling_energy);
                  h_PFClusterHits_all_EEP_ixiy.at((Int_t)entry)->Fill(pfClusterHit_ieta[icP][ipfClH],pfClusterHit_iphi[icP][ipfClH],pfClH_filling_energy);
               } else {
                  h_PFClusterHits_caloMatched_EEM_ixiy.at((Int_t)entry)->Fill(pfClusterHit_ieta[icP][ipfClH],pfClusterHit_iphi[icP][ipfClH],pfClH_filling_energy);
                  h_PFClusterHits_all_EEM_ixiy.at((Int_t)entry)->Fill(pfClusterHit_ieta[icP][ipfClH],pfClusterHit_iphi[icP][ipfClH],pfClH_filling_energy);
               }
            }
         }
      } // end loop pfClusterHits
*/

      //in case we want to save only one PFCluster per PFClusterHit, we keep the index of the PFCluster having the more hits
      //if(flag_keepOnlyOnePFCluster){
      //int matched_index = vector_matched_indices_single[0];
      //for(unsigned int i(1); i<=vector_matched_indices_single.size(); ++i){
      //if(count(vector_matched_indices.begin(), vector_matched_indices.end(), vector_matched_indices_single[i])>=count(vector_matched_indices.begin(), vector_matched_indices.end(), vector_matched_indices_single[i-1])){
      //matched_index = vector_matched_indices_single[i];
      //}
      //}
      //vector_matched_indices_single.clear();
      //vector_matched_indices_single.push_back(matched_index);
      //}

      //if(vector_matched_indices_single[0]==-1){
      //if(TMath::Abs(caloParticle_eta[icP])<1.479){
      //if(flag_doEB){
      //h_caloParticle_phi_vs_eta_ifNoPFCluster_EB->Fill(caloParticle_phi[icP], caloParticle_eta[icP]);
      //}
      //}
      //}

      // Step 2: we fill the histograms with the selected PFClusters

      //we loop on all the PFClusters associated to the same PFClusterHit and sum the energy, eta, phi
      double filling_energy=0;
      double filling_phi=0;
      double filling_eta=0;

      //vector_matched_indices = PFClusterAnalyzer::getMatchedIndices(matchingMap_caloParticle_PFCluster, icP, "numberOfHits");
      vector<int> vector_matched_indices;
      if(flag_doMatching_numberOfHits){
         vector_matched_indices = PFClusterAnalyzer::getMatchedIndices_numberOfHits(pfClusterHit_energy, icP);
      }
      else if(flag_doMatching_score){
         vector_matched_indices = PFClusterAnalyzer::getMatchedIndices_score(matchingMap_caloParticle_PFCluster, icP);
      }
      else if(flag_doMatching_deltaR){
         vector_matched_indices = PFClusterAnalyzer::getMatchedIndices_deltaR(pfCluster_energy, pfCluster_eta, pfCluster_phi, icP, 0.05);
      }
      for(unsigned int iMatched(0); iMatched<vector_matched_indices.size(); ++iMatched){
         int matched_index = vector_matched_indices[iMatched];
         int nOccurrences = count(vector_matched_indices.begin(), vector_matched_indices.end(), vector_matched_indices[iMatched]);
         //if(matched_index!=-1 && nOccurrences!=1)
         if(matched_index!=-1){
            filling_energy = pfCluster_energy[matched_index];
            filling_phi    = pfCluster_phi[matched_index];
            filling_eta    = pfCluster_eta[matched_index];

            if(flag_doEB){
               if(TMath::Abs(caloParticle_eta[icP])<1.479){
                  //number of recHit per PFCluster
                  h_PFClusters_caloMatched_nRecHit_EB->Fill(nOccurrences);

                  //number of recHit and energy
                  h_PFClusters_caloMatched_nRecHit_vs_energy_EB->Fill(nOccurrences, filling_energy);

                  N_pfCl++;
                  h_PFClusters_caloMatched_nXtals_EB->Fill(N_pfClH);
                  h_PFClusters_caloMatched_energy_EB->Fill(filling_energy);
                  h_PFClusters_caloMatched_et_EB->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_phi_EB->Fill(filling_phi);
                  h_PFClusters_caloMatched_eta_EB->Fill(filling_eta);
                  h_PFClusters_caloMatched_eOverEtrue_EB->Fill(filling_energy / caloParticle_energy[icP]);         
                  if(caloParticle_simEnergy[icP]!=-1){
                     h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB->Fill(filling_energy / caloParticle_simEnergy[icP]);         
                  }
                  h_PFClusters_caloMatched_nXtals_vs_energy_EB->Fill(N_pfClH, filling_energy);
                  //plot number of recHit related to energy and eta
                  h_PFClusters_caloMatched_nPFClusters_vs_energy_EB->Fill(N_pfCl, filling_energy);
                  //h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy->Fill(N_pfCl, caloParticle_energy[icP]);
                  h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EB->Fill(N_pfCl, caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
                  h_PFClusters_caloMatched_nPFClusters_vs_eta_EB->Fill(filling_eta, N_pfCl);
                  h_PFClusters_caloMatched_deltaR_EB->Fill(TMath::Sqrt((filling_eta-caloParticle_eta[icP])*(filling_eta-caloParticle_eta[icP]) + (filling_phi-caloParticle_phi[icP]) * (filling_phi-caloParticle_phi[icP])));

                  //we fill the histogram for E/Etrue < 0.5 and ET>50
                  if(filling_energy/caloParticle_simEnergy[icP] < 0.5 && caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))) > 50){
                     h_PFClusters_caloMatched_EoverEtrue_iEta_iPhi_EB->Fill(caloParticle_ieta[icP], caloParticle_iphi[icP], caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
                  }
                  if(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))) < 20){
                     h_PFClusters_caloMatched_et_iEta_iPhi_EB->Fill(caloParticle_ieta[icP], caloParticle_iphi[icP]);
                  }
               }

               if(caloParticle_eta[icP]>=-1.479 && caloParticle_eta[icP]<0){
                  N_pfCl_EBM++;

                  h_PFClusters_caloMatched_EBM_eta->Fill(filling_eta);
                  h_PFClusters_caloMatched_EBM_nXtals->Fill(N_pfClH);
                  h_PFClusters_caloMatched_EBM_energy->Fill(filling_energy);
                  h_PFClusters_caloMatched_EBM_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_EBM_phi->Fill(filling_phi);
                  h_PFClusters_caloMatched_EBM_eOverEtrue->Fill(filling_energy / caloParticle_energy[icP]);         
               }

               if(caloParticle_eta[icP]>=0 && caloParticle_eta[icP]<1.479){
                  N_pfCl_EBP++;

                  h_PFClusters_caloMatched_EBP_eta->Fill(filling_eta);
                  h_PFClusters_caloMatched_EBP_nXtals->Fill(N_pfClH);
                  h_PFClusters_caloMatched_EBP_energy->Fill(filling_energy);
                  h_PFClusters_caloMatched_EBP_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_EBP_phi->Fill(filling_phi);
                  h_PFClusters_caloMatched_EBP_eOverEtrue->Fill(filling_energy / caloParticle_energy[icP]);         
               }
            }

            if(flag_doEE){
               if(TMath::Abs(caloParticle_eta[icP])>1.479){
                  //number of recHit per PFCluster
                  h_PFClusters_caloMatched_nRecHit_EE->Fill(nOccurrences);

                  //number of recHit and energy
                  h_PFClusters_caloMatched_nRecHit_vs_energy_EE->Fill(nOccurrences, filling_energy);

                  N_pfCl++;
                  h_PFClusters_caloMatched_nXtals_EE->Fill(N_pfClH);
                  h_PFClusters_caloMatched_energy_EE->Fill(filling_energy);
                  h_PFClusters_caloMatched_et_EE->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_phi_EE->Fill(filling_phi);
                  h_PFClusters_caloMatched_eta_EE->Fill(filling_eta);
                  h_PFClusters_caloMatched_eOverEtrue_EE->Fill(filling_energy / caloParticle_energy[icP]);         
                  if(caloParticle_simEnergy[icP]!=-1){
                     h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE->Fill(filling_energy / caloParticle_simEnergy[icP]);         
                  }
                  h_PFClusters_caloMatched_nXtals_vs_energy_EE->Fill(N_pfClH, filling_energy);

               }
               //plot number of recHit related to energy and eta
               h_PFClusters_caloMatched_nPFClusters_vs_energy_EE->Fill(N_pfCl, filling_energy);
               //h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy->Fill(N_pfCl, caloParticle_energy[icP]);
               h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EE->Fill(N_pfCl, caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
               h_PFClusters_caloMatched_nPFClusters_vs_eta_EE->Fill(filling_eta, N_pfCl);


               if(caloParticle_eta[icP]<-1.479){
                  N_pfCl_EEM++;

                  h_PFClusters_caloMatched_EEM_eta->Fill(filling_eta);
                  h_PFClusters_caloMatched_EEM_nXtals->Fill(N_pfClH);
                  h_PFClusters_caloMatched_EEM_energy->Fill(filling_energy);
                  h_PFClusters_caloMatched_EEM_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_EEM_phi->Fill(filling_phi);
                  h_PFClusters_caloMatched_EEM_eOverEtrue->Fill(filling_energy / caloParticle_energy[icP]);         

               }

               if(caloParticle_eta[icP]>=1.479){
                  N_plCl_EEP++;

                  h_PFClusters_caloMatched_EEP_eta->Fill(filling_eta);
                  h_PFClusters_caloMatched_EEP_nXtals->Fill(N_pfClH);
                  h_PFClusters_caloMatched_EEP_energy->Fill(filling_energy);
                  h_PFClusters_caloMatched_EEP_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_EEP_phi->Fill(filling_phi);
                  h_PFClusters_caloMatched_EEP_eOverEtrue->Fill(filling_energy / caloParticle_energy[icP]);         


               }
            }

            //we fill the caloMatched histograms binned in eta and ET
            for(TString Eta_key: Eta_keys){
               for(TString Et_key: Et_keys){
                  //case 1: binning in eta and ET and using caloParticle_energy
                  double caloParticle_et = caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP])));
                  if(caloParticle_et>=Et_edges[Et_key].first && caloParticle_et<Et_edges[Et_key].second 
                        && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){

                     h_PFclusters_caloMatched_eOverEtrue_EtaEtBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_energy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     if(filling_energy / caloParticle_energy[icP] > 0.4 && filling_energy / caloParticle_energy[icP] < 1.4){
                        h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);
                     }
                  }
                  //case 2: binning in eta and ET and using caloParticle_simEnergy
                  double caloParticle_et_sim = caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP])));
                  if(caloParticle_et_sim>=Et_edges[Et_key].first && caloParticle_et_sim<Et_edges[Et_key].second 
                        && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){

                     h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEtBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_simEnergy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     if(filling_energy / caloParticle_simEnergy[icP] > 0.4 && filling_energy / caloParticle_simEnergy[icP] < 1.4){
                        h_PFclusters_caloMatched_size_EtaEtBinned_simEnergy_forEfficiency[Eta_key][Et_key]->Fill(1.);
                     }
                  }
                  //case 3: binning in eta and energy
                  if(caloParticle_energy[icP]>=Et_edges[Et_key].first && caloParticle_energy[icP]<Et_edges[Et_key].second 
                        && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){

                     h_PFclusters_caloMatched_eOverEtrue_EtaEnBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_energy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     if(filling_energy / caloParticle_energy[icP] > 0.4 && filling_energy / caloParticle_energy[icP] < 1.4){
                        h_PFclusters_caloMatched_size_EtaEnBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);
                     }
                  }
                  //case 4: binning in eta and simEnergy
                  if(caloParticle_simEnergy[icP]>=Et_edges[Et_key].first && caloParticle_simEnergy[icP]<Et_edges[Et_key].second 
                        && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){

                     h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEnBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_simEnergy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     if(filling_energy / caloParticle_simEnergy[icP] > 0.4 && filling_energy / caloParticle_simEnergy[icP] < 1.4){
                        h_PFclusters_caloMatched_size_EtaEnBinned_simEnergy_forEfficiency[Eta_key][Et_key]->Fill(1.);
                     }
                  }
               }
            }


            //plot number of recHit related to energy and eta
            //h_PFClusters_caloMatched_nPFClusters_vs_energy->Fill(N_pfCl, filling_energy);
            //h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy->Fill(N_pfCl, caloParticle_energy[icP]);
            //h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy->Fill(N_pfCl, caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
            //h_PFClusters_caloMatched_nPFClusters_vs_eta->Fill(filling_eta, N_pfCl);

         } //end of matched index
      }// end of loop on matched indices 

   } // end loop calo particles


   if(flag_doEB){
      h_caloParticle_size_EB->Fill(N_Cl);
      h_PFClusters_caloMatched_size_EB->Fill(N_pfCl);
   }
   else if(flag_doEE){
      h_caloParticle_size_EE->Fill(N_Cl);
      h_PFClusters_caloMatched_size_EE->Fill(N_pfCl);
   }

   //h_PFClusters_caloMatched_EEM_size->Fill(N_pfCl_EEM);
   // h_PFClusters_caloMatched_EBM_size->Fill(N_pfCl_EBM);
   // h_PFClusters_caloMatched_EBP_size->Fill(N_pfCl_EBP);
   //h_PFClusters_caloMatched_EEP_size->Fill(N_plCl_EEP);

   // Loop over PFclusters to retrieve energy not associated to any caloparticle
   if(entry<N_perEvent_plots){
      for (unsigned int iPFCl=0; iPFCl<pfCluster_energy.GetSize(); iPFCl++){
         for (unsigned int iPFClH=0; iPFClH<pfClusterHit_noCaloPart_energy[iPFCl].size(); iPFClH++){
            if(fabs(pfClusterHit_noCaloPart_eta[iPFCl][iPFClH])<1.479){
               h_PFClusterHits_all_EB_ietaiphi.at((Int_t)entry)->Fill(pfClusterHit_noCaloPart_ieta[iPFCl][iPFClH],pfClusterHit_noCaloPart_iphi[iPFCl][iPFClH],pfClusterHit_noCaloPart_energy[iPFCl][iPFClH]);
            } else {
               if(pfClusterHit_noCaloPart_eta[iPFCl][iPFCl]>0){
                  h_PFClusterHits_all_EEP_ixiy.at((Int_t)entry)->Fill(pfClusterHit_noCaloPart_ieta[iPFCl][iPFClH],pfClusterHit_noCaloPart_iphi[iPFCl][iPFClH],pfClusterHit_noCaloPart_energy[iPFCl][iPFClH]);
               } else { 
                  h_PFClusterHits_all_EEM_ixiy.at((Int_t)entry)->Fill(pfClusterHit_noCaloPart_ieta[iPFCl][iPFClH],pfClusterHit_noCaloPart_iphi[iPFCl][iPFClH],pfClusterHit_noCaloPart_energy[iPFCl][iPFClH]);
               }
            }
         }
      }
   } // end N_perEvent_plots

   return kTRUE;
}

void PFClusterAnalyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

   // Write histograms to outputfile
   //h_PFClusters_caloMatched_energy->Write();

   // write and close 
   fout->Write();
   fout->Close();

   Info("SlaveTerminate", "Wrote and closed output file");

}

void PFClusterAnalyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}











vector<MatchingMap> PFClusterAnalyzer::getMapCaloParticleCluster(const TTreeReaderArray<float>& pfCluster_energy, const TTreeReaderArray<float>& caloParticle_energy, const TTreeReaderArray<float>& caloParticle_simEnergy, const TTreeReaderArray<vector<float>>& simHit_energy, const TTreeReaderArray<vector<map<int,float>>>& pfClusterHit_energy){
   // The aim of this function consists in associating to each PFCluster the caloParticle that contributes to most of its energy

   // Map to store the indices of the caloParticle and the cluster
   map<int, int> map_caloParticle_cluster;

   //A MatchingMap is a structure containing the index of the caloParticle, the cluster and the score 
   vector<MatchingMap> matchingMap;

   // For each PFCluster, we loop on all the caloParticles, compute their associated score, and finally save the caloParticle with the highest one
   for(unsigned int iPF(0); iPF<pfCluster_energy.GetSize(); ++iPF){

      //map that stores for each caloParticle its associated score
      map<int, float> map_caloIndex_score;

      //cout << endl << endl << "PFCluster n° " << iPF << endl;

      for (unsigned int icP=0; icP<caloParticle_energy.GetSize(); icP++){

         //cout << endl << "caloParticle n " << icP << endl;
         //cout << "simEnergy: " << caloParticle_simEnergy[icP] << endl;

         double score = 0;
         for(unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){
            for (auto itr = pfClusterHit_energy[icP][ipfClH].begin(); itr != pfClusterHit_energy[icP][ipfClH].end(); ++itr) { 
               unsigned int index = itr->first;
               // the following conditions ensures that the caloParticle has some hits in the cluster
               if (index == iPF){
                  score += simHit_energy[icP][ipfClH] / caloParticle_simEnergy[icP];
               }
            }
         }
         //cout << "total score: " << score << endl;
         if(score!=0){
            map_caloIndex_score.insert({icP, score}); 
         }
      }



      //for(auto itr = map_caloIndex_score.begin(); itr != map_caloIndex_score.end(); ++itr){
      //cout << itr->first << " " << itr->second << endl;
      //}

      // this map will contain the caloParticle index associated to the maximum score
      map<int, float> map_highest_score;
      map_highest_score.insert({0, 0.});
      for(auto itr = map_caloIndex_score.begin(); itr != map_caloIndex_score.end(); ++itr){
         for(auto itr2 = map_highest_score.begin(); itr2 != map_highest_score.end(); ++itr2){
            if(itr->second>itr2->second){
               map_highest_score.clear();
               map_highest_score.insert({itr->first, itr->second});
            }
         }
      } 

      int index_caloParticle;
      float minFraction = 0.05;
      for(auto itr2 = map_highest_score.begin(); itr2 != map_highest_score.end(); ++itr2){
         //cout << "The associated caloParticle is " << itr2->first << " with an associated score of " << itr2->second << endl;
         MatchingMap matchingMap_tmp;
         index_caloParticle = itr2->first;
         if(itr2->second>minFraction){
            matchingMap_tmp.CaloIndex = index_caloParticle;
            matchingMap_tmp.PFIndex = iPF;
            matchingMap_tmp.Score = itr2->second;
            matchingMap.push_back(matchingMap_tmp);
         }
      }
   }

   //for(unsigned int i(0); i<matchingMap.size(); ++i){
   //cout <<  " cluster " << matchingMap[i].PFIndex << " associated with calo " << matchingMap[i].CaloIndex << " with score " << matchingMap[i].Score << endl; 
   //}

   return matchingMap;
}

// at this point we have retrieved the caloParticle contributing the most to the energy of the cluster
// In other words, only one caloParticle is associated to a given cluster
// The opposite is not true: a caloParticle can have more than one cluster associated to it

vector<int> PFClusterAnalyzer::getMatchedIndices_score(const vector<MatchingMap>& matchingMap, unsigned int icP){

   vector<int> vector_matched_indices;

   vector<pair<int, float>> pair_clusterIndex_score; 

   for(unsigned int iMap(0); iMap<matchingMap.size(); ++iMap){
      pair<float, float> pair_clusterIndex_score_tmp;
      if(icP==matchingMap[iMap].CaloIndex){
         //cout << "calo " << matchingMap[iMap].CaloIndex << " associated with " << matchingMap[iMap].PFIndex << " with score " << matchingMap[iMap].Score << endl;
         pair_clusterIndex_score_tmp.first = matchingMap[iMap].PFIndex; 
         pair_clusterIndex_score_tmp.second = matchingMap[iMap].Score;
         pair_clusterIndex_score.push_back(pair_clusterIndex_score_tmp);
      }
   }

   //cout << endl << "pair formed: " << endl;
   //for(unsigned int i(0); i<pair_clusterIndex_score.size(); ++i){
   //cout << pair_clusterIndex_score[i].first << " " << pair_clusterIndex_score[i].second << endl;
   //}

   sort(pair_clusterIndex_score.begin(), pair_clusterIndex_score.end(), sortbysecdesc);

   //cout << endl << "sorted pair: " << endl;
   //for(unsigned int i(0); i<pair_clusterIndex_score.size(); ++i){
   //cout << pair_clusterIndex_score[i].first << " " << pair_clusterIndex_score[i].second << endl;
   //}


   // if(pair_clusterIndex_score.size()>1){
   //    cout << endl;
   //for(unsigned int i(0); i<pair_clusterIndex_score.size(); ++i){
   //   cout << pair_clusterIndex_score[i].first << " " << pair_clusterIndex_score[i].second << endl;
   // }
   // }

   if(flag_keepOnlyOnePFCluster){
      if(pair_clusterIndex_score.size()!=0){
         vector_matched_indices.push_back(pair_clusterIndex_score[0].first);
      }
   }
   else{         
      for(unsigned int i(0); i<pair_clusterIndex_score.size(); ++i){
         vector_matched_indices.push_back(pair_clusterIndex_score[i].first); 
      }
   }

   //if(vector_matched_indices.size()>1){cout << "size too big!" << endl;}
   //cout << endl << "matched indices: " << endl;
   //for(unsigned int i(0); i<vector_matched_indices.size(); ++i){
   //cout << vector_matched_indices[i] << endl;
   //}

   return vector_matched_indices;  
}


vector<int> PFClusterAnalyzer::getMatchedIndices_numberOfHits(const TTreeReaderArray<vector<map<int,float>>>& pfClusterHit_energy, unsigned int icP){

   vector<int> vector_matched_indices_all{-1};
   vector<int> vector_matched_indices{-1};


   // loop over pfClusterHits associated to calo particle
   for(unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){
      //for each pfClusterHit, we save a map that lists all the pfClusters to which the hit is associated and the energy deposited in the crystal
      //A given hit can belong to more than one pfCluster, in case of an overlap
      map<int, float>  map_pfClusters = pfClusterHit_energy[icP][ipfClH];
      float pfClH_filling_energy = 0;

      //if the hit is not matched to a PFCluster, the size of the map is 0
      if(map_pfClusters.size()!=0){

         //we get the pfCluster index out of the map and store the all the indices (with repetition)
         for (auto itr = map_pfClusters.begin(); itr != map_pfClusters.end(); ++itr) { 
            vector_matched_indices_all.push_back(itr->first);
            pfClH_filling_energy += itr->second;
         }

         //same as above, but this time the index is saved only once
         for (auto itr = map_pfClusters.begin(); itr != map_pfClusters.end(); ++itr) { 
            if(std::find(vector_matched_indices.begin(), vector_matched_indices.end(), itr->first)==vector_matched_indices.end()){
               vector_matched_indices.push_back(itr->first);
            }
         }
      }//end match pfClusterHit - pfCluster
   } // end loop pfClusterHits

   //in case we want to save only one PFCluster per PFClusterHit, we keep the index of the PFCluster having the more hits
   if(flag_keepOnlyOnePFCluster){
      int matched_index = vector_matched_indices[0];
      for(unsigned int i(1); i<=vector_matched_indices.size(); ++i){
         if(count(vector_matched_indices_all.begin(), vector_matched_indices_all.end(), vector_matched_indices[i])>=count(vector_matched_indices_all.begin(), vector_matched_indices_all.end(), vector_matched_indices[i-1])){
            matched_index = vector_matched_indices[i];
         }
      }
      vector_matched_indices.clear();
      vector_matched_indices.push_back(matched_index);
   }

   return vector_matched_indices;  
}

vector<int> PFClusterAnalyzer::getMatchedIndices_deltaR(const TTreeReaderArray<float>& pfCluster_energy, const TTreeReaderArray<float>& pfCluster_eta, const TTreeReaderArray<float>& pfCluster_phi, unsigned int icP, float deltaRThreshold){

   //the deltaRThreshold is the value below which we consider the distance between cluster and caloParticle to be reasonable

   vector<int> vector_matched_indices{-1};

   map<int, float> map_clusterIndex_deltaR;
   map_clusterIndex_deltaR.insert({-1,deltaRThreshold});
   //cout << endl << endl << "caloParticle n°" << icP << endl;
   float deltaR;
   for(unsigned int iCl = 0; iCl<pfCluster_energy.GetSize(); ++iCl){
      deltaR = TMath::Sqrt((pfCluster_eta[iCl]-caloParticle_eta[icP])*(pfCluster_eta[iCl]-caloParticle_eta[icP]) 
            + (pfCluster_phi[iCl]-caloParticle_phi[icP]) * (pfCluster_phi[iCl]-caloParticle_phi[icP]));
      for (auto itr = map_clusterIndex_deltaR.begin(); itr != map_clusterIndex_deltaR.end(); ++itr){
         if(deltaR<itr->second){
            map_clusterIndex_deltaR.clear();
            map_clusterIndex_deltaR.insert({iCl, deltaR});
         }

         //cout << "cluster " << iCl << " with delta R " << deltaR << endl;

      }
   }

   for (auto itr = map_clusterIndex_deltaR.begin(); itr != map_clusterIndex_deltaR.end(); ++itr){
   //   cout << "cluster " << itr->first << " selected with deltaR " << itr->second << endl;
      vector_matched_indices.push_back(itr->first);
   }

   //cout << "selected index: " << endl;
   //for(unsigned int i(0); i<vector_matched_indices.size(); ++i){
   //   cout << vector_matched_indices[i] << endl;
   //}

   return vector_matched_indices;  
}


