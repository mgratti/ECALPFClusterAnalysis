#!/bin/bash
# run locally: 
# source AnalyserLauncher.sh

# to run on the batch:
# sbatch -p wn --account=cn-test -o logs/analyser.log -e logs/analyser.log --job-name=analyser --ntasks=10 AnalyserLauncher.sh



#----------- USER'S DECISION BOARD --------------//

# What do you want to do? 
doAnalyser=false
doPlotter=true

# Enter the production label of the files that you want to analyse
declare -a FilesArray=(
                        #"photon_E0.1to200GeV_closeEcal_EEMerged_noPU_pfrh0.5_seedRef_V04_v03_n30000"
                        #"photon_E0.1to200GeV_closeEcal_EEMerged_noPU_pfrhRef_seed3.0_V02_v03_n30000"
                        #"photon_E0.1to200GeV_closeEcal_EEMerged_noPU_pfrhRef_seedRef_V03_v03_n30000"
                        #"photon_E0.1to200GeV_closeEcal_EEMerged_noPU_pfrhRef_seedRef_thrRing_V05_v03_n30000"
                        #"photon_E1.0to100GeV_closeEcal_EB_noPU_pfrh0.5_seedRef_V04_v01_n30000"
                        #"photon_E1.0to100GeV_closeEcal_EB_noPU_pfrhRef_seed3.0_V02_v01_n30000"
                        #"photon_E1.0to100GeV_closeEcal_EB_noPU_pfrhRef_seedRef_V03_v01_n30000"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrhRef_seedRef_V03_v01_n30000_simFraction"
                        "photon_E1.0to100GeV_closeEcal_EB_noPU_pfrhRef_seedRef_V03_v01_n30000_deltaR"
                        #"photon_E1.0to100GeV_closeEcal_EB_noPU_pfrhRef_seedRef_thrRing_V05_v01_n30000"
                      )

# Enter the location of the dumped files
inDirectory="/work/anlyon/dumpedFiles/"
#inDirectory="/t3home/anlyon/CMSSW_10_6_1_patch1/src/RecoSimStudies/Dumpers/test/outputfiles/dumpedFiles/"
# Choose one of the following matching strategies
doMatching_numberOfHits=false
doMatching_simFraction=false
doMatching_deltaR=true



######################
# Plotter parameters
######################

# ----- 1.  What do you want to plot? ----- #
do_resolutionPlot=true
do_scalePlot=true
do_efficiencyPlot=true

# Plotting the scale and resolution launches the fitting procedure. 
# If you want to avoid that and only get the efficiency plot, turn this option to true
do_efficiencyPlotOnly=true

# If you want to produce the ratio plot between the two first files that you inserted WITH SAME MATCHING, turn this option to true
do_ratioPlot=true

# If you want to produce the ratio plot between the two first files that you inserted WITH DIFFERENT MATCHING, turn this option to true
do_useDifMatching=true

# If you want to compare to the ref matching (deltaR)
#do_compareToRef=false

# choose whether to use a finner binning or not
do_fineBinning_energy=true
do_fineBinning_eta=true

# do you want to enable pop-up plots?
do_popUpPlot=false



# -----  2. Advanced parameters ----- #

# Here you may want to leave the default parameters

# This option turned to true means Etrue=caloParticle_simEnergy. Otherwise, Etrue=caloParticle_energy 
use_simEnergy=true

# choose whether you want to bin in ET or energy
do_binningEt=false

# This function turned to true fits the peak only, otherwise fits the whole distribution
do_fitPeak=false

# choose one of the following fit (Crystal Ball, double-sided Crystal Ball or Bifurcated Gaussian)
do_CBfit=false
do_doubleCBfit=true
do_BGfit=false

#######################################################################


#if the user chooses more than one matching method, pick up the default value
if [ "$doMatching_numberOfHits" = true ] ; then
   if [ "$doMatching_simFraction" = true ] || [ "$doMatching_deltaR" = true ] ; then
      doMatching_numberOfHits=false
      doMatching_simFraction=true
      doMatching_deltaR=false
   fi
fi
if [ "$doMatching_simFraction" = true ] ; then
   if [ "$doMatching_numberOfHits" = true ] || [ "$doMatching_deltaR" = true ] ; then
      doMatching_numberOfHits=false
      doMatching_simFraction=true
      doMatching_deltaR=false
   fi
