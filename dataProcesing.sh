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
#  update:  added the sic onverter        2024-10 D. Torresi  
#  last update:  added the tack generator 2024-11  D. Torresi
#----------------------------------------------------------------------------------
#
#  Remember to check that all the dig variables are correct!
#
#



# Definizione variabili
#run number
<<<<<<< HEAD
run=$1
=======
run=151
>>>>>>> ac114bb308b3a74481c5f2d2883e9a35214595c6

#################################################
# directories
#bindir=/home/torresi/Analisi/NUMEN/Analysis_tracker_v1.1/Raw_data/run_$run
#caldir=/home/torresi/Analisi/NUMEN/Analysis_tracker_v1.1/Cal_data/run_$run
#merdir=/home/torresi/Analisi/NUMEN/Analysis_tracker_v1.1/Merged_data/run_$run

bindir=/home/numen/solaris/RAW_data/tracker_and_sic/
caldir=~/Analysis_tracker_v1.0/Cal_data/run_$run
merdir=~/Analysis_tracker_v1.0/Merged_data/run_$run


#################################################
#  Converting binary to root  	2nd level
################################################

# If the output directory does not exist create it
mkdir  $caldir
mkdir  $merdir
echo " binary directory: "$bindir
echo " calib  directory: "$caldir
echo " merged directory: "$merdir

# lopp on the 5 digitizers
#for dig in 22642 22643 22644 22645 22646

echo  -e "\033[40;31;1m Converting tracker files \033[0m"  

cont=1
for dig in 22642 22643 22644 22645 21247
do   
   infile=$bindir/tracker_and_sic_$run\_0$cont\_$dig\_000.sol
   outfile=$caldir/dig_$dig\_cal.root
   cont=$(($cont+1))
   
   echo " Input file:  "$infile
   echo " Output file: "$outfile
   # convert data
   root -q -l "converter_solaris_tracker.C(\"$infile\",$dig,\"$outfile\")"

done


#################################################
# Converting SiC file		2th level
#################################################
echo  -e "\033[44;37m "  
echo  -e "Converting SiC file "
echo  -e "\033[0m"  
cont=0
dig=25716

infile=$bindir/tracker_and_sic_$run\_0$cont\_$dig\_000.sol
outfile=$merdir/sic_$run.root


root -q -l "converter_solaris_sic.C(\"$infile\",$dig,\"$outfile\")"


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




#################################################
# Producing tracking data	4th level
#################################################
echo  -e "\033[40;35;1m Generating tracks \033[0m"  

root -q -l "trackGenerator.C($run,1)"




