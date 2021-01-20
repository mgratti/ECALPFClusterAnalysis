
#include "objects.h"


PlottingTools getGraph(TString whichPlot, 
      string fileName, 
      map<TString, map<TString, Float_t>> map_quantity, 
      map<TString, map<TString, vector<Float_t>>> map_quantity_error, 
      unsigned int kk, 
      Bool_t printTitle, 
      Bool_t do_EB, 
      Bool_t do_EE, 
      Bool_t do_binningEt, 
      Bool_t use_simEnergy, 
      vector<TString> ETranges, 
      vector<TString> ETAranges, 
      map<TString, Edges> ETvalue, 
      map<TString, Edges> ETAvalue, 
      map<int, EColor> color, 
      string what){
   
   TString label = fileName.c_str();
   TString filename;
   if(do_EB){
      filename = "../Analyzer/outputfiles/" + label + "_EB.root";
   }
   else if(do_EE){
      filename = "../Analyzer/outputfiles/" + label + "_EE.root";
   }

   Float_t x, quantity, error, error_stat;
   TEfficiency* eff_error;

   vector<float> range_candidate(10);

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
         if(hist_deno->GetEntries()!=0){
            quantity = hist_num->GetEntries()/hist_deno->GetEntries();
         }
         //cout << "efficiency: " << quantity << endl;
         range_candidate.push_back(quantity);
         if(TEfficiency::CheckConsistency(*hist_num,*hist_deno)){
            eff_error = new TEfficiency(*hist_num, *hist_deno);
         }
         for(int i(0); i<hist_num->GetNbinsX(); ++i){
            error_tmp += eff_error->GetEfficiencyErrorLow(i);
         }
         error = error_tmp;
         error_stat = quantity*(sqrt(hist_num->GetEntries())/hist_num->GetEntries() + sqrt(hist_deno->GetEntries())/hist_deno->GetEntries());
         //cout << "TEfficiency: " << error << " error stat: " << error_stat << endl; 
         int thisPoint = graph->GetN();
         graph->SetPoint(thisPoint, x, quantity);
         graph->SetPointError(thisPoint, (bin_sup - bin_inf)/2, (bin_sup - bin_inf)/2, error/2, error/2);

         inputFile->Close();
      }
      else if(whichPlot=="FakeRate"){
         TFile* inputFile = TFile::Open(filename);
         TH1D* hist = 0;
         hist = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_fakeRate_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist");
         //hist = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_fakeRate_caloEta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist");
         Float_t error_tmp(0);
         quantity = hist->GetMean();
         //cout << "fakeRate: " << quantity << endl;
         range_candidate.push_back(quantity);
         //if(TEfficiency::CheckConsistency(*hist_num,*hist_deno)){
         //   eff_error = new TEfficiency(*hist_num, *hist_deno);
         //}
         // for(int i(0); i<hist_num->GetNbinsX(); ++i){
         //   error_tmp += eff_error->GetEfficiencyErrorLow(i);
         //}
         //error = error_tmp;
         error=0;
         int thisPoint = graph->GetN();
         graph->SetPoint(thisPoint, x, quantity);
         graph->SetPointError(thisPoint, (bin_sup - bin_inf)/2, (bin_sup - bin_inf)/2, error/2, error/2);

         inputFile->Close();
      }

      else if(whichPlot=="Resolution" || whichPlot=="Scale"){
         quantity = map_quantity[ETranges[indexB]][ETAranges[indexA]];
         //cout << "quantity: " << quantity << endl;
         range_candidate.push_back(quantity);
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

   PlottingTools output;
   output.graph = graph;
   output.range = range_candidate;

   return output;
}


