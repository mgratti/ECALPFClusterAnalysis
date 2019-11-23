//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul  8 11:35:13 2019 by ROOT version 6.14/09
// from TTree caloTree/caloTree
// found on file: RecoSimDumper.root
//////////////////////////////////////////////////////////

#ifndef PFClusterAnalyzer_h
#define PFClusterAnalyzer_h


// ROOT libraries
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TH1F.h>
#include <TH2F.h>

// C++ libraries 
#include <vector>
#include "vector"
#include <map>
#include <cmath>

struct MatchingMap{
   unsigned int CaloIndex;
   unsigned int PFIndex;
   float Score;
};



class PFClusterAnalyzer : public TSelector {
   public :
      TTreeReader     fReader;  //!the tree reader
      TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

      // Readers
      TTreeReaderValue<Long64_t> eventId = {fReader, "eventId"};
      TTreeReaderValue<Int_t> lumiId = {fReader, "lumiId"};
      TTreeReaderValue<Int_t> runId = {fReader, "runId"};
      //TTreeReaderValue<Float_t> pu_nTrueInt = {fReader, "pu_nTrueInt"};
      //TTreeReaderValue<Int_t> pu_nPU = {fReader, "pu_nPU"};
      //TTreeReaderValue<Double_t> rho = {fReader, "rho"};
      TTreeReaderArray<int> genParticle_id = {fReader, "genParticle_id"};
      TTreeReaderArray<float> genParticle_energy = {fReader, "genParticle_energy"};
      TTreeReaderArray<float> genParticle_pt = {fReader, "genParticle_pt"};
      TTreeReaderArray<float> genParticle_eta = {fReader, "genParticle_eta"};
      TTreeReaderArray<float> genParticle_phi = {fReader, "genParticle_phi"};
      TTreeReaderArray<vector<float>> simHit_energy = {fReader, "simHit_energy"};
      TTreeReaderArray<float> caloParticle_genEnergy = {fReader, "caloParticle_genEnergy"};
      TTreeReaderArray<float> caloParticle_simEnergy = {fReader, "caloParticle_simEnergy"};
      TTreeReaderArray<float> caloParticle_genPt = {fReader, "caloParticle_genPt"};
      TTreeReaderArray<float> caloParticle_simPt = {fReader, "caloParticle_simPt"};
      TTreeReaderArray<float> caloParticle_genEta = {fReader, "caloParticle_genEta"};
      TTreeReaderArray<float> caloParticle_simEta = {fReader, "caloParticle_simEta"};
      TTreeReaderArray<float> caloParticle_genPhi = {fReader, "caloParticle_genPhi"};
      TTreeReaderArray<float> caloParticle_simPhi = {fReader, "caloParticle_simPhi"};
      TTreeReaderArray<int> caloParticle_simIeta = {fReader, "caloParticle_simIeta"};
      TTreeReaderArray<int> caloParticle_simIphi = {fReader, "caloParticle_simIphi"};
      TTreeReaderArray<int> caloParticle_simIz = {fReader, "caloParticle_simIz"};
      TTreeReaderArray<vector<int>> caloParticle_pfCluster_dR_simScore_MatchedIndex = {fReader, "caloParticle_pfCluster_dR_simScore_MatchedIndex"};
      TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_fraction_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_fraction_MatchedIndex"};
      TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_fraction_min1_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_fraction_min1_MatchedIndex"};
      TTreeReaderArray<vector<double>> pfCluster_sim_fraction_min1 = {fReader, "pfCluster_sim_fraction_min1"};
      TTreeReaderArray<vector<double>> pfCluster_dR_simScore = {fReader, "pfCluster_dR_simScore"};
      TTreeReaderArray<int> pfCluster_sim_fraction_min1_MatchedIndex = {fReader, "pfCluster_sim_fraction_min1_MatchedIndex"};
      TTreeReaderArray<int> pfCluster_dR_simScore_MatchedIndex = {fReader, "pfCluster_dR_simScore_MatchedIndex"};
      TTreeReaderArray<vector<float>> pfClusterHit_energy = {fReader, "pfClusterHit_energy"};
      TTreeReaderArray<vector<float>> pfClusterHit_eta = {fReader, "pfClusterHit_eta"};
      TTreeReaderArray<vector<float>> pfClusterHit_phi = {fReader, "pfClusterHit_phi"};
      TTreeReaderArray<vector<int>> pfClusterHit_ieta = {fReader, "pfClusterHit_ieta"};
      TTreeReaderArray<vector<int>> pfClusterHit_iphi = {fReader, "pfClusterHit_iphi"};
      TTreeReaderArray<vector<int>> pfClusterHit_iz = {fReader, "pfClusterHit_iz"};
      //TTreeReaderArray<vector<float>> pfClusterHit_noCaloPart_energy = {fReader, "pfClusterHit_noCaloPart_energy"};
      //TTreeReaderArray<vector<float>> pfClusterHit_noCaloPart_eta = {fReader, "pfClusterHit_noCaloPart_eta"};
      //TTreeReaderArray<vector<float>> pfClusterHit_noCaloPart_phi = {fReader, "pfClusterHit_noCaloPart_phi"};
      //TTreeReaderArray<vector<int>> pfClusterHit_noCaloPart_ieta = {fReader, "pfClusterHit_noCaloPart_ieta"};
      //TTreeReaderArray<vector<int>> pfClusterHit_noCaloPart_iphi = {fReader, "pfClusterHit_noCaloPart_iphi"};
      //TTreeReaderArray<vector<int>> pfClusterHit_noCaloPart_iz = {fReader, "pfClusterHit_noCaloPart_iz"};
      TTreeReaderArray<float> pfCluster_energy = {fReader, "pfCluster_energy"};
      TTreeReaderArray<float> pfCluster_eta = {fReader, "pfCluster_eta"};
      TTreeReaderArray<float> pfCluster_phi = {fReader, "pfCluster_phi"};
      TTreeReaderArray<int> pfCluster_ieta = {fReader, "pfCluster_ieta"};
      TTreeReaderArray<int> pfCluster_iphi = {fReader, "pfCluster_iphi"};
      TTreeReaderArray<int> pfCluster_iz = {fReader, "pfCluster_iz"};
      //TTreeReaderArray<vector<map<int,float> >> superClusterHit_energy = {fReader, "superClusterHit_energy"};
      //TTreeReaderArray<vector<float>> superClusterHit_noCaloPart_energy = {fReader, "superClusterHit_noCaloPart_energy"};
      //TTreeReaderArray<vector<float>> superClusterHit_noCaloPart_eta = {fReader, "superClusterHit_noCaloPart_eta"};
      //TTreeReaderArray<vector<float>> superClusterHit_noCaloPart_phi = {fReader, "superClusterHit_noCaloPart_phi"};
      //TTreeReaderArray<vector<int>> superClusterHit_noCaloPart_ieta = {fReader, "superClusterHit_noCaloPart_ieta"};
      //TTreeReaderArray<vector<int>> superClusterHit_noCaloPart_iphi = {fReader, "superClusterHit_noCaloPart_iphi"};
      //TTreeReaderArray<vector<int>> superClusterHit_noCaloPart_iz = {fReader, "superClusterHit_noCaloPart_iz"};
      //TTreeReaderArray<float> superCluster_energy = {fReader, "superCluster_energy"};
      //TTreeReaderArray<float> superCluster_eta = {fReader, "superCluster_eta"};
      //TTreeReaderArray<float> superCluster_phi = {fReader, "superCluster_phi"};
      //TTreeReaderArray<int> superCluster_ieta = {fReader, "superCluster_ieta"};
      //TTreeReaderArray<int> superCluster_iphi = {fReader, "superCluster_iphi"};
      //TTreeReaderArray<int> superCluster_iz = {fReader, "superCluster_iz"};
      //TTreeReaderArray<float> superCluster_r9 = {fReader, "superCluster_r9"};
      //TTreeReaderArray<float> superCluster_sigmaIetaIeta = {fReader, "superCluster_sigmaIetaIeta"};
      //TTreeReaderArray<float> superCluster_sigmaIetaIphi = {fReader, "superCluster_sigmaIetaIphi"};
      //TTreeReaderArray<float> superCluster_sigmaIphiIphi = {fReader, "superCluster_sigmaIphiIphi"};
      //TTreeReaderArray<float> superCluster_full5x5_r9 = {fReader, "superCluster_full5x5_r9"};
      //TTreeReaderArray<float> superCluster_full5x5_sigmaIetaIeta = {fReader, "superCluster_full5x5_sigmaIetaIeta"};
      //TTreeReaderArray<float> superCluster_full5x5_sigmaIetaIphi = {fReader, "superCluster_full5x5_sigmaIetaIphi"};
      //TTreeReaderArray<float> superCluster_full5x5_sigmaIphiIphi = {fReader, "superCluster_full5x5_sigmaIphiIphi"};
 
