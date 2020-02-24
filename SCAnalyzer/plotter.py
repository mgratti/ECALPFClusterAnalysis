import ROOT
import sys
import os
import array
import argparse
sys.path.append('/work/mratti/plotting/myplotting')
from spares import *

def doROCAnalysis(inputfile, outputdir, variables={'R9':'R9'}):

  os.system('mkdir -p {}'.format(outputdir))

  # open file and get histograms
  f=ROOT.TFile.Open(inputfile,'READ')
  if not f: raise RuntimeError('file not found')

  gROCs = []

  for var,varname in variables.items():
    
    gROC = ROOT.TGraph()
    gROCs.append(gROC)    

    hgood = f.Get('SClusters/h_goodSC_{}'.format(var))
    hfake = f.Get('SClusters/h_fakeSC_{}'.format(var))
 
    if not hgood or not hfake: 
      raise RuntimeError('histogram missing, {}, {}'.format(hgood,hfake))

    # set the style
    hgood.SetLineColor(ROOT.kRed)
    hgood.SetFillColor(ROOT.kRed)
    hgood.SetFillColorAlpha(ROOT.kRed, 0.5)
    hgood.SetMarkerColor(ROOT.kRed)
    hgood.SetMarkerStyle(41)
    hfake.SetLineColor(ROOT.kBlue)
    hfake.SetFillColor(ROOT.kBlue)
    hfake.SetFillColorAlpha(ROOT.kBlue, 0.5)
    hfake.SetMarkerColor(ROOT.kBlue)
    hfake.SetMarkerStyle(43)

    hgood.GetXaxis().SetTitle(varname)
    hgood.GetYaxis().SetTitle('a.u.')

    c = ROOT.TCanvas()
    c.SetLogy()
    hgood.Draw('hist')
    hfake.Draw('histsame')

    leg = ROOT.TLegend() 
    leg=defaultLegend(x1=0.6,y1=0.7,x2=0.95,y2=0.90)
    leg.AddEntry(hgood, 'signal #gamma', 'F')   
    leg.AddEntry(hfake, 'fake #gamma', 'F')   
    leg.Draw('same')

    c.SaveAs(outputdir + '/h_' + var + '.pdf')
    c.SaveAs(outputdir + '/h_' + var + '.png')
    c.SaveAs(outputdir + '/h_' + var + '.root')

    # now get the roc curve
    for i in range(hgood.GetNbinsX()):
      # NOTE: assumes that cut will require var < threshold
      # TODO: check other way around
      eff_sig = hgood.Integral(1, i + 1) / hgood.Integral()
      eff_bkg = hfake.Integral(1, i + 1) / hfake.Integral()
      #eff_sig = hgood.Integral(i, hgood.GetNbinsX()+1) # not ok
      #eff_bkg = hgood.Integral(i, hfake.GetNbinsX()+1) # not ok
      gROC.SetPoint(i, 1.0 - eff_bkg, eff_sig)
    gROC.GetXaxis().SetRangeUser(0.0, 1.1)
    gROC.GetYaxis().SetRangeUser(0.0, 1.1)
    gROC.GetXaxis().SetTitle('Bkg rej')
    gROC.GetYaxis().SetTitle('Sig eff')
    gROC.SetLineWidth(2)
    gROC.SetMarkerStyle(24)
    
    cROC = ROOT.TCanvas()
    gROC.Draw('ACP')

    leg = ROOT.TLegend()
    leg=defaultLegend(x1=0.6,y1=0.7,x2=0.95,y2=0.90)
    leg.AddEntry(gROC, varname, 'LP')
    leg.Draw('same')

    cROC.SaveAs(outputdir + '/roc_'+ var + '.pdf')
    cROC.SaveAs(outputdir + '/roc_'+ var + '.png')
    cROC.SaveAs(outputdir + '/roc_'+ var + '.root')

  return gROCs

def getOptions():

  parser = argparse.ArgumentParser(description='plotter for good vs fake photon study', add_help=True)
  parser.add_argument('--pl1', type=str, dest='pl1', help='production label for analysis 1', default='photon_bla')
  # TODO: in the future could be a comma separated list of production labels
  args = parser.parse_args()
  return args

if __name__ == "__main__":

  ROOT.gROOT.SetBatch(True)
  ROOT.gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyle.C')
  ROOT.gROOT.ProcessLine('setTDRStyle()')
  ROOT.gStyle.SetOptStat(000000) # remove all stats

  opt = getOptions()

  variables = {
    'R9': 'r_{9}',
    'SigmaIetaIeta' : '#sigma_{i#eta i#eta}',
    'SigmaIphiIphi' : '#sigma_{i#phi i#phi}',
    'fullR9' : 'r_{9} (full 5x5)',
    'fullSigmaIetaIeta' : '#sigma_{i#eta i#eta} (full 5x5)' ,
    'fullSigmaIphiIphi' : '#sigma_{i#phi i#phi} (full 5x5)',
  }

  # do analysis for pl1
  inputfile = './outputfiles/' + opt.pl1 + '.root'
  outputdir = './plots/' + opt.pl1 
 
  gROCs = doROCAnalysis(inputfile=inputfile, outputdir=outputdir, variables=variables) 
  
  #plotROC(gROC)
