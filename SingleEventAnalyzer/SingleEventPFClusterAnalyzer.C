#define SingleEventPFClusterAnalyzer_cxx
// The class definition in SingleEventPFClusterAnalyzer.h has been generated automatically
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
// root> T->Process("SingleEventPFClusterAnalyzer.C")
// root> T->Process("SingleEventPFClusterAnalyzer.C","some options")
// root> T->Process("SingleEventPFClusterAnalyzer.C+")
//


#include "SingleEventPFClusterAnalyzer.h"
#include <TH2.h>
#include <TStyle.h>

void SingleEventPFClusterAnalyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString foutName = GetOption();
}

void SingleEventPFClusterAnalyzer::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString foutName = GetOption();

   // events to check
   eventsToCheck.push_back(50);
   eventsToCheck.push_back(2);
   eventsToCheck.push_back(7);

   N_perEvent_plots=eventsToCheck.size();
   if(N_perEvent_plots>N_max_perEvent_plots) throw "You want to check more events than currently allowed, please correct!";
   // flags for algorithm
   flag_doEB = false;
   flag_doEE = false;
   //turns flag_doEB/flag_doEE on depending on the outputFile
   std::string foutName_str = foutName.Data();
   if(foutName_str.find("_EB") != std::string::npos){
     flag_doEB = true;
     cout << "turning on EB flag" << endl;
   }
   else if(foutName_str.find("_EE") != std::string::npos){
     flag_doEE = true;
     cout << "turning on EE flag" << endl;
   }

   // prepare output
   fout = new TFile(foutName, "RECREATE"); 
   if(!fout->IsOpen()) throw std::runtime_error("Output file could not be created");
   Info("SlaveBegin", "Writing on %s", foutName.Data());

   // create dir structure and book histograms here
   for (auto evt : eventsToCheck){
     TString evt_dir = "evt_" + TString::Format("%d", evt);
     fout->mkdir(evt_dir);
     for(auto det : detectors){
       fout->mkdir(evt_dir+"/"+det);
       fout->cd(evt_dir+"/"+det);
       TString histo_name = "PFClusters_index";
       h_PFClusters_index[evt][det] = new TH2F(histo_name, histo_name, nX[det], lX[det], hX[det], nY[det], lY[det], hY[det]);
       histo_name = "PFClusters_energy";
       h_PFClusters_energy[evt][det] = new TH2F(histo_name, histo_name, nX[det], lX[det], hX[det], nY[det], lY[det], hY[det]);
       for (int iPFCl=0; iPFCl<N_perEvent_perCluster_plots; iPFCl++){
         TString pfcl_dir = "PFCl_" + TString::Format("%d", iPFCl);
         fout->mkdir(evt_dir+"/"+det+"/"+pfcl_dir);
         fout->cd(evt_dir+"/"+det+"/"+pfcl_dir);
         histo_name = "cl_energy";
         h_PFClusters_cl_energy[evt][det][iPFCl] = new TH2F(histo_name, histo_name, nX[det], lX[det], hX[det], nY[det], lY[det], hY[det]);
         histo_name = "hits_fraction";
         h_PFClusters_hits_fraction[evt][det][iPFCl] = new TH2F(histo_name, histo_name, nX[det], lX[det], hX[det], nY[det], lY[det], hY[det]);
         histo_name = "hits_energy";
         h_PFClusters_hits_energy[evt][det][iPFCl] = new TH2F(histo_name, histo_name, nX[det], lX[det], hX[det], nY[det], lY[det], hY[det]);
         histo_name = "bestCp_energy";
         h_PFClusters_bestCp_energy[evt][det][iPFCl] = new TH2F(histo_name, histo_name, nX[det], lX[det], hX[det], nY[det], lY[det], hY[det]);
         histo_name = "bestCp_dummy";
         h_PFClusters_bestCp_dummy[evt][det][iPFCl] = new TH2F(histo_name, histo_name, nX[det], lX[det], hX[det], nY[det], lY[det], hY[det]);
         histo_name = "bestCp_simFraction";
         h_PFClusters_bestCp_simFraction[evt][det][iPFCl] = new TH2F(histo_name, histo_name, nX[det], lX[det], hX[det], nY[det], lY[det], hY[det]);
       }
     } 
   }
   

   Info("Begin", "Booked Histograms");
}

