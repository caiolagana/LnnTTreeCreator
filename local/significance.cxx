Double_t bline (Double_t *x, Double_t *par)
{
  //this is how to exclude a region from the fit
  if ((x[0] > 2.990 && x[0] < 2.998) || (x[0]<par[1])) {
    TF1::RejectPoint();
    return 0;
  }
  
  //return par[2]*TMath::Gaus(x[0],par[0],par[1],0);
  return par[0]*sqrt(fabs(x[0]-par[1]))*exp(-par[2]*(x[0]-par[1])) + par[3]*TMath::Gaus(x[0],par[4],par[5],0);
  // + par[3]*TMath::Gaus(x[0],par[4],par[5],0)  
}

void significance(TH1F *h_inv_mass)
{

  TF1 bg("bg", bline ,2.95,3.1,6);
  //TF1 *bg = new TF1("bg","[0]*TMath::Sqrt(x-[1])*exp(-[2]*(x-[1]))",2.95,3.1);
  bg.SetParameters(200,2.956,10,25,3,0.03);
  bg.SetParLimits(3,0,100);
  bg.SetParLimits(5,1e-2,10);
  //bg.SetParLimits(0,1,1e4);
  //bg.SetParLimits(1,2.9502,2.96);
  //bg.SetParLimits(2,0,200);
  bg.SetNpx(1000);
  cout<<"fitting BG"<<endl;
  h_inv_mass->Fit("bg","R","E1");//http://root.cern.ch/root/html/TH1.html#TH1:Fit@1
  //h_inv_mass->Fit("bg","NR","E1");//http://root.cern.ch/root/html/TH1.html#TH1:Fit@1
  cout<<"BG fitted"<<endl;

  
  char formula[500]; sprintf(formula, "%g*sqrt(x-%g)*exp(-%g*(x-%g)) + %g*TMath::Gaus(x,%g,%g,0)+ [2]*TMath::Gaus(x,[0],[1],0)",bg.GetParameter(0),bg.GetParameter(1),bg.GetParameter(2),bg.GetParameter(1),bg.GetParameter(3),bg.GetParameter(4),bg.GetParameter(5));
  TF1 *n = new TF1("n",formula,2.95,3.1);
  n->SetNpx(1000);
  n->SetParameters(2.9928,3e-3,52);
  n->SetParLimits(0,2.975,2.996); n->SetParName(0,"m");
  n->SetParLimits(1,1e-3,6e-3);  n->SetParName(1,"#sigma");
  n->SetParLimits(2,1,100);      n->SetParName(2,"A");
  n->SetLineColor(kGreen); n->SetLineWidth(1);// n->SetTitle(Form("C#sqrt(x-x_{0})e^{-#lambda(x-x_{0})} + A*Gaus(x,m,#sigma)"));
  cout<<"fitting SG"<<endl;
  h_inv_mass->Fit(n,"+R","E1",2.96,3.1);
  cout<<"SG fitted"<<endl;
  cout<<"SG fit formula (BG+peak): "<<formula<<endl;
  n->Print();

  //redefine background to account for the excluded region in the fit
  sprintf(formula,"%g*sqrt(x-%g)*exp(-%g*(x-%g))+ %g*TMath::Gaus(x,%g,%g,0)",bg.GetParameter(0),bg.GetParameter(1),bg.GetParameter(2),bg.GetParameter(1),bg.GetParameter(3),bg.GetParameter(4),bg.GetParameter(5));
  TF1 *b = new TF1("b", formula,2.95,3.1);
  b->SetLineStyle(2); b->SetLineWidth(1); b->SetLineColor(kGray+2); b->SetNpx(1000); b->Draw("same"); n->Draw("l same");

  double Imin = n->GetParameter(0) - 3*TMath::Abs(n->GetParameter(1));
  double Imax = n->GetParameter(0) + 3*TMath::Abs(n->GetParameter(1));
  cout << "================ SIGNIFICANCE ================" << endl;
  double N = n->Integral(Imin,Imax)/h_inv_mass->GetBinWidth(1);
  double B = b->Integral(Imin,Imax)/h_inv_mass->GetBinWidth(1);
  double S = (N - B)/sqrt(N);
  cout << "BinWidth= "<<h_inv_mass->GetBinWidth(1)*1e3<<" MeV"<<endl;
  cout << "(Imin,Imax)=("<<Imin<<","<<Imax<<")"<<endl;
  cout << "ntot = " << N << endl;
  cout << "bg integral is: " << B << endl;
  cout << "Yield = " << N - B << endl;
  cout << "significance = (n-b)/sqrt(n) = " << S << endl;

  /*
   TPaveText *pt = new TPaveText(2.97,20,3.02,30,"br");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextColor(13);
   pt->SetTextSize(0.038);
   char stext[100]; sprintf(stext, "S = #frac{n-b}{#sqrt{n}} = %1.2f",S);
   TText *text = pt->AddText(stext);
   pt->Draw();
  */
}

