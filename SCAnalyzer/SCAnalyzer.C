#define SCAnalyzer_cxx
// The class definition in SCAnalyzer.h has been generated automatically
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
// root> T->Process("SCAnalyzer.C")
// root> T->Process("SCAnalyzer.C","some options")
// root> T->Process("SCAnalyzer.C+")
//


#include "SCAnalyzer.h"
#include <TH2.h>
#include <TStyle.h>
#include <TMath.h>
#include <TVector2.h>

bool compareSCpt(const std::pair<int, double>&, const std::pair<int, double>&);
bool compareCPdr(const std::tuple<int, double, double>&, const std::tuple<int, double, double>&);

void SCAnalyzer::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();
}

void SCAnalyzer::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString foutName = GetOption();
   // prepare output
   fout = new TFile(foutName, "RECREATE"); 
   if(!fout->IsOpen()) throw std::runtime_error("Output file could not be created");
   Info("SlaveBegin", "Writing on %s", foutName.Data());
   fout->mkdir("SClusters");
   fout->cd("SClusters"); 
   h_goodSC_R9 = new TH1F("h_goodSC_R9", "h_goodSC_R9", 200,0.,2.);
   h_goodSC_SigmaIetaIeta = new TH1F("h_goodSC_SigmaIetaIeta", "h_goodSC_SigmaIetaIeta", 200,0.,0.1);
   h_goodSC_SigmaIphiIphi = new TH1F("h_goodSC_SigmaIphiIphi", "h_goodSC_SigmaIphiIphi", 200,0.,0.5);
   h_goodSC_fullR9 = new TH1F("h_goodSC_fullR9", "h_goodSC_fullR9", 200,0.,2.);
   h_goodSC_fullSigmaIetaIeta = new TH1F("h_goodSC_fullSigmaIetaIeta", "h_goodSC_fullSigmaIetaIeta", 200,0.,0.1);
   h_goodSC_fullSigmaIphiIphi = new TH1F("h_goodSC_fullSigmaIphiIphi", "h_goodSC_fullSigmaIphiIphi", 200,0.,0.5);
   h_fakeSC_R9 = new TH1F("h_fakeSC_R9", "h_fakeSC_R9", 200,0.,2.);
   h_fakeSC_SigmaIetaIeta = new TH1F("h_fakeSC_SigmaIetaIeta", "h_fakeSC_SigmaIetaIeta", 200,0.,0.1);
   h_fakeSC_SigmaIphiIphi = new TH1F("h_fakeSC_SigmaIphiIphi", "h_fakeSC_SigmaIphiIphi", 200,0.,0.5);
   h_fakeSC_fullR9 = new TH1F("h_fakeSC_fullR9", "h_fakeSC_fullR9", 200,0.,2.);
   h_fakeSC_fullSigmaIetaIeta = new TH1F("h_fakeSC_fullSigmaIetaIeta", "h_fakeSC_fullSigmaIetaIeta", 200,0.,0.1);
   h_fakeSC_fullSigmaIphiIphi = new TH1F("h_fakeSC_fullSigmaIphiIphi", "h_fakeSC_fullSigmaIphiIphi", 200,0.,0.5);

   Info("SalveBegin", "Booked Histograms");
   
}

