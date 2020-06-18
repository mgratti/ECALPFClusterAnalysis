#!/bin/bash
# run locally: 
# source AnalyserLauncher.sh

# to run on the batch:
# sbatch -p wn --account=t3 -o logs/analyser.log -e logs/analyser.log --job-name=analyser --ntasks=10 AnalyserLauncher.sh



#----------- USER'S DECISION BOARD --------------//

user="anlyon"

# What do you want to do? 
doAnalyser=true
doPlotter=false


# Enter the production label of the files that you want to analyse
declare -a FilesArray=(
                        #"photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi150_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T12_v1_t33_n30000_njd0"
                        #"photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi150_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T12_v1_t44_n30000_njd0"
                        #"photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi235_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T13_v1_t33_n30000_njd0"
                        #"photon_E1to100GeV_closeEcal_EB_noPU_thrsLumi235_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T13_v1_t44_n30000_njd0"
                        #"photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi150_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T12_v2_t33_n30000_njd0"
                        #"photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi150_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T12_v2_t44_n30000_njd0"
                        #"photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi235_pfrh3.0_seed3.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T13_v2_t33_n30000_njd0"
                        #"photon_E1to200GeV_closeEcal_EEMerged_noPU_thrsLumi235_pfrh4.0_seed4.0_noMargin_thrRingEBXtalEE_shs1.0_maxd10.0_y2021_T13_v2_t44_n30000_njd0"
                        "photon_E1to100GeV_closeEcal_EEclose_noPU_thrsLumi2021_pfrhRef_seedRef_wMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_l180_newDumper_n50"
                        #"photon_E1to100GeV_closeEcal_EEclose_noPU_thrsLumi2021_pfrhRef_seedRef_wMargin_thrXtalEBXtalEE_shs1.0_maxd10.0_l180_oldDumper_n50_njd0"
                        )


# Enter the location of the dumped files
inDirectory="/work/anlyon/dumpedFiles/"
# Choose one of the following matching strategies
doMatching_simFraction=false
doMatching_simFraction_withHF=true
doMatching_deltaR=false



######################
# Plotter parameters
######################

do_writeFiles=true

# ----- 1.  What do you want to plot? ----- #
do_resolutionPlot=false
do_scalePlot=false
do_efficiencyPlot=false
do_fakeRatePlot=false

# Plotting the scale and resolution launches the fitting procedure. 
# If you want to avoid that and only get the efficiency and fakeRate plot, turn this option to true
do_efficiencyPlotOnly=false

# If you want to produce the ratio plot between the two first files that you inserted WITH SAME MATCHING, turn this option to true
do_ratioPlot=false

# If you want to produce the ratio plot between the two first files that you inserted WITH DIFFERENT MATCHING, turn this option to true. Requires do_ratioPlot to be set to true
do_useDifMatching=false

# If you want to scan over the different thresholds, turn this option to true
do_scanThrs=false

# do you want to enable pop-up plots?
do_popUpPlot=false



# -----  2. Advanced parameters ----- #

# Here you may want to leave the default parameters

# This option turned to true means Etrue=caloParticle_simEnergy. Otherwise, Etrue=caloParticle_energy 
use_simEnergy=true

# Choose whether you want to bin in ET or energy
do_binningEt=false

# choose whether to use a finner binning or not
do_fineBinning_energy=true
do_fineBinning_eta=true


# This function turned to true fits the peak only, otherwise fits the whole distribution
do_fitPeak=true

# Choose whether setting the Yranges by hand or set it to automatic
do_autoScale=true

# Choose one of the following fit (Crystal Ball, double-sided Crystal Ball or Bifurcated Gaussian)
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
   matchingMethod="simFraction"
fi
if [ "$doMatching_simFraction" = true ] ; then
   matchingMethod="simFraction"