      // non reader members 
      // -- non root members
      float min_pfClusterHit_energy=0.08; // 80 MeV
      int N_perEvent_plots = 100;

      // -- root members
      TFile *fout;

      //Flags
      //Turn to true this flag in case you want to save only one PFCluster per caloParticle
      Bool_t flag_keepOnlyOnePFCluster;

      //matching methods
      Bool_t flag_doMatching_numberOfHits;
      Bool_t flag_doMatching_score;
      Bool_t flag_doMatching_deltaR;


      //only produce EB (EE) histograms with EB (EE) inputfiles
      Bool_t flag_doEB;
      Bool_t flag_doEE;

      //needed to define the different eta and ET bins
      std::vector<TString> Et_keys;
      std::map<TString, std::pair<Float_t,Float_t>> Et_edges;
      std::vector<TString> Eta_keys;
      std::map<TString, std::pair<Float_t,Float_t>> Eta_edges;

      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_eOverEtrue_EtaEtBinned;
      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_eOverEtrue_EtaEnBinned;

      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEnBinned;
      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_eOverEtrue_simEnergy_EtaEtBinned;

      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_size_EtaEtBinned_forEfficiency;
      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_size_EtaEnBinned_forEfficiency;

      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_size_EtaEnBinned_simEnergy_forEfficiency;
      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_size_EtaEtBinned_simEnergy_forEfficiency;

