#include"significance.cxx"
void stats(int dedx=-1,double lowerband=3.0, double upperband=3.0,double pmax=2.7)
{
  TFile *o = new TFile(Form("out.stats_%1.0f_%1.0f_%1.1f.root",lowerband,upperband,pmax),"RECREATE");
  TFile *f = new TFile("Grid.LnnTreeOutput.root", "READ");
  //TFile *fbg = new TFile("ESD/Grid.LnnTreeOutput.kCentralkSemiCentral.root", "READ");
  //TFile *fbg = new TFile("ESD/Grid.LnnTreeOutputTPC60.root", "READ");
  TTree *t = (TTree*)f->Get("stree");
  //TTree *tbg = (TTree*)fbg->Get("stree");
  t->Print();

  // cuts over V0's
  TString cuts_string;

  //geometry
  cuts_string.Append("DcaV0Daughters<0.7*(1-DcaPionToPrimVertex/7)");
  cuts_string.Append(" & DcaV0ToPrimVertex<0.3");
  cuts_string.Append(" & CosPointingAngle>0.9994");
  //cuts_string.Append(" & DcaTrigToPrimVertex<0.8*(1-DcaV0Daughters/0.8)");
  
  //cuts_string.Append(" & TMath::Abs(DcaPionToPrimVertex)>0.1");
  //cuts_string.Append(" & DecayLengthV0>1.0");

  //ITS cls
  cuts_string.Append(" & ITScls%100>=1");//pion ITS cls>=1
  cuts_string.Append(" & ITScls/100>=0");//trit ITS cls>=0

  //TPC ncls
  cuts_string.Append(" & TPCcls%1000>=60");//pion TPC cls>=60
  cuts_string.Append(" & TPCcls/1000>=60");//trit TPC cls>=60

  //kinematics
  cuts_string.Append(" & sqrt(PionPx^2 + PionPy^2)>=0.17");
  cuts_string.Append(" & sqrt(PionPx^2 + PionPy^2)<=0.35");
  cuts_string.Append(" & sqrt(TrigPx^2 + TrigPy^2)>=0.8");
  cuts_string.Append(" & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>=1");
  //cuts_string.Append(" & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)<=2.0");

  //select triton momentum window
  //cuts_string.Append(" & (sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2)<1.5 | sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2)>3)");
  //cuts_string.Append(Form(" & TMath::Abs(TrigInnP)<%1.2f",pmax));
  //cuts_string.Append(Form(" & sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2)<%1.2f",2.0));

  //Armenteros-Podolanski cut
  //cuts_string.Append("& TMath::Abs(((((PionPx+TrigPx)*PionPx + (PionPy+TrigPy)*PionPy + (PionPz+TrigPz)*PionPz)/sqrt((PionPx+TrigPx)^2+(PionPy+TrigPy)^2+(PionPz+TrigPz)^2)   -    ((PionPx+TrigPx)*TrigPx + (PionPy+TrigPy)*TrigPy + (PionPz+TrigPz)*TrigPz)/sqrt((PionPx+TrigPx)^2+(PionPy+TrigPy)^2+(PionPz+TrigPz)^2))/( ((PionPx+TrigPx)*PionPx + (PionPy+TrigPy)*PionPy + (PionPz+TrigPz)*PionPz)/sqrt((PionPx+TrigPx)^2+(PionPy+TrigPy)^2+(PionPz+TrigPz)^2)   +    ((PionPx+TrigPx)*TrigPx + (PionPy+TrigPy)*TrigPy + (PionPz+TrigPz)*TrigPz)/sqrt((PionPx+TrigPx)^2+(PionPy+TrigPy)^2+(PionPz+TrigPz)^2)   )))>0.6");
  //cuts_string.Append("& PtArmV0<0.15");


  //particle identification
  cuts_string.Append(Form(" & TPCNsigma<=%1.3f & TPCNsigma>=-%1.3f",upperband,lowerband));


  //TOF&TPC tritmass=7.92937    |||||    tritsigma=0.363387
  //dEdx-based separation
  if (dedx==-1) dedx=91;
  cuts_string.Append(Form(" & ((TMath::Abs(TrigdEdx)>%d & (TOFmass<0 | TMath::Abs(TOFmass^2-7.929) < %d*0.363)) | (TMath::Abs(TrigdEdx)<=%d & TMath::Abs(TrigdEdx)>30 & sqrt(TrigPx^2 + TrigPy^2)<%1.2f & TMath::Abs(TOFmass^2-7.929) < %d*0.363))",dedx,3,dedx,pmax,3));
  //cuts_string.Append(" & TMath::Abs(TrigdEdx)<=91");
  //cuts_string.Append(" & TMath::Abs(TrigdEdx)>91");

  //momentum-based separation
  //cuts_string.Append(Form("& (TMath::Abs(TrigInnP)<=2 | (TMath::Abs(TrigInnP)>2 && TMath::Abs(TOFmass^2-7.929) < 3*0.363)) & & sqrt(TrigPx^2 + TrigPy^2)<%1.2f",pmax));


  //remove gamma conversions
  //cuts_string.Append("& TMath::Abs(sqrt(.000511^2 + .000511^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .000511^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .000511^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz))))>0.1");

  //remove Lambda peak
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .93827^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .93827^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-1.115)>0.01");

  //remove Kaon peak
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .13957^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-0.493677)>0.01");

  //=== triton charge ===
  //cuts_string.Append(" & TrigInnP>0");
  //cuts_string.Append(" & TrigInnP<0");

  //select Lambda left window
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .93827^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .93827^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-1.1025)<=0.0075");
  //select Lambda central window
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .93827^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .93827^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-1.1175)<=0.0075");
  //select Lambda right window
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .93827^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .93827^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-1.1325)<=0.0075");

  //select Kaon left window
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .13957^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-0.4676)<=0.015");
  //select Kaon central window
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .13957^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-0.4976)<=0.015");
  //select Kaon right window
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .13957^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-0.5276)<=0.015");
  //   fk = new TF1("f","80*(1-1.3*x)",0.42,0.56)   for binsize 2.5 MeV
  // counts in central window BG: 8.47488e+02

  //select spurious point
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-3.006)<=0.004");

  cout << cuts_string.Data() << endl;

  float xmin = 0;
  float xmax = 5;
  float bin_size  = 2.5e-3;
  int   nbins = (int)((xmax - xmin)/bin_size);
  cout << nbins << " bins from " << xmin << " to " << xmax << endl;
  cout << "bin size " << bin_size*1000 << " MeV" << endl;

  TCanvas *canvas = new TCanvas(); canvas->SetWindowSize(600,600); canvas->cd();//square canvas

  TH1F *h_inv_mass = new TH1F("h_inv_mass", Form("%1.1f < n#sigma_{TPC} (^{3}H) < %1.1f",-lowerband,upperband), nbins, xmin, xmax);

  //proton mass hypothesis (Lambda's)
  //t->Draw("sqrt(.13957^2 + .93827^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .93827^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E same");

  //pion mass hypothesis (K0's)
  //t->Draw("sqrt(.13957^2 + .13957^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "");

  //electron mass hypothesis (gammas)
  //t->Draw("sqrt(.000511^2 + .000511^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .000511^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .000511^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E");

  //Triton mass hypothesis
  t->Draw("sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E same");

  //Triton <-> pion inverted
  //t->Draw("sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + 2.808921^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E same");

  //no V0-geometric cuts
  //t->Draw("sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", "sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>1 & sqrt(PionPx^2 + PionPy^2)<0.9 & sqrt(PionPx^2 + PionPy^2)>=0.17 & ITScls%100>0 & ITScls/100>0", "E same");

  h_inv_mass->SetMarkerStyle(kFullCircle); h_inv_mass->SetMarkerSize(0.7); h_inv_mass->SetMarkerColor(kAzure+2); h_inv_mass->SetLineColor(kAzure+2);
  h_inv_mass->GetXaxis()->SetRangeUser(2.94,3.1);
  h_inv_mass->GetXaxis()->SetTitle("M_{^{3}H#pi} (GeV/#it{c}^{2})");
  h_inv_mass->GetYaxis()->SetTitle(Form("Counts / %1.1f MeV",bin_size*1e3));
  h_inv_mass->GetYaxis()->SetRangeUser(0,180);

  return;
  significance(h_inv_mass);

  
