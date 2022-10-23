void sideband(int dedx=-1,double lowerband=3.0, double upperband=3.0,int dedxsigma=110,double pmax=2.7)
{
  TFile *f = new TFile("15-06-05-Grid.LnnTreeOutput.root", "READ");
  TTree *t = (TTree*)f->Get("stree");
  t->Print();

  // cuts over V0's
  TString cuts_string;

  //geometry
  cuts_string.Append("DcaV0Daughters<0.7*(1-DcaPionToPrimVertex/7)");
  cuts_string.Append(" & DcaV0ToPrimVertex<0.3");
  cuts_string.Append(" & CosPointingAngle>0.9994");
  cuts_string.Append(" & DcaTrigToPrimVertex<0.8*(1-DcaV0Daughters/0.8)");

  cuts_string.Append(" & TMath::Abs(DcaPionToPrimVertex)>0.1");
  //cuts_string.Append(" & DecayLengthV0<10");

  //ITS cls
  cuts_string.Append(" & ITScls%100>=1");//pion ITS cls>=1
  cuts_string.Append(" & ITScls/100>=0");//trit ITS cls>=0

  //TPC ncls
  cuts_string.Append(" & TPCcls%1000>=60");//pion TPC cls>=60
  cuts_string.Append(" & TPCcls/1000>=60");//trit TPC cls>=60

  //momenta
  cuts_string.Append(" & sqrt(PionPx^2 + PionPy^2)>=0.17");
  cuts_string.Append(" & sqrt(PionPx^2 + PionPy^2)<0.35");
  cuts_string.Append(" & sqrt(TrigPx^2 + TrigPy^2)>=0.8");
  cuts_string.Append(" & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>=1");
  //cuts_string.Append(" & sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)<=2.0");

  //Armenteros-Podolanski cut
  //cuts_string.Append("& TMath::Abs(((((PionPx+TrigPx)*PionPx + (PionPy+TrigPy)*PionPy + (PionPz+TrigPz)*PionPz)/sqrt((PionPx+TrigPx)^2+(PionPy+TrigPy)^2+(PionPz+TrigPz)^2)   -    ((PionPx+TrigPx)*TrigPx + (PionPy+TrigPy)*TrigPy + (PionPz+TrigPz)*TrigPz)/sqrt((PionPx+TrigPx)^2+(PionPy+TrigPy)^2+(PionPz+TrigPz)^2))/( ((PionPx+TrigPx)*PionPx + (PionPy+TrigPy)*PionPy + (PionPz+TrigPz)*PionPz)/sqrt((PionPx+TrigPx)^2+(PionPy+TrigPy)^2+(PionPz+TrigPz)^2)   +    ((PionPx+TrigPx)*TrigPx + (PionPy+TrigPy)*TrigPy + (PionPz+TrigPz)*TrigPz)/sqrt((PionPx+TrigPx)^2+(PionPy+TrigPy)^2+(PionPz+TrigPz)^2)   )))>0.6");
  //cuts_string.Append("& PtArmV0<0.15");


  //particle identification
  cuts_string.Append(Form(" & TPCNsigma<%1.3f & TPCNsigma>-%1.3f",upperband,lowerband));


  //TOF&TPC tritmass=7.92937    |||||    tritsigma=0.363387
  if (dedx==-1) dedx=91;
  cuts_string.Append(Form(" & ((TMath::Abs(TrigdEdx)>%d & (TOFmass<0 | TMath::Abs(TOFmass^2-7.929) < %d*0.363)) | (TMath::Abs(TrigdEdx)<=%d & TMath::Abs(TrigdEdx)>30 & sqrt(TrigPx^2 + TrigPy^2)<%1.2f & TMath::Abs(TOFmass^2-7.929) < %d*0.363))",dedx,3,dedx,pmax,3));
  //cuts_string.Append(" & TMath::Abs(TrigdEdx)<=91");
  //cuts_string.Append(" & TMath::Abs(TrigdEdx)>91");

  //remove gamma conversions
  //cuts_string.Append("& TMath::Abs(sqrt(.000511^2 + .000511^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .000511^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .000511^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz))))>0.1");

  //remove Lambda peak
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .93827^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .93827^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-1.115)>0.01");

  //remove Kaon peak
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + .13957^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-0.493677)>0.01");

  //select central invariant mass window
  cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-2.99326)<3*2.10682e-03");

  //select right-side IM window
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-2.99326-6*2.10682e-03)<3*2.10682e-03");

  //select left-side IM window
  //cuts_string.Append("& TMath::Abs(sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))-2.99326+6*2.10682e-03)<3*2.10682e-03");

  //=== triton charge ===
  //cuts_string.Append(" & TrigInnP>0"); 
  //cuts_string.Append(" & TrigInnP<0"); 

  cout << cuts_string.Data() << endl;

  float xmin = 0;
  float xmax = 5;
  float bin_size  = 2.5e-3;
  int   nbins = (int)((xmax - xmin)/bin_size);
  cout << nbins << " bins from " << xmin << " to " << xmax << endl;
  cout << "bin size " << bin_size*1000 << " MeV" << endl;

  //TCanvas *canvas = new TCanvas(); canvas->SetWindowSize(600,600); canvas->cd();//square canvas

  TH1F *h_inv_mass = new TH1F("h_inv_mass", "h_inv_mass", nbins, xmin, xmax);

  //proton mass hypothesis (Lambda's)
  //t->Draw("sqrt(.13957^2 + .93827^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .93827^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "same");

  //pion mass hypothesis (K0's)
  //t->Draw("sqrt(.13957^2 + .13957^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .13957^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E");

  //electron mass hypothesis (gammas)
  //t->Draw("sqrt(.000511^2 + .000511^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .000511^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + .000511^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E");

  //Triton mass hypothesis
  t->Draw("sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", cuts_string.Data(), "E same");

  //no V0-geometric cuts
  //t->Draw("sqrt(.13957^2 + 2.808921^2 + 2*(sqrt(PionPx^2 + PionPy^2 + PionPz^2 + .13957^2)*sqrt(TrigPx^2 + TrigPy^2 + TrigPz^2 + 2.808921^2) - (PionPx*TrigPx + PionPy*TrigPy + PionPz*TrigPz)))>>h_inv_mass", "sqrt((PionPx+TrigPx)^2 + (PionPy+TrigPy)^2)>1 & sqrt(PionPx^2 + PionPy^2)<0.9 & sqrt(PionPx^2 + PionPy^2)>=0.17 & ITScls%100>0 & ITScls/100>0", "E same");

  h_inv_mass->SetMarkerStyle(kFullCircle); h_inv_mass->SetMarkerSize(0.7); h_inv_mass->SetMarkerColor(kAzure+2); h_inv_mass->SetLineColor(kAzure+2);
  h_inv_mass->GetXaxis()->SetRangeUser(0,3.1); h_inv_mass->GetXaxis()->SetTitle("M_{^{3}H#pi} (GeV/#it{c}^{2})");
  h_inv_mass->GetYaxis()->SetTitle(Form("Counts / %1.1f MeV",bin_size*1e3));
  h_inv_mass->GetYaxis()->SetRangeUser(0,140);





}