Bool_t SCAnalyzer::Process(Long64_t entry)
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
   if (entry % 100 == 0) Info("Process", "processing event %d", (Int_t)entry);
   if(doDebug) std::cout << "Event number = " << *eventId << std::endl;

   // selection - only events with at least one sim photon with pt > 2 GeV
   bool keep_me=false;
   for (int iCP=0; iCP<caloParticle_simEnergy.GetSize(); iCP++){
     if(caloParticle_id[iCP]==22 && caloParticle_simEnergy[iCP]/TMath::CosH(caloParticle_simEta[iCP])>2)
       keep_me=true;
   }
   if(!keep_me) return kTRUE;

   // *******
   // SuperCluster selection: highest pt supercluster that matches with a photon caloparticle:
   // caloparticle must have DR < max_DE and DE/E < max_DR
   // *******
   bool best_SC_idx = -1;
   double max_DE = 0.20;
   double max_DR = 0.1;;

   // first order the indices of the superclusters by pt, from highest pt to lowest pt
   std::vector< std::pair<int, double> > SC_idx_pt;

   for(int iSC=0; iSC<superCluster_energy.GetSize(); iSC++){
     SC_idx_pt.push_back(std::make_pair(iSC, superCluster_energy[iSC]/TMath::CosH(superCluster_eta[iSC])));
   }

   sort(SC_idx_pt.begin(),SC_idx_pt.end(),compareSCpt);

   //std::cout << "Sorted indices " << std::endl;
   //for (auto i : SC_idx_pt) std::cout << " " << i.second << " " << i.first << std::endl;

   // then start from high pt and look for a good truth match, otherwise go to next supercluster
   int idx_goodSC = -1;
   int idx_cand_CP = -1;

   // loop over superclusters ordered indices
   for(auto SC_idx_pt_el : SC_idx_pt){ 
     
     int iSC = SC_idx_pt_el.first;
     std::vector<std::tuple<int,double,double>> CP_idx_DR_DE;
     
     // order caloparticle indices by best DeltaR
     for(int iCP=0; iCP<caloParticle_simEnergy.GetSize(); iCP++){

       if(caloParticle_id[iCP]!=22) continue;
       double Deta = abs(caloParticle_simEta[iCP]-superCluster_eta[iSC]);
       //double Dphi = abs(caloParticle_simPhi[iCP]-superCluster_phi[iSC]); BUG!
       double Dphi = abs(TVector2::Phi_mpi_pi(caloParticle_simPhi[iCP]-superCluster_phi[iSC]));
       double DR = sqrt(Deta*Deta+Dphi*Dphi);
       double DE = abs(caloParticle_simEnergy[iCP]-superCluster_energy[iSC])/superCluster_energy[iSC];
       //std::cout << "DR=" << DR << " DE=" << DE << std::endl;
       CP_idx_DR_DE.push_back(std::tuple(iCP,DR,DE));
     }
      
     sort(CP_idx_DR_DE.begin(),CP_idx_DR_DE.end(),compareCPdr);
     //std::cout << "Supercluster="<< iSC << " eta=" << superCluster_eta[iSC] << " energy=" << superCluster_energy[iSC] << std::endl;
     //std::cout << "  Sorted CPs " << std::endl;
     //for (auto i : CP_idx_DR_DE) std::cout << "    " << std::get<0>(i) << " " << std::get<1>(i) << " " << std::get<2>(i)<< std::endl;

     for(auto i: CP_idx_DR_DE){
       int iCP_idx = std::get<0>(i);
       double iCP_DR = std::get<1>(i);
       double iCP_DE = std::get<2>(i);

       if(iCP_DR <= max_DR && iCP_DE <= max_DE){
         //std::cout << "DR=" << iCP_DR << " DE=" << iCP_DE << std::endl;
         idx_goodSC = iSC;
         idx_cand_CP = iCP_idx;
         break; // you found a candidate SP matched with a candidate CP
       }
       // else go on the next caloParticle
     }
   }
   if(idx_goodSC==-1) {
     std::cout << "Did not find a candidate Good SuperCluster in the event, going to next event" << std::endl;
     return kTRUE;
   } else {
     if(doDebug) std::cout << "Candidate SC=" << idx_goodSC << " eta=" << superCluster_eta[idx_goodSC] << " energy=" << superCluster_energy[idx_goodSC] 
                           << " phi=" << superCluster_phi[idx_goodSC] << std::endl;
     if(doDebug) std::cout << "Candidate CP=" << idx_cand_CP << " eta=" << caloParticle_simEta[idx_cand_CP] << " energy=" << caloParticle_simEnergy[idx_cand_CP] 
                           << " phi=" << caloParticle_simPhi[idx_cand_CP] << std::endl;
   }

   // ************************
   // Now identify the SC from the jet... 
   // ************************
   // loop over the superclusters, ordered in pt, find the supercluster with DeltaPhi~3.14
   // find the highest in pt which has pi-tol<DeltaPhi<pi+tol
   int idx_fakeSC = -1;
   double tol = 0.3;
   for(auto SC_idx_pt_el : SC_idx_pt){
     int iSC = SC_idx_pt_el.first;
     if(iSC==idx_goodSC) continue;
     double Dphi = TVector2::Phi_mpi_pi(superCluster_phi[iSC]-superCluster_phi[idx_goodSC]);
     if(abs(Dphi)< TMath::Pi()+tol && abs(Dphi)>TMath::Pi()-tol){
       idx_fakeSC = iSC;
       break;
     }
   }
   if(idx_fakeSC==-1){
     std::cout << "Did not find a candidate Fake SuperCluster in the envent, going to next event" << std::endl;
     return kTRUE;
   } else {
     if(doDebug) std::cout << "Candidate fake SC=" << idx_fakeSC << " eta=" << superCluster_eta[idx_fakeSC] << " energy=" << superCluster_energy[idx_fakeSC] 
                           << " phi=" << superCluster_phi[idx_fakeSC] << std::endl;
   
   }


   // At this point you have two two clusters of interest, just fill the histograms
   h_goodSC_R9->Fill(superCluster_r9[idx_goodSC]);
   h_goodSC_SigmaIetaIeta->Fill(superCluster_sigmaIetaIeta[idx_goodSC]);
   h_goodSC_SigmaIphiIphi->Fill(superCluster_sigmaIphiIphi[idx_goodSC]);
   h_goodSC_fullR9->Fill(superCluster_full5x5_r9[idx_goodSC]);
   h_goodSC_fullSigmaIetaIeta->Fill(superCluster_full5x5_sigmaIetaIeta[idx_goodSC]);
   h_goodSC_fullSigmaIphiIphi->Fill(superCluster_full5x5_sigmaIphiIphi[idx_goodSC]);

   h_fakeSC_R9->Fill(superCluster_r9[idx_fakeSC]);
   h_fakeSC_SigmaIetaIeta->Fill(superCluster_sigmaIetaIeta[idx_fakeSC]);
   h_fakeSC_SigmaIphiIphi->Fill(superCluster_sigmaIphiIphi[idx_fakeSC]);
   h_fakeSC_fullR9->Fill(superCluster_full5x5_r9[idx_fakeSC]);
   h_fakeSC_fullSigmaIetaIeta->Fill(superCluster_full5x5_sigmaIetaIeta[idx_fakeSC]);
   h_fakeSC_fullSigmaIphiIphi->Fill(superCluster_full5x5_sigmaIphiIphi[idx_fakeSC]);
    

   return kTRUE;
}

void SCAnalyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   fout->Write();
   Info("SlaveTerminate", "Output file was written");
   fout->Close();
   Info("SlaveTerminate", "Closed output file"); 


}

void SCAnalyzer::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

}


bool compareSCpt(const std::pair<int, double>&i, const std::pair<int, double>&j)
{
   return i.second > j.second;
}


bool compareCPdr(const std::tuple<int, double, double>&i, const std::tuple<int, double, double>&j)
{
   return std::get<1>(i) < std::get<1>(j);
}

