##Installation 
- go in your favorite CMSSW release repository, e.g CMSSW_10_0_1
- cd CMSSW_10_0_1/src
- cmsenv
- git clone git@github.com:pfclustering/ECALPFClusterAnalysis.git


## Analyzer

To be run on top of a dumped file

```
cd ECALPFClusterAnalysis/Analyzer
```

Choose the input and output file names in start_PFClusterAnalyzer.C and run as 

```
root -l start_PFClusterAnalyzer.C+
```


## Plotter

```
cd ECALPFClusterAnalysis/Plotter
```

To plot histograms produced by Analyzer do for instance

```
python plotter.py -f histo_singlePhoton_5k_EB --doClusterAnalysis
```

where -f is the parsed fileName (produced by the analyzer) that you have to choose.

To perform the fit of E/Etrue and produce the resolution/scale/efficiency plots, open EoverEtrue_fit.C, adjust the parameters in the User's decision board to your own convenience, and run as

```
root -l EoverEtrue_fit.C+
```





