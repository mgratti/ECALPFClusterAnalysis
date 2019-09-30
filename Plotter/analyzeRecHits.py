## In this macro I must
# access to the histograms in a file
# make some gaussian fits / get mean, RMS, and other quantiles of the distribution for each bin
# store these values inside a dictionary and plot them as a function of eta
# must be done for barrel, EEM, EEP

import sys
import os
sys.path.append('/work/mratti/plotting/myplotting')
from spares import *
from array import *

from ROOT import TH1F, TGraph, TGraph2D, TCanvas, TLegend, TFile, TTree, gROOT, TF1, TLatex, gStyle, TH2D, gPad, TColor,TMultiGraph, TH1, TMath
from ROOT import kRed, kBlue, kGray, kGreen, kPink, kYellow, kBlack, kWhite, kPink, kMagenta, kTRUE, kFALSE
import glob
from array import *
import re
import os
import math



def beautify1DPlot(outputdir, inputfile, inputdir, histoname, xtitle, ytitle, xrange):
  
  gROOT.SetBatch(True)
  gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyle.C')
  gROOT.ProcessLine('setTDRStyle()')

  gStyle.SetOptStat('emMrRo')

  f=TFile(inputfile, 'READ')
  histo=f.Get('{}/{}'.format(inputdir,histoname))
  #histo=f.Get('{}'.format(histoname))
  c=TCanvas('c', 'c', 600,600)
  histo.SetLineWidth(2)
  histo.Draw('hist')
  #histo.Rebin(4)
  #c.SetLogy()
  histo.GetXaxis().SetTitle(xtitle)
  histo.GetYaxis().SetTitle(ytitle)
  if xrange!=None:
    histo.GetXaxis().SetRangeUser(xrange[0], xrange[1])
  c.SaveAs('{}/{}.png'.format(outputdir,histo.GetName()))
  c.SaveAs('{}/{}.pdf'.format(outputdir,histo.GetName()))
  c.SaveAs('{}/{}.C'.format(outputdir,histo.GetName()))
  del c

