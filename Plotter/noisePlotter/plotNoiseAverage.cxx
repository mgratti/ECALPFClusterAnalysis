/*
plotNoiseAverage is a modified version of plotObject_untouched_ringaverage.cxx (originally provided by DPG)

Takes as input a txt file of format
ix/ieta iy/iphi   iz/0       Value        rawId
-85        2       0         0.130       838904322
All xtals of EB first and all xtals of EE second

The script assumes that the input Value corresponds to:
   - 2 sigma of the noise in EB
   - 2/3 sigma of the noise in EE, for |eta|<2.5/|eta|>2.5

Performs average over ring

Outputs separate TGraphs for EB and EE at 1 sigma of the noise, as a function of eta 

root -l -b -q plotNoiseAverage.cxx\(\"PFRecHitThresholds_EB_ringaveraged_EE_2023.txt\"\)

*/


float getNoiseValue(float value, int ix_ieta, int iy_iphi, bool isEB, float outSigma=1.){

  float noiseValue=0;
  if(isEB)
    noiseValue = value/2.*outSigma;
  else{
    float eta= -log(tan(0.5*atan(sqrt((ix_ieta-50.5)*(ix_ieta-50.5)+(iy_iphi-50.5)*(iy_iphi-50.5))*2.98/328.)));
    if(abs(eta)<2.5)
      noiseValue = value/2.*outSigma;
    else
      noiseValue = value/3.*outSigma;
  }
  return noiseValue;
}