      std::map<TString, std::map<TString, TH1F*>> h_caloParticle_size_EtaEtBinned;
      std::map<TString, std::map<TString, TH1F*>> h_caloParticle_size_EtaEtBinned_simEnergy;
      std::map<TString, std::map<TString, TH1F*>> h_caloParticle_size_EtaEnBinned;
      std::map<TString, std::map<TString, TH1F*>> h_caloParticle_size_EtaEnBinned_simEnergy;

      std::map<TString, std::map<TString, TH1F*>> h_PFclusters_caloMatched_fakeRate_EtaEnBinned;
 

      // PFClusters 
      TH1F* h_PFClusters_caloMatched_size_EB;
      TH1F* h_PFClusters_caloMatched_nRecHit_EB;
      TH1F* h_PFClusters_caloMatched_energy_EB;
      TH1F* h_PFClusters_caloMatched_et_EB;
      TH1F* h_PFClusters_caloMatched_eta_EB;
      TH1F* h_PFClusters_caloMatched_phi_EB;
      //TH1F* h_PFClusters_caloMatched_fakeRate_EB;
      TH1F* h_PFClusters_caloMatched_eOverEtrue_EB;
      TH1F* h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB; 
      TH2F* h_PFClusters_caloMatched_nRecHit_vs_energy_EB;
      TH2F* h_PFClusters_caloMatched_nPFClusters_vs_energy_EB;
      TH2F* h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EB;
      TH2F* h_PFClusters_caloMatched_nPFClusters_vs_eta_EB;
      TH2F* h_PFClusters_caloMatched_EoverEtrue_iEta_iPhi_EB;
      TH2F* h_PFClusters_caloMatched_et_iEta_iPhi_EB;
      TH1F* h_PFClusters_caloMatched_deltaR_EB;
 
