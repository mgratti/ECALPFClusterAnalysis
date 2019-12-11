## Installation 
- go in your favorite CMSSW release repository, e.g CMSSW_10_0_1
- cd CMSSW_10_0_1/src
- cmsenv
- git clone git@github.com:pfclustering/ECALPFClusterAnalysis.git


## Merge EEclose and EEfar
```
python EEmerger.py --f1 file_name_1.root --f2 file_name_2.root
```

## AnalyserLauncher

Both the Analyser and the Plotter are monitored in AnalyserLauncher.sh 

All the options are to be set directly inside the file, in the User's decision board.


### Utilisation

Run locally

```
source AnalyserLauncher.sh
```

or on the batch

```
sbatch -p wn --account=cn-test -o logs/analyser.log -e logs/analyser.log --job-name=analyser --ntasks=10 AnalyserLauncher.sh
```

### Options

- Choose what you want to do with `doAnalyser` and `doPlotter`. Both can be set to true at the same time
- Enter the dumped files that you want to work on
- Enter the directory where those files are stored
- Choose which matching strategy to use (choose one at a time)

If you want to run the Plotter:
- `do_resolutionPlot`, `do_scalePlot`, `do_efficiencyPlot`, `do_fakeRatePlot`: more than one option can be turn to true
- `do_efficiencyPlotOnly`: the previous options will run the fit, which can be time consuming. If you only want to see the efficiency plot (which doesn't need the fit), turn this option to True. No need to turn to False the previous options. 
NB: if `do_fakeRatePlot`is also set to True, then the fakeRate plots are also going to be produced (they don't rely on the fit neither).
- `do_ratioPlot`: makes the ratio between two files
- `do_useDifMatching`: (requires `do_ratioPlot=true`) makes the ratio between samples with different matching. In this case, write the inputfilename with the matching name appended.
- `do_scanThrs`: produces the plots looping on all the fileNames, at fixed energy or eta 
- `do_popUpPlot`: do you want to see the plot while the code is running? 

### Validator

Before launching a new production with significant changes, it is worth to check the basic distributions compared to previous productions.
This tool will produce comparison plots of basic distrbucions, starting from two outputs of the Analyzer.

Example of usage:
```
cd Validation
python validate.py -f1 /work/mratti/validation/histo_photon_E1to100GeV_closeEcal_EEclose_noPU_pfrhRef_seedRef_thrXtalEBXtalEE_y2021_new2021_n1000_simFraction_EE.root -f2 /work/mratti/validation/histo_photon_E1to100GeV_closeEcal_EEclose_noPU_pfrhRef_seedRef_thrXtalEBXtalEE_y2023_new2023_n1000_simFraction_EE.root -l1 new_2021 -l2 new_2023 -o 2021_vs_2023 --doNorm --doLog --rebin 2
```




