import ROOT
import sys
import os
import array

colors = [1,2,6,7,5]
#colors = [ROOT.kBlack,  ROOT.kRed, ROOT.kMagenta, ROOT.kGray+1, ROOT.kViolet+1]
markers = [29,41,43,45,49]

def doPlotAnalysis(inputfile,outdir,evt=0,det='EB',pfclusters=[1,2]):

  os.system('mkdir -p {}'.format(outdir))

  strings = map(lambda x: str(x), pfclusters)
  analysis_name = 'evt{evt}_{det}_PFCl_{bla}'.format(bla='_'.join(strings), evt=evt, det=det)

  f=ROOT.TFile.Open(inputfile,'READ')
  if not f: raise RuntimeError('file not found')

  # now need to retrieve the histograms and set their styles
  pfcl_hitsfractions = []
  pfcl_hitsenergies = []
  pfcl_energy = []
  pfcl_bestcpenergy = []

  for ipfcl,idx in enumerate(pfclusters):

    pfcl_hitsenergies.append({})
    pfcl_hitsenergies[ipfcl]['histo'] = f.Get('evt_{evt}/{det}/PFCl_{idx}/{h}'.format(evt=evt,det=det,idx=idx,h='hits_energy'))
    #pfcl_hitsenergies[ipfcl]['drawopt'] = 'colz'

    if ipfcl==0:
      pfcl_energy_sum = pfcl_hitsenergies[0]['histo']
    else: 
      pfcl_energy_sum.Add(pfcl_hitsenergies[ipfcl]['histo'])

    pfcl_energy.append({})
    pfcl_energy[ipfcl]['histo'] = f.Get('evt_{evt}/{det}/PFCl_{idx}/{h}'.format(evt=evt,det=det,idx=idx,h='cl_energy'))
    pfcl_energy[ipfcl]['histo'].SetMarkerColor(colors[ipfcl]) 
    pfcl_energy[ipfcl]['histo'].SetLineColor(colors[ipfcl]) 
    #pfcl_energy[ipfcl]['histo'].SetFillColor(colors[ipfcl]) # 
    pfcl_energy[ipfcl]['drawopt'] = 'BOXsame'

    pfcl_hitsfractions.append({})
    pfcl_hitsfractions[ipfcl]['histo'] = f.Get('evt_{evt}/{det}/PFCl_{idx}/{h}'.format(evt=evt,det=det,idx=idx,h='hits_fraction'))
    pfcl_hitsfractions[ipfcl]['histo'].SetMarkerColor(colors[ipfcl])
    #pfcl_hitsfractions[ipfcl]['histo'].SetMarkerStyle(20)
    pfcl_hitsfractions[ipfcl]['histo'].SetMarkerSize(0.85)
    #print ipfcl, ipfcl % 2 == 0
    sign_bar_offset = 1 if ipfcl % 2 == True else -1
    bar_offset = sign_bar_offset * 0.2
    pfcl_hitsfractions[ipfcl]['histo'].SetBarOffset(bar_offset)
    pfcl_hitsfractions[ipfcl]['drawopt'] = 'TEXTsame'

    pfcl_bestcpenergy.append({})
    pfcl_bestcpenergy[ipfcl] = {}
    pfcl_bestcpenergy[ipfcl]['histo'] = f.Get('evt_{evt}/{det}/PFCl_{idx}/{h}'.format(evt=evt,det=det,idx=idx,h='bestCp_energy'))
    pfcl_bestcpenergy[ipfcl]['histo'].SetMarkerColor(colors[ipfcl])
    #pfcl_bestcpenergy[ipfcl]['histo'].SetMarkerColor(ROOT.kGreen+3)
    #pfcl_bestcpenergy[ipfcl]['histo'].SetFillColor(ROOT.kGreen+3)
    pfcl_bestcpenergy[ipfcl]['histo'].SetFillColorAlpha(colors[ipfcl], 0.35)
    pfcl_bestcpenergy[ipfcl]['histo'].SetMarkerStyle(markers[ipfcl])
    pfcl_bestcpenergy[ipfcl]['histo'].SetMarkerSize(0.8)
    pfcl_bestcpenergy[ipfcl]['drawopt'] = 'TEXTsame' 


  # draw first histogram
  c = ROOT.TCanvas()
  c.SetLogz()

  pfcl_energy_sum.Draw("colz")
  pfcl_energy_sum.GetZaxis().SetTitle('Energy (GeV)')
  pfcl_energy_sum.GetZaxis().SetTitleOffset(1.4)
  if det == 'EB':
    pfcl_energy_sum.GetXaxis().SetTitle('i#eta')
    pfcl_energy_sum.GetYaxis().SetTitle('i#phi')
  else:
    pfcl_energy_sum.GetXaxis().SetTitle('ix')
    pfcl_energy_sum.GetYaxis().SetTitle('iy')

  bin_center_x = pfcl_hitsfractions[0]['histo'].FindFirstBinAbove(0.000001, 1)
  bin_center_y = pfcl_hitsfractions[0]['histo'].FindFirstBinAbove(0.000001, 2)
  pfcl_energy_sum.GetXaxis().SetRange( bin_center_x-4,bin_center_x+9)
  pfcl_energy_sum.GetYaxis().SetRange( bin_center_y-4,bin_center_y+9)
  
  #draw all the rest 
  for ipfcl,idx in enumerate(pfclusters):
    #pfcl_bestcpenergy[ipfcl]['histo'].Draw(pfcl_bestcpenergy[ipfcl]['drawopt'])
    pfcl_energy[ipfcl]['histo'].Draw(pfcl_energy[ipfcl]['drawopt'])
    pfcl_hitsfractions[ipfcl]['histo'].Draw(pfcl_hitsfractions[ipfcl]['drawopt'])

    # draw a tlatex for each caloparticle
    latex = ROOT.TLatex()
    latex.SetTextSize(0.025)
    latex.SetTextAlign(13)
    x = pfcl_bestcpenergy[ipfcl]['histo'].FindFirstBinAbove(0,1)
    y = pfcl_bestcpenergy[ipfcl]['histo'].FindFirstBinAbove(0,2)
    x = pfcl_bestcpenergy[ipfcl]['histo'].GetXaxis().GetBinCenter(x) 
    y = pfcl_bestcpenergy[ipfcl]['histo'].GetYaxis().GetBinCenter(y) 
   
    string = "#color[{c}]{{#diamond}}".format(c=colors[ipfcl])
    latex.DrawLatex(x-ipfcl*0.05,y,string)
       

  c.SaveAs(outdir+'/'+analysis_name+'.pdf')
  c.SaveAs(outdir+'/'+analysis_name+'.png')
  c.SaveAs(outdir+'/'+analysis_name+'.C')
  c.SaveAs(outdir+'/'+analysis_name+'.root')