Bool_t SingleEventPFClusterAnalyzer::Process(Long64_t entry)
{

   fReader.SetLocalEntry(entry);
   if (entry % 10 == 0) Info("Process", "processing event %d", (Int_t)entry);

   // check whether current evt belongs to list of chosen events
   // choice is based  on event counter!
   int evt = (Int_t)entry; 
   bool doFill=false;
   if(std::find(eventsToCheck.begin(), eventsToCheck.end(), evt) != eventsToCheck.end()) 
     doFill = true;

   if(pfCluster_energy.GetSize() > N_perEvent_perCluster_plots) 
     std::cout << "Warning: Number of pfclusters in evt=" << evt << " is too high, please increase N_perEvent_perCluster_plots" << std::endl;
   // PFClusters loop
   for(int iPFCl=0; iPFCl<pfCluster_energy.GetSize(); iPFCl++){

     if (iPFCl >= N_perEvent_perCluster_plots) break; // protection against too high number of pfclusters

     if( (flag_doEB && fabs(pfCluster_eta[iPFCl])>1.479) || (flag_doEE && fabs(pfCluster_eta[iPFCl])<1.479) ) continue;
     
     // determine sub-detector of current pfcluster
     TString det = "EB";
     if(pfCluster_eta[iPFCl]>1.479) det = "EEP";
     else if(pfCluster_eta[iPFCl]<-1.479) det = "EEM";
     // save indexes and energy of pfcluster
     if(doFill) h_PFClusters_index[evt][det]->Fill(pfCluster_ieta[iPFCl],pfCluster_iphi[iPFCl],iPFCl);
     if(doFill) h_PFClusters_energy[evt][det]->Fill(pfCluster_ieta[iPFCl],pfCluster_iphi[iPFCl],pfCluster_energy[iPFCl]);
     if(doFill) h_PFClusters_cl_energy[evt][det][iPFCl]->Fill(pfCluster_ieta[iPFCl],pfCluster_iphi[iPFCl],pfCluster_energy[iPFCl]);

     // save RH fractions and energies
     float N_fracs_above30=0; // count pathological cases for each pfcluster
     for(int iPFClH=0; iPFClH<pfClusterHit_energy[iPFCl].size();iPFClH++){
       double fraction = pfClusterHit_energy[iPFCl][iPFClH]/pfClusterHit_rechitEnergy[iPFCl][iPFClH];
       if(fraction >= 0.3 && fraction<=0.7){ 
         N_fracs_above30++;
         if (N_fracs_above30==1) {
           N_pfcl_fracs_above30++; // increment only once for pfcluster
           std::cout << "Found fraction > 30%, frac=" << fraction << " evt="<< evt << " iPFCl="<< iPFCl << " energy=" << pfCluster_energy[iPFCl] 
                     << " det="<< det << " ieta/ix=" << pfCluster_ieta[iPFCl] << " iphi/iy=" << pfCluster_iphi[iPFCl] << std::endl;
           }
       }
       // with fraction weighting
       if(doFill) h_PFClusters_hits_energy[evt][det][iPFCl]->Fill(pfClusterHit_ieta[iPFCl][iPFClH],pfClusterHit_iphi[iPFCl][iPFClH],pfClusterHit_energy[iPFCl][iPFClH]); 
       //if(doFill) h_PFClusters_hits_fraction[evt][det][iPFCl]->Fill(pfClusterHit_ieta[iPFCl][iPFClH],pfClusterHit_iphi[iPFCl][iPFClH],pfClusterHit_energy[iPFCl][iPFClH]);
       if(doFill) h_PFClusters_hits_fraction[evt][det][iPFCl]->Fill(pfClusterHit_ieta[iPFCl][iPFClH],pfClusterHit_iphi[iPFCl][iPFClH],fraction);
     }     

     // find best matching caloP
     int bestCp_index = -1;
     double bestCp_score = -1;
     for(unsigned int icP=0; icP<caloParticle_genEnergy.GetSize(); icP++){
       if(pfCluster_sim_fraction[iPFCl][icP] > 0.01) { // pass sim fraction criterion
         if(pfCluster_sim_fraction[iPFCl][icP] > bestCp_score){
           bestCp_score = pfCluster_sim_fraction[iPFCl][icP];
           bestCp_index = icP;
         }
       }
     }
     // save bestCP information
     if(bestCp_index!=-1 && bestCp_score!=-1){
       //std::cout << "iPF=" << iPFCl << " energy=" << pfCluster_energy[iPFCl] << " bestCP=" << bestCp_index << " score=" << bestCp_score << " energy=" << caloParticle_simEnergy[bestCp_index] <<std::endl;
       if(doFill) h_PFClusters_bestCp_dummy[evt][det][iPFCl]->Fill(caloParticle_simIeta[bestCp_index],caloParticle_simIphi[bestCp_index],1);
       if(doFill) h_PFClusters_bestCp_energy[evt][det][iPFCl]->Fill(caloParticle_simIeta[bestCp_index],caloParticle_simIphi[bestCp_index],caloParticle_simEnergy[bestCp_index]);
       if(doFill) h_PFClusters_bestCp_simFraction[evt][det][iPFCl]->Fill(caloParticle_simIeta[bestCp_index],caloParticle_simIphi[bestCp_index],bestCp_score);
     }
   } // PFClusters loop

   return kTRUE;
}

void SingleEventPFClusterAnalyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.


   // 
   Info("SlaveTerminate", "Remarks about analysis");
   std::cout << "total N_fracs_above30/2=" << N_pfcl_fracs_above30/2. << " in Nevts=" << N_perEvent_plots << std::endl; 

   // write and close 
   fout->Write();
   Info("SlaveTerminate", "Output was written. Going to free memory now... will take some time");
   for (auto evt : eventsToCheck){
     std::cout << "   evt=" << evt << std::endl;
     for(auto det : detectors){
       std::cout << "      det=" << det << std::endl;
       delete h_PFClusters_index[evt][det];
       delete h_PFClusters_energy[evt][det]; 
       for (int iPFCl=0; iPFCl<N_perEvent_perCluster_plots; iPFCl++){
         //std::cout << "pfcl=" << iPFCl << std::endl;
         delete h_PFClusters_cl_energy[evt][det][iPFCl];
         delete h_PFClusters_hits_fraction[evt][det][iPFCl];
         delete h_PFClusters_hits_energy[evt][det][iPFCl];
         delete h_PFClusters_bestCp_energy[evt][det][iPFCl];
         delete h_PFClusters_bestCp_dummy[evt][det][iPFCl];
         delete h_PFClusters_bestCp_simFraction[evt][det][iPFCl];
       }
     } 
   }
   fout->Close();

   Info("SlaveTerminate", "Closed output file"); 



}

void SingleEventPFClusterAnalyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}
