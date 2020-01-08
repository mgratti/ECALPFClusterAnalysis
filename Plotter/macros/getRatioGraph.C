
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/objects.h"


PlottingTools getRatioGraph(TString whichPlot, 
      string fileName1, 
      string fileName2, 
      int iFile, 
      vector<map<TString, map<TString, Float_t>>> map_quantity, 
      vector<map<TString, map<TString, vector<Float_t>>>> map_quantity_error, 
      unsigned int kk, 
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

   vector<float> range_candidate(10);

   TGraphAsymmErrors* graph = new TGraphAsymmErrors(0);
   if(whichPlot=="Resolution"){
      graph->SetTitle("Resolution - Ratio");
   }
   else if(whichPlot=="Scale"){
      graph->SetTitle("Scale - Ratio");
   }
   else if(whichPlot=="Efficiency"){
      graph->SetTitle("Efficiency - Ratio");
   }
   else if(whichPlot=="FakeRate"){
      graph->SetTitle("FakeRate - Ratio");
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
         if(quantity2!=0){
            range_candidate.push_back(quantity1/quantity2);
         }
         inputFile1->Close();
         inputFile2->Close();
      }
      else if(whichPlot=="FakeRate"){
         TFile* inputFile1 = TFile::Open(filename1);
         TFile* inputFile2 = TFile::Open(filename2);

         TH1D* hist1  = 0;
         TH1D* hist2  = 0;

         hist1 = (TH1D*) inputFile1->Get("EtEta_binned/h_PFclusters_caloMatched_fakeRate_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist1");
         hist2 = (TH1D*) inputFile2->Get("EtEta_binned/h_PFclusters_caloMatched_fakeRate_Eta" + ETAranges[indexA] + "_En" + ETranges[indexB])->Clone("hist2");

         quantity1 = hist1->GetMean();
         quantity2 = hist2->GetMean();
         //float error_tmp1(0.), error_tmp2(0.); 
         //if(TEfficiency::CheckConsistency(*hist_num1,*hist_deno1)){
         //   eff_error1 = new TEfficiency(*hist_num1, *hist_deno1);
         //}
         //for(int i(0); i<hist_num1->GetNbinsX(); ++i){
         //   error_tmp1 += eff_error1->GetEfficiencyErrorLow(i);
         //}
         //error1 = error_tmp1;
         //if(TEfficiency::CheckConsistency(*hist_num2,*hist_deno2)){
         //   eff_error2 = new TEfficiency(*hist_num2, *hist_deno2);
         //}
         //for(int i(0); i<hist_num2->GetNbinsX(); ++i){
         //   error_tmp2 += eff_error2->GetEfficiencyErrorLow(i);
         //}
         //error2 = error_tmp2;
         error = 0; // quantity1/quantity2*(error1/quantity1 + error2/quantity2);
         int thisPoint = graph->GetN();
         graph->SetPoint(thisPoint, x, quantity1/quantity2);
         graph->SetPointError(thisPoint, (bin_sup - bin_inf)/2, (bin_sup - bin_inf)/2, error/2, error/2);
         if(quantity2!=0){
            range_candidate.push_back(quantity1/quantity2);
         }
         inputFile1->Close();
         inputFile2->Close();
      }
      else if(whichPlot=="Resolution" || whichPlot=="Scale"){
         quantity1 = map_quantity[0][ETranges[indexB]][ETAranges[indexA]];
         quantity2 = map_quantity[iFile][ETranges[indexB]][ETAranges[indexA]];
         error1 = map_quantity_error[0][ETranges[indexB]][ETAranges[indexA]][0];
         error2 = map_quantity_error[iFile][ETranges[indexB]][ETAranges[indexA]][0];
         error = quantity1/quantity2*(error1/quantity1 + error2/quantity2);
         if(quantity1!=0 && quantity2!=0){
            int thisPoint = graph->GetN();
            graph->SetPoint(thisPoint, x, quantity1/quantity2);
            graph->SetPointError(thisPoint, (bin_sup - bin_inf)/2, (bin_sup - bin_inf)/2, error, error);
            if(quantity2!=0){
               range_candidate.push_back(quantity1/quantity2);
            }
         }   
      }
   }


   if(whichPlot=="Efficiency"){
      graph->GetYaxis()->SetRangeUser(0.9, 12);
   }
   else if(whichPlot=="Scale"){
      graph->GetYaxis()->SetRangeUser(0.98, 1.02);
   }
   else{
      graph->GetYaxis()->SetRangeUser(0.93, 1.2);
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

   PlottingTools output;
   output.graph = graph;
   output.range = range_candidate;

   return output;

}


