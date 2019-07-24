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


class PFClusterAnalyzer : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers
   TTreeReaderValue<Long64_t> eventId = {fReader, "eventId"};
   TTreeReaderValue<Int_t> lumiId = {fReader, "lumiId"};
   TTreeReaderValue<Int_t> runId = {fReader, "runId"};
   TTreeReaderArray<int> genParticle_id = {fReader, "genParticle_id"};
   TTreeReaderArray<float> genParticle_energy = {fReader, "genParticle_energy"};
   TTreeReaderArray<float> genParticle_pt = {fReader, "genParticle_pt"};
   TTreeReaderArray<float> genParticle_eta = {fReader, "genParticle_eta"};
   TTreeReaderArray<float> genParticle_phi = {fReader, "genParticle_phi"};
   TTreeReaderArray<float> caloParticle_energy = {fReader, "caloParticle_energy"};
   TTreeReaderArray<float> caloParticle_simEnergy = {fReader, "caloParticle_simEnergy"};
   TTreeReaderArray<float> caloParticle_pt = {fReader, "caloParticle_pt"};
   TTreeReaderArray<float> caloParticle_eta = {fReader, "caloParticle_eta"};
   TTreeReaderArray<float> caloParticle_phi = {fReader, "caloParticle_phi"};
   //TTreeReaderArray<vector<float>> simHit_energy = {fReader, "simHit_energy"};
   //TTreeReaderArray<vector<float>> simHit_eta = {fReader, "simHit_eta"};
   //TTreeReaderArray<vector<float>> simHit_phi = {fReader, "simHit_phi"};
   //TTreeReaderArray<vector<int>> simHit_ieta = {fReader, "simHit_ieta"};
   //TTreeReaderArray<vector<int>> simHit_iphi = {fReader, "simHit_iphi"};
   //TTreeReaderArray<vector<int>> simHit_iz = {fReader, "simHit_iz"};
   //TTreeReaderArray<vector<float>> recHit_energy = {fReader, "recHit_energy"};
   //TTreeReaderArray<vector<bool>> pfRecHit_isMatched = {fReader, "pfRecHit_isMatched"};
   TTreeReaderArray<vector<float>> pfClusterHit_energy = {fReader, "pfClusterHit_energy"};
   TTreeReaderArray<vector<float>> pfClusterHit_eta = {fReader, "pfClusterHit_eta"};
   TTreeReaderArray<vector<float>> pfClusterHit_phi = {fReader, "pfClusterHit_phi"};
   TTreeReaderArray<vector<int>> pfClusterHit_ieta = {fReader, "pfClusterHit_ieta"};
   TTreeReaderArray<vector<int>> pfClusterHit_iphi = {fReader, "pfClusterHit_iphi"};
   TTreeReaderArray<vector<int>> pfClusterHit_iz = {fReader, "pfClusterHit_iz"};
   TTreeReaderArray<float> pfCluster_energy = {fReader, "pfCluster_energy"};
   TTreeReaderArray<float> pfCluster_eta = {fReader, "pfCluster_eta"};
   TTreeReaderArray<float> pfCluster_phi = {fReader, "pfCluster_phi"};
   //TTreeReaderArray<map<int,int>> map_simHit_pfCluster = {fReader, "map_simHit_pfCluster"};
   TTreeReaderArray<map<int,int>> map_pfClusterHit_pfCluster = {fReader, "map_pfClusterHit_pfCluster"};


   // non reader members 
   // -- non root members
   float min_pfClusterHit_energy=0.08; // 80 MeV
   int N_perEvent_plots = 100;

   // -- root members
   TFile *fout;
   TH1F* h_PFClusters_caloMatched_size    ;
	TH1F* h_PFClusters_caloMatched_nXtals  ;
   TH1F* h_PFClusters_caloMatched_energy  ;
   TH1F* h_PFClusters_caloMatched_et      ;
   TH1F* h_PFClusters_caloMatched_eta     ;
   TH1F* h_PFClusters_caloMatched_phi     ;
   TH1F* h_PFClusters_caloMatched_eOverEtrue;
   TH2F* h_PFClusters_caloMatched_nXtals_vs_xtalEnergy;
   TH2F* h_PFClusters_caloMatched_nXtals_vs_energy;
   std::vector<TH2F*> h_PFClusterHit_EB_ietaiphi;


	//EEM
	TH1F* h_PFClusters_caloMatched_EEM_eta;
	TH1F* h_PFClusters_caloMatched_EEM_size;
   TH1F* h_PFClusters_caloMatched_EEM_nXtals;
   TH1F* h_PFClusters_caloMatched_EEM_energy;
   TH1F* h_PFClusters_caloMatched_EEM_et;
   TH1F* h_PFClusters_caloMatched_EEM_phi;
   TH1F* h_PFClusters_caloMatched_EEM_eOverEtrue;
   
	//EBM
	TH1F* h_PFClusters_caloMatched_EBM_eta;
	TH1F* h_PFClusters_caloMatched_EBM_size;
   TH1F* h_PFClusters_caloMatched_EBM_nXtals;
   TH1F* h_PFClusters_caloMatched_EBM_energy;
   TH1F* h_PFClusters_caloMatched_EBM_et;
   TH1F* h_PFClusters_caloMatched_EBM_phi;
   TH1F* h_PFClusters_caloMatched_EBM_eOverEtrue;
   

	//EBP
	TH1F* h_PFClusters_caloMatched_EBP_eta;
	TH1F* h_PFClusters_caloMatched_EBP_size;
   TH1F* h_PFClusters_caloMatched_EBP_nXtals;
   TH1F* h_PFClusters_caloMatched_EBP_energy;
   TH1F* h_PFClusters_caloMatched_EBP_et;
   TH1F* h_PFClusters_caloMatched_EBP_phi;
   TH1F* h_PFClusters_caloMatched_EBP_eOverEtrue;
   

	//EEP
	TH1F* h_PFClusters_caloMatched_EEP_eta;
	TH1F* h_PFClusters_caloMatched_EEP_size;
   TH1F* h_PFClusters_caloMatched_EEP_nXtals;
   TH1F* h_PFClusters_caloMatched_EEP_energy;
   TH1F* h_PFClusters_caloMatched_EEP_et;
   TH1F* h_PFClusters_caloMatched_EEP_phi;
   TH1F* h_PFClusters_caloMatched_EEP_eOverEtrue;
   

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
