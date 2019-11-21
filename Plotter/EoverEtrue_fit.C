#include "TStyle.h"
#include "Rtypes.h"
#include "RooGlobalFunc.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooFitResult.h"
#include "RooMCStudy.h"
#include "RooCBShape.h"
#include "RooBifurGauss.h"
#include "RooGaussian.h"
#include "RooProdPdf.h"
#include "RooPolynomial.h"
#include "RooExponential.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooHist.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include "TPad.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TLine.h"
#include "TPaveText.h"
#include "TEfficiency.h"

#include <iostream>
#include <iomanip>
#include <math.h>
#include <fstream>

using namespace RooFit ;
using namespace std;

/*
   The main functions of this script are the following:
   1. This script enables to fit the E/Etrue distribution.The residuals are plotted and the chi square is computed.
   2. Produce the scale, resolution and efficiency plots
   */


// will be used to retrieve that boundaries of the ranges
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
};


// additional functions
FitParameters performFit(string fileName, string outputdir, Int_t kEvents, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, FlagList flaglist, string);
// converts double to string with adjusting the number of decimals
TString getString(Float_t num, int decimal = 0);
// produce the resolution and scale plots
// produce the efficiency plots
void producePlot(TString, vector<string>, vector<map<TString, map<TString, Float_t>>>, vector<map<TString, map<TString, vector<Float_t>>>>,  Bool_t,  Bool_t, Bool_t, Bool_t, Bool_t, vector<TString>, vector<TString>, map<TString, Edges>, map<TString, Edges>, map<int, EColor> color, string, Int_t, vector<TString>, vector<TString>, vector<TString>, vector<TString>, vector<TString>);


void produceScanPlots(TString, vector<string>, vector<map<TString, map<TString, Float_t>>>, vector<map<TString, map<TString, vector<Float_t>>>>,  Bool_t,  Bool_t, Bool_t, Bool_t, Bool_t, vector<TString>, vector<TString>, map<TString, Edges>, map<TString, Edges>, map<int, EColor> color, string, Int_t, vector<TString>, vector<TString>, vector<TString>, vector<TString>, vector<TString>);


TGraphAsymmErrors* getGraph(TString whichPlot, string fileName, map<TString, map<TString, Float_t>> map_sigma, map<TString, map<TString, vector<Float_t>>> map_sigma_error, unsigned int kk, Bool_t printTitle, Bool_t do_EB, Bool_t do_EE, Bool_t do_binningEt, Bool_t use_simEnergy, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, map<int, EColor> color, string what);


TGraphAsymmErrors* getRatioGraph(TString whichPlot, string fileName1, string fileName2, vector<map<TString, map<TString, Float_t>>> map_sigma, vector<map<TString, map<TString, vector<Float_t>>>> map_sigma_error, unsigned int kk, Bool_t do_EB, Bool_t do_EE, Bool_t do_binningEt, Bool_t use_simEnergy, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, map<int, EColor> color, string what);



