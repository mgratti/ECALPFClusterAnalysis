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

#include <iostream>
#include <iomanip>
#include <math.h>

using namespace RooFit ;
using namespace std;

/*
   The main functions of this script are the following:
   1. This script enables to fit the E/Etrue distribution.The residuals are plotted and the chi square is computed.
   2. Produce the scale, resolution and efficiency plots
   */



//////////////////////////////////////////////////////////////////////
//                    User's decision board                         //

// enter the file name
string fileName = "histo_photon_E0.1to200GeV_closeEcal_EE_noPU_pfrh1.0_seed3.0_V01_v61_n30000_simFraction";

// enter the number of k events
Int_t kEvents = 300;

// choose which energy range you are using (choose only one)
Bool_t do_0to20GeV  = false;
Bool_t do_0to100GeV = true;

// choose between endcap and/or barrel
Bool_t do_EB = false;
Bool_t do_EE = true;

// choose which Etrue definition you want to use (choose only one)
Bool_t use_energy    = false;
Bool_t use_simEnergy = true;

// choose whether you want to bin in ET or energy
Bool_t do_binningEt = false;
Bool_t do_binningEn = true;

// choose whether to use a finner binning or not
Bool_t do_fineBinning_energy = false;
Bool_t do_fineBinning_eta    = true;


// choose one of the following fit (Crystal Ball, double-sided Crystal Ball or Bifurcated Gaussian)
Bool_t do_CBfit       = false; 
Bool_t do_doubleCBfit = true;
Bool_t do_BGfit       = false;

// choose between fitting the whole distribution or the peak only
Bool_t do_fitAll  = true;
Bool_t do_fitPeak = false;

// choose which plots to produce
Bool_t do_resolutionPlot = true;
Bool_t do_scalePlot      = true;
Bool_t do_efficiencyPlot = true;


// choose whether to produce only the efficiency plot or not
Bool_t do_efficiencyPlotOnly = false;

//////////////////////////////////////////////////////////////////////



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
   string outputdir;
   map<TString, map<TString, Float_t>> map_sigma;
   map<TString, map<TString, vector<Float_t>>> map_sigma_error;
   map<TString, map<TString, Float_t>> map_mean;
   map<TString, map<TString, vector<Float_t>>> map_mean_error;
};


// additional functions
FitParameters performFit(string fileName, Int_t kEvents, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, FlagList flaglist, string);
// converts double to string with adjusting the number of decimals
TString getString(Float_t num, int decimal = 0);
// produce the resolution and scale plots
void producePlots(TString, Bool_t, vector<map<TString, map<TString, Float_t>>>, vector<map<TString, map<TString, vector<Float_t>>>>, vector<TString>, vector<TString>, map<TString, Edges>, map<TString, Edges>, map<int, EColor>, string, Int_t, TString);
// produce the efficiency plots
void produceEfficiencyPlot(string, Bool_t, Bool_t, Bool_t, Bool_t, vector<TString>, vector<TString>, map<TString, Edges>, map<TString, Edges>, map<int, EColor> color, string, Int_t, TString);