      TH1F* h_PFClusters_caloMatched_size_EE;
      TH1F* h_PFClusters_caloMatched_nRecHit_EE;
      TH1F* h_PFClusters_caloMatched_energy_EE;
      TH1F* h_PFClusters_caloMatched_et_EE;
      TH1F* h_PFClusters_caloMatched_eta_EE;
      TH1F* h_PFClusters_caloMatched_phi_EE;
      //TH1F* h_PFClusters_caloMatched_fakeRate_EE;
      //TH1F* h_PFClusters_caloMatched_fakeRate2_EE;
      TH1F* h_PFClusters_caloMatched_eOverEtrue_EE;
      TH1F* h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE; 
      TH2F* h_PFClusters_caloMatched_nRecHit_vs_energy_EE;
      TH2F* h_PFClusters_caloMatched_nPFClusters_vs_energy_EE;
      TH2F* h_PFClusters_caloMatched_nPFClusters_vs_caloEnergy_EE;
      TH2F* h_PFClusters_caloMatched_nPFClusters_vs_eta_EE;


      std::vector<TH2F*> h_PFClusterHit_EB_ietaiphi;
 
      TH1F* h_PFClusters_caloMatched_EEM_eta;
      TH1F* h_PFClusters_caloMatched_EEM_size;
      TH1F* h_PFClusters_caloMatched_EEM_energy;
      TH1F* h_PFClusters_caloMatched_EEM_et;
      TH1F* h_PFClusters_caloMatched_EEM_phi;
      TH1F* h_PFClusters_caloMatched_EEM_eOverEtrue;

      TH1F* h_PFClusters_caloMatched_EBM_eta;
      TH1F* h_PFClusters_caloMatched_EBM_size;
      TH1F* h_PFClusters_caloMatched_EBM_energy;
      TH1F* h_PFClusters_caloMatched_EBM_et;
      TH1F* h_PFClusters_caloMatched_EBM_phi;
      TH1F* h_PFClusters_caloMatched_EBM_eOverEtrue;

      TH1F* h_PFClusters_caloMatched_EBP_eta;
      TH1F* h_PFClusters_caloMatched_EBP_size;
      TH1F* h_PFClusters_caloMatched_EBP_energy;
      TH1F* h_PFClusters_caloMatched_EBP_et;
      TH1F* h_PFClusters_caloMatched_EBP_phi;
      TH1F* h_PFClusters_caloMatched_EBP_eOverEtrue;

      TH1F* h_PFClusters_caloMatched_EEP_eta;
      TH1F* h_PFClusters_caloMatched_EEP_size;
      TH1F* h_PFClusters_caloMatched_EEP_energy;
      TH1F* h_PFClusters_caloMatched_EEP_et;
      TH1F* h_PFClusters_caloMatched_EEP_phi;
      TH1F* h_PFClusters_caloMatched_EEP_eOverEtrue;

      // calo particles
      TH1F* h_caloParticle_size_EB;
      TH1F* h_caloParticle_genEnergy_EB;
      TH1F* h_caloParticle_simEnergy_EB;
      TH1F* h_caloParticle_simEt_EB;
      TH1F* h_caloParticle_genEta_EB;
      TH1F* h_caloParticle_genPhi_EB;
      TH1F* h_caloParticle_simEta_EB;
      TH2F* h_caloParticle_genPhi_vs_eta_ifNoPFCluster_EB;
      TH1F* h_caloParticle_simPhi_EB;
      TH2F* h_caloParticle_simPhi_vs_eta_ifNoPFCluster_EB;
      
      TH1F* h_caloParticle_size_EE;
      TH1F* h_caloParticle_genEnergy_EE;
      TH1F* h_caloParticle_simEnergy_EE;
      TH1F* h_caloParticle_simEt_EE;
      TH1F* h_caloParticle_genEta_EE;
      TH1F* h_caloParticle_genPhi_EE;
      TH1F* h_caloParticle_simEta_EE;
      TH1F* h_caloParticle_simPhi_EE;



