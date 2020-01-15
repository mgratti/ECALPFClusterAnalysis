import ROOT

graph_saver = []

def getGraph(inputfileName,graphName,doSmoothing=False):

  f = ROOT.TFile.Open(inputfileName,'r')
  graph = f.Get(graphName)
  if graph==None: raise RuntimeError('no graph was found')
  #graph.SetDirectory(0)
  

  if doSmoothing:
    global graph_saver
    gs = ROOT.TGraphSmooth("normal")
    graph_saver.append(gs)
    grout = gs.Approx(graph,"linear")
    grout.SetName('smooth')
    graph_saver.append(grout) # because grout is a pointer, and in python you can't just return a pointer if the original object is destroyed
    return grout

  return graph

def saveGraphs(outputfileName, graphs):

  f = ROOT.TFile(outputfileName,'RECREATE')

  for ig in graphs:
    ig.Write()
    


def mergeGraphs(g1,g2,suffix='',mult=1):

  g = ROOT.TGraph()
  g.SetName('merged_{s}_{m}'.format(s=suffix,m=mult))
  x,y=ROOT.Double(),ROOT.Double()

  for i in range(1,g1.GetN()+1):
    g1.GetPoint(i,x,y)
    g.SetPoint(i,x,mult*y)

  for j in range(g1.GetN()+1,g1.GetN()+1+g2.GetN()+1):
    g2.GetPoint(j-g1.GetN(),x,y)
    g.SetPoint(j,x,y*mult)

  return g

def smoothGraph(graph):
  # create a histogram with enough points
  h = ROOT.TH1D('h','h', 3000,0.,3.)
  for i in range(1,graph.GetN()+1):
    x,y=ROOT.Double(),ROOT.Double()
    graph.GetPoint(i,x,y)
    h.Fill(x,y)

  h.Smooth()

  return h

def testGraphs(cname, g1, g2):

  c = ROOT.TCanvas()
  g1.SetMarkerColor(ROOT.kRed)
  g1.SetMarkerStyle(22)
  g2.SetMarkerStyle(24)

  mg = ROOT.TMultiGraph()
  mg.SetTitle("mg")
  mg.Add(g1)
  mg.Add(g2)
  mg.Draw("AP")
  c.SetLogy()
  c.SaveAs(cname+'.pdf')

if __name__ == "__main__":

  ROOT.gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyle.C')
  ROOT.gROOT.ProcessLine('setTDRStyle()')
  ROOT.gROOT.SetBatch(True)

  from argparse import ArgumentParser
  parser = ArgumentParser(description='', add_help=True)

  parser.add_argument('-t', '--tag', type=str, dest='tag', help='')
  options = parser.parse_args()

  inputfileName = '{t}/graphs.root'.format(t=options.tag)
  outputfileName = '{t}/mergedGraphs.root'.format(t=options.tag)

  graphEB = getGraph(inputfileName,graphName='gr_EB_Object',doSmoothing=False)
  graphEE = getGraph(inputfileName,graphName='gr_EE_Object',doSmoothing=False)
  graphEB_smooth = getGraph(inputfileName,graphName='gr_EB_Object',doSmoothing=True)
  graphEE_smooth = getGraph(inputfileName,graphName='gr_EE_Object',doSmoothing=True)

  mergedGraph1 = mergeGraphs(g1=graphEB,g2=graphEE,mult=1)  
  mergedGraph2 = mergeGraphs(g1=graphEB,g2=graphEE,mult=2)  
  mergedGraph3 = mergeGraphs(g1=graphEB,g2=graphEE,mult=3)  

  mergedGraph1_smooth = mergeGraphs(g1=graphEB_smooth,g2=graphEE_smooth,suffix='smooth',mult=1)
  mergedGraph2_smooth = mergeGraphs(g1=graphEB_smooth,g2=graphEE_smooth,suffix='smooth',mult=2)
  mergedGraph3_smooth = mergeGraphs(g1=graphEB_smooth,g2=graphEE_smooth,suffix='smooth',mult=3)

  #testGraphs(cname='test', g1=mergedGraph1, g2=mergedGraph1_smooth)

  saveGraphs(outputfileName,graphs=[mergedGraph1, mergedGraph2, mergedGraph3, mergedGraph1_smooth, mergedGraph2_smooth, mergedGraph3_smooth])


