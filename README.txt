###################################################################################################
# README file for the data analysius of the tracker data.
# created november 2022 by V. Soukeras, D. Torresi
#
# lasta update 25-11-2022
###################################################################################################

DIRECTORIES
################
Raw_data 	raw binary files from DAQ. One file for each digitizer.

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

Merged_data	One file for each run, all the data form different digitizers are merged in a 
		single file where the entries are ordered by time (raw+fine)

Clusterd_data	File where all the data from Merged data are reorganized in cluster
		Each entry is now a cluster no more an hit.

Analysis	Analys Macros

Analysis_Temp	

Info		General information files


###################################################################################################
FILES
####################
converter.C	converts raw data from binary to root, calibrates the charge and pads #
		Input from   Raw_data
			     channel2pad.txt	
			     chargeCalib_PA.txt
		Output to    Cal_data
                
                use:  converter.C("Raw_data/Run_18/dig22642.bin",22642,"Cal_data/out.root") 

merger.C	merge 2 root files at a time
		Input from Cal_data 
 		Output to Cal_data 

		
dataProcessing.sh  Shell script that use in a smart way converter.C and merger.C generating in
		an automatic way the final merged file. Require in input just run number and the
		correct path where the file are.
		$ dataProcessing n 
		where n is the number of the run
		

