
#ifndef OBJECTS_H
#define OBJECTS_H

/*****************************
 
  - We define here some structures that are called in the various functions of the plotting framework

*****************************/

struct Edges{
   Float_t first;
   Float_t second;
};

// collects all the flags entered in the user's decision board
struct FlagList{
   Bool_t use_energy;
   Bool_t use_simEnergy;
   Bool_t do_binningEt;
   Bool_t do_binningEn;
   Bool_t do_CBfit;
   Bool_t do_doubleCBfit;
   Bool_t do_BGfit;
   Bool_t do_fitAll;
   Bool_t do_fitPeak;
};

struct FitParameters{
   TString inputFile;
   map<TString, map<TString, Float_t>> map_sigma;
   map<TString, map<TString, vector<Float_t>>> map_sigma_error;
   map<TString, map<TString, Float_t>> map_mean;
   map<TString, map<TString, vector<Float_t>>> map_mean_error;
   map<TString, map<TString, Float_t>> map_chisquare;
   map<TString, map<TString, Float_t>> map_rms;
   map<TString, map<TString, Float_t>> map_rms_error;
};

struct PlottingTools{
   TGraphAsymmErrors* graph;
   vector<float> range;
};

#endif
