from ROOT import TH1F, TGraph, TGraph2D, TCanvas, TLegend, TFile, TTree, gROOT, TF1, TLatex, gStyle, TH2D, gPad, TColor 
from ROOT import kRed, kBlue, kGray, kGreen, kPink, kYellow, kBlack, kWhite, kPink 
import glob 
import array 
import re 
import os 
import math 
import sys 

graph_saver = []

def defaultLabels(labels, x, y, spacing = 0.04, size = 0.027, dx = 0.12):
  l = TLatex()
  l.SetNDC()
  l.SetTextColor(kBlack)
  l.SetTextFont(42) #42, 72
  l.SetTextSize(0.027*1.1)
  for i in range(len(labels)):
    #if 'ATLAS' in labels[i]: ATLASlabel(x,y-spacing*i, dx)
    l.DrawLatex(x, y-spacing*i, labels[i])

def angleLabel(label, color, x, y, angle): #"m_{#eta} = m_{#chi}",kGray+2, x=0.3, y=0.5, angle=63) # arctan(2) , given the x,y ranges
  l = TLatex()
  l.SetNDC()
  l.SetTextColor(color)
  l.SetTextAngle(angle)
  l.SetTextFont(42)
  l.SetTextSize(0.027*1.1)
  l.DrawLatex(x, y, label)


def defaultLegend(x1, y1, x2, y2):
  legend = TLegend(x1, y1, x2, y2)
  legend.SetFillColor(0)
  legend.SetFillStyle(0)
  legend.SetLineColor(0)
  legend.SetBorderSize(0)
  legend.SetTextFont(42)
  legend.SetTextSize(0.025*1.6)
  global graph_saver
  graph_saver.append(legend)
  return legend

def defaultLegendGraph(x1, y1, x2, y2):
  legend = TLegend(x1, y1, x2, y2)
  legend.SetFillColor(0)
  legend.SetFillStyle(0)
  legend.SetLineColor(0)
  legend.SetBorderSize(0)
  legend.SetTextFont(42)
  legend.SetTextSize(0.025*1.2)
  global graph_saver
  graph_saver.append(legend)
  return legend

def getOverflowedHisto(h):
  htemp = h.Clone()
  nbins = htemp.GetNbinsX()
  last_plus_overflow = htemp.GetBinContent(nbins) + htemp.GetBinContent(nbins+1)
  last_plus_overflow_error = math.sqrt( htemp.GetBinError(nbins)*htemp.GetBinError(nbins)  + htemp.GetBinError(nbins+1)*htemp.GetBinError(nbins+1))
  htemp.SetBinContent(nbins,last_plus_overflow )
  htemp.SetBinError(nbins,last_plus_overflow_error)
  return htemp


if __name__ == "__main__":

  print 'hello world'
