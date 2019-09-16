#example of use: 
# python plotter.py -f histo_singlePhoton_5k_EB --doClusterAnalysis



from ROOT import TH1F, TGraph, TGraph2D, TCanvas, TLegend, TFile, TTree, gROOT, TF1, TLatex, gStyle, TH2D, gPad, TColor,TMultiGraph, TH1
from ROOT import kRed, kBlue, kGray, kGreen, kPink, kYellow, kBlack, kWhite, kPink, kMagenta, kTRUE, kFALSE, kOrange, kViolet, kAzure, kSpring
from ROOT import TEfficiency
import itertools
import sys
import os
sys.path.append('utils')
sys.path.append('./')
from spares import *
import graphUtils as gU
from array import *
from analyzeRecHits import beautify2DPlot, beautify1DPlot


colors = [   kOrange+1, kRed, kMagenta+2, kViolet+8, kAzure-8, kAzure+6 ,
             kGreen+1, kSpring+4, kYellow -5, kYellow -3, kYellow, kOrange ]


if __name__ == "__main__":

    from argparse import ArgumentParser
    parser = ArgumentParser(description='', add_help=True)
    parser.add_argument('-f', '--inputFile', type=str, dest='inputFile', help='e.g. path/to/file/filename', default=None)
    parser.add_argument('--doClusterAnalysis', dest='doClusterAnalysis', help='beautify 1D plot', action='store_true', default=False)

options = parser.parse_args()


		  #gStyle.SetOptStat('emMrRo')
TH1.StatOverflows(kTRUE) # if false stats will be calculated without overflow, must be set also at filling time
TH1.SetDefaultSumw2()

		  ####################################
		  ## Define input, output and parameters
		  ####################################
#version = '{}_{}'.format(options.prodVersion, options.ecalVersion)
		  #inputfile = '{a}_{v}_numEvent{n}.root'

inputFile = options.inputFile
inputfile = '../Analyzer/outputfiles/{f}.root'.format(f=inputFile)


outputdir_pfCluster = 'myPlots/{f}/pfCluster_caloMatched'.format(f=inputFile)
outputdir_caloParticle = 'myPlots/{f}/caloParticle'.format(f=inputFile)
os.system('mkdir myPlots/{f}'.format(f=inputFile))
os.system('mkdir {}'.format(outputdir_pfCluster))
os.system('mkdir {}'.format(outputdir_caloParticle))


		  ####################################
		  ## Do analysis
		  ####################################


