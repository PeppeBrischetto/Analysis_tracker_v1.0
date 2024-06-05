// PAy attention to:
//    1)  if in the file is written [System] or [admin]  in one case for(int i=0;i<22;i++) in the other for(int i=0;i<23;i++)
//    2) the input file is written two times. First time is opened to read the initial time, the second to read the parameters.


void rateDischarge(char *filename)
{
   gStyle->SetTitleSize(0.1,"t");

// Parametri da impostare
   //char namerefch[]="IMon";
   char namerefch[]="IMonReal";
   //char namerefch[]="IMonDet";   
   int channelref=2;
   float dischargeCurr=1;   	//current i uA
   int lowerTime=-1;
   int higherTime=1000000000;
         
   int dischargeNum[6]={0};
   int flagDis[6]={0};  	//0 no discharge, inside a discharge;
 
   ifstream in;		// input file
   in.open(filename);
   
   char name[30];
   char buffer[100];
   char title[100];
   sprintf(title, "", channelref);
   char ch;
   int channel, var;
   int k=0, j=0, l=0, lines=0;
   int j0=0, j1=0, j2=0, j3=0, j4=0, j5=0, j6=0;
   int mese, gg, ore, min, sec, secTot, zero;
   float value;
  
   TGraph *gra0 =new TGraph(0); // current versus second
   TGraph *grb0 =new TGraph(0); // current versus entry
   
   TGraph *gra1 =new TGraph(0); // current versus second
   TGraph *grb1 =new TGraph(0); // current versus entry
   
   TGraph *gra2 =new TGraph(0); // current versus second
   TGraph *grb2 =new TGraph(0); // current versus entry
   
   TGraph *gra3 =new TGraph(0); // current versus second
   TGraph *grb3 =new TGraph(0); // current versus entry
   
   TGraph *gra4 =new TGraph(0); // current versus second
   TGraph *grb4 =new TGraph(0); // current versus entry

   TGraph *gra5 =new TGraph(0); // current versus second
   TGraph *grb5 =new TGraph(0); // current versus entry

   TGraph *gra6 =new TGraph(0); // current versus second
   TGraph *grb6 =new TGraph(0); // current versus entry
   
// take initial time
   for(int i=0;i<6;i++){
      in.get(ch);
      //cout<<ch;
   }      
   in>>mese>>ch>>gg>>ch>>ore>>ch>>min>>ch>>sec;
   cout<<mese<<" "<<ch<<" "<<gg<<" "<<ch<<" "<<ore<<" "<<ch<<" "<<min<<" "<<ch<<" "<<sec;
   zero=sec+min*60+ore*60*60+gg*24*60*60;
   cout<<" | zero sec: "<< zero<<endl;
   
   while(in.getline(buffer,100)){   
      //cout<<buffer<<endl; 
      lines++;
   }
   cout<<" Total number of lines: "<<lines<<endl;
   
   if(lowerTime<0) lowerTime=0;
   
   in.close();
   in.open(filename);
// END take initial time


   for(l=0; l<lines; l++){
   //while(!in.eof()){
      // jump to the value of the correspondig channel
      cout<<"\n******** "<<l<<" **********"<<endl;          
      for(int i=0;i<6;i++){
         in.get(ch);
         cout<<ch;
      }            
      in>>mese>>ch>>gg>>ch>>ore>>ch>>min>>ch>>sec;
      secTot=sec+min*60+ore*60*60+gg*24*60*60-zero;
      cout<<mese<<" "<<gg<<" "<<ore<<" "<<min<<" "<<sec<<" | "<<secTot<<endl;

      for(int i=0;i<22;i++){
         in.get(ch);
         cout<<ch;
      }
      //cout<<endl;

      // read the channel value
      in>>channel;  
      cout<<channel<<endl; 
   
      // Skip the characters up to the variable name
      for(int i=0;i<7;i++){
         in.get(ch);
         //cout<<ch;
      }
      
      // Read the variable name
      while(var==0){
         //ch=in.peek();
         in.get(ch);
         cout<<ch;
         if(ch==']'){
            var++;
            cout<<var;
            name[k]='\0';	// termino l'array
            k=0; 
         }else{
            name[k++]=ch;
         }
      }      
      //cout<<"\n"<<name<<endl;

      // Skip the characters up to the variable value
      for(int i=0;i<6;i++){
         in.get(ch);
         //cout<<ch;
      }
      //cout<<endl;
     
      // read the variable
      in>>value;
      cout<<"*"<<value<<endl;
      
      if(value>dischargeCurr){   //current in uA
         for(int i=0; i<6; i++){   
            if(flagDis[i]==0 && secTot>lowerTime && secTot<higherTime && channel==i){
               dischargeNum[i]++;
               flagDis[i]=1;
               cout<<"*** "<<value<<"\t"<<dischargeCurr<<endl;
            }
         }
      }else{
         for(int i=0; i<6; i++){   
            if(channel==i)flagDis[i]=0;
         }
      }
      
      //leggi fino alla fine
      in.get(buffer,10);
      //cout<<buffer;
      //in.get(buffer,10);
      in.get(ch);
      //cout<<ch<<endl;
           
      cout<<"\033[30;31m"<<name<<"|\t"<<value<<"\033[0m"<<"\n";
      //ss << name;
      //ss >> namesr;
      
      //if (namesr != nameref){
      if (strcmp(name,namerefch) == 0 && channel==0){
         //cout << name << " is not equal to "<<endl;
         gra0->SetPoint(j0, secTot, value);
         grb0->SetPoint(j0, j0++, value);
         cout<<" channel "<<channelref<<"\t"<<name<<"\t"<<value<<endl;
      }else if (strcmp(name,namerefch) == 0 && channel==1){
         //cout << namesr << " is equal to "<<endl;
         gra1->SetPoint(j1, secTot, value);
         grb1->SetPoint(j1, j1++, value);
      }else if (strcmp(name,namerefch) == 0 && channel==2){
         //cout << namesr << " is equal to "<<endl;
         gra2->SetPoint(j2, secTot, value);
         grb2->SetPoint(j2, j2++, value);
      }else if (strcmp(name,namerefch) == 0 && channel==3){
         //cout << namesr << " is equal to "<<endl;
         gra3->SetPoint(j3, secTot, value);
         grb3->SetPoint(j3, j3++, value);
      }else if (strcmp(name,namerefch) == 0 && channel==4){
         //cout << namesr << " is equal to "<<endl;
         gra4->SetPoint(j4, secTot, value);
         grb4->SetPoint(j4, j4++, value);
      }else if (strcmp(name,namerefch) == 0 && channel==5){
         //cout << namesr << " is equal to "<<endl;
         gra5->SetPoint(j5, secTot, value);
         grb5->SetPoint(j5, j5++, value);
      }else if (strcmp(name,namerefch) == 0 && channel==6){
         //cout << namesr << " is equal to "<<endl;
         gra6->SetPoint(j6, secTot, value);
         grb6->SetPoint(j6, j6++, value);
      }
      // azzera le variabili
      var=0;
      //if(secTot>2000){break;}
      
   }
         
      
   cout<<"\n ENTRIES: "<<j0<<" "<<j1<<" "<<j2<<" "<<j3<<" "<<j4<<endl;
   
   TLegend *leg=new TLegend(0.8,0.65,0.9,0.9);
   leg->AddEntry(gra0,"Ch 0", "P");
   leg->AddEntry(gra1,"Ch 1", "P");
   leg->AddEntry(gra2,"Ch 2", "P");      
   leg->AddEntry(gra3,"Ch 3", "P");
   leg->AddEntry(gra4,"Ch 4", "P");
   leg->AddEntry(gra5,"Ch 5", "P");   
   //leg->AddEntry(grb6,"Ch 6", "P");         
   
   TH2D *bg= new TH2D("bg","bg2",1000,0,secTot,1000,-15,15);
   
   TCanvas *c1= new TCanvas("c1");
   
   bg->GetXaxis()->SetTitle("seconds");
   bg->GetYaxis()->SetTitle("current (nA)");
   bg->GetYaxis()->SetTitleOffset(1.25);
   
   bg->GetXaxis()->SetTitleOffset(0.4);
   bg->GetXaxis()->SetLabelSize(0.09);
   bg->GetXaxis()->SetTitleSize(0.09);
   bg->GetYaxis()->SetTitleOffset(0.4);
   bg->GetYaxis()->SetLabelSize(0.09);
   bg->GetYaxis()->SetTitleSize(0.09);

   bg->SetTitle(title);
   bg->SetStats(0);
   bg->Draw();
   
   gra2->SetMarkerStyle(22);
   gra2->SetMarkerSize(1);
   //if(j2>0)gra2->Draw("P");
   gra1->SetMarkerStyle(22);
   gra1->SetMarkerSize(1.3);
   gra1->SetMarkerColor(kOrange);
   gra1->SetLineColor(kOrange);
   gra1->SetLineWidth(2);
   if(j1>0)gra1->Draw("Pl");
   gra3->SetMarkerStyle(22);
   gra3->SetMarkerSize(1);
   gra3->SetMarkerColor(kBlue);
   gra3->SetLineColor(kBlue);
   gra3->SetLineWidth(2);
   //if(j3>0)gra3->Draw("P");
   gra4->SetMarkerStyle(22);
   gra4->SetMarkerSize(1.3);
   gra4->SetMarkerColor(kGray +2);
   gra4->SetLineColor(kGray +2);
   gra4->SetLineWidth(2);
   if(j4>0)gra4->Draw("Pl");
   gra5->SetMarkerStyle(22);
   gra5->SetMarkerSize(1);
   gra5->SetMarkerColor(kOrange +3);
   gra5->SetLineColor(kOrange +3);
   gra5->Draw("P");
   gra6->SetMarkerStyle(22);
   gra6->SetMarkerSize(1);
   //gra6->SetMarkerColor(kAzure +10);
   gra6->Draw("P");
   gra0->SetMarkerStyle(22);
   gra0->SetMarkerSize(1.3);
   gra0->SetMarkerColor(kBlue);
   gra0->SetLineColor(kBlue);
   gra0->SetLineWidth(2);

   if(j0>0)gra0->Draw("Pl");
   leg->Draw("SAME");
  
   TCanvas *c2= new TCanvas("c2");
   c2->Divide(1,6);
   c2->cd(1);
   //bg->Draw();
   gra0->GetXaxis()->SetTitle("seconds");
   gra0->GetYaxis()->SetTitle("current (#muA)");
   gra0->GetYaxis()->SetTitleOffset(1.25);
   gra0->SetTitle("Anode");
   gra0->GetXaxis()->SetTitleOffset(0.4);
   gra0->GetXaxis()->SetLabelSize(0.09);
   gra0->GetXaxis()->SetTitleSize(0.09);
   gra0->GetYaxis()->SetTitleOffset(0.4);
   gra0->GetYaxis()->SetLabelSize(0.09);
   gra0->GetYaxis()->SetTitleSize(0.09);

   if(j0>0)gra0->Draw("APl");
   c2->cd(2);
   //bg->Draw();
   gra1->GetXaxis()->SetTitle("seconds");
   gra1->GetYaxis()->SetTitle("current (#muA)");
   gra1->GetYaxis()->SetTitleOffset(1.25);
   gra1->SetTitle("Top 3");
   gra1->GetXaxis()->SetTitleOffset(0.4);
   gra1->GetXaxis()->SetLabelSize(0.09);
   gra1->GetXaxis()->SetTitleSize(0.09);
   gra1->GetYaxis()->SetTitleOffset(0.4);
   gra1->GetYaxis()->SetLabelSize(0.09);
   gra1->GetYaxis()->SetTitleSize(0.09);
   if(j1>0)gra1->Draw("APl");
   c2->cd(3);
   gra2->GetXaxis()->SetTitle("seconds");
   gra2->GetYaxis()->SetTitle("current (#muA)");
   gra2->GetYaxis()->SetTitleOffset(1.25);
   gra2->SetTitle("Top 2");
   gra2->GetXaxis()->SetTitleOffset(0.4);
   gra2->GetXaxis()->SetLabelSize(0.09);
   gra2->GetXaxis()->SetTitleSize(0.09);
   gra2->GetYaxis()->SetTitleOffset(0.4);
   gra2->GetYaxis()->SetLabelSize(0.09);
   gra2->GetYaxis()->SetTitleSize(0.09);

   if(j2>0)gra2->Draw("APl");
   c2->cd(4);
   gra3->GetXaxis()->SetTitle("seconds");
   gra3->GetYaxis()->SetTitle("current (#muA)");
   gra3->GetYaxis()->SetTitleOffset(1.25);
   gra3->SetTitle("Top 1");
   gra3->GetXaxis()->SetTitleOffset(0.4);
   gra3->GetXaxis()->SetLabelSize(0.09);
   gra3->GetXaxis()->SetTitleSize(0.09);
   gra3->GetYaxis()->SetTitleOffset(0.4);
   gra3->GetYaxis()->SetLabelSize(0.09);
   gra3->GetYaxis()->SetTitleSize(0.09);

   if(j3>0)gra3->Draw("APl");

   c2->cd(5);
   //bg->Draw();
   gra4->GetXaxis()->SetTitle("seconds");
   gra4->GetYaxis()->SetTitle("current (#muA)");
   gra4->GetYaxis()->SetTitleOffset(1.25);
   gra4->SetTitle("Bot 1");
   gra4->GetXaxis()->SetTitleOffset(0.4);
   gra4->GetXaxis()->SetLabelSize(0.09);
   gra4->GetXaxis()->SetTitleSize(0.09);
   gra4->GetYaxis()->SetTitleOffset(0.4);
   gra4->GetYaxis()->SetLabelSize(0.09);
   gra4->GetYaxis()->SetTitleSize(0.09);
   if(j4>0)gra4->Draw("APl");
   c2->cd(6);
   //bg->Draw();
   gra5->GetXaxis()->SetTitle("seconds");
   gra5->GetYaxis()->SetTitle("current (#muA)");
   gra5->GetYaxis()->SetTitleOffset(1.25);
   gra5->SetTitle("Cathode");
   gra5->GetXaxis()->SetTitleOffset(0.4);
   gra5->GetXaxis()->SetLabelSize(0.09);
   gra5->GetXaxis()->SetTitleSize(0.09);
   gra5->GetYaxis()->SetTitleOffset(0.4);
   gra5->GetYaxis()->SetLabelSize(0.09);
   gra5->GetYaxis()->SetTitleSize(0.09);
   gra5->Draw("APl");

     cout<<" Total number of lines: "<<lines<<endl;
   /*TCanvas *c2= new TCanvas("c2");
   grb2->SetMarkerStyle(22);
   grb2->SetMarkerSize(1);
   grb2->GetXaxis()->SetTitle("entry");
   grb2->GetYaxis()->SetTitle("current (#muA)");
   grb2->SetTitle( title);
   grb2->Draw("AP");
   grb1->SetMarkerStyle(22);
   grb1->SetMarkerSize(1);
   grb1->SetMarkerColor(kRed);
   grb1->Draw("P");
   grb3->SetMarkerStyle(22);
   grb3->SetMarkerSize(1);
   grb3->SetMarkerColor(kBlue);
   grb3->Draw("P");
   grb4->SetMarkerStyle(22);
   grb4->SetMarkerSize(1);
   grb4->SetMarkerColor(kGreen);
   grb4->Draw("P");
   grb5->SetMarkerStyle(22);
   grb5->SetMarkerSize(1);
   grb5->SetMarkerColor(kViolet);
   //grb5->Draw("P");
   grb6->SetMarkerStyle(22);
   grb6->SetMarkerSize(1);
   grb6->SetMarkerColor(kAzure +10);
   //grb6->Draw("P");
   grb0->SetMarkerStyle(22);
   grb0->SetMarkerSize(1);
   grb0->SetMarkerColor(kOrange);
   grb0->Draw("P");
   leg->Draw("SAME");*/
   
 
   TCanvas *c31= new TCanvas("c31");  
   if(j0>0)gra0->Draw("APl");

   TCanvas *c32= new TCanvas("c32");
   if(j1>0)gra1->Draw("APl");

   gra1->GetXaxis()->SetTitleOffset(0.7);
   gra1->GetXaxis()->SetLabelSize(0.05);
   gra1->GetXaxis()->SetTitleSize(0.05);
   gra1->GetYaxis()->SetTitleOffset(0.7);
   gra1->GetYaxis()->SetLabelSize(0.05);
   gra1->GetYaxis()->SetTitleSize(0.05);
   
   
  cout<<"****************************************"<<endl;
  cout<<" current value defining a discharge: "<<dischargeCurr<<endl;
  cout<<" total discharge number: "<<dischargeNum[0]<<endl;
  cout<<" total discharge number: "<<dischargeNum[1]<<endl;
  cout<<" total discharge number: "<<dischargeNum[2]<<endl;
  cout<<" total discharge number: "<<dischargeNum[3]<<endl;
  cout<<" total discharge number: "<<dischargeNum[4]<<endl;
  cout<<" total discharge number: "<<dischargeNum[5]<<endl;
  cout<<" Total time:             "<<secTot-lowerTime<<" sec\t"<<(secTot-lowerTime)/60<<" min"<<endl; 
  cout<<" Rate:                   "<<dischargeNum[0]/(float(secTot-lowerTime)/60)<<" discharge/min"<<endl;
  cout<<" Rate:                   "<<dischargeNum[1]/(float(secTot-lowerTime)/60)<<" discharge/min"<<endl;  
  cout<<" Rate:                   "<<dischargeNum[2]/(float(secTot-lowerTime)/60)<<" discharge/min"<<endl;
  cout<<" Rate:                   "<<dischargeNum[3]/(float(secTot-lowerTime)/60)<<" discharge/min"<<endl;
  cout<<" Rate:                   "<<dischargeNum[4]/(float(secTot-lowerTime)/60)<<" discharge/min"<<endl;
  cout<<" Rate:                   "<<dischargeNum[5]/(float(secTot-lowerTime)/60)<<" discharge/min"<<endl;
  cout<<"****************************************"<<endl;
     
}
