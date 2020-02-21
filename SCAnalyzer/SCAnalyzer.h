//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 20 16:51:30 2020 by ROOT version 6.14/09
// from TTree caloTree/caloTree
// found on file: gjetEM_test_y2021_G1_v1_t33_n-1.root
//////////////////////////////////////////////////////////

#ifndef SCAnalyzer_h
#define SCAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>



class SCAnalyzer : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Long64_t> eventId = {fReader, "eventId"};
   TTreeReaderValue<Int_t> lumiId = {fReader, "lumiId"};
   TTreeReaderValue<Int_t> runId = {fReader, "runId"};
//   TTreeReaderValue<Int_t> nVtx = {fReader, "nVtx"};
   TTreeReaderArray<int> genParticle_id = {fReader, "genParticle_id"};
   TTreeReaderArray<float> genParticle_energy = {fReader, "genParticle_energy"};
   TTreeReaderArray<float> genParticle_pt = {fReader, "genParticle_pt"};
   TTreeReaderArray<float> genParticle_eta = {fReader, "genParticle_eta"};
   TTreeReaderArray<float> genParticle_phi = {fReader, "genParticle_phi"};
//   TTreeReaderArray<vector<int>> genParticle_pfCluster_dR_genScore_MatchedIndex = {fReader, "genParticle_pfCluster_dR_genScore_MatchedIndex"};
//   TTreeReaderArray<vector<int>> genParticle_superCluster_dR_genScore_MatchedIndex = {fReader, "genParticle_superCluster_dR_genScore_MatchedIndex"};
   TTreeReaderArray<int> caloParticle_id = {fReader, "caloParticle_id"};
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
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_dR_simScore_MatchedIndex = {fReader, "caloParticle_pfCluster_dR_simScore_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_n_shared_xtals_MatchedIndex = {fReader, "caloParticle_pfCluster_n_shared_xtals_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_fraction_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_fraction_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_fraction_min1_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_fraction_min1_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_fraction_min3_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_fraction_min3_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_rechit_diff_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_rechit_diff_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_rechit_fraction_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_rechit_fraction_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_global_sim_rechit_fraction_MatchedIndex = {fReader, "caloParticle_pfCluster_global_sim_rechit_fraction_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_hgcal_caloToCluster_MatchedIndex = {fReader, "caloParticle_pfCluster_hgcal_caloToCluster_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_pfCluster_hgcal_clusterToCalo_MatchedIndex = {fReader, "caloParticle_pfCluster_hgcal_clusterToCalo_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_dR_simScore_MatchedIndex = {fReader, "caloParticle_superCluster_dR_simScore_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_n_shared_xtals_MatchedIndex = {fReader, "caloParticle_superCluster_n_shared_xtals_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_sim_fraction_MatchedIndex = {fReader, "caloParticle_superCluster_sim_fraction_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_sim_fraction_min1_MatchedIndex = {fReader, "caloParticle_superCluster_sim_fraction_min1_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_sim_fraction_min3_MatchedIndex = {fReader, "caloParticle_superCluster_sim_fraction_min3_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_sim_rechit_diff_MatchedIndex = {fReader, "caloParticle_superCluster_sim_rechit_diff_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_sim_rechit_fraction_MatchedIndex = {fReader, "caloParticle_superCluster_sim_rechit_fraction_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_global_sim_rechit_fraction_MatchedIndex = {fReader, "caloParticle_superCluster_global_sim_rechit_fraction_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_hgcal_caloToCluster_MatchedIndex = {fReader, "caloParticle_superCluster_hgcal_caloToCluster_MatchedIndex"};
//   TTreeReaderArray<vector<int>> caloParticle_superCluster_hgcal_clusterToCalo_MatchedIndex = {fReader, "caloParticle_superCluster_hgcal_clusterToCalo_MatchedIndex"};
//   TTreeReaderArray<vector<float>> simHit_energy = {fReader, "simHit_energy"};
//   TTreeReaderArray<vector<float>> simHit_eta = {fReader, "simHit_eta"};
//   TTreeReaderArray<vector<float>> simHit_phi = {fReader, "simHit_phi"};
//   TTreeReaderArray<vector<int>> simHit_ieta = {fReader, "simHit_ieta"};
//   TTreeReaderArray<vector<int>> simHit_iphi = {fReader, "simHit_iphi"};
//   TTreeReaderArray<vector<int>> simHit_iz = {fReader, "simHit_iz"};
//   TTreeReaderArray<float> pfCluster_energy = {fReader, "pfCluster_energy"};
//   TTreeReaderArray<float> pfCluster_eta = {fReader, "pfCluster_eta"};
//   TTreeReaderArray<float> pfCluster_phi = {fReader, "pfCluster_phi"};
//   TTreeReaderArray<int> pfCluster_ieta = {fReader, "pfCluster_ieta"};
//   TTreeReaderArray<int> pfCluster_iphi = {fReader, "pfCluster_iphi"};
//   TTreeReaderArray<int> pfCluster_iz = {fReader, "pfCluster_iz"};
//   TTreeReaderArray<vector<int>> pfCluster_superClustersIndex = {fReader, "pfCluster_superClustersIndex"};
//   TTreeReaderArray<int> pfCluster_dR_genScore_MatchedIndex = {fReader, "pfCluster_dR_genScore_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_dR_simScore_MatchedIndex = {fReader, "pfCluster_dR_simScore_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_n_shared_xtals_MatchedIndex = {fReader, "pfCluster_n_shared_xtals_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_sim_fraction_MatchedIndex = {fReader, "pfCluster_sim_fraction_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_sim_fraction_min1_MatchedIndex = {fReader, "pfCluster_sim_fraction_min1_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_sim_fraction_min3_MatchedIndex = {fReader, "pfCluster_sim_fraction_min3_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_sim_rechit_diff_MatchedIndex = {fReader, "pfCluster_sim_rechit_diff_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_sim_rechit_fraction_MatchedIndex = {fReader, "pfCluster_sim_rechit_fraction_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_global_sim_rechit_fraction_MatchedIndex = {fReader, "pfCluster_global_sim_rechit_fraction_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_hgcal_caloToCluster_MatchedIndex = {fReader, "pfCluster_hgcal_caloToCluster_MatchedIndex"};
//   TTreeReaderArray<int> pfCluster_hgcal_clusterToCalo_MatchedIndex = {fReader, "pfCluster_hgcal_clusterToCalo_MatchedIndex"};
//   TTreeReaderArray<vector<double>> pfCluster_dR_genScore = {fReader, "pfCluster_dR_genScore"};
//   TTreeReaderArray<vector<double>> pfCluster_dR_simScore = {fReader, "pfCluster_dR_simScore"};
//   TTreeReaderArray<vector<int>> pfCluster_n_shared_xtals = {fReader, "pfCluster_n_shared_xtals"};
//   TTreeReaderArray<vector<double>> pfCluster_sim_fraction = {fReader, "pfCluster_sim_fraction"};
//   TTreeReaderArray<vector<double>> pfCluster_sim_fraction_min1 = {fReader, "pfCluster_sim_fraction_min1"};
//   TTreeReaderArray<vector<double>> pfCluster_sim_fraction_min3 = {fReader, "pfCluster_sim_fraction_min3"};
//   TTreeReaderArray<vector<double>> pfCluster_sim_rechit_diff = {fReader, "pfCluster_sim_rechit_diff"};
//   TTreeReaderArray<vector<double>> pfCluster_sim_rechit_fraction = {fReader, "pfCluster_sim_rechit_fraction"};
//   TTreeReaderArray<vector<double>> pfCluster_global_sim_rechit_fraction = {fReader, "pfCluster_global_sim_rechit_fraction"};
//   TTreeReaderArray<vector<double>> pfCluster_hgcal_caloToCluster = {fReader, "pfCluster_hgcal_caloToCluster"};
//   TTreeReaderArray<vector<double>> pfCluster_hgcal_clusterToCalo = {fReader, "pfCluster_hgcal_clusterToCalo"};
   TTreeReaderArray<float> superCluster_energy = {fReader, "superCluster_energy"};
   TTreeReaderArray<float> superCluster_eta = {fReader, "superCluster_eta"};
   TTreeReaderArray<float> superCluster_phi = {fReader, "superCluster_phi"};
   TTreeReaderArray<float> superCluster_etaWidth = {fReader, "superCluster_etaWidth"};
   TTreeReaderArray<float> superCluster_phiWidth = {fReader, "superCluster_phiWidth"};
   TTreeReaderArray<float> superCluster_R = {fReader, "superCluster_R"};
   TTreeReaderArray<int> superCluster_ieta = {fReader, "superCluster_ieta"};
   TTreeReaderArray<int> superCluster_iphi = {fReader, "superCluster_iphi"};
   TTreeReaderArray<int> superCluster_iz = {fReader, "superCluster_iz"};
   TTreeReaderArray<int> superCluster_seedIndex = {fReader, "superCluster_seedIndex"};
   TTreeReaderArray<vector<int>> superCluster_pfClustersIndex = {fReader, "superCluster_pfClustersIndex"};
   TTreeReaderArray<vector<float>> psCluster_energy = {fReader, "psCluster_energy"};
   TTreeReaderArray<vector<float>> psCluster_eta = {fReader, "psCluster_eta"};
   TTreeReaderArray<vector<float>> psCluster_phi = {fReader, "psCluster_phi"};
//   TTreeReaderArray<int> superCluster_dR_genScore_MatchedIndex = {fReader, "superCluster_dR_genScore_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_dR_simScore_MatchedIndex = {fReader, "superCluster_dR_simScore_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_n_shared_xtals_MatchedIndex = {fReader, "superCluster_n_shared_xtals_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_sim_fraction_MatchedIndex = {fReader, "superCluster_sim_fraction_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_sim_fraction_min1_MatchedIndex = {fReader, "superCluster_sim_fraction_min1_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_sim_fraction_min3_MatchedIndex = {fReader, "superCluster_sim_fraction_min3_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_sim_rechit_diff_MatchedIndex = {fReader, "superCluster_sim_rechit_diff_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_sim_rechit_fraction_MatchedIndex = {fReader, "superCluster_sim_rechit_fraction_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_global_sim_rechit_fraction_MatchedIndex = {fReader, "superCluster_global_sim_rechit_fraction_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_hgcal_caloToCluster_MatchedIndex = {fReader, "superCluster_hgcal_caloToCluster_MatchedIndex"};
//   TTreeReaderArray<int> superCluster_hgcal_clusterToCalo_MatchedIndex = {fReader, "superCluster_hgcal_clusterToCalo_MatchedIndex"};
   TTreeReaderArray<vector<double>> superCluster_dR_genScore = {fReader, "superCluster_dR_genScore"};
   TTreeReaderArray<vector<double>> superCluster_dR_simScore = {fReader, "superCluster_dR_simScore"};
//   TTreeReaderArray<vector<int>> superCluster_n_shared_xtals = {fReader, "superCluster_n_shared_xtals"};
//   TTreeReaderArray<vector<double>> superCluster_sim_fraction = {fReader, "superCluster_sim_fraction"};
//   TTreeReaderArray<vector<double>> superCluster_sim_fraction_min1 = {fReader, "superCluster_sim_fraction_min1"};
//   TTreeReaderArray<vector<double>> superCluster_sim_fraction_min3 = {fReader, "superCluster_sim_fraction_min3"};
//   TTreeReaderArray<vector<double>> superCluster_sim_rechit_diff = {fReader, "superCluster_sim_rechit_diff"};
//   TTreeReaderArray<vector<double>> superCluster_sim_rechit_fraction = {fReader, "superCluster_sim_rechit_fraction"};
//   TTreeReaderArray<vector<double>> superCluster_global_sim_rechit_fraction = {fReader, "superCluster_global_sim_rechit_fraction"};
//   TTreeReaderArray<vector<double>> superCluster_hgcal_caloToCluster = {fReader, "superCluster_hgcal_caloToCluster"};
//   TTreeReaderArray<vector<double>> superCluster_hgcal_clusterToCalo = {fReader, "superCluster_hgcal_clusterToCalo"};
   TTreeReaderArray<float> superCluster_r9 = {fReader, "superCluster_r9"};
   TTreeReaderArray<float> superCluster_sigmaIetaIeta = {fReader, "superCluster_sigmaIetaIeta"};
   TTreeReaderArray<float> superCluster_sigmaIetaIphi = {fReader, "superCluster_sigmaIetaIphi"};
   TTreeReaderArray<float> superCluster_sigmaIphiIphi = {fReader, "superCluster_sigmaIphiIphi"};
   TTreeReaderArray<float> superCluster_full5x5_r9 = {fReader, "superCluster_full5x5_r9"};
   TTreeReaderArray<float> superCluster_full5x5_sigmaIetaIeta = {fReader, "superCluster_full5x5_sigmaIetaIeta"};
   TTreeReaderArray<float> superCluster_full5x5_sigmaIetaIphi = {fReader, "superCluster_full5x5_sigmaIetaIphi"};
   TTreeReaderArray<float> superCluster_full5x5_sigmaIphiIphi = {fReader, "superCluster_full5x5_sigmaIphiIphi"};

   // histograms
   TH1F *h_goodSC_R9;
   TH1F *h_goodSC_SigmaIetaIeta;
   TH1F *h_goodSC_SigmaIphiIphi;
   TH1F *h_goodSC_fullR9;
   TH1F *h_goodSC_fullSigmaIetaIeta;
   TH1F *h_goodSC_fullSigmaIphiIphi;
   TH1F *h_fakeSC_R9;
   TH1F *h_fakeSC_SigmaIetaIeta;
   TH1F *h_fakeSC_SigmaIphiIphi;
   TH1F *h_fakeSC_fullR9;
   TH1F *h_fakeSC_fullSigmaIetaIeta;
   TH1F *h_fakeSC_fullSigmaIphiIphi;

   // other algorithm variables
   TFile *fout;
   bool doDebug=false;
   SCAnalyzer(TTree * /*tree*/ =0) { }
   virtual ~SCAnalyzer() { }
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

   // custom functions
   //bool compareSCpt(const std::pair<int, double>&, const std::pair<int, double>&);

   ClassDef(SCAnalyzer,0);

};

#endif

#ifdef SCAnalyzer_cxx
void SCAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t SCAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef SCAnalyzer_cxx
