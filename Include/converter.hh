









// Convert Section to Column
int Sec2Col(int Sec){
   int Col;
   Col=Sec%4;
   return Col;
}

// Convert Section to Row
int Sec2Row(int Sec){
   int Row;
   Row=Sec/4;
   return Row;
}


// Read the TrackerCongig.txt file and store in two arrays of int 
void readTrackerConfig(int preampID[20], int digID[20])
{
   cout<<"\n## Function readTrackerConfig ########"<<endl;
   char buffer[100];
   
   int Sec[20], Row[20], Col[20];

   ifstream confFile;
   
   confFile.open("trackerConfig.txt");
   if(!confFile){ cerr << "Error opening the tracker configuration file (not found)" << endl;} 
  
   for(int i=0; i<5; i++){
      confFile.getline(buffer,100); 
      //cout<<buffer<<endl;
   }
   
   cout<<"Section Column  Row	Dig	Preamp"<<endl;
   for(int j=0; j<20; j++){
      confFile>>Sec[j]>>Col[j]>>Row[j]>>digID[j]>>preampID[j];
      if(Col[j]==0){cout<<Sec[j]<<"\t"<<Col[j]<<"\t"<<Row[j]<<"\t\033[40;31;1m"<<digID[j]<<" \033[0m\t"<<preampID[j]<<endl;}
      if(Col[j]==1){cout<<Sec[j]<<"\t"<<Col[j]<<"\t"<<Row[j]<<"\t\033[40;33;1m"<<digID[j]<<" \033[0m\t"<<preampID[j]<<endl;}
      if(Col[j]==2){cout<<Sec[j]<<"\t"<<Col[j]<<"\t"<<Row[j]<<"\t\033[40;35;1m"<<digID[j]<<" \033[0m\t"<<preampID[j]<<endl;}
      if(Col[j]==3){cout<<Sec[j]<<"\t"<<Col[j]<<"\t"<<Row[j]<<"\t\033[40;34;1m"<<digID[j]<<" \033[0m\t"<<preampID[j]<<endl;}
      
      
   }
   cout<<"## END ## Function readTrackerConfig #\n"<<endl;

}
