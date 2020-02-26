import ROOT
import sys
import os
import array
import argparse
sys.path.append('/work/mratti/plotting/myplotting')
from spares import *

colors =[   
    ROOT.kOrange+1, ROOT.kRed, ROOT.kMagenta+2, ROOT.kViolet+8, ROOT.kAzure-8, ROOT.kAzure+6 ,
    ROOT.kGreen+1, ROOT.kSpring+4, ROOT.kYellow -5, ROOT.kYellow -3, ROOT.kYellow, ROOT.kOrange
    ]


'''
Performs the individual plot of sig vs bkg, calculates and plots the ROC, returns the ROC
'''
def doBinnedROCAnalysis(inputfile, outputdir, var, varname, det, detname, doNorm):
  gROC = ROOT.TGraph()

  f= ROOT.TFile.Open(inputfile,'READ')
  if not f: raise RuntimeError('file not found')

  hgood_temp = f.Get('SClusters/h_goodSC_{}_{}'.format(var,det))
  hfake_temp = f.Get('SClusters/h_fakeSC_{}_{}'.format(var,det))

  # get overflowed histo
  hgood = getOverflowedHisto(hgood_temp)
  hfake = getOverflowedHisto(hfake_temp)
   
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
  #c.SetLogy()

  max_y = max(hgood.GetMaximum(),hfake.GetMaximum())
  hgood.GetYaxis().SetRangeUser(0,max_y*1.4)

  if not doNorm:
    hgood.Draw('hist')
    hfake.Draw('histsame')
  else:
    hgood.DrawNormalized('hist')
    hfake.DrawNormalized('histsame')

  leg = ROOT.TLegend() 
  leg=defaultLegend(x1=0.6,y1=0.7,x2=0.95,y2=0.90)
  leg.AddEntry(hgood, 'signal #gamma', 'F')   
  leg.AddEntry(hfake, 'fake #gamma', 'F')   
  leg.Draw('same')

  # Tlatex with detector information
  defaultLabels([detname], x=0.6, y=0.65, spacing = 0.04, size = 0.03, dx = 0.12)

  c.SaveAs(outputdir + '/h_' + var + '_' + det + '.pdf')
  c.SaveAs(outputdir + '/h_' + var + '_' + det + '.png')
  c.SaveAs(outputdir + '/h_' + var + '_' + det + '.root')

  # now get the roc curve
  for i in range(hgood.GetNbinsX()):
    # NOTE: assumes that cut will require var < threshold
    # TODO: check other way around
    eff_sig = hgood.Integral(1, i + 1) / hgood.Integral()
    eff_bkg = hfake.Integral(1, i + 1) / hfake.Integral()
    if 'R9' in var:
      eff_sig = hgood.Integral(i+1, hgood.GetNbinsX()+1) / hgood.Integral()
      eff_bkg = hfake.Integral(i+1, hfake.GetNbinsX()+1) / hfake.Integral()

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
  leg=defaultLegend(x1=0.6,y1=0.8,x2=0.95,y2=0.90)
  leg.AddEntry(gROC, varname, 'LP')
  leg.Draw('same')
  defaultLabels([detname], x=0.7, y=0.7, spacing = 0.04, size = 0.03, dx = 0.12)

  cROC.SaveAs(outputdir + '/roc_'+ var + '_' + det + '.pdf')
  cROC.SaveAs(outputdir + '/roc_'+ var + '_' + det + '.png')
  cROC.SaveAs(outputdir + '/roc_'+ var + '_' + det + '.root')


  return gROC

'''
Calls individual ROC analysis  over variables and detectors and returns a dictionary of graphs
'''
def doROCAnalysis(inputfile, outputdir, variables={'R9':'R9'}, dets={'all':'EB+EE'},doNorm=False):

  os.system('mkdir -p {}'.format(outputdir))

  gROCs = {}

  for det,detname in dets.items():
    gROCs[det]={}
    
    for var,varname in variables.items():
      gROCs[det][var] =  doBinnedROCAnalysis(inputfile, outputdir, var, varname, det, detname, doNorm)

  return gROCs

def plotROCcomparison(gROCs,  variables={'R9':'R9'}, dets={'all':'EB+EE'}):

  for det,detname in dets.items():
    for var,varname in variables.items():
      c = ROOT.TCanvas()
      leg = ROOT.TLegend()
      leg=defaultLegend(x1=0.6,y1=0.8,x2=0.95,y2=0.90)
      for i,lab in enumerate(gROCs.keys()):
        gROCs[lab][det][var].SetLineColor(colors[i])
        gROCs[lab][det][var].SetMarkerColor(colors[i])
        # marker style 
        if i==0:
          gROCs[lab][det][var].Draw('ACP')
        else:
          gROCs[lab][det][var].Draw('same')
          
        leg.AddEntry(gROCs[lab][det][var], lab , 'LP')
  
      leg.Draw('same')
      defaultLabels([detname,varname], x=0.7, y=0.7, spacing = 0.04, size = 0.03, dx = 0.12)
      c.SaveAs('./plots/roc_comparison_{var}_{det}_{l1}_{l2}.pdf'.format(var=var, det=det, l1=gROCs.keys()[0],l2=gROCs.keys()[1]))
      c.SaveAs('./plots/roc_comparison_{var}_{det}_{l1}_{l2}.png'.format(var=var, det=det, l1=gROCs.keys()[0],l2=gROCs.keys()[1]))
      c.SaveAs('./plots/roc_comparison_{var}_{det}_{l1}_{l2}.root'.format(var=var, det=det, l1=gROCs.keys()[0],l2=gROCs.keys()[1]))


def getOptions():

  parser = argparse.ArgumentParser(description='plotter for good vs fake photon study', add_help=True)
  parser.add_argument('--pl1', type=str, dest='pl1', help='production label for analysis 1', default='photon_bla')
  parser.add_argument('--pl2', type=str, dest='pl2', help='production label for analysis 2', default='photon_bla')
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

  dets = {
    'all' : 'EB+EE',
    'EB'  : 'EB, |eta|<1.44',
    'EEclose' : 'EE, 1.48<|#eta|<2.2',
    'EEfar' : 'EE, |#eta|>2.2',
  }

  # do analysis for pl1
  inputfile = './outputfiles/' + opt.pl1 + '.root'
  outputdir = './plots/' + opt.pl1 
 
  gROCs_pl1 = doROCAnalysis(inputfile=inputfile, outputdir=outputdir, variables=variables, dets=dets) 
  print gROCs_pl1

  # do analysis for pl2
  inputfile = './outputfiles/' + opt.pl2 + '.root'
  outputdir = './plots/' + opt.pl2 
  
  gROCs_pl2 = doROCAnalysis(inputfile=inputfile, outputdir=outputdir, variables=variables, dets=dets) 
  
  gROCs = {}
  gROCs[opt.pl1] = gROCs_pl1
  gROCs[opt.pl2] = gROCs_pl2

  plotROCcomparison(gROCs,variables,dets)
