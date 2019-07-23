from ROOT import gROOT, TH1F, TH1, kBlack, kRed, kBlue, TCanvas, gStyle, TLegend, TLatex, TFile, TChain, TPad, kWhite

def makeHistSettings(h):
  h.GetYaxis().SetNdivisions(510);
  h.GetYaxis().SetLabelSize(0.06);
  h.GetXaxis().SetLabelSize(0.06);
  h.GetYaxis().SetTitleSize(0.06);
  h.GetXaxis().SetTitleSize(0.08);
  h.GetYaxis().SetTitleOffset(0.96);
  h.GetXaxis().SetTitleOffset(1.0);
  h.GetYaxis().SetLabelOffset(0.01);
  h.GetXaxis().SetLabelOffset(0.01);
  h.GetXaxis().SetTickLength(0.06);
  #h.SetMarkerStyle(1)

def makeRatioSettings(ratioMC):
  ratioMC.GetYaxis().SetRangeUser(0.75,1.25)
  #ratioMC.GetYaxis().SetNdivisions(510);
  ratioMC.GetYaxis().SetNdivisions(508);
  ratioMC.GetYaxis().SetLabelSize(0.15);
  ratioMC.GetXaxis().SetLabelSize(0.15);
  ratioMC.GetYaxis().SetTitleSize(0.15);
  ratioMC.GetXaxis().SetTitleSize(0.15);
  ratioMC.GetYaxis().SetTitleOffset(0.35);
  ratioMC.GetXaxis().SetTitleOffset(1.2);
  ratioMC.GetYaxis().SetLabelOffset(0.01);
  ratioMC.GetXaxis().SetLabelOffset(0.03);
  ratioMC.GetXaxis().SetTickLength(0.06);
  #ratioMC.SetFillColor(kBlack)
  ratioMC.SetFillStyle(3004)
 # ratioMC.SetLineColor( kBlack )
  #ratioMC.SetLineWidth(4 )
  ratioMC.SetMarkerStyle(1)
  ratioMC.SetMarkerColor(kBlack)
  ratioMC.SetLineWidth(2)

# hnum nominal, hden var1, hden2 var2
def makeRatioPlot(hNum, hDen, hDen2, nameNum="num", nameDen="den", nameDen2="den2", xtitle="pt",ytitle="Entries", ratiotitle="Ratio", norm="False"):

  # prepare settings of histos
  hNum.SetLineColor(kBlack)
  hNum.SetLineWidth(2)
  hNum.SetMarkerStyle(20)
  hNum.SetMarkerColor(kBlack)
  hNum.GetYaxis().SetTitle(ytitle)

  hDen.SetLineColor(kRed+2)
  hDen.SetMarkerColor(kRed+2)
  hDen.SetLineWidth(2)
  hDen.SetMarkerStyle(21)

  hDen2.SetLineColor(kBlue)
  hDen2.SetMarkerColor(kBlue)
  hDen2.SetLineWidth(2)
  hDen2.SetMarkerStyle(22)

  makeHistSettings(hNum)
  makeHistSettings(hDen)
  makeHistSettings(hDen2)#




  # prepare canva
  canvas=TCanvas()
  canvas.cd()
  yMinP1=0.305;
  bottomMarginP1=0.005;
  pad1 = TPad('pad1','pad1',0,yMinP1,0.99,1)
  pad1.SetLogy()
  pad1.SetBottomMargin(bottomMarginP1)
  pad1.SetFillColor(kWhite)
  pad1.SetTickx()
  pad1.SetTicky()
  pad2 = TPad('pad2','pad2',0.,0.01,.99,0.300)
  pad2.SetTopMargin(0.010)
  pad2.SetBottomMargin(0.40)
  pad2.SetFillColor(kWhite)
  pad2.SetGridy()
  pad1.SetNumber(1)
  pad2.SetNumber(2)
  pad1.Draw()
  pad2.Draw()

  # prepare legend
  leg = TLegend(0.45,0.68,0.82,0.88,'')
  leg.SetBorderSize(0)
  leg.SetTextSize(0.05)


  # Draw
  pad1.cd()


  hNumNorm = hNum.Clone()
  if norm:
    hNumNorm = hNum.DrawNormalized('hist')
  else:
    hNum.Draw('hist')


  #leg.Draw('same')

  hDenNorm = hDen.Clone()
  if norm:
    hDenNorm = hDen.DrawNormalized('samehist')
  else:
    hDen.Draw('samehist')

  hDenNorm2 = hDen2.Clone()
  if norm:
    hDenNorm2 = hDen2.DrawNormalized('samehist')
  else:
    hDen2.Draw('LP')

  leg.AddEntry(hDen2, nameDen, 'L')
  leg.AddEntry(hDen, nameDen, 'L')
  leg.AddEntry(hNum, nameNum, 'L')
  leg.Draw('same')



  #leg.Draw('same')

  pad2.cd()

  hRatio = hDen.Clone()
  hRatio.SetLineColor(kRed+2)
  hRatio.Divide(hDen2)
  makeRatioSettings(hRatio)
  hRatio.GetXaxis().SetTitle(xtitle)
  hRatio.GetYaxis().SetTitle(ratiotitle)
  hRatio.SetTitle('')
  #hRatio2 = hNum.Clone()
  #hRatio2.SetLineColor(kBlue)
  #hRatio2.Divide(hDen2)
  #makeRatioSettings(hRatio2)




  hRatio.Draw('LP')
  #hRatio2.Draw('LPsame')
  #hRatio.GetXaxis().SetRangeUser(200.,2000.)

  canvas.Update()
  canvas.SaveAs('plots/ratio_{}_{}.pdf'.format(hNum.GetName(), hDen.GetName()))
  canvas.SaveAs('plots/ratio_{}_{}.C'.format(hNum.GetName(), hDen.GetName()))
  #canvas.SaveAs('plots/ratio_{}_{}.root'.format(hNum.GetName(), hDen.GetName()))
  canvas.Close()
