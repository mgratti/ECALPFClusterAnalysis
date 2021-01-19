## Installation 
- go in your favorite CMSSW release repository, e.g CMSSW_10_6_1_patch1
- cd CMSSW_10_6_1_patch1/src
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


## Tuning
Add files that you want to study and which plots to produce in TuningLauncher.sh and do
```
source TuningLauncher.sh
```

### Get ratio maps
Write filenames of samples that you wish to compare, and do
```
python doRatioMaps.py
```

### Validator

Before launching a new production with significant changes, it is worth to check the basic distributions compared to previous productions.
This tool will produce comparison plots of basic distrbucions, starting from two outputs of the Analyzer.

Example of usage:
```
cd Validator
python validate.py -f1 /work/mratti/validation/histo_photon_E1to100GeV_closeEcal_EEclose_noPU_pfrhRef_seedRef_thrXtalEBXtalEE_y2021_new2021_n1000_simFraction_EE.root -f2 /work/mratti/validation/histo_photon_E1to100GeV_closeEcal_EEclose_noPU_pfrhRef_seedRef_thrXtalEBXtalEE_y2023_new2023_n1000_simFraction_EE.root -l1 new_2021 -l2 new_2023 -o 2021_vs_2023 --doNorm --doLog --rebin 2
```

### NoisePlotter
Multiple tasks are handled by this sub-package:
* determine ring-averages starting from xtal-dep thresholds
* write in txt files values of proposed thresholds to be sent to conveners for tag creation
* plot noise curves

##### 1) Dump the thresholds
```
cd $CMSSW_BASE/src/
```
Follow instructions https://github.com/ferriff/usercode#dumper-setup to setup the ```conddb_dumper```.

Start from existing threshold tags produced with x-tal by x-tal thresholds, e.g. ```EcalPFRecHitThresholds_TL235_mixed``` ( [link to conddb entry](https://cms-conddb.cern.ch/cmsDbBrowser/list/Prod/tags/EcalPFRecHitThresholds_TL235_mixed) )

Set the list of tags to dump:
```
LUMIS="TL150 TL235 TL315 TL400 TL450 TL550"
```

Use the ```conddb_dumper```
```
for i in $LUMIS
do
  echo $i
  conddb_dumper -O EcalPFRecHitThresholds -t EcalPFRecHitThresholds_${i}_mixed
  head -61200 dump_EcalPFRecHitThresholds__since_00000001_till_18446744073709551615.dat > PFRecHitThresholds_EB_${i}.txt
  tail -14648 dump_EcalPFRecHitThresholds__since_00000001_till_18446744073709551615.dat > PFRecHitThresholds_EE_${i}.txt
  rm dump_EcalPFRecHitThresholds__since_00000001_till_18446744073709551615.dat
done
```
Copy the just obtained txt files for usage in the sample production:
```
cp PFRecHitThresholds_E*.txt $CMSSW_BASE/src/RecoSimStudies/Dumpers/data/noise/
```

Note that for TL180, the name of the original tag has a different convention: ```EcalPFRecHitThresholds_UL_2018_2e3sig```, so for this one the dumping has to be performed separately.


##### 2) Perform the ring average and outlier removal for EB
For EB only, perform the average over the ring; the outliers, aka crystals more than 5 sigma away from the average, keep their own threshold.
The file for the ring average will be created, copied to $CMSSW_BASE/src/RecoSimStudies/Dumpers/data/noise/ and thus made available for use in the sample production.

```
cd $CMSSW_BASE/src/ECALPFClusterAnalysis/Plotter/noisePlotter/

for i in $LUMIS
do
  echo "*********************************"
  echo $i
  cd getEBRingAverage
  cp $CMSSW_BASE/src/RecoSimStudies/Dumpers/data/noise/PFRecHitThresholds_EB_$i.txt .
  root -l -b -q plotObject_untouched_ringaverage.cxx\(\"PFRecHitThresholds_EB_$i.txt\"\)
  root -l -b -q root2txt.C
  cp productCleaned.txt $CMSSW_BASE/src/RecoSimStudies/Dumpers/data/noise/PFRecHitThresholds_EB_ringaveraged_$i.txt
  cat $CMSSW_BASE/src/RecoSimStudies/Dumpers/data/noise/PFRecHitThresholds_EB_ringaveraged_$i.txt $CMSSW_BASE/src/RecoSimStudies/Dumpers/data/noise/PFRecHitThresholds_EE_$i.txt > ../PFRecHitThresholds_EB_ringaveraged_EE_$i.txt
  cd ..
  #
done
```

##### 3) Create new threshold files with new sigma multipliers
Now we can create new txt files with different noise multipliers which will be sent to conveners and used to create new PFRecHit db records.
This step also create graphs with ring averages performed for plotting purposes.
Keep in mind that for EB we will always assume that the ring average has been performed with the correct outlier treatment, while for EE not.

If needed, edit ```plotNoiseAverage.cxx``` to change the noise multipliers in getPropThrValue(), then run it:
```
for i in $LUMIS
do
  root -l -b -q plotNoiseAverage.cxx\(\"PFRecHitThresholds_EB_ringaveraged_EE_${i}.txt\"\)
done
```
Output will be a directory of the form PFRecHitThresholds_EB_ringaveraged_EE_${i}, containing:
  * ```proposedThresholds_34sigma.txt``` => file for thresholds
  * ```graphs.root``` => containing ROOT TGraphs with ring averages separately for sub-detectors

##### 4) Plot the noise graphs
Then smooth the noise graph and merge EB and EE in a single one.
```
for i in $LUMIS
do
  python getMergedGraph.py -t PFRecHitThresholds_EB_ringaveraged_EE_${i}
done
```
Output is in ```PFRecHitThresholds_EB_ringaveraged_EE_${i}/mergedGraphs.root```
Graphs to be used are ```merged_smooth_i``` where i is the multiplier of the sigma noise, to be drawn with "AL" options.

### SingleEventAnalyzer
Used to analyze and plot PFClusters in a single event.

Will produce histograms for all events chosen and for each pfcluster in the event, with x-tal level information on the energy of the rechits, fractions, best-matched caloparticle, energy of the cluster, etc.

* Choose an event number to check and add it in ```eventsToCheck``` in ```SingleEventPFClusterAnalyzer.C```; 
* note the convention on the event number is: evt=eventId-1
* define input and output in ```start_SingleEventPFClusterAnalyzer.C```
* run with ```root -l -b -q start_SingleEventPFClusterAnalyzer.C+```

Do the plotting:

* specify the events and clusters of your choice in ```eventPlotter.py```
* run with ```python eventPlotter.py```


### SCAnalyzer
To study the shower shapes in g+jets EM enriched samples.





