//***********************************
//
//This function is the implementation of a fitting routine using roofit
//
//***********************************

#include "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/objects.h"
#include "/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/utils.C"

#include "TKey.h"

using namespace RooFit;

FitParameters performFit(string fileName, 
      string outputdir, 
      Int_t kEvents, 
      vector<TString> ETranges, 
      vector<TString> ETAranges, 
      map<TString, Edges> ETvalue, 
      map<TString, Edges> ETAvalue,
      TString PFRecHit_thrs,
      TString seeding_thrs,
      FlagList flagList, 
      string do_where){

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
   // struct defined in objects.h
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

   // for the chi2
   map<TString, map<TString, Float_t>> map_chisquare;

   // for the RMS (of the distribution - without fit)
   map<TString, map<TString, Float_t>> map_rms;
   map<TString, map<TString, Float_t>> map_rms_error;
   

   TFile* inputFile = 0;
   TString name_tmp = fileName.c_str();
   if(do_EB){
      inputFile = TFile::Open("../Analyzer/outputfiles/" + name_tmp + "_EB.root");
   }else if(do_EE){
      inputFile = TFile::Open("../Analyzer/outputfiles/" + name_tmp + "_EE.root");
   }

   fitParameters.inputFile = inputFile->GetName();

   // ranges of the distribution
   Double_t rangeMin = 0.;
   Double_t rangeMax = 2.;

   // we loop on the different ET and ETA ranges
   for(unsigned int i(0); i<ETranges.size(); ++i){
      for(unsigned int j(0); j<ETAranges.size(); ++j){
         // we get the necessary files and histograms

         TString histo_name;
         if(use_energy && do_binningEt){
            histo_name = "h_PFclusters_caloMatched_eOverEtrue_Eta" + ETAranges[j] + "_Et" + ETranges[i];
         }
         else if(use_energy && do_binningEn){
            histo_name = "h_PFclusters_caloMatched_eOverEtrue_Eta" + ETAranges[j] + "_En" + ETranges[i];
         }
         else if(use_simEnergy && do_binningEt){
            histo_name = "h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + ETAranges[j] + "_Et" + ETranges[i];
         }
         else if(use_simEnergy && do_binningEn){
            histo_name = "h_PFclusters_caloMatched_eOverEtrue_simEnergy_Eta" + ETAranges[j] + "_En" + ETranges[i];
         }
         
         inputFile->cd("EtEta_binned");
         TDirectory *subDir = gDirectory;

         TH1D* dmhist = 0;
         if(subDir->GetListOfKeys()->Contains(histo_name)){
            dmhist = (TH1D*) subDir->Get(histo_name)->Clone("dmhist");
         }
         else{
            cout << "ERROR: Couldn't find histogram with EnRange " << ETranges[i] << " and EtaRange " << ETAranges[j] << endl;
            cout << "Please make sure that the given bin was included in the PFClusterAnalyzer.C" << endl;
            cout << "Or check the input bin in EoverEtrue.C" << endl;
            cout << "--> Aborting" << endl;
            exit(11);
         }

         // get the standard deviation and RMS of the files
         float rms = dmhist->GetRMS();
         float rms_error = dmhist->GetRMSError();

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
         float mean_init, mean_min;
         if(ETAvalue[ETAranges[j]].first >= 2.0){
            mean_init = 0.9*dmhist->GetMean();
         }
         else{
            mean_init = dmhist->GetMean();
         }

         if(mean_init<1.0){
            mean_min = 0.8*dmhist->GetMean();
         }
         else{
            mean_min = 0.5*dmhist->GetMean();
         }

         //float mean_min = 0.8*dmhist->GetMean();
         float mean_max = 1.2*dmhist->GetMean();

         float sigma_init; 
         float sigma_min = 0.01*dmhist->GetStdDev();
         float sigma_max; 

         //if(mean_init<1.96){
         //   sigma_init = dmhist->GetStdDev();
         //   sigma_max = 1.6*dmhist->GetStdDev();
         //}
         //else{
         //   sigma_init = dmhist->GetStdDev()/4;
         //   sigma_max = 1.8*dmhist->GetStdDev()/4;
         //}


         float c1, c2;
         if(PFRecHit_thrs == "pfrh4.0" && seeding_thrs == "seed4.0"){
            c1 = 2.3;
            c2 = 1.98;
         }
         else if(PFRecHit_thrs == "pfrh3.0" && seeding_thrs == "seed4.0"){
            c1 = 2.3;
            c2 = 1.98;
         }
         else if(PFRecHit_thrs == "pfrh3.0" && seeding_thrs == "seed3.0"){
            c1 = 2.2;
            c2 = 2.25;
         }
         else if (PFRecHit_thrs == "pfrh2.0" && seeding_thrs == "seed4.0"){
            c1 = 2.2;
            c2 = 2.05;
         }
         else if (PFRecHit_thrs == "pfrh2.0" && seeding_thrs == "seed3.0"){
            c1 = 2.2;
            c2 = 2.2;
         }
         else if (PFRecHit_thrs == "pfrh2.0" && seeding_thrs == "seed2.0"){
            c1 = 2.3;
            c2 = 2.0;
         }
         else{
            c1 = 2.3;
            c2 = 1.98;
         }

         if (ETAvalue[ETAranges[j]].first >= 2.0 && ETvalue[ETranges[i]].second <= 20){
            sigma_init = dmhist->GetStdDev()/c1;
            sigma_max = c2*dmhist->GetStdDev()/c1;
         }
         else{
            sigma_init = 0.8*dmhist->GetStdDev();
            sigma_max = 1.6*dmhist->GetStdDev();
         } 

         //if (ETAvalue[ETAranges[j]].first >= 2.0){
         //   sigma_init = dmhist->GetStdDev()/3;
         //   sigma_max = 1.8*dmhist->GetStdDev()/3;
         //}
         //else{
         //   sigma_init = dmhist->GetStdDev();
         //   sigma_max = 1.6*dmhist->GetStdDev();
         //}


         float alpha_1_init = 10;
         float alpha_1_min = -10;
         float alpha_1_max = 10;

         float alpha_2_init = 1;
         float alpha_2_min = -10;
         float alpha_2_max = 10;

         float n_1_min = 0;
         float n_1_max = 50;

         float n_2_min = 0;
         float n_2_max = 35;


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
         label->AddText(PFRecHit_thrs + " " + seeding_thrs);
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

         map_chisquare[ETranges[i]][ETAranges[j]] = chisquare;
         
         map_rms[ETranges[i]][ETAranges[j]] = rms;
         map_rms_error[ETranges[i]][ETAranges[j]] = rms_error;
      }
   }

   fitParameters.map_sigma       = map_sigma;
   fitParameters.map_sigma_error = map_sigma_error;
   fitParameters.map_mean        = map_mean;
   fitParameters.map_mean_error  = map_mean_error;
   fitParameters.map_chisquare   = map_chisquare;
   fitParameters.map_rms         = map_rms; 
   fitParameters.map_rms_error   = map_rms_error; 

   return fitParameters;

}









