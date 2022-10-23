Double_t lpnbline (Double_t *x, Double_t *par)
{
  //this is how to exclude a region from the fit
  if (x[0] > 2.98 && x[0] < 3) {
    TF1::RejectPoint();
    return 0;
  }
  return par[0]+x[0]*par[1];
}

void lpnsignificance(TH1F *h_inv_mass)
{
  TF1 *_b = new TF1("bg", lpnbline ,2.95,3.1,4);
  _b->SetParameters(10,-1);
  h_inv_mass->Fit(_b,"","same",2.95,3.1);


  char formula[500]; sprintf(formula, "%g + %g*x  + [2]*TMath::Gaus(x,[0],[1],0)",_b->GetParameter(0),_b->GetParameter(1));
  TF1 *n = new TF1("n",formula,2.95,3.1);
  n->SetNpx(1000);
  n->SetParameters(2.9928,2e-3,30);
  n->SetParLimits(0,2.983,2.995);
  n->SetParLimits(1,7e-4,6e-3);
  n->SetLineColor(kRed+1); n->SetLineWidth(1);
  cout<<"fitting SG"<<endl;
  h_inv_mass->Fit(n,"R","E1 same",2.95,3.1);

  //redefine background to account for the excluded region in the fit
  sprintf(formula,"%g + %g*x",_b->GetParameter(0),_b->GetParameter(1));
  TF1 *b = new TF1("bg", formula,2.95,3.1);

  double Imin = n->GetParameter(0) - 3*n->GetParameter(1);
  double Imax = n->GetParameter(0) + 3*n->GetParameter(1);
  cout << "================ SIGNIFICANCE ================" << endl;
  double N = n->Integral(Imin,Imax)/h_inv_mass->GetBinWidth(1);
  double B = b->Integral(Imin,Imax)/h_inv_mass->GetBinWidth(1);
  double S = (N - B)/sqrt(B);
  cout << "(Imin,Imax)=("<<Imin<<","<<Imax<<")"<<endl;
  cout << "ntot = " << N << endl;
  cout << "bg integral is: " << (b->Integral(Imin,Imax))/h_inv_mass->GetBinWidth(1) << endl;
  cout << "Yield = " << N - B << endl;
  cout << "significance = (n-b)/sqrt(b) = " << S << endl;


   TPaveText *pt = new TPaveText(3.04,20,3.055,30,"br");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetTextColor(13);
   pt->SetTextSize(0.038);
   char stext[100]; sprintf(stext, "S = #frac{n-b}{#sqrt{b}} = %1.2f",S);
   TText *text = pt->AddText(stext);
   pt->Draw();
}

