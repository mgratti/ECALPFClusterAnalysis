import sys 
import os
import collections
import operator
import numpy as np
from math import sin, atan, exp, log, sqrt

sys.path.append('noisePlotter/')
import getMergedGraph as noisePlotter

from array import *
from ROOT import TH2D, TH2Poly, TH1D, TF1, TLegend, TCanvas, TPaveText, TPad, TGraph, TLine, gROOT, gStyle
from ROOT import kWhite, kMagenta, kAzure, kPink, kSpring, kOrange, kCyan, kRed, kGreen, kBlue, kBird, kCherry


#---------------------------------------------------------#

# choose title of the map
title = "Fixed thresholds thrsTL400"

# choose label of z axis
#label = "condTL450/condTL400"
label = "condTL450,condTL400"

# choose at which lumi to plot the noise curves
thrlabel = "180"

### file1 ###
doRefThrs1 = False
doModThrs1 = True

sample1 = []
                        
#sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi235_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T13_v1_t33_n30000_njd0")
#sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi235_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T13_v1_t44_n30000_njd0")
#sample1.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi235_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T13_v2_t33_n30000_njd0")
#sample1.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi235_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T13_v2_t44_n30000_njd0")

#sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi450_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T18_v1_t33_n30000_simFraction_wHF.txt")
#sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi450_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T18_v1_t44_n30000_simFraction_wHF.txt")
#sample1.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi450_pfrh3.0_seed3.0_noMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_l450_T18_v2_t33_n30000_simFraction_wHF.txt")
#sample1.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi450_pfrh4.0_seed4.0_noMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_l450_T18_v2_t44_n30000_simFraction_wHF.txt")

#sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_noiseCond2023_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T9_v1_t33_n30000_simFraction.txt")
#sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_noiseCond2023_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T9_v1_t44_n30000_simFraction.txt")
#sample1.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_noiseCond2023_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T9_v2_t33_n30000_simFraction.txt")
#sample1.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_noiseCond2023_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T9_v2_t44_n30000_simFraction.txt")

sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi400_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T16_v1_t33_n30000_njd0_simFraction.txt")
sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi400_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T16_v1_t44_n30000_njd0_simFraction.txt")
sample1.append("photon_E1to100GeV_closeEcal_EEMerged_noPU_thrsLumi400_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T16_v2_t33_n30000_njd0_simFraction.txt")
sample1.append("photon_E1to100GeV_closeEcal_EEMerged_noPU_thrsLumi400_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T16_v2_t44_n30000_njd0_simFraction.txt")


#sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi450_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l400_T15_v1_t33_n30000_njd0_simFraction.txt")
#sample1.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi450_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l400_T15_v1_t44_n30000_njd0_simFraction.txt")
#sample1.append("photon_E1to100GeV_closeEcal_EEMerged_noPU_thrsLumi450_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l400_T15_v2_t33_n30000_njd0_simFraction.txt")
#sample1.append("photon_E1to100GeV_closeEcal_EEMerged_noPU_thrsLumi450_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l400_T15_v2_t44_n30000_njd0_simFraction.txt")
#--------------

### file2 ###
doRefThrs2 = False
doModThrs2 = True

sample2 = []

#sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_noiseCond2021_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T11_v1_t33_n30000_njd0_simFraction.txt")
#sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_noiseCond2021_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T11_v1_t44_n30000_njd0_simFraction.txt")
#sample2.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_noiseCond2021_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T11_v2_t33_n30000_njd0_simFraction.txt")
#sample2.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_noiseCond2021_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T11_v2_t44_n30000_njd0_simFraction.txt")

#sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi450_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T18_v1_t33_n30000_simFraction.txt")
#sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi450_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T18_v1_t44_n30000_simFraction.txt")
#sample2.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi450_pfrh3.0_seed3.0_noMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_l450_T18_v2_t33_n30000_simFraction.txt")
#sample2.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi450_pfrh4.0_seed4.0_noMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_l450_T18_v2_t44_n30000_simFraction.txt")

#sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2023_T6_v1_t33_n30000_simFraction.txt")
#sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2023_T6_v1_t44_n30000_simFraction.txt")
#sample2.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_pfrh3.0_seed3.0_noMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_y2023_T6_v2_t33_n30000_simFraction.txt")
#sample2.append("photon_E1to200GeV_closeEcal_EEMerged_noPU_pfrh4.0_seed4.0_noMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_y2023_T6_v2_t44_n30000_simFraction.txt")

#sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi450_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T160_v1_t33_n30000_njd0_simFraction.txt")
#sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi450_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T160_v1_t44_n30000_njd0_simFraction.txt")
#sample2.append("photon_E1to100GeV_closeEcal_EEMerged_noPU_thrsLumi450_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T160_v2_t33_n30000_njd0_simFraction.txt")
#sample2.append("photon_E1to100GeV_closeEcal_EEMerged_noPU_thrsLumi450_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l450_T160_v2_t44_n30000_njd0_simFraction.txt")

sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi400_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l400_T150_v1_t33_n30000_njd0_simFraction.txt")
sample2.append("photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi400_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l400_T150_v1_t44_n30000_njd0_simFraction.txt")
sample2.append("photon_E1to100GeV_closeEcal_EEMerged_noPU_thrsLumi400_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l400_T150_v2_t33_n30000_njd0_simFraction.txt")
sample2.append("photon_E1to100GeV_closeEcal_EEMerged_noPU_thrsLumi400_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_l400_T150_v2_t44_n30000_njd0_simFraction.txt")

#--------------

# choose to define the resolution as sigma (=False) or sigma/mu (=True)
do_resoOverScale = 'True'
do_absDiffNoise = 'False'

#---------------------------------------------------------#

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
      

# function that retrieves the necessary parameters from the fit output txt files located in samples/
# this function returns a Sample at a given En and Eta
def getSampleItems(inputsamples):
 
   #the output is a vector of samples
   output = []
   EnBins = []
   EtaBins = [] 
   PFRecHit = []
   Seeding = []

   for sample in inputsamples:
      #inputfile = 'samples/histo_' + sample[0:len(sample)] + '_simFraction.txt'
      inputfile = 'samples/histo_' + sample[0:len(sample)] 

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


def getInfBin(input):
   index = input[0].find('_')
   return input[0][0:index]   


def getSupBin(input):
   index = input[len(input)-1].find('_')
   return input[len(input)-1][index+1:len(input[len(input)-1])]


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

table_pair_mod = {}
table_pair_mod['0p00_0p40'] = '3.0 3.0'
table_pair_mod['0p40_0p80'] = '3.0 3.0'
table_pair_mod['0p80_1p00'] = '3.0 3.0'
table_pair_mod['1p00_1p20'] = '3.0 3.0'
table_pair_mod['1p20_1p44'] = '3.0 3.0'
table_pair_mod['1p48_1p64'] = '3.0 3.0'
table_pair_mod['1p64_1p85'] = '3.0 3.0'
table_pair_mod['1p85_2p00'] = '3.0 3.0'
table_pair_mod['2p00_2p20'] = '3.0 3.0'
table_pair_mod['2p20_2p40'] = '3.0 3.0'
table_pair_mod['2p40_2p60'] = '3.0 3.0'
table_pair_mod['2p60_2p80'] = '4.0 4.0'
table_pair_mod['2p80_3p00'] = '4.0 4.0'


