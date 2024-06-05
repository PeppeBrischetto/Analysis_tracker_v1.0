///////////////////////////////////////////////////////////////////////////////////////////////////
// Converts the binary data files to root. 
// Some additional variables is written
// Charge_calib - calibrated charge
// Single_Pads - pad number  
// A time sorting of the entries, based on the total timestamp (raw+fine) is also performed.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
// use it
// converter.C("PRaw_dataath/filename",board number, "outputfilename");
// Example:  converter.C("Raw_data/Run_18/dig22642.bin",22642,"out.root")
// ////////////////////////////////////////////////////////////////////////////////////////////////
// created        July 2022  F. Dalaunay
// modified: 	 15 Nov 2022  F. Delaunay,  F.Soukeras
// last update  25 Nov 2022  added row and section variables in the output file  D. Torresi
///////////////////////////////////////////////////////////////////////////////////////////////////
// To do
// option to do not convert the event with no trapezoid flag or zero charge
// take board_id from config file
//


#include <iostream>
#include <fstream>
#include <stdio.h>
#include "TFile.h"
#include "TTree.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
///////////////////////////////////////////////////////////////////////////////////////////////////

int verbosity=0;
// File names
char fileCalCharge[200]="Calib_files/chargeCalib_PA.txt";   // charge calibration file 
char filePadMapping[200]="Calib_files/channel2pad_2.txt";        // pad mapping file

// Board mapping
// the first five number are the first columnm, the second five are the second column and so on
// Int_t board_id[20]={22642,22643,22644,22645,22646};  // run 18-18-20-21
//Int_t board_id[20]={21247,22645,22644,22643,22642};  // run 184
Int_t board_id[20]={22642,22643,22644,22645,21247};  // run 184                       

// calibration array where the calibration parameters are stored
Double_t cal_fac[5][64]; // matrix 5x64 for calibration factors for 5 PAs x 64 channels
UShort_t pad[64];	 // the i-th value of the array is the pad corresponding to the the i-th
			 // electronics (digitizer) channel

