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
   // here you could handle more complcated options
   TString foutName = option;

   // output
   fout = new TFile(foutName, "RECREATE"); 
   if(!fout->IsOpen()) throw std::runtime_error("Output file could not be created");
   Info("SlaveBegin", "Writing on %s", foutName.Data());

   // initialize histograms
   h_PFClusters_caloMatched_size    = new TH1F("h_PFClusters_caloMatched_size","h_PFClusters_caloMatched_size",50,0.,50.);
   h_PFClusters_caloMatched_nXtals  = new TH1F("h_PFClusters_caloMatched_nXtals","h_PFClusters_caloMatched_nXtals",50,0.,50.);
   h_PFClusters_caloMatched_energy  = new TH1F("h_PFClusters_caloMatched_energy","h_PFClusters_caloMatched_energy",500,0.,100.);
   h_PFClusters_caloMatched_et      = new TH1F("h_PFClusters_caloMatched_et","h_PFClusters_caloMatched_et",500,0.,100.);
   h_PFClusters_caloMatched_eta     = new TH1F("h_PFClusters_caloMatched_eta","h_PFClusters_caloMatched_eta",300,-3.,3.);
   h_PFClusters_caloMatched_phi     = new TH1F("h_PFClusters_caloMatched_phi","h_PFClusters_caloMatched_phi",128,-3.2,3.2);
   h_PFClusters_caloMatched_eOverEtrue = new TH1F("h_PFClusters_caloMatched_eOverEtrue","h_PFClusters_caloMatched_eOverEtrue",100,0.,2.);

   h_PFClusters_caloMatched_nXtals_vs_xtalEnergy = new TH2F("h_PFClusters_caloMatched_nXtals_vs_xtalEnergy", "h_PFClusters_caloMatched_nXtals_vs_xtalEnergy", 50, 0., 50., 50., 0., 50.);
   h_PFClusters_caloMatched_nXtals_vs_energy = new TH2F("h_PFClusters_caloMatched_nXtals_vs_energy", "h_PFClusters_caloMatched_nXtals_vs_energy", 50, 0., 50., 100., 0., 100.);

   for (int i=0; i<N_perEvent_plots; i++){
     TString histo_name = "h_PFClusterHit_EB_ietaiphi_" + TString::Format("%d", i);
     h_PFClusterHit_EB_ietaiphi.push_back(new TH2F(histo_name, histo_name, 172,-86.,86.,360,1.,361.));
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

   std::cout << "Event information: evt=" << *eventId << std::endl;

   // loop over genParticles
   for (unsigned int igP=0; igP<genParticle_energy.GetSize(); igP++){
     std::cout << "igP=" << igP << " energy=" << genParticle_energy[igP] << " eta=" << genParticle_eta[igP] << " phi=" << genParticle_phi[igP] << std::endl;
   }

   int N_pfCl = 0;

   // loop over caloParticles
   for (unsigned int icP=0; icP<caloParticle_energy.GetSize(); icP++){
     std::cout << "icP=" << icP << " energy=" << caloParticle_energy[icP] << " eta=" << caloParticle_eta[icP] << " phi=" << caloParticle_phi[icP] << std::endl;
     //std::cout                  << " genParticle_energy=" << genParticle_energy[icP] << " eta=" << genParticle_eta[icP] << " phi=" << genParticle_phi[icP] << std::endl;

     int N_pfClH=0;
     int match_pfC_idx=-1;

     // loop over pfClusterHits associated to calo particle
     for (unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){

       // if there is a match bw pfClusterHit and PFClusters, save the index of the PFCluster
       if (map_pfClusterHit_pfCluster[icP][ipfClH] != -1 and pfClusterHit_energy[icP][ipfClH]>min_pfClusterHit_energy){

         N_pfClH++;

         if (entry<N_perEvent_plots) h_PFClusterHit_EB_ietaiphi.at(entry)->Fill(pfClusterHit_ieta[icP][ipfClH], pfClusterHit_iphi[icP][ipfClH], pfClusterHit_energy[icP][ipfClH]);

         match_pfC_idx = map_pfClusterHit_pfCluster[icP][ipfClH]; 

         std::cout << "   ipfClH=" << ipfClH <<  " energy= " << pfClusterHit_energy[icP][ipfClH] << " associated to PFCluster= " << match_pfC_idx << std::endl;


       } // if match 
     } // end loop pfClusterHits

     // fill histograms of the matched pfclusters
     if (match_pfC_idx!=-1){

       N_pfCl++;
       h_PFClusters_caloMatched_nXtals->Fill(N_pfClH);
       h_PFClusters_caloMatched_energy->Fill(pfCluster_energy[match_pfC_idx]);
       h_PFClusters_caloMatched_et->Fill(pfCluster_energy[match_pfC_idx]*TMath::Sin(2*TMath::ATan(TMath::Exp(-pfCluster_eta[match_pfC_idx]))));
       h_PFClusters_caloMatched_phi->Fill(pfCluster_phi[match_pfC_idx]);
       h_PFClusters_caloMatched_eta->Fill(pfCluster_eta[match_pfC_idx]);
       h_PFClusters_caloMatched_eOverEtrue->Fill(pfCluster_energy[match_pfC_idx] / caloParticle_energy[icP]);         

       h_PFClusters_caloMatched_nXtals_vs_energy->Fill(N_pfClH, pfCluster_energy[match_pfC_idx]);
     }
     // reloop over pfCluserHits 
     for (unsigned int ipfClH=0; ipfClH<pfClusterHit_energy[icP].size(); ipfClH++){
       if (map_pfClusterHit_pfCluster[icP][ipfClH] != -1){
         h_PFClusters_caloMatched_nXtals_vs_xtalEnergy->Fill(N_pfClH,pfClusterHit_energy[icP][ipfClH]);
       }
     }

   } // end loop calo particles

   h_PFClusters_caloMatched_size->Fill(N_pfCl);

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
