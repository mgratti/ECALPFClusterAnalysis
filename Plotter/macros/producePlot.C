
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/objects.h"
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/getGraph.C"
#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/getRatioGraph.C"

/************************************

  - This function is needed to produce the resolution/scale/efficiency/fakeRate (=Quantity) plots

  - It produces two different kinds of plots:
  -  Quantity as a function of the energy bin, plotted on the same canvas for the different eta bins
  -  Quantity as a function of the eta bin, plotted on the same canvas for the different energy bins

  - On top of this, if the do_ratioPlot option is on, it will plot, on the same canvas, three pads, the bottom one being the ratio between the two top ones

  - The function calls the getGraph and getGraphRatio (if ratio flag is on) that produce the necessary graphs

  - It takes as an argument the necessary plotting options, e.g
  -  the binning ranges
  -  PUtag
  -  thresholds 
  -  ringAvg vs Xtal tags
  -  color...

  - The autoscale option is also implemented 

  - It saves the plots in .png and .pdf formats

 ***********************************/


void producePlot(
      TString whichPlot, 
      vector<string> fileName, 
      vector<map<TString, map<TString, Float_t>>> map_quantity, 
      vector<map<TString, map<TString, vector<Float_t>>>> map_quantity_error, 
      Bool_t do_ratioPlot, 
      vector<Bool_t> do_EB, 
      vector<Bool_t> do_EE, 
      Bool_t do_binningEt, 
      Bool_t use_simEnergy, 
      Bool_t do_autoScale, 
      vector<TString> ETAranges_EB, 
      vector<TString> ETAranges_EE, 
      vector<vector<TString>> ETranges, 
      map<TString, Edges> ETvalue, 
      map<TString, Edges> ETAvalue, 
      map<int, EColor> color, 
      vector<string> outputdir, 
      vector<Int_t> kEvents, 
      vector<TString> matching, 
      vector<TString> PUtag, 
      vector<TString> pfrechit_thrs, 
      vector<TString> seeding_thrs, 
      vector<TString> dependency){

         // two kinds of plots will be produced: xAxis as energy bins and different graph (with associated color) for the eta bins 
         // and vice versa
         // The following quantity indices which binning is on the xAxis
         vector<string> xAxisQuantity = {"vsEnergy", "vsEta"};

         for(unsigned int iAxis(0); iAxis<xAxisQuantity.size(); ++iAxis){

            TCanvas* c1;
            if(!do_ratioPlot){
               c1 = new TCanvas("c1", "c1", 700, 600);
            }
            else{
               c1 = new TCanvas("c1", "c1", 700, 1000);
            }
            TLegend* leg1 = new TLegend(0.7, 0.55, 0.9, 0.85);

            c1->cd();
            TPad *pad1 = new TPad("pad1","pad1",0,0.625,1,1);
            pad1->SetBottomMargin(0.15);
            TPad *pad2 = new TPad("pad2","pad2",0,0.25,1,0.625);
            pad2->SetBottomMargin(0.15);
            TPad *pad3 = new TPad("pad3","pad3",0,0,1,0.25);
            pad3->SetBottomMargin(0.15);
            pad3->SetGrid();

            if(do_ratioPlot){
               pad1->Draw();
               pad2->Draw();
               pad3->Draw();
            }

            vector<float> vector_range1;
            vector<float> vector_range2;
            vector<float> vector_rangeR;

            vector<TString> ETAranges;
            unsigned int ETAsize;

            if(do_EB[0] == true){
               ETAranges = ETAranges_EB;
               ETAsize = ETAranges_EB.size();
            }
            else{
               ETAranges = ETAranges_EE;
               ETAsize = ETAranges_EE.size();
            }

            unsigned int xSize;
            if(xAxisQuantity[iAxis]=="vsEnergy"){
               if(do_EB[0] == true){
                  xSize = ETAranges_EB.size();
               }
               else{
                  xSize = ETAranges_EE.size();
               }
            }
            else if(xAxisQuantity[iAxis]=="vsEta"){
               xSize = ETranges.size();
            }


            for(unsigned int kk(0); kk<xSize; ++kk){
               vector<float> range_graph1 = getGraph(whichPlot, fileName[0], map_quantity[0], map_quantity_error[0], kk, true, do_EB[0], do_EE[0], do_binningEt, use_simEnergy, ETranges[0], ETAranges, ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).range;
               for(unsigned int i(0); i<range_graph1.size(); ++i){
                  vector_range1.push_back(range_graph1[i]);
               }
               if(do_ratioPlot){
                  vector<float> range_graph2 = getGraph(whichPlot, fileName[1], map_quantity[1], map_quantity_error[1], kk, false, do_EB[1], do_EE[1], do_binningEt, use_simEnergy, ETranges[1], ETAranges, ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).range;
                  for(unsigned int i(0); i<range_graph2.size(); ++i){
                     vector_range2.push_back(range_graph2[i]);
                  }

                  vector<float> range_graphR =  getRatioGraph(whichPlot, fileName[0], fileName[1], 1,  map_quantity, map_quantity_error, kk, do_EB[0], do_EE[0], do_binningEt, use_simEnergy, ETranges[0], ETAranges, ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).range;
                  for(unsigned int i(0); i<range_graphR.size(); ++i){
                     vector_rangeR.push_back(range_graphR[i]);
                  }
               }
            }

            float max_range1 = *max_element(vector_range1.begin(), vector_range1.end());
            float min_range1 = *min_element(vector_range1.begin(), vector_range1.end());

            float max_range2; 
            float min_range2;
            float max_rangeR;
            float min_rangeR;

            if(do_ratioPlot){
               max_range2 = *max_element(vector_range2.begin(), vector_range2.end());
               min_range2 = *min_element(vector_range2.begin(), vector_range2.end());
               max_rangeR = *max_element(vector_rangeR.begin(), vector_rangeR.end());
               min_rangeR = *min_element(vector_rangeR.begin(), vector_rangeR.end());
            }


            for(unsigned int kk(0); kk<ETAsize; ++kk){
               TGraphAsymmErrors* graph1;
               graph1 = getGraph(whichPlot, fileName[0], map_quantity[0], map_quantity_error[0], kk, true, do_EB[0], do_EE[0], do_binningEt, use_simEnergy, ETranges[0], ETAranges, ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).graph; 
               if(do_autoScale){
                  graph1->GetYaxis()->SetRangeUser(min_range1-0.05*(max_range1-min_range1), max_range1+0.8*(max_range1-min_range1));
               }

               if(do_ratioPlot){
                  pad1->cd();
               }
               if(kk==0){
                  graph1->Draw("A*");
               }
               else{
                  graph1->Draw("*, same");
               }
               if(xAxisQuantity[iAxis]=="vsEnergy"){
                  leg1 -> AddEntry(graph1, ETAranges[kk]);
               }
               else{
                  leg1 -> AddEntry(graph1, ETranges[0][kk]);
               }
               leg1 -> SetTextSize(0.025);
               leg1 -> SetLineColor(0);
               leg1 -> SetFillColorAlpha(0, 0);
               leg1 -> SetBorderSize(0);
               leg1 -> Draw("same");
               if(do_ratioPlot){
                  TGraphAsymmErrors* graph2 = getGraph(whichPlot, fileName[1], map_quantity[1], map_quantity_error[1], kk, false, do_EB[1], do_EE[1], do_binningEt, use_simEnergy, ETranges[1], ETAranges, ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).graph; 
                  if(do_autoScale){
                     graph2->GetYaxis()->SetRangeUser(min_range2-0.05*(max_range2-min_range2), max_range2+0.8*(max_range2-min_range2));
                  }

                  pad2->cd();
                  if(kk==0){
                     graph2->Draw("A*");
                  }
                  else{
                     graph2->Draw("*, same");
                  }
                  leg1->Draw("same");

                  TGraphAsymmErrors* graph3 = getRatioGraph(whichPlot, fileName[0], fileName[1], 1,  map_quantity, map_quantity_error, kk, do_EB[0], do_EE[0], do_binningEt, use_simEnergy, ETranges[0], ETAranges, ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).graph; 
                  if(do_autoScale){
                     graph3->GetYaxis()->SetRangeUser(min_rangeR-0.05*(max_rangeR-min_rangeR), max_rangeR+0.05*(max_rangeR-min_rangeR));
                  }

                  pad3->cd();
                  if(kk==0){
                     graph3->Draw("A*");
                  }
                  else{
                     graph3->Draw("*, same");
                  }
               }
            }
            TPaveText* label = new TPaveText(0.7,0.85,0.8,0.88,"brNDC");
            label->SetBorderSize(0);
            label->SetFillColor(kWhite);
            label->SetTextSize(0.025);
            label->SetTextFont(42);
            label->SetTextAlign(11);
            if(xAxisQuantity[iAxis]=="vsEnergy"){
               label->AddText("#eta bins: ");
            }
            else{
               label->AddText("Energy bins: ");
            }
            if(do_ratioPlot){
               pad1->cd();
            }
            label->Draw("same");
            if(do_ratioPlot){
               pad2->cd();
               label->Draw("same");
            }

            TPaveText* label_info_up = new TPaveText(0.15,0.73,0.45,0.85,"brNDC");
            label_info_up->SetBorderSize(0);
            label_info_up->SetFillColor(kWhite);
            label_info_up->SetTextSize(0.028);
            label_info_up->SetTextFont(42);
            label_info_up->SetTextAlign(11);
            TString nEvents = to_string(kEvents[0]);
            label_info_up->AddText(nEvents + "k Events, " + PUtag[0]);
            label_info_up->AddText(matching[0] + " matching");
            if(do_ratioPlot){
               pad1->cd();
            }
            label_info_up->Draw("same");

            TPaveText* label_info_down = new TPaveText(0.15,0.73,0.45,0.85,"brNDC");
            label_info_down->SetBorderSize(0);
            label_info_down->SetFillColor(kWhite);
            label_info_down->SetTextSize(0.028);
            label_info_down->SetTextFont(42);
            label_info_down->SetTextAlign(11);
            TString nEvents_down = to_string(kEvents[1]);
            if(do_ratioPlot){
               label_info_down->AddText(nEvents_down + "k Events, " + PUtag[1]);
               label_info_down->AddText(matching[1] +  " matching");
               pad2->cd();
               label_info_down->Draw("same");
            }

            TPaveText* label_thrs_up = new TPaveText(0.15,0.60,0.45,0.72,"brNDC");
            label_thrs_up->SetBorderSize(0);
            label_thrs_up->SetFillColor(kWhite);
            label_thrs_up->SetTextSize(0.028);
            label_thrs_up->SetTextFont(42);
            label_thrs_up->SetTextAlign(11);
            label_thrs_up->AddText(pfrechit_thrs[0] + " " + dependency[0]);
            if(seeding_thrs[0]=="seedRef"){
               label_thrs_up->AddText(seeding_thrs[0]);
            }
            else{
               label_thrs_up->AddText(seeding_thrs[0] + " " + dependency[0]);
            }
            if(do_ratioPlot){
               pad1->cd();
            }
            label_thrs_up->Draw("same");

            TPaveText* label_thrs_down = new TPaveText(0.15,0.60,0.45,0.72,"brNDC");
            label_thrs_down->SetBorderSize(0);
            label_thrs_down->SetFillColor(kWhite);
            label_thrs_down->SetTextSize(0.028);
            label_thrs_down->SetTextFont(42);
            label_thrs_down->SetTextAlign(11);
            if(do_ratioPlot){
               label_thrs_down->AddText(pfrechit_thrs[1] + " " + dependency[1]);
               if(seeding_thrs[1]=="seedRef"){
                  label_thrs_down->AddText(seeding_thrs[1]);
               }
               else{
                  label_thrs_down->AddText(seeding_thrs[1] + " " + dependency[1]);
               }  
               pad2->cd();
               label_thrs_down->Draw("same");
            }


            TString dir = outputdir[0].c_str();
            TString nameSave;
            if(whichPlot=="Resolution"){
               nameSave = "resolution";
            }
            else if(whichPlot=="Scale"){
               nameSave = "scale";
            }
            else if(whichPlot=="Efficiency"){
               nameSave = "efficiency";
            }
            else if(whichPlot=="FakeRate"){
               nameSave = "fakeRate";
            }

            TString xAxisName;
            if(xAxisQuantity[iAxis]=="vsEnergy"){
               xAxisName = "energy";
            }
            else if(xAxisQuantity[iAxis]=="vsEta"){
               xAxisName = "eta";
            }

            c1->cd();
            c1->SaveAs(dir + nameSave + "_vs_" + xAxisName + ".png");
            c1->SaveAs(dir + nameSave + "_vs_" + xAxisName + ".pdf");

         }
      }




