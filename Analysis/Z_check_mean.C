
void Z_check_mean()
{

   TH1F *hrow=new TH1F("r0","r0",62,-0.5,61.5);
   hrow->Fill(29, 1220);
   hrow->Fill(30, 2450);
   hrow->Fill(31, 3200);
   hrow->Fill(32, 2907);
   hrow->Fill(33, 876);
   
   hrow->Draw();
   
   float mean=hrow->GetMean();
   
   int  binmax = hrow->GetMaximumBin(); 
   float charge=0;
   float totalcharge= hrow->Integral(1,62);
   float weigthedpos=0;
   float centroid=0;
   for(int k=1; k<=62; k++){
      charge = hrow->GetBinContent(k);
      weigthedpos = (k-1)*charge/totalcharge;
      centroid = centroid + weigthedpos;
   }
     
   cout<<"centroid "<< centroid<<endl;
   cout<<"mean "<<mean<<endl;
  
}
