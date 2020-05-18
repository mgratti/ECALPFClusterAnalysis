//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan 20 10:28:40 2020 by ROOT version 6.14/09
// from TTree caloTree/caloTree
// Analysis of single events, saves per event histograms
// for PFClustering studies
// TODO: check eventId instead of entry?
//////////////////////////////////////////////////////////

#ifndef SingleEventPFClusterAnalyzer_h
#define SingleEventPFClusterAnalyzer_h

// ROOT Libraries
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
#include <map>
#include <cmath>



class SingleEventPFClusterAnalyzer : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Long64_t> eventId = {fReader, "eventId"};
   TTreeReaderValue<Int_t> lumiId = {fReader, "lumiId"};
   TTreeReaderValue<Int_t> runId = {fReader, "runId"};
   TTreeReaderValue<Int_t> nVtx = {fReader, "nVtx"};
   TTreeReaderArray<int> genParticle_id = {fReader, "genParticle_id"};
   TTreeReaderArray<float> genParticle_energy = {fReader, "genParticle_energy"};
   TTreeReaderArray<float> genParticle_pt = {fReader, "genParticle_pt"};
   TTreeReaderArray<float> genParticle_eta = {fReader, "genParticle_eta"};
   TTreeReaderArray<float> genParticle_phi = {fReader, "genParticle_phi"};
   TTreeReaderArray<vector<int>> genParticle_pfCluster_dR_genScore_MatchedIndex = {fReader, "genParticle_pfCluster_dR_genScore_MatchedIndex"};
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
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_dR_simScore_MatchedIndex = {fReader, "caloParticle_pfCluster_dR_simScore_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_n_shared_xtals_MatchedIndex = {fReader, "caloParticle_pfCluster_n_shared_xtals_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_fraction_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_fraction_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_fraction_min1_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_fraction_min1_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_fraction_min3_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_fraction_min3_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_rechit_diff_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_rechit_diff_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_sim_rechit_fraction_MatchedIndex = {fReader, "caloParticle_pfCluster_sim_rechit_fraction_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_global_sim_rechit_fraction_MatchedIndex = {fReader, "caloParticle_pfCluster_global_sim_rechit_fraction_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_hgcal_caloToCluster_MatchedIndex = {fReader, "caloParticle_pfCluster_hgcal_caloToCluster_MatchedIndex"};
   TTreeReaderArray<vector<int>> caloParticle_pfCluster_hgcal_clusterToCalo_MatchedIndex = {fReader, "caloParticle_pfCluster_hgcal_clusterToCalo_MatchedIndex"};
   TTreeReaderArray<vector<float>> simHit_energy = {fReader, "simHit_energy"};
   TTreeReaderArray<vector<float>> simHit_eta = {fReader, "simHit_eta"};
   TTreeReaderArray<vector<float>> simHit_phi = {fReader, "simHit_phi"};
   TTreeReaderArray<vector<int>> simHit_ieta = {fReader, "simHit_ieta"};
   TTreeReaderArray<vector<int>> simHit_iphi = {fReader, "simHit_iphi"};
   TTreeReaderArray<vector<int>> simHit_iz = {fReader, "simHit_iz"};
   TTreeReaderArray<float> recHit_noPF_energy = {fReader, "recHit_noPF_energy"};
   TTreeReaderArray<float> recHit_noPF_eta = {fReader, "recHit_noPF_eta"};
   TTreeReaderArray<float> recHit_noPF_phi = {fReader, "recHit_noPF_phi"};
   TTreeReaderArray<int> recHit_noPF_ieta = {fReader, "recHit_noPF_ieta"};
   TTreeReaderArray<int> recHit_noPF_iphi = {fReader, "recHit_noPF_iphi"};
   TTreeReaderArray<int> recHit_noPF_iz = {fReader, "recHit_noPF_iz"};
   TTreeReaderArray<float> pfRecHit_unClustered_energy = {fReader, "pfRecHit_unClustered_energy"};
   TTreeReaderArray<float> pfRecHit_unClustered_eta = {fReader, "pfRecHit_unClustered_eta"};
   TTreeReaderArray<float> pfRecHit_unClustered_phi = {fReader, "pfRecHit_unClustered_phi"};
   TTreeReaderArray<int> pfRecHit_unClustered_ieta = {fReader, "pfRecHit_unClustered_ieta"};
   TTreeReaderArray<int> pfRecHit_unClustered_iphi = {fReader, "pfRecHit_unClustered_iphi"};
   TTreeReaderArray<int> pfRecHit_unClustered_iz = {fReader, "pfRecHit_unClustered_iz"};
   TTreeReaderArray<float> pfCluster_energy = {fReader, "pfCluster_energy"};
   TTreeReaderArray<float> pfCluster_eta = {fReader, "pfCluster_eta"};
   TTreeReaderArray<float> pfCluster_phi = {fReader, "pfCluster_phi"};
   TTreeReaderArray<int> pfCluster_ieta = {fReader, "pfCluster_ieta"};
   TTreeReaderArray<int> pfCluster_iphi = {fReader, "pfCluster_iphi"};
   TTreeReaderArray<int> pfCluster_iz = {fReader, "pfCluster_iz"};
   TTreeReaderArray<int> pfCluster_dR_genScore_MatchedIndex = {fReader, "pfCluster_dR_genScore_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_dR_simScore_MatchedIndex = {fReader, "pfCluster_dR_simScore_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_n_shared_xtals_MatchedIndex = {fReader, "pfCluster_n_shared_xtals_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_sim_fraction_MatchedIndex = {fReader, "pfCluster_sim_fraction_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_sim_fraction_min1_MatchedIndex = {fReader, "pfCluster_sim_fraction_min1_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_sim_fraction_min3_MatchedIndex = {fReader, "pfCluster_sim_fraction_min3_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_sim_rechit_diff_MatchedIndex = {fReader, "pfCluster_sim_rechit_diff_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_sim_rechit_fraction_MatchedIndex = {fReader, "pfCluster_sim_rechit_fraction_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_global_sim_rechit_fraction_MatchedIndex = {fReader, "pfCluster_global_sim_rechit_fraction_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_hgcal_caloToCluster_MatchedIndex = {fReader, "pfCluster_hgcal_caloToCluster_MatchedIndex"};
   TTreeReaderArray<int> pfCluster_hgcal_clusterToCalo_MatchedIndex = {fReader, "pfCluster_hgcal_clusterToCalo_MatchedIndex"};
   TTreeReaderArray<vector<double>> pfCluster_dR_genScore = {fReader, "pfCluster_dR_genScore"};
   TTreeReaderArray<vector<double>> pfCluster_dR_simScore = {fReader, "pfCluster_dR_simScore"};
   TTreeReaderArray<vector<int>> pfCluster_n_shared_xtals = {fReader, "pfCluster_n_shared_xtals"};
   TTreeReaderArray<vector<double>> pfCluster_sim_fraction = {fReader, "pfCluster_sim_fraction"};
   TTreeReaderArray<vector<double>> pfCluster_sim_fraction_min1 = {fReader, "pfCluster_sim_fraction_min1"};
   TTreeReaderArray<vector<double>> pfCluster_sim_fraction_min3 = {fReader, "pfCluster_sim_fraction_min3"};
   TTreeReaderArray<vector<double>> pfCluster_sim_rechit_diff = {fReader, "pfCluster_sim_rechit_diff"};
   TTreeReaderArray<vector<double>> pfCluster_sim_rechit_fraction = {fReader, "pfCluster_sim_rechit_fraction"};
   TTreeReaderArray<vector<double>> pfCluster_global_sim_rechit_fraction = {fReader, "pfCluster_global_sim_rechit_fraction"};
   TTreeReaderArray<vector<double>> pfCluster_hgcal_caloToCluster = {fReader, "pfCluster_hgcal_caloToCluster"};
   TTreeReaderArray<vector<double>> pfCluster_hgcal_clusterToCalo = {fReader, "pfCluster_hgcal_clusterToCalo"};
   TTreeReaderArray<vector<float>> pfClusterHit_energy = {fReader, "pfClusterHit_energy"};
   TTreeReaderArray<vector<float>> pfClusterHit_rechitEnergy = {fReader, "pfClusterHit_rechitEnergy"};
   TTreeReaderArray<vector<float>> pfClusterHit_eta = {fReader, "pfClusterHit_eta"};
   TTreeReaderArray<vector<float>> pfClusterHit_phi = {fReader, "pfClusterHit_phi"};
   TTreeReaderArray<vector<int>> pfClusterHit_ieta = {fReader, "pfClusterHit_ieta"};
   TTreeReaderArray<vector<int>> pfClusterHit_iphi = {fReader, "pfClusterHit_iphi"};
   TTreeReaderArray<vector<int>> pfClusterHit_iz = {fReader, "pfClusterHit_iz"};

   // histograms
   // event number, sub-detector, pfcluster index
   std::map<int,std::map<TString,TH2F*>>               h_PFClusters_index;
   std::map<int,std::map<TString,TH2F*>>               h_PFClusters_energy;
   std::map<int,std::map<TString,std::map<int,TH2F*>>> h_PFClusters_cl_energy;
   std::map<int,std::map<TString,std::map<int,TH2F*>>> h_PFClusters_hits_fraction;
   std::map<int,std::map<TString,std::map<int,TH2F*>>> h_PFClusters_hits_energy;
   std::map<int,std::map<TString,std::map<int,TH2F*>>> h_PFClusters_bestCp_energy;
   std::map<int,std::map<TString,std::map<int,TH2F*>>> h_PFClusters_bestCp_dummy;
   std::map<int,std::map<TString,std::map<int,TH2F*>>> h_PFClusters_bestCp_simFraction;

   // binning of histograms
   std::map<TString,int>   nX = {{"EB",172}, {"EEP",100}, {"EEM",100}};
   std::map<TString,float> lX = {{"EB",-86.},{"EEP",0.},  {"EEM",0.}};
   std::map<TString,float> hX = {{"EB",86.}, {"EEP",100.},{"EEM",100.}};
   std::map<TString,int>   nY = {{"EB",360}, {"EEP",100}, {"EEM",100}};
   std::map<TString,float> lY = {{"EB",1.},  {"EEP",0.},  {"EEM",0.}};
   std::map<TString,float> hY = {{"EB",361.},{"EEP",100.},{"EEM",100.}};
   std::vector<TString> detectors = {"EB", "EEP", "EEM"};
   
   // algorithm variables
   TFile *fout;
   std::vector<int> eventsToCheck;
   int N_max_perEvent_plots = 10; // please don't go above 20, otherwise memory consumption will explode...
   int N_perEvent_plots;
   int N_perEvent_perCluster_plots = 300; // do not exceed 400
   bool flag_doEB;
   bool flag_doEE;

   // counters
   int N_pfcl_fracs_above30=0;
   
   // Selector methods 
   SingleEventPFClusterAnalyzer(TTree * /*tree*/ =0) { }
   virtual ~SingleEventPFClusterAnalyzer() { }
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

   ClassDef(SingleEventPFClusterAnalyzer,0);

};

#endif

#ifdef SingleEventPFClusterAnalyzer_cxx
void SingleEventPFClusterAnalyzer::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t SingleEventPFClusterAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef SingleEventPFClusterAnalyzer_cxx