// main function
void EoverEtrue_fit(){

   FlagList flagList = {use_energy, use_simEnergy, do_binningEt, do_binningEn, do_CBfit, do_doubleCBfit, do_BGfit, do_fitAll, do_fitPeak};

   // define the different Et and Eta slots
   vector<TString> ETranges;
   if(do_0to20GeV){
      ETranges = {"0_5", "5_10", "10_15", "15_20"};
   }
   else if(do_0to100GeV){
      if(!do_fineBinning_energy){
         ETranges = {"1_20", "20_40", "40_60", "60_80", "80_100"};
      }
      else{
         ETranges = {"0_5", "5_10", "10_15", "15_20", "20_40", "40_60", "60_80", "80_100"};
      }
   }
   vector<TString> ETAranges_EB;
   if(!do_fineBinning_eta){
      ETAranges_EB = {"0p00_0p50", "0p50_1p00", "1p00_1p48"};
   }
   else{
      ETAranges_EB = {"0p00_0p20", "0p20_0p40", "0p40_0p60", "0p60_0p80", "0p80_1p00", "1p00_1p20", "1p20_1p44", "1p44_1p48"};
   }


   vector<TString> ETAranges_EE;
   if(!do_fineBinning_eta){
      ETAranges_EE = {"1p48_2p00", "2p00_2p50", "2p50_3p00"};
   }
   else{
      ETAranges_EE = {"1p48_1p57", "1p57_1p65", "1p65_1p85", "1p85_2p00", "2p00_2p20", "2p20_2p40", "2p40_2p60", "2p60_2p80", "2p80_3p00"};
   }


   map<TString, Edges> ETvalue;
   map<TString, Edges> ETAvalue;

   // and adjust the boundaries
   ETvalue["0_5"].first = 0.;
   ETvalue["0_5"].second = 5.;
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

   ETAvalue["0p00_0p20"].first = 0.;
   ETAvalue["0p00_0p20"].second = 0.2;
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
   ETAvalue["1p57_1p65"].first = 1.566;
   ETAvalue["1p57_1p65"].second = 1.653;
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
   color[1]=kRed;
   color[2]=kMagenta;
   color[3]=kPink;
   color[4]=kGreen;
   color[5]=kCyan;
   color[6]=kBlue;
   color[7]=kViolet;
   color[8]=kBlack;

   // we get the matching strategy from the fileName
   TString matching;
   if(fileName.find("numberOfHits") != std::string::npos){
      matching = "numberOfHits";
   }
   else if(fileName.find("simFraction") != std::string::npos){
      matching = "simFraction";
   }
   else if(fileName.find("deltaR") != std::string::npos){
      matching = "deltaR";
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
   string outputdir;

   if(!do_efficiencyPlotOnly){
      if(do_EB){
         fitParameters_EB = performFit(fileName, kEvents, ETranges, ETAranges_EB, ETvalue, ETAvalue, flagList, "EB");
      }
      if(do_EE){
         fitParameters_EE = performFit(fileName, kEvents, ETranges, ETAranges_EE, ETvalue, ETAvalue, flagList, "EE");
      }

      if(do_EB){
         sigma.push_back(fitParameters_EB.map_sigma);
         sigma_error.push_back(fitParameters_EB.map_sigma_error);
         mean.push_back(fitParameters_EB.map_mean);
         mean_error.push_back(fitParameters_EB.map_mean_error);

         if(do_EE){
            sigma.push_back(fitParameters_EE.map_sigma);
            sigma_error.push_back(fitParameters_EE.map_sigma_error);
            mean.push_back(fitParameters_EE.map_mean);
            mean_error.push_back(fitParameters_EE.map_mean_error);
         }
      }
      else{
         sigma.push_back(fitParameters_EE.map_sigma);
         sigma_error.push_back(fitParameters_EE.map_sigma_error);
         mean.push_back(fitParameters_EE.map_mean);
         mean_error.push_back(fitParameters_EE.map_mean_error);
      }

      // we get the other elements needed to produce the resolution/scale/efficiency plots
      if(do_EB){
         input.push_back(fitParameters_EB.inputFile);
         outputdir = fitParameters_EB.outputdir;
         if(do_EE){
            input.push_back(fitParameters_EE.inputFile);
         }
      }
      else{
         input.push_back(fitParameters_EE.inputFile);
         outputdir = fitParameters_EE.outputdir;
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
   if(do_resolutionPlot && !do_efficiencyPlotOnly){
      producePlots("resolution", flagList.do_binningEt, sigma, sigma_error, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching);
   }

   if(do_scalePlot && !do_efficiencyPlotOnly){
      producePlots("scale", flagList.do_binningEt, mean, mean_error, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching);
   }

   if(do_efficiencyPlot || do_efficiencyPlotOnly){
      produceEfficiencyPlot(fileName, do_EB, do_EE, flagList.do_binningEt, flagList.use_simEnergy, ETranges, ETAranges, ETvalue, ETAvalue, color, outputdir, kEvents, matching);
   }


}




FitParameters performFit(string fileName, Int_t kEvents, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, FlagList flagList, string do_where){

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
   if(do_EB==true){
      inputFile = TFile::Open("../Analyzer/outputfiles/" + name_tmp + "_EB.root");
   }
   else if(do_EE==true){
      inputFile = TFile::Open("../Analyzer/outputfiles/" + name_tmp + "_EE.root");
   }

   fitParameters.inputFile = inputFile->GetName();

   // define the output directory
   string outputdir = "myPlots/fits/" + fileName;

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

   fitParameters.outputdir = outputdir;

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


         //Define the PDF to fit: 

         // crystal ball (gaussian + exponential decaying tails)
         // we declare all the parameters needed for the fits	
         RooRealVar *mean   = new RooRealVar("mean","mean",1.015,0.9,1.1);
         RooRealVar *sigma  = new RooRealVar("sigma","sigma",0.027, 0.0, 0.05);
         RooRealVar *alpha  = new RooRealVar("alpha", "alpha", 1., 0, 2.);
         RooRealVar *n      = new RooRealVar("n", "n", 1., 0., 10.);

         RooCBShape *CBpdf = new RooCBShape("CBpdf", "CBpdf", *EoverEtrue, *mean, *sigma, *alpha, *n);


         // double crystal ball (same gaussian body but different exponential tails)
         RooRealVar *alpha_1  = new RooRealVar("alpha_1", "alpha_1", 3.0, -15, 15.);
         RooRealVar *n_1      = new RooRealVar("n_1", "n_1", 5., 0., 50.);
         RooRealVar *alpha_2  = new RooRealVar("alpha_2", "alpha_2", -2.0, -15, 15.);
         RooRealVar *n_2      = new RooRealVar("n_2", "n_2", 1., 0., 50.);

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
         if(do_CBfit==true){
            if(do_fitAll){
               result = CBpdf->fitTo(*rdh);
            }
            else if(do_fitPeak){
               if(use_energy){
                  EoverEtrue->setRange("peak",  0.6, 1.1);
               }
               else if(use_simEnergy){
                  EoverEtrue->setRange("peak",  0.9, 1.4);
               }
               result = CBpdf->fitTo(*rdh, Range("peak"));
            }      
         }
         else if(do_doubleCBfit){
            if(do_fitAll){
               result = doubleCBpdf->fitTo(*rdh);
            }
            else if(do_fitPeak){
               EoverEtrue->setRange("peak",  0.9, 1.1);
               result = doubleCBpdf->fitTo(*rdh, Range("peak"));
            }      
         }
         else if(do_BGfit==true){
            if(do_fitAll){
               result = BGpdf->fitTo(*rdh);
            }
            else if(do_fitPeak){
               EoverEtrue->setRange("peak",  0.6, 1.1);
               result = BGpdf->fitTo(*rdh, Range("peak"));
            }      
         }


         // plot the fit 		
         if(do_CBfit==true){
            CBpdf->plotOn(frame,LineColor(4),RooFit::Name("CBpdf"),Components("CBpdf"));
         }
         if(do_doubleCBfit==true){
            doubleCBpdf->plotOn(frame,LineColor(4),RooFit::Name("doubleCBpdf"),Components("doubleCBpdf"));
         }
         else if(do_BGfit==true){
            BGpdf->plotOn(frame,LineColor(4),RooFit::Name("BGpdf"),Components("BGpdf"));
         }


         // and write the fit parameters
         if(do_CBfit==true){
            CBpdf->paramOn(frame,   
                  Layout(0.2, 0.4, 0.8),
                  Format("NEU",AutoPrecision(1))
                  );
         }
         else if(do_doubleCBfit==true){
            doubleCBpdf->paramOn(frame,   
                  Layout(0.2, 0.4, 0.8),
                  Format("NEU",AutoPrecision(1))
                  );
         }
         else if(do_BGfit==true){
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
         label->AddText(kevt + "k events");
         if(do_binningEt){
            label->AddText(getString(ETvalue[ETranges[i]].first, 0) + " < E_{T} < " + getString(ETvalue[ETranges[i]].second, 0) + "  GeV");
         }
         else if(do_binningEn){
            label->AddText(getString(ETvalue[ETranges[i]].first, 0) + " < E < " + getString(ETvalue[ETranges[i]].second, 0) + "  GeV");
         }
         label->AddText(getString(ETAvalue[ETAranges[j]].first, 2) + " < #eta < " + getString(ETAvalue[ETAranges[j]].second, 2));

         // we compute the chisquare
         Double_t chisquare;
         if(do_CBfit==true){
            chisquare = frame->chiSquare("CBpdf","data");
         }
         else if(do_doubleCBfit==true){
            chisquare = frame->chiSquare("doubleCBpdf","data");
         }
         else if(do_BGfit==true){
            chisquare = frame->chiSquare("BGpdf","data");
         }

         // and print it
         TPaveText* label_2 = new TPaveText(0.62,0.65,0.72,0.8,"brNDC");
         label_2->SetBorderSize(0);
         label_2->SetFillColor(kWhite);
         label_2->SetTextSize(0.03);
         label_2->SetTextFont(42);
         label_2->SetTextAlign(11);
         if(do_CBfit==true){
            label_2->AddText("CrystalBall PDF");
         }
         else if(do_doubleCBfit==true){
            label_2->AddText("Double-Sided CrystalBall PDF");
         }
         else if(do_BGfit==true){
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

         // create the output directory
         system(Form("mkdir -p %s", outputdir.c_str()));

         // save output
         canv->SaveAs(outputdir + fileName + "_EoverEtrue_fit_Et_" + ETranges[i] + "_Eta_" + ETAranges[j] + ".png");

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








void producePlots(TString what, Bool_t do_binningEt, vector<map<TString, map<TString, Float_t>>> map_sigma, vector<map<TString, map<TString, vector<Float_t>>>> map_sigma_error, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, map<int, EColor> color, string outputdir, Int_t kEvents, TString matching){

   // we first produce the plot of the resolution as a function of the energy for different eta ranges
   TCanvas* c1 = new TCanvas("c1", "c1", 700, 600);
   TLegend* leg1 = new TLegend(0.55, 0.55, 0.75, 0.8);

   for(unsigned int kk(0); kk<ETAranges.size(); ++kk){
      Float_t x, resolution, error_hi, error_lo;
      TGraphAsymmErrors* graph = new TGraphAsymmErrors(0);
      if(what=="resolution"){
         graph->SetTitle("Resolution");
      }
      else if(what=="scale"){
         graph->SetTitle("Scale");
      }
      for(unsigned int ii(0); ii<ETranges.size(); ++ii){
         for(unsigned int iSigma(0); iSigma<map_sigma.size(); ++iSigma){
            x = (ETvalue[ETranges[ii]].first + ETvalue[ETranges[ii]].second)/2;     
            resolution = map_sigma[iSigma][ETranges[ii]][ETAranges[kk]];
            error_hi = map_sigma_error[iSigma][ETranges[ii]][ETAranges[kk]][0];
            if(resolution!=0){
               int thisPoint = graph->GetN();
               graph->SetPoint(thisPoint, x, resolution);
               graph->SetPointError(thisPoint, (ETvalue[ETranges[ii]].second - ETvalue[ETranges[ii]].first)/2, (ETvalue[ETranges[ii]].second - ETvalue[ETranges[ii]].first)/2, error_hi, error_hi);
            }
         }
      }

      if(what=="resolution"){
         graph->GetYaxis()->SetRangeUser(0, 0.1);
      }
      else if(what=="scale"){
         graph->GetYaxis()->SetRangeUser(0.7, 1.3);
      }
      graph->GetYaxis()->SetTitleSize(0.04);
      graph->GetYaxis()->SetTitleOffset(1.2);
      if(do_binningEt){
         graph->GetXaxis()->SetTitle("E_{T} [GeV]");
      }
      else{
         graph->GetXaxis()->SetTitle("E [GeV]");
      }
      graph->GetXaxis()->SetTitleSize(0.04);
      graph->GetXaxis()->SetTitleOffset(1.1);
      graph->SetLineColor(color[kk]);
      graph->SetMarkerColor(color[kk]);
      if(kk==0){
         graph->Draw("A*");
      }
      else{
         graph->Draw("*, same");
      }

      leg1 -> AddEntry(graph, ETAranges[kk]);
      leg1 -> SetTextSize(0.04);
      leg1 -> SetLineColor(0);
      leg1 -> SetFillColorAlpha(0, 0);
      leg1 -> SetBorderSize(0);
      leg1 -> Draw("same");

   }
   TPaveText* label = new TPaveText(0.55,0.83,0.75,0.86,"brNDC");
   label->SetBorderSize(0);
   label->SetFillColor(kWhite);
   label->SetTextSize(0.04);
   label->SetTextFont(42);
   label->SetTextAlign(11);
   label->AddText("#eta bins: ");
   label->Draw("same");

   TPaveText* label_info = new TPaveText(0.15,0.73,0.45,0.6,"brNDC");
   label_info->SetBorderSize(0);
   label_info->SetFillColor(kWhite);
   label_info->SetTextSize(0.034);
   label_info->SetTextFont(42);
   label_info->SetTextAlign(11);
   TString nEvents = to_string(kEvents);
   label_info->AddText(nEvents + "k Events");
   label_info->AddText("Matching: " + matching);
   label_info->Draw("same");

   TString dir = outputdir.c_str();

   if(what=="resolution"){
      c1->SaveAs(dir + "resolution_vs_energy.png");
   }
   else if(what=="scale"){
      c1->SaveAs(dir + "scale_vs_energy.png");
   }


   // we then produce the plot of the resolution as a function of eta for different energy ranges
   TCanvas* c2 = new TCanvas("c2", "c2", 700, 600);
   TLegend* leg2 = new TLegend(0.55, 0.6, 0.75, 0.8);


   for(unsigned int kk(0); kk<ETranges.size(); ++kk){
      Float_t x, resolution, error_hi, error_lo;
      TGraphAsymmErrors* graph = new TGraphAsymmErrors(0);
      if(what=="resolution"){
         graph->SetTitle("Resolution");
      }
      else if(what=="scale"){
         graph->SetTitle("Scale");
      }
      for(unsigned int ii(0); ii<ETAranges.size(); ++ii){
         for(unsigned int iSigma(0); iSigma<map_sigma.size(); ++iSigma){
            x = (ETAvalue[ETAranges[ii]].first + ETAvalue[ETAranges[ii]].second)/2;     
            resolution = map_sigma[iSigma][ETranges[kk]][ETAranges[ii]];
            error_hi = map_sigma_error[iSigma][ETranges[kk]][ETAranges[ii]][0];

            if(resolution!=0){
               int thisPoint = graph->GetN();
               graph->SetPoint(thisPoint, x, resolution);
               graph->SetPointError(thisPoint, (ETAvalue[ETAranges[ii]].second - ETAvalue[ETAranges[ii]].first)/2, (ETAvalue[ETAranges[ii]].second - ETAvalue[ETAranges[ii]].first)/2 ,error_hi, error_hi);
            }
         }
      }
      if(what=="resolution"){
         graph->GetYaxis()->SetRangeUser(0, 0.1);
      }
      else if(what=="scale"){
         graph->GetYaxis()->SetRangeUser(0.7, 1.3);
      }
      graph->GetYaxis()->SetTitleSize(0.04);
      graph->GetYaxis()->SetTitleOffset(1.2);
      graph->GetXaxis()->SetTitle("#eta");
      graph->GetXaxis()->SetTitleSize(0.04);
      graph->GetXaxis()->SetTitleOffset(1.1);
      graph->SetLineColor(color[kk]);
      graph->SetMarkerColor(color[kk]);
      if(kk==0){
         graph->Draw("A*");
      }
      else{
         graph->Draw("*, same");
      }

      leg2 -> AddEntry(graph, ETranges[kk]);
      leg2 -> SetTextSize(0.04);
      leg2 -> SetLineColor(0);
      leg2 -> SetFillColorAlpha(0, 0);
      leg2 -> SetBorderSize(0);
      leg2 -> Draw("same");

   }

   TPaveText* label2 = new TPaveText(0.55,0.8,0.75,0.83,"brNDC");
   label2->SetBorderSize(0);
   label2->SetFillColor(kWhite);
   label2->SetTextSize(0.04);
   label2->SetTextFont(42);
   label2->SetTextAlign(11);
   if(do_binningEt){
      label2->AddText("E_{T} bins: ");
   }
   else{
      label2->AddText("Energy bins: ");
   }
   label2->Draw("same");
   label_info->Draw("same");


   if(what=="resolution"){
      c2->SaveAs(dir + "resolution_vs_eta.png");
   }
   else if(what=="scale"){
      c2->SaveAs(dir + "scale_vs_eta.png");
   }

   delete c1;
   delete c2;
}




void produceEfficiencyPlot(string fileName, Bool_t do_EB, Bool_t do_EE, Bool_t do_binningEt, Bool_t use_simEnergy, vector<TString> ETranges, vector<TString> ETAranges, map<TString, Edges> ETvalue, map<TString, Edges> ETAvalue, map<int, EColor> color, string outputdir, Int_t kEvents, TString matching){

   // we first produce the plot of the efficiency as a function of the energy for different eta ranges
   TCanvas* c1 = new TCanvas("c1", "c1", 700, 600);
   TLegend* leg1 = new TLegend(0.55, 0.55, 0.75, 0.8);

   vector<TString> input;
   TString filename = fileName.c_str();
   if(do_EB==true){
      input.push_back("../Analyzer/outputfiles/" + filename + "_EB.root");
   }

   if(do_EE==true){
      input.push_back("../Analyzer/outputfiles/" + filename + "_EE.root");
   }


   for(unsigned int kk(0); kk<ETAranges.size(); ++kk){
      Float_t x, efficiency, error;
      TGraphAsymmErrors* graph = new TGraphAsymmErrors(0);
      graph->SetTitle("Efficiency");

      for(unsigned int ii(0); ii<ETranges.size(); ++ii){
         for(unsigned int iFile(0); iFile<input.size(); ++iFile){
            TFile* inputFile = TFile::Open(input[iFile]);
            if(input[iFile].Contains("EB") && ETAvalue[ETAranges[kk]].first>1.4) continue;
            if(input[iFile].Contains("EE") && ETAvalue[ETAranges[kk]].first<1.4) continue;

            TH1D* hist_num  = 0;
            TH1D* hist_deno = 0;
            if(do_binningEt && !use_simEnergy){
               hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[kk] + "_Et" + ETranges[ii] + "_forEfficiency")->Clone("hist_num");

               hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[kk] + "_Et" + ETranges[ii])->Clone("hist_deno");
            }
            else if(!do_binningEt && !use_simEnergy){
               hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[kk] + "_En" + ETranges[ii] + "_forEfficiency")->Clone("hist_num");
               hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[kk] + "_En" + ETranges[ii])->Clone("hist_deno");
            }
            if(do_binningEt && use_simEnergy){
               hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[kk] + "_Et" + ETranges[ii] + "_forEfficiency")->Clone("hist_num");
               hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[kk] + "_Et" + ETranges[ii])->Clone("hist_deno");
            }
            else if(!do_binningEt && use_simEnergy){
               hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[kk] + "_En" + ETranges[ii] + "_forEfficiency")->Clone("hist_num");
               hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[kk] + "_En" + ETranges[ii])->Clone("hist_deno");
            }


            x = (ETvalue[ETranges[ii]].first + ETvalue[ETranges[ii]].second)/2;     
            efficiency = hist_num->GetEntries()/hist_deno->GetEntries();
            cout << "efficiency: " << efficiency << endl;
            error = efficiency*(sqrt(hist_num->GetEntries())/hist_num->GetEntries() + sqrt(hist_deno->GetEntries())/hist_deno->GetEntries());
            int thisPoint = graph->GetN();
            graph->SetPoint(thisPoint, x, efficiency);
            graph->SetPointError(thisPoint, (ETvalue[ETranges[ii]].second - ETvalue[ETranges[ii]].first)/2, (ETvalue[ETranges[ii]].second - ETvalue[ETranges[ii]].first)/2, error/2, error/2);
         }
      }
      graph->GetYaxis()->SetRangeUser(0.4, 1.5);
      graph->GetYaxis()->SetTitleSize(0.04);
      graph->GetYaxis()->SetTitleOffset(1.2);
      if(do_binningEt){
         graph->GetXaxis()->SetTitle("E_{T} [GeV]");
      }
      else{
         graph->GetXaxis()->SetTitle("E [GeV]");
      }
      graph->GetXaxis()->SetTitleSize(0.04);
      graph->GetXaxis()->SetTitleOffset(1.1);
      graph->SetLineColor(color[kk]);
      graph->SetMarkerColor(color[kk]);
      if(kk==0){
         graph->Draw("A*");
      }
      else{
         graph->Draw("*, same");
      }

      leg1 -> AddEntry(graph, ETAranges[kk]);
      leg1 -> SetTextSize(0.04);
      leg1 -> SetLineColor(0);
      leg1 -> SetFillColorAlpha(0, 0);
      leg1 -> SetBorderSize(0);
      leg1 -> Draw("same");

   }

   TPaveText* label = new TPaveText(0.55,0.83,0.75,0.86,"brNDC");
   label->SetBorderSize(0);
   label->SetFillColor(kWhite);
   label->SetTextSize(0.04);
   label->SetTextFont(42);
   label->SetTextAlign(11);
   label->AddText("#eta bins: ");
   label->Draw("same");

   TPaveText* label_info = new TPaveText(0.15,0.73,0.45,0.6,"brNDC");
   label_info->SetBorderSize(0);
   label_info->SetFillColor(kWhite);
   label_info->SetTextSize(0.034);
   label_info->SetTextFont(42);
   label_info->SetTextAlign(11);
   TString nEvents = to_string(kEvents);
   label_info->AddText(nEvents + "k Events");
   label_info->AddText("Matching: " + matching);
   label_info->Draw("same");

   TString dir = outputdir.c_str();

   c1->SaveAs(dir + "efficiency_vs_energy.png");



   // we then produce the plot of the efficiency as a function of eta for different energy ranges
   TCanvas* c2 = new TCanvas("c2", "c2", 700, 600);
   TLegend* leg2 = new TLegend(0.55, 0.6, 0.75, 0.8);

   TH1D* hist_num = 0;
   TH1D* hist_deno = 0;

   TFile* inputFile = 0;
   TString name_tmp = fileName.c_str();
   if(do_EB==true){
      inputFile = TFile::Open("../Analyzer/outputfiles/" + name_tmp + "_EB.root");
   }
   else if(do_EE==true){
      inputFile = TFile::Open("../Analyzer/outputfiles/" + name_tmp + "_EE.root");
   }


   for(unsigned int kk(0); kk<ETranges.size(); ++kk){

      Float_t x, efficiency, error; 
      TGraphAsymmErrors* graph = new TGraphAsymmErrors(0);
      graph->SetTitle("Efficiency");
      for(unsigned int ii(0); ii<ETAranges.size(); ++ii){
         for(unsigned int iFile(0); iFile<input.size(); ++iFile){
            TFile* inputFile = TFile::Open(input[iFile]);
            if(input[iFile].Contains("EB") && ETAvalue[ETAranges[ii]].first>1.4) continue;
            if(input[iFile].Contains("EE") && ETAvalue[ETAranges[ii]].first<1.4) continue;

            if(do_binningEt && !use_simEnergy){
               hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[ii] + "_Et" + ETranges[kk] + "_forEfficiency")->Clone("hist_num");
               hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[ii] + "_Et" + ETranges[kk])->Clone("hist_deno");
            }
            else if(!do_binningEt && !use_simEnergy){
               hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_Eta" + ETAranges[ii] + "_En" + ETranges[kk] + "_forEfficiency")->Clone("hist_num");
               hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_Eta" + ETAranges[ii] + "_En" + ETranges[kk])->Clone("hist_deno");
            } 
            if(do_binningEt && use_simEnergy){
               hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[ii] + "_Et" + ETranges[kk] + "_forEfficiency")->Clone("hist_num");
               hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[ii] + "_Et" + ETranges[kk])->Clone("hist_deno");
            }
            else if(!do_binningEt && use_simEnergy){
               hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[ii] + "_En" + ETranges[kk] + "_forEfficiency")->Clone("hist_num");
               hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[ii] + "_En" + ETranges[kk])->Clone("hist_deno");
            } 


            x = (ETAvalue[ETAranges[ii]].first + ETAvalue[ETAranges[ii]].second)/2;     
            efficiency = hist_num->GetEntries()/hist_deno->GetEntries();
            error = efficiency*(sqrt(hist_num->GetEntries())/hist_num->GetEntries() + sqrt(hist_deno->GetEntries())/hist_deno->GetEntries());

            int thisPoint = graph->GetN();
            graph->SetPoint(thisPoint, x, efficiency);
            graph->SetPointError(thisPoint, (ETAvalue[ETAranges[ii]].second - ETAvalue[ETAranges[ii]].first)/2, (ETAvalue[ETAranges[ii]].second - ETAvalue[ETAranges[ii]].first)/2, error/2, error/2);

         }
      }
      graph->GetYaxis()->SetRangeUser(0.4, 1.5);
      graph->GetYaxis()->SetTitleSize(0.04);
      graph->GetYaxis()->SetTitleOffset(1.2);
      graph->GetXaxis()->SetTitle("#eta");
      graph->GetXaxis()->SetTitleSize(0.04);
      graph->GetXaxis()->SetTitleOffset(1.1);
      graph->SetLineColor(color[kk]);
      graph->SetMarkerColor(color[kk]);
      if(kk==0){
         graph->Draw("A*");
      }
      else{
         graph->Draw("*, same");
      }

      leg2 -> AddEntry(graph, ETranges[kk]);
      leg2 -> SetTextSize(0.04);
      leg2 -> SetLineColor(0);
      leg2 -> SetFillColorAlpha(0, 0);
      leg2 -> SetBorderSize(0);
      leg2 -> Draw("same");

   }
   TPaveText* label2 = new TPaveText(0.55,0.8,0.75,0.83,"brNDC");
   label2->SetBorderSize(0);
   label2->SetFillColor(kWhite);
   label2->SetTextSize(0.04);
   label2->SetTextFont(42);
   label2->SetTextAlign(11);
   if(do_binningEt){
      label2->AddText("E_{T} bins: ");
   }
   else{
      label2->AddText("Energy bins: ");
   }
   label2->Draw("same");
   label_info->Draw("same");

   c2->SaveAs(dir + "efficiency_vs_eta.png");

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


