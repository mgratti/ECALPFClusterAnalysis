//
//---- plot Object untouched (no_outliers cleaning) and  EE * 0.866754 commented
//double eta= -log(tan(0.5*atan(sqrt((iX-50.5)*(iX-50.5)+(iY-50.5)*(iY-50.5))*2.98/328.)));


void plotObject_untouched_ringaverage(std::string nameInputFile = "dump/dump_EcalCondObjectContainer__since_00000001_till_18446744073709551615.dat")
{
  
  gStyle->SetOptStat(0);
 
  
  TCanvas* ccEB = new TCanvas ("ccEB","",1600,600);
   
  
  //   
  // input file format:
  //
  // ix/ieta iy/iphi   iz/0       Object        rawId
  //
  // -85    2    0        0.130 838904322
  // 
  
  
  
  std::vector<int> ix_ieta;
  std::vector<int> iy_iphi;
  std::vector<int> iz;
  std::vector<bool> ioutlier;
  
  std::vector<float> Object;
  
  std::ifstream file (nameInputFile.c_str()); 
  
  std::string buffer;
  
  if(!file.is_open()) {
    std::cerr << "** ERROR: Can't open '" << nameInputFile.c_str() << "' for input" << std::endl;
    exit(0);
  }
  
  int num;
  float value;
  while(!file.eof()) {
    getline(file,buffer);
//     std::cout << "buffer = " << buffer << std::endl;
    if (buffer != "" && buffer.at(0) != '#'){ ///---> save from empty line at the end and from comments starting from "#"
      std::stringstream line( buffer );      
      line >> num; 
      ix_ieta.push_back(num);
      line >> num; 
      iy_iphi.push_back(num);
      line >> num; 
      iz.push_back(num); 
      line >> value; 
      Object.push_back(value);
      ioutlier.push_back(false);
 
    } 
  }
  
  
  std::cout << " ix_ieta.size() = " << ix_ieta.size() << std::endl;
    
  if (ix_ieta.size() > 75848) {
    std::cout << " Attention: you appended the tag twice!" << std::endl;
  }    
  
  
  
  
  //---- EB ----ring average
  
  //   
  //   iring = ieta in EB
  //   
  
  std::vector<float> EBringPlus_Object;          std::vector<float> EBringMinus_Object;          std::vector<float> EBring_Object;
  std::vector<float> EBringPlus_Objectsq;        std::vector<float> EBringMinus_Objectsq;        std::vector<float> EBring_Objectsq;
  std::vector<int> EBringPlus_Objectcount;       std::vector<int> EBringMinus_Objectcount;       std::vector<int> EBring_Objectcount;
  
  for (int iter = 0; iter < 85; iter++) {
    EBringPlus_Object.push_back(0);            EBringMinus_Object.push_back(0);             EBring_Object.push_back(0);
    EBringPlus_Objectsq.push_back(0);          EBringMinus_Objectsq.push_back(0);           EBring_Objectsq.push_back(0);
    EBringPlus_Objectcount.push_back(0);       EBringMinus_Objectcount.push_back(0);        EBring_Objectcount.push_back(0);
  }

  // first loop over all xtals, increment to compute average and rms
  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (iz.at(iter) == 0) {
      int iEBring = abs(ix_ieta.at(iter)) - 1 ;
      
      if (iEBring > 84 || iEBring < 0) std::cout << " what ?!?   iEBring = " << iEBring << std::endl;
      if (Object.at(iter) > 0){     
        if (ix_ieta.at(iter) > 0) {
          EBringPlus_Object.at(iEBring) += Object.at(iter);
          EBring_Object.at(iEBring)     += Object.at(iter);
          EBringPlus_Objectsq.at(iEBring) += Object.at(iter)*Object.at(iter);
          EBring_Objectsq.at(iEBring)     += Object.at(iter)*Object.at(iter);
          EBringPlus_Objectcount.at(iEBring) += 1;
          EBring_Objectcount.at(iEBring) += 1 ;      
        } else {
          EBringMinus_Object.at(iEBring) += Object.at(iter);
          EBring_Object.at(iEBring)      += Object.at(iter);
          EBringMinus_Objectsq.at(iEBring) += Object.at(iter)*Object.at(iter);
          EBring_Objectsq.at(iEBring)      += Object.at(iter)*Object.at(iter);
          EBringMinus_Objectcount.at(iEBring) += 1;      
          EBring_Objectcount.at(iEBring) += 1;      
        
        }
      }  
    } 
  } // end first loop

  // second loop, calculate average and check if the xtal is within mu+/- 5 sigma, aka set outlier

  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (iz.at(iter) == 0) {
      int iEBring = abs(ix_ieta.at(iter)) - 1 ;
      if (iEBring > 84 || iEBring < 0) std::cout << " what ?!?   iEBring = " << iEBring << std::endl;
      if (ix_ieta.at(iter) > 0 ){
        float ring_average = EBringPlus_Object.at(iEBring)/EBringPlus_Objectcount.at(iEBring);   
        float ring_rms =sqrt(EBringPlus_Objectsq.at(iEBring)/EBringPlus_Objectcount.at(iEBring) - ring_average*ring_average);
        if(fabs( Object.at(iter) - ring_average ) / ring_rms > 5 ) ioutlier.at(iter)=true;
        //if(ioutlier.at(iter)==false) Object.at(iter) = ring_average; do not set the threshold, because you first need to recacluate the average
      }
      if (ix_ieta.at(iter) < 0 ){
        float ring_average =  EBringMinus_Object.at(iEBring)/EBringMinus_Objectcount.at(iEBring);
        float ring_rms =sqrt(EBringMinus_Objectsq.at(iEBring)/EBringMinus_Objectcount.at(iEBring) - ring_average*ring_average);
        if(fabs( Object.at(iter) - ring_average ) / ring_rms > 5 ) ioutlier.at(iter)=true;
        //Object.at(iter) = ring_average; do not set the threshold yet... 
      }
    }
  }

  // third loop, recalculate average and rms w/o outlier (could have been done recursively, but here we are...)
  for (int iter = 0; iter < ix_ieta.size(); iter++) {

    if(ioutlier.at(iter) == false) continue; // 

    if (iz.at(iter) == 0) {
      int iEBring = abs(ix_ieta.at(iter)) - 1 ;
      
      if (iEBring > 84 || iEBring < 0) std::cout << " what ?!?   iEBring = " << iEBring << std::endl;
      if (Object.at(iter) > 0){     
        if (ix_ieta.at(iter) > 0) {
          EBringPlus_Object.at(iEBring) += Object.at(iter);
          EBring_Object.at(iEBring)     += Object.at(iter);
          EBringPlus_Objectsq.at(iEBring) += Object.at(iter)*Object.at(iter);
          EBring_Objectsq.at(iEBring)     += Object.at(iter)*Object.at(iter);
          EBringPlus_Objectcount.at(iEBring) += 1;
          EBring_Objectcount.at(iEBring) += 1 ;      
        } else {
          EBringMinus_Object.at(iEBring) += Object.at(iter);
          EBring_Object.at(iEBring)      += Object.at(iter);
          EBringMinus_Objectsq.at(iEBring) += Object.at(iter)*Object.at(iter);
          EBring_Objectsq.at(iEBring)      += Object.at(iter)*Object.at(iter);
          EBringMinus_Objectcount.at(iEBring) += 1;      
          EBring_Objectcount.at(iEBring) += 1;      
        
        }
      }  
    } 
  } // end third loop

  // fourth and final loop, calculate average and set the threshold
  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (iz.at(iter) == 0) {
      int iEBring = abs(ix_ieta.at(iter)) - 1 ;
      if (iEBring > 84 || iEBring < 0) std::cout << " what ?!?   iEBring = " << iEBring << std::endl;
      if (ix_ieta.at(iter) > 0 ){
        float ring_average = EBringPlus_Object.at(iEBring)/EBringPlus_Objectcount.at(iEBring);   
        float ring_rms =sqrt(EBringPlus_Objectsq.at(iEBring)/EBringPlus_Objectcount.at(iEBring) - ring_average*ring_average);
        //if(fabs( Object.at(iter) - ring_average ) / ring_rms > 5 ) ioutlier.at(iter)=true;
        if(ioutlier.at(iter)==false) Object.at(iter) = ring_average;
        else std::cout << "keep same thr for outlier found at ieta=" << ix_ieta.at(iter) << " iphi=" << iy_iphi.at(iter) << " thr=" <<Object.at(iter) << " avg=" << ring_average << std::endl; 
      }
      if (ix_ieta.at(iter) < 0 ){
        float ring_average =  EBringMinus_Object.at(iEBring)/EBringMinus_Objectcount.at(iEBring);
        float ring_rms =sqrt(EBringMinus_Objectsq.at(iEBring)/EBringMinus_Objectcount.at(iEBring) - ring_average*ring_average);
        if(ioutlier.at(iter)==false) Object.at(iter) = ring_average;
        else std::cout << "keep same thr for outlier found at ieta=" << ix_ieta.at(iter) << " iphi=" << iy_iphi.at(iter) << " thr=" <<Object.at(iter) << " avg=" << ring_average << std::endl;
      }
    }
  } // end fourth loop
    
  
  //---- EB ----2D
  
  TH2F* histoEB_Object = new TH2F ("histoEB_Object", "Object" ,  360, 0.5, 360.5,  171, -85.5, 85.5);
  histoEB_Object->GetXaxis()->SetTitle("i#phi");
  histoEB_Object->GetYaxis()->SetTitle("i#eta");
  
  
  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (iz.at(iter) == 0) {
      
      histoEB_Object->Fill(iy_iphi.at(iter), ix_ieta.at(iter), Object.at(iter) );
  
    }
  }
  
  ccEB->cd();
  histoEB_Object->Draw("colz");
  //ccEB->SaveAs("Object_EB.png");
  //ccEB->SaveAs("Object_EB.root");
  
  

  
  TGraph *gr_EBPlus_Object = new TGraph();     TGraph *gr_EBMinus_Object = new TGraph();
  TGraph *gr_EB_Object = new TGraph();  

  for (int iter = 0; iter < 85; iter++) {
    //  eta.at(iter)= (iter+1) * 0.0174;

    //    gr_EBPlus_Object-> SetPoint (iter,    iter+1,   EBringPlus_Objectcount.at(iter) ? EBringPlus_Object.at(iter) / EBringPlus_Objectcount.at(iter) : 0 ) ;           
    //    gr_EBMinus_Object-> SetPoint (iter,    iter+1,   EBringMinus_Objectcount.at(iter) ?  EBringMinus_Object.at(iter) / EBringMinus_Objectcount.at(iter) : 0 ) ;  
    gr_EBPlus_Object-> SetPoint (iter,    (iter+1) * 0.0174,   EBringPlus_Objectcount.at(iter) ? EBringPlus_Object.at(iter) / EBringPlus_Objectcount.at(iter) : 0 ) ;           
    gr_EBMinus_Object-> SetPoint (iter,    (iter+1) * 0.0174,   EBringMinus_Objectcount.at(iter) ?  EBringMinus_Object.at(iter) / EBringMinus_Objectcount.at(iter) : 0 ) ;  
    
    gr_EB_Object-> SetPoint (iter,    (iter+1) * 0.0174,   EBring_Objectcount.at(iter) ?  EBring_Object.at(iter) / EBring_Objectcount.at(iter) : 0 ) ;  
    
  }
  
  
  //---- style ----
  
  gr_EBPlus_Object->SetMarkerSize  (1);                           gr_EBMinus_Object->SetMarkerSize  (1);
  gr_EBPlus_Object->SetMarkerStyle (24);                          gr_EBMinus_Object->SetMarkerStyle (22);
  gr_EBPlus_Object->SetMarkerColor (kRed);                        gr_EBMinus_Object->SetMarkerColor (kRed);
  gr_EBPlus_Object->SetLineWidth (1);                             gr_EBMinus_Object->SetLineWidth (1);
  gr_EBPlus_Object->SetLineColor (kRed);                          gr_EBMinus_Object->SetLineColor (kRed);

  gr_EB_Object->SetMarkerSize  (1);
  gr_EB_Object->SetMarkerStyle (26);
  gr_EB_Object->SetMarkerColor (kBlack);
  gr_EB_Object->SetLineWidth (1);
  gr_EB_Object->SetLineWidth (1);

  
  //---- style (end) ----
  
  
  TCanvas* ccRingEB = new TCanvas ("ccRingEB","",800,600);
  
  gr_EBPlus_Object->Draw("APL");
  gr_EBMinus_Object->Draw("PL");
  gr_EB_Object->Draw("PL");
  gr_EBPlus_Object->GetYaxis()->SetTitleOffset(1.2);
  gr_EBPlus_Object->GetYaxis()->SetTitle("<Object>");
  gr_EBPlus_Object->GetXaxis()->SetTitle("i#eta");
  gr_EBPlus_Object->GetXaxis()->SetTitle("eta");
  
  //ccRingEB->SaveAs("Object_EB_perRing.png");
  //ccRingEB->SaveAs("Object_EB_perRing.root");    


  TFile fileout("productCleaned_untouched_ringaverage.root","RECREATE");
  fileout.cd();
  TH2F* resultEB = (TH2F*)histoEB_Object->Clone("resultEB");

  resultEB->Write("resultEB");
  gr_EBPlus_Object->Write("gr_EBPlus_Object");
  gr_EBMinus_Object->Write("gr_EBMinus_Object");
  gr_EB_Object->Write("gr_EB_Object");


}
