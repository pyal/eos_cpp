// Entered struct
// Pos Denc temp Opac_Coef
double EmisTemp,NormDens;
EmisTemp=1e5;
double DoIt(MatrCl &GotData)
  {
   MatrCl GotData;
   for (int k=1;k<Gotdata.DimX;k++)
      GotData(k,2)=(GotData(k+1,1)-GotData(k,1))*GotData(k,2);
   k=GotData.DimX;
   GotData(k,2)=(GotData(k,1)-GotData(k-1,1))*GotData(k,2);
   double Sum=0;
   for (int k=1;k<Gotdata.DimX;k++)
     {
      GotData(k,2)=GotData(k,4)*GotData(k,2)/NormDens;
      Sum=Sum*exp(-GotData(k,2))+
           (1-exp(-GotData(k,2)))/(exp(EmisTemp/GotData(k,3))-1);
   return Sum;
  }; 
double FindTemp(double Int)
  { return EmisTemp/(log(1/Int)+1);};