// main function
void EoverEtrue_fit(TString fineBinning_energy, TString fineBinning_eta, TString simEnergy, TString binningEt, TString CBfit, TString doubleCBfit, TString BGfit, TString fitPeak, TString resolutionPlot, TString scalePlot, TString efficiencyPlot, TString efficiencyPlotOnly, TString ratioPlot, TString scanThrs){

   // we fetch argurments of the function
   Bool_t do_fineBinning_energy = fineBinning_energy=="true" ? true : false;
   Bool_t do_fineBinning_eta = fineBinning_eta=="true" ? true : false;
   Bool_t use_simEnergy = simEnergy=="true" ? true : false;
   Bool_t do_binningEt = binningEt=="true" ? true : false;
   Bool_t do_CBfit = CBfit=="true" ? true : false;
   Bool_t do_doubleCBfit = doubleCBfit=="true" ? true : false;
   Bool_t do_BGfit = BGfit=="true" ? true : false;
   Bool_t do_fitPeak = fitPeak=="true" ? true : false;
   Bool_t do_resolutionPlot = resolutionPlot=="true" ? true : false;
   Bool_t do_scalePlot = scalePlot=="true" ? true : false;
   Bool_t do_efficiencyPlot = efficiencyPlot=="true" ? true : false;
   Bool_t do_efficiencyPlotOnly = efficiencyPlotOnly=="true" ? true : false;
   Bool_t do_ratioPlot = ratioPlot=="true" ? true : false;
   Bool_t do_scanThrs = scanThrs=="true" ? true : false;


   // we get the file names from the txt file produced by the AnalyserLauncher 
   vector<string> fileName;

   string item;
   ifstream myfile ("file.txt");
   if(myfile.is_open()){
      while(getline(myfile, item)){
         cout << item << endl;
         fileName.push_back(item);
      }
      myfile.close();
   }
   else{
      cout << "Couldn't open the file listing the production label to be analysed!" << endl;
   }


   if(do_ratioPlot==true && fileName.size()<2){
      cout << " WARNING - Please enter two file names in order to produce the ratio plot" << endl;
      cout << "Proceeding to regular plotting" << endl;
      do_ratioPlot=false;
   }

   if(do_scanThrs==true && fileName.size()<2){
      cout << "WARNING - Please enter at least two file names in order to proceed to the thresholds scan" << endl;
      cout << "Proceeding to regular plotting" << endl;
      do_scanThrs=false;
   }

   Bool_t do_EB = false;
   Bool_t do_EE = false;

   if(fileName[0].find("EB") != std::string::npos){
      do_EB = true;
   }
   else if(fileName[0].find("EE") != std::string::npos){
      do_EE = true;
   }
   else{
      cout << "Didn't find 'EB' nor 'EE' in the file" << endl;
      cout << "Aborting" << endl;
      exit(11);
   }


   Bool_t do_0to20GeV = false;
   Bool_t do_0to100GeV = false;
   Bool_t do_0p1to200GeV = false;

   if(fileName[0].find("0to20GeV") != std::string::npos){
      do_0to20GeV = true;
   }
   else if(fileName[0].find("0to100GeV") != std::string::npos){
      do_0to100GeV = true;
   }
   else if(fileName[0].find("0.1to200GeV") != std::string::npos){
      do_0p1to200GeV = true;
   }

   else{
      cout << "Didn't find energy range" << endl;
      cout << "Aborting" << endl;
      exit(11);
   }


   Int_t kEvents = 150;
   if(fileName[0].find("30000") != std::string::npos){
      if(do_EB){
         kEvents = 300;
      }
      else if(do_EE){
         kEvents = 600;
      }
   }


   Bool_t use_energy = false; 
   if(!use_simEnergy) use_energy = true;

   Bool_t do_binningEn = true;
   if(do_binningEt) do_binningEn = false;

   Bool_t do_fitAll = true;
   if(do_fitPeak) do_fitAll = false;

   FlagList flagList = {use_energy, use_simEnergy, do_binningEt, do_binningEn, do_CBfit, do_doubleCBfit, do_BGfit, do_fitAll, do_fitPeak};


   // define the different Et and Eta slots
   vector<TString> ETranges;
   if(do_0to20GeV){
      ETranges = {"1_5", "5_10", "10_15", "15_20"};
   }
   else if(do_0to100GeV){
      if(!do_fineBinning_energy){
         ETranges = {"1_20", "20_40", "40_60", "60_80", "80_100"};
      }
      else{
         ETranges = {"1_5", "5_10", "10_15", "15_20", "20_40", "40_60", "60_80", "80_100"};
      }
   }
   else if(do_0p1to200GeV){
      if(!do_fineBinning_energy){
         ETranges = {"1_20", "20_40", "40_60", "60_80", "80_100", "100_120", "120_140", "140_160", "160_180", "180_200"};
      }
      else{
         ETranges = {"1_5", "5_10", "10_15", "15_20", "20_40", "40_60", "60_80", "80_100", "100_120", "120_140", "140_160", "160_180", "180_200"};
         //ETranges = {"1_5", "100_120"};
      }
   }
   vector<TString> ETAranges_EB;
   if(!do_fineBinning_eta){
      ETAranges_EB = {"0p00_0p50", "0p50_1p00", "1p00_1p48"};
   }
   else{
      ETAranges_EB = {"0p00_0p40", "0p40_0p80", "0p80_1p00", "1p00_1p20", "1p20_1p44", "1p44_1p48"};
   }


   vector<TString> ETAranges_EE;
   if(!do_fineBinning_eta){
      ETAranges_EE = {"1p48_2p00", "2p00_2p50", "2p50_3p00"};
   }
   else{
      ETAranges_EE = {"1p48_1p64", "1p64_1p85", "1p85_2p00", "2p00_2p20", "2p20_2p40", "2p40_2p60", "2p60_2p80", "2p80_3p00"};
      //ETAranges_EE = {"1p48_1p64"};
   }


   map<TString, Edges> ETvalue;
   map<TString, Edges> ETAvalue;

   // and adjust the boundaries
   ETvalue["0p1_1"].first = 0.1;
   ETvalue["0p1_1"].second = 1.;
   ETvalue["1_5"].first = 1.;
   ETvalue["1_5"].second = 5.;
   ETvalue["5_10"].first = 5.;
   ETvalue["5_10"].second = 10.;
   ETvalue["10_15"].first = 10.;
   ETvalue["10_15"].second = 15.;
   ETvalue["15_20"].first = 15.;
   ETvalue["15_20"].second = 20.;
   ETvalue["1_20"].first  = 1;
   ETvalue["1_20"].second = 20; 
   ETvalue["20_40"].first  = 20;
   ETvalue["20_40"].second = 40; 
   ETvalue["40_60"].first  = 40;
   ETvalue["40_60"].second = 60; 
   ETvalue["60_80"].first  = 60;
   ETvalue["60_80"].second = 80; 
   ETvalue["80_100"].first  = 80;
   ETvalue["80_100"].second = 100; 
   ETvalue["100_120"].first = 100.;
   ETvalue["100_120"].second = 120.;
   ETvalue["120_140"].first = 120.;
   ETvalue["120_140"].second = 140.;
   ETvalue["140_160"].first = 140.;
   ETvalue["140_160"].second = 160.;
   ETvalue["160_180"].first = 160.;
   ETvalue["160_180"].second = 180.;
   ETvalue["180_200"].first = 180.;
   ETvalue["180_200"].second = 200.;


   ETAvalue["0p00_0p20"].first = 0.;
   ETAvalue["0p00_0p20"].second = 0.2;
   ETAvalue["0p00_0p40"].first = 0.;
   ETAvalue["0p00_0p40"].second = 0.4;
   ETAvalue["0p40_0p80"].first = 0.4;
   ETAvalue["0p40_0p80"].second = 0.8;
   ETAvalue["0p20_0p40"].first = 0.2;
   ETAvalue["0p20_0p40"].second = 0.4;
   ETAvalue["0p40_0p60"].first = 0.4;
   ETAvalue["0p40_0p60"].second = 0.6;
   ETAvalue["0p60_0p80"].first = 0.6;
   ETAvalue["0p60_0p80"].second = 0.8;
   ETAvalue["0p80_1p00"].first = 0.8;
   ETAvalue["0p80_1p00"].second = 1.;
   ETAvalue["1p00_1p20"].first = 1.;
   ETAvalue["1p00_1p20"].second = 1.2;
   ETAvalue["1p20_1p44"].first = 1.2;
   ETAvalue["1p20_1p44"].second = 1.44;
   ETAvalue["1p40_1p60"].first = 1.4;
   ETAvalue["1p40_1p60"].second = 1.6;
   ETAvalue["0p00_0p50"].first  = 0.0;
   ETAvalue["0p00_0p50"].second = 0.5;
   ETAvalue["0p50_1p00"].first  = 0.5;
   ETAvalue["0p50_1p00"].second = 1.0;
   ETAvalue["1p00_1p44"].first  = 1.0;
   ETAvalue["1p00_1p44"].second = 1.44;
   ETAvalue["1p48_1p57"].first = 1.479;
   ETAvalue["1p48_1p57"].second = 1.566;
   ETAvalue["1p48_1p64"].first = 1.479;
   ETAvalue["1p48_1p64"].second = 1.64;
   ETAvalue["1p57_1p65"].first = 1.566;
   ETAvalue["1p57_1p65"].second = 1.653;
   ETAvalue["1p64_1p85"].first = 1.64;
   ETAvalue["1p64_1p85"].second = 1.85;
   ETAvalue["1p65_1p85"].first = 1.653;
   ETAvalue["1p65_1p85"].second = 1.85;
   ETAvalue["1p85_2p00"].first = 1.85;
   ETAvalue["1p85_2p00"].second = 2.0;
   ETAvalue["2p00_2p20"].first = 2.0;
   ETAvalue["2p00_2p20"].second = 2.20;
   ETAvalue["2p20_2p40"].first = 2.20;
   ETAvalue["2p20_2p40"].second = 2.40;
   ETAvalue["2p40_2p60"].first = 2.40;
   ETAvalue["2p40_2p60"].second = 2.60;
   ETAvalue["2p60_2p80"].first = 2.60;
   ETAvalue["2p60_2p80"].second = 2.80;
   ETAvalue["2p80_3p00"].first = 2.80;
   ETAvalue["2p80_3p00"].second = 3.0;
   ETAvalue["1p44_1p48"].first  = 1.44;
   ETAvalue["1p44_1p48"].second = 1.48;
   ETAvalue["1p00_1p48"].first  = 1.0;
   ETAvalue["1p00_1p48"].second = 1.48;
   ETAvalue["1p48_2p00"].first  = 1.48;
   ETAvalue["1p48_2p00"].second = 2.0;
   ETAvalue["2p00_2p50"].first  = 2.0;
   ETAvalue["2p00_2p50"].second = 2.5;
   ETAvalue["2p50_3p00"].first  = 2.5;
   ETAvalue["2p50_3p00"].second = 3.0;


   //color for the resolution, scale and efficiency plots
   map<int, EColor> color;
   color[0]=kOrange;
   color[1]=kGreen;
   color[2]=kCyan;
   color[3]=kRed;
   color[4]=kMagenta;
   color[5]=kViolet;
   color[6]=kAzure;
   //color[7]=kBlue;
   color[7]=kBlack;

   // define the output directory
   string outputdir = "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/myPlots/fits/" + fileName[0];
   if(do_binningEt){
      outputdir += "_EtaEtBinned";
   }
   else if(do_binningEn){
      outputdir += "_EtaEnBinned";
   }
   if(do_CBfit){
      outputdir += "_CB";
   }
   else if(do_doubleCBfit){
      outputdir += "_doubleCB";
   }
   if(do_BGfit){
      outputdir += "_BG";
   }

   if(use_simEnergy){
      outputdir += "_simEnergy";
   }
   outputdir += "/";

   if(do_ratioPlot){
      outputdir += "ratio/" + fileName[1] + "/";
   }

   // create the output directory
   system(Form("mkdir -p %s", outputdir.c_str()));


   // we get the matching strategy from the fileName
   vector<TString> matching;
   for(unsigned int iFile(0); iFile<fileName.size(); ++ iFile){
      if(fileName[iFile].find("numberOfHits") != std::string::npos){
         matching.push_back("numberOfHits");
      }
      else if(fileName[iFile].find("simFraction") != std::string::npos){
         matching.push_back("simFraction_min1");
      }
      else if(fileName[iFile].find("deltaR") != std::string::npos){
         matching.push_back("deltaR");
      }
   }

   // we get the pileUp tag from the fileName
   vector<TString> PUtag;
   for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
      if(fileName[iFile].find("noPU") != std::string::npos){
         PUtag.push_back("noPU");
      }
      else if(fileName[iFile].find("wPU") != std::string::npos){
         PUtag.push_back("wPU");
      }
   }

   // we get the thresholds from the fileName
   vector<TString> pfrechit_thrs;
   vector<TString> seeding_thrs;
   for(unsigned int iFile(0); iFile<fileName.size(); ++ iFile){

      std::size_t found_pfrechit_inf = fileName[iFile].find("pfrh");
      std::size_t found_pfrechit_sup = fileName[iFile].find('_', found_pfrechit_inf);
      string pfrechit_thrs_value = fileName[iFile].substr(found_pfrechit_inf, found_pfrechit_sup-found_pfrechit_inf);
      TString thrs_rechit = pfrechit_thrs_value.c_str();
      pfrechit_thrs.push_back(thrs_rechit);

      std::size_t found_seeding_inf = fileName[iFile].find("seed");
      std::size_t found_seeding_sup = fileName[iFile].find('_', found_seeding_inf);
      string seeding_thrs_value = fileName[iFile].substr(found_seeding_inf, found_seeding_sup-found_seeding_inf);
      TString thrs_seeding = seeding_thrs_value.c_str();
      seeding_thrs.push_back(thrs_seeding);

   }

   // we retrieve whether it was crytal dependent or ring dependent
   vector<TString> dependency;
   for(unsigned int iFile(0); iFile<fileName.size(); ++ iFile){
      if(fileName[iFile].find("thrRing") != std::string::npos){
         dependency.push_back("#etaRing");
      }
      else{
         dependency.push_back("Xtal");
      }
   }

   // we perform the fit
   FitParameters fitParameters_EB;
   FitParameters fitParameters_EE;

   // we retrieve parameters of the fit
   vector<map<TString, map<TString, Float_t>>> sigma;
   vector<map<TString, map<TString, vector<Float_t>>>> sigma_error;
   vector<map<TString, map<TString, Float_t>>> mean;
   vector<map<TString, map<TString, vector<Float_t>>>> mean_error;

   vector<TString> input;


   if(!do_efficiencyPlotOnly){
      if(do_EB){
         if(!do_ratioPlot){
            fitParameters_EB = performFit(fileName[0], outputdir, kEvents, ETranges, ETAranges_EB, ETvalue, ETAvalue, flagList, "EB");
            sigma.push_back(fitParameters_EB.map_sigma);
            sigma_error.push_back(fitParameters_EB.map_sigma_error);
            mean.push_back(fitParameters_EB.map_mean);
            mean_error.push_back(fitParameters_EB.map_mean_error);
         }
         else{
            for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
               fitParameters_EB = performFit(fileName[iFile], outputdir, kEvents, ETranges, ETAranges_EB, ETvalue, ETAvalue, flagList, "EB");
               sigma.push_back(fitParameters_EB.map_sigma);
               sigma_error.push_back(fitParameters_EB.map_sigma_error);
               mean.push_back(fitParameters_EB.map_mean);
               mean_error.push_back(fitParameters_EB.map_mean_error);
            }
         }
      }
      if(do_EE){
         if(!do_ratioPlot){
            fitParameters_EE = performFit(fileName[0], outputdir, kEvents, ETranges, ETAranges_EE, ETvalue, ETAvalue, flagList, "EE");
            sigma.push_back(fitParameters_EE.map_sigma);
            sigma_error.push_back(fitParameters_EE.map_sigma_error);
            mean.push_back(fitParameters_EE.map_mean);
            mean_error.push_back(fitParameters_EE.map_mean_error);
         }
         else{
            for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
               fitParameters_EE = performFit(fileName[iFile], outputdir, kEvents, ETranges, ETAranges_EE, ETvalue, ETAvalue, flagList, "EE");
               sigma.push_back(fitParameters_EE.map_sigma);
               sigma_error.push_back(fitParameters_EE.map_sigma_error);
               mean.push_back(fitParameters_EE.map_mean);
               mean_error.push_back(fitParameters_EE.map_mean_error);
            }
         }
      }
   }
   vector<TString> ETAranges;
   if(do_EB){
      ETAranges = ETAranges_EB;
      if(do_EE){
         ETAranges.insert(ETAranges.end(), ETAranges_EE.begin(), ETAranges_EE.end());
      }
   }
   else{
      ETAranges = ETAranges_EE;
   }

   // we get the resolution, scale and efficiency plots
   if(do_resolutionPlot && !do_efficiencyPlotOnly && !do_scanThrs){
      producePlot("Resolution", fileName, sigma, sigma_error, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
   }

   if(do_scalePlot && !do_efficiencyPlotOnly && !do_scanThrs){
      producePlot("Scale", fileName, mean, mean_error, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
   }

   if((do_efficiencyPlot || do_efficiencyPlotOnly) && !do_scanThrs){
      vector<map<TString, map<TString, Float_t>>> map_dummy(2);
      vector<map<TString, map<TString, vector<Float_t>>>> map_error_dummy(2);
      producePlot("Efficiency", fileName, map_dummy, map_error_dummy, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
      cout << "je suis ici" << endl;
   }

   if(do_scanThrs){
      vector<map<TString, map<TString, Float_t>>> map_dummy(999);
      vector<map<TString, map<TString, vector<Float_t>>>> map_error_dummy(999);
      if(fileName.size()>0){
         for(unsigned int iFile(1); iFile<fileName.size(); ++iFile){
            if(PUtag[iFile]!=PUtag[0]){
               cout << "You introduced in the list a file with wrong PU tag!" << endl;
               cout << "Aborting" << endl;
               exit(11);
            }
            if(matching[iFile]!=matching[0]){
               cout << "You introduced in the list a file with a different matching strategy!" << endl;
               cout << "Aborting" << endl;
               exit(11);
            }
         }
      }
      produceScanPlots("Efficiency", fileName, map_dummy, map_error_dummy, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
   }

}




FitParameters performFit(string fileName, string outputdir, Int_t kEvents, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, FlagList flagList, string do_where){

   Bool_t do_EB = false;
   Bool_t do_EE = false;

   if(do_where=="EB"){
      do_EB = true;
      do_EE = false;
   }
   else if(do_where=="EE"){
      do_EB = false;
      do_EE = true;
   }

   // struct of fit parameters that will be filled at the end 
   FitParameters fitParameters;

   // initialization of the bools
   Bool_t use_energy = flagList.use_energy;
   Bool_t use_simEnergy = flagList.use_simEnergy;
   Bool_t do_binningEt = flagList.do_binningEt;
   Bool_t do_binningEn = flagList.do_binningEn;
   Bool_t do_CBfit = flagList.do_CBfit;
   Bool_t do_doubleCBfit = flagList.do_doubleCBfit;
   Bool_t do_BGfit = flagList.do_BGfit;
   Bool_t do_fitAll = flagList.do_fitAll;
   Bool_t do_fitPeak = flagList.do_fitPeak;

   // for the resolution (sigma of the gaussian)
   map<TString, map<TString, Float_t>> map_sigma;
   map<TString, map<TString, vector<Float_t>>> map_sigma_error;

   // for the scale (mean of the gaussian)
   map<TString, map<TString, Float_t>> map_mean;
   map<TString, map<TString, vector<Float_t>>> map_mean_error;


   TFile* inputFile = 0;
   TString name_tmp = fileName.c_str();
   if(do_EB){
      inputFile = TFile::Open("../Analyzer/outputfiles/" + name_tmp + "_EB.root");
   }
   else if(do_EE){
      inputFile = TFile::Open("../Analyzer/outputfiles/" + name_tmp + "_EE.root");
   }

   fitParameters.inputFile = inputFile->GetName();

   // ranges of the distribution
   Double_t rangeMin = 0.;
   Double_t rangeMax = 2.;


   // we loop on the different ET and ETA ranges
   for(unsigned int i(0); i<ETranges.size(); ++i){
      for(unsigned int j(0); j<ETAranges.size(); ++j){
         // we get te necessary files and histograms
         TH1D* dmhist = 0;
         if(use_energy && do_binningEt){
            dmhist = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_eOverEtrue_Eta" + ETAranges[j] + "_Et" + ETranges[i])->Clone("dmhist");
         }
         else if(use_energy && do_binningEn){
            dmhist = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_eOverEtrue_Eta" + ETAranges[j] + "_En" + ETranges[i])->Clone("dmhist");
         }
         else if(use_simEnergy && do_binningEt){
            dmhist = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + ETAranges[j] + "_Et" + ETranges[i])->Clone("dmhist");
         }
         else if(use_simEnergy && do_binningEn){
            dmhist = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + ETAranges[j] + "_En" + ETranges[i])->Clone("dmhist");
         }
         // we declare dm as a RooRealVar (for the residual) and as a RooDataHist (for the fit):
         RooRealVar* EoverEtrue = new RooRealVar("EoverEtrue","EoverEtrue" ,rangeMin,rangeMax);
         EoverEtrue ->setBins(150);

         RooDataHist* rdh = new RooDataHist("rdh", "rdh", *EoverEtrue, dmhist);


         float input_mean = dmhist->GetMean();
         float input_sigma = dmhist->GetStdDev();

         TString mean_tmp =to_string(input_mean);
         TString sigma_tmp = to_string(input_sigma);

         //Define the PDF to fit: 

         // crystal ball (gaussian + exponential decaying tails)
         // we declare all the parameters needed for the fits	
         float mean_init = dmhist->GetMean();
         float mean_min = 0.8*dmhist->GetMean();
         float mean_max = 1.2*dmhist->GetMean();

         float sigma_init; 
         float sigma_min = 0;
         float sigma_max; 

        if(mean_init<1.96){
            sigma_init = dmhist->GetStdDev();
            sigma_max = 1.8*dmhist->GetStdDev();
         }
         else{
            sigma_init = dmhist->GetStdDev()/4;
            sigma_max = 1.8*dmhist->GetStdDev()/4;
         }
         float alpha_1_init = 10;
         float alpha_1_min = -10;
         float alpha_1_max = 10;

         float alpha_2_init = 1;
         float alpha_2_min = -10;
         float alpha_2_max = 10;

         float n_1_min = 0;
         float n_1_max = 50;

         float n_2_min = 0;
         float n_2_max = 15;


         /*if(ETvalue[ETranges[i]].second <= 30){
            mean_init = 1.7;
            mean_min = 0.7;
            mean_max = 1.2;
            sigma_init = 0.07;
            sigma_max = 0.16;
            alpha_1_init = 10.;
            alpha_2_init = 1.;
            n_1_init = 1.;
            n_2_init = 1.;
         }
         else{
            mean_init = 1.015;
            mean_min = 0.7;
            mean_max = 1.2;
            sigma_init = 0.027;
            sigma_max = 0.16;
            alpha_1_init = 3.0;
            alpha_2_init = -2.0;
            n_1_init = 5;
            n_2_init = 1;
         }
         */
         RooRealVar *mean   = new RooRealVar("mean","mean", mean_init, mean_min, mean_max);
         RooRealVar *sigma  = new RooRealVar("sigma","sigma", sigma_init, sigma_min, sigma_max);
         RooRealVar *alpha  = new RooRealVar("alpha", "alpha", 1., 0, 2.);
         RooRealVar *n      = new RooRealVar("n", "n", 1., 0., 10.);

         RooCBShape *CBpdf = new RooCBShape("CBpdf", "CBpdf", *EoverEtrue, *mean, *sigma, *alpha, *n);


         // double crystal ball (same gaussian body but different exponential tails)
         RooRealVar *alpha_1  = new RooRealVar("alpha_1", "alpha_1", 1., alpha_1_min, alpha_2_max);
         RooRealVar *n_1      = new RooRealVar("n_1", "n_1", n_1_min, n_1_max);
         RooRealVar *alpha_2  = new RooRealVar("alpha_2", "alpha_2", 1., alpha_2_min, alpha_2_max);
         RooRealVar *n_2      = new RooRealVar("n_2", "n_2", n_2_min, n_2_max);


         RooCBShape *CBpdf_1 = new RooCBShape("CBpdf_1", "CBpdf_1", *EoverEtrue, *mean, *sigma, *alpha_1, *n_1);
         RooCBShape *CBpdf_2 = new RooCBShape("CBpdf_2", "CBpdf_2", *EoverEtrue, *mean, *sigma, *alpha_2, *n_2);

         // defines the relative importance of the fit
         RooRealVar *sigfrac  = new RooRealVar("sigfrac","sigfrac", 0.0 ,1.0);

         // we add the two CB pdfs together
         RooAddPdf *doubleCBpdf   = new RooAddPdf("doubleCBpdf", "doubleCBpdf", *CBpdf_1, *CBpdf_2, *sigfrac); 

         // bifurcated gaussian:
         RooRealVar *mean_BG     = new RooRealVar("mean_BG","mean_BG",0.98,0.9,1.1);
         RooRealVar *sigma_left  = new RooRealVar("#sigma_{l}","sigmal",0.015, 0.0, 0.1);
         RooRealVar *sigma_right = new RooRealVar("#sigma_{r}","sigmar",0.0075, 0.0, 0.1);

         RooAbsPdf *BGpdf = new RooBifurGauss("BGpdf","BGpdf", *EoverEtrue, *mean_BG, *sigma_left, *sigma_right);


         // we define the frame where to plot
         TCanvas *canv = new TCanvas("canv", "canv", 700, 600);
         RooPlot *frame = EoverEtrue->frame(Title("Single photon produced in front of ECAL"));

         // plot the data
         rdh->plotOn(frame, RooFit::Name("data"));

         // fit the PDF to the data
         RooFitResult *result;
         if(do_CBfit){
            if(do_fitAll){
               result = CBpdf->fitTo(*rdh);
            }
            else if(do_fitPeak){
               EoverEtrue->setRange("peak", input_mean-4*input_sigma, input_mean+4*input_sigma);
               result = CBpdf->fitTo(*rdh, Range("peak"));
            }      
         }
         else if(do_doubleCBfit){
            if(do_fitAll){
               result = doubleCBpdf->fitTo(*rdh);
            }
            else if(do_fitPeak){
               EoverEtrue->setRange("peak", input_mean-4*input_sigma, input_mean+4*input_sigma);
               result = doubleCBpdf->fitTo(*rdh, Range("peak"));
            }      
         }
         else if(do_BGfit){
            if(do_fitAll){
               result = BGpdf->fitTo(*rdh);
            }
            else if(do_fitPeak){
               EoverEtrue->setRange("peak", input_mean-4*input_sigma, input_mean+4*input_sigma);
               result = BGpdf->fitTo(*rdh, Range("peak"));
            }      
         }


         // plot the fit 		
         if(do_CBfit){
            CBpdf->plotOn(frame,LineColor(4),RooFit::Name("CBpdf"),Components("CBpdf"));
         }
         if(do_doubleCBfit){
            doubleCBpdf->plotOn(frame,LineColor(4),RooFit::Name("doubleCBpdf"),Components("doubleCBpdf"));
         }
         else if(do_BGfit){
            BGpdf->plotOn(frame,LineColor(4),RooFit::Name("BGpdf"),Components("BGpdf"));
         }


         // and write the fit parameters
         if(do_CBfit){
            CBpdf->paramOn(frame,   
                  Layout(0.2, 0.4, 0.8),
                  Format("NEU",AutoPrecision(1))
                  );
         }
         else if(do_doubleCBfit){
            doubleCBpdf->paramOn(frame,   
                  Layout(0.2, 0.4, 0.8),
                  Format("NEU",AutoPrecision(1))
                  );
         }
         else if(do_BGfit){
            BGpdf->paramOn(frame,   
                  Layout(0.2, 0.4, 0.8),
                  Format("NEU",AutoPrecision(1))
                  );
         }
         frame->getAttText()->SetTextSize(0.03);
         frame->getAttLine()->SetLineColorAlpha(0, 0);
         frame->getAttFill()->SetFillColorAlpha(0, 0);

         // define the pavetext
         TPaveText* label;
         if(!do_doubleCBfit){
            label = new TPaveText(0.2,0.25,0.4,0.45,"brNDC");
         }
         else{
            label = new TPaveText(0.626,0.25,0.726,0.45,"brNDC");
         }
         label->SetBorderSize(0);
         label->SetFillColor(kWhite);
         label->SetTextSize(0.03);
         label->SetTextFont(42);
         label->SetTextAlign(11);
         TString kevt = to_string(kEvents);
         label->AddText(kevt + "k events " + mean_tmp + "  " + sigma_tmp);
         if(do_binningEt){
            label->AddText(getString(ETvalue[ETranges[i]].first, 0) + " < E_{T} < " + getString(ETvalue[ETranges[i]].second, 0) + "  GeV");
         }
         else if(do_binningEn){
            label->AddText(getString(ETvalue[ETranges[i]].first, 0) + " < E < " + getString(ETvalue[ETranges[i]].second, 0) + "  GeV");
         }
         label->AddText(getString(ETAvalue[ETAranges[j]].first, 2) + " < #eta < " + getString(ETAvalue[ETAranges[j]].second, 2));

         // we compute the chisquare
         Double_t chisquare;
         if(do_CBfit){
            chisquare = frame->chiSquare("CBpdf","data");
         }
         else if(do_doubleCBfit){
            chisquare = frame->chiSquare("doubleCBpdf","data");
         }
         else if(do_BGfit){
            chisquare = frame->chiSquare("BGpdf","data");
         }

         // and print it
         TPaveText* label_2 = new TPaveText(0.62,0.65,0.72,0.8,"brNDC");
         label_2->SetBorderSize(0);
         label_2->SetFillColor(kWhite);
         label_2->SetTextSize(0.03);
         label_2->SetTextFont(42);
         label_2->SetTextAlign(11);
         if(do_CBfit){
            label_2->AddText("CrystalBall PDF");
         }
         else if(do_doubleCBfit){
            label_2->AddText("Double-Sided CrystalBall PDF");
         }
         else if(do_BGfit){
            label_2->AddText("Bifurcated gaussian PDF");
         }
         TString chi2 = to_string(chisquare);
         label_2->AddText("#chi^{2} = " + chi2);
         cout << "chisquare = " << chisquare << endl;


         // We define and plot the residuals 		

         // construct a histogram with the pulls of the data w.r.t the curve
         RooHist* hpull = frame->pullHist();
         for(int i(0); i<frame->GetNbinsX(); ++i)
         {
            hpull->SetPointError(i,0,0,0,0);
         }

         // create a new frame to draw the pull distribution and add the distribution to the frame
         RooPlot* frame2 = EoverEtrue->frame(Title(" "));
         frame2->addPlotable(hpull,"P");//,"E3");

         canv->Divide(1,2);

         // plot of the curve and the fit
         canv->cd(1);
         gPad->SetLeftMargin(0.15) ; 
         gPad->SetPad(0.01,0.2,0.99,0.99);
         frame->GetXaxis()->SetTitleSize(0.04);
         frame->GetXaxis()->SetTitle("E/E_{true}");
         frame->GetYaxis()->SetTitleSize(0.04);
         frame->GetYaxis()->SetTitleOffset(1.1);
         frame->Draw();
         label->Draw();
         label_2->Draw();

         // plot of the residuals
         canv->cd(2);
         gPad->SetLeftMargin(0.15); 
         gPad->SetPad(0.01,0.01,0.99,0.2);

         frame2->GetYaxis()->SetNdivisions(3);
         frame2->GetYaxis()->SetLabelSize(0.17);
         frame2->GetYaxis()->SetTitleSize(0.17);
         frame2->GetYaxis()->SetTitleOffset(0.24);
         frame2->GetYaxis()->SetRangeUser(-5,5);	
         frame2->GetYaxis()->SetTitle("Pulls");	
         frame2->GetXaxis()->SetTitle("");	
         frame2->GetXaxis()->SetLabelOffset(5);	
         frame2->Draw();

         TLine *line = new TLine();
         line->DrawLine(rangeMin,0,rangeMax,0);
         line->SetLineColor(2);
         line->DrawLine(rangeMin,-3,rangeMax,-3);
         line->DrawLine(rangeMin,3,rangeMax,3);


         // save output
         canv->SaveAs(outputdir + fileName + "_EoverEtrue_fit_Et_" + ETranges[i] + "_Eta_" + ETAranges[j] + ".png");
         canv->SaveAs(outputdir + fileName + "_EoverEtrue_fit_Et_" + ETranges[i] + "_Eta_" + ETAranges[j] + ".pdf");


         //delete canv;

         // we retrieve the information necessary for the resolution/scale/efficiency plots
         map_sigma[ETranges[i]][ETAranges[j]] = sigma->getVal();
         map_mean[ETranges[i]][ETAranges[j]]  = mean->getVal();

         map_sigma_error[ETranges[i]][ETAranges[j]].push_back(sigma->getAsymErrorHi());
         map_sigma_error[ETranges[i]][ETAranges[j]].push_back(sigma->getAsymErrorLo());

         map_mean_error[ETranges[i]][ETAranges[j]].push_back(mean->getAsymErrorHi());
         map_mean_error[ETranges[i]][ETAranges[j]].push_back(mean->getAsymErrorLo());


      }
   }

   fitParameters.map_sigma       = map_sigma;
   fitParameters.map_sigma_error = map_sigma_error;
   fitParameters.map_mean        = map_mean;
   fitParameters.map_mean_error  = map_mean_error;

   return fitParameters;

}






TGraphAsymmErrors* getGraph(TString whichPlot, string fileName, map<TString, map<TString, Float_t>> map_quantity, map<TString, map<TString, vector<Float_t>>> map_quantity_error, unsigned int kk, Bool_t printTitle, Bool_t do_EB, Bool_t do_EE, Bool_t do_binningEt, Bool_t use_simEnergy, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, map<int, EColor> color, string what){
   TString label = fileName.c_str();
   TString filename;
   if(do_EB){
      filename = "../Analyzer/outputfiles/" + label + "_EB.root";
   }
   else if(do_EE){
      filename = "../Analyzer/outputfiles/" + label + "_EE.root";
   }


   Float_t x, quantity, error;
   TEfficiency* eff_error;

   TGraphAsymmErrors* graph = new TGraphAsymmErrors(0);
   if(printTitle){
      graph->SetTitle(whichPlot);
   }
   else{
      graph->SetTitle("");
   }

   unsigned int size(0);
   if(what=="vsEnergy"){
      size = ETranges.size();
   }
   else if(what=="vsEta"){
      size = ETAranges.size();
   }

   int indexA(0), indexB(0);

   for(unsigned int ii(0); ii<size; ++ii){
      Float_t bin_inf(0.), bin_sup(0.);
      if(what=="vsEnergy"){
         bin_inf = ETvalue[ETranges[ii]].first;
         bin_sup = ETvalue[ETranges[ii]].second;
      }
      else if(what=="vsEta"){
         bin_inf = ETAvalue[ETAranges[ii]].first;
         bin_sup = ETAvalue[ETAranges[ii]].second;
      }
      x = (bin_inf + bin_sup)/2;     
      if(what=="vsEnergy"){
         indexA = kk;
         indexB = ii;
      }
      else if(what=="vsEta"){
         indexA = ii;
         indexB = kk;
      }


      if(whichPlot=="Efficiency"){
         TFile* inputFile = TFile::Open(filename);

         TH1D* hist_num  = 0;
         TH1D* hist_deno = 0;
         if(do_binningEt && !use_simEnergy){
            hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num");

            hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB])->Clone("hist_deno");
         }
         else if(!do_binningEt && !use_simEnergy){
            hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num");
            hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist_deno");
         }
         if(do_binningEt && use_simEnergy){
            hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num");
            hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB])->Clone("hist_deno");
         }
         else if(!do_binningEt && use_simEnergy){
            hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num");
            hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist_deno");
         }

         Float_t error_tmp(0);
         quantity = hist_num->GetEntries()/hist_deno->GetEntries();
         //cout << "efficiency: " << quantity << endl;i
         if(TEfficiency::CheckConsistency(*hist_num,*hist_deno)){
            eff_error = new TEfficiency(*hist_num, *hist_deno);
         }
         for(int i(0); i<hist_num->GetNbinsX(); ++i){
            error_tmp += eff_error->GetEfficiencyErrorLow(i);
         }
         error = error_tmp;
         int thisPoint = graph->GetN();
         graph->SetPoint(thisPoint, x, quantity);
         graph->SetPointError(thisPoint, (bin_sup - bin_inf)/2, (bin_sup - bin_inf)/2, error/2, error/2);
      }



      else if(whichPlot=="Resolution" || whichPlot=="Scale"){
         quantity = map_quantity[ETranges[indexB]][ETAranges[indexA]];
         error = map_quantity_error[ETranges[indexB]][ETAranges[indexA]][0];
         if(quantity!=0){
            int thisPoint = graph->GetN();
            graph->SetPoint(thisPoint, x, quantity);
            graph->SetPointError(thisPoint, (bin_sup - bin_inf)/2, (bin_sup - bin_inf)/2, error, error);
         }   
      }
   }


   if(whichPlot=="Efficiency"){
      graph->GetYaxis()->SetRangeUser(0.3, 1.5);
   }
   else if(whichPlot=="Resolution"){
      graph->GetYaxis()->SetRangeUser(0., 0.11);
   }
   else if(whichPlot=="Scale"){
      graph->GetYaxis()->SetRangeUser(0.7, 1.3);
   }
   graph->GetYaxis()->SetTitleSize(0.04);
   graph->GetYaxis()->SetTitleOffset(1.2);
   if(what=="vsEnergy" && do_binningEt){
      graph->GetXaxis()->SetTitle("E_{T} [GeV]");
   }
   else if(what=="vsEnergy" && !do_binningEt){
      graph->GetXaxis()->SetTitle("E [GeV]");
   }
   else if(what=="vsEta"){
      graph->GetXaxis()->SetTitle("#eta");
   }
   graph->GetXaxis()->SetTitleSize(0.04);
   graph->GetXaxis()->SetTitleOffset(1.1);
   graph->SetLineColor(color[kk]);
   graph->SetMarkerColor(color[kk]);

   return graph;
}





