/**************************************
 
 This function initializes the plotter for each file by getting information from the file name

**************************************/


void plotterInit(vector<string> fileName, 
      vector<Bool_t>& do_EB, 
      vector<Bool_t>& do_EE, 
      vector<TString>& matching, 
      vector<TString>& PUtag, 
      vector<TString>& pfrechit_thrs, 
      vector<TString>& seeding_thrs, 
      vector<TString>& dependency, 
      vector<Int_t>& kEvents, 
      vector<Bool_t>& do_0to20GeV, 
      vector<Bool_t>& do_0to100GeV, 
      vector<Bool_t>& do_0p1to200GeV){

   for(unsigned int iFile(0); iFile<fileName.size(); ++ iFile){
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


      if(fileName[iFile].find("numberOfHits") != std::string::npos){
         matching.push_back("numberOfHits");
      }
      else if(fileName[iFile].find("simFraction") != std::string::npos){
         matching.push_back("simFraction_min1");
      }
      else if(fileName[iFile].find("deltaR") != std::string::npos){
         matching.push_back("deltaR");
      }

      if(fileName[iFile].find("noPU") != std::string::npos){
         PUtag.push_back("noPU");
      }
      else if(fileName[iFile].find("wPU") != std::string::npos){
         PUtag.push_back("wPU");
      }


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


      if(fileName[iFile].find("thrRing") != std::string::npos){
         dependency.push_back("#etaRing");
      }
      else{
         dependency.push_back("Xtal");
      }


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


      if(fileName[iFile].find("0to20GeV") != std::string::npos){
         do_0to20GeV.push_back(true);
         do_0to100GeV.push_back(false);
         do_0p1to200GeV.push_back(false);
      }
      else if(fileName[iFile].find("0to100GeV") != std::string::npos || fileName[iFile].find("1.0to100GeV") != std::string::npos || fileName[iFile].find("1to100GeV") != std::string::npos){
         do_0to20GeV.push_back(false);
         do_0to100GeV.push_back(true);
         do_0p1to200GeV.push_back(false);
      }
      else if((fileName[iFile].find("0.1to200GeV") != std::string::npos) || (fileName[iFile].find("0to200GeV") != std::string::npos) || (fileName[iFile].find("1to200GeV") != std::string::npos)){
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
}


