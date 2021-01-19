import ROOT
import sys
sys.path.append('/work/mratti/plotting/myplotting')
sys.path.append('../Plotter')
from spares import *

if __name__ == '__main__':

  ROOT.gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyle.C')
  ROOT.gROOT.ProcessLine('setTDRStyle()')
  ROOT.gROOT.SetBatch(True)

  tags = [ '2021', 'TL150', 'TL235', 'TL315', 'TL400', '2023', 'TL550']
  tags.reverse()
  colors = [   ROOT.kOrange+1, ROOT.kRed, ROOT.kMagenta+2, ROOT.kViolet+8, ROOT.kAzure-8, ROOT.kAzure+6 ,
                            ROOT.kGreen+1, ROOT.kSpring+4, ROOT.kYellow -5, ROOT.kYellow -3, ROOT.kYellow, ROOT.kOrange
                                              ]

  filename = 'PFRecHitThresholds_EB_ringaveraged_EE_{tag}/mergedGraphs.root'

  c = ROOT.TCanvas()
  leg = ROOT.TLegend
  leg=defaultLegend(x1=0.6,y1=0.7,x2=0.95,y2=0.90)
  #leg.AddEntry(hgood, 'signal SC', 'F')
  #leg.AddEntry(hfake, 'fake SC', 'F')

  for i,tag in enumerate(tags):
    f = ROOT.TFile.Open(filename.format(tag=tag), 'READ')
    if not f: raise RuntimeError('file not found')
    graph = f.Get('merged_smooth_1')
    graph.SetLineColor(colors[i]) 
    graph.SetLineWidth(2)
    leg.AddEntry(graph, tag, 'LP')
    if i==0:
      graph.Draw('AL')
    else:
      graph.Draw('same')

  c.SetLogy()    
  c.SetGridx()
  c.SetGridy()
  leg.Draw('same')

  c.SaveAs('noise1sigma_comparison.pdf')