fi
if [ "$doMatching_deltaR" = true ] ; then
   if [ "$doMatching_numberOfHits" = true ] || [ "$doMatching_simFraction" = true ] ; then
      doMatching_numberOfHits=false
      doMatching_simFraction=true
      doMatching_deltaR=false
   fi
fi

echo "Start" 

if [ "$doMatching_numberOfHits" = true ] ; then
   matchingMethod="numberOfHits"
fi
if [ "$doMatching_simFraction" = true ] ; then
   matchingMethod="simFraction"
fi
if [ "$doMatching_deltaR" = true ] ; then
   matchingMethod="deltaR"
fi


for iFile in ${FilesArray[@]}; do
   if [ "$doAnalyser" = true ] ; then
      echo "#############################################################################################"
      echo "# " 
      echo "   Running Analyser on"
      echo "   "$iFile 
      echo "   with matching" $matchingMethod 
      echo "# " 
      echo "#############################################################################################"
      root -l -q "start_PFClusterAnalyzer.C+(\"$iFile\", \"$inDirectory\", \"$matchingMethod\")"
   fi
   if [ "$doPlotter" = true ] && [ "$do_ratioPlot" = false ] ; then
      fileNameforPlotter="histo_"$iFile
      if [ "$doMatching_numberOfHits" = true ] ; then
         fileNameforPlotter=$fileNameforPlotter"_numberOfHits"
      fi
      if [ "$doMatching_simFraction" = true ] ; then
         fileNameforPlotter=$fileNameforPlotter"_simFraction"
      fi
      if [ "$doMatching_deltaR" = true ] ; then
         fileNameforPlotter=$fileNameforPlotter"_deltaR"
      fi
      echo $fileNameforPlotter >> file.txt
      echo "#############################################################################################"
      echo "# " 
      echo "   Running Plotter on"
      echo "   "$fileNameforPlotter 
      echo "# " 
      echo "#############################################################################################"
      
      if [ "$do_popUpPlot" = false ] ; then       
         root -l -q -b "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", \"$do_resolutionPlot\", \"$do_scalePlot\", \"$do_efficiencyPlot\", \"$do_efficiencyPlotOnly\", \"$do_ratioPlot\")"
      fi 
      if [ "$do_popUpPlot" = true ] ; then
         root -l -q "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", \"$do_resolutionPlot\", \"$do_scalePlot\", \"$do_efficiencyPlot\", \"$do_efficiencyPlotOnly\", \"$do_ratioPlot\")"
      fi 
      rm file.txt
   fi
 
done

if [ "$doPlotter" = true ] && [ "$do_ratioPlot" = true ] ; then
   for iFile in ${FilesArray[@]}; do
      fileNameforPlotter="histo_"$iFile
      if [ "$do_useDifMatching" = false ] ; then
         if [ "$doMatching_numberOfHits" = true ] ; then
            fileNameforPlotter=$fileNameforPlotter"_numberOfHits"
         fi
         if [ "$doMatching_simFraction" = true ] ; then
            fileNameforPlotter=$fileNameforPlotter"_simFraction"
         fi
         if [ "$doMatching_deltaR" = true ] ; then
            fileNameforPlotter=$fileNameforPlotter"_deltaR"
         fi
      fi
      echo $fileNameforPlotter >> file.txt
   done

   if [ "$do_popUpPlot" = false ] ; then     
      root -l -q -b "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", \"$do_resolutionPlot\", \"$do_scalePlot\", \"$do_efficiencyPlot\", \"$do_efficiencyPlotOnly\", \"$do_ratioPlot\")"
   fi
   if [ "$do_popUpPlot" = true ] ; then
      root -l -q "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", \"$do_resolutionPlot\", \"$do_scalePlot\", \"$do_efficiencyPlot\", \"$do_efficiencyPlotOnly\", \"$do_ratioPlot\")"
   fi 

   rm file.txt
 
fi

#dir="../Plotter/myPlots/fits/histo_photon_E0.1to200GeV_closeEcal_EE_noPU_pfrh0.5_seedRef_V04_vMerged_n30000_simFraction_EtaEnBinned_doubleCB_simEnergy/efficiency_vs_energy.png"

#tar -czvf ${dir}.tar.gz ${dir}

#scp ${dir}.tar.gz anlyon@lxplus.cern.ch:/eos/user/a/anlyon/www/ECAL_DPG/

echo "Done"

#implement security things, like for do_ratioPlot there are at least two files




