void plotdedx()
{
  TFile *f1 = new TFile("Grid.LnnTreeOutput.root");
  TFile *f2 = new TFile("Grid.LnnHistOutput.root");
  TTree *t = (TTree*)f1->Get("stree");
  TList *l = (TList*)f2->Get("shist");

  l->At(0)->Draw("col"); gPad->SetLogz();

  t->Draw("TMath::Abs(TrigdEdx):TrigInnP","TPCNsigma<=3 & TPCNsigma>-3","same");

  // tuned paramatrizations
  TF1 *lower = new TF1("lower","(286.685/x^2+34.4211)*(1+  0.0140353*(exp(-15.4253*(x-1.03345)))/(1+exp(-15.4253*(x-1.03345)))  )",0.1,6);
  TF1 *upper = new TF1("upper","394.818/x^2+39.0215",0.1,6);
  TF1 *center = new TF1("center","(339.948/x^2+33.722)*(1+  0.0118361*(exp(9.26771*(x-1.73745)))/(1+exp(9.26771*(x-1.73745)))  )",0.1,6);

  lower->Draw("same");
  upper->Draw("same");
  center->Draw("same");

return;

TH2F *fHistdEdxAll = new TH2F("hdedx","TPC dEdx x P;P GeV/c;dEdx (a.u.)",  1600,-5,5,    1600,0,3000);

fHistdEdxAll->SetMarkerStyle(8);
fHistdEdxAll->SetMarkerSize(0.5);

  t->Draw("TMath::Abs(TrigdEdx):TrigInnP>>hdedx");
  t->Draw("TMath::Abs(TrigdEdx):TrigInnP>>hdedx","DcaV0Daughters<0.6*(1-DcaPionToPrimVertex/6) & DcaV0ToPrimVertex<0.3 & CosPointingAngle>0.9994 & DcaTrigToPrimVertex<0.6*(1-DcaV0Daughters/0.6) & TMath::Abs(DcaPionToPrimVertex)>0.1 & ITScls%100>=1 & ITScls/100>=0 & TPCcls%1000>=60 & TPCcls/1000>=60 & sqrt(PionPx^2 + PionPy^2)>=0.17 & sqrt(PionPx^2 + PionPy^2)<0.9 & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>=1 & TPCNsigma<3.000 & TPCNsigma>0 & ((TMath::Abs(TrigdEdx)>91 & (TOFmass<0 | TMath::Abs(TOFmass^2-7.929) < 3*0.363)) | (TMath::Abs(TrigdEdx)<=91 & TMath::Abs(TrigdEdx)>30 & sqrt(TrigPx^2 + TrigPy^2)<2.70 & TMath::Abs(TOFmass^2-7.929) < 3*0.363))& TMath::Abs(sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-3.006)<=0.004","");

}
