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
#  update:       added the tack generator 2024-11  D. Torresi
#  last update:  the file used for mergere are named in an univocal way, for run 
#                number lower than 100 is not necessary use 0 before the run 
#                (i.e. 12 instead of 012) 2024-12  D. Torresi
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
#bindir=/home/numen/solaris/RAW_data/tracker_and_sic/
#caldir=/home/torresi/Analisi/NUMEN/Analysis_tracker_v1.1/Cal_data/run_$run
#merdir=/home/torresi/Analisi/NUMEN/Analysis_tracker_v1.1/Merged_data/run_$run

if [ $run -le 9 ]; then 
bindir=Raw_data_Br/run00$run
caldir=Cal_data_Br/run_00$run
merdir=Merged_data_Br/run_00$run
elif [ $run -le 99 ]; then
bindir=Raw_data_Br/run0$run
caldir=Cal_data_Br/run_0$run
merdir=Merged_data_Br/run_0$run
else
bindir=Raw_data_Br/run$run
caldir=Cal_data_Br/run_$run
merdir=Merged_data_Br/run_$run
fi


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
  
   if [ $run -le 9 ]; then 
      infile=$bindir/TrackerSP_00$run\_0$cont\_$dig\_000.sol
   elif [ $run -le 99 ]; then
      infile=$bindir/TrackerSP_0$run\_0$cont\_$dig\_000.sol
   else
      infile=$bindir/TrackerSP_$run\_0$cont\_$dig\_000.sol
   fi
   
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


if [ $run -le 9 ]; then 
infile=$bindir/TrackerSP_00$run\_0$cont\_$dig\_000.sol
outfile=$merdir/sic_00$run.root
elif [ $run -le 99 ]; then
infile=$bindir/TrackerSP_0$run\_0$cont\_$dig\_000.sol
outfile=$merdir/sic_0$run.root
else
infile=$bindir/TrackerSP_$run\_0$cont\_$dig\_000.sol
outfile=$merdir/sic_$run.root
fi

root -q -l "converter_solaris_sic.C(\"$infile\",$dig,\"$outfile\")"

#################################################
# Merging data			3rd level
#################################################
echo  -e "\033[40;35;1m Merging files \033[0m"  


#merfile=$merdir/merged.root

initfile=$caldir/dig_22642_cal.root    # very first file to be merged
file1=a_run$run.root			        # first argument of macro
file2=b_run$run.root			        # second argument of macro
outfile=c_run$run.root				# macro output

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

if [ $run -le 9 ]; then 
mv $outfile  $merdir/merg_00$run.root
elif [ $run -le 99 ]; then
mv $outfile  $merdir/merg_0$run.root
else
mv $outfile  $merdir/merg_$run.root
fi


#################################################
# Producing tracking data	4th level
#################################################
echo ""
echo "---------"
echo  -e "\033[40;35;1m Generating tracks \033[0m"  



#root -q -l "trackGenerator.C($run,1)"
echo ""



