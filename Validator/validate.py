
import ROOT
import ratioPlot as RP
import os
import os.path


qs_common = [

## End-caps
{'hname':'caloParticle/h_caloParticle_size_EE', 'title':'caloparticle size EE'},
{'hname':'caloParticle/h_caloParticle_genEnergy_EE', 'title':'caloparticle gen Energy EE (GeV)'},
{'hname':'caloParticle/h_caloParticle_simEnergy_EE', 'title':'caloparticle sim Energy EE (GeV)'},
{'hname':'caloParticle/h_caloParticle_simEt_EE', 'title':'caloparticle sim Et EE (GeV)'},
{'hname':'caloParticle/h_caloParticle_genEta_EE', 'title':'caloparticle gen #eta EE'},
{'hname':'caloParticle/h_caloParticle_simEta_EE', 'title':'caloparticle sim #eta EE'},
{'hname':'caloParticle/h_caloParticle_genPhi_EE', 'title':'caloparticle gen #phi EE'},
{'hname':'caloParticle/h_caloParticle_simPhi_EE', 'title':'caloparticle sim #phi EE'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_size_EE', 'title':'matched pfclusters size EE'},
#{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_nRecHit_EE', 'title':'matched pfclusters nXtals EE'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_energy_EE', 'title':'matched pfclusters energy EE (GeV)'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_et_EE', 'title':'matched pfclusters Et EE (GeV)'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_eta_EE', 'title':'matched pfclusters #eta EE'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_phi_EE', 'title':'matched pfclusters #phi EE'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_eOverEtrue_EE', 'title':'matched pfclusters E_{pf}/E_{gen} EE'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_eOverEtrue_simEnergy_EE', 'title':'matched pfclusters E_{pf}/E_{sim} EE'},

## Barrel
{'hname':'caloParticle/h_caloParticle_size_EB', 'title':'caloparticle size EB'},
{'hname':'caloParticle/h_caloParticle_genEnergy_EB', 'title':'caloparticle gen Energy EB (GeV)'},
{'hname':'caloParticle/h_caloParticle_simEnergy_EB', 'title':'caloparticle sim Energy EB (GeV)'},
{'hname':'caloParticle/h_caloParticle_simEt_EB', 'title':'caloparticle sim Et EB (GeV)'},
{'hname':'caloParticle/h_caloParticle_genEta_EB', 'title':'caloparticle gen #eta EB'},
{'hname':'caloParticle/h_caloParticle_simEta_EB', 'title':'caloparticle sim #eta EB'},
{'hname':'caloParticle/h_caloParticle_genPhi_EB', 'title':'caloparticle gen #phi EB'},
{'hname':'caloParticle/h_caloParticle_simPhi_EB', 'title':'caloparticle sim #phi EB'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_size_EB', 'title':'matched pfclusters size EB'},
#{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_nRecHit_EB', 'title':'matched pfclusters nXtals EB'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_energy_EB', 'title':'matched pfclusters energy EB (GeV)'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_et_EB', 'title':'matched pfclusters Et EB (GeV)'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_eta_EB', 'title':'matched pfclusters #eta EB'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_phi_EB', 'title':'matched pfclusters #phi EB'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_eOverEtrue_EB', 'title':'matched pfclusters E_{pf}/E_{gen} EB'},
{'hname':'PFCluster_caloMatched/h_PFClusters_caloMatched_eOverEtrue_simEnergy_EB', 'title':'matched pfclusters E_{pf}/E_{sim} EB'},

]


def getRelDiff(n1, n2):
  if n2!= 0:
    res = float(n1)/float(n2) - 1
  elif n2==0 and n1==0 :
    res = 0
  else:
    print 'Cannot compute rel diff wrt 0, skipping'
    res = None
  return res

def getBinDiff(h1, h2):
  ROOT.TH1.SetDefaultSumw2()
  #if ROOT.TH1.CheckConsistency(h1, h2): # if they're consistent check difference between the two
  # currently a protected member, cannot be used
  for iBin in range(1,h1.GetNbinsX()):
    diff = getRelDiff(h1.GetBinContent(iBin), h2.GetBinContent(iBin))
    if diff!=None: print 'Bin={}, evts1={}, evts2={}, rel diff={:.4f}%'.format(iBin, h1.GetBinContent(iBin), h2.GetBinContent(iBin), diff*100)

  totdiff = getRelDiff(h1.GetEntries(), h2.GetEntries())
  print 'Tot diff, evts1={}, evts2={}, rel diff={:.2f}%'.format(h1.GetEntries(), h2.GetEntries(), totdiff)


def getOptions():
  from argparse import ArgumentParser
  parser = ArgumentParser(description='Validation of pfcluster analysis', add_help=True)

  parser.add_argument('-f1', '--file1', type=str, dest='fname1', help='')
  parser.add_argument('-f2', '--file2', type=str, dest='fname2', help='')
  #parser.add_argument('-t1', '--tree1', type=str, dest='treename1', help='', default = 'Events')
  #parser.add_argument('-t2', '--tree2', type=str, dest='treename2', help='', default = 'mt2')
  parser.add_argument('-l1', '--label1', type=str, dest='label1', help='', default = 'A')
  parser.add_argument('-l2', '--label2', type=str, dest='label2', help='', default = 'B')
  parser.add_argument('-o', '--outdir', type=str, dest='outdirname', help='output dir')
  parser.add_argument('--doNorm', dest='doNorm', help='Do shape comparison', action='store_true', default=False)
  parser.add_argument('--doLog', dest='doLog', help='Use Log scale on y axis', action='store_true', default=False)
  parser.add_argument('--doBin', dest='doBin', help='Print bin-by-bin differences', action='store_true', default=False)
  parser.add_argument('--rebin', type=int, dest='rebin', help='Multiplier for rebinning', default=None)
  #parser.add_argument('--doFriend', dest='doFriend', help='do friend trees checks', action='store_true', default=False)
  #parser.add_argument('-s', '--selection', type=str, dest='sel', help='selection; in case of friend tree, an example is abs(ht/new.ht-1)>0.05', default='(1)')

  return parser.parse_args()


if __name__ == "__main__":

  options = getOptions()

  ROOT.gROOT.ProcessLine('.L /work/mratti/CMS_style/tdrstyle.C')
  ROOT.gROOT.ProcessLine('setTDRStyle()')
  ROOT.gROOT.SetBatch(True)
  
  import os
  os.system('mkdir {d}'.format(d=options.outdirname))

  if os.path.isfile(options.fname1)==False or os.path.isfile(options.fname2)==False: raise RuntimeError('One of the two files is not available, \n{} \n{}'.format(options.fname1, options.fname2))

  qs_to_run = qs_common

  for q in qs_to_run:
    print '\n\n=> Investigating quantity', q['title']
    q['name'] = q['hname'].split('/')[1]
    ret1,histo1 = RP.getHistoFromFile(infilename=options.fname1,inhistoname=q['hname'],outhistoname=q['name']+'_1',outhistorebin=options.rebin,addOverflow=True)
    ret2,histo2 = RP.getHistoFromFile(infilename=options.fname2,inhistoname=q['hname'],outhistoname=q['name']+'_2',outhistorebin=options.rebin,addOverflow=True)
   
    # now you should have the histograms
    if ret1 != -1 and ret2 != -1:
      # do the ratio plot
      ytitle = 'Entries' if not options.doNorm else 'Entries (norm)'
      RP.makeRatioPlot(hNum=histo1, hDen=histo2, nameNum=options.label1, nameDen=options.label2, xtitle=q['title'],ytitle=ytitle, ratiotitle="Ratio", norm=options.doNorm, log=options.doLog, outDir=options.outdirname, plotName=q['name'])

      # entries
      print '     Entries histo1', histo1.GetEntries(), ' histo2', histo2.GetEntries()

      if options.doBin:
        # catch bin-by-bin differences
        print '     Analyzing bin differences'
        getBinDiff(h1=histo1, h2=histo2)
        print '\n'
    else:
      print '     Skipping ', q['name']