//return;
  
  /*

  
  TString cuts_bg;
  cuts_bg.Append("DcaV0Daughters<0.7*(1-DcaPionToPrimVertex/7)");
  cuts_bg.Append(" & DcaV0ToPrimVertex<0.3");
  cuts_bg.Append(" & CosPointingAngle>0.9994");
  //cuts_bg.Append(" & DcaTrigToPrimVertex<0.8*(1-DcaV0Daughters/0.8)");
  //cuts_bg.Append(" & TMath::Abs(DcaPionToPrimVertex)>0.1");
  cuts_bg.Append(" & ITScls%100>=1");//pion ITS cls>=1
  cuts_bg.Append(" & ITScls/100>=0");//trit ITS cls>=0
  cuts_bg.Append(" & sqrt(PionPx^2 + PionPy^2)>=0.175");
  cuts_bg.Append(" & sqrt(PionPx^2 + PionPy^2)<=0.35");
  cuts_bg.Append(" & sqrt(TrigPx^2 + TrigPy^2)>=0.8");
  cuts_bg.Append(" & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>=1");
  //=== triton charge ===
  //cuts_bg.Append(" & TrigInnP>0");
  //cuts_bg.Append(" & TrigInnP<0");
  cuts_bg.Append(Form(" & TPCNsigma<%1.3f & TPCNsigma>-%1.3f",upperband,lowerband));
  if (dedx==-1) dedx=91;
  cuts_bg.Append(Form(" & ((TMath::Abs(TrigdEdx)>%d & (TOFmass<0 | TMath::Abs(TOFmass^2-7.929) < %d*0.363)) | (TMath::Abs(TrigdEdx)<=%d & TMath::Abs(TrigdEdx)>30 & sqrt(TrigPx^2 + TrigPy^2)<%1.2f & TMath::Abs(TOFmass^2-7.929) < %d*0.363))",dedx,3,dedx,pmax,3));  
  
 
tbg->Draw("sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))", cuts_bg.Data(), "same");

//return;

o->cd();
//canvas->Write();
h_inv_mass->Write();
o->Close();
*/

}
