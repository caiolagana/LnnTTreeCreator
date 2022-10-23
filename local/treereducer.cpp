#include"HyperRecoDecay.h"

void treereducer()
{
  TFile *f = new TFile("treereduced.root", "recreate");

  TTree *oldtree = (TTree*)_file0->Get("stree");
  TTree *newtree = new TTree("reducedtree", "reducedtree");

  newtree= oldtree->CopyTree("CosPointingAngle>.996 & DcaV0ToPrimVertex<1 & DcaV0Daughters<1.0 & DcaPionToPrimVertex>0.1");
  //newtree= oldtree->CopyTree("InvMass<3.3 & OnTheFly==0");

  newtree->Print();
  f->cd();
  newtree->Write();
  f->Close();
}
