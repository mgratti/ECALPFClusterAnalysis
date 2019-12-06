#usage:
# python EEmerger.py --f1 file1.root --f2 file2.root

import sys 
import os
import glob
import ROOT

def getOptions():
   from argparse import ArgumentParser
   parser = ArgumentParser(description='File to merge EEclose and EEfar root ntuples', add_help=True)
   parser.add_argument('--f1', type=str, dest='f1', help='first filename', default='test.root')
   parser.add_argument('--f2', type=str, dest='f2', help='second filename', default='test.root')
   return parser.parse_args()



if __name__ == "__main__":

   opt = getOptions()

   file1 = opt.f1
   file2 = opt.f2

   if((file1.find('EEclose')==-1 and file2.find('EEclose')==-1) or (file1.find('EEfar')==-1 and file2.find('EEfar')==-1)):
      print "You are not merging a EEclose with a EEfar file!"
      print "Aborting"
      sys.exit(0)

   if((file1.find('noPU')==-1 and file2.find('noPU')!=-1) or (file1.find('wPU')!=-1 and file2.find('wPU')==-1)):
      print "You are trying to merge a wPU file with a noPU file!"
      print "Aborting"
      sys.exit(0)


   indexSeedinf1 = file1.find('seed')
   indexSeedsup1 = file1.find('_', indexSeedinf1)
   indexSeedinf2 = file2.find('seed')
   indexSeedsup2 = file2.find('_', indexSeedinf1)

   seedThrs1 = file1[indexSeedinf1:indexSeedsup1]
   seedThrs2 = file2[indexSeedinf2:indexSeedsup2]
  
   if(seedThrs1!=seedThrs2):
      print "You are trying to merge files with different seeding thresholds!"
      print "Aborting"
      sys.exit(0)

   indexPFRecHitinf1 = file1.find('pfrh')
   indexPFRecHitsup1 = file1.find('_', indexPFRecHitinf1)
   indexPFRecHitinf2 = file2.find('pfrh')
   indexPFRecHitsup2 = file2.find('_', indexPFRecHitinf1)

   PFRecHitThrs1 = file1[indexPFRecHitinf1:indexPFRecHitsup1]
   PFRecHitThrs2 = file2[indexPFRecHitinf2:indexPFRecHitsup2]
 
   if(PFRecHitThrs1!=PFRecHitThrs2):
      print "You are trying to merge files with different PFRecHit thresholds!"
      print "Aborting"
      sys.exit(0)

   if(file1.find('EEclose')!=-1):   
      outputfile = file1.replace('EEclose', 'EEMerged')
   elif(file1.find('EEfar')!=-1):
      outputfile = file1.replace('EEfar', 'EEMerged')
   
   print "Going to merge files {file1} \n and {file2} \n into file {output}".format(file1=file1, file2=file2, output=outputfile)
   os.system('hadd {output} {file1} {file2}'.format(output=outputfile, file1=opt.f1, file2=opt.f2))
   print "Done"