TGraphAsymmErrors* getRatioGraph(TString whichPlot, string fileName1, string fileName2, vector<map<TString, map<TString, Float_t>>> map_quantity, vector<map<TString, map<TString, vector<Float_t>>>> map_quantity_error, unsigned int kk, Bool_t do_EB, Bool_t do_EE, Bool_t do_binningEt, Bool_t use_simEnergy, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, map<int, EColor> color, string what){

   TString label1 = fileName1.c_str();
   TString filename1; 

   TString label2 = fileName2.c_str();
   TString filename2;

   if(do_EB){
      filename1 = "../Analyzer/outputfiles/" + label1 + "_EB.root";
      filename2 = "../Analyzer/outputfiles/" + label2 + "_EB.root";
   }
   else if(do_EE){
      filename1 = "../Analyzer/outputfiles/" + label1 + "_EE.root";
      filename2 = "../Analyzer/outputfiles/" + label2 + "_EE.root";
   }

   Float_t x, quantity1, quantity2, error, error1, error2;
   TEfficiency* eff_error1;
   TEfficiency* eff_error2;

   TGraphAsymmErrors* graph = new TGraphAsymmErrors(0);
   graph->SetTitle("Ratio");

   unsigned int size(0);
   if(what=="vsEnergy"){
      size = ETranges.size();
   }
   else if(what=="vsEta"){
      size = ETAranges.size();
   }

   int indexA(0), indexB(0);

   for(unsigned int ii(0); ii<size; ++ii){
      Float_t bin_inf(0.), bin_sup(0.);
      if(what=="vsEnergy"){
         bin_inf = ETvalue[ETranges[ii]].first;
         bin_sup = ETvalue[ETranges[ii]].second;
      }
      else if(what=="vsEta"){
         bin_inf = ETAvalue[ETAranges[ii]].first;
         bin_sup = ETAvalue[ETAranges[ii]].second;
      }

      x = (bin_inf + bin_sup)/2;     

      if(what=="vsEnergy"){
         indexA = kk;
         indexB = ii;
      }
      else if(what=="vsEta"){
         indexA = ii;
         indexB = kk;
      }


      if(whichPlot=="Efficiency"){
         TFile* inputFile1 = TFile::Open(filename1);
         TFile* inputFile2 = TFile::Open(filename2);
         //if(filename.Contains("EB") && ETAvalue[ETAranges[kk]].first>1.4) continue;
         //if(filename.Contains("EE") && ETAvalue[ETAranges[kk]].first<1.4) continue;

         TH1D* hist_num1  = 0;
         TH1D* hist_num2  = 0;
         TH1D* hist_deno1 = 0;
         TH1D* hist_deno2 = 0;

         if(do_binningEt && !use_simEnergy){

            hist_num1  = (TH1D*) inputFile1->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num1");
            hist_deno1 = (TH1D*) inputFile1->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB])->Clone("hist_deno1");
            hist_num2  = (TH1D*) inputFile2->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num2");
            hist_deno2 = (TH1D*) inputFile2->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB])->Clone("hist_deno2");

         }
         else if(!do_binningEt && !use_simEnergy){

            hist_num1 = (TH1D*) inputFile1->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num1");
            hist_deno1 = (TH1D*) inputFile1->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist_deno1");
            hist_num2 = (TH1D*) inputFile2->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num2");
            hist_deno2 = (TH1D*) inputFile2->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist_deno2");

         }
         if(do_binningEt && use_simEnergy){

            hist_num1 = (TH1D*) inputFile1->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num1");
            hist_deno1 = (TH1D*) inputFile1->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB])->Clone("hist_deno1");
            hist_num2 = (TH1D*) inputFile2->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num2");
            hist_deno2 = (TH1D*) inputFile2->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[indexA] + "_Et" + ETranges[indexB])->Clone("hist_deno2");

         }
         else if(!do_binningEt && use_simEnergy){

            hist_num1 = (TH1D*) inputFile1->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num1");
            hist_deno1 = (TH1D*) inputFile1->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist_deno1");
            hist_num2 = (TH1D*) inputFile2->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB] + "_forEfficiency")->Clone("hist_num2");
            hist_deno2 = (TH1D*) inputFile2->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist_deno2");

         }

         quantity1 = hist_num1->GetEntries()/hist_deno1->GetEntries();
         quantity2 = hist_num2->GetEntries()/hist_deno2->GetEntries();
         float error_tmp1(0.), error_tmp2(0.); 
         if(TEfficiency::CheckConsistency(*hist_num1,*hist_deno1)){
            eff_error1 = new TEfficiency(*hist_num1, *hist_deno1);
         }
         for(int i(0); i<hist_num1->GetNbinsX(); ++i){
            error_tmp1 += eff_error1->GetEfficiencyErrorLow(i);
         }
         error1 = error_tmp1;
         if(TEfficiency::CheckConsistency(*hist_num2,*hist_deno2)){
            eff_error2 = new TEfficiency(*hist_num2, *hist_deno2);
         }
         for(int i(0); i<hist_num2->GetNbinsX(); ++i){
            error_tmp2 += eff_error2->GetEfficiencyErrorLow(i);
         }
         error2 = error_tmp2;
         error = quantity1/quantity2*(error1/quantity1 + error2/quantity2);
         int thisPoint = graph->GetN();
         graph->SetPoint(thisPoint, x, quantity1/quantity2);
         graph->SetPointError(thisPoint, (bin_sup - bin_inf)/2, (bin_sup - bin_inf)/2, error/2, error/2);
      }
      else if(whichPlot=="Resolution" || whichPlot=="Scale"){
         quantity1 = map_quantity[0][ETranges[indexB]][ETAranges[indexA]];
         quantity2 = map_quantity[1][ETranges[indexB]][ETAranges[indexA]];
         error1 = map_quantity_error[0][ETranges[indexB]][ETAranges[indexA]][0];
         error2 = map_quantity_error[1][ETranges[indexB]][ETAranges[indexA]][0];
         error = quantity1/quantity2*(error1/quantity1 + error2/quantity2);
         if(quantity1!=0 && quantity2!=0){
            int thisPoint = graph->GetN();
            graph->SetPoint(thisPoint, x, quantity1/quantity2);
            graph->SetPointError(thisPoint, (bin_sup - bin_inf)/2, (bin_sup - bin_inf)/2, error, error);
         }   
      }
   }

   if(whichPlot=="Efficiency"){
      graph->GetYaxis()->SetRangeUser(0.98, 1.02);
   }
   else if(whichPlot=="Scale"){
      graph->GetYaxis()->SetRangeUser(0.98, 1.02);
   }
   else{
      graph->GetYaxis()->SetRangeUser(0.8, 1.2);
   } 
   graph->GetYaxis()->SetTitleSize(0.055);
   graph->GetYaxis()->SetTitleOffset(1.2);
   graph->GetYaxis()->SetLabelSize(0.05);
   if(what=="vsEnergy" && do_binningEt){
      graph->GetXaxis()->SetTitle("E_{T} [GeV]");
   }
   else if(what=="vsEnergy" && !do_binningEt){
      graph->GetXaxis()->SetTitle("E [GeV]");
   }
   else if(what=="vsEta"){
      graph->GetXaxis()->SetTitle("#eta");
   }
   graph->GetXaxis()->SetTitleSize(0.055);
   graph->GetXaxis()->SetTitleOffset(1.1);
   graph->GetXaxis()->SetLabelSize(0.04);
   graph->SetLineColor(color[kk]);
   graph->SetMarkerColor(color[kk]);

   return graph;
}


