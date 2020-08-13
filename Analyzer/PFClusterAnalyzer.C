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
   if(fileName.find("_EB") != std::string::npos){
      flag_doEB = true;
      cout << "turning on EB flag" << endl;
   }
   else if(fileName.find("_EE") != std::string::npos){
      flag_doEE = true;
      cout << "turning on EE flag" << endl;
   }


   flag_doMatching_score        = false;
   flag_doMatching_deltaR       = false;
   flag_use_simfraction         = false;
   flag_use_simfraction_wHF     = false;

   if(fileName.find("simFraction_E") != std::string::npos){
      flag_doMatching_score = true;
      flag_use_simfraction = true;
   }
   else if(fileName.find("simFraction_wHF") != std::string::npos){
      flag_doMatching_score = true;
      flag_use_simfraction_wHF = true;
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
   fout->mkdir("perEvent");

   //Et_keys.push_back("0p1_1");
   Et_keys.push_back("1_5");
   Et_keys.push_back("5_10");
   Et_keys.push_back("10_15");
   Et_keys.push_back("15_20");

   //Et_keys.push_back("1_20");
   Et_keys.push_back("20_40");
   Et_keys.push_back("40_60");
   Et_keys.push_back("60_80");
   Et_keys.push_back("80_100");
   Et_keys.push_back("100_120");
   Et_keys.push_back("120_140");
   Et_keys.push_back("140_160");
   Et_keys.push_back("160_180");
   Et_keys.push_back("180_200");


   Et_edges["0p1_1"].first = 0.1;
   Et_edges["0p1_1"].second = 1.;
   Et_edges["1_5"].first = 1.;
   Et_edges["1_5"].second = 5.;
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
   Et_edges["100_120"].first = 100.;
   Et_edges["100_120"].second = 120.;
   Et_edges["120_140"].first = 120.;
   Et_edges["120_140"].second = 140.;
   Et_edges["140_160"].first = 140.;
   Et_edges["140_160"].second = 160.;
   Et_edges["160_180"].first = 160.;
   Et_edges["160_180"].second = 180.;
   Et_edges["180_200"].first = 180.;
   Et_edges["180_200"].second = 200.;


   if(flag_doEB){
      Eta_keys.push_back("0p00_0p50");
      Eta_keys.push_back("0p00_0p20");
      Eta_keys.push_back("0p00_0p40");
      Eta_keys.push_back("0p40_0p80");
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
      Eta_edges["0p00_0p40"].first = 0.;
      Eta_edges["0p00_0p40"].second = 0.4;
      Eta_edges["0p40_0p80"].first = 0.4;
      Eta_edges["0p40_0p80"].second = 0.8;
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
      //Eta_keys.push_back("1p48_1p57");
      Eta_keys.push_back("1p48_1p64");
      //Eta_keys.push_back("1p57_1p65");
      Eta_keys.push_back("1p64_1p85");
      //Eta_keys.push_back("1p65_1p85");
      Eta_keys.push_back("1p85_2p00");
      Eta_keys.push_back("2p00_2p20");
      Eta_keys.push_back("2p20_2p40");
      Eta_keys.push_back("2p40_2p60");
      Eta_keys.push_back("2p60_2p80");
      Eta_keys.push_back("2p80_3p00");
      //Eta_keys.push_back("1p48_2p00");
      //Eta_keys.push_back("2p00_2p50");
      //Eta_keys.push_back("2p50_3p00");

      Eta_edges["1p48_1p57"].first = 1.479;
      Eta_edges["1p48_1p57"].second = 1.566;
      Eta_edges["1p48_1p64"].first = 1.479;
      Eta_edges["1p48_1p64"].second = 1.64;
      Eta_edges["1p57_1p65"].first = 1.566;
      Eta_edges["1p57_1p65"].second = 1.653;
      Eta_edges["1p64_1p85"].first = 1.64;
      Eta_edges["1p64_1p85"].second = 1.85;
      Eta_edges["1p65_1p85"].first = 1.653;
      Eta_edges["1p65_1p85"].second = 1.85;
      Eta_edges["1p85_2p00"].first = 1.85;
      Eta_edges["1p85_2p00"].second = 2.0;
      Eta_edges["2p00_2p20"].first = 2.0;
      Eta_edges["2p00_2p20"].second = 2.20;
      Eta_edges["2p20_2p40"].first = 2.20;
      Eta_edges["2p20_2p40"].second = 2.40;
      Eta_edges["2p40_2p60"].first = 2.40;
      Eta_edges["2p40_2p60"].second = 2.60;
      Eta_edges["2p60_2p80"].first = 2.60;
      Eta_edges["2p60_2p80"].second = 2.80;
      Eta_edges["2p80_3p00"].first = 2.80;
      Eta_edges["2p80_3p00"].second = 3.0;
      Eta_edges["1p48_2p00"].first = 1.479;
      Eta_edges["1p48_2p00"].second = 2.0;

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
      h_PFClusters_score_simFraction_EE = new TH1F("h_PFClusters_score_simFraction_EE","h_PFClusters_score_simFraction_EE",150,0.,1.1);
      h_PFClusters_score_simFraction_withHF_EE = new TH1F("h_PFClusters_score_simFraction_withHF_EE","h_PFClusters_score_simFraction_withHF_EE",150,0.,1.1);
      h_PFClusters_score_ratio_EE = new TH1F("h_PFClusters_score_ratio_EE","h_PFClusters_score_ratio_EE",150,0.,1.1);

      h_PFClusters_caloMatched_size_EE    = new TH1F("h_PFClusters_caloMatched_size_EE","h_PFClusters_caloMatched_size_EE",50,0.,50.);
      h_PFClusters_caloMatched_nRecHit_EE = new TH1F("h_PFClusters_caloMatched_nRecHit_EE","h_PFClusters_caloMatched_nRecHit_EE",50,0.,50.);
      h_PFClusters_caloMatched_rawEnergy_EE  = new TH1F("h_PFClusters_caloMatched_rawEnergy_EE","h_PFClusters_caloMatched_rawEnergy_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_et_EE      = new TH1F("h_PFClusters_caloMatched_et_EE","h_PFClusters_caloMatched_et_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_eta_EE     = new TH1F("h_PFClusters_caloMatched_eta_EE","h_PFClusters_caloMatched_eta_EE",300,-3.,3.);
      h_PFClusters_caloMatched_phi_EE     = new TH1F("h_PFClusters_caloMatched_phi_EE","h_PFClusters_caloMatched_phi_EE",128,-3.2,3.2);
      h_PFClusters_caloMatched_fakeRate_EE          = new TH1F("h_PFClusters_caloMatched_fakeRate_EE","h_PFClusters_caloMatched_fakeRate_EE",100,0,1);
      h_PFClusters_caloMatched_eOverEtrue_EE        = new TH1F("h_PFClusters_caloMatched_eOverEtrue_EE","h_PFClusters_caloMatched_eOverEtrue_EE",100,0.,2.);
      h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE = new TH1F("h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE","h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE",100,0.,2.);
      h_PFClusters_caloMatched_nRecHit_vs_energy_EE = new TH2F("h_PFClusters_caloMatched_nRecHit_vs_energy_EE", "h_PFClusters_caloMatched_nRecHit_vs_energy_EE", 30, 0., 30., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_energy_EE = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_energy_EE", "h_PFClusters_caloMatched_nPFCLusters_vs_energy_EE", 8, 0., 8., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EE = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EE", "h_PFClusters_caloMatched_nPFCLusters_vs_caloEnergy_EE", 8, 0., 8., nBins_energy, rangeMin_energy, rangeMax_energy);
      h_PFClusters_caloMatched_nPFClusters_vs_eta_EE = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_eta_EE", "h_PFClusters_caloMatched_nPFCLusters_vs_eta_EE", 270, -3., 3., 8, 0., 8.);

      //EEM
      h_PFClusters_caloMatched_EEM_eta     = new TH1F("h_PFClusters_caloMatched_EEM_eta","h_PFClusters_caloMatched_EEM_eta",300,-3.,3.);
      h_PFClusters_caloMatched_EEM_size    = new TH1F("h_PFClusters_caloMatched_EEM_size","h_PFClusters_caloMatched_EEM_size",50,0.,50.);
      h_PFClusters_caloMatched_EEM_rawEnergy  = new TH1F("h_PFClusters_caloMatched_EEM_rawEnergy","h_PFClusters_caloMatched_EEM_rawEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EEM_et      = new TH1F("h_PFClusters_caloMatched_EEM_et","h_PFClusters_caloMatched_EEM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EEM_phi     = new TH1F("h_PFClusters_caloMatched_EEM_phi","h_PFClusters_caloMatched_EEM_phi",128,-3.2,3.2);
      h_PFClusters_caloMatched_EEM_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EEM_eOverEtrue","h_PFClusters_caloMatched_EEM_eOverEtrue",100,0.,2.);

      //EEP
      h_PFClusters_caloMatched_EEP_eta     = new TH1F("h_PFClusters_caloMatched_EEP_eta","h_PFClusters_caloMatched_EEP_eta",300,-3.,3.);
      h_PFClusters_caloMatched_EEP_size    = new TH1F("h_PFClusters_caloMatched_EEP_size","h_PFClusters_caloMatched_EEP_size",50,0.,50.);
      h_PFClusters_caloMatched_EEP_rawEnergy  = new TH1F("h_PFClusters_caloMatched_EEP_rawEnergy","h_PFClusters_caloMatched_EEP_rawEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EEP_et      = new TH1F("h_PFClusters_caloMatched_EEP_et","h_PFClusters_caloMatched_EEP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EEP_phi     = new TH1F("h_PFClusters_caloMatched_EEP_phi","h_PFClusters_caloMatched_EEP_phi",128,-3.2,3.2);
      h_PFClusters_caloMatched_EEP_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EEP_eOverEtrue","h_PFClusters_caloMatched_EEP_eOverEtrue",100,0.,2.);
   }

   if(flag_doEB){
      h_PFClusters_score_simFraction_EB = new TH1F("h_PFClusters_score_simFraction_EB","h_PFClusters_score_simFraction_EB",150,0.,1.2);
      h_PFClusters_score_simFraction_withHF_EB = new TH1F("h_PFClusters_score_simFraction_withHF_EB","h_PFClusters_score_simFraction_withHF_EB",150,0.,1.2);
      h_PFClusters_score_ratio_EB = new TH1F("h_PFClusters_score_ratio_EB","h_PFClusters_score_ratio_EB",150,0.,1.1);
      
      h_PFClusters_caloMatched_size_EB    = new TH1F("h_PFClusters_caloMatched_size_EB","h_PFClusters_caloMatched_size_EB",50,0.,50.);
      h_PFClusters_caloMatched_nRecHit_EB = new TH1F("h_PFClusters_caloMatched_nRecHit_EB","h_PFClusters_caloMatched_nRecHit_EB",50,0.,50.);
      h_PFClusters_caloMatched_rawEnergy_EB  = new TH1F("h_PFClusters_caloMatched_rawEnergy_EB","h_PFClusters_caloMatched_rawEnergy_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_et_EB      = new TH1F("h_PFClusters_caloMatched_et_EB","h_PFClusters_caloMatched_et_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_eta_EB     = new TH1F("h_PFClusters_caloMatched_eta_EB","h_PFClusters_caloMatched_eta_EB",300,-3.,3.);
      h_PFClusters_caloMatched_phi_EB     = new TH1F("h_PFClusters_caloMatched_phi_EB","h_PFClusters_caloMatched_phi_EB",128,-3.2,3.2);
      h_PFClusters_caloMatched_fakeRate_EB     = new TH1F("h_PFClusters_caloMatched_fakeRate_EB","h_PFClusters_caloMatched_fakeRate_EB",100,0,1);
      h_PFClusters_caloMatched_eOverEtrue_EB = new TH1F("h_PFClusters_caloMatched_eOverEtrue_EB","h_PFClusters_caloMatched_eOverEtrue_EB",100,0.,2.);
      h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB = new TH1F("h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB","h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB",100,0.,2.);
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
      h_PFClusters_caloMatched_EBM_rawEnergy  = new TH1F("h_PFClusters_caloMatched_EBM_rawEnergy","h_PFClusters_caloMatched_EBM_rawEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EBM_et      = new TH1F("h_PFClusters_caloMatched_EBM_et","h_PFClusters_caloMatched_EBM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EBM_phi     = new TH1F("h_PFClusters_caloMatched_EBM_phi","h_PFClusters_caloMatched_EBM_phi",128,-3.2,3.2);
      h_PFClusters_caloMatched_EBM_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EBM_eOverEtrue","h_PFClusters_caloMatched_EBM_eOverEtrue",100,0.,2.);

      //EBP
      h_PFClusters_caloMatched_EBP_eta     = new TH1F("h_PFClusters_caloMatched_EBP_eta","h_PFClusters_caloMatched_EBP_eta",300,-3.,3.);
      h_PFClusters_caloMatched_EBP_size    = new TH1F("h_PFClusters_caloMatched_EBP_size","h_PFClusters_caloMatched_EBP_size",50,0.,50.);
      h_PFClusters_caloMatched_EBP_rawEnergy  = new TH1F("h_PFClusters_caloMatched_EBP_rawEnergy","h_PFClusters_caloMatched_EBP_rawEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EBP_et      = new TH1F("h_PFClusters_caloMatched_EBP_et","h_PFClusters_caloMatched_EBP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_PFClusters_caloMatched_EBP_phi     = new TH1F("h_PFClusters_caloMatched_EBP_phi","h_PFClusters_caloMatched_EBP_phi",128,-3.2,3.2);
      h_PFClusters_caloMatched_EBP_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EBP_eOverEtrue","h_PFClusters_caloMatched_EBP_eOverEtrue",100,0.,2.);
   }


   fout->cd("caloParticle");
   if(flag_doEE){
      h_caloParticle_size_EE       = new TH1F("h_caloParticle_size_EE","h_caloParticle_size_EE",50,0.,50.);
      h_caloParticle_genEnergy_EE     = new TH1F("h_caloParticle_genEnergy_EE","h_caloParticle_genEnergy_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_simEnergy_EE  = new TH1F("h_caloParticle_simEnergy_EE","h_caloParticle_simEnergy_EE", nBins_energy, rangeMin_energy, rangeMax_energy);
      h_caloParticle_simEt_EE      = new TH1F("h_caloParticle_simEt_EE","h_caloParticle_simEt_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_genEta_EE     = new TH1F("h_caloParticle_genEta_EE","h_caloParticle_genEta_EE",300,-3.,3.);
      h_caloParticle_simEta_EE     = new TH1F("h_caloParticle_simEta_EE","h_caloParticle_simEta_EE",300,-3.,3.);
      h_caloParticle_genPhi_EE     = new TH1F("h_caloParticle_genPhi_EE","h_caloParticle_genPhi_EE",128,-3.2,3.2);
      h_caloParticle_simPhi_EE     = new TH1F("h_caloParticle_simPhi_EE","h_caloParticle_simPhi_EE",128,-3.2,3.2);


      h_caloParticle_EEM_size       = new TH1F("h_caloParticle_EEM_size","h_caloParticle_EEM_size",50,0.,50.);
      h_caloParticle_EEM_energy     = new TH1F("h_caloParticle_EEM_energy","h_caloParticle_EEM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEM_simEnergy  = new TH1F("h_caloParticle_EEM_simEnergy","h_caloParticle_EEM_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEM_simEt      = new TH1F("h_caloParticle_EEM_simEt","h_caloParticle_EEM_simEt",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEM_eta     = new TH1F("h_caloParticle_EEM_eta","h_caloParticle_EEM_eta",300,-3.,3.);
      h_caloParticle_EEM_phi     = new TH1F("h_caloParticle_EEM_phi","h_caloParticle_EEM_phi",128,-3.2,3.2);

      h_caloParticle_EEP_size       = new TH1F("h_caloParticle_EEP_size","h_caloParticle_EEP_size",50,0.,50.);
      h_caloParticle_EEP_energy     = new TH1F("h_caloParticle_EEP_energy","h_caloParticle_EEP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEP_simEnergy  = new TH1F("h_caloParticle_EEP_simEnergy","h_caloParticle_EEP_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEP_simEt      = new TH1F("h_caloParticle_EEP_simEt","h_caloParticle_EEP_simEt",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EEP_eta     = new TH1F("h_caloParticle_EEP_eta","h_caloParticle_EEP_eta",300,-3.,3.);
      h_caloParticle_EEP_phi     = new TH1F("h_caloParticle_EEP_phi","h_caloParticle_EEP_phi",128,-3.2,3.2);
   }

   if(flag_doEB){
      h_caloParticle_size_EB       = new TH1F("h_caloParticle_size_EB","h_caloParticle_size_EB",50,0.,50.);
      h_caloParticle_genEnergy_EB     = new TH1F("h_caloParticle_genEnergy_EB","h_caloParticle_genEnergy_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_simEnergy_EB  = new TH1F("h_caloParticle_simEnergy_EB","h_caloParticle_simEnergy_EB", nBins_energy, rangeMin_energy, rangeMax_energy);
      h_caloParticle_simEt_EB      = new TH1F("h_caloParticle_simEt_EB","h_caloParticle_simEt_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_genEta_EB     = new TH1F("h_caloParticle_genEta_EB","h_caloParticle_genEta_EB",300,-3.,3.);
      h_caloParticle_simEta_EB     = new TH1F("h_caloParticle_simEta_EB","h_caloParticle_simEta_EB",300,-3.,3.);
      h_caloParticle_genPhi_EB     = new TH1F("h_caloParticle_genPhi_EB","h_caloParticle_genPhi_EB",128,-3.2,3.2);
      h_caloParticle_simPhi_EB     = new TH1F("h_caloParticle_simPhi_EB","h_caloParticle_simPhi_EB",128,-3.2,3.2);

      h_caloParticle_simPhi_vs_eta_ifNoPFCluster_EB = new TH2F("h_caloParticle_simPhi_vs_eta_ifNoPFCluster_EB", "h_caloParticle_simPhi_vs_eta_ifNoPFCluster_EB", 50, -3.14, 3.14, 50., -1.479, 1.479);

      h_caloParticle_EBM_size       = new TH1F("h_caloParticle_EBM_size","h_caloParticle_EBM_size",50,0.,50.);
      h_caloParticle_EBM_energy     = new TH1F("h_caloParticle_EBM_energy","h_caloParticle_EBM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBM_simEnergy  = new TH1F("h_caloParticle_EBM_simEnergy","h_caloParticle_EBM_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBM_simEt      = new TH1F("h_caloParticle_EBM_simEt","h_caloParticle_EBM_simEt",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBM_eta     = new TH1F("h_caloParticle_EBM_eta","h_caloParticle_EBM_eta",300,-3.,3.);
      h_caloParticle_EBM_phi     = new TH1F("h_caloParticle_EBM_phi","h_caloParticle_EBM_phi",128,-3.2,3.2);

      h_caloParticle_EBP_size       = new TH1F("h_caloParticle_EBP_size","h_caloParticle_EBP_size",50,0.,50.);
      h_caloParticle_EBP_energy     = new TH1F("h_caloParticle_EBP_energy","h_caloParticle_EBP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBP_simEnergy  = new TH1F("h_caloParticle_EBP_simEnergy","h_caloParticle_EBP_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBP_simEt      = new TH1F("h_caloParticle_EBP_simEt","h_caloParticle_EBP_simEt",nBins_energy,rangeMin_energy,rangeMax_energy);
      h_caloParticle_EBP_eta     = new TH1F("h_caloParticle_EBP_eta","h_caloParticle_EBP_eta",300,-3.,3.);
      h_caloParticle_EBP_phi     = new TH1F("h_caloParticle_EBP_phi","h_caloParticle_EBP_phi",128,-3.2,3.2);
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

         TString histo_name_fakeRate = "h_PFclusters_caloMatched_fakeRate_Eta" + Eta_key + "_En" + Et_key;
         h_PFclusters_caloMatched_fakeRate_EtaEnBinned[Eta_key][Et_key] = new TH1F(histo_name_fakeRate,histo_name_fakeRate,110,0,1.1);
         TString histo_name_noiseOccupancy = "h_PFclusters_nonCaloMatched_noiseOccupancy_Eta" + Eta_key + "_En" + Et_key;
         h_PFclusters_nonCaloMatched_noiseOccupancy_EtaEnBinned[Eta_key][Et_key] = new TH1F(histo_name_noiseOccupancy,histo_name_noiseOccupancy,100,0,100);
         
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

   fReader.SetLocalEntry(entry);
   if (entry % 1000 == 0) Info("Process", "processing event %d", (Int_t)entry);

   //if(entry>5){ throw std::invalid_argument("aborting");}

   //cout << endl << endl << "EVENT " << entry << endl;

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

   //needed the fake rate computation
   map<TString, map<TString, float>> N_PFtot_binned;
   map<TString, map<TString, float>> N_PFmatched_binned;

   for (unsigned int icP=0; icP<caloParticle_genEnergy.GetSize(); icP++){

      N_Cl++;

      //---caloParticle---

      //cout << endl << "caloParticle " << icP << " with simEnergy " << caloParticle_simEnergy[icP] << ", simEta " << caloParticle_simEta[icP] << " and simPhi " << caloParticle_simPhi[icP] << endl;
      
      if(flag_doEB){
         if(TMath::Abs(caloParticle_simEta[icP])<1.479){
            h_caloParticle_genEnergy_EB->Fill(caloParticle_genEnergy[icP]);
            h_caloParticle_simEnergy_EB->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_simEt_EB->Fill(caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
            h_caloParticle_simPhi_EB->Fill(caloParticle_simPhi[icP]);
            h_caloParticle_genPhi_EB->Fill(caloParticle_genPhi[icP]);
            h_caloParticle_simEta_EB->Fill(caloParticle_simEta[icP]);
            h_caloParticle_genEta_EB->Fill(caloParticle_genEta[icP]);
         }

         if(caloParticle_simEta[icP]>-1.479 && caloParticle_simEta[icP]<0){
            N_Cl_EBM++;
            h_caloParticle_EBM_energy->Fill(caloParticle_genEnergy[icP]);
            h_caloParticle_EBM_simEnergy->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_EBM_simEt->Fill(caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
            h_caloParticle_EBM_phi->Fill(caloParticle_simPhi[icP]);
            h_caloParticle_EBM_eta->Fill(caloParticle_simEta[icP]);
         }

         if(caloParticle_simEta[icP]>=0 && caloParticle_simEta[icP]<1.479){
            N_Cl_EBP++;
            h_caloParticle_EBP_energy->Fill(caloParticle_genEnergy[icP]);
            h_caloParticle_EBP_simEnergy->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_EBP_simEt->Fill(caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
            h_caloParticle_EBP_phi->Fill(caloParticle_simPhi[icP]);
            h_caloParticle_EBP_eta->Fill(caloParticle_simEta[icP]);
         }
      }

      if(flag_doEE){
         if(TMath::Abs(caloParticle_simEta[icP])>1.479){
            h_caloParticle_genEnergy_EE->Fill(caloParticle_genEnergy[icP]);
            h_caloParticle_simEnergy_EE->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_simEt_EE->Fill(caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
            h_caloParticle_simPhi_EE->Fill(caloParticle_simPhi[icP]);
            h_caloParticle_genPhi_EE->Fill(caloParticle_genPhi[icP]);
            h_caloParticle_simEta_EE->Fill(caloParticle_simEta[icP]);
            h_caloParticle_genEta_EE->Fill(caloParticle_genEta[icP]);
         }

         if(caloParticle_simEta[icP]<-1.479){
            N_Cl_EEM++;
            h_caloParticle_EEM_energy->Fill(caloParticle_genEnergy[icP]);
            h_caloParticle_EEM_simEnergy->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_EEM_simEt->Fill(caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
            h_caloParticle_EEM_phi->Fill(caloParticle_simPhi[icP]);
            h_caloParticle_EEM_eta->Fill(caloParticle_simEta[icP]);
         }

         if(caloParticle_simEta[icP]>1.479){
            N_Cl_EEP++;
            h_caloParticle_EEP_energy->Fill(caloParticle_genEnergy[icP]);
            h_caloParticle_EEP_simEnergy->Fill(caloParticle_simEnergy[icP]);
            h_caloParticle_EEP_simEt->Fill(caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
            h_caloParticle_EEP_phi->Fill(caloParticle_simPhi[icP]);
            h_caloParticle_EEP_eta->Fill(caloParticle_simEta[icP]);
         }
      }

      //h_caloParticle_EEM_size->Fill(N_Cl_EEM); 
      //h_caloParticle_EBM_size->Fill(N_Cl_EBM);
      //h_caloParticle_EBP_size->Fill(N_Cl_EBP);
      //h_caloParticle_EEP_size->Fill(N_Cl_EEP);

      //eta ET binned quantity that stores the number of events (equal to the number of entries of the histogram)
      for(TString Eta_key: Eta_keys){
         for(TString Et_key: Et_keys){
            double caloParticle_et = caloParticle_genEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP])));
            if(caloParticle_et>=Et_edges[Et_key].first && caloParticle_et<Et_edges[Et_key].second 
                  && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEtBinned[Eta_key][Et_key]->Fill(1.);
            }
            double caloParticle_et_sim = caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP])));
            if(caloParticle_et_sim>=Et_edges[Et_key].first && caloParticle_et_sim<Et_edges[Et_key].second 
                  && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEtBinned_simEnergy[Eta_key][Et_key]->Fill(1.);
            }
         }
      }

      //eta energy binned quantity that stores the number of events (equal to the number of entries of the histogram)
      for(TString Eta_key: Eta_keys){
         for(TString Et_key: Et_keys){
            if(caloParticle_genEnergy[icP]>=Et_edges[Et_key].first && caloParticle_genEnergy[icP]<Et_edges[Et_key].second 
                  && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEnBinned[Eta_key][Et_key]->Fill(1.);
            }
            if(caloParticle_simEnergy[icP]>=Et_edges[Et_key].first && caloParticle_simEnergy[icP]<Et_edges[Et_key].second 
                  && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEnBinned_simEnergy[Eta_key][Et_key]->Fill(1.);
            }
         }
      }

      // per-event 2D maps
      if(entry<N_perEvent_plots){
         if(fabs(caloParticle_simEta[icP])<1.479){
            h_caloParticle_EB_ietaiphi.at((Int_t)entry)->Fill(caloParticle_simIeta[icP],caloParticle_simIphi[icP],caloParticle_genEnergy[icP]);
         } else { // end-cap
            if(caloParticle_simEta[icP]>0){ 
               h_caloParticle_EEP_ixiy.at((Int_t)entry)->Fill(caloParticle_simIeta[icP],caloParticle_simIphi[icP],caloParticle_genEnergy[icP]);
            } else {
               h_caloParticle_EEM_ixiy.at((Int_t)entry)->Fill(caloParticle_simIeta[icP],caloParticle_simIphi[icP],caloParticle_genEnergy[icP]);
            }
         }
      }


      map<TString, map<TString, float>> N_PFtot_binned_tmp;

      for(TString Eta_key: Eta_keys){
         for(TString Et_key: Et_keys){
            for(unsigned int iPFCl(0); iPFCl<pfCluster_rawEnergy.GetSize(); iPFCl++){
               if(pfCluster_rawEnergy[iPFCl]>=Et_edges[Et_key].first && pfCluster_rawEnergy[iPFCl]<Et_edges[Et_key].second 
                     && std::abs(pfCluster_eta[iPFCl])>=Eta_edges[Eta_key].first && std::abs(pfCluster_eta[iPFCl])<Eta_edges[Eta_key].second){
                  ++N_PFtot_binned_tmp[Eta_key][Et_key];
               }
            }
            //if(N_PFtot_binned_tmp[Eta_key][Et_key]!=0) cout << Eta_key << " " << Et_key << " tot: " << N_PFtot_binned_tmp[Eta_key][Et_key] << endl;
            N_PFtot_binned[Eta_key][Et_key] = N_PFtot_binned_tmp[Eta_key][Et_key];
         }
      }

      //we loop on all the PFClusters associated to the same PFClusterHit and sum the energy, eta, phi
      double filling_energy=0;
      double filling_phi=0;
      double filling_eta=0;

      vector<int> vector_matched_indices;

      if(flag_doMatching_score){

         vector<int> input_vector;
         float score_thrs;
         if(flag_use_simfraction){ 
            input_vector = caloParticle_pfCluster_sim_fraction_MatchedIndex[icP];
            score_thrs = 0.01;
         }
         else if(flag_use_simfraction_wHF){
            input_vector = caloParticle_pfCluster_sim_fraction_noHitsFraction_MatchedIndex[icP];
            score_thrs = 0.4;
         }

         vector<pair<int,float>> pair_clusterIndex_score;

         for(unsigned int iPF(0); iPF<input_vector.size(); ++iPF){
            if(input_vector[iPF]!=-1){
               pair<int,float> pair_clusterIndex_score_tmp;
               pair_clusterIndex_score_tmp.first = input_vector[iPF];
               if(flag_use_simfraction){
                  pair_clusterIndex_score_tmp.second = pfCluster_sim_fraction[input_vector[iPF]][icP];
               }
               else if(flag_use_simfraction_wHF){
                  pair_clusterIndex_score_tmp.second = pfCluster_sim_fraction_noHitsFraction[input_vector[iPF]][icP];
               }
               if(flag_doEB){
                  h_PFClusters_score_simFraction_EB->Fill(pfCluster_sim_fraction_noHitsFraction[input_vector[iPF]][icP]);
                  h_PFClusters_score_simFraction_withHF_EB->Fill(pfCluster_sim_fraction[input_vector[iPF]][icP]);
                  h_PFClusters_score_ratio_EB->Fill(pfCluster_sim_fraction[input_vector[iPF]][icP]/pfCluster_sim_fraction_noHitsFraction[input_vector[iPF]][icP]);
               }
               else if(flag_doEE){
                  //h_PFClusters_score_simFraction_EE->Fill(pfCluster_sim_fraction_noHitsFraction[input_vector[iPF]][icP]);
                  //h_PFClusters_score_simFraction_withHF_EE->Fill(pfCluster_sim_fraction[input_vector[iPF]][icP]);
                  //h_PFClusters_score_ratio_EE->Fill(pfCluster_sim_fraction[input_vector[iPF]][icP]/pfCluster_sim_fraction_noHitsFraction[input_vector[iPF]][icP]);
               }
               // we apply cut on score > score_thrs
               if(pair_clusterIndex_score_tmp.second > score_thrs){
                  pair_clusterIndex_score.push_back(pair_clusterIndex_score_tmp);
               }
            }
         }

         sort(pair_clusterIndex_score.begin(), pair_clusterIndex_score.end(), sortbysecdesc);
         h_PFClusters_score_simFraction_EE->Fill(pair_clusterIndex_score[0].second);
         //h_PFClusters_score_simFraction_withHF_EE->Fill();

         //if(input_vector.size()>2){ 
         // cout << "sorted: " << endl;
         // for(unsigned int iSort(0); iSort<pair_clusterIndex_score.size(); ++iSort){
         //    cout << "pfCluster " <<  pair_clusterIndex_score[iSort].first << " with score " << pair_clusterIndex_score[iSort].second << endl;
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
      }

      //cout << "vector matched indices: " << endl;
      //for(unsigned int i(0); i<vector_matched_indices.size(); ++i){
      //   cout << vector_matched_indices[i] << endl;
      //}

      if(flag_doMatching_deltaR){
         vector<int> input_vector = caloParticle_pfCluster_dR_simScore_MatchedIndex[icP];

         vector<pair<int,float>> pair_clusterIndex_score;

         for(unsigned int iPF(0); iPF<input_vector.size(); ++iPF){
            if(input_vector[iPF]!=-1){
               pair<int,float> pair_clusterIndex_score_tmp;
               pair_clusterIndex_score_tmp.first = input_vector[iPF];
               pair_clusterIndex_score_tmp.second = pfCluster_dR_simScore[input_vector[iPF]][icP];
               pair_clusterIndex_score.push_back(pair_clusterIndex_score_tmp);
            }
         }

         sort(pair_clusterIndex_score.begin(), pair_clusterIndex_score.end(), sortbysecdesc);
         //if(input_vector.size()>2){ 
         // cout << "sorted: " << endl;
         // for(unsigned int iSort(0); iSort<pair_clusterIndex_score.size(); ++iSort){
         //    cout << pair_clusterIndex_score[iSort].first << " " << pair_clusterIndex_score[iSort].second << endl;
         // }}

         if(flag_keepOnlyOnePFCluster){
            if(pair_clusterIndex_score.size()!=0){
               vector_matched_indices.push_back(pair_clusterIndex_score[pair_clusterIndex_score.size()-1].first);
            }
         }
         else{
            for(unsigned int i(0); i<pair_clusterIndex_score.size(); ++i){
               vector_matched_indices.push_back(pair_clusterIndex_score[i].first);
            }
         }
      }
      
      for(unsigned int iMatched(0); iMatched<vector_matched_indices.size(); ++iMatched){
         int matched_index = vector_matched_indices[iMatched];
         int nOccurrences;  
         if(matched_index!=-1){
            filling_energy = pfCluster_rawEnergy[matched_index];
            filling_phi    = pfCluster_phi[matched_index];
            filling_eta    = pfCluster_eta[matched_index];

            //cout << "matched PFCluster " << matched_index << " with energy " << filling_energy << ", eta " << filling_eta << " and phi " << filling_phi << endl;
            
            if(flag_doEB){
               if(TMath::Abs(caloParticle_simEta[icP])<1.479){
                  nOccurrences = pfClusterHit_rechitEnergy[matched_index].size();

                  //number of recHit per PFCluster
                  h_PFClusters_caloMatched_nRecHit_EB->Fill(nOccurrences);

                  //number of recHit and energy
                  h_PFClusters_caloMatched_nRecHit_vs_energy_EB->Fill(nOccurrences, filling_energy);

                  N_pfCl++;
                  h_PFClusters_caloMatched_rawEnergy_EB->Fill(filling_energy);
                  h_PFClusters_caloMatched_et_EB->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_phi_EB->Fill(filling_phi);
                  h_PFClusters_caloMatched_eta_EB->Fill(filling_eta);
                  h_PFClusters_caloMatched_eOverEtrue_EB->Fill(filling_energy / caloParticle_genEnergy[icP]);         
                  if(caloParticle_simEnergy[icP]!=-1){
                     h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB->Fill(filling_energy / caloParticle_simEnergy[icP]);         
                  }
                  //plot number of recHit related to energy and eta
                  h_PFClusters_caloMatched_nPFClusters_vs_energy_EB->Fill(N_pfCl, filling_energy);
                  h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EB->Fill(N_pfCl, caloParticle_genEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
                  h_PFClusters_caloMatched_nPFClusters_vs_eta_EB->Fill(filling_eta, N_pfCl);
                  h_PFClusters_caloMatched_deltaR_EB->Fill(TMath::Sqrt((filling_eta-caloParticle_simEta[icP])*(filling_eta-caloParticle_simEta[icP]) + (filling_phi-caloParticle_simPhi[icP]) * (filling_phi-caloParticle_simPhi[icP])));

                  //we fill the histogram for E/Etrue < 0.5 and ET>50
                  if(filling_energy/caloParticle_simEnergy[icP] < 0.5 && caloParticle_genEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))) > 50){
                     h_PFClusters_caloMatched_EoverEtrue_iEta_iPhi_EB->Fill(caloParticle_simIeta[icP], caloParticle_simIphi[icP], caloParticle_genEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
                  }
                  if(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))) < 20){
                     h_PFClusters_caloMatched_et_iEta_iPhi_EB->Fill(caloParticle_simIeta[icP], caloParticle_simIphi[icP]);
                  }
               }

               if(caloParticle_simEta[icP]>=-1.479 && caloParticle_simEta[icP]<0){
                  N_pfCl_EBM++;

                  h_PFClusters_caloMatched_EBM_eta->Fill(filling_eta);
                  h_PFClusters_caloMatched_EBM_rawEnergy->Fill(filling_energy);
                  h_PFClusters_caloMatched_EBM_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_EBM_phi->Fill(filling_phi);
                  h_PFClusters_caloMatched_EBM_eOverEtrue->Fill(filling_energy / caloParticle_genEnergy[icP]);         
               }

               if(caloParticle_simEta[icP]>=0 && caloParticle_simEta[icP]<1.479){
                  N_pfCl_EBP++;

                  h_PFClusters_caloMatched_EBP_eta->Fill(filling_eta);
                  h_PFClusters_caloMatched_EBP_rawEnergy->Fill(filling_energy);
                  h_PFClusters_caloMatched_EBP_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_EBP_phi->Fill(filling_phi);
                  h_PFClusters_caloMatched_EBP_eOverEtrue->Fill(filling_energy / caloParticle_genEnergy[icP]);         
               }
            }

            if(flag_doEE){
               if(TMath::Abs(caloParticle_simEta[icP])>1.479){
                  nOccurrences = pfClusterHit_rechitEnergy[matched_index].size();

                  //number of recHit per PFCluster
                  h_PFClusters_caloMatched_nRecHit_EE->Fill(nOccurrences);

                  //number of recHit and energy
                  h_PFClusters_caloMatched_nRecHit_vs_energy_EE->Fill(nOccurrences, filling_energy);

                  N_pfCl++;
                  h_PFClusters_caloMatched_rawEnergy_EE->Fill(filling_energy);
                  h_PFClusters_caloMatched_et_EE->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_phi_EE->Fill(filling_phi);
                  h_PFClusters_caloMatched_eta_EE->Fill(filling_eta);
                  h_PFClusters_caloMatched_eOverEtrue_EE->Fill(filling_energy / caloParticle_genEnergy[icP]);         
                  if(caloParticle_simEnergy[icP]!=-1){
                     h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE->Fill(filling_energy / caloParticle_simEnergy[icP]);         
                  }

               }
               //plot number of recHit related to energy and eta
               h_PFClusters_caloMatched_nPFClusters_vs_energy_EE->Fill(N_pfCl, filling_energy);
               h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EE->Fill(N_pfCl, caloParticle_genEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
               h_PFClusters_caloMatched_nPFClusters_vs_eta_EE->Fill(filling_eta, N_pfCl);


               if(caloParticle_simEta[icP]<-1.479){
                  N_pfCl_EEM++;

                  h_PFClusters_caloMatched_EEM_eta->Fill(filling_eta);
                  h_PFClusters_caloMatched_EEM_rawEnergy->Fill(filling_energy);
                  h_PFClusters_caloMatched_EEM_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_EEM_phi->Fill(filling_phi);
                  h_PFClusters_caloMatched_EEM_eOverEtrue->Fill(filling_energy / caloParticle_genEnergy[icP]);         

               }

               if(caloParticle_simEta[icP]>=1.479){
                  N_plCl_EEP++;

                  h_PFClusters_caloMatched_EEP_eta->Fill(filling_eta);
                  h_PFClusters_caloMatched_EEP_rawEnergy->Fill(filling_energy);
                  h_PFClusters_caloMatched_EEP_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
                  h_PFClusters_caloMatched_EEP_phi->Fill(filling_phi);
                  h_PFClusters_caloMatched_EEP_eOverEtrue->Fill(filling_energy / caloParticle_genEnergy[icP]);         


               }
            }

            //we fill the caloMatched histograms binned in eta and ET
            for(TString Eta_key: Eta_keys){
               for(TString Et_key: Et_keys){
                  //case 1: binning in eta and ET and using caloParticle_genEnergy
                  double caloParticle_et = caloParticle_genEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP])));
                  if(caloParticle_et>=Et_edges[Et_key].first && caloParticle_et<Et_edges[Et_key].second 
                        && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){

                     h_PFclusters_caloMatched_eOverEtrue_EtaEtBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_genEnergy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     //if(filling_energy / caloParticle_genEnergy[icP] > 0.4 && filling_energy / caloParticle_genEnergy[icP] < 1.4){
                     //  h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);
                     //}
                  }
                  //case 2: binning in eta and ET and using caloParticle_simEnergy
                  double caloParticle_et_sim = caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP])));
                  if(caloParticle_et_sim>=Et_edges[Et_key].first && caloParticle_et_sim<Et_edges[Et_key].second 
                        && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){

                     h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEtBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_simEnergy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     //if(filling_energy / caloParticle_simEnergy[icP] > 0.4 && filling_energy / caloParticle_simEnergy[icP] < 1.4){
                     //   h_PFclusters_caloMatched_size_EtaEtBinned_simEnergy_forEfficiency[Eta_key][Et_key]->Fill(1.);
                     //}
                  }
                  //case 3: binning in eta and energy
                  if(caloParticle_genEnergy[icP]>=Et_edges[Et_key].first && caloParticle_genEnergy[icP]<Et_edges[Et_key].second 
                        && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){

                     h_PFclusters_caloMatched_eOverEtrue_EtaEnBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_genEnergy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     //if(filling_energy / caloParticle_genEnergy[icP] > 0.4 && filling_energy / caloParticle_genEnergy[icP] < 1.4){
                     //   h_PFclusters_caloMatched_size_EtaEnBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);
                     //}
                  }
                  //case 4: binning in eta and simEnergy
                  if(caloParticle_simEnergy[icP]>=Et_edges[Et_key].first && caloParticle_simEnergy[icP]<Et_edges[Et_key].second 
                        && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){
                     h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEnBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_simEnergy[icP]);
                  }

                  if(pfCluster_rawEnergy[matched_index]>=Et_edges[Et_key].first && pfCluster_rawEnergy[matched_index]<Et_edges[Et_key].second 
                        && std::abs(pfCluster_eta[matched_index])>=Eta_edges[Eta_key].first && std::abs(pfCluster_eta[matched_index])<Eta_edges[Eta_key].second){
                     ++N_PFmatched_binned[Eta_key][Et_key];
                  }
                  //if(N_PFmatched_binned[Eta_key][Et_key]!=0) cout << Eta_key << " " << Et_key << " matched:" << N_PFmatched_binned[Eta_key][Et_key] << endl;
               }
            }

            //plot number of recHit related to energy and eta
            //h_PFClusters_caloMatched_nPFClusters_vs_energy->Fill(N_pfCl, filling_energy);
            //h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy->Fill(N_pfCl, caloParticle_genEnergy[icP]);
            //h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy->Fill(N_pfCl, caloParticle_genEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP]))));
            //h_PFClusters_caloMatched_nPFClusters_vs_eta->Fill(filling_eta, N_pfCl);

         } //end of matched index
      }// end of loop on matched indices


      // we get the number of caloParticle having at least one matched pfcluster for efficiency computation
      if(vector_matched_indices.size()>0){
         if(vector_matched_indices[0]!=-1){
            for(TString Eta_key: Eta_keys){
               for(TString Et_key: Et_keys){
                  //case 1: binning in eta and ET and using caloParticle_genEnergy
                  double caloParticle_et = caloParticle_genEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP])));
                  if(caloParticle_et>=Et_edges[Et_key].first && caloParticle_et<Et_edges[Et_key].second 
                        && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);
                  }
                  //case 2: binning in eta and ET and using caloParticle_simEnergy
                  double caloParticle_et_sim = caloParticle_simEnergy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_simEta[icP])));
                  if(caloParticle_et_sim>=Et_edges[Et_key].first && caloParticle_et_sim<Et_edges[Et_key].second 
                        && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     h_PFclusters_caloMatched_size_EtaEtBinned_simEnergy_forEfficiency[Eta_key][Et_key]->Fill(1.);
                  }
                  //case 3: binning in eta and energy
                  if(caloParticle_genEnergy[icP]>=Et_edges[Et_key].first && caloParticle_genEnergy[icP]<Et_edges[Et_key].second 
                        && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     h_PFclusters_caloMatched_size_EtaEnBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);
                  }
                  //case 4: binning in eta and simEnergy
                  if(caloParticle_simEnergy[icP]>=Et_edges[Et_key].first && caloParticle_simEnergy[icP]<Et_edges[Et_key].second 
                        && std::abs(caloParticle_simEta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_simEta[icP])<Eta_edges[Eta_key].second){

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     h_PFclusters_caloMatched_size_EtaEnBinned_simEnergy_forEfficiency[Eta_key][Et_key]->Fill(1.);
                  }
               }
            }
         }
      }
   } // end loop calo particles

   //fake rate defined as 1 - (N_caloMatched_PFClusters / N_tot_PFClusters)
   for(TString Eta_key: Eta_keys){
      for(TString Et_key: Et_keys){
         if(N_PFtot_binned[Eta_key][Et_key]!=0){
            //cout << Eta_key << " " << Et_key << " " << " Ntot: " << N_PFtot_binned[Eta_key][Et_key] << " Nmatched: " << N_PFmatched_binned[Eta_key][Et_key]  << " fake rate: " << 1-N_PFmatched_binned[Eta_key][Et_key] / N_PFtot_binned[Eta_key][Et_key] << " noise occupancy: " << N_PFtot_binned[Eta_key][Et_key]-N_PFmatched_binned[Eta_key][Et_key] << endl;
            h_PFclusters_caloMatched_fakeRate_EtaEnBinned[Eta_key][Et_key]->Fill(1-N_PFmatched_binned[Eta_key][Et_key]/N_PFtot_binned[Eta_key][Et_key]);
            h_PFclusters_nonCaloMatched_noiseOccupancy_EtaEnBinned[Eta_key][Et_key]->Fill(N_PFtot_binned[Eta_key][Et_key]-N_PFmatched_binned[Eta_key][Et_key]);
         }
      }
   }


  // integrated fake rate:
   if(flag_doEB){
      float N_tot = pfCluster_rawEnergy.GetSize();
      h_PFClusters_caloMatched_fakeRate_EB->Fill(1-N_pfCl/N_tot);
   }
   else if(flag_doEE){
      float N_tot = pfCluster_rawEnergy.GetSize();
      h_PFClusters_caloMatched_fakeRate_EE->Fill(1-N_pfCl/N_tot);
   }

   if(flag_doEB){
      h_caloParticle_size_EB->Fill(N_Cl);
      h_PFClusters_caloMatched_size_EB->Fill(N_pfCl);
   }
   else if(flag_doEE){
      h_caloParticle_size_EE->Fill(N_Cl);
      h_PFClusters_caloMatched_size_EE->Fill(N_pfCl);
   }


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




