/************************************

  - This function is needed to produce scan plots of resolution/scale/efficiency/fakeRate (=Quantity) with different threshold values 

  - It produces the following plots:
  - 
  -  Quantity as a function of the energy bin, at a given eta bin, scanning on the different threshold values
  -  Quantity as a function of the eta bin, at a given energy bin, scanning on the different threshold values
  -  Same as above but quantity ratio wrt to a ref quantity (the first tuning value) instead of plain quantity

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



#include "/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/macros/objects.h"


void produceScanPlots(TString whichPlot, 
      vector<string> fileName, 
      vector<map<TString, map<TString, Float_t>>> map_quantity, 
      vector<map<TString, map<TString, vector<Float_t>>>> map_quantity_error, 
      Bool_t do_ratioPlot, 
      vector<Bool_t> do_EB, 
      vector<Bool_t> do_EE, 
      Bool_t do_binningEt, 
      Bool_t use_simEnergy, 
      Bool_t do_autoScale, 
      vector<vector<TString>> ETranges, 
      vector<vector<TString>> ETAranges, 
      map<TString, Edges> ETvalue, 
      map<TString, Edges> ETAvalue, 
      map<int, EColor> color, 
      string outputdir, 
      vector<Int_t> kEvents, 
      vector<TString> matching, 
      vector<TString> PUtag, 
      vector<TString> pfrechit_thrs, 
      vector<TString> seeding_thrs, 
      vector<TString> dependency)
{

   // two kinds of plots will be produced: xAxis as energy bins and different graph (with associated color) for the eta bins 
   // and vice versa
   // The following quantity indices which binning is on the xAxis
   vector<string> xAxisQuantity = {"vsEnergy", "vsEta"};

   for(unsigned int iAxis(0); iAxis<xAxisQuantity.size(); ++iAxis){

      unsigned int xSize;
      if(xAxisQuantity[iAxis]=="vsEnergy"){
         xSize = ETAranges[0].size();
      }
      else if(xAxisQuantity[iAxis]=="vsEta"){
         xSize = ETranges[0].size();
      }

      for(unsigned int kk(0); kk<xSize; ++kk){
         TCanvas* c1 = new TCanvas("c1", "c1", 700, 600);
         TLegend* leg1 = new TLegend(0.55, 0.65, 0.9, 0.83);

         TCanvas* cr = new TCanvas("cr", "cr", 700, 600);
         TLegend* legr = new TLegend(0.55, 0.65, 0.9, 0.83);

         vector<float> vector_range1;
         vector<float> vector_rangeR;
         for(unsigned int ll(0); ll<fileName.size(); ++ll){
            vector<float> range_graph1 = getGraph(whichPlot, fileName[ll], map_quantity[ll], map_quantity_error[ll], kk, true, do_EB[ll], do_EE[ll], do_binningEt, use_simEnergy, ETranges[ll], ETAranges[ll], ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).range; 

            for(unsigned int i(0); i<range_graph1.size(); ++i){
               vector_range1.push_back(range_graph1[i]);
            }

            vector<float> range_graphR= getRatioGraph(whichPlot, fileName[ll], fileName[0], ll,  map_quantity, map_quantity_error, kk, do_EB[ll], do_EE[ll], do_binningEt, use_simEnergy, ETranges[ll], ETAranges[ll], ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).range; 

            for(unsigned int i(0); i<range_graphR.size(); ++i){
               vector_rangeR.push_back(range_graphR[i]);
            }
         }

         float max_range1;
         float min_range1; 

         if(vector_range1.size()!=0){
            max_range1 = *max_element(vector_range1.begin(), vector_range1.end());
            min_range1 = *min_element(vector_range1.begin(), vector_range1.end());
         }

         float max_rangeR;
         float min_rangeR; 

         if(vector_rangeR.size()!=0){
            max_rangeR = *max_element(vector_rangeR.begin(), vector_rangeR.end());
            min_rangeR = *min_element(vector_rangeR.begin(), vector_rangeR.end());
         }

         for(unsigned int ll(0); ll<fileName.size(); ++ll){
            TGraphAsymmErrors* graph1;
            graph1 = getGraph(whichPlot, fileName[ll], map_quantity[ll], map_quantity_error[ll], kk, true, do_EB[ll], do_EE[ll], do_binningEt, use_simEnergy, ETranges[ll], ETAranges[ll], ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).graph; 
            if(do_autoScale){
               graph1->GetYaxis()->SetRangeUser(min_range1-0.05*(max_range1-min_range1), max_range1+0.8*(max_range1-min_range1));
            }

            graph1->SetLineColor(color[ll]);
            graph1->SetMarkerColor(color[ll]);
            c1->cd();
            if(ll==0){
               graph1->Draw("A*");
            }
            else{
               graph1->Draw("*, same");
            }

            if(seeding_thrs[ll]=="seedRef"){
               leg1 -> AddEntry(graph1, pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll]);
            }
            else{
               leg1 -> AddEntry(graph1, pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll] + " " + dependency[ll]);
            }
            leg1 -> SetTextSize(0.025);
            leg1 -> SetLineColor(0);
            leg1 -> SetFillColorAlpha(0, 0);
            leg1 -> SetBorderSize(0);
            leg1 -> Draw("same");

            cr->cd();
            TGraphAsymmErrors* graph_ratio = getRatioGraph(whichPlot, fileName[ll], fileName[0], ll,  map_quantity, map_quantity_error, kk, do_EB[ll], do_EE[ll], do_binningEt, use_simEnergy, ETranges[ll], ETAranges[ll], ETvalue, ETAvalue, color, xAxisQuantity[iAxis]).graph; 

            if(do_autoScale){
               graph_ratio->GetYaxis()->SetRangeUser(min_rangeR-0.05*(max_rangeR-min_rangeR), max_rangeR+0.8*(max_rangeR-min_rangeR));
            }

            graph_ratio->SetLineColor(color[ll]);
            graph_ratio->SetMarkerColor(color[ll]);
            if(ll==0){
               graph_ratio->Draw("A*");
            }
            else{
               graph_ratio->Draw("*, same");
            }

            if(seeding_thrs[ll]=="seedRef"){
               if(ll==0){
                  legr -> AddEntry(graph_ratio, "ref: " + pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll]);
               }
               else{
                  legr -> AddEntry(graph_ratio, pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll]);
               }
            }
            else{
               if(ll==0){
                  legr -> AddEntry(graph_ratio, "ref: " + pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll] + " " + dependency[ll]);
               }
               else{
                  legr -> AddEntry(graph_ratio, pfrechit_thrs[ll] + " " + dependency[ll] + " - " + seeding_thrs[ll] + " " + dependency[ll]);
               }
            }
            legr -> SetTextSize(0.025);
            legr -> SetLineColor(0);
            legr -> SetFillColorAlpha(0, 0);
            legr -> SetBorderSize(0);
            legr -> Draw("same");

         }


         TPaveText* label = new TPaveText(0.63,0.85,0.8,0.88,"brNDC");
         label->SetBorderSize(0);
         label->SetFillColor(kWhite);
         label->SetTextSize(0.025);
         label->SetTextFont(42);
         label->SetTextAlign(11);
         if(xAxisQuantity[iAxis]=="vsEnergy"){
            label->AddText("#eta bin: " + ETAranges[0][kk]);
         }
         else if(xAxisQuantity[iAxis]=="vsEta"){
            label->AddText("Energy bin: " + ETranges[0][kk]);
         }
         c1->cd();
         label->Draw("same");
         cr->cd();
         label->Draw("same");

         TPaveText* label_info_up = new TPaveText(0.15,0.73,0.45,0.85,"brNDC");
         label_info_up->SetBorderSize(0);
         label_info_up->SetFillColor(kWhite);
         label_info_up->SetTextSize(0.028);
         label_info_up->SetTextFont(42);
         label_info_up->SetTextAlign(11);
         TString nEvents = to_string(kEvents[0]);
         label_info_up->AddText(nEvents + "k Events, " + PUtag[0]);
         label_info_up->AddText(matching[0] + " matching");
         c1->cd();
         label_info_up->Draw("same");
         cr->cd();
         label_info_up->Draw("same");

         TString dir = outputdir.c_str();
         TString nameSave;
         if(whichPlot=="Resolution"){
            nameSave = "resolution_";
         }
         else if(whichPlot=="Scale"){
            nameSave = "scale_";
         }
         else if(whichPlot=="Efficiency"){
            nameSave = "efficiency_";
         }
         nameSave += "eta_" + ETAranges[0][kk];

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
         delete c1;

         cr->cd();
         cr->SaveAs(dir + nameSave + "_vs_" + xAxisName + "_ratioScan.png");
         cr->SaveAs(dir + nameSave + "_vs_" + xAxisName + "_ratioScan.pdf");
         delete cr;

      }
   }
}



