import sys 
import os
import collections
import operator
import numpy as np
from math import sin, atan, exp, log

sys.path.append('noisePlotter/')
import getMergedGraph as noisePlotter

from array import *
from ROOT import TH2D, TH2Poly, TH1D, TF1, TLegend, TCanvas, TPaveText, TPad, TGraph, TLine, gROOT, gStyle
from ROOT import kWhite, kMagenta, kAzure, kPink, kSpring, kOrange, kCyan, kRed, kGreen, kBlue, kBird, kCherry

# define a class Sample which has all the attributes to be retrieved from the samples
class Sample(object):
   def __init__(self, energy=0, eta=0, chi2=0, pfRecHit=0, seeding=0, resolution=0, scale=0, efficiency=0, noiseRate=0, resolution_error=0, efficiency_error=0, noiseRate_error=0, scale_error=0, noiseOccupancy=0, rms=0, rms_error=0):
      self.energy = energy
      self.eta = eta
      self.chi2 = chi2
      self.pfRecHit = pfRecHit
      self.seeding = seeding
      self.resolution = resolution
      self.scale = scale
      self.efficiency = efficiency
      self.noiseRate = noiseRate
      self.resolution_error = resolution_error
      self.efficiency_error = efficiency_error
      self.noiseRate_error = noiseRate_error
      self.scale_error = scale_error
      self.noiseOccupancy = noiseOccupancy
      self.rms = rms
      self.rms_error = rms_error
      
# get the parser options
def getOptions():
   from argparse import ArgumentParser
   parser = ArgumentParser(description='File to produce necessary plots for threshold tuning', add_help=True)
   parser.add_argument('--doResolutionPlot', type=str, dest='doResolutionPlot', help='resolution plots', default='True')
   parser.add_argument('--doScalePlot', type=str, dest='doScalePlot', help='scale plots', default='True')
   parser.add_argument('--doEfficiencyPlot', type=str, dest='doEfficiencyPlot', help='efficiency plots', default='True')
   parser.add_argument('--doNoiseRatePlot', type=str, dest='doNoiseRatePlot', help='noise rate plots', default='True')
   parser.add_argument('--doRankingPlot', type=str, dest='doRankingPlot', help='ranking plot', default='True')
   parser.add_argument('--doSummaryPlot', type=str, dest='doSummaryPlot', help='summary plot', default='True')
   parser.add_argument('--doDecisionPlot', type=str, dest='doDecisionPlot', help='decision plot', default='True')
   parser.add_argument('--doChi2Plot', type=str, dest='doChi2Plot', help='chi2 as a function of quantity ratio', default='True')
   parser.add_argument('--doResoOverScale', type=str, dest='doResoOverScale', help='use reso/scale?', default='True')
   parser.add_argument('--doPopUpPlot', type=str, dest='doPopUpPlot', help='want plots to pop up?', default='True')
   parser.add_argument('--label', type=str, dest='label', help='label', default='')
   return parser.parse_args()


# function that retrieves the necessary parameters from the fit output txt files located in samples/
# this function returns a Sample at a given En and Eta
def getSampleItems(inputfile):
 
   #the output is a vector of samples
   output = []
   EnBins = []
   EtaBins = [] 
   PFRecHit = []
   Seeding = []

   fSample = open(inputfile)
   lines_sample = fSample.readlines()
   for sample in lines_sample:

      inputfile = 'samples/' + sample[0:len(sample)-1] + '.txt'

      #we get the seeding and pfrechit thresholds from the fileName
      indexSeedinf = inputfile.find('seed', 0)
      indexSeedsup = inputfile.find('_', indexSeedinf)
      seedThrs = inputfile[indexSeedinf+4:indexSeedsup]

      indexPFRecHitinf = inputfile.find('pfrh')
      indexPFRecHitsup = inputfile.find('_', indexPFRecHitinf)
      PFRecHitThrs = inputfile[indexPFRecHitinf+4:indexPFRecHitsup]

      # insert dummy value in case of reference thresholds
      if seedThrs == 'Ref': seedThrs = '0.0'
      if PFRecHitThrs == 'Ref': PFRecHitThrs = '0.0'

      if float(seedThrs) not in Seeding: 
         Seeding.append(float(seedThrs))

      if float(PFRecHitThrs) not in PFRecHit:
         PFRecHit.append(float(PFRecHitThrs))

      f = open(inputfile)
      lines = f.readlines()
      for line in lines:
         #print(line)
         index1 = line.find(' ', 0)
         energyBin = line[0:index1]
      
         index2 = line.find(' ', index1+1)
         etaBin = line[index1+1:index2]
      
         index3 = line.find(' ', index2+1) 
         chiSquare = line[index2+1:index3]

         index4 = line.find(' ', index3+1)
         reso = line[index3+1:index4]
      
         index5 = line.find(' ', index4+1)
         mean = line[index4+1:index5]

         index6 = line.find(' ', index5+1)
         eff = line[index5+1:index6]

         index7 = line.find(' ', index6+1)
         noise = line[index6+1:index7]

         index8 = line.find(' ', index7+1)
         reso_err = line[index7+1:index8]

         index9 = line.find(' ', index8+1)
         eff_err = line[index8+1:index9]

         index10 = line.find(' ', index9+1)
         noise_err = line[index9+1:index10]

         index11 = line.find(' ', index10+1)
         scale_err = line[index10+1:index11]

         index12 = line.find(' ', index11+1)
         noiseOcc = line[index11+1:index12]
          
         index13 = line.find(' ', index12+1)
         rms = line[index12+1:index13]

         index14 = line.find(' ', index13+1)
         rms_err = line[index13+1:index14]
         
         #print('pfrechit {t1} seeding {t2} energyBin {a1} etaBin {a2} reso {a3} mean {a4} eff {a5}').format(t1=PFRecHitThrs, t2=seedThrs, a1=energyBin, a2=etaBin, a3=reso, a4=mean, a5=eff)
      
         thisSample = Sample(energy=energyBin, eta=etaBin, chi2=chiSquare, pfRecHit=PFRecHitThrs, seeding=seedThrs, resolution=reso, scale=mean, efficiency=eff, noiseRate=noise, resolution_error = reso_err, efficiency_error = eff_err, noiseRate_error = noise_err, scale_error=scale_err, noiseOccupancy=noiseOcc, rms=rms, rms_error=rms_err)
         output.append(thisSample)
 
         if energyBin not in EnBins and energyBin != 'ETranges':
            EnBins.append(energyBin)
      
         if etaBin not in EtaBins and etaBin != 'ETAranges':
            EtaBins.append(etaBin)
  
   return output, EnBins, EtaBins, PFRecHit, Seeding


# the Sample items are strings, so here is a function that converts them into floats 
def getFloat(input, condition=None):
   if(condition!='p'):
      dotIndex = input.find('.',0)
   else:
      dotIndex = input.find('p', 0)

   if(dotIndex!=-1):   
      if input.find('e', 0)!=-1:
         firstPart = float(input[0:dotIndex])
         secondPart = float(input[dotIndex+1:input.find('e', 0)])
         expo = len(input[dotIndex+1:input.find('e', 0)])
         expotot = float(input[input.find('e', 0)+1:len(input)])
         return (firstPart+secondPart*pow(10, -expo))*pow(10, expotot)
      else:
         firstPart = float(input[0:dotIndex])
         secondPart = float(input[dotIndex+1:len(input)])
         expo = len(input[dotIndex+1:len(input)])
         if str(firstPart) == '-0.0':
            return firstPart+pow(10, -expo)*secondPart*(-1)
         else:
            return firstPart+pow(10, -expo)*secondPart
   else:
         return int(input)

# define accessors
def getUpperBin(input):
   index = input.find('_')
   return input[index+1:len(input)]


def getLowerBin(input):
   index = input.find('_')
   return input[0:index]


def getQuantity(input):
   index1 = input.find(' ')
   index2 = input.find(' ', index1+1)
   return input[index2:len(input)]


def getScore(input):
   phrase = getQuantity(input)
   index1 = phrase.find(' ')
   index2 = phrase.find(' ', index1+1)
   index3 = phrase.find(' ', index2+1)
   index4 = phrase.find(' ', index3+1)
   return float(phrase[index3+1:index4])


def getScoreError(input):
   phrase = getQuantity(input)
   index1 = phrase.find(' ')
   index2 = phrase.find(' ', index1+1)
   index3 = phrase.find(' ', index2+1)
   index4 = phrase.find(' ', index3+1)
   return float(phrase[index4+1:len(input)])


def getFirstElement(input):
   index1 = input.find(' ')
   return input[0:index1]


def getSecondElement(input, cond=None):
   index1 = input.find(' ')
   index2 = input.find(' ', index1+1)
   if(cond=='all'):
      return input[index1+1:len(input)]
   else:
      return input[index1+1:index2]


def getThirdElementPercentage(input, num=3):
   percentage = (getFloat(getQuantity(input))-1)*100
   return round(percentage, num)