      TH1F* h_caloParticle_EEM_size;
      TH1F* h_caloParticle_EEM_energy;
      TH1F* h_caloParticle_EEM_simEnergy;
      TH1F* h_caloParticle_EEM_simEt;
      TH1F* h_caloParticle_EEM_eta;
      TH1F* h_caloParticle_EEM_phi;

      TH1F* h_caloParticle_EBM_size;
      TH1F* h_caloParticle_EBM_energy;
      TH1F* h_caloParticle_EBM_simEnergy;
      TH1F* h_caloParticle_EBM_simEt;
      TH1F* h_caloParticle_EBM_eta;
      TH1F* h_caloParticle_EBM_phi;

      TH1F* h_caloParticle_EBP_size;
      TH1F* h_caloParticle_EBP_energy;
      TH1F* h_caloParticle_EBP_simEnergy;
      TH1F* h_caloParticle_EBP_simEt;
      TH1F* h_caloParticle_EBP_eta;
      TH1F* h_caloParticle_EBP_phi;

      TH1F* h_caloParticle_EEP_size;
      TH1F* h_caloParticle_EEP_energy;
      TH1F* h_caloParticle_EEP_simEnergy;
      TH1F* h_caloParticle_EEP_simEt;
      TH1F* h_caloParticle_EEP_eta;
      TH1F* h_caloParticle_EEP_phi;



      // per Event maps
      std::vector<TH2F*> h_caloParticle_EB_ietaiphi;
      std::vector<TH2F*> h_caloParticle_EEP_ixiy;
      std::vector<TH2F*> h_caloParticle_EEM_ixiy;
      std::vector<TH2F*> h_PFClusterHits_caloMatched_EB_ietaiphi;
      std::vector<TH2F*> h_PFClusterHits_caloMatched_EEP_ixiy;
      std::vector<TH2F*> h_PFClusterHits_caloMatched_EEM_ixiy;
      std::vector<TH2F*> h_PFClusterHits_all_EB_ietaiphi;
      std::vector<TH2F*> h_PFClusterHits_all_EEP_ixiy;
      std::vector<TH2F*> h_PFClusterHits_all_EEM_ixiy;

      // functions
      PFClusterAnalyzer(TTree * /*tree*/ =0) { }
      virtual ~PFClusterAnalyzer() { }
      virtual Int_t   Version() const { return 2; }
      virtual void    Begin(TTree *tree);
      virtual void    SlaveBegin(TTree *tree);
      virtual void    Init(TTree *tree);
      virtual Bool_t  Notify();
      virtual Bool_t  Process(Long64_t entry);
      virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
      virtual void    SetOption(const char *option) { fOption = option; }
      virtual void    SetObject(TObject *obj) { fObject = obj; }
      virtual void    SetInputList(TList *input) { fInput = input; }
      virtual TList  *GetOutputList() const { return fOutput; }
      virtual void    SlaveTerminate();
      virtual void    Terminate();
      
      
      vector<MatchingMap> getMapCaloParticleCluster(const TTreeReaderArray<float>& pfCluster_energy, const TTreeReaderArray<float>& caloParticle_genEnergy, const TTreeReaderArray<float>& caloParticle_simEnergy, const TTreeReaderArray<vector<float>>& simHit_energy, const TTreeReaderArray<vector<map<int,float>>>& pfClusterHit_energy);


      vector<int> getMatchedIndices_score(const vector<MatchingMap>& matchingMap, unsigned int icP);
      vector<int> getMatchedIndices_numberOfHits(const TTreeReaderArray<vector<map<int,float>>>& pfClusterHit_energy, unsigned int icP);
      vector<int> getMatchedIndices_deltaR(const TTreeReaderArray<float>& pfCluster_energy, const TTreeReaderArray<float>& pfCluster_eta, const TTreeReaderArray<float>& pfCluster_phi, unsigned int icP, float deltaRThreshold);
      
      vector<int> getMatchedIndices(vector<int> input_vector, unsigned int icP, const TTreeReaderArray<vector<float>>& score);


      
      ClassDef(PFClusterAnalyzer,0);

};

#endif

#ifdef PFClusterAnalyzer_cxx
void PFClusterAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t PFClusterAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef PFClusterAnalyzer_cxx
