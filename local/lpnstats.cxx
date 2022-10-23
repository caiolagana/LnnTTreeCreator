#include"lpnsignificance.cxx"
void lpnstats()
{
  TFile *f = new TFile("lpntree.root", "READ");
  TTree *t = (TTree*)f->Get("ltree");
  TString cuts_string;

  //3H+pi cuts
/*  cuts_string.Append(" DcaV0Daughters<0.7*(1-DcaPionToPrimVertex/7)");
  cuts_string.Append(" & DcaV0ToPrimVertex<0.3");
  cuts_string.Append(" & CosPointingAngle>0.9992");
  cuts_string.Append(" & sqrt(PionPx^2 + PionPy^2)>=0.17");
  cuts_string.Append(" & sqrt(PionPx^2 + PionPy^2)<=0.35");
  cuts_string.Append(" & sqrt(TrigPx^2 + TrigPy^2)>=0.8");
  cuts_string.Append(" & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>=1");
  cuts_string.Append(" & sqrt( TrigPx^2 + TrigPy^2)<2.7");
*/  
  
  // 3He+pi cuts
  cuts_string.Append("DcaV0Daughters<0.6");
  cuts_string.Append(" & CosPointingAngle>0.998");
  cuts_string.Append(" & DcaV0ToPrimVertex<1.2");
  cuts_string.Append(" & DcaPionToPrimVertex>0.20");
  cuts_string.Append(" & sqrt( PionPx^2 + PionPy^2)>0.15");
  cuts_string.Append(" & sqrt( (PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>=1.4");
  
  
  cuts_string.Append(" & sqrt( TrigPx^2 + TrigPy^2 + TrigPz^2)<2.0");



  float xmin = 0;
  float xmax = 5;
  float bin_size  = 2.5e-3;
  int   nbins = int((xmax - xmin)/bin_size);

  //TCanvas *canvas = new TCanvas(); canvas->SetWindowSize(600,600); canvas->cd();
  cout << cuts_string.Data() << endl;

  TH1F *h_inv_mass = new TH1F("h_inv_mass", Form("h_inv_mass; M_{^{3}He#pi} (GeV/#it{c}^{2}); Counts / %1.1f MeV",bin_size*1e3), nbins, xmin, xmax);

  //remove gamma conversions
  //cuts_string.Append("& TMath::Abs(sqrt(.000511^2 + .000511^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .000511^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .000511^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz))))>0.1");

  //remove Lambda peak
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .93827^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .93827^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-1.115)>0.01");

  //remove Kaon peak
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .13957^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-0.493677)>0.01");

  //3He mass hypothesis (hypertriton)
  t->Draw("sqrt(.13957^2 + 2.80941^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt( (2*TrigPx)^2 + (2*TrigPy)^2 + (2*TrigPz)^2 + 2.80941^2) - (PionPx*2*TrigPx + PionPy*2*TrigPy + PionPz*2*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E same");

  //3H mass hypothesis (hyperneutron)
  //t->Draw("sqrt(.13957^2 + 2.80892^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt( (2*TrigPx)^2 + (2*TrigPy)^2 + (2*TrigPz)^2 + 2.80892^2) - (PionPx*2*TrigPx + PionPy*2*TrigPy + PionPz*2*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E");

  h_inv_mass->SetMarkerStyle(kFullCircle); h_inv_mass->SetMarkerSize(0.7); h_inv_mass->SetMarkerColor(kAzure+2); h_inv_mass->SetLineColor(kAzure+2);
  h_inv_mass->GetXaxis()->SetRangeUser(2.94,3.1);
  //h_inv_mass->GetYaxis()->SetRangeUser(0,60);
  //gPad->SetGridx(); gPad->SetGridy();

 lpnsignificance(h_inv_mass); 
}