def getThirdElement(input, num=3):
   percentage = getFloat(getQuantity(input))
   return round(percentage, num)


def getMiddleBin(input):
   underScoreIndex = input.find('_',0)
   return (getFloat(input[0:underScoreIndex], 'p') + getFloat(input[underScoreIndex+1:len(input)], 'p'))/2


def getInfBin(input):
   index = input[0].find('_')
   return input[0][0:index]   


def getSupBin(input):
   index = input[len(input)-1].find('_')
   return input[len(input)-1][index+1:len(input[len(input)-1])]


# function that checks if different scores are compatible within their uncertainty
def areOverlapping(input1, input2):
   if input2 < input1:
      return True
   else:
      return False

# function to attribute a colour to each pair of thresholds
def getColor(a, b):
   if a==1 and b==1:
      return kPink-5
   elif a==1 and b==2:
      return kOrange-6
   elif a==1 and b==3:
      return kSpring-5
   elif a==1 and b==4:
      return kCyan+1
   elif a==2 and b==2:
      return kAzure+6
   elif a==2 and b==3:
      return kMagenta+1
   elif a==2 and b==4:
      return kRed
   elif a==3 and b==3:
      return kOrange+2
   elif a==3 and b==4:
      return kGreen+3
   elif a==4 and b==4:
      return kBlue+2
   elif a==0 and b==0:
      return kWhite