table_pair_ref = {}
table_pair_ref['0p00_0p40'] = '0.0 0.0'
table_pair_ref['0p40_0p80'] = '0.0 0.0'
table_pair_ref['0p80_1p00'] = '0.0 0.0'
table_pair_ref['1p00_1p20'] = '0.0 0.0'
table_pair_ref['1p20_1p44'] = '0.0 0.0'
table_pair_ref['1p48_1p64'] = '0.0 0.0'
table_pair_ref['1p64_1p85'] = '0.0 0.0'
table_pair_ref['1p85_2p00'] = '0.0 0.0'
table_pair_ref['2p00_2p20'] = '0.0 0.0'
table_pair_ref['2p20_2p40'] = '0.0 0.0'
table_pair_ref['2p40_2p60'] = '0.0 0.0'
table_pair_ref['2p60_2p80'] = '0.0 0.0'
table_pair_ref['2p80_3p00'] = '0.0 0.0'
   
   
if __name__ == "__main__":
      
   gROOT.SetBatch(True)

   #outputdirectory
   outputdir = '/t3home/anlyon/CMSSW_10_6_1_patch1/src/ECALPFClusterAnalysis/Plotter/myPlots/ratioMaps/ratioMaps_{}'.format(label.replace(',','_'))
   os.system('mkdir -p {}'.format(outputdir))

   # file dir
   os.system('mkdir -p files/ratiomaps')
   
   # get the samples
   samples1, EnRanges, EtaRanges, pfRechitThrs, seedingThrs = getSampleItems(sample1)

   #since we want a graph per energy/eta bin, we bin the samples vector
   # samples_binned is a list of dictionaries
   samples_binned1 = {}
   for iEn in EnRanges:
      samples_binned1[iEn] = {}
      for iEta in EtaRanges:
         samples_binned1[iEn][iEta] = []

   for iEn in EnRanges:
      for iEta in EtaRanges:
         for sample in samples1:
            if((iEn==sample.energy) and (iEta==sample.eta)):
               samples_binned1[iEn][iEta].append(sample)
 

   samples2, EnRanges, EtaRanges, pfRechitThrs, seedingThrs = getSampleItems(sample2)

   samples_binned2 = {}
   for iEn in EnRanges:
      samples_binned2[iEn] = {}
      for iEta in EtaRanges:
         samples_binned2[iEn][iEta] = []

   for iEn in EnRanges:
      for iEta in EtaRanges:
         for sample in samples2:
            if((iEn==sample.energy) and (iEta==sample.eta)):
               samples_binned2[iEn][iEta].append(sample)
   

   # we create the files with the different quantities for each bin 
   whichQuantities = ['Resolution', 'Efficiency', 'NoiseRate', 'Scale']
   
   # first file

   table_pair1 = {}
   if doRefThrs1:   table_pair1 = table_pair_ref
   elif doModThrs1: table_pair1 = table_pair_mod
   
   for item in whichQuantities:
      fileQte = open("files/ratiomaps/{a}_file1.txt".format(a=item), "w+")
      for iEn in EnRanges:
         for iEta in EtaRanges:
            if iEta == '1p44_1p48': continue
            for iSample in samples_binned1[iEn][iEta]:
               if iSample.pfRecHit==getFirstElement(table_pair1[iEta]) and iSample.seeding==getSecondElement(table_pair1[iEta], 'all'):
                           if item == 'Resolution':
                              if do_resoOverScale != 'True':
                                 quantity = getFloat(iSample.resolution)
                                 quantity_error = getFloat(iSample.resolution_error)
                              else:
                                 if getFloat(iSample.scale) != 0 and getFloat(iSample.resolution) != 0:
                                    quantity = getFloat(iSample.resolution)/getFloat(iSample.scale)
                                    quantity_error = quantity * (getFloat(iSample.resolution_error)/getFloat(iSample.resolution) + getFloat(iSample.scale_error)/getFloat(iSample.scale))
                                 else:
                                    quantity = getFloat(iSample.resolution)
                                    quantity_error = getFloat(iSample.resolution_error)
                           elif item == 'Efficiency':
                              quantity = getFloat(iSample.efficiency)
                              quantity_error = getFloat(iSample.efficiency_error)
                           elif item == 'NoiseRate':
                              quantity = getFloat(iSample.noiseRate)
                              quantity_error = getFloat(iSample.noiseRate_error)
                           elif item == 'Scale':
                              quantity = getFloat(iSample.scale)
                              quantity_error = getFloat(iSample.scale_error)
                           if quantity == 0: quantity = 0.0001
                           
                           fileQte.write('{a} {b} {c} {d} \n'.format(a=iEn, b=iEta, c=quantity, d=quantity_error))

      fileQte.close()


   # second file

   table_pair2 = {}
   if doRefThrs2:   table_pair2 = table_pair_ref
   elif doModThrs2: table_pair2 = table_pair_mod

   
   for item in whichQuantities:
      fileQte = open("files/ratiomaps/{a}_file2.txt".format(a=item), "w+")
      for iEn in EnRanges:
         for iEta in EtaRanges:
            if iEta == '1p44_1p48': continue
            for iSample in samples_binned2[iEn][iEta]:
               if iSample.pfRecHit==getFirstElement(table_pair2[iEta]) and iSample.seeding==getSecondElement(table_pair2[iEta], 'all'):
                           if item == 'Resolution':
                              if do_resoOverScale != 'True':
                                 quantity = getFloat(iSample.resolution)
                                 quantity_error = getFloat(iSample.resolution_error)
                              else:
                                 if getFloat(iSample.scale) != 0 and getFloat(iSample.resolution) != 0:
                                    quantity = getFloat(iSample.resolution)/getFloat(iSample.scale)
                                    quantity_error = quantity * (getFloat(iSample.resolution_error)/getFloat(iSample.resolution) + getFloat(iSample.scale_error)/getFloat(iSample.scale))
                                 else:
                                    quantity = getFloat(iSample.resolution)
                                    quantity_error = getFloat(iSample.resolution_error)
                           elif item == 'Efficiency':
                              quantity = getFloat(iSample.efficiency)
                              quantity_error = getFloat(iSample.efficiency_error)
                           elif item == 'NoiseRate':
                              quantity = getFloat(iSample.noiseRate)
                              quantity_error = getFloat(iSample.noiseRate_error)
                           elif item == 'Scale':
                              quantity = getFloat(iSample.scale)
                              quantity_error = getFloat(iSample.scale_error)
                           if quantity == 0: quantity = 0.0001

                           fileQte.write('{a} {b} {c} {d} \n'.format(a=iEn, b=iEta, c=quantity, d=quantity_error))

      fileQte.close()


   # create ratio file between file1 and file2                        

   for item in whichQuantities:
      filer = open("files/ratiomaps/{a}_ratio.txt".format(a=item), "w+")
      file1 = open("files/ratiomaps/{a}_file1.txt".format(a=item), "r")
      file2 = open("files/ratiomaps/{a}_file2.txt".format(a=item), "r")
      lines1 = file1.readlines()
      lines2 = file2.readlines()

      for line1 in lines1:
         for line2 in lines2:

            index1_a = line1.find(' ')
            index1_b = line1.find(' ', index1_a+1)
            index1_c = line1.find(' ', index1_b+1)
            index1_d = line1.find(' ', index1_c+1)
            ETrange1  = line1[0:index1_a]
            ETArange1 = line1[index1_a+1:index1_b]
            quantity1 = line1[index1_b+1:index1_c]
            error1 = line1[index1_c+1:index1_d]

            index2_a = line2.find(' ')
            index2_b = line2.find(' ', index2_a+1)
            index2_c = line2.find(' ', index2_b+1)
            index2_d = line2.find(' ', index2_c+1)
            ETrange2  = line2[0:index2_a]
            ETArange2 = line2[index2_a+1:index2_b]
            quantity2 = line2[index2_b+1:index2_c]
            error2 = line2[index2_c+1:index2_d]

            if ETrange1 == ETrange2:
               if ETArange1 == ETArange2:
                  if item == 'NoiseRate' and do_absDiffNoise=='True':
                     ratio = (float(quantity1)-float(quantity2))
                     ratio_err = sqrt(float(error1)*float(error1) + float(error2)*float(error2))
                  else:
                     if float(quantity2) != 0:
                        ratio = (float(quantity1)/float(quantity2)-1)*100
                        ratio_err = 100*float(error1)/float(quantity2) + 100*float(quantity1)*float(error2)/(float(quantity2)*float(quantity2)) 
                     else:
                        ratio = 0.0
                     if ratio == 0.0:
                        ratio = 0.0001
                  filer.write('{a} {b} {c} {d} \n'.format(a=ETrange1, b=ETArange1, c=(ratio), d=(ratio_err)))
      filer.close()
   
   # produce the ratio map
   
   # bins where the statistics is too low to be tuned on
   lowStatBins = [['1_5','2p80_3p00'], ['1_5','2p60_2p80'], ['1_5','2p40_2p60'], ['5_10','2p80_3p00'], ['5_10','2p60_2p80'], ['10_15','2p80_3p00']]

   printWithNumber = True #true means plotting pair, false means plotting number of z dimension
   printError = True
 
   # in order not to plot the full energy range
   for iEn in EnRanges[:]:
      if getFloat(getUpperBin(iEn)) > 100:
         EnRanges.remove(iEn)

   # removing the crack bin
   for iEta in EtaRanges[:]:
      if iEta == '1p44_1p48':
         EtaRanges.remove(iEta)

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

   gStyle.SetOptStat(0)
   gStyle.SetPadRightMargin(0.16) 
   
   whichQuantities = ['Resolution', 'Efficiency', 'NoiseRate', 'Scale']
   
   label2 = label
   for item in whichQuantities:
      histo_summary = TH2D('histo_summary_{a}'.format(a=item), 'histo_summary_{a}'.format(a=item), nBins_eta, binsEta, nBins_energy, binsEnergy)
      histo_summary.SetTitle(title)
      
      histo_summary.GetXaxis().SetTitle('#eta')
      histo_summary.GetXaxis().SetLabelSize(0.028)
      histo_summary.GetXaxis().SetTitleSize(0.04)
      histo_summary.GetXaxis().SetTitleOffset(1.2)

      histo_summary.GetYaxis().SetTitle('Energy')
      histo_summary.GetYaxis().SetLabelSize(0.028)
      histo_summary.GetYaxis().SetTitleSize(0.04)
      histo_summary.GetYaxis().SetTitleOffset(1.2)

      if item=='NoiseRate' and do_absDiffNoise=='True':
        label2 = label2.replace('/', '-')
        label3 = 'Abs. diff.' 
        label4 = ''
      else:
        label3 = 'Rel. diff.'  # was 'Ratio'
        label4 = '[%]'
      histo_summary.GetZaxis().SetTitle("{a}({b}) in {c} {d}".format(a=label3, b=label2, c=item, d=label4))
      histo_summary.GetZaxis().SetTitleSize(0.04)
      histo_summary.GetZaxis().SetTitleOffset(1.2)
      if item == 'Resolution':
         histo_summary.GetZaxis().SetRangeUser(-100,60)
      elif item == 'Efficiency':
         histo_summary.GetZaxis().SetRangeUser(-100,10)
      elif item == 'NoiseRate':
         if do_absDiffNoise == 'True':
           histo_summary.GetZaxis().SetRangeUser(-10,10)
         else:
           histo_summary.GetZaxis().SetRangeUser(-100,0.001)
      elif item == 'Scale':
         histo_summary.GetZaxis().SetRangeUser(-10, 0)

      c_summary = TCanvas('c_summary_{a}'.format(a=item), 'c_summary_{a}'.format(a=item), 1500, 1500)

      fileToRead = open("files/ratiomaps/{a}_ratio.txt".format(a=item), "r")
      lineQte = fileToRead.readlines()

      for iEn in EnRanges:
         for iEta in EtaRanges:
            if iEta == '1p44_1p48': continue
            for iLine in lineQte:
                  
                     index1_a = iLine.find(' ')
                     index1_b = iLine.find(' ', index1_a+1)
                     index1_c = iLine.find(' ', index1_b+1)
                     ETrange  = iLine[0:index1_a]
                     ETArange = iLine[index1_a+1:index1_b]
                     if iEn == ETrange and iEta ==ETArange:
                        quantity = float(iLine[index1_b+1:index1_c])
                        histo_summary.Fill(iEta, iEn, quantity)
                        if printError:
                           histo_summary.SetBarOffset(0.3)
         if printWithNumber == True:
            histo_summary.Draw('text' + 'colz')
         else:
            histo_summary.Draw('colz')
     
      #make so that only 1 digit is printed when using text function
      gStyle.SetPaintTextFormat(".1f");

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
            if printError and (iEn == "1_5" or iEn == '5_10' or iEn == '10_15' or iEn == '15_20'):
               y1 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 - (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.75
            else:
               y1 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 - (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.25
            y2 = (getFloat(getUpperBin(iEn)) + getFloat(getLowerBin(iEn)))/2 + (getFloat(getUpperBin(iEn)) - getFloat(getLowerBin(iEn)))*0.25
            score_print = TPaveText(x1, y1, x2, y2)
            if [iEn, iEta] not in lowStatBins:
                  if printError: #we print the error
                     for iLine in lineQte:
                  
                        index1_a = iLine.find(' ')
                        index1_b = iLine.find(' ', index1_a+1)
                        index1_c = iLine.find(' ', index1_b+1)
                        index1_d = iLine.find(' ', index1_c+1)
                        ETrange  = iLine[0:index1_a]
                        ETArange = iLine[index1_a+1:index1_b]
                        if iEn == ETrange and iEta ==ETArange:
                           score_print.AddText('#pm {a}'.format(a=round(float(iLine[index1_c+1:index1_d]), 1)))
                           score_print.SetFillColorAlpha(0, 0)

            else:
               score_print.SetFillColor(1)
               score_print.SetFillStyle(3244)
            score_label.append(score_print)
         
      for label in score_label:
         if printError:
            label.Draw('same')
         label.SetBorderSize(0)
         label.SetTextSize(0.015)
         label.SetTextFont(62)
         label.SetTextAlign(11)
      
      gStyle.SetOptStat(0)
      gStyle.SetPadRightMargin(0.16) 
      
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
      inputFileName = 'noisePlotter/PFRecHitThresholds_EB_ringaveraged_EE_TL{tl}/graphs.root'.format(tl=thrlabel)
      #inputFileName = 'noisePlotter/PFRecHitThresholds_EB_ringaveraged_EE_2021/graphs.root'
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
      #mergedGraph1.Draw('C')

      mergedGraph2.SetLineWidth(4)
      mergedGraph2.SetMarkerSize(1.2)
      mergedGraph2.SetMarkerStyle(8)
      mergedGraph2.SetMarkerColor(1)
      #mergedGraph2.Draw('C')

      mergedGraph3.SetLineWidth(4)
      mergedGraph3.SetMarkerSize(1.2)
      mergedGraph3.SetMarkerStyle(8)
      mergedGraph3.SetMarkerColor(1)
      #mergedGraph3.Draw('C')

      mergedGraph4.SetLineWidth(4)
      mergedGraph4.SetMarkerSize(1.2)
      mergedGraph4.SetMarkerStyle(8)
      mergedGraph4.SetMarkerColor(1)
      #mergedGraph4.Draw('C')
     

      legend4 = TLegend(0.1, 0.7, 0.4, 0.75)
      legend4.AddEntry('merged_smooth_1', 'noise curves {1,..,4}#sigma', 'l')
      legend4.SetTextSize(0.025);
      legend4.SetLineColor(0);
      legend4.SetFillColorAlpha(0,0);
      legend4.SetBorderSize(0);
      #legend4.Draw('same')
     

      if item == 'Resolution': 
         c_summary.SaveAs('{a}/summaryPlot_resolution.png'.format(a=outputdir)) 
      elif item == 'Efficiency':
         c_summary.SaveAs('{a}/summaryPlot_efficiency.png'.format(a=outputdir))
      elif item == 'NoiseRate':
         c_summary.SaveAs('{a}/summaryPlot_noiseRate.png'.format(a=outputdir))
      elif item == 'Scale':
         c_summary.SaveAs('{a}/summaryPlot_scale.png'.format(a=outputdir))







