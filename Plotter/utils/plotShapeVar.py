import ROOT
from ratioPlot import *
import array
import glob
import os



class SampleProperty(object):
  def __init__(self,name,filename,treename ):
    self.name = name
    self.filename = filename
    self.treename = treename

    self.q1={
     'SR': '(n_jet)',
     'CR1e': '(n_jet)',
     'CR1mubveto':'(n_jet)',
     'CR1mubtag': '(n_jet)',
     'CR2mu': '(n_jet)',
    }

    self.q2={
     'SR': '(met_nomuon_tst_et/1000.)',
     'CR1e': '(met_nomuon_tst_et/1000.)',
     'CR1mubveto': '(met_nomuon_tst_et/1000.)',
     'CR1mubtag': '(met_nomuon_tst_et/1000.)',
     'CR2mu': '(met_nomuon_tst_et/1000.)',
    }




def makeHistoFromNtuple(infilename, intreename, outhistoname, outhistobinning, outhistoquantity, outhistoweight, outhistoselection="(1)", outhistosmooth="True"):

  # output
  #bins = array('d', outhistobins)
  #bins_n = len(outhistobins)-1
  histo = TH1F(outhistoname, outhistoname, *(outhistobinning))

  # input
  chain = TChain(intreename)
  chain.Add(infilename)

  print 'Projecting tree to histogram...'

  chain.Project(histo.GetName(), outhistoquantity, '('+outhistoselection + ')*(' + outhistoweight + ')')

  if outhistosmooth:
    histo.Scale(1, 'width')

  return histo



if __name__ == "__main__":

  gROOT.ProcessLine('.L /afs/cern.ch/work/m/mratti/private/MonoJetPlotting/AtlasStyle.C')
  gROOT.ProcessLine('SetAtlasStyle()')
  TH1.SetDefaultSumw2(True);

  weights = {
  'nom' : '((abs(mconly_weight)>100)*(weight/mconly_weight) + (abs(mconly_weight)<=100)*(weight))',
  'ckkw15' : '((abs(mconly_weight)>100)*(weight/mconly_weight*ckkw15_Weight) + (abs(mconly_weight)<=100)*(weight*ckkw15_Weight))',
  'ckkw30' : '((abs(mconly_weight)>100)*(weight/mconly_weight*ckkw30_Weight) + (abs(mconly_weight)<=100)*(weight*ckkw30_Weight))',
  #weight = '((abs(mconly_weight)>100)*(weight/mconly_weight) + (abs(mconly_weight)<=100)*(weight))*(weight_VJetsEW_Nominal)'
  }

  selections = {

  'SR' : '(isSR  && jet_pt[0]/1000>250 && met_nomuon_tst_et/1000>250)',
  'CR1mubveto' : '(isCR1mubveto    && jet_pt[0]/1000>250 && met_nomuon_tst_et/1000>250  && munu_mT / 1000 > 30 && munu_mT / 1000 <= 100)',
  'CR1mubtag' : '(isCR1mubtag      && jet_pt[0]/1000>250 && met_nomuon_tst_et/1000>250  && munu_mT / 1000 > 30 && munu_mT / 1000 <= 100)',
  'CR1e' : '(isCR1e_metnoel  && jet_pt[0]/1000>250 && met_noelectron_tst_et/1000>250 && enu_mT / 1000 > 30 && enu_mT / 1000 <= 100 && (abs(el_eta[0]) < 1.37 || abs(el_eta[0]) > 1.52) && el_pt[0]>30000 &&  el_topoetcone20[0]/el_pt[0] < 0.06 && el_ptvarcone20[0]/el_pt[0] < 0.06 && met_wmuon_tst_et>70000)',
  'CR2mu' : '(isCR2mu && jet_pt[0]/1000>250 && met_nomuon_tst_et/1000>250  && mumu_m / 1000 > 66 && mumu_m / 1000 < 116)',

  }

  #samples_regions = [
#    'Znunu_SR', 'Wmunu_SR', 'Wmunu_CR1mubveto', 'Wmunu_CR1mubtag', 'Wenu_SR',
#     'Wenu_CR1e_metnoel', 'Wtaunu_SR', 'Wtaunu_SR', 'Wtaunu_CR1mubveto', 'Zmumu_CR2mu', 'Zmumu_SR','Ztautau_SR',
#  ]
  #samples_regions = ['Wmunu_CR1mubveto', 'Wenu_CR1e', 'Znunu_SR', 'Zmumu_CR2mu', 'Wtaunu_SR']
  samples_regions = ['Znunu_SR']

  path = '/afs/cern.ch/work/m/mgratti/public/Shorttree/MJ238/'
  sp = {}
  sp['Wmunu'] = SampleProperty('Wmunu', 'run_MJ238_Wmunu.root', 'tree_Wmunu')
  sp['Wenu'] = SampleProperty('Wenu', 'run_MJ238_Wenu.root', 'tree_Wenu')
  sp['Wtaunu'] = SampleProperty('Wtaunu', 'run_MJ238_Wtaunu.root', 'tree_Wtaunu')
  sp['Zmumu'] = SampleProperty('Zmumu', 'run_MJ238_Zmumu.root', 'tree_Zmumu')
  sp['Zee'] = SampleProperty('Zee', 'run_MJ238_Zee.root', 'tree_Zee')
  sp['Znunu'] = SampleProperty('Znunu', 'run_MJ238_Znunu.root', 'tree_Znunu')
  sp['Ztautau'] = SampleProperty('Ztautau', 'run_MJ238_Ztautau.root', 'tree_Ztautau')



  binning_njet = (8, 0., 8.)
  binning_met  = (50, 250., 1250.)

  #infilename, intreename, outhistoname, outhistobinning=(100,250.,1250.), outhistoquantity, outhistoweight, outhistoselection="(1)", outhistosmooth="True"

  histo_saver = []

  for sample_region in samples_regions:
    sample,region = sample_region.split('_')
    # I want two plots  for every sample and region_met
    # need to create the variation histograms for each quantity
    histos = {}

    #for what in ['nom', 'ckkw15', 'ckkw30']:
    #  histos[what] = makeHistoFromNtuple( path + sp[sample].filename, sp[sample].treename, sample_region + what + '_n_jet', binning_njet, sp[sample].q1[region], weights[what], selections[region], outhistosmooth=False  )
    #histo_saver.append(histos)
    #makeRatioPlot(histos['nom'], histos['ckkw15'], histos['ckkw30'], 'nom', 'ckkw15', 'ckkw30' ,xtitle='Number of signal jets', ytitle='Yield normalized to 1/fb', ratiotitle='ckkw15/ckkw30' , norm=True)

    for what in ['nom', 'ckkw15', 'ckkw30']:
      histos[what] = makeHistoFromNtuple( path + sp[sample].filename, sp[sample].treename, sample_region + what + '_met', binning_met, sp[sample].q2[region], weights[what], selections[region], outhistosmooth=False  )
    #histo_saver.append(histos)
    makeRatioPlot(histos['nom'], histos['ckkw15'], histos['ckkw30'], 'nom', 'ckkw15', 'ckkw30' ,xtitle='p_{T}^{recoil} (GeV)', ytitle='Yield normalized to unity', ratiotitle='ckkw15/ckkw30' , norm=True)