void plotNoiseAverage(std::string nameInputFile = "dump_EcalCondObjectContainer__since_00000001_till_18446744073709551615.dat", float outSigma=1.)
{

  gStyle->SetOptStat(0);

  // ---------------
  // READING INPUT
  // --------------
  std::vector<int> ix_ieta;
  std::vector<int> iy_iphi;
  std::vector<int> iz;
  std::vector<float> Object;
  std::ifstream file (nameInputFile.c_str());
  std::string buffer;

  if(!file.is_open()) {
    std::cerr << "** ERROR: Can't open '" << nameInputFile.c_str() << "' for input" << std::endl;
    exit(0);
  }

  int num, single_ix_ieta, single_iy_iphi;
  float value;
  int countLines=0;
  while(!file.eof()) {
    getline(file,buffer);
    if (buffer != "" && buffer.at(0) != '#'){ ///---> save from empty line at the end and from comments starting from "#"
      countLines++;
      bool isEB=true;
      if(countLines>61200) isEB=false;
      std::stringstream line( buffer );
      line >> single_ix_ieta;
      ix_ieta.push_back(single_ix_ieta);
      line >> single_iy_iphi;
      iy_iphi.push_back(single_iy_iphi);
      line >> num;
      iz.push_back(num);
      line >> value;
      float noiseValue = getNoiseValue(value, single_ix_ieta, single_iy_iphi, isEB);
      Object.push_back(noiseValue);
    }
  }

  std::cout << " ix_ieta.size() = " << ix_ieta.size() << std::endl;

  if (ix_ieta.size() > 75848) {
    std::cout << " ERROR: you may have appended the tag twice or the input is not of the desired format !" << std::endl;
  }

  // ---------------
  // EE
  // --------------
  std::vector<float> ringPlus_Object;          std::vector<float> ringMinus_Object;      std::vector<float> ring_Object;
  std::vector<int> ringPlus_Objectcount;       std::vector<int> ringMinus_Objectcount;   std::vector<int> ring_Objectcount;
  std::vector<double> eta;

  for (int iter = 0; iter < (50-11+3); iter++) {
    ringPlus_Object.push_back(0);            ringMinus_Object.push_back(0);       ring_Object.push_back(0);
    ringPlus_Objectcount.push_back(0);       ringMinus_Objectcount.push_back(0);  ring_Objectcount.push_back(0);
    eta.push_back(0);
  }

  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (Object.at(iter) > 0){
      if (iz.at(iter) != 0) {

        float dx = ix_ieta.at(iter) - 50.5;
        float dy = iy_iphi.at(iter) - 50.5;

        float ring = sqrt( dx*dx + dy*dy );

        int iring = round(ring) - 12;  //---- 12 [ = (62 - 50 - 1) from the 2D plot] is the first ring
        eta.at(iring)= -log(tan(0.5*atan(sqrt((ix_ieta.at(iter)-50.5)*(ix_ieta.at(iter)-50.5)+(iy_iphi.at(iter)-50.5)*(iy_iphi.at(iter)-50.5))*2.98/328.)));


        if (iring > (50-11+2) || iring < 0) std::cout << " what ?!?   iring = " << iring << " dx = " << dx << " dy = " << dy << " :::: ix = " << ix_ieta.at(iter) << "  iy = " << iy_iphi.at(iter) << std::endl;
        if (Object.at(iter) > 0) {
          if (iz.at(iter) > 0) {
            ringPlus_Object.at(iring) = ringPlus_Object.at(iring) + Object.at(iter);
            ring_Object.at(iring) = ring_Object.at(iring) + Object.at(iter);
            ringPlus_Objectcount.at(iring) =  ringPlus_Objectcount.at(iring) + 1 ;
	           //computing the overall average of EEPlus and EEMinus
	           ring_Objectcount.at(iring) =  ring_Objectcount.at(iring) + 1 ;
          } else {
            ringMinus_Object.at(iring) = ringMinus_Object.at(iring) + Object.at(iter);
            ring_Object.at(iring) = ring_Object.at(iring) + Object.at(iter);
            ringMinus_Objectcount.at(iring) =  ringMinus_Objectcount.at(iring) + 1 ;
	           //computing the overall average of EEPlus and EEMinus
            ring_Objectcount.at(iring) =  ring_Objectcount.at(iring) + 1 ;
          }
        }
      }
    }
  }
  for (int iter = 0; iter < ix_ieta.size(); iter++) {
     if (iz.at(iter) != 0) {
        //float ring_average;
        float dx = ix_ieta.at(iter) - 50.5;
        float dy = iy_iphi.at(iter) - 50.5;
        float ring = sqrt( dx*dx + dy*dy );

        int iring = round(ring) - 12;  //---- 12 [ = (62 - 50 - 1) from the 2D plot] is the first ring


        if (iring > (50-11+2) || iring < 0) std::cout << " what ?!?   iring = " << iring << " dx = " << dx << " dy = " << dy << " :::: ix = " << ix_ieta.at(iter) << "  iy = " << iy_iphi.at(iter) << std::endl;

        if (iz.at(iter) > 0) {
	         float ring_average = ringPlus_Object.at(iring)/ringPlus_Objectcount.at(iring);
          Object.at(iter) = ring_average;
        } else {
	        float ring_average = ringMinus_Object.at(iring)/ringMinus_Objectcount.at(iring);
          Object.at(iter) = ring_average;
        }
     }
  }

  // ---------------
  // Prepare output dir
  // --------------
  TString path = TString(nameInputFile.c_str());
  path.ReplaceAll(".txt", "");
  gSystem->Exec("mkdir " + path );


  /*
  // ---------------
  // Plot EE 2D
  // --------------
  TH2F* histoEE_Object = new TH2F ("histoEE_Object", "Object" ,  200, 0.5, 200.5,  100, 0.5, 100.5);


  histoEE_Object->GetXaxis()->SetTitle("x");
  histoEE_Object->GetYaxis()->SetTitle("y");

  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (iz.at(iter) != 0) {

      histoEE_Object->Fill(ix_ieta.at(iter) + 100*(iz.at(iter)>0), iy_iphi.at(iter), Object.at(iter));
      //histoEE_Object->Fill(ix_ieta.at(iter) + 100*(iz.at(iter)>0), iy_iphi.at(iter), Object.at(iter)*0.866754);

    }
  }
		TCanvas* ccEE = new TCanvas ("ccEE","",1600,600);
  ccEE->cd();
  histoEE_Object->Draw("colz");
  ccEE->SaveAs(path+"/"+"Object_EE.png");
  ccEE->SaveAs(path+"/"+"Object_EE.root");
		*/

  // ---------------
  // Plot EE Ring
  // --------------
  TGraph *gr_EEPlus_Object = new TGraph();     TGraph *gr_EEMinus_Object = new TGraph();
  TGraph *gr_EE_Object = new TGraph();

  // for (int iter = 0; iter < (50-11+3); iter++) {
  for (int iter = 0; iter < (50-11); iter++) {

    //    gr_EEPlus_Object-> SetPoint (iter,  42 -  iter,   ringPlus_Objectcount.at(iter)  ?  ringPlus_Object.at(iter)  / ringPlus_Objectcount.at(iter)  : 0 ) ;
    //    gr_EEMinus_Object-> SetPoint (iter, 42 -   iter,  ringMinus_Objectcount.at(iter) ?  ringMinus_Object.at(iter) / ringMinus_Objectcount.at(iter) : 0 ) ;
    //    gr_EEPlus_Object-> SetPoint (iter,  38 -  iter,   ringPlus_Objectcount.at(iter)  ?  ringPlus_Object.at(iter)  / ringPlus_Objectcount.at(iter)  : 0 ) ;
    //    gr_EEMinus_Object-> SetPoint (iter, 38 -   iter,  ringMinus_Objectcount.at(iter) ?  ringMinus_Object.at(iter) / ringMinus_Objectcount.at(iter) : 0 ) ;
    //    gr_EE_Object-> SetPoint (iter, 38 - iter,  ring_Objectcount.at(iter) ?  ring_Object.at(iter) / ring_Objectcount.at(iter) : 0 ) ;
    gr_EEPlus_Object-> SetPoint (iter,  eta.at(iter),   ringPlus_Objectcount.at(iter)  ?  ringPlus_Object.at(iter)  / ringPlus_Objectcount.at(iter)  : 0 ) ;
    gr_EEMinus_Object-> SetPoint (iter, eta.at(iter),  ringMinus_Objectcount.at(iter) ?  ringMinus_Object.at(iter) / ringMinus_Objectcount.at(iter) : 0 ) ;
    gr_EE_Object-> SetPoint (iter, eta.at(iter),  ring_Objectcount.at(iter) ?  ring_Object.at(iter) / ring_Objectcount.at(iter) : 0 ) ;
     //    cout<<"iter = "<< 38-iter<< "ring_average = " << ring_Object.at(iter)/ring_Objectcount.at(iter)<< " eta.at(iter) = "<<  eta.at(iter) <<endl;
     // if(eta.at(iter)==0)   cout<<"iter = "<<38-iter<< "ring_average = " << ring_Object.at(iter)/ring_Objectcount.at(iter)<< " eta.at(iter) = "<<  eta.at(iter) <<endl;
  }


  //---- style ----

  gr_EEPlus_Object->SetMarkerSize  (1);                           gr_EEMinus_Object->SetMarkerSize  (1);
  gr_EEPlus_Object->SetMarkerStyle (24);                          gr_EEMinus_Object->SetMarkerStyle (22);
  gr_EEPlus_Object->SetMarkerColor (kRed);                        gr_EEMinus_Object->SetMarkerColor (kRed);
  gr_EEPlus_Object->SetLineWidth (1);                             gr_EEMinus_Object->SetLineWidth (1);
  gr_EEPlus_Object->SetLineColor (kRed);                          gr_EEMinus_Object->SetLineColor (kRed);

  gr_EE_Object->SetMarkerSize  (1);
  gr_EE_Object->SetMarkerStyle (26);
  gr_EE_Object->SetMarkerColor (kBlack);
  gr_EE_Object->SetLineWidth (1);
  gr_EE_Object->SetLineWidth (1);


  //---- style (end) ----

  TCanvas* ccRing = new TCanvas ("ccRing","",800,600);

  gr_EEPlus_Object->Draw("APL");
  gr_EEMinus_Object->Draw("PL");
  gr_EE_Object->Draw("PL");
  gr_EEPlus_Object->GetYaxis()->SetTitleOffset(1.2);

  gr_EEPlus_Object->GetYaxis()->SetTitle("<Object>");
  gr_EEPlus_Object->GetXaxis()->SetTitle("iRing");
  gr_EEPlus_Object->GetXaxis()->SetTitle("eta");

  ccRing->SetLogy();
  ccRing->SaveAs(path+"/"+"Object_EE_perRing.png");
  ccRing->SaveAs(path+"/"+"Object_EE_perRing.root");


  // ---------------
  // EB
  // --------------
  //
  //   iring = ieta in EB
  //

  std::vector<float> EBringPlus_Object;          std::vector<float> EBringMinus_Object;          std::vector<float> EBring_Object;
  std::vector<int> EBringPlus_Objectcount;       std::vector<int> EBringMinus_Objectcount;       std::vector<int> EBring_Objectcount;

  for (int iter = 0; iter < 85; iter++) {
    EBringPlus_Object.push_back(0);            EBringMinus_Object.push_back(0);             EBring_Object.push_back(0);
    EBringPlus_Objectcount.push_back(0);       EBringMinus_Objectcount.push_back(0);        EBring_Objectcount.push_back(0);
  }

  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (iz.at(iter) == 0) {
      int iEBring = abs(ix_ieta.at(iter)) - 1 ;
      if (iEBring > 84 || iEBring < 0) std::cout << " what ?!?   iEBring = " << iEBring << std::endl;
      if (Object.at(iter) > 0){
        if (ix_ieta.at(iter) > 0) {
          EBringPlus_Object.at(iEBring) = EBringPlus_Object.at(iEBring) + Object.at(iter);
          EBring_Object.at(iEBring) = EBring_Object.at(iEBring) + Object.at(iter);
          EBringPlus_Objectcount.at(iEBring) =  EBringPlus_Objectcount.at(iEBring) + 1 ;
	         //computing the overall average of EBPlus and EBMinus
          EBring_Objectcount.at(iEBring) =  EBring_Objectcount.at(iEBring) + 1 ;
	       } else {
          EBringMinus_Object.at(iEBring) = EBringMinus_Object.at(iEBring) + Object.at(iter);
          EBring_Object.at(iEBring) = EBring_Object.at(iEBring) + Object.at(iter);
          EBringMinus_Objectcount.at(iEBring) =  EBringMinus_Objectcount.at(iEBring) + 1 ;
          //computing the overall average of EBPlus and EBMinus
	         EBring_Objectcount.at(iEBring) =  EBring_Objectcount.at(iEBring) + 1 ;
        }
      }
    }
  }

  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (iz.at(iter) == 0) {
      int iEBring = abs(ix_ieta.at(iter)) - 1 ;
      if (iEBring > 84 || iEBring < 0) std::cout << " what ?!?   iEBring = " << iEBring << std::endl;
      // if (ix_ieta.at(iter) > 0 && Object.at(iter) <= 0)
      if (ix_ieta.at(iter) > 0 ) {
	       float ring_average = EBringPlus_Object.at(iEBring)/EBringPlus_Objectcount.at(iEBring);
	       Object.at(iter) = ring_average;
	     }
      //if (ix_ieta.at(iter) < 0 && Object.at(iter) <= 0)
      if (ix_ieta.at(iter) < 0 ) {
        float ring_average =  EBringMinus_Object.at(iEBring)/EBringMinus_Objectcount.at(iEBring);
        Object.at(iter) = ring_average;
      }
    }
  }

  /*
  // ---------------
  // Plot EB 2D
  // --------------
  TH2F* histoEB_Object = new TH2F ("histoEB_Object", "Object" ,  360, 0.5, 360.5,  171, -85.5, 85.5);
  histoEB_Object->GetXaxis()->SetTitle("i#phi");
  histoEB_Object->GetYaxis()->SetTitle("i#eta");


  for (int iter = 0; iter < ix_ieta.size(); iter++) {
    if (iz.at(iter) == 0) {

      histoEB_Object->Fill(iy_iphi.at(iter), ix_ieta.at(iter), Object.at(iter) );

    }
  }

		TCanvas* ccEB = new TCanvas ("ccEB","",1600,600);
  ccEB->cd();
  histoEB_Object->Draw("colz");
  ccEB->SaveAs(path+"/"+"Object_EB.png");
  ccEB->SaveAs(path+"/"+"Object_EB.root");
  */


  // ---------------
  // Plot EB Ring
  // --------------
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

  ccRingEB->SaveAs(path+"/"+"Object_EB_perRing.png");
  ccRingEB->SaveAs(path+"/"+"Object_EB_perRing.root");


  // ---------------
  // Output root objects
  // --------------
  TFile fileout(path+"/"+"graphs.root","RECREATE");
  fileout.cd();
  //TH2F* resultEB = (TH2F*)histoEB_Object->Clone("resultEB");
  //TH2F* resultEE = (TH2F*)histoEE_Object->Clone("resultEE");

  //resultEB->Write("resultEB");
  //resultEE->Write("resultEE");
  gr_EEPlus_Object->Write("gr_EEPlus_Object");
  gr_EEMinus_Object->Write("gr_EEMinus_Object");
  gr_EE_Object->Write("gr_EE_Object");
  gr_EBPlus_Object->Write("gr_EBPlus_Object");
  gr_EBMinus_Object->Write("gr_EBMinus_Object");
  gr_EB_Object->Write("gr_EB_Object");


}