if options.doClusterAnalysis:


		  #inputfile = inputfile.format(a=anaName, s=1.0, g=1.0, n=150000, v=version)
        inputfile = inputfile.format(f=inputFile)

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_energy', xtitle='PFClusters_caloMatched_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_size', xtitle='PFClusters_caloMatched_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_nRecHit', xtitle='PFClusters_caloMatched_nRecHit', ytitle='Entries', xrange=(0.,40.))


        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_nXtals', xtitle='PFClusters_caloMatched_nXtals', ytitle='Entries', xrange=(0.,45.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_et', xtitle='PFClusters_caloMatched_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_eta', xtitle='PFClusters_caloMatched_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_phi', xtitle='PFClusters_caloMatched_phi', ytitle='Entries', xrange=(-3.2,3.2))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_eOverEtrue', xtitle='E/Etrue', ytitle='Entries', xrange=(0.,1.5))
 
        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_eOverEtrue_simEnergy', xtitle='E/Etrue', ytitle='Entries', xrange=(0.,1.5))
       


        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEM_energy', xtitle='EEM_PFClusters_caloMatched_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEM_size', xtitle='EEM_PFClusters_caloMatched_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEM_nXtals', xtitle='EEM_PFClusters_caloMatched_nXtals', ytitle='Entries', xrange=(0.,45.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEM_et', xtitle='EEM_PFClusters_caloMatched_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEM_eta', xtitle='EEM_PFClusters_caloMatched_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEM_phi', xtitle='EEM_PFClusters_caloMatched_phi', ytitle='Entries', xrange=(-3.2,3.2))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEM_eOverEtrue', xtitle='EEM E/Etrue', ytitle='Entries', xrange=(0.,1.5))
       


        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBM_energy', xtitle='EBM_PFClusters_caloMatched_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBM_size', xtitle='EBM_PFClusters_caloMatched_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBM_nXtals', xtitle='EBM_PFClusters_caloMatched_nXtals', ytitle='Entries', xrange=(0.,45.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBM_et', xtitle='EBM_PFClusters_caloMatched_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBM_eta', xtitle='EBM_PFClusters_caloMatched_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBM_phi', xtitle='EBM_PFClusters_caloMatched_phi', ytitle='Entries', xrange=(-3.2,3.2))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBM_eOverEtrue', xtitle='EBM E/Etrue', ytitle='Entries', xrange=(0.,1.5))
       

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBP_energy', xtitle='EBP_PFClusters_caloMatched_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBP_size', xtitle='EBP_PFClusters_caloMatched_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBP_nXtals', xtitle='EBP_PFClusters_caloMatched_nXtals', ytitle='Entries', xrange=(0.,45.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBP_et', xtitle='EBP_PFClusters_caloMatched_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBP_eta', xtitle='EBP_PFClusters_caloMatched_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBP_phi', xtitle='EBP_PFClusters_caloMatched_phi', ytitle='Entries', xrange=(-3.2,3.2))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EBP_eOverEtrue', xtitle='EBP E/Etrue', ytitle='Entries', xrange=(0.,1.5))
       


        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEP_energy', xtitle='EEP_PFClusters_caloMatched_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEP_size', xtitle='EEP_PFClusters_caloMatched_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEP_nXtals', xtitle='EEP_PFClusters_caloMatched_nXtals', ytitle='Entries', xrange=(0.,45.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEP_et', xtitle='EEP_PFClusters_caloMatched_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEP_eta', xtitle='EEP_PFClusters_caloMatched_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEP_phi', xtitle='EEP_PFClusters_caloMatched_phi', ytitle='Entries', xrange=(-3.2,3.2))

        beautify1DPlot(outputdir=outputdir_pfCluster, inputfile=inputfile, inputdir='PFCluster_caloMatched', histoname='h_PFClusters_caloMatched_EEP_eOverEtrue', xtitle='EEP E/Etrue', ytitle='Entries', xrange=(0.,1.5))
       







        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_energy', xtitle='caloParticle_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_simEnergy', xtitle='caloParticle_simEnergy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_size', xtitle='caloParticle_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_et', xtitle='caloParticle_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_eta', xtitle='caloParticle_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_phi', xtitle='caloParticle_phi', ytitle='Entries', xrange=(-3.2,3.2))



        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEM_energy', xtitle='EEM_caloParticle_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEM_simEnergy', xtitle='EEM_caloParticle_simEnergy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEM_size', xtitle='EEM_caloParticle_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEM_et', xtitle='EEM_caloParticle_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEM_eta', xtitle='EEM_caloParticle_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEM_phi', xtitle='EEM_caloParticle_phi', ytitle='Entries', xrange=(-3.2,3.2))



        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBM_energy', xtitle='EBM_caloParticle_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBM_simEnergy', xtitle='EBM_caloParticle_simEnergy [GeV]', ytitle='Entries', xrange=None)    
 
        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBM_size', xtitle='EBM_caloParticle_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBM_et', xtitle='EBM_caloParticle_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBM_eta', xtitle='EBM_caloParticle_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBM_phi', xtitle='EBM_caloParticle_phi', ytitle='Entries', xrange=(-3.2,3.2))

       

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBP_energy', xtitle='EBP_caloParticle_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBP_simEnergy', xtitle='EBP_caloParticle_simEnergy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBP_size', xtitle='EBP_caloParticle_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBP_et', xtitle='EBP_caloParticle_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBP_eta', xtitle='EBP_caloParticle_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EBP_phi', xtitle='EBP_caloParticle_phi', ytitle='Entries', xrange=(-3.2,3.2))
       


        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEP_energy', xtitle='EEP_caloParticle_energy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEP_simEnergy', xtitle='EEP_caloParticle_simEnergy [GeV]', ytitle='Entries', xrange=None)    

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEP_size', xtitle='EEP_caloParticle_size', ytitle='Entries', xrange=(0.,10.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEP_et', xtitle='EEP_caloParticle_ET [GeV]', ytitle='Entries', xrange=None)
        
        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEP_eta', xtitle='EEP_caloParticle_eta', ytitle='Entries', xrange=(-4.,4.))

        beautify1DPlot(outputdir=outputdir_caloParticle, inputfile=inputfile, inputdir='caloParticle', histoname='h_caloParticle_EEP_phi', xtitle='EEP_caloParticle_phi', ytitle='Entries', xrange=(-3.2,3.2))

       






