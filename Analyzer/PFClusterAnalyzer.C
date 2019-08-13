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

   Et_keys.push_back("1_20");
   Et_keys.push_back("20_40");
   Et_keys.push_back("40_60");
   Et_keys.push_back("60_80");
   Et_keys.push_back("80_100");
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

   int nBins_energy = 220;
   int rangeMin_energy = -5;
   int rangeMax_energy = 105;

   // initialize histograms
   h_PFClusters_caloMatched_size    = new TH1F("h_PFClusters_caloMatched_size","h_PFClusters_caloMatched_size",50,0.,50.);
   h_PFClusters_caloMatched_nXtals  = new TH1F("h_PFClusters_caloMatched_nXtals","h_PFClusters_caloMatched_nXtals",50,0.,50.);
   h_PFClusters_caloMatched_energy  = new TH1F("h_PFClusters_caloMatched_energy","h_PFClusters_caloMatched_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_PFClusters_caloMatched_et      = new TH1F("h_PFClusters_caloMatched_et","h_PFClusters_caloMatched_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_PFClusters_caloMatched_eta     = new TH1F("h_PFClusters_caloMatched_eta","h_PFClusters_caloMatched_eta",300,-3.,3.);
   h_PFClusters_caloMatched_phi     = new TH1F("h_PFClusters_caloMatched_phi","h_PFClusters_caloMatched_phi",128,-3.2,3.2);
   h_PFClusters_caloMatched_eOverEtrue = new TH1F("h_PFClusters_caloMatched_eOverEtrue","h_PFClusters_caloMatched_eOverEtrue",100,0.,2.);
   h_PFClusters_caloMatched_eOverEtrue_simEnergy = new TH1F("h_PFClusters_caloMatched_eOverEtrue_simEnergy","h_PFClusters_caloMatched_eOverEtrue_simEnergy",100,0.,2.);
   h_PFClusters_caloMatched_nXtals_vs_xtalEnergy = new TH2F("h_PFClusters_caloMatched_nXtals_vs_xtalEnergy", "h_PFClusters_caloMatched_nXtals_vs_xtalEnergy", 50, 0., 50., 50., 0., 50.);
   h_PFClusters_caloMatched_nXtals_vs_energy = new TH2F("h_PFClusters_caloMatched_nXtals_vs_energy", "h_PFClusters_caloMatched_nXtals_vs_energy", 50, 0., 50., 100., 0., 100.);


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
   h_caloParticle_simEnergy  = new TH1F("h_caloParticle_simEnergy","h_caloParticle_simEnergy",500,-5.,100.);
   h_caloParticle_et      = new TH1F("h_caloParticle_et","h_caloParticle_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_eta     = new TH1F("h_caloParticle_eta","h_caloParticle_eta",300,-3.,3.);
   h_caloParticle_phi     = new TH1F("h_caloParticle_phi","h_caloParticle_phi",128,-3.2,3.2);

   h_caloParticle_EEM_size       = new TH1F("h_caloParticle_EEM_size","h_caloParticle_EEM_size",50,0.,50.);
   h_caloParticle_EEM_energy     = new TH1F("h_caloParticle_EEM_energy","h_caloParticle_EEM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEM_simEnergy  = new TH1F("h_caloParticle_EEM_simEnergy","h_caloParticle_EEM_simEnergy",500,-5.,100.);
   h_caloParticle_EEM_et      = new TH1F("h_caloParticle_EEM_et","h_caloParticle_EEM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEM_eta     = new TH1F("h_caloParticle_EEM_eta","h_caloParticle_EEM_eta",300,-3.,3.);
   h_caloParticle_EEM_phi     = new TH1F("h_caloParticle_EEM_phi","h_caloParticle_EEM_phi",128,-3.2,3.2);

   h_caloParticle_EBM_size       = new TH1F("h_caloParticle_EBM_size","h_caloParticle_EBM_size",50,0.,50.);
   h_caloParticle_EBM_energy     = new TH1F("h_caloParticle_EBM_energy","h_caloParticle_EBM_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EBM_simEnergy  = new TH1F("h_caloParticle_EBM_simEnergy","h_caloParticle_EBM_simEnergy",500,-5.,100.);
   h_caloParticle_EBM_et      = new TH1F("h_caloParticle_EBM_et","h_caloParticle_EBM_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EBM_eta     = new TH1F("h_caloParticle_EBM_eta","h_caloParticle_EBM_eta",300,-3.,3.);
   h_caloParticle_EBM_phi     = new TH1F("h_caloParticle_EBM_phi","h_caloParticle_EBM_phi",128,-3.2,3.2);

   h_caloParticle_EBP_size       = new TH1F("h_caloParticle_EBP_size","h_caloParticle_EBP_size",50,0.,50.);
   h_caloParticle_EBP_energy     = new TH1F("h_caloParticle_EBP_energy","h_caloParticle_EBP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EBP_simEnergy  = new TH1F("h_caloParticle_EBP_simEnergy","h_caloParticle_EBP_simEnergy",500,-5.,100.);
   h_caloParticle_EBP_et      = new TH1F("h_caloParticle_EBP_et","h_caloParticle_EBP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EBP_eta     = new TH1F("h_caloParticle_EBP_eta","h_caloParticle_EBP_eta",300,-3.,3.);
   h_caloParticle_EBP_phi     = new TH1F("h_caloParticle_EBP_phi","h_caloParticle_EBP_phi",128,-3.2,3.2);

   h_caloParticle_EEP_size       = new TH1F("h_caloParticle_EEP_size","h_caloParticle_EEP_size",50,0.,50.);
   h_caloParticle_EEP_energy     = new TH1F("h_caloParticle_EEP_energy","h_caloParticle_EEP_energy",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEP_simEnergy  = new TH1F("h_caloParticle_EEP_simEnergy","h_caloParticle_EEP_simEnergy",500,-5.,100.);
   h_caloParticle_EEP_et      = new TH1F("h_caloParticle_EEP_et","h_caloParticle_EEP_et",nBins_energy,rangeMin_energy,rangeMax_energy);
   h_caloParticle_EEP_eta     = new TH1F("h_caloParticle_EEP_eta","h_caloParticle_EEP_eta",300,-3.,3.);
   h_caloParticle_EEP_phi     = new TH1F("h_caloParticle_EEP_phi","h_caloParticle_EEP_phi",128,-3.2,3.2);




   fout->cd("EtEta_binned");
   for (TString Et_key : Et_keys){
      for (TString Eta_key: Eta_keys){
         TString histo_name            = "h_PFclusters_caloMatched_eOverEtrue_Eta" + Eta_key + "_Et" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name,histo_name,100,0.,2.);
         TString histo_name_simEnergy  = "h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + Eta_key + "_Et" + Et_key;
         h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name_simEnergy,histo_name_simEnergy,100,0.,2.);
         TString histo_name_efficiency = "h_PFclusters_caloMatched_size_Eta" + Eta_key + "_Et" + Et_key + "_forEfficiency";
         h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency[Eta_key][Et_key] = new TH1F(histo_name_efficiency,histo_name_efficiency,100,0.,2.);
         TString histo_name_size       = "h_caloParticle_size_Eta" + Eta_key + "_Et" + Et_key;
         h_caloParticle_size_EtaEtBinned[Eta_key][Et_key] = new TH1F(histo_name_size,histo_name_size,100,0.,2.);

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

   //std::cout << "Event information: evt=" << *eventId << std::endl;

   // loop over genParticles
   //for (unsigned int igP=0; igP<genParticle_energy.GetSize(); igP++){
   //std::cout << "igP=" << igP << " energy=" << genParticle_energy[igP] << " eta=" << genParticle_eta[igP] << " phi=" << genParticle_phi[igP] << std::endl;
   //}


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

      //std::cout << "icP=" << icP << " energy=" << caloParticle_energy[icP] << " eta=" << caloParticle_eta[icP] << " phi=" << caloParticle_phi[icP] << std::endl;

      int N_pfClH=0;
      int match_pfC_idx=-1;

      //this vector will contain wht indices of the PFClusters that match with a given PFClusterHit.
      //Most of the time a PFClusterHit is associated to only one PFCluster, but this is not always the case
      vector<int> vector_matched_indices{-1};

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

      //---PFClusters_caloMatched---
      // loop over pfClusterHits associated to calo particle
      for(unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){

         // if there is a match bw pfClusterHit and PFClusters, save the index of the PFCluster
         if(map_pfClusterHit_pfCluster[icP][ipfClH] != -1 and pfClusterHit_energy[icP][ipfClH]>min_pfClusterHit_energy){

            N_pfClH++;

            if(entry<N_perEvent_plots){
               h_PFClusterHit_EB_ietaiphi.at(entry)->Fill(pfClusterHit_ieta[icP][ipfClH], pfClusterHit_iphi[icP][ipfClH], pfClusterHit_energy[icP][ipfClH]);
            }

            match_pfC_idx = map_pfClusterHit_pfCluster[icP][ipfClH]; 

            //we only save the index of the PFCluster if it has not been saved yet
            if(std::find(vector_matched_indices.begin(), vector_matched_indices.end(), match_pfC_idx)==vector_matched_indices.end()){
               vector_matched_indices.push_back(match_pfC_idx);
            }

         } // if match 
      } // end loop pfClusterHits

      //we loop on all the PFClusters associated to the same PFClusterHit and sum the energy, eta, phi
      for(unsigned int i(0); i<vector_matched_indices.size(); ++i){
      }

      double filling_energy=0;
      double filling_phi=0;
      double filling_eta=0;

      for(unsigned int iMatched(0); iMatched<vector_matched_indices.size(); ++iMatched){
         int matched_index = vector_matched_indices[iMatched];
         if (matched_index!=-1){
            filling_energy += pfCluster_energy[matched_index];
            filling_phi    += pfCluster_phi[matched_index];
            filling_eta    += pfCluster_eta[matched_index];
         }
      }

      // eta and phi as the average on all the PFClusters
      filling_eta = filling_eta/(vector_matched_indices.size()-1);
      filling_phi = filling_phi/(vector_matched_indices.size()-1);
      
      //we fill the histograms if at least one PFCluster has been found to match with the caloParticle
      if(vector_matched_indices.size()>1){

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
      }

      // reloop over pfCluserHits 
      for (unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){
         if (map_pfClusterHit_pfCluster[icP][ipfClH] != -1){
            h_PFClusters_caloMatched_nXtals_vs_xtalEnergy->Fill(N_pfClH,pfClusterHit_energy[icP][ipfClH]);
         }
      }



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
