#!/bin/bash
# run locally: 
# source AnalyserLauncher.sh

# to run on the batch:
# sbatch -p wn --account=cn-test -o logs/analyser.log -e logs/analyser.log --job-name=analyser --ntasks=10 AnalyserLauncher.sh



#----------- USER'S DECISION BOARD --------------//


# Enter the production label of the files that you want to analyse
declare -a FilesArray=(
                        #"photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh0.5_seedRef_V04_v01_n30000"
                        #"photon_E1.0to100GeV_closeEcal_EB_noPU_pfrhRef_seed3.0_V02_v01_n30000"
                        #"photon_E1.0to100GeV_closeEcal_EB_noPU_pfrhRef_seedRef_V03_v01_n30000"
                        #"photon_E0.1to200GeV_closeEcal_EEMerged_noPU_pfrh0.5_seedRef_V04_v03_n30000"
                        #"photon_E0.1to200GeV_closeEcal_EEMerged_noPU_pfrhRef_seed3.0_V02_v03_n30000"
                        #"photon_E0.1to200GeV_closeEcal_EEMerged_noPU_pfrhRef_seedRef_V03_v03_n30000"
                        #"photon_E1.0to100GeV_closeEcal_EB_noPU_pfrhRef_seedRef_thrXtalEBXtalEE_y2023_T2_v1_t0_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed1.0_thrRingEBXtalEE_y2023_T2_v1_t11_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed2.0_thrRingEBXtalEE_y2023_T2_v1_t12_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed3.0_thrRingEBXtalEE_y2023_T2_v1_t13_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh1.0_seed4.0_thrRingEBXtalEE_y2023_T2_v1_t14_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh2.0_seed2.0_thrRingEBXtalEE_y2023_T1_v1_t22_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh2.0_seed3.0_thrRingEBXtalEE_y2023_T2_v1_t23_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh2.0_seed4.0_thrRingEBXtalEE_y2023_T2_v1_t24_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh3.0_seed3.0_thrRingEBXtalEE_y2023_T2_v1_t33_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh3.0_seed4.0_thrRingEBXtalEE_y2023_T2_v1_t34_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh4.0_seed4.0_thrRingEBXtalEE_y2023_T2_v1_t44_n30000"
                        #"photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrhRef_seedRef_thrXtalEBXtalEE_y2023_T1_v2_t0_n30000"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh1.0_seed1.0_thrXtalEBXtalEE_y2023_T2_v2_t11_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh1.0_seed2.0_thrXtalEBXtalEE_y2023_T2_v2_t12_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh1.0_seed3.0_thrXtalEBXtalEE_y2023_T2_v2_t13_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh1.0_seed4.0_thrXtalEBXtalEE_y2023_T2_v2_t14_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh2.0_seed2.0_thrXtalEBXtalEE_y2023_T2_v2_t22_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh2.0_seed3.0_thrXtalEBXtalEE_y2023_T2_v2_t23_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh2.0_seed4.0_thrXtalEBXtalEE_y2023_T2_v2_t24_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh3.0_seed3.0_thrXtalEBXtalEE_y2023_T2_v2_t33_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh3.0_seed4.0_thrXtalEBXtalEE_y2023_T2_v2_t34_n30000_highStat2"
                        "photon_E1.0to200GeV_closeEcal_EEMerged_noPU_pfrh4.0_seed4.0_thrXtalEBXtalEE_y2023_T2_v2_t44_n30000_highStat2"
                       )


do_writeFiles=false

doMatching_simFraction=true
doMatching_deltaR=false



do_resolutionPlot='False'
do_scalePlot='False'
do_efficiencyPlot='False'
do_noiseRatePlot='False'
do_rankingPlot='False'
do_summaryPlot='True'
do_decisionPlot='False'

do_resoOverScale='True'
do_popUpPlot='False'

# -----  2. Advanced parameters ----- #

# Here you may want to leave the default parameters

# This option turned to true means Etrue=caloParticle_simEnergy. Otherwise, Etrue=caloParticle_energy 
use_simEnergy=true

# Choose whether you want to bin in ET or energy
do_binningEt=false

# Choose whether to use a finner binning or not
do_fineBinning_energy=true
do_fineBinning_eta=true

# This function turned to true fits the peak only, otherwise fits the whole distribution
do_fitPeak=true

# Choose one of the following fit (Crystal Ball, double-sided Crystal Ball or Bifurcated Gaussian)
do_CBfit=false
do_doubleCBfit=true
do_BGfit=false

#######################################################################

echo "Start" 

if [ "$doMatching_simFraction" = true ] ; then
   matchingMethod="simFraction"
fi
if [ "$doMatching_deltaR" = true ] ; then
   matchingMethod="deltaR"
fi


for iFile in ${FilesArray[@]}; do
   fileNameforPlotter="histo_"$iFile
   if [ "$doMatching_simFraction" = true ] ; then
      fileNameforPlotter=$fileNameforPlotter"_simFraction"
   fi
   if [ "$doMatching_deltaR" = true ] ; then
      fileNameforPlotter=$fileNameforPlotter"_deltaR"
   fi
   echo $fileNameforPlotter
   echo $fileNameforPlotter >> fileSamples.txt
done


if [ "$do_writeFiles" = true ] ; then
   if [ "$do_popUpPlot" = 'False' ] ; then     
      root -l -q -b "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", "false", "false", "false", "false", "false", "false", "false", "false", \"$do_writeFiles\")"
   fi
   if [ "$do_popUpPlot" = 'True' ] ; then
      root -l -q "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", "false", "false", "false", "false", "false", "false", "false", "false", \"$do_writeFiles\")"
   fi 


fi

python 2DPlotProducer.py --doResolutionPlot=$do_resolutionPlot --doScalePlot=$do_scalePlot --doEfficiencyPlot=$do_efficiencyPlot --doNoiseRatePlot=$do_noiseRatePlot --doRankingPlot=$do_rankingPlot --doSummaryPlot=$do_summaryPlot --doDecisionPlot=$do_decisionPlot --doPopUpPlot=$do_popUpPlot --doResoOverScale=$do_resoOverScale


rm fileSamples.txt
 
echo "Done"