if __name__ == "__main__":
 
   opt = getOptions()
   
   do_resolutionPlot=opt.doResolutionPlot
   do_scalePlot=opt.doScalePlot
   do_efficiencyPlot=opt.doEfficiencyPlot
   do_noiseRatePlot=opt.doNoiseRatePlot
   do_rankingPlot=opt.doRankingPlot
   do_summaryPlot=opt.doSummaryPlot
   do_decisionPlot=opt.doDecisionPlot
   do_chi2Plot=opt.doChi2Plot
   do_resoOverScale=opt.doResoOverScale
   do_popUpPlot = opt.doPopUpPlot
   
   if do_popUpPlot == 'False':
      gROOT.SetBatch(True)

   #outputdirectory
   outputdir = '/work/mratti/cmssw_workarea/NEW_RECO_DEVs/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/myPlots/2DScan{l}/'.format(l=opt.label)
   #outputdir = '/afs/cern.ch/user/a/anlyon/CMSSW_10_6_1_patch1/src/Plotter/myPlots/2DScan'
   os.system('mkdir {}'.format(outputdir))

 
   samples, EnRanges, EtaRanges, pfRechitThrs, seedingThrs = getSampleItems('fileSamples.txt')

   #EnRanges  = ["80_100"] #, "5_10", "10_15", "15_20", "20_40", "40_60", "60_80"]
   #EtaRanges = ["0p00_0p40"] #, "0p40_0p80"]

   #since we want a graph per energy/eta bin, we bin the samples vector
   # samples_binned is a list of dictionaries
   samples_binned = {}
   for iEn in EnRanges:
      samples_binned[iEn] = {}
      for iEta in EtaRanges:
         samples_binned[iEn][iEta] = []

   for iEn in EnRanges:
      for iEta in EtaRanges:
         for sample in samples:
            if((iEn==sample.energy) and (iEta==sample.eta)):
               samples_binned[iEn][iEta].append(sample)

         
   # list containing the selected pair for each bin
   selected_pair = {}
   for iEn in EnRanges:
      selected_pair[iEn] = {}
      for iEta in EtaRanges: 
         selected_pair[iEn][iEta] = [] #' - '

 
   whichPlot = [] 
   if do_resolutionPlot=='True': 
      whichPlot.append('Resolution')
   if do_scalePlot=='True': 
      whichPlot.append('Scale')
   if do_efficiencyPlot=='True':
      whichPlot.append('Efficiency')
   if do_noiseRatePlot=='True':
      whichPlot.append('NoiseRate')

   if (do_rankingPlot=='True' or do_summaryPlot=='True') and do_resolutionPlot=='False':
      whichPlot.append('Resolution')
   if (do_rankingPlot=='True' or do_summaryPlot=='True') and do_efficiencyPlot=='False':
      whichPlot.append('Efficiency')
   if (do_rankingPlot=='True' or do_summaryPlot=='True') and do_noiseRatePlot=='False':
      whichPlot.append('NoiseRate')


   #testFile = open("testFile_seeding3.txt", "w+")

   #start of the plotting
   for iEn in EnRanges: 
      for iEta in EtaRanges:

         resolutions = []
         efficiencies = []
         noiseRates = []
         
         if iEn == 'ETranges': continue
         if iEta == 'ETAranges': continue

         # EB samples are run until 100GeV while EE one until 200GeV
         if getFloat(getUpperBin(iEn)) > 100:
            if getFloat(getUpperBin(iEta), 'p')<1.48: continue

         for what in whichPlot:
            if iEn == 'ETranges': continue
            if iEta == 'ETAranges': continue
           
            histo = TH2D('histo', 'histo', 4, 1, 5, 4, 1, 5)
            histo_ratio = TH2D('histo_ratio', 'histo_ratio', 4, 1, 5, 4, 1, 5) 
            
	         # we get the reference quantity (=deno of the ratio)
            n=0
            for iPFRecHit in pfRechitThrs:
               for iSeeding in seedingThrs:
            
                  if(iPFRecHit > iSeeding): continue
                  
		            #filling histo and getting the reference values for the ratio
                  for iSample in samples_binned[iEn][iEta]:
                      if getFloat(iSample.seeding)==iSeeding and getFloat(iSample.pfRecHit)==iPFRecHit:
                        if what == 'Resolution':
                           if do_resoOverScale != 'True':
                              quantity = iSample.resolution
                              if n==0: ref_quantity=quantity
                              else: 
                                 if quantity<ref_quantity and quantity != '0': ref_quantity=quantity
                              n=n+1
                           else:
                              #print('{a} {b} {c} {d} {e}'.format(a=iEn, b=iEta, c=iPFRecHit, d=iSeeding, e=getFloat(iSample.scale)))
                              if getFloat(iSample.scale)!=0:
                                 quantity = str(getFloat(iSample.resolution)/getFloat(iSample.scale))
                                 if n==0: ref_quantity = quantity
                                 else:
                                    if getFloat(quantity)<getFloat(ref_quantity) and quantity != '0': ref_quantity=quantity
                                 n=n+1
                        elif what == 'Scale':
                           quantity = iSample.scale
                           if n==0: ref_quantity=quantity
                           n=n+1
                        elif what == 'Efficiency':
                           quantity = iSample.efficiency
                           if n==0: ref_quantity=quantity
                           else:
                              if quantity>ref_quantity: ref_quantity=quantity
                           n=n+1
                           #if iSeeding==3 and iPFRecHit==1: 
                           #   ref_quantity = quantity
                        elif what == 'NoiseRate':
                           #put the reference quantity to 0 but we will anyway not plot the ratio plot of the noiseRate
                           quantity = iSample.noiseRate
                           if quantity=='0': quantity='0.000001'
                           ref_quantity='0'

                        histo.Fill(getFloat(iSample.seeding), getFloat(iSample.pfRecHit), getFloat(quantity))
                        #print('{a} {b} {c} {d} {e}'.format(a=what, b=iPFRecHit, c=iSeeding, d=quantity, e=ref_quantity))
	    
            #filling the histo_ratio
            for iPFRecHit in pfRechitThrs:
               for iSeeding in seedingThrs:
                  if(iPFRecHit > iSeeding): continue
                  for iSample in samples_binned[iEn][iEta]:
                      if getFloat(iSample.seeding)==iSeeding and getFloat(iSample.pfRecHit)==iPFRecHit:
                        if what == 'Resolution':
                           if do_resoOverScale != 'True':
                              quantity = iSample.resolution
                              if getFloat(ref_quantity)!= 0 :
                                 ratio = getFloat(quantity)/getFloat(ref_quantity)
                                 resolutions.append('{a} {b} {c}'.format(a=iPFRecHit, b=iSeeding, c=ratio))
                           else:
                              if getFloat(iSample.scale)!=0:
                                 quantity = str(getFloat(iSample.resolution)/getFloat(iSample.scale))
                                 #print('{a} {b}'.format(a=quantity, b=ref_quantity))
                                 if getFloat(ref_quantity)!=0:
                                    ratio = getFloat(quantity)/getFloat(ref_quantity)
                                    resolutions.append('{a} {b} {c}'.format(a=iPFRecHit, b=iSeeding, c=ratio))
                        elif what == 'Scale':
                           quantity = iSample.scale
                        elif what == 'Efficiency':
                           quantity = iSample.efficiency
                           if getFloat(ref_quantity)!= 0:
                              ratio = getFloat(quantity)/getFloat(ref_quantity)
                              efficiencies.append('{a} {b} {c}'.format(a=iPFRecHit, b=iSeeding, c=ratio))
                        if what == 'NoiseRate':
                           quantity = iSample.noiseRate
                           noiseRates.append('{a} {b} {c}'.format(a=iPFRecHit, b=iSeeding, c=quantity))
                           # note: we won't plot the ratio for the noiseRate
                        if getFloat(ref_quantity)!= 0:   
                           histo_ratio.Fill(getFloat(iSample.seeding), getFloat(iSample.pfRecHit), getFloat(quantity)/getFloat(ref_quantity))
                           #if getFloat(quantity)/getFloat(ref_quantity) >= 1.01:
                           #   testFile.write('{a} {b} {c} {d} {e}% \n'.format(a=iEn, b=iEta, c=iPFRecHit, d=iSeeding, e=round((getFloat(quantity)/getFloat(ref_quantity)-1)*100, 3)))
            
            gStyle.SetOptStat(0)
            gStyle.SetPadRightMargin(0.16) 

            histo.SetTitle('Energy bin: {a}   #eta bin: {b}'.format(a=iEn, b=iEta))
            histo_ratio.SetTitle('Energy bin: {a}   #eta bin: {b}'.format(a=iEn, b=iEta))
            
            histo.GetXaxis().SetTitle('Seeding thrs multiplicity')
            histo.GetXaxis().SetTitleSize(0.04)
            histo.GetXaxis().SetTitleOffset(1.2)
      
            histo.GetYaxis().SetTitle('PFRecHit thrs multiplicity')
            histo.GetYaxis().SetTitleSize(0.04)
            histo.GetYaxis().SetTitleOffset(1.2)
 
   
            if what == 'Resolution':
               if do_resoOverScale != 'True':
                  histo.GetZaxis().SetTitle('Resolution')
               else:
                  histo.GetZaxis().SetTitle('Resolution')
            elif what == 'Scale':
               histo.GetZaxis().SetTitle('Scale')
            elif what == 'Efficiency':
               histo.GetZaxis().SetTitle('Efficiency')
            elif what == 'NoiseRate':
               histo.GetZaxis().SetTitle('Noise Rate')

            if what == 'Resolution':
               histo.GetZaxis().SetRangeUser(0, 0.4)
            elif what == 'Scale':
               histo.GetZaxis().SetRangeUser(0.5, 1.5)
            elif what == 'Efficiency':
               histo.GetZaxis().SetRangeUser(0, 1)
            elif what == 'NoiseRate':
               histo.GetZaxis().SetRangeUser(0, 1)
 
            histo.GetZaxis().SetTitleSize(0.04)
            histo.GetZaxis().SetTitleOffset(1.2)
    
            histo_ratio.GetXaxis().SetTitle('Seeding thrs multiplicity')
            histo_ratio.GetXaxis().SetTitleSize(0.04)
            histo_ratio.GetXaxis().SetTitleOffset(1.2)
      
            histo_ratio.GetYaxis().SetTitle('PFRecHit thrs multiplicity')
            histo_ratio.GetYaxis().SetTitleSize(0.04)
            histo_ratio.GetYaxis().SetTitleOffset(1.2)
 
            if what == 'Resolution':
               if do_resoOverScale != 'True':
                  histo_ratio.GetZaxis().SetTitle('Resolution')
               else:
                  histo_ratio.GetZaxis().SetTitle('Resolution')
            elif what == 'Scale':
               histo_ratio.GetZaxis().SetTitle('Scale')
            elif what == 'Efficiency':
               histo_ratio.GetZaxis().SetTitle('Efficiency')

            if what == 'Resolution':
               histo_ratio.GetZaxis().SetRangeUser(0, 0.4)
            elif what == 'Scale':
               histo_ratio.GetZaxis().SetRangeUser(0.5, 1.5)
            elif what == 'Efficiency':
               histo_ratio.GetZaxis().SetRangeUser(0, 1)
 
            histo_ratio.GetZaxis().SetTitleSize(0.04)
            histo_ratio.GetZaxis().SetTitleOffset(1.2)
            
            
            c = TCanvas('c_{a}_{b}_{c}'.format(a=iEn, b=iEta, c=what), 'c', 900, 900)
            histo.Draw('colz')
            histo.SetMarkerSize(2)
            histo.Draw('text' +'same')
 
            c_ratio = TCanvas('c_ratio_{a}_{b}_{c}'.format(a=iEn, b=iEta, c=what), 'c', 900, 900)
            histo_ratio.Draw('colz')
            histo_ratio.SetMarkerSize(2)
            histo_ratio.Draw('text' +'same')
            
            
            if do_resolutionPlot == 'True' and what == 'Resolution':
               if do_resoOverScale != 'True':
                  c.SaveAs("{dir}/resolution_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
               else:
                  c.SaveAs("{dir}/resolutionOverScale_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_scalePlot == 'True' and what == 'Scale':
               c.SaveAs("{dir}/scale_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_efficiencyPlot == 'True' and what == 'Efficiency':
               c.SaveAs("{dir}/efficiency_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_noiseRatePlot == 'True' and what == 'NoiseRate':
               c.SaveAs("{dir}/noiseRate_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))


            if do_resolutionPlot == 'True' and what == 'Resolution':
               if do_resoOverScale != 'True':
                  c_ratio.SaveAs("{dir}/ratio_resolution_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
               else:
                  c_ratio.SaveAs("{dir}/ratio_resolutionOverScale_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_scalePlot == 'True' and what == 'Scale':
               c_ratio.SaveAs("{dir}/ratio_scale_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_efficiencyPlot == 'True' and what == 'Efficiency':
               c_ratio.SaveAs("{dir}/ratio_efficiency_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
	    	
            del histo
            del histo_ratio


         #selection
         # score-based selection criteria implemented in the following
         if do_rankingPlot == 'True' or do_summaryPlot == 'True':
            selection = []

            if iEn == 'ETranges': continue
            if iEta == 'ETAranges': continue
            

            n=0
            for iPFRecHit in pfRechitThrs:
               for iSeeding in seedingThrs:
               
                  if(iPFRecHit > iSeeding): continue
                    
                  # first getting the reference values (best reso, best eff)
                  for iSample in samples_binned[iEn][iEta]:
                      if getFloat(iSample.seeding)==iSeeding and getFloat(iSample.pfRecHit)==iPFRecHit:
                           if do_resoOverScale != 'True':
                              reso = iSample.resolution
                           else:
                              if getFloat(iSample.scale)!=0:
                                 reso = str(getFloat(iSample.resolution)/getFloat(iSample.scale))
                           eff = iSample.efficiency
                           if do_resoOverScale != 'True':
                              reso_err = iSample.resolution_error
                           else:
                              if getFloat(iSample.scale)!=0 and getFloat(iSample.resolution)!=0:
                                 reso_err = str(getFloat(iSample.resolution)/getFloat(iSample.scale)*(getFloat(iSample.resolution_error)/getFloat(iSample.resolution) + getFloat(iSample.scale_error)/getFloat(iSample.scale)))
                           eff_err = iSample.efficiency_error
                           if n==0:
                              ref_reso=reso
                              ref_eff=eff
                              ref_reso_err=reso_err
                              ref_eff_err=eff_err
                           else: 
                              if reso<ref_reso and reso != '0': 
                                 ref_reso=reso
                                 ref_reso_err=reso_err
                              if eff>ref_eff: 
                                 ref_eff=eff
                                 ref_eff_err=eff_err
                           n=n+1
                              
                           #print('{a} {b} {c} {d} {e}'.format(a=what, b=iPFRecHit, c=iSeeding, d=reso, e=ref_reso))
           

            # if the score_reso or score_eff are below 1% put thme to reference, so that we don't descriminate them
            putToRef = True

            # add noiseRate in the definition of the score
            addNoiseRateWeight = True

            # define the score
            for iPFRecHit in pfRechitThrs:
               for iSeeding in seedingThrs:
                  if(iPFRecHit > iSeeding): continue
                  for iSample in samples_binned[iEn][iEta]:
                     if getFloat(iSample.seeding)==iSeeding and getFloat(iSample.pfRecHit)==iPFRecHit:
                        if do_resoOverScale != 'True':                    
                           reso=iSample.resolution
                        else:
                           if getFloat(iSample.scale)!=0:
                              reso = str(getFloat(iSample.resolution)/getFloat(iSample.scale))
                        eff=iSample.efficiency 
                        if do_resoOverScale != 'True':
                           reso_err=iSample.resolution_error
                        else:
                           if getFloat(iSample.scale)!=0 and getFloat(iSample.resolution)!=0:
                              reso_err = str(getFloat(iSample.resolution)/getFloat(iSample.scale)*(getFloat(iSample.resolution_error)/getFloat(iSample.resolution) + getFloat(iSample.scale_error)/getFloat(iSample.scale)))
                        eff_err=iSample.efficiency_error
                        if reso == '0': continue
                        if getFloat(ref_reso)!=0 and getFloat(ref_eff)!=0:
                           ratio_reso = getFloat(reso)/getFloat(ref_reso)
                           ratio_eff = getFloat(eff)/getFloat(ref_eff)
                           if(ratio_reso!=1):
                              score_reso = round((ratio_reso-1)*100, 2)
                           else:
                              score_reso = -0.001 #0 #1
                           score_reso_err = abs(score_reso)*(abs(getFloat(reso_err)/getFloat(reso)) + abs(getFloat(ref_reso_err)/getFloat(ref_reso)))
                           #print('{a} {b}'.format(a=reso_err, b=getFloat(reso_err)))
                           #print('{a} * {b} {c} + {d} {e}'.format(a=score_reso, b=getFloat(reso_err), c=getFloat(reso), d=getFloat(ref_reso_err), e=getFloat(ref_reso))) 
                           if(ratio_eff!=1):
                              score_eff = round((ratio_eff-1)*100, 2)
                           else:
                              score_eff = 0 #-1
                           if getFloat(eff)!=0:
                              score_eff_err = abs(score_eff)*(abs(getFloat(eff_err)/getFloat(eff)) + abs(getFloat(ref_eff_err)/getFloat(ref_eff)))
                     
                           #if the fluctuation is below 1%, we put the score to the ref
                           if putToRef:   
                              if score_reso > 0 and score_reso < 1:
                                 score_reso = -0.001
                              if abs(score_eff) < 1:
                                 score_eff = 0
                            
                           
                           weight_reso = 1
                           weight_eff = 1
                           
                           if addNoiseRateWeight:
                              if getFloat(iSample.noiseRate) > 0.25: continue
                              weight_noiseRate = exp(1*getFloat(iSample.noiseRate))
                              # weight on eff:
                              if score_eff == 0:
                                 score = weight_reso*abs(score_reso) + weight_eff*weight_noiseRate
                              else:
                                 score = weight_reso*abs(score_reso) + weight_eff*abs(score_eff)*weight_noiseRate
                              # weight on reso:
                              #if score_reso == 0:
                              #   score = weight_reso*weight_noiseRate + weight_eff*abs(score_eff)
                              #else:
                              #   score = weight_reso*abs(score_reso)*weight_noiseRate + weight_eff*abs(score_eff)
                              
                              score_error = score_reso_err + (score_eff_err + 1*score_eff*getFloat(iSample.noiseRate_error))*weight_noiseRate
                           else:
                              score = weight_reso*score_reso - weight_eff*score_eff
                              score_error=0
                           selection.append('{a} {b} {c} {d} {e} {f}'.format(a=iPFRecHit, b=iSeeding, c=score_reso, d=score_eff, e=score, f=score_error))
                           #print('{a} {b} score_reso: {c} score_reso_err: {d} score_eff: {e} score_eff_err: {f} noiseRate: {g} noiseRate_err: {h} score: {i} score_err: {j}'.format(a=iPFRecHit, b=iSeeding, c=score_reso, d=score_reso_err, e=score_eff, f=score_eff_err, g=weight_noiseRate, h=iSample.noiseRate_error, i=score, j=score_error))
            
            selection.sort(key=getScore)
            #print('\n')
            #for i in selection:
               #print(getScore(i))

            # we select all the pairs compatible within uncertainties
            if addNoiseRateWeight:
               if len(selection)>0:
                  for i, val in enumerate(selection):
                     if areOverlapping(getScore(selection[0])+getScoreError(selection[0]), getScore(selection[i])-getScoreError(selection[i])):
                        selected_pair[iEn][iEta].append('{a} {b}'.format(a=getFloat(getFirstElement(selection[i])), b=getFloat(getSecondElement(selection[i]))))
               else:
                  if getInfBin(iEta) < 2:
                     selected_pair[iEn][iEta].append(' - ')
                  else:
                     #bins where stat is not enough are hardcoded (for plotting concerns) 
                     selected_pair[iEn][iEta].append('1.0 2.0')
                
            #print('Selected index: {a}').format(a=selected_pair[iEn][iEta]) 
            #for item in selected_pair[iEn][iEta]:
            #   print item
            if not addNoiseRateWeight:
               if len(selection)>0:
                  firstPair_left = getFloat(getFirstElement(selection[0]))
                  firstPair_right = getFloat(getSecondElement(selection[0]))
                  secondPair_left = getFloat(getFirstElement(selection[1]))
                  secondPair_right = getFloat(getSecondElement(selection[1]))
                  thirdPair_left = getFloat(getFirstElement(selection[2]))
                  thirdPair_right = getFloat(getSecondElement(selection[2]))
       
                  selected_pair[iEn][iEta].append('{a} {b}'.format(a=getFloat(getFirstElement(selection[0])), b=getFloat(getSecondElement(selection[0]))))
                  #if (getScore(selection[1])-getScore(selection[0]))/getScore(selection[0]) < 0.01:
                  #   selected_pair[iEn][iEta].append('{a} {b}'.format(a=secondPair_left, b=secondPair_right))
                  #if (getScore(selection[2])-getScore(selection[0]))/getScore(selection[0]) < 0.01:
                  #   selected_pair[iEn][iEta].append('{a} {b}'.format(a=thirdPair_left, b=thirdPair_right))
               else:
                  selected_pair[iEn][iEta].append(' - ')

        

         # ranking plots
         if do_rankingPlot=='True':
            c_ranking = TCanvas('c_ranking_{a}_{b}_{c}'.format(a=iEn, b=iEta, c='ranking'), 'c', 1000, 1000)
           
            resolutions.sort(key=getQuantity)
            efficiencies.sort(key=getQuantity, reverse=True)
            noiseRates.sort(key=getQuantity)      
            
            titleRanking = TPaveText(0.15,0.85,0.85,0.95,"brNDC")
            titleRanking.AddText('Energy bin: {a}   #eta bin: {b}'.format(a=iEn, b=iEta))
            titleRanking.SetBorderSize(0)
            titleRanking.SetFillColor(kWhite)
            titleRanking.SetTextSize(0.04)
            titleRanking.SetTextFont(42)
            titleRanking.SetTextAlign(11)
            titleRanking.Draw()
            
            resolutions_ranking = TPaveText(0.1,0.1,0.3,0.8,"brNDC")
            resolutions_ranking.SetBorderSize(0)
            resolutions_ranking.SetFillColor(kWhite)
            resolutions_ranking.SetTextSize(0.032)
            resolutions_ranking.SetTextFont(42)
            resolutions_ranking.SetTextAlign(11)
            if do_resoOverScale != 'True':
               resolutions_ranking.AddText('     Resolution')
            else:
               resolutions_ranking.AddText('     Resolution')
            n_reso=0
            for item in resolutions:
               if n_reso==0: value = 'ref'
               else: value=str(getThirdElementPercentage(item, 2))+'%'
               n_reso=n_reso+1
               resolutions_ranking.AddText('({a}, {b})  {c}'.format(a=getFirstElement(item), b=getSecondElement(item), c=value))
               resolutions_ranking.GetListOfLines().Last().SetTextColor(getColor(float(getFirstElement(item)), float(getSecondElement(item))));
               resolutions_ranking.GetListOfLines().Last().SetTextFont(12);
               for i, val in enumerate(selected_pair[iEn][iEta]):
                  if getFirstElement(item)==getFirstElement(selected_pair[iEn][iEta][i]) and getSecondElement(item)==getSecondElement(selected_pair[iEn][iEta][i], 'all'):
                     resolutions_ranking.GetListOfLines().Last().SetTextColor(1);
                     resolutions_ranking.GetListOfLines().Last().SetTextFont(62);
                     resolutions_ranking.Draw()

 
            efficiencies_ranking = TPaveText(0.4,0.1,0.6,0.8,"brNDC")
            efficiencies_ranking.SetBorderSize(0)
            efficiencies_ranking.SetFillColor(kWhite)
            efficiencies_ranking.SetTextSize(0.032)
            efficiencies_ranking.SetTextFont(42)
            efficiencies_ranking.SetTextAlign(11)
            efficiencies_ranking.AddText('     Efficiency')
            n_eff=0
            for item in efficiencies:
               if n_eff==0: value = 'ref'
               else: value=str(getThirdElementPercentage(item, 2))+'%'
               n_eff=n_eff+1
               efficiencies_ranking.AddText('({a}, {b})  {c}'.format(a=getFirstElement(item), b=getSecondElement(item), c=value))
               efficiencies_ranking.GetListOfLines().Last().SetTextColor(getColor(float(getFirstElement(item)), float(getSecondElement(item))));
               efficiencies_ranking.GetListOfLines().Last().SetTextFont(12);   
               for i, val in enumerate(selected_pair[iEn][iEta]):
                  if getFirstElement(item)==getFirstElement(selected_pair[iEn][iEta][i]) and getSecondElement(item)==getSecondElement(selected_pair[iEn][iEta][i], 'all'):
                     efficiencies_ranking.GetListOfLines().Last().SetTextColor(1);
                     efficiencies_ranking.GetListOfLines().Last().SetTextFont(62);
                     efficiencies_ranking.Draw()
                  
            
 
            noiseRates_ranking = TPaveText(0.7,0.1,0.9,0.8,"brNDC")
            noiseRates_ranking.SetBorderSize(0)
            noiseRates_ranking.SetFillColor(kWhite)
            noiseRates_ranking.SetTextSize(0.032)
            noiseRates_ranking.SetTextFont(42)
            noiseRates_ranking.SetTextAlign(11)
            noiseRates_ranking.AddText('    Noise Rate')
            #n_eff=0
            for item in noiseRates:
               #if n_eff==0: value = 'ref'
               #else: value=str(getThirdElement(item, 2))+'%'
               #n_eff=n_eff+1
               value=str(getThirdElement(item, 3))
               noiseRates_ranking.AddText('({a}, {b})  {c}'.format(a=getFirstElement(item), b=getSecondElement(item), c=value))
               noiseRates_ranking.GetListOfLines().Last().SetTextColor(getColor(float(getFirstElement(item)), float(getSecondElement(item))));
               noiseRates_ranking.GetListOfLines().Last().SetTextFont(12);
               for i, val in enumerate(selected_pair[iEn][iEta]):
                  if getFirstElement(item)==getFirstElement(selected_pair[iEn][iEta][i]) and getSecondElement(item)==getSecondElement(selected_pair[iEn][iEta][i], 'all'):
                     noiseRates_ranking.GetListOfLines().Last().SetTextColor(1);
                     noiseRates_ranking.GetListOfLines().Last().SetTextFont(62);
                     noiseRates_ranking.Draw()
               
            c_ranking.SaveAs("{dir}/ranking_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))

  

   # those are the fixed pair of thresholds per eta bin
   # needed for the decisionPlots, and for the summaryPlot depending on the option
   '''
   table_pair = {}
   table_pair['0p00_0p40'] = '0.0 0.0'
   table_pair['0p40_0p80'] = '0.0 0.0'
   table_pair['0p80_1p00'] = '0.0 0.0'
   table_pair['1p00_1p20'] = '0.0 0.0'
   table_pair['1p20_1p44'] = '0.0 0.0'
   table_pair['1p48_1p64'] = '0.0 0.0'
   table_pair['1p64_1p85'] = '0.0 0.0'
   table_pair['1p85_2p00'] = '0.0 0.0'
   table_pair['2p00_2p20'] = '0.0 0.0'
   table_pair['2p20_2p40'] = '0.0 0.0'
   table_pair['2p40_2p60'] = '0.0 0.0'
   table_pair['2p60_2p80'] = '0.0 0.0'
   table_pair['2p80_3p00'] = '0.0 0.0'
   ''' 
   table_pair = {}
   table_pair['0p00_0p40'] = '3.0 3.0'
   table_pair['0p40_0p80'] = '3.0 3.0'
   table_pair['0p80_1p00'] = '3.0 3.0'
   table_pair['1p00_1p20'] = '3.0 3.0'
   table_pair['1p20_1p44'] = '3.0 3.0'
   table_pair['1p48_1p64'] = '3.0 3.0'
   table_pair['1p64_1p85'] = '3.0 3.0'
   table_pair['1p85_2p00'] = '3.0 3.0'
   table_pair['2p00_2p20'] = '3.0 3.0'
   table_pair['2p20_2p40'] = '3.0 3.0'
   table_pair['2p40_2p60'] = '3.0 3.0'
   table_pair['2p60_2p80'] = '4.0 4.0'
   table_pair['2p80_3p00'] = '4.0 4.0'
   #''' 
   # bins where the statistics is too low to be tuned on
   lowStatBins = [['1_5','2p80_3p00'], ['1_5','2p60_2p80'], ['1_5','2p40_2p60'], ['5_10','2p80_3p00'], ['5_10','2p60_2p80'], ['10_15','2p80_3p00']]
   

   # summary plot
   if do_summaryPlot=='True':
      print('Producing summary plot')

      printWithColour = True  #true means plotting the z scale (reso/eff/noise maps)
      printFromTable = True   #true means taking fixed pairs of thresholds from table instead of outcome of selection
      printWithNumber = True #true means plotting number of z dimension
      printPair = False
      printError = False
      printChi2 = False
      doPlotCurves = True


      if printWithColour == False:
         printWithNumber = False

      # in order not to plot the full energy range
      for iEn in EnRanges[:]:
         if getFloat(getUpperBin(iEn)) > 100:
            EnRanges.remove(iEn)

      # removing the crack bin
      for iEta in EtaRanges[:]:
         if iEta == '1p44_1p48':
            EtaRanges.remove(iEta)
         # provisory
         #if iEta == '2p80_3p00':
         #   EtaRanges.remove(iEta)


      nBins_energy = len(EnRanges)
      nBins_eta = len(EtaRanges)

      binInf_energy = getFloat(getInfBin(EnRanges))
      binInf_eta = getFloat(getInfBin(EtaRanges), 'p')

      binSup_energy = getFloat(getSupBin(EnRanges))
      binSup_eta = getFloat(getSupBin(EtaRanges), 'p')
     
      energy_boundaries = [ 0 ]
      eta_boundaries = [ 0 ]
      for iEn in EnRanges:
         energy_boundaries.append(getFloat(getUpperBin(iEn)))

      for iEta in EtaRanges:
         eta_boundaries.append(getFloat(getUpperBin(iEta), 'p'))
         

      binsEnergy = array('d', energy_boundaries)
      binsEta = array('d', eta_boundaries)

      whichQuantities = ['']
      if printWithColour:
         whichQuantities = ['Resolution', 'Efficiency', 'NoiseRate', 'Scale']
      
      for item in whichQuantities:
         if printWithColour:
           histo_summary = TH2D('histo_summary_{a}'.format(a=item), 'histo_summary_{a}'.format(a=item), nBins_eta, binsEta, nBins_energy, binsEnergy)
         else:
           histo_summary = TH2D('histo_summary', 'histo_summary', nBins_eta, binInf_eta, binSup_eta, nBins_energy, binInf_energy, binSup_energy)
         histo_summary.SetTitle(' ')
         
         histo_summary.GetXaxis().SetTitle('#eta')
         histo_summary.GetXaxis().SetLabelSize(0.028)
         histo_summary.GetXaxis().SetTitleSize(0.04)
         histo_summary.GetXaxis().SetTitleOffset(1.2)

         histo_summary.GetYaxis().SetTitle('Energy')
         histo_summary.GetYaxis().SetLabelSize(0.028)
         histo_summary.GetYaxis().SetTitleSize(0.04)
         histo_summary.GetYaxis().SetTitleOffset(1.2)
   
         if printWithColour:
            if do_resoOverScale != 'True':
               histo_summary.GetZaxis().SetTitle(item)
            else:
               if item == 'Resolution':
                  histo_summary.GetZaxis().SetTitle('Resolution')
               else:
                  histo_summary.GetZaxis().SetTitle(item)
            histo_summary.GetZaxis().SetTitleSize(0.04)
            histo_summary.GetZaxis().SetTitleOffset(1.2)
            if item == 'Resolution':
               histo_summary.GetZaxis().SetRangeUser(-0.0001,0.3)
            elif item == 'Efficiency':
               histo_summary.GetZaxis().SetRangeUser(-0.0001,1)
            elif item == 'NoiseRate':
               histo_summary.GetZaxis().SetRangeUser(-0.0001,1)
            elif item == 'NoiseOccupancy':
               histo_summary.GetZaxis().SetRangeUser(0, 20)
            elif item == 'Scale':
               histo_summary.GetZaxis().SetRangeUser(0.7, 1.3)
            elif item == 'RMS':
               histo_summary.GetZaxis().SetRangeUser(0, 0.3)

         c_summary = TCanvas('c_summary_{a}'.format(a=item), 'c_summary_{a}'.format(a=item), 1500, 1500)

         if printWithColour:
            for iEn in EnRanges:
               for iEta in EtaRanges:
                  if iEta == '1p44_1p48': continue
                  for iSample in samples_binned[iEn][iEta]:
                     if not printFromTable:
                        if(len(selected_pair[iEn][iEta])>2):
                           if iSample.pfRecHit==getFirstElement(selected_pair[iEn][iEta][0]):  
                              if iSample.seeding[0:len(iSample.pfRecHit)-1]==getSecondElement(selected_pair[iEn][iEta][0]):
                                 if item == 'Resolution':
                                    if do_resoOverScale != 'True':
                                       quantity = getFloat(iSample.resolution)
                                    else:
                                       if getFloat(iSample.scale) != 0:
                                          quantity = getFloat(iSample.resolution)/getFloat(iSample.scale)
                                 elif item == 'Efficiency':
                                    quantity = getFloat(iSample.efficiency)
                                 elif item == 'NoiseRate':
                                    quantity = getFloat(iSample.noiseRate)
                                    #if quantity == 0.: quantity=0.001
                                 elif item == 'NoiseOccupancy':
                                    quantity = getFloat(iSample.noiseOccupancy)
                                 elif item == 'Scale':
                                    quantity = getFloat(iSample.scale)
                                 elif item == 'RMS':
                                    quantity = getFloat(iSample.rms)
                                 histo_summary.Fill(iEta, iEn, quantity)
                        else:
                            if iSample.pfRecHit==getFirstElement(selected_pair[iEn][iEta][0]):  
                              if iSample.seeding[0:len(iSample.pfRecHit)-1]==getSecondElement(selected_pair[iEn][iEta][0]):
                                 if item == 'Resolution':
                                    if do_resoOverScale != 'True':
                                       quantity = getFloat(iSample.resolution)
                                    else:
                                       quantity = getFloat(iSample.resolution)/getFloat(iSample.scale)
                                 elif item == 'Efficiency':
                                    quantity = getFloat(iSample.efficiency)
                                 elif item == 'NoiseRate':
                                    quantity = getFloat(iSample.noiseRate)
                                 elif item == 'NoiseOccupancy':
                                    quantity = getFloat(iSample.noiseOccupancy)
                                 elif item == 'Scale':
                                    quantity = getFloat(iSample.scale)
                                 elif item == 'RMS':
                                    quantity = getFloat(iSample.rms)
                                 histo_summary.Fill(iEta, iEn, quantity)
                                 if printError or printChi2: #shifts the text so that error can be printed
                                    histo_summary.SetBarOffset(0.3)
                     else:
                        if iSample.pfRecHit==getFirstElement(table_pair[iEta]) and iSample.seeding==getSecondElement(table_pair[iEta], 'all'):
                           if item == 'Resolution':
                              if do_resoOverScale != 'True':
                                 quantity = getFloat(iSample.resolution)
                              else:
                                 if getFloat(iSample.scale) != 0:
                                    quantity = getFloat(iSample.resolution)/getFloat(iSample.scale)
                                 else:
                                    quantity = getFloat(iSample.resolution)
                           elif item == 'Efficiency':
                              quantity = getFloat(iSample.efficiency)
                           elif item == 'NoiseRate':
                              quantity = getFloat(iSample.noiseRate)
                           elif item == 'NoiseOccupancy':
                              quantity = getFloat(iSample.noiseOccupancy)
                           elif item == 'Scale':
                              quantity = getFloat(iSample.scale)
                           elif item == 'RMS':
                              quantity = getFloat(iSample.rms)
                           if quantity == 0: quantity = 0.0001
                           histo_summary.Fill(iEta, iEn, quantity)
                           if printError or printChi2: #shifts the text so that error can be printed
                              histo_summary.SetBarOffset(0.3)
               if printWithNumber == True:
                  histo_summary.Draw('text' + 'colz')
               else:
                  histo_summary.Draw('colz')
         else:
            histo_summary.Draw()
        
         #make so that only 1 digit is printed when using text function
         gStyle.SetPaintTextFormat(".2f");

         #draw dashed lines
         dashed_lines = []
       
         for iEn in EnRanges: 
            for iEta in EtaRanges:
               if getFloat(getUpperBin(iEn)) != getFloat(getSupBin(EnRanges)):
                  line_hor = TLine(getFloat(getInfBin(EtaRanges), 'p'), getFloat(getUpperBin(iEn)), getFloat(getSupBin(EtaRanges), 'p'), getFloat(getUpperBin(iEn)))
                  dashed_lines.append(line_hor)
               if getFloat(getUpperBin(iEta), 'p') !=  getFloat(getSupBin(EtaRanges), 'p'):
                  line_vert = TLine(getFloat(getUpperBin(iEta), 'p'), getFloat(getInfBin(EnRanges)), getFloat(getUpperBin(iEta), 'p'), getFloat(getSupBin(EnRanges))) 
                  dashed_lines.append(line_vert)


         for line in dashed_lines:
            line.SetLineStyle(9)
            line.SetLineWidth(3)
            line.Draw('same')


         # we draw the best score 
         # if printWithNumber is off, then the pair the thresholds is instead written
         score_label = []
         for iEn in EnRanges:
            for iEta in EtaRanges:
               if iEta == '1p44_1p48': continue
               x1 = (getFloat(getUpperBin(iEta), 'p') + getFloat(getLowerBin(iEta), 'p'))/2 - (getFloat(getUpperBin(iEta), 'p') - getFloat(getLowerBin(iEta), 'p'))*0.25
               x2 = (getFloat(getUpperBin(iEta), 'p') + getFloat(getLowerBin(iEta), 'p'))/2 + (getFloat(getUpperBin(iEta), 'p') - getFloat(getLowerBin(iEta), 'p'))*0.25
               if (printError or printChi2) and (iEn == "1_5" or iEn == '5_10' or iEn == '10_15' or iEn == '15_20'):
                  y1 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 - (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.75
               else:
                  y1 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 - (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.25
               y2 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 + (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.25
               score_print = TPaveText(x1, y1, x2, y2)
               if not printFromTable:
                  if [iEn, iEta] not in lowStatBins:
                     if len(selected_pair[iEn][iEta])!=0 and selected_pair[iEn][iEta][0]!=' - ':
                        for iPair in selected_pair[iEn][iEta]:
                           score_print.AddText('({a}, {b})'.format(a=int(getFloat(getFirstElement(iPair))), b=int(getFloat(getSecondElement(iPair, 'all')))))
                           score_print.GetListOfLines().Last().SetTextColor(getColor(int(getFloat(getFirstElement(iPair))), int(getFloat(getSecondElement(iPair, 'all')))))
                     else:
                        score_print.AddText(' - ')
                     score_print.SetFillColorAlpha(0, 0)
                  else: #we don't print pair in low stat bins
                     score_print.SetFillColor(1)
                     score_print.SetFillStyle(3244)
                     
               else:
                  if [iEn, iEta] not in lowStatBins:
                     if printPair:
                        score_print.AddText('({a}, {b})'.format(a=int(getFloat(getFirstElement(table_pair[iEta]))), b=int(getFloat(getSecondElement(table_pair[iEta], 'all')))))
                        score_print.GetListOfLines().Last().SetTextColor(getColor(int(getFloat(getFirstElement(table_pair[iEta]))), int(getFloat(getSecondElement(table_pair[iEta], 'all')))))
                        score_print.SetFillColorAlpha(0, 0)
                     elif printError: #we print the error
                        for iSample in samples_binned[iEn][iEta]:
                                 if item == 'Resolution':
                                    if do_resoOverScale != 'True':
                                       quantity_error = getFloat(iSample.resolution_error)
                                    else:
                                       if getFloat(iSample.scale) != 0 and getFloat(iSample.resolution) != 0:
                                          quantity_error = quantity * (getFloat(iSample.resolution_error)/getFloat(iSample.resolution) + getFloat(iSample.scale_error)/getFloat(iSample.scale))
                                       else:
                                          quantity_error = getFloat(iSample.resolution_error)
                                 elif item == 'Efficiency':
                                    quantity_error = getFloat(iSample.efficiency_error)
                                 elif item == 'NoiseRate':
                                    quantity_error = getFloat(iSample.noiseRate_error)
                                 #elif item == 'NoiseOccupancy':
                                 #   quantity = getFloat(iSample.noiseOccupancy)
                                 elif item == 'RMS':
                                    quantity_error = getFloat(iSample.rms_error)
                                 
                                 if iSample.pfRecHit==getFirstElement(table_pair[iEta]) and iSample.seeding==getSecondElement(table_pair[iEta], 'all'):
                                    score_print.AddText('#pm {a}'.format(a=round(float(quantity_error), 3)))
                                    score_print.SetFillColorAlpha(0, 0)
                     elif printChi2: #we print the chi2 (for the resolution)
                        for iSample in samples_binned[iEn][iEta]:
                           if item == 'Resolution' or item == 'Scale':
                              if iSample.pfRecHit==getFirstElement(table_pair[iEta]) and iSample.seeding==getSecondElement(table_pair[iEta], 'all'):
                                 score_print.AddText("{a}{b}".format(a='#chi^{2}', b=round(float(iSample.chi2), 2)))
                                 score_print.SetFillColorAlpha(0, 0)
                  else:
                     score_print.SetFillColor(1)
                     score_print.SetFillStyle(3244)
               score_label.append(score_print)
            
         for label in score_label:
            if printPair or printError or printChi2:
               label.Draw('same')
            label.SetBorderSize(0)
            label.SetTextSize(0.015)
            label.SetTextFont(62)
            label.SetTextAlign(11)
         
         # we plot transverse energy at given values
         f_ET_2 = TF1('f_ET_2', '2/sin(2*atan(exp(-x)))', getFloat(getInfBin(EtaRanges), 'p'), getFloat(getSupBin(EtaRanges), 'p')) 
         f_ET_2.SetLineWidth(4)

         f_ET_5 = TF1('f_ET_5', '5/sin(2*atan(exp(-x)))', getFloat(getInfBin(EtaRanges), 'p'), getFloat(getSupBin(EtaRanges), 'p')) 
         f_ET_5.SetLineWidth(4)
         f_ET_5.SetLineColor(kBlue)

         f_ET_10 = TF1('f_ET_10', '10/sin(2*atan(exp(-x)))', getFloat(getInfBin(EtaRanges), 'p'), getFloat(getSupBin(EtaRanges), 'p')) 
         f_ET_10.SetLineWidth(4)
         f_ET_10.SetLineColor(kMagenta)

         if doPlotCurves:
           f_ET_2.Draw('same')
           f_ET_5.Draw('same')
           f_ET_10.Draw('same')


         legend1 = TLegend(0.1, 0.85, 0.4, 0.9)
         legend1.AddEntry('f_ET_2', 'E_{T} = 2GeV', 'l')
         legend1.SetTextSize(0.025);
         legend1.SetLineColor(0);
         legend1.SetFillColorAlpha(0, 0);
         legend1.SetBorderSize(0);

         legend2 = TLegend(0.1, 0.8, 0.4, 0.85)
         legend2.AddEntry('f_ET_5', 'E_{T} = 5GeV', 'l')
         legend2.SetTextSize(0.025);
         legend2.SetLineColor(0);
         legend2.SetFillColorAlpha(0, 0);
         legend2.SetBorderSize(0);

         legend3 = TLegend(0.1, 0.75, 0.4, 0.8)
         legend3.AddEntry('f_ET_10', 'E_{T} = 10GeV', 'l')
         legend3.SetTextSize(0.025);
         legend3.SetLineColor(0);
         legend3.SetFillColorAlpha(0, 0);
         legend3.SetBorderSize(0);

         if doPlotCurves:
            legend1.Draw('same')
            legend2.Draw('same')
            legend3.Draw('same')

         # noise curves
         #inputFileName = 'noisePlotter/PFRecHitThresholds_EB_ringaveraged_EE_2023/graphs.root'
         inputFileName = 'noisePlotter/PFRecHitThresholds_EB_ringaveraged_EE_TL{tl}/graphs.root'.format(tl=opt.label.split('thl')[1])
         graphEB = noisePlotter.getGraph(inputFileName, graphName='gr_EB_Object', doSmoothing=True)
         graphEE= noisePlotter.getGraph(inputFileName, graphName='gr_EE_Object', doSmoothing=True)
         
         
         mergedGraph1 = noisePlotter.mergeGraphs(g1=graphEB,g2=graphEE,suffix='smooth',mult=1)
         mergedGraph2 = noisePlotter.mergeGraphs(g1=graphEB,g2=graphEE,suffix='smooth',mult=2)
         mergedGraph3 = noisePlotter.mergeGraphs(g1=graphEB,g2=graphEE,suffix='smooth',mult=3)
         mergedGraph4 = noisePlotter.mergeGraphs(g1=graphEB,g2=graphEE,suffix='smooth',mult=4)

         mergedGraph1.SetLineWidth(4)
         mergedGraph1.SetMarkerSize(1.2)
         mergedGraph1.SetMarkerStyle(8)
         mergedGraph1.SetMarkerColor(1)

         mergedGraph2.SetLineWidth(4)
         mergedGraph2.SetMarkerSize(1.2)
         mergedGraph2.SetMarkerStyle(8)
         mergedGraph2.SetMarkerColor(1)

         mergedGraph3.SetLineWidth(4)
         mergedGraph3.SetMarkerSize(1.2)
         mergedGraph3.SetMarkerStyle(8)
         mergedGraph3.SetMarkerColor(1)

         mergedGraph4.SetLineWidth(4)
         mergedGraph4.SetMarkerSize(1.2)
         mergedGraph4.SetMarkerStyle(8)
         mergedGraph4.SetMarkerColor(1)

         if doPlotCurves:
           mergedGraph1.Draw('C')
           mergedGraph2.Draw('C')
           mergedGraph3.Draw('C')
           mergedGraph4.Draw('C')

         legend4 = TLegend(0.1, 0.7, 0.4, 0.75)
         legend4.AddEntry('merged_smooth_1', 'noise curves {1,..,4}#sigma', 'l')
         legend4.SetTextSize(0.025);
         legend4.SetLineColor(0);
         legend4.SetFillColorAlpha(0,0);
         legend4.SetBorderSize(0);

         if doPlotCurves:
           legend4.Draw('same')


         if item == 'Resolution': 
            if do_resoOverScale == 'True':
               c_summary.SaveAs('{a}/summaryPlot_resolution.png'.format(a=outputdir)) 
            else:
               c_summary.SaveAs('{a}/summaryPlot_resolutionOnly.png'.format(a=outputdir)) 
         elif item == 'Efficiency':
            c_summary.SaveAs('{a}/summaryPlot_efficiency.png'.format(a=outputdir))
         elif item == 'NoiseRate':
            c_summary.SaveAs('{a}/summaryPlot_noiseRate.png'.format(a=outputdir))
         elif item == 'NoiseOccupancy':
            c_summary.SaveAs('{a}/summaryPlot_noiseOccupancy.png'.format(a=outputdir))
         elif item == 'Scale':
            c_summary.SaveAs('{a}/summaryPlot_scale.png'.format(a=outputdir))
         elif item == 'RMS':
            c_summary.SaveAs('{a}/summaryPlot_rms.png'.format(a=outputdir))
         else:
            c_summary.SaveAs('{a}/summaryPlot.png'.format(a=outputdir))

   '''
   table_pair = {}
   table_pair['0p00_0p40'] = '3.0 3.0'
   table_pair['0p40_0p80'] = '2.0 3.0'
   table_pair['0p80_1p00'] = '3.0 3.0'
   table_pair['1p00_1p20'] = '3.0 3.0'
   table_pair['1p20_1p44'] = '3.0 3.0'
   table_pair['1p48_1p64'] = '3.0 3.0'
   table_pair['1p64_1p85'] = '4.0 4.0'
   table_pair['1p85_2p00'] = '4.0 4.0'
   table_pair['2p00_2p20'] = '4.0 4.0'
   table_pair['2p20_2p40'] = '4.0 4.0'
   table_pair['2p40_2p60'] = '4.0 4.0'
   table_pair['2p60_2p80'] = '4.0 4.0'
   table_pair['2p80_3p00'] = '4.0 4.0'
   '''
   
   #decision plot
   if do_decisionPlot == 'True':
      print('producing decision plot')

      # we produce one plot per eta range
      for iEta in EtaRanges:

         if iEta =='1p44_1p48': continue

         if getFloat(getUpperBin(iEta), 'p') < 1.48:
            for iEn in EnRanges[:]:
               if getFloat(getUpperBin(iEn), 'p') > 100:
                  EnRanges.remove(iEn)
    
         nBins_energy = len(EnRanges)
         energy_boundaries = [ 0 ]
         for iEn in EnRanges:
            energy_boundaries.append(getFloat(getUpperBin(iEn)))
         binsEnergy = array('d', energy_boundaries)

    
         histo_decision_reso = TH2D('histo_decision_reso_{a}'.format(a=iEta), 'histo_decision_reso_{a}'.format(a=iEta), 1, getFloat(getLowerBin(iEta), 'p'), getFloat(getUpperBin(iEta), 'p'), nBins_energy, binsEnergy) 
         if do_resoOverScale != 'True':
            histo_decision_reso.SetTitle('Resolution          ')
         else:
            histo_decision_reso.SetTitle('Resolution          ')
         
         histo_decision_reso.GetXaxis().SetTitle('#eta')
         histo_decision_reso.GetXaxis().SetLabelSize(0.052)
         histo_decision_reso.GetXaxis().SetTitleSize(0.04)
         histo_decision_reso.GetXaxis().SetTitleOffset(1.2)

         histo_decision_reso.GetYaxis().SetTitle('Energy')
         histo_decision_reso.GetYaxis().SetLabelSize(0.038)
         histo_decision_reso.GetYaxis().SetTitleSize(0.04)
         histo_decision_reso.GetYaxis().SetTitleOffset(1.2)

         histo_decision_reso.GetZaxis().SetTitleSize(0.04)
         histo_decision_reso.GetZaxis().SetTitleOffset(1.2)
         histo_decision_reso.GetZaxis().SetRangeUser(0,0.1)

         histo_decision_eff = TH2D('histo_decision_eff_{a}'.format(a=iEta), 'histo_decision_eff_{a}'.format(a=iEta), 1, getFloat(getLowerBin(iEta), 'p'), getFloat(getUpperBin(iEta), 'p'), nBins_energy, binsEnergy) 
         histo_decision_eff.SetTitle('Efficiency         ')
         
         histo_decision_eff.GetXaxis().SetTitle('#eta')
         histo_decision_eff.GetXaxis().SetLabelSize(0.052)
         histo_decision_eff.GetXaxis().SetTitleSize(0.04)
         histo_decision_eff.GetXaxis().SetTitleOffset(1.2)

         histo_decision_eff.GetYaxis().SetTitle('Energy')
         histo_decision_eff.GetYaxis().SetLabelSize(0.038)
         histo_decision_eff.GetYaxis().SetTitleSize(0.04)
         histo_decision_eff.GetYaxis().SetTitleOffset(1.2)

         histo_decision_eff.GetZaxis().SetTitleSize(0.04)
         histo_decision_eff.GetZaxis().SetTitleOffset(1.2)
         histo_decision_eff.GetZaxis().SetRangeUser(0.9,1)

         histo_decision_noise = TH2D('histo_decision_noise_{a}'.format(a=iEta), 'histo_decision_noise_{a}'.format(a=iEta), 1, getFloat(getLowerBin(iEta), 'p'), getFloat(getUpperBin(iEta), 'p'), nBins_energy, binsEnergy) 
         histo_decision_noise.SetTitle('NoiseRate         ')
         
         histo_decision_noise.GetXaxis().SetTitle('#eta')
         histo_decision_noise.GetXaxis().SetLabelSize(0.052)
         histo_decision_noise.GetXaxis().SetTitleSize(0.04)
         histo_decision_noise.GetXaxis().SetTitleOffset(1.2)

         histo_decision_noise.GetYaxis().SetTitle('Energy')
         histo_decision_noise.GetYaxis().SetLabelSize(0.038)
         histo_decision_noise.GetYaxis().SetTitleSize(0.04)
         histo_decision_noise.GetYaxis().SetTitleOffset(1.2)

         histo_decision_noise.GetZaxis().SetTitleSize(0.04)
         histo_decision_noise.GetZaxis().SetTitleOffset(1.2)
         histo_decision_noise.GetZaxis().SetRangeUser(-0.0001,0.1)


         c_decision = TCanvas('c_decision_{a}'.format(a=iEta), 'c_decision_{a}'.format(a=iEta), 1500, 1000)
         pad1 = TPad('pad1_{a}'.format(a=iEta), 'pad1_{a}'.format(a=iEta), 0, 0, 0.33, 1)
         pad2 = TPad('pad2_{a}'.format(a=iEta), 'pad2_{a}'.format(a=iEta), 0.33, 0, 0.66, 1)
         pad3 = TPad('pad3_{a}'.format(a=iEta), 'pad3_{a}'.format(a=iEta), 0.66, 0, 1, 1)
         pad1.Draw()
         pad2.Draw()
         pad3.Draw()
         
         for iEn in EnRanges:
            for iSample in samples_binned[iEn][iEta]:
               if iSample.pfRecHit == getFirstElement(table_pair[iEta]) and iSample.seeding == getSecondElement(table_pair[iEta], 'all'):   
                  if do_resoOverScale != 'True':
                     resolution = getFloat(iSample.resolution)
                  else:
                     if getFloat(iSample.scale) != 0:
                        resolution = getFloat(iSample.resolution)/getFloat(iSample.scale)
                  histo_decision_reso.Fill(iEta, iEn, resolution)
                  efficiency = getFloat(iSample.efficiency)
                  histo_decision_eff.Fill(iEta, iEn, efficiency)
                  noiseRate = getFloat(iSample.noiseRate)
                  histo_decision_noise.Fill(iEta, iEn, noiseRate)


         pad1.cd()
         histo_decision_reso.Draw('colz')
         pad2.cd()
         histo_decision_eff.Draw('colz')
         pad3.cd()
         histo_decision_noise.Draw('colz')


         #draw dashed lines
         dashed_lines = []
       
         for iEn in EnRanges: 
            if getFloat(getUpperBin(iEn)) != getFloat(getSupBin(EnRanges)):
               line_hor = TLine(getFloat(getLowerBin(iEta), 'p'), getFloat(getUpperBin(iEn)), getFloat(getUpperBin(iEta), 'p'), getFloat(getUpperBin(iEn)))
               dashed_lines.append(line_hor)

         for line in dashed_lines:
            line.SetLineStyle(9)
            line.SetLineWidth(3)
            pad1.cd()
            line.Draw('same')
            pad2.cd()
            line.Draw('same')
            pad3.cd()
            line.Draw('same')

         # plotting the pairs
         score_label = []
         for iEn in EnRanges:
               x1 = (getFloat(getUpperBin(iEta), 'p') + getFloat(getLowerBin(iEta), 'p'))/2 - (getFloat(getUpperBin(iEta), 'p') - getFloat(getLowerBin(iEta), 'p'))*0.15
               x2 = (getFloat(getUpperBin(iEta), 'p') + getFloat(getLowerBin(iEta), 'p'))/2 + (getFloat(getUpperBin(iEta), 'p') - getFloat(getLowerBin(iEta), 'p'))*0.15
               y1 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 - (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.25
               y2 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 + (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.25
               score_print = TPaveText(x1, y1, x2, y2)
               if [iEn, iEta] not in lowStatBins:
                  score_print.AddText('({a}, {b})'.format(a=getFirstElement(table_pair[iEta]), b=getSecondElement(table_pair[iEta], 'all')))
                  score_print.SetFillColorAlpha(0, 0)
               else:      
                  score_print.SetFillColor(1)
                  score_print.SetFillStyle(3244)
               score_label.append(score_print)
         
         for label in score_label:
            label.SetBorderSize(0)
            label.SetTextSize(0.035)
            label.SetTextFont(62)
            label.SetTextAlign(11)
            pad1.cd()
            label.Draw('same')
            pad2.cd()
            label.Draw('same')
            pad3.cd()
            label.Draw('same')

         # adding ET=2GeV reference line
         f_ET_2 = TF1('f_ET_2', '2/sin(2*atan(exp(-x)))', getFloat(getInfBin(EtaRanges), 'p'), getFloat(getSupBin(EtaRanges), 'p')) 
         f_ET_2.SetLineWidth(4)
         f_ET_2.SetLineColor(6)
         pad1.cd()
         f_ET_2.Draw('same')
         pad2.cd()
         f_ET_2.Draw('same')
         pad3.cd()
         f_ET_2.Draw('same')
         

         gStyle.SetOptStat(0)
         gStyle.SetPadRightMargin(0.36)

         c_decision.cd()
         label1 = getFirstElement(table_pair[iEta])[0:1]
         label2 = getSecondElement(table_pair[iEta], 'all')[0:1]
         c_decision.SaveAs('{d}/decisionPlot_{a}_{b}_{c}.png'.format(d=outputdir, a=iEta, b=label1, c=label2))


   if do_chi2Plot == 'True':
      # histo chi2
      histo_chi2 = TH2D('histo_chi2', 'histo_chi2', 1000, -100, 100, 50, 0, 15)
      
      # file with ratio resolution
      fileToRead = open("scale_2021_ratio.txt", "r")
      lineQte = fileToRead.readlines()


      for iEn in EnRanges:
         for iEta in EtaRanges:
            for iSample in samples_binned[iEn][iEta]:
               if iSample.pfRecHit==getFirstElement(table_pair[iEta]) and iSample.seeding==getSecondElement(table_pair[iEta], 'all'):
                  chiSquare = iSample.chi2
                  if chiSquare == '-nan': continue
               for iLine in lineQte:
                  index1_a = iLine.find(' ')
                  index1_b = iLine.find(' ', index1_a+1)
                  index1_c = iLine.find(' ', index1_b+1)
                  ETrange  = iLine[0:index1_a]
                  ETArange = iLine[index1_a+1:index1_b]
                  if iEn == ETrange and iEta ==ETArange:
                     if iSample.pfRecHit==getFirstElement(table_pair[iEta]) and iSample.seeding==getSecondElement(table_pair[iEta], 'all'):
                        reso_ratio = float(iLine[index1_b+1:index1_c])
                        histo_chi2.Fill(reso_ratio, getFloat(chiSquare))

      gStyle.SetOptStat(0)
      gStyle.SetPadRightMargin(0.16) 
     
      histo_chi2.SetTitle(' ')

      histo_chi2.GetXaxis().SetTitle('Scale Ratio 2021')
      histo_chi2.GetXaxis().SetTitleSize(0.04)
      histo_chi2.GetXaxis().SetTitleOffset(1.2)
         
      histo_chi2.GetYaxis().SetTitle('{a} of Ref'.format(a='#chi^{2}'))
      histo_chi2.GetYaxis().SetTitleSize(0.04)
      histo_chi2.GetYaxis().SetTitleOffset(1.2)
    
      c_chi2 = TCanvas('c_chi2', 'c', 900, 900)
      histo_chi2.SetMarkerStyle(21)
      histo_chi2.SetMarkerSize(1.5)
      histo_chi2.SetMarkerColor(4)
      histo_chi2.Draw()
      #histo_chi2.Draw('text' +'same')

      c_chi2.SaveAs('{dir}/chi2.png'.format(dir=outputdir))



