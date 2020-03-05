import ROOT
import sys
import os
import array
import argparse
sys.path.append('/work/mratti/plotting/myplotting')
sys.path.append('../Plotter')
from spares import *
from analyzeRecHits import beautify1DPlot
#def beautify1DPlot(outputdir, inputfile, inputdir, histoname, xtitle, ytitle, xrange):


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

  hgood_temp = f.Get('SClusters/h_goodSC_{}{}'.format(var,det))
  hfake_temp = f.Get('SClusters/h_fakeSC_{}{}'.format(var,det))
  
  if not hgood_temp or not hfake_temp: 
    print 'hgood_temp or hfake_temp not available, SClusters/h_goodSC_{}{}'.format(var,det)
    return gROC 
  #  raise RuntimeError('histogram missing, {}, {}'.format(hgood,hfake))

  # get overflowed histo
  hgood = getOverflowedHisto(hgood_temp)
  hfake = getOverflowedHisto(hfake_temp)
   
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

  # draw the histograms 
  c = ROOT.TCanvas()
  cNorm = ROOT.TCanvas()
  cs=[c,cNorm]
  
  for i,c in enumerate(cs):
    c.cd()

    if i==0: # not norm
      norm = ''
      hgood.Draw('hist')
      hfake.Draw('histsame')
      max_y = max(hgood.GetMaximum(),hfake.GetMaximum())
      hgood.GetYaxis().SetRangeUser(0,max_y*1.4)
    elif i==1: # norm
      norm = '_norm'
      hgood.DrawNormalized('hist')
      hfake.DrawNormalized('histsame')

    leg = ROOT.TLegend() 
    leg=defaultLegend(x1=0.6,y1=0.7,x2=0.95,y2=0.90)
    leg.AddEntry(hgood, 'signal #gamma', 'F')   
    leg.AddEntry(hfake, 'fake #gamma', 'F')   
    leg.Draw('same')

    # Tlatex with detector information
    defaultLabels([detname], x=0.6, y=0.65, spacing = 0.04, size = 0.03, dx = 0.12)

    c.SaveAs(outputdir + '/h_' + var + det + norm + '.pdf')
    c.SaveAs(outputdir + '/h_' + var + det + norm + '.png')
    c.SaveAs(outputdir + '/h_' + var + det + norm + '.root')

  # now get the roc curve
  for i in range(hgood.GetNbinsX()):
    # TODO: check for other variables
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

  cROC.SaveAs(outputdir + '/roc_'+ var + det + '.pdf')
  cROC.SaveAs(outputdir + '/roc_'+ var + det + '.png')
  cROC.SaveAs(outputdir + '/roc_'+ var + det + '.root')


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

'''
Plots the comparison between rocs for specified variables and detectors
'''
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
  parser.add_argument('--pl1', type=str, dest='pl1', help='production label for analysis 1', default=None)
  parser.add_argument('--pl2', type=str, dest='pl2', help='production label for analysis 2', default=None)
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
    'kineta' : "#eta",
    'kinphi' : "#phi",
    'kinet' : "E_{T} (GeV)",
    'kinenergy' : "Energy (GeV)",
  }

  dets = {
    '_all' : 'EB+EE',
    '_EB'  : 'EB, |eta|<1.44',
    '_EEclose' : 'EE, 1.48<|#eta|<2.2',
    '_EEfar' : 'EE, |#eta|>2.2',
    '': ' ',
  }

  # do analysis for pl1
  inputfile = './outputfiles/' + opt.pl1 + '.root'
  outputdir = './plots/' + opt.pl1 
 
  gROCs_pl1 = doROCAnalysis(inputfile=inputfile, outputdir=outputdir, variables=variables, dets=dets, doNorm=True) 
  #print gROCs_pl1

  ## for pl1 only, also look at basic distributions...
  #beautify1DPlot(outputdir, inputfile, inputdir='SClusters', histoname='h_goodSC', xtitle='', ytitle='', xrange='')

  # do analysis for pl2
  if opt.pl2!=None:
    inputfile = './outputfiles/' + opt.pl2 + '.root'
    outputdir = './plots/' + opt.pl2 
  
    gROCs_pl2 = doROCAnalysis(inputfile=inputfile, outputdir=outputdir, variables=variables, dets=dets) 

    # now compare
    gROCs = {}
    gROCs[opt.pl1] = gROCs_pl1
    gROCs[opt.pl2] = gROCs_pl2

    plotROCcomparison(gROCs,variables,dets)



