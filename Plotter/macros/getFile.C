/******************************
 
  - This function writes the parameters of the fits in a txt file
  
  - The output file is for instance needed for the python framework to run


******************************/


void getFile(string fileName, 
      Bool_t do_EB, 
      map<TString, map<TString, Float_t>> map_sigma, 
      map<TString, map<TString, vector<Float_t>>> map_sigma_error, 
      map<TString, map<TString, Float_t>> map_mean, 
      map<TString, map<TString, vector<Float_t>>> map_mean_error, 
      map<TString, map<TString, Float_t>> map_chisquare, 
      map<TString, map<TString, Float_t>> map_rms, 
      map<TString, map<TString, Float_t>> map_rms_error, 
      vector<TString> ETranges, 
      vector<TString> ETAranges,
      string user){

   ofstream outFile("/t3home/" + user + "/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/samples/" + fileName + ".txt"); 
   outFile << "ETranges ETAranges chi2 resolution scale efficiency noiseRate resolution_error efficiency_error noiseRate_error scale_error noiseOccupancy rms rms_error" << endl;
   TString label = fileName.c_str();
   TString filename;
   if(do_EB){
      filename = "../Analyzer/outputfiles/" + label + "_EB.root";
   }
   else if(!do_EB){
      filename = "../Analyzer/outputfiles/" + label + "_EE.root";
   }

   TFile* inputFile = TFile::Open(filename);

   for(unsigned int iEn(0); iEn<ETranges.size(); ++iEn){
      for(unsigned int iEta(0); iEta<ETAranges.size(); ++iEta){

         float resolution = map_sigma[ETranges[iEn]][ETAranges[iEta]];
         float resolution_error = map_sigma_error[ETranges[iEn]][ETAranges[iEta]][0];
         float scale = map_mean[ETranges[iEn]][ETAranges[iEta]];
         float scale_error = map_mean_error[ETranges[iEn]][ETAranges[iEta]][0];
         float chisquare = map_chisquare[ETranges[iEn]][ETAranges[iEta]];
         float rms = map_rms[ETranges[iEn]][ETAranges[iEta]];
         float rms_error = map_rms_error[ETranges[iEn]][ETAranges[iEta]];

         float efficiency, efficiency_error;
         TEfficiency* eff_error;
         
         TH1D* hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[iEta] + "_En" + ETranges[iEn] + "_forEfficiency")->Clone("hist_num");
         TH1D* hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[iEta] + "_En" + ETranges[iEn])->Clone("hist_deno");

         if(hist_deno->GetEntries()!=0){
            efficiency = hist_num->GetEntries()/hist_deno->GetEntries();
         }
         //efficiency_error = efficiency*(sqrt(hist_num->GetEntries())/hist_num->GetEntries() + sqrt(hist_deno->GetEntries())/hist_deno->GetEntries());
         
         if(TEfficiency::CheckConsistency(*hist_num,*hist_deno)){
            eff_error = new TEfficiency(*hist_num, *hist_deno);
         }

         float error_tmp(0.);
         for(int i(0); i<hist_num->GetNbinsX(); ++i){
            error_tmp += eff_error->GetEfficiencyErrorLow(i);
         }
         efficiency_error = error_tmp;
 
         float noiseRate, noiseRate_error;
         TH1D* hist = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_fakeRate_Eta" + ETAranges[iEta] + "_En" + ETranges[iEn])->Clone("hist");
         noiseRate = hist->GetMean();
         noiseRate_error = hist->GetMeanError();

         float noiseOccupancy;
         TH1D* hist_noise = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_nonCaloMatched_noiseOccupancy_Eta" + ETAranges[iEta] + "_En" + ETranges[iEn])->Clone("hist_noise");
         noiseOccupancy = hist_noise->GetMean();

         outFile << ETranges[iEn] << " " << ETAranges[iEta] << " " << chisquare << " " << resolution << " " << scale << " " << efficiency << " " << noiseRate << " " << resolution_error << " " << efficiency_error << " " << noiseRate_error << " " << scale_error << " " << noiseOccupancy  << " " << rms << " " << rms_error << endl;
      }
   }
   
   inputFile->Close();
}


