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
#include <TStyle.h>
#include <TMath.h>
#include <TVector2.h>

bool compareSCpt(const std::pair<int, double>&, const std::pair<int, double>&);
bool compareCPpt(const std::tuple<int, double, double, double>&, const std::tuple<int, double, double, double>&);

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

   // define the binning
   detBins["all"] = std::make_pair(0.,3.0);
   detBins["EB"] = std::make_pair(0.,1.44);
   detBins["EEclose"] = std::make_pair(1.48,2.2);
   detBins["EEfar"] = std::make_pair(2.2,3.0);

   TString foutName = GetOption();
   // prepare output
   fout = new TFile(foutName, "RECREATE"); 
   if(!fout->IsOpen()) throw std::runtime_error("Output file could not be created");
   Info("SlaveBegin", "Writing on %s", foutName.Data());
   fout->mkdir("SClusters");
   fout->cd("SClusters");
   for (auto detBin: detBins){ 
     h_goodSC_R9[detBin.first] = new TH1F("h_goodSC_R9_"+detBin.first, "h_goodSC_R9_"+detBin.first, 200,0.,2.);
     h_goodSC_SigmaIetaIeta[detBin.first] = new TH1F("h_goodSC_SigmaIetaIeta_"+detBin.first, "h_goodSC_SigmaIetaIeta_"+detBin.first, 200,0.,0.1);
     h_goodSC_SigmaIphiIphi[detBin.first] = new TH1F("h_goodSC_SigmaIphiIphi_"+detBin.first, "h_goodSC_SigmaIphiIphi_"+detBin.first, 200,0.,0.1);
     h_goodSC_fullR9[detBin.first] = new TH1F("h_goodSC_fullR9_"+detBin.first, "h_goodSC_fullR9_"+detBin.first, 200,0.,2.);
     h_goodSC_fullSigmaIetaIeta[detBin.first] = new TH1F("h_goodSC_fullSigmaIetaIeta_"+detBin.first, "h_goodSC_fullSigmaIetaIeta_"+detBin.first, 200,0.,0.1);
     h_goodSC_fullSigmaIphiIphi[detBin.first] = new TH1F("h_goodSC_fullSigmaIphiIphi_"+detBin.first, "h_goodSC_fullSigmaIphiIphi_"+detBin.first, 200,0.,0.1);
     h_fakeSC_R9[detBin.first] = new TH1F("h_fakeSC_R9_"+detBin.first, "h_fakeSC_R9_"+detBin.first, 200,0.,2.);
     h_fakeSC_SigmaIetaIeta[detBin.first] = new TH1F("h_fakeSC_SigmaIetaIeta_"+detBin.first, "h_fakeSC_SigmaIetaIeta_"+detBin.first, 200,0.,0.1);
     h_fakeSC_SigmaIphiIphi[detBin.first] = new TH1F("h_fakeSC_SigmaIphiIphi_"+detBin.first, "h_fakeSC_SigmaIphiIphi_"+detBin.first, 200,0.,0.1);
     h_fakeSC_fullR9[detBin.first] = new TH1F("h_fakeSC_fullR9_"+detBin.first, "h_fakeSC_fullR9_"+detBin.first, 200,0.,2.);
     h_fakeSC_fullSigmaIetaIeta[detBin.first] = new TH1F("h_fakeSC_fullSigmaIetaIeta_"+detBin.first, "h_fakeSC_fullSigmaIetaIeta_"+detBin.first, 200,0.,0.1);
     h_fakeSC_fullSigmaIphiIphi[detBin.first] = new TH1F("h_fakeSC_fullSigmaIphiIphi_"+detBin.first, "h_fakeSC_fullSigmaIphiIphi_"+detBin.first, 200,0.,0.1);
   }

   h_goodSC_eta = new TH1F("h_goodSC_kineta", "h_goodSC_kineta", 60,0.,3.);
   h_goodSC_phi = new TH1F("h_goodSC_kinphi", "h_goodSC_kinphi", 64,-3.2,3.2);
   h_goodSC_et = new TH1F("h_goodSC_kinet", "h_goodSC_kinet", 100,0.,100.);
   h_goodSC_energy = new TH1F("h_goodSC_kinenergy", "h_goodSC_kinenergy", 100,0.,100.);

   h_fakeSC_eta = new TH1F("h_fakeSC_kineta", "h_fakeSC_kineta", 60,0.,3.);
   h_fakeSC_phi = new TH1F("h_fakeSC_kinphi", "h_fakeSC_kinphi", 64,-3.2,3.2);
   h_fakeSC_et = new TH1F("h_fakeSC_kinet", "h_fakeSC_kinet", 100,0.,100.);
   h_fakeSC_energy = new TH1F("h_fakeSC_kinenergy", "h_fakeSC_kinenergy", 100,0.,100.);
   
   h_goodFakeSC_deta = new TH1F("h_goodFakeSC_kindeta", "h_goodFakeSC_kindeta", 60,0.,3.);
   h_goodFakeSC_dphi = new TH1F("h_goodFakeSC_kindphi", "h_goodFakeSC_kindphi", 32,0.,3.2);
   h_goodFakeSC_detrel = new TH1F("h_goodFakeSC_kindetrel", "h_goodFakeSC_kindetrel", 100,0.,10.);
   h_goodFakeSC_denergyrel = new TH1F("h_goodFakeSC_kindenergyrel", "h_goodFakeSC_kindenergyrel", 100,0.,10.);

   h_ptleading = new TH1F("h_ptleading", "h_ptleading", 100,0.,100.);
   h_leadSCmatched_et = new TH1F("h_leadSCmatched_et", "h_leadSCmatched_et", 100,0.,100.);
   h_leadSCmatched_eta = new TH1F("h_leadSCmatched_eta", "h_leadSCmatched_eta", 60,0.,3.);
   h_phSimMatched_et = new TH1F("h_phSimMatched_kinet", "h_phSimMatched_kinet", 100,0.,100.);
   h_phSimMatched_eta = new TH1F("h_phSimMatched_kineta", "h_phSimMatched_kineta", 60,0.,3.);
   //h_phGenLead_et = new TH1F("h_phGenLead_kinet", "h_phGenLead_kinet", 100,0.,100.);
   //h_phGenLead_eta = new TH1F("h_phGenLead_kineta", "h_phGenLead_kineta", 60,0.,3.);
   h_phSimLead_et = new TH1F("h_phSimLead_kinet", "h_phSimLead_kinet", 100,0.,100.);
   h_phSimLead_eta = new TH1F("h_phSimLead_kineta", "h_phSimLead_kineta", 60,0.,3.);
   h_goodSCptOverCPpt_below15 = new TH1F("h_goodSCptOverCPpt_below15", "h_goodSCptOverCPpt_below15", 100., 0., 2.);
   h_goodSCptOverCPpt_above15 = new TH1F("h_goodSCptOverCPpt_above15", "h_goodSCptOverCPpt_above15", 100., 0., 2.);
   h_below15_EB_ietaiphi = new TH2F("h_below15_EB_ietaiphi", "h_below15_EB_ietaiphi",  200, -100., 100., 360,0.,360.);
   h_below15_EE_ietaiphi = new TH2F("h_below15_EE_ietaiphi", "h_below15_EE_ietaiphi",  100, 0., 100., 100,0.,100.);
   h_below15_rOtbelow80_EB_ietaiphi = new TH2F("h_below15_rOtbelow80_EB_ietaiphi", "h_below15_rOtbelow80_EB_ietaiphi",  200, -100., 100., 360,0.,360.);
   h_below15_rOtbelow80_EE_ietaiphi = new TH2F("h_below15_rOtbelow80_EE_ietaiphi", "h_below15_rOtbelow80_EE_ietaiphi",  100, 0., 100., 100,0.,100.);

   Info("SalveBegin", "Booked Histograms");
   
}

