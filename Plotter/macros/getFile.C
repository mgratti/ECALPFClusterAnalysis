/******************************
 
  - This function writes the parameters of the fits in a txt file
  
  - The output file is for instance needed for the python framework to work


******************************/


void getFile(string fileName, 
      Bool_t do_EB, 
      map<TString, map<TString, Float_t>> map_sigma, 
      map<TString, map<TString, vector<Float_t>>> map_sigma_error, 
      map<TString, map<TString, Float_t>> map_mean, 
      map<TString, map<TString, vector<Float_t>>> map_mean_error, 
      map<TString, map<TString, Float_t>> map_chisquare, 
      vector<TString> ETranges, 
      vector<TString> ETAranges){

   ofstream outFile("/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/samples/" + fileName + ".txt"); 
   outFile << "ETranges ETAranges chi2 resolution scale efficiency noiseRate" << endl;
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
         float scale = map_mean[ETranges[iEn]][ETAranges[iEta]];
         float chisquare = map_chisquare[ETranges[iEn]][ETAranges[iEta]];

         TH1D* hist_num = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_size_simEnergy_Eta" + ETAranges[iEta] + "_En" + ETranges[iEn] + "_forEfficiency")->Clone("hist_num");
         TH1D* hist_deno = (TH1D*) inputFile->Get("EtEta_binned/h_caloParticle_size_simEnergy_Eta" + ETAranges[iEta] + "_En" + ETranges[iEn])->Clone("hist_deno");

         float efficiency;
         if(hist_deno->GetEntries()!=0){
            efficiency = hist_num->GetEntries()/hist_deno->GetEntries();
         }

         float noiseRate;
         TH1D* hist = (TH1D*) inputFile->Get("EtEta_binned/h_PFclusters_caloMatched_fakeRate_Eta" + ETAranges[iEta] + "_En" + ETranges[iEn])->Clone("hist");
         noiseRate = hist->GetMean();

         outFile << ETranges[iEn] << " " << ETAranges[iEta] << " " << chisquare << " " << resolution << " " << scale << " " << efficiency << " " << noiseRate << endl;
      }
   }
   
   inputFile->Close();
}