fi
if [ "$doMatching_simFraction_withHF" = true ] ; then
   matchingMethod="simFraction_withHF"
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
   if [ "$doPlotter" = true ] && [ "$do_ratioPlot" = false ] && [ "$do_scanThrs" = false ] ; then
      fileNameforPlotter="histo_"$iFile
      if [ "$doMatching_numberOfHits" = true ] ; then
         fileNameforPlotter=$fileNameforPlotter"_numberOfHits"
      fi
      if [ "$doMatching_simFraction" = true ] ; then
         fileNameforPlotter=$fileNameforPlotter"_simFraction"
      fi
      if [ "$doMatching_simFraction_withHF" = true ] ; then
         fileNameforPlotter=$fileNameforPlotter"_simFraction_wHF"
      fi
      if [ "$doMatching_deltaR" = true ] ; then
         fileNameforPlotter=$fileNameforPlotter"_deltaR"
      fi
      echo $fileNameforPlotter >> fileSamples.txt
      echo "#############################################################################################"
      echo "# " 
      echo "   Running Plotter on"
      echo "   "$fileNameforPlotter 
      echo "# " 
      echo "#############################################################################################"
      
      if [ "$do_popUpPlot" = false ] ; then       
         root -l -q -b "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", \"$do_resolutionPlot\", \"$do_scalePlot\", \"$do_efficiencyPlot\", \"$do_fakeRatePlot\", \"$do_efficiencyPlotOnly\", \"$do_ratioPlot\", \"$do_scanThrs\", \"$do_autoScale\", \"$do_writeFiles\", \"$user\")"
      fi 
      if [ "$do_popUpPlot" = true ] ; then
         root -l -q "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", \"$do_resolutionPlot\", \"$do_scalePlot\", \"$do_efficiencyPlot\", \"$do_fakeRatePlot\", \"$do_efficiencyPlotOnly\", \"$do_ratioPlot\", \"$do_scanThrs\", \"$do_autoScale\", \"$do_writeFiles\", \"$user\")"
      fi 
      rm fileSamples.txt
   fi
 
done

if [ "$doPlotter" = true ] && ([ "$do_ratioPlot" = true ] || [ "$do_scanThrs" = true ]) ; then
   for iFile in ${FilesArray[@]}; do
      fileNameforPlotter="histo_"$iFile
      if [ "$do_useDifMatching" = false ] ; then
         if [ "$doMatching_numberOfHits" = true ] ; then
            fileNameforPlotter=$fileNameforPlotter"_numberOfHits"
         fi
         if [ "$doMatching_simFraction" = true ] ; then
            fileNameforPlotter=$fileNameforPlotter"_simFraction"
         fi
         if [ "$doMatching_simFraction_withHF" = true ] ; then
            fileNameforPlotter=$fileNameforPlotter"_simFraction_wHF"
         fi
         if [ "$doMatching_deltaR" = true ] ; then
            fileNameforPlotter=$fileNameforPlotter"_deltaR"
         fi
      fi
      echo $fileNameforPlotter >> fileSamples.txt
   done

   if [ "$do_popUpPlot" = false ] ; then     
      root -l -q -b "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", \"$do_resolutionPlot\", \"$do_scalePlot\", \"$do_efficiencyPlot\", \"$do_fakeRatePlot\", \"$do_efficiencyPlotOnly\", \"$do_ratioPlot\", \"$do_scanThrs\", \"$do_autoScale\", \"$do_writeFiles\", \"$user\")"
   fi
   if [ "$do_popUpPlot" = true ] ; then
      root -l -q "../Plotter/EoverEtrue_fit.C+(\"$do_fineBinning_energy\", \"$do_fineBinning_eta\", \"$use_simEnergy\", \"$do_binningEt\", \"$do_CBfit\", \"$do_doubleCBfit\", \"$do_BGfit\", \"$do_fitPeak\", \"$do_resolutionPlot\", \"$do_scalePlot\", \"$do_efficiencyPlot\", \"$do_fakeRatePlot\", \"$do_efficiencyPlotOnly\", \"$do_ratioPlot\", \"$do_scanThrs\", \"$do_autoScale\", \"$do_writeFiles\", \"$user\")"
   fi 

   rm fileSamples.txt
 
fi

#dir="../Plotter/myPlots/fits/histo_photon_E0.1to200GeV_closeEcal_EE_noPU_pfrh0.5_seedRef_V04_vMerged_n30000_simFraction_EtaEnBinned_doubleCB_simEnergy/efficiency_vs_energy.png"

#tar -czvf ${dir}.tar.gz ${dir}

#scp ${dir}.tar.gz anlyon@lxplus.cern.ch:/eos/user/a/anlyon/www/ECAL_DPG/

echo "Done"

#implement security things, like for do_ratioPlot there are at least two files




