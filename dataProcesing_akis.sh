#!/bin/bash
#
# This script all the binary file of a run in root by mean
# of converter_solaris_tracker.C and merge all the root file in a single root file
# by mean of merger.C
# requires as input the run number and the correct path of the input
# and output directories
#
#  created 25-11-2022  D. Torresi
# 
#  update:  	 added the sic onverter 2024-10 D. Torresi  
#  last update:  added the tack generator 2024-11  D. Torresi
#----------------------------------------------------------------------------------
#
#  Remember to check that all the dig variables are correct!
#
#


# Definizione variabili
#run number
run=$1

#################################################
# directories
#bindir=/home/torresi/Analisi/NUMEN/Analysis_tracker_v1.1/Raw_data/run_$run
#caldir=/home/torresi/Analisi/NUMEN/Analysis_tracker_v1.1/Cal_data/run_$run
#merdir=/home/torresi/Analisi/NUMEN/Analysis_tracker_v1.1/Merged_data/run_$run

bindir=/home/numen/solaris/RAW_data/tracker_and_sic/
caldir=~/Analysis_tracker_v1.0/Cal_data/run_$run
merdir=~/Analysis_tracker_v1.0/Merged_data/run_$run



#################################################
# Merging data			3rd level
#################################################
echo  -e "\033[40;35;1m Merging files \033[0m"  

merfile=$merdir/merged.root

initfile=$caldir/dig_22642_cal.root    # very first file to be merged
file1=a.root			        # first argument of macro
file2=b.root			        # second argument of macro
outfile=c.root				# macro output

cp $initfile $file1

echo "---------"

for dig in 22643 22644 22645 21247
   do
   # fill the $file2 
   cp $caldir/dig_$dig\_cal.root $file2
   echo "file 1: " $file1
   echo "file 2: " $file2

   root -q -l "merger.C(\"$file1\",\"$file2\", \"$outfile\")"
   # make the output the first argument of the next step
   cp $outfile $file1
   
done

# remove unecesary files
rm $file1 $file2				# file usde for the merging
#for dig in 22642 22643 22644 22645 21247
#rm $caldir/dig_$dig\_cal.root			# calibrated file that are used only for the merging

# put the final output in the correct directory
mv $outfile  $merdir/merg_$run.root
