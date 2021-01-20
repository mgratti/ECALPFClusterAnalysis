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

#include "./macros/objects.h"
#include "./macros/utils.h"
#include "./macros/performFit.C"
#include "./macros/producePlot.C"
#include "./macros/produceScanPlots.C"
#include "./macros/getFile.C"
#include "./macros/getBoundaries.C"
#include "./macros/plotterInit.C"


using namespace RooFit ;
using namespace std;


/**********************************

  - This script allows to 
  - perform the fit of Ereco/Esim looping of files and save the fit parameters in a txt file
  - produce the reso/scale/efficiency/fakeRate plots, with and without ratio
  - produce the scan thresholds plots, with and without ratio


 **********************************/



void EoverEtrue_fit(TString fineBinning_energy, 
      TString fineBinning_eta, 
      TString simEnergy, 
      TString binningEt, 
      TString CBfit, 
      TString doubleCBfit, 
      TString BGfit, 
      TString fitPeak, 
      TString resolutionPlot, 
      TString scalePlot, 
      TString efficiencyPlot, 
      TString fakeRatePlot, 
      TString efficiencyPlotOnly, 
      TString ratioPlot, 
      TString scanThrs, 
      TString autoScale, 
      TString writeFiles){

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

   Bool_t do_binningEn = true;
   if(do_binningEt) do_binningEn = false;

   Bool_t do_fitAll = true;
   if(do_fitPeak) do_fitAll = false;

   Bool_t use_energy = false; 
   if(!use_simEnergy) use_energy = true;

   // we gather all the flags together
   FlagList flagList = {use_energy, use_simEnergy, do_binningEt, do_binningEn, do_CBfit, do_doubleCBfit, do_BGfit, do_fitAll, do_fitPeak};

   // variable declaration
   vector<Bool_t> do_EB, do_EE;
   vector<Bool_t> do_0to20GeV, do_0to100GeV, do_0p1to200GeV;
   vector<Int_t> kEvents;
   vector<TString> matching, PUtag, dependency;
   vector<TString> pfrechit_thrs, seeding_thrs;

   // initialization
   plotterInit(fileName, do_EB, do_EE, matching, PUtag, pfrechit_thrs, seeding_thrs, dependency, kEvents, do_0to20GeV, do_0to100GeV, do_0p1to200GeV);

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
            //ETranges.push_back({"10_15"});
         }
      }
      else if(do_0p1to200GeV[iFile]){
         if(!do_fineBinning_energy){
            ETranges.push_back({"1_20", "20_40", "40_60", "60_80", "80_100", "100_120", "120_140", "140_160", "160_180", "180_200"});
         }
         else{
            ETranges.push_back({"1_5", "5_10", "10_15", "15_20", "20_40", "40_60", "60_80", "80_100", "100_120", "120_140", "140_160", "160_180", "180_200"});
            //ETranges.push_back({"60_80"});
         }
      }
   }

   vector<TString> ETAranges_EB;
   if(!do_fineBinning_eta){
      ETAranges_EB = {"0p00_0p50", "0p50_1p00", "1p00_1p48"};
   }
   else{
      ETAranges_EB = {"0p00_0p40", "0p40_0p80", "0p80_1p00", "1p00_1p20", "1p20_1p44", "1p44_1p48"};
      //ETAranges_EB = {"0p40_0p80"};
   }

   vector<TString> ETAranges_EE;
   if(!do_fineBinning_eta){
      ETAranges_EE = {"1p48_2p00", "2p00_2p50", "2p50_3p00"};
   }
   else{
      ETAranges_EE = {"1p48_1p64", "1p64_1p85", "1p85_2p00", "2p00_2p20", "2p20_2p40", "2p40_2p60", "2p60_2p80", "2p80_3p00"};
      //ETAranges_EE = {"2p00_2p20"};
   }

   // and adjust the boundaries
   map<TString, Edges> ETvalue;
   map<TString, Edges> ETAvalue;

   getBoundaries(ETvalue, ETAvalue);

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
   vector<string> outputdir;
   for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
      string outputdir_tmp = "./myPlots/fits/" + fileName[iFile];
      if(do_binningEt){
         outputdir_tmp += "_EtaEtBinned";
      }
      else if(do_binningEn){
         outputdir_tmp += "_EtaEnBinned";
      }
      if(do_CBfit){
         outputdir_tmp += "_CB";
      }
      else if(do_doubleCBfit){
         outputdir_tmp += "_doubleCB";
      }
      if(do_BGfit){
         outputdir_tmp += "_BG";
      }

      if(use_simEnergy){
         outputdir_tmp += "_simEnergy";
      }
      outputdir_tmp += "/";

      if(do_ratioPlot){
         outputdir_tmp += "ratio/" + fileName[1] + "/";
      }

      outputdir.push_back(outputdir_tmp);

      // create the output directory
      system(Form("mkdir -p %s", outputdir_tmp.c_str()));
   }

   // struct that will get the fit parameters
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

   // we write in a txt file the parameters of the fit for each file
   if(do_writeFiles){
      for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
         ifstream file("samples/" + fileName[iFile] + ".txt");

         if(!file.is_open()){
            if(do_EB[iFile]){
               fitParameters_EB = performFit(fileName[iFile], outputdir[iFile], kEvents[iFile], ETranges[iFile], ETAranges_EB, ETvalue, ETAvalue, pfrechit_thrs[iFile], seeding_thrs[iFile], flagList, "EB");
               getFile(fileName[iFile], true, fitParameters_EB.map_sigma, fitParameters_EB.map_sigma_error, fitParameters_EB.map_mean, fitParameters_EB.map_mean_error, fitParameters_EB.map_chisquare, fitParameters_EB.map_rms, fitParameters_EB.map_rms_error, ETranges[iFile], ETAranges[iFile]);
            }
            else{
               fitParameters_EE = performFit(fileName[iFile], outputdir[iFile], kEvents[iFile], ETranges[iFile], ETAranges_EE, ETvalue, ETAvalue, pfrechit_thrs[iFile], seeding_thrs[iFile], flagList, "EE");
               getFile(fileName[iFile], false, fitParameters_EE.map_sigma, fitParameters_EE.map_sigma_error, fitParameters_EE.map_mean, fitParameters_EE.map_mean_error, fitParameters_EE.map_chisquare, fitParameters_EE.map_rms, fitParameters_EE.map_rms_error, ETranges[iFile], ETAranges[iFile]);
            }
         }
         else{
            cout << "Sample file " << fileName[iFile] << " already processed" << endl;
            cout << "--> Skipping " << endl;
         }
      }
   }


   // we produce the different plots
   if(!do_writeFiles){
      if(!do_efficiencyPlotOnly){
         for(unsigned int iFile(0); iFile<fileName.size(); ++iFile){
            if(do_EB[iFile]){
               fitParameters_EB = performFit(fileName[iFile], outputdir[iFile], kEvents[iFile], ETranges[iFile], ETAranges_EB, ETvalue, ETAvalue, pfrechit_thrs[iFile], seeding_thrs[iFile], flagList, "EB");
               sigma.push_back(fitParameters_EB.map_sigma);
               sigma_error.push_back(fitParameters_EB.map_sigma_error);
               mean.push_back(fitParameters_EB.map_mean);
               mean_error.push_back(fitParameters_EB.map_mean_error);
               chisquare.push_back(fitParameters_EB.map_chisquare);
            }
            else{
               fitParameters_EE = performFit(fileName[iFile], outputdir[iFile], kEvents[iFile], ETranges[iFile], ETAranges_EE, ETvalue, ETAvalue, pfrechit_thrs[iFile], seeding_thrs[iFile], flagList, "EE");
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