void produceScanPlots(TString whichPlot, vector<string> fileName, vector<map<TString, map<TString, Float_t>>> map_quantity, vector<map<TString, map<TString, vector<Float_t>>>> map_quantity_error, Bool_t do_ratioPlot, Bool_t do_EB, Bool_t do_EE, Bool_t do_binningEt, Bool_t use_simEnergy, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, map<int, EColor> color, string outputdir, Int_t kEvents, vector<TString> matching, vector<TString> PUtag, vector<TString> pfrechit_thrs, vector<TString> seeding_thrs, vector<TString> dependency){

   // we first produce the plot of the efficiency as a function of the energy at fixed eta ranges

   for(unsigned int kk(0); kk<ETAranges.size(); ++kk){
      TCanvas* c1 = new TCanvas("c1", "c1", 700, 600);
      TLegend* leg1 = new TLegend(0.55, 0.65, 0.9, 0.83);

      c1->cd();

      for(unsigned int ll(0); ll<fileName.size(); ++ll){
         TGraphAsymmErrors* graph1;
         graph1 = getGraph(whichPlot, fileName[ll], map_quantity[ll], map_quantity_error[ll], kk, true, do_EB, do_EE, do_binningEt, use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, "vsEnergy"); 
         graph1->SetLineColor(color[ll]);
         graph1->SetMarkerColor(color[ll]);


         if(ll==0){
            graph1->Draw("A*");
         }
         else{
            graph1->Draw("*, same");
         }

         if(seeding_thrs[ll]=="seedRef"){
            leg1 -> AddEntry(graph1, pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll]);
         }
         else{
            leg1 -> AddEntry(graph1, pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll] + " " + dependency[ll]);
         }
         leg1 -> SetTextSize(0.025);
         leg1 -> SetLineColor(0);
         leg1 -> SetFillColorAlpha(0, 0);
         leg1 -> SetBorderSize(0);
         leg1 -> Draw("same");
      }


      TPaveText* label = new TPaveText(0.63,0.85,0.8,0.88,"brNDC");
      label->SetBorderSize(0);
      label->SetFillColor(kWhite);
      label->SetTextSize(0.025);
      label->SetTextFont(42);
      label->SetTextAlign(11);
      label->AddText("#eta bin: " + ETAranges[kk]);
      label->Draw("same");

      TPaveText* label_info_up = new TPaveText(0.15,0.73,0.45,0.85,"brNDC");
      label_info_up->SetBorderSize(0);
      label_info_up->SetFillColor(kWhite);
      label_info_up->SetTextSize(0.028);
      label_info_up->SetTextFont(42);
      label_info_up->SetTextAlign(11);
      TString nEvents = to_string(kEvents);
      label_info_up->AddText(nEvents + "k Events, " + PUtag[0]);
      label_info_up->AddText(matching[0] + " matching");
      label_info_up->Draw("same");


      TString dir = outputdir.c_str();
      TString nameSave;
      if(whichPlot=="Resolution"){
         nameSave = "resolution_";
      }
      else if(whichPlot=="Scale"){
         nameSave = "scale_";
      }
      else if(whichPlot=="Efficiency"){
         nameSave = "efficiency_";
      }
      nameSave += "eta_" + ETAranges[kk];

      c1->cd();
      c1->SaveAs(dir + nameSave + "_vs_energy.png");
      c1->SaveAs(dir + nameSave + "_vs_energy.pdf");
      delete c1;
   }

   // we then produce the plot of the efficiency as a function of eta at fixed energy

   for(unsigned int kk(0); kk<ETranges.size(); ++kk){
      TCanvas* c1 = new TCanvas("c1", "c1", 700, 600);
      TLegend* leg1 = new TLegend(0.55, 0.65, 0.9, 0.83);

      c1->cd();

      for(unsigned int ll(0); ll<fileName.size(); ++ll){
         TGraphAsymmErrors* graph1;
         graph1 = getGraph(whichPlot, fileName[ll], map_quantity[ll], map_quantity_error[ll], kk, true, do_EB, do_EE, do_binningEt, use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, "vsEta"); 
         graph1->SetLineColor(color[ll]);
         graph1->SetMarkerColor(color[ll]);


         if(ll==0){
            graph1->Draw("A*");
         }
         else{
            graph1->Draw("*, same");
         }

         if(seeding_thrs[ll]=="seedRef"){
            leg1 -> AddEntry(graph1, pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll]);
         }
         else{
            leg1 -> AddEntry(graph1, pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll] + " " + dependency[ll]);
         }
         leg1 -> SetTextSize(0.025);
         leg1 -> SetLineColor(0);
         leg1 -> SetFillColorAlpha(0, 0);
         leg1 -> SetBorderSize(0);
         leg1 -> Draw("same");
      }


      TPaveText* label = new TPaveText(0.63,0.85,0.8,0.88,"brNDC");
      label->SetBorderSize(0);
      label->SetFillColor(kWhite);
      label->SetTextSize(0.025);
      label->SetTextFont(42);
      label->SetTextAlign(11);
      label->AddText("Energy bin: " + ETranges[kk]);
      label->Draw("same");

      TPaveText* label_info_up = new TPaveText(0.15,0.73,0.45,0.85,"brNDC");
      label_info_up->SetBorderSize(0);
      label_info_up->SetFillColor(kWhite);
      label_info_up->SetTextSize(0.028);
      label_info_up->SetTextFont(42);
      label_info_up->SetTextAlign(11);
      TString nEvents = to_string(kEvents);
      label_info_up->AddText(nEvents + "k Events, " + PUtag[0]);
      label_info_up->AddText(matching[0] + " matching");
      label_info_up->Draw("same");


      TString dir = outputdir.c_str();
      TString nameSave;
      if(whichPlot=="Resolution"){
         nameSave = "resolution_";
      }
      else if(whichPlot=="Scale"){
         nameSave = "scale_";
      }
      else if(whichPlot=="Efficiency"){
         nameSave = "efficiency_";
      }
      nameSave += "E_" + ETranges[kk];

      c1->cd();
      c1->SaveAs(dir + nameSave + "_vs_eta.png");
      c1->SaveAs(dir + nameSave + "_vs_eta.pdf");
      delete c1;
   }





}






