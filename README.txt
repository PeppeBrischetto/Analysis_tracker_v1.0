###################################################################################################
# README file for the data analysius of the tracker data.
# created november 2022 by V. Soukeras, D. Torresi
#
# updated 25-11-2022
# last update 07-10-2024  D.Torresi
###################################################################################################

DIRECTORIES
################
Raw_data 	raw binary files from DAQ. One file for each digitizer.
                Raw data could be directly red from the output directory of the DAQ

Calib_files	Calibration files. They are used by converter.C to get the charge calibration and 
		the physical physical channel from the electronics channel. 
-----------
	*	channel2pad.txt		pad mapping. from electronics channel to pad number.
		pad# corresponds to channels 0,1,2,...n
		!!Do not change data formatting and the number of comment lines!!
		
	*	chargeCalib_PA.txt	charge calibration parameters (assuming a y=a*x) taking 
		into account only the preamplifiers calibration for the moment
		!!Do not change data formatting and the number of comment lines!!

Cal_data	calibrated root files, still one file for each digitizer.
		Not really necessary after Merged data are produced, can be erased.

Merged_data	One file for each run, all the data form different digitizers are merged in a 
		single file where the entries are ordered by time (raw+fine)

#Clustered_data	File where all the data from Merged data are reorganized in cluster
		Each entry is now a cluster no more an hit.  OBSOLETE

Analysis	Analysis Macros that can be considered working properly and are 

Analysis_Temp	Analysis directory where are te

Info		General information files

Tracks          File where all the data from Merged data are reorganized in tracks


###################################################################################################
FILES
####################
converter_nudaq.C	converts the raw data produced by nudaq DAQ from binary to root, calibrates 
		the charge and pads #
		Input from   Raw_data
			     channel2pad.txt	
			     chargeCalib_PA.txt
		Output to    Cal_data
                
                use:  converter_nudaq.C("Raw_data/Run_18/dig22642.bin",22642,"Cal_data/out.root") 

converter_solaris_tracker.C	converts the raw data produced by solaris DAQ for the tracker from 
		binary to root, calibrates 
		the charge and pads #
		Input from   Raw_data
			     channel2pad.txt	
			     chargeCalib_PA.txt
		Output to    Cal_data
                
                use:  converter_solaris.C("Raw_data/~/solaris/RAW_data/tracker_and_sic/tracker_and_sic_116_01_22642_000.sol",22642,"Cal_data/out.root") 

converter_solaris_tracker.cc	same as converter_solaris_tracker.C but do not use root, uses its libraries.
			
		UNDER DEVELOPMENT
		use: for the moment the input data name, the digitizer ID, the output
		file name must be introduced explicitely inside the code.

converter_solaris_Sic.C	


merger.C	merge 2 root files at a time
		Input from Cal_data 
 		Output to Cal_data 
		
trackerGenerator.C		Take as input file from tracker and (optionally) from SiC and produce a file with tracks. Each entry of the
		generated file is a tracks with a number of info for the track.  It must be introduced in the dataProcessing.sh macro
		
		Input from   Merged_data

		Output to    Tracks
                	
		
		
dataProcessing.sh  Shell script that use converter_xxx_.C and merger.C generating in the final merged 
		file in a single step. Require in input just run number
		check the correct path where the file are.
		$ dataProcessing n 
		where n is the number of the run
		
compila.sh	Shell script that compile converter.cc
