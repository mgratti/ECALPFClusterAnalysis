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
#include "TGraph2D.h"
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
#include <algorithm>

#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/objects.h"
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/utils.h"
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/performFit.C"
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/producePlot.C"
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/produceScanPlots.C"
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/getFile.C"




using namespace RooFit ;
using namespace std;

/*
   The main functions of this script are the following:
   1. This script enables to fit the E/Etrue distribution.The residuals are plotted and the chi square is computed.
   2. Produce the scale, resolution and efficiency plots
   */



void EoverEtrue_fit(TString fineBinning_energy, TString fineBinning_eta, TString simEnergy, TString binningEt, TString CBfit, TString doubleCBfit, TString BGfit, TString fitPeak, TString resolutionPlot, TString scalePlot, TString efficiencyPlot, TString fakeRatePlot, TString efficiencyPlotOnly, TString ratioPlot, TString scanThrs, TString autoScale, TString writeFiles){

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
   Bool_t do_fakeRatePlot = fakeRatePlot=="true" ? true : false;
   Bool_t do_efficiencyPlotOnly = efficiencyPlotOnly=="true" ? true : false;
   Bool_t do_ratioPlot = ratioPlot=="true" ? true : false;
   Bool_t do_scanThrs = scanThrs=="true" ? true : false;
   Bool_t do_autoScale = autoScale=="true" ? true : false;
   Bool_t do_writeFiles = writeFiles=="true" ? true : false;


   // we get the file names from the txt file produced by the AnalyserLauncher 
   vector<string> fileName;

   string item;
   ifstream myfile ("fileSamples.txt");
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

   vector<Bool_t> do_EB;
   vector<Bool_t> do_EE;

   for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
      if(fileName[iFile].find("_EB") != std::string::npos){
         do_EB.push_back(true);
         do_EE.push_back(false);
      }
      else if(fileName[iFile].find("_EE") != std::string::npos){
         do_EE.push_back(true);
         do_EB.push_back(false);
      }  
      else{
         cout << "Didn't find 'EB' nor 'EE' in the file" << endl;
         cout << "Aborting" << endl;
         exit(11);
      }
   }

   vector<Bool_t> do_0to20GeV;
   vector<Bool_t> do_0to100GeV;
   vector<Bool_t> do_0p1to200GeV;

   for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
      if(fileName[iFile].find("0to20GeV") != std::string::npos){
         do_0to20GeV.push_back(true);
         do_0to100GeV.push_back(false);
         do_0p1to200GeV.push_back(false);
      }
      else if(fileName[iFile].find("0to100GeV") != std::string::npos){
         do_0to20GeV.push_back(false);
         do_0to100GeV.push_back(true);
         do_0p1to200GeV.push_back(false);
      }
      else if((fileName[iFile].find("0.1to200GeV") != std::string::npos) || (fileName[0].find("0to200GeV") != std::string::npos)){
         do_0to20GeV.push_back(false);
         do_0to100GeV.push_back(false);
         do_0p1to200GeV.push_back(true);
      }
      else{
         cout << "Didn't find energy range" << endl;
         cout << "Aborting" << endl;
         exit(11);
      }
   }


   vector<Int_t> kEvents;
   for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
      if(fileName[iFile].find("30000") != std::string::npos){
         if(do_EB[iFile]){
            kEvents.push_back(300);
         }
         else if(do_EE[iFile]){
            kEvents.push_back(600);
         }
      }
      else{
         kEvents.push_back(150);
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
   vector<vector<TString>> ETranges;
   for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
      if(do_0to20GeV[iFile]){
         ETranges.push_back({"1_5", "5_10", "10_15", "15_20"});
      }
      else if(do_0to100GeV[iFile]){
         if(!do_fineBinning_energy){
            ETranges.push_back({"1_20", "20_40", "40_60", "60_80", "80_100"});
         }
         else{
            ETranges.push_back({"1_5", "5_10", "10_15", "15_20", "20_40", "40_60", "60_80", "80_100"});
         }
      }
      else if(do_0p1to200GeV[iFile]){
         if(!do_fineBinning_energy){
            ETranges.push_back({"1_20", "20_40", "40_60", "60_80", "80_100", "100_120", "120_140", "140_160", "160_180", "180_200"});
         }
         else{
            ETranges.push_back({"1_5", "5_10", "10_15", "15_20", "20_40", "40_60", "60_80", "80_100", "100_120", "120_140", "140_160", "160_180", "180_200"});
            //ETranges = {"80_100"};
         }
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
   vector<map<TString, map<TString, Float_t>>> chisquare;

   vector<TString> input;

   vector<vector<TString>> ETAranges;
   for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
      if(do_EB[iFile]){
         ETAranges.push_back(ETAranges_EB);
      }
      else{
         ETAranges.push_back(ETAranges_EE);
      }
   }

   if(do_writeFiles){
      for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
         ifstream file("samples/" + fileName[iFile] + ".txt");

         if(!file.is_open()){
            if(do_EB[iFile]){
               fitParameters_EB = performFit(fileName[iFile], outputdir, kEvents[iFile], ETranges[iFile], ETAranges_EB, ETvalue, ETAvalue, flagList, "EB");
               cout << "finished the fit" << endl;
               cout << "checking" << endl;
               cout << "ETranges " << ETranges[iFile].size() <<  endl;
               cout << "ETAranges " << ETAranges[iFile].size() << endl; 
               getFile(fileName[iFile], true, fitParameters_EB.map_sigma, fitParameters_EB.map_sigma_error, fitParameters_EB.map_mean, fitParameters_EB.map_mean_error, fitParameters_EB.map_chisquare, ETranges[iFile], ETAranges[iFile]);
            }
            else{
               fitParameters_EE = performFit(fileName[iFile], outputdir, kEvents[iFile], ETranges[iFile], ETAranges_EE, ETvalue, ETAvalue, flagList, "EE");
               getFile(fileName[iFile], false, fitParameters_EE.map_sigma, fitParameters_EE.map_sigma_error, fitParameters_EE.map_mean, fitParameters_EE.map_mean_error, fitParameters_EE.map_chisquare, ETranges[iFile], ETAranges[iFile]);
            }
         }
         else{
            cout << "Sample file " << fileName[iFile] << " already processed" << endl;
            cout << "--> Skipping " << endl;
         }
      }
   }



   if(!do_writeFiles){
      if(!do_efficiencyPlotOnly){
         for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
            if(do_EB[iFile]){
               fitParameters_EB = performFit(fileName[iFile], outputdir, kEvents[iFile], ETranges[iFile], ETAranges_EB, ETvalue, ETAvalue, flagList, "EB");
               sigma.push_back(fitParameters_EB.map_sigma);
               sigma_error.push_back(fitParameters_EB.map_sigma_error);
               mean.push_back(fitParameters_EB.map_mean);
               mean_error.push_back(fitParameters_EB.map_mean_error);
               chisquare.push_back(fitParameters_EB.map_chisquare);
            }
            else{
               fitParameters_EE = performFit(fileName[iFile], outputdir, kEvents[iFile], ETranges[iFile], ETAranges_EE, ETvalue, ETAvalue, flagList, "EE");
               sigma.push_back(fitParameters_EE.map_sigma);
               sigma_error.push_back(fitParameters_EE.map_sigma_error);
               mean.push_back(fitParameters_EE.map_mean);
               mean_error.push_back(fitParameters_EE.map_mean_error);
               chisquare.push_back(fitParameters_EE.map_chisquare);
            }
         }
      }
   }


   // we get the resolution, scale and efficiency plots
   if(do_resolutionPlot && !do_efficiencyPlotOnly && !do_scanThrs){
      producePlot("Resolution", fileName, sigma, sigma_error, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, do_autoScale, ETAranges_EB, ETAranges_EE, ETranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
   }

   if(do_scalePlot && !do_efficiencyPlotOnly && !do_scanThrs){
      producePlot("Scale", fileName, mean, mean_error, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, do_autoScale, ETAranges_EB, ETAranges_EE, ETranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
   }

   if((do_efficiencyPlot || do_efficiencyPlotOnly) && !do_scanThrs){
      vector<map<TString, map<TString, Float_t>>> map_dummy(2);
      vector<map<TString, map<TString, vector<Float_t>>>> map_error_dummy(2);
      producePlot("Efficiency", fileName, map_dummy, map_error_dummy, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, do_autoScale, ETAranges_EB, ETAranges_EE, ETranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
      if(do_fakeRatePlot){
         producePlot("FakeRate", fileName, map_dummy, map_error_dummy, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, do_autoScale, ETAranges_EB, ETAranges_EE, ETranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
      }
   }

   if(do_scanThrs){
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
      if(do_resolutionPlot && !do_efficiencyPlotOnly){
         produceScanPlots("Resolution", fileName, sigma, sigma_error, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, do_autoScale, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
      }
      if(do_scalePlot && !do_efficiencyPlotOnly){
         produceScanPlots("Scale", fileName, mean, mean_error, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, do_autoScale, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
      }
      if(do_efficiencyPlot || do_efficiencyPlotOnly){
         vector<map<TString, map<TString, Float_t>>> map_dummy(999);
         vector<map<TString, map<TString, vector<Float_t>>>> map_error_dummy(999);
         produceScanPlots("Efficiency", fileName, map_dummy, map_error_dummy, do_ratioPlot, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, do_autoScale, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency);
      }
   }
}