Bool_t SCAnalyzer::Process(Long64_t entry)
{

   fReader.SetLocalEntry(entry);
   if (entry % 100 == 0) Info("Process", "processing event %d", (Int_t)entry);
   if(doDebug) { if ((Int_t)entry == 5000) throw std::runtime_error("exiting");}
   if(doDebug) std::cout << "Event number = " << *eventId << std::endl;
   NtotEvents++;

   // ********
   // Event Selection
   // ********
   // only events with at least one GEN photon with pt > 20 GeV (TODO: 40 GeV if using different sample)

   // order calo particles in pt
   std::vector< std::pair<int, double> > CP_idx_pt;
   for (int iCP=0; iCP<caloParticle_simEnergy.GetSize(); iCP++){
     CP_idx_pt.push_back(std::make_pair(iCP, caloParticle_simPt[iCP]));
   }
   sort(CP_idx_pt.begin(),CP_idx_pt.end(),compareSCpt);

   // select events only if the leading gen photon > 20 GeV and within ECAL acceptance!
   bool keep_me=false;
   double etmin = 20.;
   //for (int iCP=0; iCP<genParticle_energy.GetSize(); iCP++){
   for (auto CP_idx_pt_el : CP_idx_pt){
     int iCP=CP_idx_pt_el.first;
    
     if(caloParticle_id[iCP]==22 && abs(caloParticle_simEta[iCP])<3.0 && caloParticle_simPt[iCP]>etmin && (abs(caloParticle_simEta[iCP])>1.48 || abs(caloParticle_simEta[iCP])<1.44) ){
       h_phSimLead_et->Fill(caloParticle_simPt[iCP]);
       h_phSimLead_eta->Fill(caloParticle_simEta[iCP]);
       keep_me=true;
       if(doDebug) { std::cout << "Sim leading Ph=" << iCP << " pt=" << caloParticle_simPt[iCP] 
                             << " eta=" << caloParticle_simEta[iCP] << " phi=" << caloParticle_simPhi[iCP] << std::endl; }
       break;
     }
   }
   if(!keep_me) return kTRUE;

   // only events with at least one supercluster are kept
   if(superCluster_energy.GetSize()==0) return kTRUE;

   // *******
   // Good SuperCluster selection: 
   // *******
   //   highest pt supercluster that matches with a photon caloparticle:
   //   caloparticle must have DR < max_DR and DE/E < max_DE
   bool best_SC_idx = -1;
   double max_DE = 0.10;
   double max_DR = 0.02;;

   // first order the indices of the superclusters by pt, from highest pt to lowest pt
   std::vector< std::pair<int, double> > SC_idx_pt;

   for(int iSC=0; iSC<superCluster_energy.GetSize(); iSC++){
     SC_idx_pt.push_back(std::make_pair(iSC, superCluster_energy[iSC]/TMath::CosH(superCluster_eta[iSC])));
   }
   sort(SC_idx_pt.begin(),SC_idx_pt.end(),compareSCpt);

   if(doDebug) std::cout << "Sorted SuperClusters: " << std::endl;
   if(doDebug) for (auto i : SC_idx_pt) std::cout << " idx=" << i.first << " pt=" << i.second << " px=" << i.second*TMath::Sin(superCluster_phi[i.first]) << " py=" << i.second*TMath::Cos(superCluster_phi[i.first]) << " eta=" << superCluster_eta[i.first] << " energy=" <<  superCluster_energy[i.first] << std::endl;

   // then start from high pt and look for a good truth match, otherwise go to next supercluster
   int idx_goodSC = -1;
   int idx_cand_CP = -1;
   int count=0;

   auto SC_idx_pt_el = SC_idx_pt[0];
   int iSC = SC_idx_pt_el.first;
   double iSC_pt = SC_idx_pt_el.second;
   // only consider the leading photon for  
   //if(count==0) { // leading photon selection

   //if (iSC_pt < etmin) return kTRUE;
   //if (abs(superCluster_eta[iSC])>1.44 && abs(superCluster_eta[iSC])<1.48) return kTRUE;
   h_ptleading->Fill(superCluster_energy[iSC]/TMath::CosH(superCluster_eta[iSC]));

   //}

   if(doDebug) std::cout << "Supercluster="<< iSC << " pt=" << SC_idx_pt_el.second << " eta=" << superCluster_eta[iSC] 
                         << " phi=" << superCluster_phi[iSC] << " energy=" << superCluster_energy[iSC] << std::endl;
   if(doDebug) std::cout << "  Sorted CPs " << std::endl;
   int count_CP=0;

   // loop over ordered in pt caloparticles
   for(auto CP_idx_pt_el: CP_idx_pt){

     int iCP = CP_idx_pt_el.first;

     if(caloParticle_id[iCP]!=22) continue; // only consider photon caloparticles...
     if(caloParticle_simPt[iCP]<etmin) continue; // only consider high pt caloparticles
     if(caloParticle_simEta[iCP]>1.44 && caloParticle_simEta[iCP]<1.48) continue; // not falling in the crack region

     double iCP_Deta = abs(caloParticle_simEta[iCP]-superCluster_eta[iSC]);
     double iCP_Dphi = abs(TVector2::Phi_mpi_pi(caloParticle_simPhi[iCP]-superCluster_phi[iSC]));
     double iCP_DR = sqrt(iCP_Deta*iCP_Deta+iCP_Dphi*iCP_Dphi);
     double iCP_DE = abs(caloParticle_simEnergy[iCP]-superCluster_energy[iSC])/superCluster_energy[iSC];

     if (doDebug) {
       std::cout << "    idx=" << iCP << " pt=" << caloParticle_simPt[iCP] << " eta=" << caloParticle_simEta[iCP] 
                 << " phi=" << caloParticle_simPhi[iCP] << " energy=" << caloParticle_simEnergy[iCP]  
                 << " DR=" << iCP_DR << " DE=" << iCP_DE << std::endl; 
       count_CP++; 
       if(count_CP==3) break;
     }

     if(iCP_DR <= max_DR && iCP_DE <= max_DE){
       //std::cout << "DR=" << iCP_DR << " DE=" << iCP_DE << std::endl;
       idx_goodSC = iSC;
       idx_cand_CP = iCP;

       h_leadSCmatched_et->Fill(superCluster_energy[idx_goodSC]/TMath::CosH(superCluster_eta[idx_goodSC]));
       h_leadSCmatched_eta->Fill(superCluster_eta[idx_goodSC]);
       h_phSimMatched_et->Fill(caloParticle_simPt[idx_cand_CP]);
       h_phSimMatched_eta->Fill(caloParticle_simEta[idx_cand_CP]);

       if(superCluster_energy[idx_goodSC]/TMath::CosH(superCluster_eta[idx_goodSC])< 15) {
         h_goodSCptOverCPpt_below15->Fill(superCluster_energy[idx_goodSC]/TMath::CosH(superCluster_eta[idx_goodSC]) * 1/caloParticle_simPt[idx_cand_CP]);
         if(abs(superCluster_eta[idx_goodSC])<1.48)
           h_below15_EB_ietaiphi->Fill(superCluster_ieta[idx_goodSC], superCluster_iphi[idx_goodSC]);
         else
           h_below15_EE_ietaiphi->Fill(superCluster_ieta[idx_goodSC], superCluster_iphi[idx_goodSC]);
           

         if(superCluster_energy[idx_goodSC]/TMath::CosH(superCluster_eta[idx_goodSC]) * 1/caloParticle_simPt[idx_cand_CP]<0.8){
           if(abs(superCluster_eta[idx_goodSC])<1.48)
             h_below15_rOtbelow80_EB_ietaiphi->Fill(superCluster_ieta[idx_goodSC], superCluster_iphi[idx_goodSC]);
           else
             h_below15_rOtbelow80_EE_ietaiphi->Fill(superCluster_ieta[idx_goodSC], superCluster_iphi[idx_goodSC]); 
         }
       } else{ 
         h_goodSCptOverCPpt_above15->Fill(superCluster_energy[idx_goodSC]/TMath::CosH(superCluster_eta[idx_goodSC]) * 1/caloParticle_simPt[idx_cand_CP]);
       }
       break; // you found a candidate SP matched with a candidate CP
     }
     // else go on the next caloParticle
   } // loop over caloparticles 

   if(idx_goodSC==-1) {
     if(doDebug) std::cout << "Did not find a candidate Good SuperCluster in the event, going to next event" << std::endl;
     return kTRUE;
   } else {
     if(doDebug) std::cout << "Candidate SC=" << idx_goodSC << " eta=" << superCluster_eta[idx_goodSC] << " et=" << superCluster_energy[idx_goodSC]/TMath::CosH(superCluster_eta[idx_goodSC]) 
                           << " phi=" << superCluster_phi[idx_goodSC] << std::endl;
     if(doDebug) std::cout << "Candidate CP=" << idx_cand_CP << " eta=" << caloParticle_simEta[idx_cand_CP] << " et=" << caloParticle_simEnergy[idx_cand_CP] / TMath::CosH(caloParticle_simEta[idx_cand_CP])
                           << " phi=" << caloParticle_simPhi[idx_cand_CP] << std::endl;
   }

   // ************************
   // Now identify the SC from the jet... 
   // ************************
   // loop over the superclusters, ordered in pt, find the supercluster with DeltaPhi~3.14
   // find the highest in pt which has pi-tol<DeltaPhi<pi+tol
   // check if diff in pt wrt good is within given tolerance
   int idx_fakeSC = -1;
   double tol_Dphi = 0.3;
   //double tol_Detrel = 0.20;
   for(auto SC_idx_pt_el : SC_idx_pt){
     int iSC = SC_idx_pt_el.first;
     if(iSC==idx_goodSC) continue;
     double Dphi = TVector2::Phi_mpi_pi(superCluster_phi[iSC]-superCluster_phi[idx_goodSC]);
     double Detrel = 1 - (superCluster_energy[iSC]/TMath::CosH(superCluster_eta[iSC])) / (superCluster_energy[idx_goodSC]/TMath::CosH(superCluster_eta[idx_goodSC]));
     //if(abs(Detrel)<tol_Detrel){
     //if(abs(Dphi)<TMath::Pi()+tol_Dphi && abs(Dphi)>TMath::Pi()-tol_Dphi && abs(Detrel)<tol_Detrel){
     if(abs(Dphi)<TMath::Pi()+tol_Dphi && abs(Dphi)>TMath::Pi()-tol_Dphi){
       idx_fakeSC = iSC;
       break;
     }
   }
   if(idx_fakeSC==-1){
     if(doDebug) std::cout << "Did not find a candidate Fake SuperCluster in the envent, going to next event" << std::endl;
     return kTRUE;
   } else {
     if(doDebug) std::cout << "Candidate fake SC=" << idx_fakeSC << " eta=" << superCluster_eta[idx_fakeSC] << " et=" << superCluster_energy[idx_fakeSC] / TMath::CosH(superCluster_eta[idx_fakeSC])
                           << " Dphi=" << TVector2::Phi_mpi_pi(superCluster_phi[idx_fakeSC]-superCluster_phi[idx_goodSC]) << std::endl;
   
   }


   // At this point you have two two clusters of interest, just fill the histograms
   NcandEvents++;
   for (auto detBin: detBins){

     if (abs(superCluster_eta[idx_goodSC]) > detBin.second.first && abs(superCluster_eta[idx_goodSC]) < detBin.second.second){
       h_goodSC_R9[detBin.first]->Fill(superCluster_r9[idx_goodSC]);
       h_goodSC_SigmaIetaIeta[detBin.first]->Fill(superCluster_sigmaIetaIeta[idx_goodSC]);
       h_goodSC_SigmaIphiIphi[detBin.first]->Fill(superCluster_sigmaIphiIphi[idx_goodSC]);
       h_goodSC_fullR9[detBin.first]->Fill(superCluster_full5x5_r9[idx_goodSC]);
       h_goodSC_fullSigmaIetaIeta[detBin.first]->Fill(superCluster_full5x5_sigmaIetaIeta[idx_goodSC]);
       h_goodSC_fullSigmaIphiIphi[detBin.first]->Fill(superCluster_full5x5_sigmaIphiIphi[idx_goodSC]);
     }

     if (abs(superCluster_eta[idx_fakeSC]) > detBin.second.first && abs(superCluster_eta[idx_fakeSC]) < detBin.second.second){

       h_fakeSC_R9[detBin.first]->Fill(superCluster_r9[idx_fakeSC]);
       h_fakeSC_SigmaIetaIeta[detBin.first]->Fill(superCluster_sigmaIetaIeta[idx_fakeSC]);
       h_fakeSC_SigmaIphiIphi[detBin.first]->Fill(superCluster_sigmaIphiIphi[idx_fakeSC]);
       h_fakeSC_fullR9[detBin.first]->Fill(superCluster_full5x5_r9[idx_fakeSC]);
       h_fakeSC_fullSigmaIetaIeta[detBin.first]->Fill(superCluster_full5x5_sigmaIetaIeta[idx_fakeSC]);
       h_fakeSC_fullSigmaIphiIphi[detBin.first]->Fill(superCluster_full5x5_sigmaIphiIphi[idx_fakeSC]);
     }   
   }


   h_goodSC_eta->Fill(superCluster_eta[idx_goodSC]);
   h_goodSC_phi->Fill(superCluster_phi[idx_goodSC]);
   h_goodSC_et->Fill(superCluster_energy[idx_goodSC]/TMath::CosH(superCluster_eta[idx_goodSC]));
   h_goodSC_energy->Fill(superCluster_energy[idx_goodSC]);

   h_fakeSC_eta->Fill(superCluster_eta[idx_fakeSC]);
   h_fakeSC_phi->Fill(superCluster_phi[idx_fakeSC]);
   h_fakeSC_et->Fill(superCluster_energy[idx_fakeSC]/TMath::CosH(superCluster_eta[idx_fakeSC]));
   h_fakeSC_energy->Fill(superCluster_energy[idx_fakeSC]);

   h_goodFakeSC_deta->Fill(abs(superCluster_eta[idx_goodSC]-superCluster_eta[idx_fakeSC]));
   h_goodFakeSC_dphi->Fill(abs(TVector2::Phi_mpi_pi(superCluster_phi[idx_goodSC]-superCluster_phi[idx_fakeSC])));
   h_goodFakeSC_detrel->Fill(abs(1-superCluster_energy[idx_fakeSC]/TMath::CosH(superCluster_eta[idx_fakeSC])*TMath::CosH(superCluster_eta[idx_goodSC])/superCluster_energy[idx_goodSC]));
   h_goodFakeSC_denergyrel->Fill(abs(1-superCluster_energy[idx_fakeSC]/superCluster_energy[idx_goodSC]));

   return kTRUE;
}

void SCAnalyzer::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

   Info("SlaveTerminate", "Good-fake candidates / total : %i / %i ", NcandEvents, NtotEvents );

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


bool compareCPpt(const std::tuple<int, double, double, double>&i, const std::tuple<int, double, double, double>&j)
{
   return std::get<1>(i) > std::get<1>(j);
}

