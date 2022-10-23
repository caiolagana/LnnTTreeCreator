void plotdedx()
{
  TFile *f1 = new TFile("15-06-05-Grid.LnnTreeOutput.root");
  TFile *f2 = new TFile("15-06-05-Grid.LnnHistOutput.root");
  TTree *t = (TTree*)f1->Get("stree");
  TList *l = (TList*)f2->Get("shist");

//  l->At(1)->Draw("col"); gPad->SetLogz();

//  t->Draw("TMath::Abs(TrigdEdx):TrigInnP","DcaV0Daughters<0.6*(1-DcaPionToPrimVertex/6) & DcaV0ToPrimVertex<0.3 & CosPointingAngle>0.9994 & DcaTrigToPrimVertex<0.6*(1-DcaV0Daughters/0.6) & TMath::Abs(DcaPionToPrimVertex)>0.1 & ITScls%100>=1 & ITScls/100>=0 & TPCcls%1000>=60 & TPCcls/1000>=60 & sqrt(PionPx^2 + PionPy^2)>=0.17 & sqrt(PionPx^2 + PionPy^2)<0.9 & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>=1 & TPCNsigma<3.000 & TPCNsigma>-3.000 & ((TMath::Abs(TrigdEdx)>91 & (TOFmass<0 | TMath::Abs(TOFmass^2-7.929) < 3*0.363)) | (TMath::Abs(TrigdEdx)<=91 & TMath::Abs(TrigdEdx)>30 & sqrt(TrigPx^2 + TrigPy^2)<2.70 & TMath::Abs(TOFmass^2-7.929) < 3*0.363))","same");

//select spurious point
TH2F *fHistdEdxAll = new TH2F("hdedx","TPC dEdx x P;P GeV/c;dEdx (a.u.)",  1600,-5,5,    1600,0,3000);

fHistdEdxAll->SetMarkerStyle(8);
fHistdEdxAll->SetMarkerSize(0.5);

  t->Draw("TMath::Abs(TrigdEdx):TrigInnP>>hdedx","DcaV0Daughters<0.6*(1-DcaPionToPrimVertex/6) & DcaV0ToPrimVertex<0.3 & CosPointingAngle>0.9994 & DcaTrigToPrimVertex<0.6*(1-DcaV0Daughters/0.6) & TMath::Abs(DcaPionToPrimVertex)>0.1 & ITScls%100>=1 & ITScls/100>=0 & TPCcls%1000>=60 & TPCcls/1000>=60 & sqrt(PionPx^2 + PionPy^2)>=0.17 & sqrt(PionPx^2 + PionPy^2)<0.9 & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>=1 & TPCNsigma<3.000 & TPCNsigma>0 & ((TMath::Abs(TrigdEdx)>91 & (TOFmass<0 | TMath::Abs(TOFmass^2-7.929) < 3*0.363)) | (TMath::Abs(TrigdEdx)<=91 & TMath::Abs(TrigdEdx)>30 & sqrt(TrigPx^2 + TrigPy^2)<2.70 & TMath::Abs(TOFmass^2-7.929) < 3*0.363))& TMath::Abs(sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-3.006)<=0.004","");

}
