//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul  8 11:35:13 2019 by ROOT version 6.14/09
// from TTree caloTree/caloTree
// found on file: RecoSimDumper.root
//////////////////////////////////////////////////////////

#ifndef PFClusterAnalyzer_h
#define PFClusterAnalyzer_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>

#include "vector"

#include <map>



class PFClusterAnalyzer : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<UInt_t> event = {fReader, "event"};
   TTreeReaderValue<UInt_t> run = {fReader, "run"};
   TTreeReaderValue<UInt_t> genParticle_id = {fReader, "genParticle_id"};
   TTreeReaderValue<Float_t> genParticle_energy = {fReader, "genParticle_energy"};
   TTreeReaderValue<Float_t> genParticle_pt = {fReader, "genParticle_pt"};
   TTreeReaderValue<Float_t> genParticle_eta = {fReader, "genParticle_eta"};
   TTreeReaderValue<Float_t> genParticle_phi = {fReader, "genParticle_phi"};
   TTreeReaderValue<Float_t> caloParticle_energy = {fReader, "caloParticle_energy"};
   TTreeReaderValue<Float_t> caloParticle_pt = {fReader, "caloParticle_pt"};
   TTreeReaderValue<Float_t> caloParticle_eta = {fReader, "caloParticle_eta"};
   TTreeReaderValue<Float_t> caloParticle_phi = {fReader, "caloParticle_phi"};
   //TTreeReaderArray<float> caloHit_energy = {fReader, "caloHit_energy"};
   //TTreeReaderArray<float> caloHit_time = {fReader, "caloHit_time"};
   TTreeReaderArray<float> simHit_energy = {fReader, "simHit_energy"};
   TTreeReaderArray<float> simHit_eta = {fReader, "simHit_eta"};
   TTreeReaderArray<float> simHit_phi = {fReader, "simHit_phi"};
   TTreeReaderArray<int> simHit_ieta = {fReader, "simHit_ieta"};
   TTreeReaderArray<int> simHit_iphi = {fReader, "simHit_iphi"};
   TTreeReaderArray<int> simHit_iz = {fReader, "simHit_iz"};
   TTreeReaderArray<float> recHit_energy = {fReader, "recHit_energy"};
   TTreeReaderValue<vector<bool>> pfRecHit_isMatched = {fReader, "pfRecHit_isMatched"};
   TTreeReaderArray<float> pfClusterHit_energy = {fReader, "pfClusterHit_energy"};
   TTreeReaderArray<float> pfCluster_energy = {fReader, "pfCluster_energy"};
   TTreeReaderArray<float> pfCluster_eta = {fReader, "pfCluster_eta"};
   TTreeReaderArray<float> pfCluster_phi = {fReader, "pfCluster_phi"};
   TTreeReaderArray<pair<int,int>> map_simHit_pfCluster = {fReader, "map_simHit_pfCluster"};


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
