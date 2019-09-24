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

using namespace std;


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

   // output
   fout = new TFile(foutName, "RECREATE"); 
   if(!fout->IsOpen()) throw std::runtime_error("Output file could not be created");
   Info("SlaveBegin", "Writing on %s", foutName.Data());

   fout->mkdir("PFCluster_caloMatched");
   fout->mkdir("caloParticle");
   fout->mkdir("EtEta_binned");
   fout->mkdir("SuperCluster");

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

   Eta_keys.push_back("0p00_0p50");
   Eta_keys.push_back("0p50_1p00");
   Eta_keys.push_back("1p00_1p48");
   Eta_keys.push_back("1p48_2p00");
   Eta_keys.push_back("2p00_2p50");
   Eta_keys.push_back("2p50_3p00");
   Eta_edges["0p00_0p50"].first = 0.;
   Eta_edges["0p00_0p50"].second = 0.5;
   Eta_edges["0p50_1p00"].first = 0.5;
   Eta_edges["0p50_1p00"].second = 1.0;
   Eta_edges["1p00_1p48"].first = 1.0;
   Eta_edges["1p00_1p48"].second = 1.479;
   Eta_edges["1p48_2p00"].first = 1.479;
   Eta_edges["1p48_2p00"].second = 2.0;
   Eta_edges["2p00_2p50"].first = 2.0;
   Eta_edges["2p00_2p50"].second = 2.5;
   Eta_edges["2p50_3p00"].first = 2.5;
   Eta_edges["2p50_3p00"].second = 3.0;




   fout->cd("PFCluster_caloMatched");

   int nBins_energy = 200;
   int rangeMin_energy = -5;
   int rangeMax_energy = 105;

   // initialize histograms
   h_PFClusters_caloMatched_size    = new TH1F("h_PFClusters_caloMatched_size","h_PFClusters_caloMatched_size",50,0.,50.);
   h_PFClusters_caloMatched_nRecHit = new TH1F("h_PFClusters_caloMatched_nRecHit","h_PFClusters_caloMatched_nRecHit",50,0.,50.);
   h_PFClusters_caloMatched_nXtals  = new TH1F("h_PFClusters_caloMatched_nXtals","h_PFClusters_caloMatched_nXtals",50,0.,50.);
   h_PFClusters_caloMatched_energy  = new TH1F("h_PFClusters_caloMatched_energy","h_PFClusters_caloMatched_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_PFClusters_caloMatched_et      = new TH1F("h_PFClusters_caloMatched_et","h_PFClusters_caloMatched_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_PFClusters_caloMatched_eta     = new TH1F("h_PFClusters_caloMatched_eta","h_PFClusters_caloMatched_eta",300,-3.,3.);
   h_PFClusters_caloMatched_phi     = new TH1F("h_PFClusters_caloMatched_phi","h_PFClusters_caloMatched_phi",128,-3.2,3.2);
   h_PFClusters_caloMatched_eOverEtrue = new TH1F("h_PFClusters_caloMatched_eOverEtrue","h_PFClusters_caloMatched_eOverEtrue",100,0.,2.);
   h_PFClusters_caloMatched_eOverEtrue_simEnergy = new TH1F("h_PFClusters_caloMatched_eOverEtrue_simEnergy","h_PFClusters_caloMatched_eOverEtrue_simEnergy",100,0.,2.);
   h_PFClusters_caloMatched_nXtals_vs_xtalEnergy = new TH2F("h_PFClusters_caloMatched_nXtals_vs_xtalEnergy", "h_PFClusters_caloMatched_nXtals_vs_xtalEnergy", 50, 0., 50., 50., 0., 50.);
   h_PFClusters_caloMatched_nXtals_vs_energy = new TH2F("h_PFClusters_caloMatched_nXtals_vs_energy", "h_PFClusters_caloMatched_nXtals_vs_energy", 50, 0., 50., 100., 0., 100.);
   h_PFClusters_caloMatched_nRecHit_vs_energy = new TH2F("h_PFClusters_caloMatched_nRecHit_vs_energy", "h_PFClusters_caloMatched_nRecHit_vs_energy", 30, 0., 30., nBins_energy, rangeMin_energy, rangeMax_energy);
   h_PFClusters_caloMatched_nPFClusters_vs_energy = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_energy", "h_PFClusters_caloMatched_nPFCLusters_vs_energy", 8, 0., 8., nBins_energy, rangeMin_energy, rangeMax_energy);
   h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy", "h_PFClusters_caloMatched_nPFCLusters_vs_caloEnergy", 8, 0., 8., nBins_energy, rangeMin_energy, rangeMax_energy);
   h_PFClusters_caloMatched_nPFClusters_vs_eta = new TH2F("h_PFClusters_caloMatched_nPFClusters_vs_eta", "h_PFClusters_caloMatched_nPFCLusters_vs_eta", 270, -3., 3., 8, 0., 8.);


   //EEM
   h_PFClusters_caloMatched_EEM_eta     = new TH1F("h_PFClusters_caloMatched_EEM_eta","h_PFClusters_caloMatched_EEM_eta",300,-3.,3.);
   h_PFClusters_caloMatched_EEM_size    = new TH1F("h_PFClusters_caloMatched_EEM_size","h_PFClusters_caloMatched_EEM_size",50,0.,50.);
   h_PFClusters_caloMatched_EEM_nXtals  = new TH1F("h_PFClusters_caloMatched_EEM_nXtals","h_PFClusters_caloMatched_EEM_nXtals",50,0.,50.);
   h_PFClusters_caloMatched_EEM_energy  = new TH1F("h_PFClusters_caloMatched_EEM_energy","h_PFClusters_caloMatched_EEM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_PFClusters_caloMatched_EEM_et      = new TH1F("h_PFClusters_caloMatched_EEM_et","h_PFClusters_caloMatched_EEM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_PFClusters_caloMatched_EEM_phi     = new TH1F("h_PFClusters_caloMatched_EEM_phi","h_PFClusters_caloMatched_EEM_phi",128,-3.2,3.2);
   h_PFClusters_caloMatched_EEM_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EEM_eOverEtrue","h_PFClusters_caloMatched_EEM_eOverEtrue",100,0.,2.);



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


   //EEP
   h_PFClusters_caloMatched_EEP_eta     = new TH1F("h_PFClusters_caloMatched_EEP_eta","h_PFClusters_caloMatched_EEP_eta",300,-3.,3.);
   h_PFClusters_caloMatched_EEP_size    = new TH1F("h_PFClusters_caloMatched_EEP_size","h_PFClusters_caloMatched_EEP_size",50,0.,50.);
   h_PFClusters_caloMatched_EEP_nXtals  = new TH1F("h_PFClusters_caloMatched_EEP_nXtals","h_PFClusters_caloMatched_EEP_nXtals",50,0.,50.);
   h_PFClusters_caloMatched_EEP_energy  = new TH1F("h_PFClusters_caloMatched_EEP_energy","h_PFClusters_caloMatched_EEP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_PFClusters_caloMatched_EEP_et      = new TH1F("h_PFClusters_caloMatched_EEP_et","h_PFClusters_caloMatched_EEP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_PFClusters_caloMatched_EEP_phi     = new TH1F("h_PFClusters_caloMatched_EEP_phi","h_PFClusters_caloMatched_EEP_phi",128,-3.2,3.2);
   h_PFClusters_caloMatched_EEP_eOverEtrue = new TH1F("h_PFClusters_caloMatched_EEP_eOverEtrue","h_PFClusters_caloMatched_EEP_eOverEtrue",100,0.,2.);



   for (int i=0; i<N_perEvent_plots; i++){
      TString histo_name = "h_PFClusterHit_EB_ietaiphi_" + TString::Format("%d", i);
      h_PFClusterHit_EB_ietaiphi.push_back(new TH2F(histo_name, histo_name, 172,-86.,86.,360,1.,361.));
   }


   fout->cd("caloParticle");
   h_caloParticle_size       = new TH1F("h_caloParticle_size","h_caloParticle_size",50,0.,50.);
   h_caloParticle_energy     = new TH1F("h_caloParticle_energy","h_caloParticle_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_simEnergy  = new TH1F("h_caloParticle_simEnergy","h_caloParticle_simEnergy", nBins_energy, rangeMin_energy, rangeMax_energy);
   h_caloParticle_et      = new TH1F("h_caloParticle_et","h_caloParticle_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_eta     = new TH1F("h_caloParticle_eta","h_caloParticle_eta",300,-3.,3.);
   h_caloParticle_phi     = new TH1F("h_caloParticle_phi","h_caloParticle_phi",128,-3.2,3.2);

   h_caloParticle_EEM_size       = new TH1F("h_caloParticle_EEM_size","h_caloParticle_EEM_size",50,0.,50.);
   h_caloParticle_EEM_energy     = new TH1F("h_caloParticle_EEM_energy","h_caloParticle_EEM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEM_simEnergy  = new TH1F("h_caloParticle_EEM_simEnergy","h_caloParticle_EEM_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEM_et      = new TH1F("h_caloParticle_EEM_et","h_caloParticle_EEM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEM_eta     = new TH1F("h_caloParticle_EEM_eta","h_caloParticle_EEM_eta",300,-3.,3.);
   h_caloParticle_EEM_phi     = new TH1F("h_caloParticle_EEM_phi","h_caloParticle_EEM_phi",128,-3.2,3.2);

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

   h_caloParticle_EEP_size       = new TH1F("h_caloParticle_EEP_size","h_caloParticle_EEP_size",50,0.,50.);
   h_caloParticle_EEP_energy     = new TH1F("h_caloParticle_EEP_energy","h_caloParticle_EEP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEP_simEnergy  = new TH1F("h_caloParticle_EEP_simEnergy","h_caloParticle_EEP_simEnergy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEP_et      = new TH1F("h_caloParticle_EEP_et","h_caloParticle_EEP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEP_eta     = new TH1F("h_caloParticle_EEP_eta","h_caloParticle_EEP_eta",300,-3.,3.);
   h_caloParticle_EEP_phi     = new TH1F("h_caloParticle_EEP_phi","h_caloParticle_EEP_phi",128,-3.2,3.2);


   fout->cd("SuperCluster");
   h_superCluster_energy_EB = new TH1F("h_superCluster_energy_EB","h_superCluster_energy_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_superCluster_energy_EE = new TH1F("h_superCluster_energy_EE","h_superCluster_energy_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_superCluster_e3x3_EB   = new TH1F("h_superCluster_e3x3_EB","h_superCluster_e3x3_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_superCluster_e3x3_EE   = new TH1F("h_superCluster_e3x3_EE","h_superCluster_e3x3_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_superCluster_eta_EB    = new TH1F("h_superCluster_eta_EB","h_superCluster_eta_EB", 300,-3, 3);
   h_superCluster_eta_EE    = new TH1F("h_superCluster_eta_EE","h_superCluster_eta_EE", 300, -3, 3);
   h_superCluster_phi_EB    = new TH1F("h_superCluster_phi_EB","h_superCluster_phi_EB", 128, -3.2, 3.2);
   h_superCluster_phi_EE    = new TH1F("h_superCluster_phi_EE","h_superCluster_phi_EE", 128, -3.2, 3.2);
   h_superCluster_R9_EB     = new TH1F("h_superCluster_R9_EB","h_superCluster_R9_EB",500, 0, 1.2);
   h_superCluster_R9_EE     = new TH1F("h_superCluster_R9_EE","h_superCluster_R9_EE",500, 0, 1.2);

   h_superCluster_caloMatched_energy_EB = new TH1F("h_superCluster_caloMatched_energy_EB","h_superCluster_caloMatched_energy_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_superCluster_caloMatched_energy_EE = new TH1F("h_superCluster_caloMatched_energy_EE","h_superCluster_caloMatched_energy_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_superCluster_caloMatched_e3x3_EB   = new TH1F("h_superCluster_caloMatched_e3x3_EB","h_superCluster_caloMatched_e3x3_EB",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_superCluster_caloMatched_e3x3_EE   = new TH1F("h_superCluster_caloMatched_e3x3_EE","h_superCluster_caloMatched_e3x3_EE",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_superCluster_caloMatched_eta_EB    = new TH1F("h_superCluster_caloMatched_eta_EB","h_superCluster_caloMatched_eta_EB", 300,-3, 3);
   h_superCluster_caloMatched_eta_EE    = new TH1F("h_superCluster_caloMatched_eta_EE","h_superCluster_caloMatched_eta_EE", 300, -3, 3);
   h_superCluster_caloMatched_phi_EB    = new TH1F("h_superCluster_caloMatched_phi_EB","h_superCluster_caloMatched_phi_EB", 128, -3.2, 3.2);
   h_superCluster_caloMatched_phi_EE    = new TH1F("h_superCluster_caloMatched_phi_EE","h_superCluster_caloMatched_phi_EE", 128, -3.2, 3.2);
   h_superCluster_caloMatched_R9_EB     = new TH1F("h_superCluster_caloMatched_R9_EB","h_superCluster_caloMatched_R9_EB",500, 0, 1.2);
   h_superCluster_caloMatched_R9_EE     = new TH1F("h_superCluster_caloMatched_R9_EE","h_superCluster_caloMatched_R9_EE",500, 0, 1.2);




   fout->cd("EtEta_binned");
   for (TString Et_key : Et_keys){
      for (TString Eta_key: Eta_keys){
         TString histo_name            = "h_PFclusters_caloMatched_eOverEtrue_Eta" + Eta_key + "_Et" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name,histo_name,100,0.,2.);
         TString histo_name_n          = "h_PFclusters_caloMatched_eOverEtrue_Eta" + Eta_key + "_En" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_EtaEnBinned[Eta_key][Et_key] = new TH1F(histo_name_n,histo_name_n,100,0.,2.);

         TString histo_name_simEnergy   = "h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + Eta_key + "_Et" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name_simEnergy,histo_name_simEnergy,100,0.,2.);
         TString histo_name_simEnergy_n = "h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + Eta_key + "_En" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEnBinned[Eta_key][Et_key] = new TH1F(histo_name_simEnergy_n,histo_name_simEnergy_n,100,0.,2.);


         TString histo_name_efficiency   = "h_PFclusters_caloMatched_size_Eta" + Eta_key + "_Et" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiency,histo_name_efficiency,100,0.,2.);
         TString histo_name_efficiency_n = "h_PFclusters_caloMatched_size_Eta" + Eta_key + "_En" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEnBinned_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiency_n,histo_name_efficiency_n,100,0.,2.);

         TString histo_name_efficiencySim_n = "h_PFclusters_caloMatched_size_simEnergy_Eta" + Eta_key + "_En" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEnBinned_simEnergy_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiencySim_n,histo_name_efficiencySim_n,100,0.,2.);
         TString histo_name_efficiencySim = "h_PFclusters_caloMatched_size_simEnergy_Eta" + Eta_key + "_Et" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEtBinned_simEnergy_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiencySim,histo_name_efficiencySim,100,0.,2.);


         TString histo_name_size         = "h_caloParticle_size_Eta" + Eta_key + "_Et" + Et_key;
         h_caloParticle_size_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name_size,histo_name_size,100,0.,2.);
         TString histo_name_size_n       = "h_caloParticle_size_Eta" + Eta_key + "_En" + Et_key;
         h_caloParticle_size_EtaEnBinned[Eta_key][Et_key] = new TH1F(histo_name_size_n,histo_name_size_n,100,0.,2.);


      }
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

   fReader.SetLocalEntry(entry); // read the event 

   if (entry % 1000 == 0) Info("Process", "processing event %d", (Int_t)entry);

   // loop over genParticles
   //for (unsigned int igP=0; igP<genParticle_energy.GetSize(); igP++){
   //std::cout << "igP=" << igP << " energy=" << genParticle_energy[igP] << " eta=" << genParticle_eta[igP] << " phi=" << genParticle_phi[igP] << std::endl;
   //}


   //we loop on the (reco) superCluster
   for(unsigned int iSC(0); iSC<superCluster_energy.GetSize(); ++iSC){
      if(superCluster_eta[iSC]>=-1.479 && superCluster_eta[iSC]<=1.479){
         h_superCluster_energy_EB->Fill(superCluster_energy[iSC]);
         h_superCluster_e3x3_EB->Fill(superCluster_e3x3[iSC]);
         h_superCluster_eta_EB->Fill(superCluster_eta[iSC]);
         h_superCluster_phi_EB->Fill(superCluster_phi[iSC]);
         h_superCluster_R9_EB->Fill(superCluster_R9[iSC]);
      }
      if(superCluster_eta[iSC]<-1.479 || superCluster_eta[iSC]>1.479){
         h_superCluster_energy_EE->Fill(superCluster_energy[iSC]);
         h_superCluster_e3x3_EE->Fill(superCluster_e3x3[iSC]);
         h_superCluster_eta_EE->Fill(superCluster_eta[iSC]);
         h_superCluster_phi_EE->Fill(superCluster_phi[iSC]);
         h_superCluster_R9_EE->Fill(superCluster_R9[iSC]);
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


   // loop over caloParticles
   for (unsigned int icP=0; icP<caloParticle_energy.GetSize(); icP++){

      //counts the number of crystals per caloParticle
      int N_pfClH=0;

      //counts the number of superClusterHit per caloParticle
      int N_spClH=0;

      //this vector will contain wht indices of the PFClusters that match with a given PFClusterHit.
      //Most of the time a PFClusterHit is associated to only one PFCluster, but this is not always the case
      vector<int> vector_matched_indices{-1};
      vector<int> vector_matched_indices_single{-1};

      vector<int> vector_spCl_matched_indices{-1};
      vector<int> vector_spCl_matched_indices_single{-1};


      N_Cl++;

      //---caloParticle---
      h_caloParticle_energy->Fill(caloParticle_energy[icP]);
      h_caloParticle_simEnergy->Fill(caloParticle_simEnergy[icP]);
      h_caloParticle_et->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
      h_caloParticle_phi->Fill(caloParticle_phi[icP]);
      h_caloParticle_eta->Fill(caloParticle_eta[icP]);

      if(caloParticle_eta[icP]<-1.479){
         N_Cl_EEM++;
         h_caloParticle_EEM_energy->Fill(caloParticle_energy[icP]);
         h_caloParticle_EEM_simEnergy->Fill(caloParticle_simEnergy[icP]);
         h_caloParticle_EEM_et->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
         h_caloParticle_EEM_phi->Fill(caloParticle_phi[icP]);
         h_caloParticle_EEM_eta->Fill(caloParticle_eta[icP]);
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

      if(caloParticle_eta[icP]>=1.479){
         N_Cl_EEP++;
         h_caloParticle_EEP_energy->Fill(caloParticle_energy[icP]);
         h_caloParticle_EEP_simEnergy->Fill(caloParticle_simEnergy[icP]);
         h_caloParticle_EEP_et->Fill(caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
         h_caloParticle_EEP_phi->Fill(caloParticle_phi[icP]);
         h_caloParticle_EEP_eta->Fill(caloParticle_eta[icP]);
      }

      h_caloParticle_size->Fill(N_Cl);
      h_caloParticle_EEM_size->Fill(N_Cl_EEM); 
      h_caloParticle_EBM_size->Fill(N_Cl_EBM);
      h_caloParticle_EBP_size->Fill(N_Cl_EBP);
      h_caloParticle_EEP_size->Fill(N_Cl_EEP);

      //eta ET binned quantity that stores the number of events (equal to the number of entries of the histogram)
      for(TString Eta_key: Eta_keys){
         for(TString Et_key: Et_keys){
            double caloParticle_et = caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP])));
            if(caloParticle_et>=Et_edges[Et_key].first && caloParticle_et<Et_edges[Et_key].second 
                  && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){
               h_caloParticle_size_EtaEtBinned[Eta_key][Et_key]->Fill(1.);
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
         }
      }


      // caloMatched superCluster
      for(unsigned int ispCl=0; ispCl<superClusterHit_energy[icP].size(); ispCl++){

         //for each superClusterHit, we save a map that lists all the superClusters to which the hit is associated and the energy deposited in the crystal
         map<int, float>  map_superClusters = superClusterHit_energy[icP][ispCl];

         //if the hit is not matched to a PFCluster, the size of the map is 0
         if(map_superClusters.size()!=0){
            //cout << "got it! " << ispCl << endl;
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
      }

      //it has been checked that at most one SuperCluster was matched with the caloParticle
      if(vector_spCl_matched_indices_single.size()>1){
         if(caloParticle_eta[icP]>=-1.479 && caloParticle_eta[icP]<=1.479){
            h_superCluster_caloMatched_energy_EB->Fill(superCluster_energy[0]); 
            h_superCluster_caloMatched_e3x3_EB->Fill(superCluster_e3x3[0]); 
            h_superCluster_caloMatched_eta_EB->Fill(superCluster_eta[0]); 
            h_superCluster_caloMatched_phi_EB->Fill(superCluster_phi[0]); 
            h_superCluster_caloMatched_R9_EB->Fill(superCluster_R9[0]); 
         }
         if(caloParticle_eta[icP]<-1.479 || caloParticle_eta[icP]>1.479){
            h_superCluster_caloMatched_energy_EE->Fill(superCluster_energy[0]); 
            h_superCluster_caloMatched_e3x3_EE->Fill(superCluster_e3x3[0]); 
            h_superCluster_caloMatched_eta_EE->Fill(superCluster_eta[0]); 
            h_superCluster_caloMatched_phi_EE->Fill(superCluster_phi[0]); 
            h_superCluster_caloMatched_R9_EE->Fill(superCluster_R9[0]); 

         }
      }


      // end of caloMatched superCluster


      //---PFClusters_caloMatched---
      // loop over pfClusterHits associated to calo particle
      for(unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){

         //for each pfClusterHit, we save a map that lists all the pfClusters to which the hit is associated and the energy deposited in the crystal
         //A given hit can belong to more than one pfCluster, in case of an overlap

         map<int, float>  map_pfClusters = pfClusterHit_energy[icP][ipfClH];

         //if the hit is not matched to a PFCluster, the size of the map is 0
         if(map_pfClusters.size()!=0){
            N_pfClH++;

            //we get the pfCluster index out of the map and store the all the indices (with repetition)
            for (auto itr = map_pfClusters.begin(); itr != map_pfClusters.end(); ++itr) { 
               vector_matched_indices.push_back(itr->first);
            }

            //same as above, but this time the index is saved only once
            for (auto itr = map_pfClusters.begin(); itr != map_pfClusters.end(); ++itr) { 
               if(std::find(vector_matched_indices_single.begin(), vector_matched_indices_single.end(), itr->first)==vector_matched_indices_single.end()){
                  vector_matched_indices_single.push_back(itr->first);
               }
            }
         }//end match pfClusterHit - pfCluster

         //if(entry<N_perEvent_plots){
         //   h_PFClusterHit_EB_ietaiphi.at(entry)->Fill(pfClusterHit_ieta[icP][ipfClH], pfClusterHit_iphi[icP][ipfClH], pfClusterHit_energy[icP][ipfClH]);
         // }

      } // end loop pfClusterHits


      //cout << "vector of indices: " << endl;
      //for(unsigned int i(0); i<vector_matched_indices.size(); ++i){
      //   cout << vector_matched_indices[i] << endl;
      //}
      //cout << "reduced vector: " << endl;
      //for(unsigned int i(0); i<vector_matched_indices_single.size(); ++i){
      //   cout << vector_matched_indices_single[i] << endl;
      //}

      //for(unsigned int i(0); i<vector_matched_indices_single.size(); ++i){
      //   int nOccurrences = count(vector_matched_indices.begin(), vector_matched_indices.end(), vector_matched_indices_single[i]);
      //   cout << vector_matched_indices_single[i] << " appears " << nOccurrences << endl;
      //}
      //cout << endl;

      //uncomment the next lines in one want to save only one cluster per hit

      //in case a pfClusterHit is associated to more than one pfCluster, we keep the index of the pfCluster having the more hits
      //int matched_index = vector_matched_indices_single[0];
      //for(unsigned int i(1); i<=vector_matched_indices_single.size(); ++i){
      //if(count(vector_matched_indices.begin(), vector_matched_indices.end(), vector_matched_indices_single[i])>count(vector_matched_indices.begin(), vector_matched_indices.end(), vector_matched_indices_single[i-1])){
      //matched_index = vector_matched_indices_single[i];
      //}
      //}
      //cout << "selected index: " << matched_index << endl;


      //we loop on all the PFClusters associated to the same PFClusterHit and sum the energy, eta, phi
      double filling_energy=0;
      double filling_phi=0;
      double filling_eta=0;

      for(unsigned int iMatched(0); iMatched<vector_matched_indices_single.size(); ++iMatched){
         int matched_index = vector_matched_indices_single[iMatched];
         int nOccurrences = count(vector_matched_indices.begin(), vector_matched_indices.end(), vector_matched_indices_single[iMatched]);
         //if(matched_index!=-1 && nOccurrences!=1)
         if(matched_index!=-1){
            filling_energy = pfCluster_energy[matched_index];
            filling_phi    = pfCluster_phi[matched_index];
            filling_eta    = pfCluster_eta[matched_index];

            //number of recHit per PFCluster
            h_PFClusters_caloMatched_nRecHit->Fill(nOccurrences);

            //number of recHit and energy
            h_PFClusters_caloMatched_nRecHit_vs_energy->Fill(nOccurrences, filling_energy);

            N_pfCl++;
            h_PFClusters_caloMatched_nXtals->Fill(N_pfClH);
            h_PFClusters_caloMatched_energy->Fill(filling_energy);
            h_PFClusters_caloMatched_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
            h_PFClusters_caloMatched_phi->Fill(filling_phi);
            h_PFClusters_caloMatched_eta->Fill(filling_eta);
            h_PFClusters_caloMatched_eOverEtrue->Fill(filling_energy / caloParticle_energy[icP]);         
            if(caloParticle_simEnergy[icP]!=-1){
               h_PFClusters_caloMatched_eOverEtrue_simEnergy->Fill(filling_energy / caloParticle_simEnergy[icP]);         
            }
            h_PFClusters_caloMatched_nXtals_vs_energy->Fill(N_pfClH, filling_energy);

            if(caloParticle_eta[icP]<-1.479){
               N_pfCl_EEM++;

               h_PFClusters_caloMatched_EEM_eta->Fill(filling_eta);
               h_PFClusters_caloMatched_EEM_nXtals->Fill(N_pfClH);
               h_PFClusters_caloMatched_EEM_energy->Fill(filling_energy);
               h_PFClusters_caloMatched_EEM_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
               h_PFClusters_caloMatched_EEM_phi->Fill(filling_phi);
               h_PFClusters_caloMatched_EEM_eOverEtrue->Fill(filling_energy / caloParticle_energy[icP]);         

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

            if(caloParticle_eta[icP]>=1.479){
               N_plCl_EEP++;

               h_PFClusters_caloMatched_EEP_eta->Fill(filling_eta);
               h_PFClusters_caloMatched_EEP_nXtals->Fill(N_pfClH);
               h_PFClusters_caloMatched_EEP_energy->Fill(filling_energy);
               h_PFClusters_caloMatched_EEP_et->Fill(filling_energy*TMath::Sin(2*TMath::ATan(TMath::Exp(-filling_eta))));
               h_PFClusters_caloMatched_EEP_phi->Fill(filling_phi);
               h_PFClusters_caloMatched_EEP_eOverEtrue->Fill(filling_energy / caloParticle_energy[icP]);         


            }

            //we fill the caloMatched histograms binned in eta and ET
            for(TString Eta_key: Eta_keys){
               for(TString Et_key: Et_keys){
                  double caloParticle_et = caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP])));
                  if(caloParticle_et>=Et_edges[Et_key].first && caloParticle_et<Et_edges[Et_key].second 
                        && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){
                     h_PFclusters_caloMatched_eOverEtrue_EtaEtBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_energy[icP]);
                     h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEtBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_simEnergy[icP]);


                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     if(filling_energy / caloParticle_energy[icP] > 0.4 && filling_energy / caloParticle_energy[icP] < 1.4){
                        h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);

                     }
                  }
               }
            }


            //we fill the caloMatched histograms binned in eta and energy
            for(TString Eta_key: Eta_keys){
               for(TString Et_key: Et_keys){
                  if(caloParticle_energy[icP]>=Et_edges[Et_key].first && caloParticle_energy[icP]<Et_edges[Et_key].second 
                        && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){


                     h_PFclusters_caloMatched_eOverEtrue_EtaEnBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_energy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     if(filling_energy / caloParticle_energy[icP] > 0.4 && filling_energy / caloParticle_energy[icP] < 1.4){
                        h_PFclusters_caloMatched_size_EtaEnBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);
                        h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency[Eta_key][Et_key]->Fill(1.);

                     }
                  }
                  if(caloParticle_simEnergy[icP]>=Et_edges[Et_key].first && caloParticle_simEnergy[icP]<Et_edges[Et_key].second 
                        && std::abs(caloParticle_eta[icP])>=Eta_edges[Eta_key].first && std::abs(caloParticle_eta[icP])<Eta_edges[Eta_key].second){


                     h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEnBinned[Eta_key][Et_key]->Fill(filling_energy / caloParticle_simEnergy[icP]);

                     //for efficiency calculation get the number of PfClusters_caloMatched with 0.4 < E/Etrue < 1.4
                     if(filling_energy / caloParticle_simEnergy[icP] > 0.4 && filling_energy / caloParticle_simEnergy[icP] < 1.4){
                        h_PFclusters_caloMatched_size_EtaEnBinned_simEnergy_forEfficiency[Eta_key][Et_key]->Fill(1.);
                        h_PFclusters_caloMatched_size_EtaEtBinned_simEnergy_forEfficiency[Eta_key][Et_key]->Fill(1.);


                     }
                  }
               }
            }

            // reloop over pfCluserHits 
            //      for (unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){
            //         if (map_pfClusterHit_pfCluster[icP][ipfClH] != -1){
            //            h_PFClusters_caloMatched_nXtals_vs_xtalEnergy->Fill(N_pfClH,pfClusterHit_energy[icP][ipfClH]);
            //         }
            //      }


            //plot number of recHit related to energy and eta
            h_PFClusters_caloMatched_nPFClusters_vs_energy->Fill(N_pfCl, filling_energy);
            //h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy->Fill(N_pfCl, caloParticle_energy[icP]);
            h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy->Fill(N_pfCl, caloParticle_energy[icP]*TMath::Sin(2*TMath::ATan(TMath::Exp(-caloParticle_eta[icP]))));
            h_PFClusters_caloMatched_nPFClusters_vs_eta->Fill(filling_eta, N_pfCl);

         } //end of matched index
      }// end of loop on matched indices            
   } // end loop calo particles

   h_PFClusters_caloMatched_size->Fill(N_pfCl);
   h_PFClusters_caloMatched_EEM_size->Fill(N_pfCl_EEM);
   h_PFClusters_caloMatched_EBM_size->Fill(N_pfCl_EBM);
   h_PFClusters_caloMatched_EBP_size->Fill(N_pfCl_EBP);
   h_PFClusters_caloMatched_EEP_size->Fill(N_plCl_EEP);



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