if __name__ == "__main__":

  ROOT.gROOT.SetBatch(True)
  ROOT.gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyle2D.C')
  ROOT.gROOT.ProcessLine('setTDRStyle2D()')
  ROOT.gStyle.SetOptStat(000000) # remove all stats
  ROOT.gStyle.SetPaintTextFormat("1.e");

  odir = './eventPlots/'

  # shower sigma varied
  variations = ['s1', 's2', 's0.5', 's10', 's0.1', 's100', 's0.01']

  for var in variations:
    print 'considering variation ', var
    inputfile = './outputfiles/T2_v1_t12_{}.root'.format(var)
    outdir=odir+inputfile.split('/')[2].split('.root')[0]
    doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=63,det='EB',pfclusters=[2,3])


  
#  inputfile = '../Analyzer/outputfiles/T2_v1_t12_EB.root'; 
#  outdir=odir+inputfile.split('/')[3].split('.root')[0]
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=5,det='EB',pfclusters=[5,6]) 
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=34,det='EB',pfclusters=[0,1]) 
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=61,det='EB',pfclusters=[0,1]) 
#  #doPlotAnalysis(inputfile=inputfile,evt=83,det='EB',pfclusters=[1,2]) 
#
#  inputfile = '../Analyzer/outputfiles/T2_v2_t12_EE.root'
#  outdir=odir+inputfile.split('/')[3].split('.root')[0]
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=1,det='EEP',pfclusters=[5,6,15,16])
#
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=42,det='EEM',pfclusters=[3,4])
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=95,det='EEM',pfclusters=[4,5])
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=99,det='EEM',pfclusters=[0,1])
#
#  # EB wPU
#  inputfile = '../Analyzer/outputfiles/T2_v4_t0_wPU_EB.root'
#  outdir=odir+inputfile.split('/')[3].split('.root')[0]
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=28, pfclusters=[51,52])
#
#  # am analysis
#  inputfile = '../Analyzer/outputfiles/T2_v1_t11_nj1_AM.root'
#  outdir=odir+inputfile.split('/')[3].split('.root')[0]
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=28,det='EB',pfclusters=[26,27,28,29])
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=28,det='EB',pfclusters=[30,31])
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=18,det='EB',pfclusters=[32,33])
#  
#  inputfile = '../Analyzer/outputfiles/T2_v1_t12_nj1_AM.root'
#  outdir=odir+inputfile.split('/')[3].split('.root')[0]
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=28,det='EB',pfclusters=[8])
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=28,det='EB',pfclusters=[9,10])
#  doPlotAnalysis(inputfile=inputfile,outdir=outdir,evt=18,det='EB',pfclusters=[10])


  #inputdir = 'perEvent'