void producePlot(TString whichPlot, vector<string> fileName, vector<map<TString, map<TString, Float_t>>> map_quantity, vector<map<TString, map<TString, vector<Float_t>>>> map_quantity_error, Bool_t do_ratioPlot, Bool_t do_EB, Bool_t do_EE, Bool_t do_binningEt, Bool_t use_simEnergy, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, map<int, EColor> color, string outputdir, Int_t kEvents, vector<TString> matching, vector<TString> PUtag, vector<TString> pfrechit_thrs, vector<TString> seeding_thrs, vector<TString> dependency){

   // we first produce the plot of the efficiency as a function of the energy for different eta ranges
   TCanvas* c1;
   if(!do_ratioPlot){
      c1 = new TCanvas("c1", "c1", 700, 600);
   }
   else{
      c1 = new TCanvas("c1", "c1", 700, 1000);
   }
   TLegend* leg1 = new TLegend(0.7, 0.55, 0.9, 0.85);

   c1->cd();
   TPad *pad1 = new TPad("pad1","pad1",0,0.625,1,1);
   pad1->SetBottomMargin(0.15);
   TPad *pad2 = new TPad("pad2","pad2",0,0.25,1,0.625);
   pad2->SetBottomMargin(0.15);
   TPad *pad3 = new TPad("pad3","pad3",0,0,1,0.25);
   pad3->SetBottomMargin(0.15);
   pad3->SetGrid();

   if(do_ratioPlot){
      pad1->Draw();
      pad2->Draw();
      pad3->Draw();
   }


   for(unsigned int kk(0); kk<ETAranges.size(); ++kk){
      TGraphAsymmErrors* graph1;
      graph1 = getGraph(whichPlot, fileName[0], map_quantity[0], map_quantity_error[0], kk, true, do_EB, do_EE, do_binningEt, use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, "vsEnergy"); 

      if(do_ratioPlot){
         pad1->cd();
      }
      if(kk==0){
         graph1->Draw("A*");
      }
      else{
         graph1->Draw("*, same");
      }
      leg1 -> AddEntry(graph1, ETAranges[kk]);
      leg1 -> SetTextSize(0.025);
      leg1 -> SetLineColor(0);
      leg1 -> SetFillColorAlpha(0, 0);
      leg1 -> SetBorderSize(0);
      leg1 -> Draw("same");

      if(do_ratioPlot){
         TGraphAsymmErrors* graph2 = getGraph(whichPlot, fileName[1], map_quantity[1], map_quantity_error[1], kk, false, do_EB, do_EE, do_binningEt, use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, "vsEnergy"); 
         pad2->cd();
         if(kk==0){
            graph2->Draw("A*");
         }
         else{
            graph2->Draw("*, same");
         }
         leg1->Draw("same");

         TGraphAsymmErrors* graph3 = getRatioGraph(whichPlot, fileName[0], fileName[1], map_quantity, map_quantity_error, kk, do_EB, do_EE, do_binningEt, use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, "vsEnergy"); 
         pad3->cd();
         if(kk==0){
            graph3->Draw("A*");
         }
         else{
            graph3->Draw("*, same");
         }
      }
   }

   TPaveText* label = new TPaveText(0.7,0.85,0.8,0.88,"brNDC");
   label->SetBorderSize(0);
   label->SetFillColor(kWhite);
   label->SetTextSize(0.025);
   label->SetTextFont(42);
   label->SetTextAlign(11);
   label->AddText("#eta bins: ");
   if(do_ratioPlot){
      pad1->cd();
   }
   label->Draw("same");
   if(do_ratioPlot){
      pad2->cd();
      label->Draw("same");
   }

   TPaveText* label_info_up = new TPaveText(0.15,0.73,0.45,0.85,"brNDC");
   label_info_up->SetBorderSize(0);
   label_info_up->SetFillColor(kWhite);
   label_info_up->SetTextSize(0.028);
   label_info_up->SetTextFont(42);
   label_info_up->SetTextAlign(11);
   TString nEvents = to_string(kEvents);
   label_info_up->AddText(nEvents + "k Events, " + PUtag[0]);
   label_info_up->AddText(matching[0] + " matching");
   if(do_ratioPlot){
      pad1->cd();
   }
   label_info_up->Draw("same");

   TPaveText* label_info_down = new TPaveText(0.15,0.73,0.45,0.85,"brNDC");
   label_info_down->SetBorderSize(0);
   label_info_down->SetFillColor(kWhite);
   label_info_down->SetTextSize(0.028);
   label_info_down->SetTextFont(42);
   label_info_down->SetTextAlign(11);
   label_info_down->AddText(nEvents + "k Events, " + PUtag[1]);
   label_info_down->AddText(matching[1] +  " matching");
   if(do_ratioPlot){
      pad2->cd();
      label_info_down->Draw("same");
   }

   TPaveText* label_thrs_up = new TPaveText(0.15,0.60,0.45,0.72,"brNDC");
   label_thrs_up->SetBorderSize(0);
   label_thrs_up->SetFillColor(kWhite);
   label_thrs_up->SetTextSize(0.028);
   label_thrs_up->SetTextFont(42);
   label_thrs_up->SetTextAlign(11);
   label_thrs_up->AddText(pfrechit_thrs[0] + " " + dependency[0]);
   if(seeding_thrs[0]=="seedRef"){
      label_thrs_up->AddText(seeding_thrs[0]);
   }
   else{
      label_thrs_up->AddText(seeding_thrs[0] + " " + dependency[0]);
   }
   if(do_ratioPlot){
      pad1->cd();
   }
   label_thrs_up->Draw("same");

   TPaveText* label_thrs_down = new TPaveText(0.15,0.60,0.45,0.72,"brNDC");
   label_thrs_down->SetBorderSize(0);
   label_thrs_down->SetFillColor(kWhite);
   label_thrs_down->SetTextSize(0.028);
   label_thrs_down->SetTextFont(42);
   label_thrs_down->SetTextAlign(11);
   label_thrs_down->AddText(pfrechit_thrs[1] + " " + dependency[1]);
   if(seeding_thrs[1]=="seedRef"){
      label_thrs_down->AddText(seeding_thrs[1]);
   }
   else{
      label_thrs_down->AddText(seeding_thrs[1] + " " + dependency[1]);
   }
   if(do_ratioPlot){
      pad2->cd();
      label_thrs_down->Draw("same");
   }


   TString dir = outputdir.c_str();
   TString nameSave;
   if(whichPlot=="Resolution"){
      nameSave = "resolution";
   }
   else if(whichPlot=="Scale"){
      nameSave = "scale";
   }
   else if(whichPlot=="Efficiency"){
      nameSave = "efficiency";
   }

   c1->cd();
   c1->SaveAs(dir + nameSave + "_vs_energy.png");
   c1->SaveAs(dir + nameSave + "_vs_energy.pdf");



   // we then produce the plot of the efficiency as a function of eta for different energy ranges
   TCanvas* c2;
   if(!do_ratioPlot){
      c2 = new TCanvas("c2", "c2", 700, 600);
   }
   else{
      c2 = new TCanvas("c2", "c2", 700, 1000);
   }
   TLegend* leg2 = new TLegend(0.7, 0.55, 0.9, 0.85);

   c2->cd();

   if(do_ratioPlot){
      pad1->Draw();
      pad2->Draw();
      pad3->Draw();
   }


   for(unsigned int kk(0); kk<ETranges.size(); ++kk){
      TGraphAsymmErrors* graph1 = getGraph(whichPlot, fileName[0], map_quantity[0], map_quantity_error[0], kk, true, do_EB, do_EE, do_binningEt, use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, "vsEta"); 
      if(do_ratioPlot){
         pad1->cd();
      }
      if(kk==0){
         graph1->Draw("A*");
      }
      else{
         graph1->Draw("*, same");
      }
      leg2 -> AddEntry(graph1, ETranges[kk]);
      leg2 -> SetTextSize(0.025);
      leg2 -> SetLineColor(0);
      leg2 -> SetFillColorAlpha(0, 0);
      leg2 -> SetBorderSize(0);
      leg2 -> Draw("same");

      if(do_ratioPlot){
         TGraphAsymmErrors* graph2 = getGraph(whichPlot, fileName[1], map_quantity[1], map_quantity_error[1], kk, false, do_EB, do_EE, do_binningEt, use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, "vsEta"); 
         pad2->cd();
         if(kk==0){
            graph2->Draw("A*");
         }
         else{
            graph2->Draw("*, same");
         }
         leg2->Draw("same");

         TGraphAsymmErrors* graph3 = getRatioGraph(whichPlot, fileName[0], fileName[1], map_quantity, map_quantity_error, kk, do_EB, do_EE, do_binningEt, use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, "vsEta"); 
         pad3->cd();
         if(kk==0){
            graph3->Draw("A*");
         }
         else{
            graph3->Draw("*, same");
         }
      }
   }
   TPaveText* label2 = new TPaveText(0.7,0.85,0.8,0.88,"brNDC");
   label2->SetBorderSize(0);
   label2->SetFillColor(kWhite);
   label2->SetTextSize(0.025);
   label2->SetTextFont(42);
   label2->SetTextAlign(11);
   if(do_binningEt){
      label2->AddText("E_{T} bins: ");
   }
   else{
      label2->AddText("Energy bins: ");
   }
   if(do_ratioPlot){
      pad1->cd();
   }
   label2->Draw("same");
   label_info_up->Draw("same");
   label_thrs_up->Draw("same");
   if(do_ratioPlot){
      pad2->cd();
      label2->Draw("same");
      label_info_down->Draw("same");
      label_thrs_down->Draw("same");
   }


   c2->cd();

   c2->SaveAs(dir + nameSave + "_vs_eta.png");
   c2->SaveAs(dir + nameSave + "_vs_eta.pdf");


   delete c1;
   delete c2;
}





TString getString(Float_t num, int decimal) {
   // this function allows to convert a float to a string with adjusting the number of decimal

   std::ostringstream streamObj3;

   // Set Fixed-Point Notation
   streamObj3 << std::fixed;

   // Set precision to your chosen number of digits
   streamObj3 << std::setprecision(decimal);

   // Add double to stream
   streamObj3 << num;

   // Get string from output string stream
   std::string strObj3 = streamObj3.str();

   return strObj3.c_str();
}


