import ROOT


def getGraph(inputfileName,graphName,doSmoothing=False):

  f = ROOT.TFile.Open(inputfileName,'r')
  graph = f.Get(graphName)
  if graph==None: raise RuntimeError('no graph was found')
  #graph.SetDirectory(0)

  if doSmoothing:
    pass

  return graph

def saveGraphs(outputfileName, graphs):

  f = ROOT.TFile(outputfileName,'RECREATE')

  for ig in graphs:
    ig.Write()
    


def mergeGraphs(g1,g2,mult=1):

  g = ROOT.TGraph()
  g.SetName('merged_{m}'.format(m=mult))
  x,y=ROOT.Double(),ROOT.Double()

  for i in range(1,g1.GetN()+1):
    g1.GetPoint(i,x,y)
    g.SetPoint(i,x,mult*y)

  for j in range(g1.GetN()+1,g1.GetN()+1+g2.GetN()+1):
    g2.GetPoint(j-g1.GetN(),x,y)
    g.SetPoint(j,x,y*mult)

  return g

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

  mergedGraph1 = mergeGraphs(g1=graphEB,g2=graphEE,mult=1)  
  mergedGraph2 = mergeGraphs(g1=graphEB,g2=graphEE,mult=2)  
  mergedGraph3 = mergeGraphs(g1=graphEB,g2=graphEE,mult=3)  

  saveGraphs(outputfileName,graphs=[mergedGraph1, mergedGraph2, mergedGraph3])


  #c = ROOT.TCanvas()
  #mergedGraph.SetLineColor(ROOT.kRed)
  #mergedGraph.Draw('AP')
  #c.SaveAs('graph.pdf')
