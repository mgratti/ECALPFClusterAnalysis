
from ROOT import TH1F, TGraph, TGraph2D, TCanvas, TLegend, TFile, TTree, gROOT, TF1, TLatex, gStyle, TH2D, gPad, TColor,TMultiGraph, TH1, TGraphErrors, TGraph2DErrors
from ROOT import kRed, kBlue, kGray, kGreen, kPink, kYellow, kBlack, kWhite, kPink, kMagenta, kTRUE, kFALSE
import sys
import os
sys.path.append('/work/mratti/plotting/myplotting')
from spares import defaultLegendGraph, defaultLabels



def makeGraph(xs, xerrs, ys, yerrs, xtitle, ytitle, title, color, style):

  if len(xs)!=len(ys): raise RuntimeError('wrong size of parameters, please check your inputs')

  if yerrs==None:
    yerrs = [0 for i in ys ]
  if xerrs==None:
    xerrs = [0 for i in ys ]

  g = TGraphErrors()
  for i in range(0, len(xs)):
    #print 'point ', i
    g.SetPoint(i, xs[i], ys[i])
    g.SetPointError(i, xerrs[i], yerrs[i])


  g.SetMarkerColor(color)
  g.SetLineColor(color)
  g.SetMarkerStyle(style)
  g.GetXaxis().SetTitle(xtitle)
  g.GetYaxis().SetTitle(ytitle)
  g.SetTitle(title)

  return g


def make2DGraph(xs, xerrs, ys, yerrs, zs, zerrs, xtitle, ytitle, ztitle, title, color, style):

  if len(xs)!=len(ys) or len(xs)!=len(zs) or len(ys)!=len(zs)  : raise RuntimeError('wrong size of parameters, please check your inputs')

  if yerrs==None:
    yerrs = [0 for i in ys ]
  if xerrs==None:
    xerrs = [0 for i in xs ]
  if zerrs==None:
    zerrs = [0 for i in zs ]

  g = TGraph2DErrors()
  for i in range(0, len(xs)):
    #print 'point ', i
    g.SetPoint(i, xs[i], ys[i], zs[i])
    g.SetPointError(i, xerrs[i], yerrs[i], zerrs[i])


  g.SetMarkerColor(color)
  g.SetLineColor(color)
  g.SetMarkerStyle(style)
  g.GetXaxis().SetTitle(xtitle)
  g.GetYaxis().SetTitle(ytitle)
  g.GetZaxis().SetTitle(ztitle)
  g.SetTitle(title)

  return g


def makePlot(graphs, plotName, outputdir='.', xrange=None, yrange=None, labels=[]):

  gROOT.SetBatch(True)
  gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyleGraph.C')
  gROOT.ProcessLine('setTDRStyle()')

  c = TCanvas()
  leg=defaultLegendGraph(x1=0.65,y1=0.90-0.05*len(graphs),x2=0.85,y2=0.90)
  mg=TMultiGraph()

  for graph in graphs:
    mg.Add(graph, 'PE')
    leg.AddEntry(graph, graph.GetTitle(), 'PE')


    #if i==0:
    #  graph.Draw('AP')
    #else:
    #  graph.Draw('Psame')
    #leg.AddEntry(graph, graph.GetTitle(), 'P')

  #gROOT.SetStyle("Plain")

  mg.Draw('a')
  leg.Draw('same')

  mg.GetXaxis().SetTitle(graphs[0].GetXaxis().GetTitle())
  mg.GetYaxis().SetTitle(graphs[0].GetYaxis().GetTitle())
  # set axes appropriately
  #mg.GetXaxis().SetRangeUser( mg.GetXaxis().GetXmin() * 0.1 , mg.GetXaxis().GetXmax() * 10 )
  mg.GetXaxis().SetLimits(    mg.GetXaxis().GetXmin() * 0.8 ,  mg.GetXaxis().GetXmax() * 1.2)
  mg.GetYaxis().SetRangeUser( mg.GetYaxis().GetXmin() * 0.7 ,  mg.GetYaxis().GetXmax() * 1.3 )
  #mg.GetYaxis().SetRangeUser(0.03, 0.06)

  if xrange != None:
    mg.GetXaxis().SetLimits(xrange[0], xrange[1])
  if yrange != None:
    mg.GetYaxis().SetRangeUser(yrange[0], yrange[1])

  if len(labels)!=0:
    defaultLabels(labels, x=0.25, y=0.85, spacing = 0.04, size=0.05, dx=0.12)

  c.SaveAs(outputdir + '/' + plotName + '.pdf')
  c.SaveAs(outputdir + '/' + plotName + '.png')




def make2DPlot(histo, plotName, outputdir='.', xrange=None, yrange=None, zrange=None, option='errp0', option2='text'):

  gROOT.SetBatch(True)
  gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyle2D.C')
  gROOT.ProcessLine('setTDRStyle2D()')

  c = TCanvas()
  #leg=defaultLegendGraph(x1=0.65,y1=0.90-0.05*len(graphs),x2=0.85,y2=0.90)

  histo.Draw(option)
  if (option2 == 'text'): histo.SetMarkerSize(2)

  if option2!=None:
    histo.Draw(option2 +'same')
    #histo.Draw(option + 'same')

  #graph.GetXaxis().SetLimits(    graph.GetXaxis().GetXmin() * 0.8 ,  graph.GetXaxis().GetXmax() * 1.2)
  #graph.GetYaxis().SetRangeUser( graph.GetYaxis().GetXmin() * 0.7 ,  graph.GetYaxis().GetXmax() * 1.3 )
  #graph.GetZaxis().SetRangeUser( graph.GetZaxis().GetXmin() * 0.7 ,  graph.GetZaxis().GetXmax() * 1.3 )

  if xrange != None:
    histo.GetXaxis().SetLimits(xrange[0], xrange[1])
  if yrange != None:
    histo.GetYaxis().SetRangeUser(yrange[0], yrange[1])
  if zrange != None:
    histo.GetZaxis().SetRangeUser(zrange[0], zrange[1])

  c.SaveAs(outputdir + '/' + plotName + '.pdf')
  c.SaveAs(outputdir + '/' + plotName + '.png')


if __name__ == "__main__":

  gROOT.SetBatch(True)
  gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyle.C')
  gROOT.ProcessLine('setTDRStyle()')
  TH1.StatOverflows(kTRUE) # if false stats will be calculated without overflow, must be set also at filling time



  g1 = makeGraph( xs=[0.8, 0.9, 1.0], ys=[0.2, 0.3, 0.4], xtitle='Efficiency', ytitle='#sigma(E)/E', label='Et true = 9-10 GeV', color=kPink, style=20)
  g2 = makeGraph( xs=[0.76,0.88,0.9], ys=[0.24,0.29,0.31], xtitle='Efficiency', ytitle='#sigma(E)/E', label='Et true = 4-5 GeV', color=kBlue, style=21)
  # g2 = , g3 = ...
  graphs=[g1, g2]
  makePlot(graphs=graphs, plotName='EffVsReso')
