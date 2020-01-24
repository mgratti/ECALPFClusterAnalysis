import sys 
import os
import collections
import operator
import numpy as np
from math import sin, atan, exp

sys.path.append('noisePlotter/')
import getMergedGraph as noisePlotter

from array import *
from ROOT import TH2D, TH2Poly, TH1D, TF1, TLegend, TCanvas, TPaveText, TGraph, TLine, gROOT, gStyle
from ROOT import kWhite, kMagenta, kAzure, kPink, kSpring, kOrange, kCyan, kRed, kGreen, kBlue, kBird, kCherry

class Sample(object):
   def __init__(self, energy=0, eta=0, pfRecHit=0, seeding=0, resolution=0, scale=0, efficiency=0, noiseRate=0):
      self.energy = energy
      self.eta = eta
      self.pfRecHit = pfRecHit
      self.seeding = seeding
      self.resolution = resolution
      self.scale = scale
      self.efficiency = efficiency
      self.noiseRate = noiseRate


def getOptions():
   from argparse import ArgumentParser
   parser = ArgumentParser(description='File to produce necessary plots for threshold tuning', add_help=True)
   parser.add_argument('--doResolutionPlot', type=str, dest='doResolutionPlot', help='resolution plots', default='True')
   parser.add_argument('--doScalePlot', type=str, dest='doScalePlot', help='scale plots', default='True')
   parser.add_argument('--doEfficiencyPlot', type=str, dest='doEfficiencyPlot', help='efficiency plots', default='True')
   parser.add_argument('--doNoiseRatePlot', type=str, dest='doNoiseRatePlot', help='noise rate plots', default='True')
   parser.add_argument('--doRankingPlot', type=str, dest='doRankingPlot', help='ranking plot', default='True')
   parser.add_argument('--doSummaryPlot', type=str, dest='doSummaryPlot', help='summary plot', default='True')
   parser.add_argument('--doPopUpPlot', type=str, dest='doPopUpPlot', help='want plots to pop up?', default='True')
   return parser.parse_args()


#write a function that will retrieve the necessary parameters from the txt files
#this function should return a Sample at a given En and Eta

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
      
         index3 = line.find(' ', index2+1) #we skip the chi2
         index4 = line.find(' ', index3+1)
         reso = line[index3+1:index4]
      
         index5 = line.find(' ', index4+1)
         mean = line[index4+1:index5]

         index6 = line.find(' ', index5+1)
         eff = line[index5+1:index6]

         index7 = line.find(' ', index6+1)
         noise = line[index6+1:index7]

         #print('pfrechit {t1} seeding {t2} energyBin {a1} etaBin {a2} reso {a3} mean {a4} eff {a5}').format(t1=PFRecHitThrs, t2=seedThrs, a1=energyBin, a2=etaBin, a3=reso, a4=mean, a5=eff)
      
         thisSample = Sample(energy=energyBin, eta=etaBin, pfRecHit=PFRecHitThrs, seeding=seedThrs, resolution=reso, scale=mean, efficiency=eff, noiseRate=noise)
         output.append(thisSample)
 
         if energyBin not in EnBins and energyBin != 'ETranges':
            EnBins.append(energyBin)
      
         if etaBin not in EtaBins and etaBin != 'ETAranges':
            EtaBins.append(etaBin)
  
   return output, EnBins, EtaBins, PFRecHit, Seeding


def getFloat(input, condition=None):
   if(condition!='p'):
      dotIndex = input.find('.',0)
   else:
      dotIndex = input.find('p', 0)

   if(dotIndex!=-1):   
      firstPart = float(input[0:dotIndex])
      secondPart = float(input[dotIndex+1:len(input)])
      #print('dotIndex {a} firstPart {b} secondPart {c}'.format(a=dotIndex, b=firstPart, c=secondPart))
      expo = len(input[dotIndex+1:len(input)])
      #print(firstPart+pow(10, -expo)*secondPart)
      if str(firstPart) == '-0.0':
         return firstPart+pow(10, -expo)*secondPart*(-1)
      else:
         return firstPart+pow(10, -expo)*secondPart
   else:
      return int(input)

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
   return float(phrase[index3+1:len(phrase)])

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
   #print('first part: {a} second part: {b} then {c} and {d} and {e}'.format(a=input[0:underScoreIndex], b=input[underScoreIndex+1:len(input)], c=getFloat(input[0:underScoreIndex]), d=getFloat(input[underScoreIndex+1:len(input)]), e=(getFloat(input[0:underScoreIndex]) + getFloat(input[underScoreIndex+1:len(input)]))/2))
   return (getFloat(input[0:underScoreIndex], 'p') + getFloat(input[underScoreIndex+1:len(input)], 'p'))/2


