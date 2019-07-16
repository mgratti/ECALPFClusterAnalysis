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

void PFClusterAnalyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
   Info("Begin", "Called with option %s", option.Data());
   
   TH1::SetDefaultSumw2();

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
   h_PFClusters_caloMatched_energy = new TH1D("h_PFclusters_genMatched_EB_energy","h_PFclusters_genMatched_EB_energy",500,0.,100.);
   
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

   // loop over caloParticles
   for (unsigned int icP=0; icP<caloParticle_energy.GetSize(); icP++){
     std::cout << "icP=" << icP << " energy=" << caloParticle_energy[icP] << " eta=" << caloParticle_eta[icP] << " phi=" << caloParticle_phi[icP] << std::endl;

     // loop over SimHits associated to calo particle
     for (unsigned int isH=0; isH<simHit_energy[icP].size(); isH++){
       // if there is match bw SH and PFClusters 
       if (map_simHit_pfCLuster[icP][isH] != -1 and simHit_energy[icP][isH] >= min_simHit_energy){
         std::cout << "   isH=" << isH <<  " energy= " << simHit_energy[icP][isH] << " associated to PFCluster= " << map_simHit_pfCLuster[icP][isH] << std::endl;
         int match_PFCluster_index = map_simHit_pfCLuster[icP][isH]; 
         h_PFClusters_caloMatched_energy->Fill(pfCluster_energy[icP][match_PFCluster_index]);
       }
     } // end loop simhits
   } // end loop calo particles



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