// Struct used as single channel data record in binary files. Has to be identical to struct used in DAQ program
struct data { 
   uint8_t  Channel;
   Double_t Coarse_Time_micros;
   uint16_t Fine_Time_int;
   uint16_t LoPFlags;
   uint16_t HiPFlags;
   uint16_t Energy;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
//

// Row 0-4 are row segmented in pad, from 5 to 10 are unsegmented row
int findRow(int dig, int pad){
    int row=-1;
    if(dig==board_id[0]){
       if(pad==100){
          row=5;
       }else{
          row=0;
       }
    }else if(dig==board_id[1]){
       if(pad==100){
          row=6;
       }else{
          row=1;
       }
    }else if(dig==board_id[2]){
    	if(pad==100){
          row=7;
       }else{
    	  row=2;
       }
    }else if(dig==board_id[3]){
       if(pad==100){
          row=8;
       }else{
          row=3;
       }
    }else if(dig==board_id[4]){
       if(pad==100){
          row=9;
       }else if(pad==101){
          row=10;
       }else{
          row=4;
       }
    }
    return row;
}

int findColumn(int dig){
    int Column=-1;
    if(dig==board_id[0]){Column=0;}
    else if(dig==board_id[1]){Column=1;}
    else if(dig==board_id[2]){Column=2;}
    else if(dig==board_id[3]){Column=3;}
    else if(dig==board_id[4]){Column=4;}
    return Column;
}


int findSection(int dig){
    int section=-1;
    if(dig==board_id[0]){section=0;}
    else if(dig==board_id[1]){section=4;}
    else if(dig==board_id[2]){section=8;}
    else if(dig==board_id[3]){section=12;}
    else if(dig==board_id[4]){section=16;}
    return section;
}


// initialize the charge calibration function, store the charge calibration parameter in the
// array cal_fac[][]
void calib_init(int ver=0){
   ifstream chargeCalibFile;   // cahrge calibration parameter file
   chargeCalibFile.open(fileCalCharge);
   char buffer[200];   // buffer for text
   int PA_ID;	       // preamplifier (PA) number
   //##  read header ##

   for(int i=0;i<8;i++){
      chargeCalibFile.getline(buffer,200);
      if(ver>0)cout<<buffer<<endl;
   }
   //##  read  parameters ##
   cout<<" read calibration parameters"<<endl;
   for(int k=0;k<5;k++){ //loop for 5 PAs
      chargeCalibFile.getline(buffer,200);   //read 1st empty line
      if(ver>0)cout<<buffer<<endl;
      chargeCalibFile.getline(buffer,200);   //read 2nd line with PA & Dig. info
      if(ver>0)cout<<buffer<<endl;
      chargeCalibFile>>PA_ID;		     //read 3rd line with PA ID
      if(ver>0)cout<<PA_ID<<endl;
      chargeCalibFile.getline(buffer,200);   //read 3rd line space
      chargeCalibFile.getline(buffer,200);   //read 4th empty line
      if(ver>0)cout<<buffer<<endl;
      for(int i=0;i<64;i++){   // loop for the 64 channels of one PA 
         chargeCalibFile>>cal_fac[k][i];
         if(ver>0)cout<<cal_fac[k][i]<<endl;
      }
      chargeCalibFile.getline(buffer,200);   //read last line space
   }
   chargeCalibFile.close();
}

// Initialize the pad mapping function
void pad_init(int ver=0){
   ifstream fp1;
   fp1.open(filePadMapping);

   char buffer[200];   // buffer for text
   int channel;
   //##  read header ##
   for(int i=0;i<9;i++){
      fp1.getline(buffer,200);
      if(ver>0)cout<<buffer<<endl;
   }
   //##  read parameters ##
   for(int i=0;i<64;i++){  
      fp1>>channel>>pad[i];
      if(ver>0)cout<<channel<<" \t "<<pad[i]<<endl;
   }
   fp1.close();
}


// time sorting function
// order all the entries with the same coarse time by total timestamp
void quicksort(UShort_t arr0[], Double_t arr1[], UShort_t arr2[], UShort_t arr3[], UShort_t arr4[], ULong64_t arr5[], UInt_t arr6[], Double_t arr7[], UShort_t arr8[], int low, int high) {
   int i = low;
   int j = high;
   UShort_t y0, y2, y3, y4, y8;
   y0 = y2 = y3 = y4 = y8 = 0;
   ULong64_t y5 = 0;
   Double_t y1, y7;
   y1 = y7 = 0;
   UInt_t y6 = 0;
   // compare value 
   UShort_t z = arr0[(low + high) / 2];

   // partition 
   do {
   /* fiind member above ... */
      while(arr0[i] < z) i++;

   /* find element below ... */
      while(arr0[j] > z) j--;

      if(i <= j) {
         /* swap two elements */
         y0 = arr0[i];
         arr0[i] = arr0[j]; 
         arr0[j] = y0;
   
         y1 = arr1[i];
         arr1[i] = arr1[j]; 
         arr1[j] = y1;
   
         y2 = arr2[i];
         arr2[i] = arr2[j]; 
         arr2[j] = y2;

         y3 = arr3[i];
         arr3[i] = arr3[j]; 
         arr3[j] = y3;

         y4 = arr4[i];
         arr4[i] = arr4[j]; 
         arr4[j] = y4;

         y5 = arr5[i];
         arr5[i] = arr5[j]; 
         arr5[j] = y5;

         y6 = arr6[i];
         arr6[i] = arr6[j]; 
         arr6[j] = y6;
      
         y7 = arr7[i];
         arr7[i] = arr7[j]; 
         arr7[j] = y7;
  
         y8 = arr8[i];
         arr8[i] = arr8[j]; 
         arr8[j] = y8;

         i++; 
         j--;
      }
   } while(i <= j);

   // recurse 
   if(low < j) 
      quicksort(arr0, arr1, arr2, arr3, arr4, arr5, arr6, arr7, arr8, low, j);

   if(i < high) 
      quicksort(arr0, arr1, arr2, arr3, arr4, arr5, arr6, arr7, arr8, i, high);
};







// Convert a binary file of a single digitizer in root, provide also calibration
void converter (const char *data_file_name_0, const UShort_t Board0 = 0, const char *output_file_name = "out.root") {

  ////////////////////////////////////////////////////////////////////////////////////
  // Declaration of variables
  ///////
   const Double_t us_to_ps = 1.0e6; // Conversion factor micros -> ps
   const Double_t fineTS_to_ps = 8000.0/1024.0; // Conversion factor fine timestamp -> ps (8 ns over 1024 bins)
   char block_0[sizeof(struct data)]; // Block to read data from bin file (requires char* type)

   struct data *data_0; // Single channel data from file 0

   Double_t Coarse_Time0_micros; // Coarse time stamp in micros (to store last coarse timestamps read in files)
   Coarse_Time0_micros = 1e11; // Initialization value (> 1 day)
   UInt_t HiPFlags0; // High priority flags (8 bits used) (Intermediate variable to produce single flags)
   UInt_t LoPFlags0; // Low priority flags (12 bits used) (Intermediate variable to produce single flags)
   ULong64_t Time0_ps; // Timestamp (Coarse+Fine) in ps

   // Branch variables of output data tree (single channel events, time sorted with coarse time stamp)
   UShort_t Single_Channel;
   ULong64_t Single_Time_ps;
   ULong64_t Single_Coarse_Time_int;
   UShort_t Single_Fine_Time_int;
   UShort_t Single_Board;
   UShort_t Single_Charge;
   UInt_t Single_Flags;
   UShort_t Single_Pads;
   Double_t Single_Charge_cal;
   UShort_t Single_Row;
   UShort_t Single_Column;
   UShort_t Single_Section;

   // array of variables for sorting the fine time)
   UShort_t array_Channel[100];
   ULong64_t array_Time_ps[100];
   Double_t array_Coarse_Time[100];
   UShort_t array_Fine_Time_int[100];
   UShort_t array_Board[100];
   UShort_t array_Charge[100];
   UInt_t array_Flags[100];
   UShort_t array_Pads[100];
   Double_t array_Charge_cal[100];

   // OPEN BIN DATA FILES
   std::ifstream data_file_0 ( data_file_name_0, ios::binary );
   if ( !data_file_0.is_open() ) {
      std::cout << "Error opening data file " << data_file_name_0 << std::endl;
      return;
   }

   // Output file
   TFile *out_file = new TFile(output_file_name,"recreate");
   // Output tree single channel events
   TTree *out_tree = new TTree("Data_R","Data tree");

   out_tree->Branch("Board",&Single_Board,"Board/s");
   out_tree->Branch("Channel",&Single_Channel,"Channel/s");
   out_tree->Branch("FineTSInt",&Single_Fine_Time_int,"FineTSInt/s");
   out_tree->Branch("CoarseTSInt",&Single_Coarse_Time_int,"CoarseTSInt/l");
   out_tree->Branch("Timestamp",&Single_Time_ps,"Timestamp/l");
   out_tree->Branch("Charge",&Single_Charge,"Charge/s");
   out_tree->Branch("Flags",&Single_Flags,"Flags/i");
   out_tree->Branch("Pads",&Single_Pads,"Pads/s");
   out_tree->Branch("Row",&Single_Row,"Row/s");
   out_tree->Branch("Section",&Single_Section,"Section/s");   
   out_tree->Branch("Charge_cal",&Single_Charge_cal,"Charge_cal/D");

   ULong64_t nwritten0 = 0;
   ULong64_t nwritten1 = 0;

   int k=0;	// this index is the number of event with the same coarse gain

   // Read the first entry of the input file.
   data_file_0.read(block_0,sizeof(struct data));
   data_0 = (struct data*)block_0;

   array_Channel[k]=(UShort_t)(data_0->Channel);
   array_Time_ps[k]=ULong64_t(us_to_ps*(data_0->Coarse_Time_micros)) + ULong64_t((Double_t)(data_0->Fine_Time_int)*fineTS_to_ps);
   array_Coarse_Time[k]=data_0->Coarse_Time_micros;
   array_Fine_Time_int[k]=(UShort_t)(data_0->Fine_Time_int);
   array_Board[k]=Board0;
   array_Charge[k]=(UShort_t)(data_0->Energy);
   LoPFlags0 = (UInt_t)(data_0->LoPFlags);
   HiPFlags0 = (UInt_t)(data_0->HiPFlags);
   array_Flags[k]=LoPFlags0 << 16;
   array_Flags[k]+= HiPFlags0;


   //#  charge calibration initialisation #
   calib_init(verbosity);
   //#  initialise pad mapping #
   pad_init(verbosity);

   //##################### Charge calibration ############################
   for(UInt_t cp=0; cp<64; cp++){    
      if(array_Board[k]==board_id[0] && array_Channel[k]==cp){
         array_Charge_cal[k]=array_Charge[k]*cal_fac[0][cp];	//Charge_cal is the Charge calibration
         array_Pads[k]=pad[cp];				//a tracker pad number is associated to the corresponding digitizer channel
      }
      if(array_Board[k]==board_id[1] && array_Channel[k]==cp){
         array_Charge_cal[k]=array_Charge[k]*cal_fac[1][cp];		
         array_Pads[k]=pad[cp];				
      }
      if(array_Board[k]==board_id[2] && array_Channel[k]==cp){
         array_Charge_cal[k]=array_Charge[k]*cal_fac[2][cp];		
         array_Pads[k]=pad[cp];				
      }
      if(array_Board[k]==board_id[3] && array_Channel[k]==cp){
         array_Charge_cal[k]=array_Charge[k]*cal_fac[3][cp];		
         array_Pads[k]=pad[cp];				
      }
      if(array_Board[k]==board_id[4] && array_Channel[k]==cp){
         array_Charge_cal[k]=array_Charge[k]*cal_fac[4][cp];		
         array_Pads[k]=pad[cp];				
      }
   }
   //##################### END Charge calibration ############################


// Loop on data file

   for (int i=1; i<1000000; i++){   // if you want to convert just part of the file
   //for (int i=1; i>0; i++){
      if(data_file_0.eof()) break;
      data_file_0.read(block_0,sizeof(struct data));
      data_0 = (struct data*)block_0;
      k++;
  
      array_Channel[k]=(UShort_t)(data_0->Channel);
      array_Time_ps[k]=ULong64_t(us_to_ps*(data_0->Coarse_Time_micros)) + ULong64_t((Double_t)(data_0->Fine_Time_int)*fineTS_to_ps);
      array_Coarse_Time[k]=(data_0->Coarse_Time_micros);
      array_Fine_Time_int[k]=(UShort_t)(data_0->Fine_Time_int);
      array_Board[k]=Board0;
      array_Charge[k]=(UShort_t)(data_0->Energy);
      LoPFlags0 = (UInt_t)(data_0->LoPFlags);
      HiPFlags0 = (UInt_t)(data_0->HiPFlags);  
      array_Flags[k]=LoPFlags0 << 16;
      array_Flags[k]+= HiPFlags0;
     
      //##################### Charge calibration ############################
  
      for(UInt_t cp=0; cp<64; cp++){    
         if(array_Board[k]==board_id[0] && array_Channel[k]==cp){
            array_Charge_cal[k]=array_Charge[k]*cal_fac[0][cp];	//Charge_cal is the Charge calibration
            array_Pads[k]=pad[cp];				//a tracker pad number is associated to the corresponding digitizer channel
         }
         if(array_Board[k]==board_id[1] && array_Channel[k]==cp){
            array_Charge_cal[k]=array_Charge[k]*cal_fac[1][cp];		
            array_Pads[k]=pad[cp];				
         }
         if(array_Board[k]==board_id[2] && array_Channel[k]==cp){
            array_Charge_cal[k]=array_Charge[k]*cal_fac[2][cp];		
            array_Pads[k]=pad[cp];				
         }
         if(array_Board[k]==board_id[3] && array_Channel[k]==cp){
            array_Charge_cal[k]=array_Charge[k]*cal_fac[3][cp];		
            array_Pads[k]=pad[cp];				
         }
         if(array_Board[k]==board_id[4] && array_Channel[k]==cp){
            array_Charge_cal[k]=array_Charge[k]*cal_fac[4][cp];		
            array_Pads[k]=pad[cp];				
         }
      }
      //##################### END Charge calibration ############################
   
      // compare the Coarse time of the running entry with that of the previous entry
      // if they are different write on file the previous entry
      // else write on an array all the entries with the same coarse time and order them with quicksort function.
      if( array_Coarse_Time[k]!= array_Coarse_Time[k-1]){
         if(k==1){ //the entry has a coarse time different with that of the previous entry.
            Single_Coarse_Time_int =  (ULong64_t)(array_Coarse_Time[k-1]*us_to_ps);
            Single_Board = array_Board[k-1];
            Single_Channel = array_Channel[k-1];
            Single_Charge = array_Charge[k-1];
            Single_Fine_Time_int = array_Fine_Time_int[k-1];
            Single_Time_ps = array_Time_ps[k-1];
            Single_Flags = array_Flags[k-1];
            Single_Charge_cal = array_Charge_cal[k-1];
            Single_Pads = array_Pads[k-1];
            Single_Row=findRow(Single_Board, Single_Pads);
            Single_Section=findSection(Single_Board);

            array_Coarse_Time[k-1] = array_Coarse_Time[k];
            array_Board[k-1] = array_Board[k];
            array_Channel[k-1] = array_Channel[k];
            array_Charge[k-1] = array_Charge[k];
            array_Fine_Time_int[k-1] = array_Fine_Time_int[k];
            array_Time_ps[k-1] = array_Time_ps[k];
            array_Flags[k-1] = array_Flags[k];  
 
            array_Charge_cal[k-1]=array_Charge_cal[k];
            array_Pads[k-1]=array_Pads[k];



            out_tree->Fill();
            nwritten0++;
            k=0;
         }else if(k>1){ // if more than one entry has the same coarse time. Order them by the fine time!
         // sorting of the elements of the array by fine time
            quicksort(array_Fine_Time_int, array_Coarse_Time, array_Board, array_Channel, array_Charge, array_Time_ps, array_Flags, array_Charge_cal, array_Pads, 0, k-1);
    
            for(int h=0; h<k; h++){
               Single_Coarse_Time_int = (ULong64_t)(array_Coarse_Time[h]*us_to_ps);
               Single_Board = array_Board[h];
               Single_Channel = array_Channel[h];
               Single_Charge = array_Charge[h];
               Single_Fine_Time_int = array_Fine_Time_int[h];
               Single_Time_ps = array_Time_ps[h];
               Single_Flags = array_Flags[h];
               Single_Charge_cal = array_Charge_cal[h];
               Single_Pads = array_Pads[h];
               Single_Row=findRow(Single_Board, Single_Pads);
               Single_Section=findSection(Single_Board);

               out_tree->Fill();
               nwritten0++;
            }
            // make the last entry of the array the first entry of a new group
            array_Coarse_Time[0] = array_Coarse_Time[k];
            array_Board[0] = array_Board[k];
            array_Channel[0] = array_Channel[k];
            array_Charge[0] = array_Charge[k];
            array_Fine_Time_int[0] = array_Fine_Time_int[k];
            array_Time_ps[0] = array_Time_ps[k];
            array_Flags[0] = array_Flags[k];     
            array_Charge_cal[0]=array_Charge_cal[k];
            array_Pads[0]=array_Pads[k]; 
      
            k=0;
         }
    
      }
  
   }

   std::cout << "Events written to tree: " << out_tree->GetEntries() << std::endl;
   std::cout << "Events from file: " << nwritten0 << std::endl;

   data_file_0.close();

   out_tree->Write();
   out_file->Purge();
   out_file->Close();

   return;
}