def getInfBin(input):
   index = input[0].find('_')
   return input[0][0:index]   

def getSupBin(input):
   index = input[len(input)-1].find('_')
   return input[len(input)-1][index+1:len(input[len(input)-1])]



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


if __name__ == "__main__":
 
   opt = getOptions()
   
   do_resolutionPlot=opt.doResolutionPlot
   do_scalePlot=opt.doScalePlot
   do_efficiencyPlot=opt.doEfficiencyPlot
   do_noiseRatePlot=opt.doNoiseRatePlot
   do_rankingPlot=opt.doRankingPlot
   do_summaryPlot=opt.doSummaryPlot

   do_popUpPlot = opt.doPopUpPlot
   
   if do_popUpPlot == 'False':
      gROOT.SetBatch(True)

   #outputdirectory
   outputdir = '/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/myPlots/2DScan'
   #outputdir = '/afs/cern.ch/user/a/anlyon/CMSSW_10_6_1_patch1/src/Plotter/myPlots/2DScan'
   os.system('mkdir {}'.format(outputdir))

 
   samples, EnRanges, EtaRanges, pfRechitThrs, seedingThrs = getSampleItems('fileSamples.txt')
  

   #EnRanges  = ["10_15"] #, "5_10", "10_15", "15_20", "20_40", "40_60", "60_80"]
   #EtaRanges = ["1p20_1p44"] #, "0p40_0p80"]

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
                           quantity = iSample.resolution
                           if n==0: ref_quantity=quantity
                           else: 
                              if quantity<ref_quantity and quantity != '0': ref_quantity=quantity
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
                           quantity = iSample.noiseRate
                           if n==0: ref_quantity=quantity
                           else:
                              if quantity>ref_quantity: ref_quantity=quantity

                        histo.Fill(getFloat(iSample.seeding), getFloat(iSample.pfRecHit), getFloat(quantity))
                        #print('{a} {b} {c} {d} {e}'.format(a=what, b=iPFRecHit, c=iSeeding, d=quantity, e=ref_quantity))
	    
            #filling the histo_ratio
            for iPFRecHit in pfRechitThrs:
               for iSeeding in seedingThrs:
                  if(iPFRecHit > iSeeding): continue
                  for iSample in samples_binned[iEn][iEta]:
                      if getFloat(iSample.seeding)==iSeeding and getFloat(iSample.pfRecHit)==iPFRecHit:
                        if what == 'Resolution':
                           quantity = iSample.resolution
                           if getFloat(ref_quantity)!= 0 :
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
                           if getFloat(ref_quantity)!=0:
                              ratio = getFloat(quantity)/getFloat(ref_quantity)
                              #noiseRates.append('{a} {b} {c}'.format(a=iPFRecHit, b=iSeeding, c=ratio))
                        if getFloat(ref_quantity)!= 0:   
                           histo_ratio.Fill(getFloat(iSample.seeding), getFloat(iSample.pfRecHit), getFloat(quantity)/getFloat(ref_quantity))
                           #if getFloat(quantity)/getFloat(ref_quantity) >= 1.01:
                           #   testFile.write('{a} {b} {c} {d} {e}% \n'.format(a=iEn, b=iEta, c=iPFRecHit, d=iSeeding, e=round((getFloat(quantity)/getFloat(ref_quantity)-1)*100, 3)))
            
	    #gStyle.SetOptStat(0)
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
               histo.GetZaxis().SetTitle('Resolution')
            elif what == 'Scale':
               histo.GetZaxis().SetTitle('Scale')
            elif what == 'Efficiency':
               histo.GetZaxis().SetTitle('Efficiency')
            elif what == 'noiseRate':
               histo.GetZaxis().SetTitle('Noise Rate')

            if what == 'Resolution':
               histo.GetZaxis().SetRangeUser(0, 0.4)
            elif what == 'Scale':
               histo.GetZaxis().SetRangeUser(0.5, 1.5)
            elif what == 'Efficiency':
               histo.GetZaxis().SetRangeUser(0, 1)
            elif what == 'NoiseRate':
               histo.GetZaxis().SetRangeUser(0, 2)
 
            histo.GetZaxis().SetTitleSize(0.04)
            histo.GetZaxis().SetTitleOffset(1.2)
    
            histo_ratio.GetXaxis().SetTitle('Seeding thrs multiplicity')
            histo_ratio.GetXaxis().SetTitleSize(0.04)
            histo_ratio.GetXaxis().SetTitleOffset(1.2)
      
            histo_ratio.GetYaxis().SetTitle('PFRecHit thrs multiplicity')
            histo_ratio.GetYaxis().SetTitleSize(0.04)
            histo_ratio.GetYaxis().SetTitleOffset(1.2)
 
            if what == 'Resolution':
               histo_ratio.GetZaxis().SetTitle('Resolution')
            elif what == 'Scale':
               histo_ratio.GetZaxis().SetTitle('Scale')
            elif what == 'Efficiency':
               histo_ratio.GetZaxis().SetTitle('Efficiency')
            elif what == 'NoiseRate':
               histo_ratio.GetZaxis().SetTitle('Noise Rate')

            if what == 'Resolution':
               histo_ratio.GetZaxis().SetRangeUser(0, 0.4)
            elif what == 'Scale':
               histo_ratio.GetZaxis().SetRangeUser(0.5, 1.5)
            elif what == 'Efficiency':
               histo_ratio.GetZaxis().SetRangeUser(0, 1)
            elif what == 'NoiseRate':
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
               c.SaveAs("{dir}/resolution_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_scalePlot == 'True' and what == 'Scale':
               c.SaveAs("{dir}/scale_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_efficiencyPlot == 'True' and what == 'Efficiency':
               c.SaveAs("{dir}/efficiency_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_noiseRatePlot == 'True' and what == 'NoiseRate':
               c.SaveAs("{dir}/noiseRate_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))


            if do_resolutionPlot == 'True' and what == 'Resolution':
               c_ratio.SaveAs("{dir}/ratio_resolution_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_scalePlot == 'True' and what == 'Scale':
               c_ratio.SaveAs("{dir}/ratio_scale_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_efficiencyPlot == 'True' and what == 'Efficiency':
               c_ratio.SaveAs("{dir}/ratio_efficiency_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
            elif do_noiseRatePlot == 'True' and what == 'NoiseRate':
               c_ratio.SaveAs("{dir}/ratio_noiseRate_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))
	    	

            del histo
            del histo_ratio

         #selection
         if do_rankingPlot == 'True' or do_summaryPlot == 'True':
            selection = []

            if iEn == 'ETranges': continue
            if iEta == 'ETAranges': continue
            

            n=0
            for iPFRecHit in pfRechitThrs:
               for iSeeding in seedingThrs:
               
                  if(iPFRecHit > iSeeding): continue
                     
                  for iSample in samples_binned[iEn][iEta]:
                      if getFloat(iSample.seeding)==iSeeding and getFloat(iSample.pfRecHit)==iPFRecHit:
                           reso = iSample.resolution
                           eff = iSample.efficiency
                           if n==0:
                              ref_reso=reso
                              ref_eff=eff
                           else: 
                              if reso<ref_reso and reso != '0': ref_reso=reso
                              if eff>ref_eff: ref_eff=eff
                           n=n+1
                              
                           #print('{a} {b} {c} {d} {e}'.format(a=what, b=iPFRecHit, c=iSeeding, d=reso, e=ref_reso))
             
            for iPFRecHit in pfRechitThrs:
               for iSeeding in seedingThrs:
                  if(iPFRecHit > iSeeding): continue
                  for iSample in samples_binned[iEn][iEta]:
                     if getFloat(iSample.seeding)==iSeeding and getFloat(iSample.pfRecHit)==iPFRecHit:
                        reso=iSample.resolution
                        eff=iSample.efficiency
                        if reso == '0': continue
                        if getFloat(ref_reso)!=0 and getFloat(ref_eff)!=0:
                           ratio_reso = getFloat(reso)/getFloat(ref_reso)
                           ratio_eff = getFloat(eff)/getFloat(ref_eff)
                           if(ratio_reso!=1):
                              score_reso = round((ratio_reso-1)*100, 2)
                           else:
                              score_reso = -0.001 #0 #1
                           if(ratio_eff!=1):
                              score_eff = round((ratio_eff-1)*100, 2)
                           else:
                              score_eff = 0 #-1
                        
                           #if the fluctuation is below 1%, we put the score to the ref
                           if score_reso > 0 and score_reso < 1:
                              score_reso = 0
                           if abs(score_eff) < 1:
                              score_eff = 0
                           weight_reso = 1
                           weight_eff = 1
                           score = weight_reso*score_reso - weight_eff*score_eff
                           #score = -1*score_reso*score_eff
                           selection.append('{a} {b} {c} {d} {e}'.format(a=iPFRecHit, b=iSeeding, c=score_reso, d=score_eff, e=score))
                           #print('{a} {b} {c} {d} {e}'.format(a=iPFRecHit, b=iSeeding, c=score_reso, d=score_eff, e=score))
             
            selection.sort(key=getScore)
            #print('\n')
            #for i in selection:
            #   print(getScore(i))
            if len(selection)>0:
               firstPair_left = getFloat(getFirstElement(selection[0]))
               firstPair_right = getFloat(getSecondElement(selection[0]))
               secondPair_left = getFloat(getFirstElement(selection[1]))
               secondPair_right = getFloat(getSecondElement(selection[1]))
               thirdPair_left = getFloat(getFirstElement(selection[2]))
               thirdPair_right = getFloat(getSecondElement(selection[2]))
    
               selected_pair[iEn][iEta].append('{a} {b}'.format(a=getFloat(getFirstElement(selection[0])), b=getFloat(getSecondElement(selection[0]))))
               if (getScore(selection[1])-getScore(selection[0]))/getScore(selection[0]) < 0.01:
                  selected_pair[iEn][iEta].append('{a} {b}'.format(a=secondPair_left, b=secondPair_right))
               if (getScore(selection[2])-getScore(selection[0]))/getScore(selection[0]) < 0.01:
                  selected_pair[iEn][iEta].append('{a} {b}'.format(a=thirdPair_left, b=thirdPair_right))
            else:
               selected_pair[iEn][iEta].append(' - ')
                    
            #print('Selected index: {a}').format(a=selected_pair[iEn][iEta]) 
           


 
         # ranking
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
            
            #resolutions_ranking = TPaveText(0.15,0.1,0.45,0.8,"brNDC")
            resolutions_ranking = TPaveText(0.1,0.1,0.3,0.8,"brNDC")
            resolutions_ranking.SetBorderSize(0)
            resolutions_ranking.SetFillColor(kWhite)
            resolutions_ranking.SetTextSize(0.032)
            resolutions_ranking.SetTextFont(42)
            resolutions_ranking.SetTextAlign(11)
            resolutions_ranking.AddText('     Resolution')
            n_reso=0
            for item in resolutions:
               if n_reso==0: value = 'ref'
               else: value=str(getThirdElementPercentage(item, 2))+'%'
               n_reso=n_reso+1
	            #resolutions_ranking.SetTextFont(12)
               resolutions_ranking.AddText('({a}, {b})  {c}'.format(a=getFirstElement(item), b=getSecondElement(item), c=value))
               if getFirstElement(item)==getFirstElement(selected_pair[iEn][iEta][0]) and getSecondElement(item)==getSecondElement(selected_pair[iEn][iEta][0], 'all'):
                  resolutions_ranking.GetListOfLines().Last().SetTextColor(1);
                  resolutions_ranking.GetListOfLines().Last().SetTextFont(62);
               else:
                  resolutions_ranking.GetListOfLines().Last().SetTextColor(getColor(float(getFirstElement(item)), float(getSecondElement(item))));
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
               if getFirstElement(item)==getFirstElement(selected_pair[iEn][iEta][0]) and getSecondElement(item)==getSecondElement(selected_pair[iEn][iEta][0], 'all'):
                  efficiencies_ranking.GetListOfLines().Last().SetTextColor(1);
                  efficiencies_ranking.GetListOfLines().Last().SetTextFont(62);
               else:
                  efficiencies_ranking.GetListOfLines().Last().SetTextColor(getColor(float(getFirstElement(item)), float(getSecondElement(item))));
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
               if getFirstElement(item)==getFirstElement(selected_pair[iEn][iEta][0]) and getSecondElement(item)==getSecondElement(selected_pair[iEn][iEta][0], 'all'):
                  noiseRates_ranking.GetListOfLines().Last().SetTextColor(1);
                  noiseRates_ranking.GetListOfLines().Last().SetTextFont(62);
               else:
                  noiseRates_ranking.GetListOfLines().Last().SetTextColor(getColor(float(getFirstElement(item)), float(getSecondElement(item))));
                  noiseRates_ranking.Draw()
            
            c_ranking.SaveAs("{dir}/ranking_E_{a}_Eta_{b}.png".format(dir=outputdir, a=iEn, b=iEta))


   # summary plot
   if do_summaryPlot=='True':
      print('Producing summary plot')

      printWithColour = True

      # in order not to plot the full energy range
      for iEn in EnRanges[:]:
         if getFloat(getUpperBin(iEn)) > 80:
            EnRanges.remove(iEn)

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
         whichQuantities = ['Resolution', 'Efficiency']
      
      for item in whichQuantities:
         if printWithColour:
           histo_summary = TH2D('histo_summary_{a}'.format(a=item), 'histo_summary_{a}'.format(a=item), nBins_eta, binsEta, nBins_energy, binsEnergy)
         else:
           histo_summary = TH2D('histo_summary', 'histo_summary', nBins_eta, binInf_eta, binSup_eta, nBins_energy, binInf_energy, binSup_energy)
         histo_summary.SetTitle(' ')
         
         c_summary = TCanvas('c_summary_{a}'.format(a=item), 'c_summary_{a}'.format(a=item), 1500, 1500)
         histo_summary.GetXaxis().SetTitle('#eta')
         histo_summary.GetXaxis().SetLabelSize(0.028)
         histo_summary.GetXaxis().SetTitleSize(0.04)
         histo_summary.GetXaxis().SetTitleOffset(1.2)

         histo_summary.GetYaxis().SetTitle('Energy')
         histo_summary.GetYaxis().SetLabelSize(0.028)
         histo_summary.GetYaxis().SetTitleSize(0.04)
         histo_summary.GetYaxis().SetTitleOffset(1.2)
   
         if printWithColour:
            histo_summary.GetZaxis().SetTitle(item)
            histo_summary.GetZaxis().SetTitleSize(0.04)
            histo_summary.GetZaxis().SetTitleOffset(1.2)
            histo_summary.GetZaxis().SetRangeUser(0,0.6)

         if printWithColour:
            for iEn in EnRanges:
               for iEta in EtaRanges:
                  for iSample in samples_binned[iEn][iEta]:
                     if(len(selected_pair[iEn][iEta])>1):
                        if iSample.pfRecHit==getFirstElement(selected_pair[iEn][iEta][0]):  
                           if iSample.seeding[0:len(iSample.pfRecHit)-1]==getSecondElement(selected_pair[iEn][iEta][0]):
                              if item == 'Resolution':
                                 quantity = getFloat(iSample.resolution)
                              elif item == 'Efficiency':
                                 quantity = getFloat(iSample.efficiency)
                              histo_summary.Fill(iEta, iEn, quantity)
                     else:
                         if iSample.pfRecHit==getFirstElement(selected_pair[iEn][iEta][0]):  
                           if iSample.seeding[0:len(iSample.pfRecHit)-1]==getSecondElement(selected_pair[iEn][iEta][0]):
                              if item == 'Resolution':
                                 quantity = getFloat(iSample.resolution)
                              elif item == 'Efficiency':
                                 quantity = getFloat(iSample.efficiency)
                              histo_summary.Fill(iEta, iEn, quantity)
            histo_summary.Draw('colz')
         else:
            histo_summary.Draw()
         

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
         score_label = []
         for iEn in EnRanges:
            for iEta in EtaRanges:
               if iEta == '1p44_1p48': continue
               #print(selected_pair[iEn][iEta])   
               x1 = (getFloat(getUpperBin(iEta), 'p') + getFloat(getLowerBin(iEta), 'p'))/2 - (getFloat(getUpperBin(iEta), 'p') - getFloat(getLowerBin(iEta), 'p'))*0.25
               x2 = (getFloat(getUpperBin(iEta), 'p') + getFloat(getLowerBin(iEta), 'p'))/2 + (getFloat(getUpperBin(iEta), 'p') - getFloat(getLowerBin(iEta), 'p'))*0.25
               y1 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 - (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.25
               y2 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 + (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.25
               #print('{aa} {bb} {a} {b} {c} {d}').format(aa=iEn, bb=iEta, a=x1, b=x2, c=y1, d=y2)   
               #print('{a} {b}').format(a=int(getPairInf(selected_pair[iEn][iEta])), b=getPairSup(selected_pair[iEn][iEta]))
               score_print = TPaveText(x1, y1, x2, y2)
               #if selected_pair[iEn][iEta][0]!=' - ':
               if len(selected_pair[iEn][iEta])!=0:
                  #score_print.AddText(selected_pair[iEn][iEta])
                  for iPair in selected_pair[iEn][iEta]:
                     score_print.AddText('({a}, {b})'.format(a=int(getFloat(getFirstElement(iPair))), b=int(getFloat(getSecondElement(iPair, 'all')))))
                  #score_print.SetTextColor(getColor(int(getPairInf(selected_pair[iEn][iEta])), int(getPairSup(selected_pair[iEn][iEta]))))
                     score_print.GetListOfLines().Last().SetTextColor(getColor(int(getFloat(getFirstElement(iPair))), int(getFloat(getSecondElement(iPair, 'all')))))
               else:
                  score_print.AddText(' - ')
               score_label.append(score_print)         
         
         for label in score_label:
            label.Draw('same')
            label.SetBorderSize(0)
            label.SetFillColorAlpha(0, 0)
            label.SetTextSize(0.015)
            label.SetTextFont(62)
            label.SetTextAlign(11)
         
         # we plot transverse energy at given values
         f_ET_2 = TF1('f_ET_2', '2/sin(2*atan(exp(-x)))', getFloat(getInfBin(EtaRanges), 'p'), getFloat(getSupBin(EtaRanges), 'p')) 
         f_ET_2.SetLineWidth(4)
         f_ET_2.Draw('same')

         f_ET_5 = TF1('f_ET_5', '5/sin(2*atan(exp(-x)))', getFloat(getInfBin(EtaRanges), 'p'), getFloat(getSupBin(EtaRanges), 'p')) 
         f_ET_5.SetLineWidth(4)
         f_ET_5.SetLineColor(kBlue)
         f_ET_5.Draw('same')

         f_ET_10 = TF1('f_ET_10', '10/sin(2*atan(exp(-x)))', getFloat(getInfBin(EtaRanges), 'p'), getFloat(getSupBin(EtaRanges), 'p')) 
         f_ET_10.SetLineWidth(4)
         f_ET_10.SetLineColor(kMagenta)
         f_ET_10.Draw('same')


         legend1 = TLegend(0.1, 0.85, 0.4, 0.9)
         legend1.AddEntry('f_ET_2', 'E_{T} = 2GeV', 'l')
         legend1.SetTextSize(0.025);
         legend1.SetLineColor(0);
         legend1.SetFillColorAlpha(0, 0);
         legend1.SetBorderSize(0);
         legend1.Draw('same')

         legend2 = TLegend(0.1, 0.8, 0.4, 0.85)
         legend2.AddEntry('f_ET_5', 'E_{T} = 5GeV', 'l')
         legend2.SetTextSize(0.025);
         legend2.SetLineColor(0);
         legend2.SetFillColorAlpha(0, 0);
         legend2.SetBorderSize(0);
         legend2.Draw('same')

         legend3 = TLegend(0.1, 0.75, 0.4, 0.8)
         legend3.AddEntry('f_ET_10', 'E_{T} = 10GeV', 'l')
         legend3.SetTextSize(0.025);
         legend3.SetLineColor(0);
         legend3.SetFillColorAlpha(0, 0);
         legend3.SetBorderSize(0);
         legend3.Draw('same')

         # noise curves
         inputFileName = 'noisePlotter/PFRecHitThresholds_EB_ringaveraged_EE_2023/graphs.root'
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
         mergedGraph1.Draw('C')

         mergedGraph2.SetLineWidth(4)
         mergedGraph2.SetMarkerSize(1.2)
         mergedGraph2.SetMarkerStyle(8)
         mergedGraph2.SetMarkerColor(1)
         mergedGraph2.Draw('C')

         mergedGraph3.SetLineWidth(4)
         mergedGraph3.SetMarkerSize(1.2)
         mergedGraph3.SetMarkerStyle(8)
         mergedGraph3.SetMarkerColor(1)
         mergedGraph3.Draw('C')

         mergedGraph4.SetLineWidth(4)
         mergedGraph4.SetMarkerSize(1.2)
         mergedGraph4.SetMarkerStyle(8)
         mergedGraph4.SetMarkerColor(1)
         mergedGraph4.Draw('C')
        

         legend4 = TLegend(0.1, 0.7, 0.4, 0.75)
         legend4.AddEntry('merged_smooth_1', 'noise curves {1,..,4}#sigma', 'l')
         legend4.SetTextSize(0.025);
         legend4.SetLineColor(0);
         legend4.SetFillColorAlpha(0,0);
         legend4.SetBorderSize(0);
         legend4.Draw('same')
        

         if item == 'Resolution': 
            c_summary.SaveAs('{a}/summaryPlot_resolution.png'.format(a=outputdir)) 
         elif item == 'Efficiency':
            c_summary.SaveAs('{a}/summaryPlot_efficiency.png'.format(a=outputdir))








