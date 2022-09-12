#include "TChain.h"
#include "TTree.h"
#include "TList.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TF1.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"

#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisTaskSE.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliESDEvent.h"
#include "AliESDInputHandler.h"
#include "AliAODEvent.h"
#include "AliAODHeader.h"
#include "AliVEvent.h"
#include "AliCentrality.h"
#include "AliAODHandler.h"
#include "AliInputEventHandler.h"
#include "AliESDpid.h"
#include "AliESDtrackCuts.h"
#include "AliTPCPIDResponse.h"
#include "AliAnalysisFilter.h"
#include "AliAODv0.h"
#include "LnnTTreeCreator.h"
#include "AliPIDResponse.h"

class LnnTTreeCreator;

using namespace std;

ClassImp(LnnTTreeCreator)

//
float pmass = .93827;
float nmass = .93956;
float mtrit = 2.808921;//2*nmass + pmass - .14949; correction thanks to Benjamin
float m3he  = 2.80941;//2*pmass + nmass - .14931;
float mpion = .13957;
  Double_t prim_vtx_position[3];//!prim vtx
  Double_t d0z0[2], cov[3];    //!DCA propagation

typedef struct HyperRecoDecay
{
  float CosPointingAngle;
  float DcaPionToPrimVertex;
  float DcaTrigToPrimVertex;
  float DcaV0Daughters;
  float DcaV0ToPrimVertex;
  float DecayLengthV0;
  float PtArmV0;
  float PionPx;
  float PionPy;
  float PionPz;
  float TrigPx;
  float TrigPy;
  float TrigPz;
  Int_t ITScls;
  Int_t TPCcls;
  float TPCNsigma;
  float TOFmass;
  float TrigdEdx;
  float TrigInnP;
};

HyperRecoDecay shrd;

//________________________________________________________________________
LnnTTreeCreator::LnnTTreeCreator(const char *name) : AliAnalysisTaskSE(name),
    fAOD(0),
    fOutputList(0),
    fOutputTree(0),
    track(0),
    tr1(0),
    tr2(0),
    tritTrack(0),
    pionTrack(0),
    v0(0),
    fPIDResponse(0),
    aodCentrality(0),
    Nch(0),
    center(0),
    lower(0),
    upper(0),
    chargeProduct(0),
    prim_vtx(0)
{
  // Input slot #0 works with a TChain
  DefineInput(0, TChain::Class());
  // Output slot #1 writes into a TH1 container
  DefineOutput(1, TList::Class());
  // Output slot #2 for Ntuple
  DefineOutput(2, TTree::Class());
}

LnnTTreeCreator::~LnnTTreeCreator()
{
  // Destructor
  if (fAOD) delete fAOD;
  if (fOutputList) delete fOutputList; 
  if (fOutputTree) delete fOutputTree;
  if (fPIDResponse) delete fPIDResponse;
  if (aodCentrality) delete aodCentrality;
  if (tritTrack) delete tritTrack;
  if (pionTrack) delete pionTrack;
  if (tr1) delete tr1;
  if (tr2) delete tr2;
  if (track) delete track;
  if (v0) delete v0;
  if (center) delete center;
  if (lower) delete lower;
  if (upper) delete upper;
  if (prim_vtx) delete prim_vtx;
}

//________________________________________________________________________
void LnnTTreeCreator::UserCreateOutputObjects()
{
  OpenFile(2); // Needed in case the object should be connected to the output file (trees)

  // Get PIDResponse object. Used for TOF PID for dEdx<=90
  fPIDResponse = ((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->GetPIDResponse();

  // Output list
  fOutputList = new TList();
  fOutputList->SetOwner();

  // Output histograms
  // dEdx
  TH2F *fHistdEdxAll = new TH2F("fHistdEdxAll","TPC dEdx x P;P GeV/c;dEdx (a.u.)",  1600,-5,5,    1600,0,3000);
  fOutputList->Add(fHistdEdxAll);
  // TOF-cleaned dEdx
  TH2F *fHistdEdxTOF = new TH2F("fHistdEdxTOF","TPC dEdx x P;P GeV/c;dEdx (a.u.)",  1600,-5,5,    1600,0,3000);
  fOutputList->Add(fHistdEdxTOF);
  // ITS-cleaned dEdx
  TH2F *fHistdEdxITS = new TH2F("fHistdEdxITS","TPC dEdx x P;P GeV/c;dEdx (a.u.)",  1600,-5,5,    1600,0,3000);
  fOutputList->Add(fHistdEdxITS);
  // TOFITS-cleaned dEdx
  TH2F *fHistdEdxTOFITS = new TH2F("fHistdEdxTOFITS","TPC dEdx x P;P GeV/c;dEdx (a.u.)",  1600,-5,5,    1600,0,3000);
  fOutputList->Add(fHistdEdxTOFITS);
  // Nch
  TH1F *fHistNch = new TH1F("fHistNch","fHistNch", 20000, 0, 10000);
  fOutputList->Add(fHistNch);

  // tuned paramatrizations
  lower = new TF1("lower","(286.685/x^2+34.4211)*(1+  0.0140353*(exp(-15.4253*(x-1.03345)))/(1+exp(-15.4253*(x-1.03345)))  )",0.1,6);
  upper = new TF1("upper","394.818/x^2+39.0215",0.1,6);
  center = new TF1("center","(339.948/x^2+33.722)*(1+  0.0118361*(exp(9.26771*(x-1.73745)))/(1+exp(9.26771*(x-1.73745)))  )",0.1,6);

  // Output TTree
  TString LeafString;
  LeafString.Append("CosPointingAngle/F:");
  LeafString.Append("DcaPionToPrimVertex/F:");
  LeafString.Append("DcaTrigToPrimVertex/F:");
  LeafString.Append("DcaV0Daughters/F:");
  LeafString.Append("DcaV0ToPrimVertex/F:");
  LeafString.Append("DecayLengthV0/F:");
  LeafString.Append("PtArmV0/F:");
  LeafString.Append("PionPx/F:");
  LeafString.Append("PionPy/F:");
  LeafString.Append("PionPz/F:");
  LeafString.Append("TrigPx/F:");
  LeafString.Append("TrigPy/F:");
  LeafString.Append("TrigPz/F:");
  LeafString.Append("ITScls/I:");
  LeafString.Append("TPCcls/I:");
  LeafString.Append("TPCNsigma/F:");
  LeafString.Append("TOFmass/F:");
  LeafString.Append("TrigdEdx/F:");
  LeafString.Append("TrigInnP/F");
  cout << "creating shrd tree: " << LeafString.Data() << endl;
  fOutputTree = new TTree("stree", "stree");
  fOutputTree->Branch("HyperRecoDecays", &shrd, LeafString.Data());

  PostData(1, fOutputList);
  PostData(2, fOutputTree);
} 

// Standard track cuts
bool LnnTTreeCreator::PassCuts(AliAODTrack *tr)
{
  bool pass = true;

  // TPC refit
  ULong_t status = tr->GetStatus();
  if ((status&AliESDtrack::kTPCrefit)==0) pass = false;

  // kink daughter
  AliAODVertex *vtx = (AliAODVertex*)tr->GetProdVertex();
  Int_t vtyp=(Int_t)vtx->GetType();
  Int_t ttyp=(Int_t)tr->GetType();
  if (ttyp==AliAODTrack::kFromDecayVtx && vtyp==AliAODVertex::kKink) pass = false;

  // default cuts
  if (tr->GetTPCNcls() < 60) pass = false;//TPCnlcs is stored in the final TTree so >80 can be applied
  if (tr->Chi2perNDF() > 5.) pass = false;
  if (fabs(tr->Eta()) > 0.9) pass = false;
  if (tr->P() < 0.15)        pass = false;
  if (!tr->GetDetPid())      pass = false;//otherwise crashes...

  return pass;
}

bool LnnTTreeCreator::IsPionCandidate(AliAODTrack *tr)
{
  AliPIDResponse::EDetPidStatus statusTPC;
  Double_t nSigmaTPC = -999;
  statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC, tr, AliPID::kPion, nSigmaTPC);
  bool z = false;
  if (statusTPC == AliPIDResponse::kDetPidOk && TMath::Abs(nSigmaTPC) <= 3) z = true;

  return z;
}

Double_t LnnTTreeCreator::GetTOFmass(AliAODTrack *tr)
{
  Double_t m = -987;//mass
  Double_t b = -999;//beta
  Double_t trackLeng = 0;
  Double_t tofSignal = 0;

  if ((tr->GetStatus() & AliVTrack::kTOFout) == AliVTrack::kTOFout)
  {
    trackLeng = tr->GetIntegratedLength();
    tofSignal = tr->GetDetPid()->GetTOFsignal();
    b = (trackLeng/100.)/((TMath::C()*tofSignal)/1e12);
    if (b > 0 && b < 1) m = (tr->P())*sqrt(pow(b,-2)-1);
  }

  return m;
}

Double_t LnnTTreeCreator::GetTritNsigma(AliAODTrack *tr)
{
  Double_t dEdx = tr->GetDetPid()->GetTPCsignal();
  Double_t p = tr->GetDetPid()->GetTPCmomentum();
  Double_t nsigma=-99;

  if (dEdx>=center->Eval(p)) nsigma = 2.9*(dEdx - center->Eval(p))/(upper->Eval(p) - center->Eval(p));
  if (dEdx<center->Eval(p))  nsigma = 2.2*(dEdx - center->Eval(p))/(center->Eval(p) - lower->Eval(p));

  return nsigma;
}

// hybrid PID for tritiums (TPC & TOF)
bool LnnTTreeCreator::IsTritCandidate(AliAODTrack *tr)
{
  // variables
  bool z = false;
  bool isTPCtrit = false;
  bool isTOFtrit = false;
  Double_t TOFmass = GetTOFmass(tr);
  Double_t dEdx = tr->GetDetPid()->GetTPCsignal();

  // TPC tuned nsigma
  if (GetTritNsigma(tr)>=-3.5 && GetTritNsigma(tr)<=4.5) isTPCtrit = true;

  // cut in TOF mass
  if (TOFmass > 2.35 && TOFmass < 3.25) isTOFtrit = true;

  // final conditions
  if (tr->GetDetPid()->GetTPCmomentum() > 0.6 && isTPCtrit == true && (dEdx > 91 || (dEdx <= 91 && isTOFtrit == true))) z = true;

  return z;
}

//________________________________________________________________________
void LnnTTreeCreator::UserExec(Option_t *) 
{
  // Main loop
  // Called for each event

  // Get AOD event
  fAOD = dynamic_cast<AliAODEvent*>(InputEvent()); if (!fAOD) return;

  // General stuff
  Nch = 0;
  prim_vtx = fAOD->GetPrimaryVertex();
  prim_vtx->GetXYZ(prim_vtx_position);

  // Centrality
  //aodCentrality = fAOD->GetCentrality();
  //Float_t centralityPercentile = -1;
  //centralityPercentile = aodCentrality->GetCentralityPercentile("V0M");

  //loop over tracks for dE/dx and Nch distributions
  for (int i = 0; i < fAOD->GetNumberOfTracks(); i++)
  {
    track = (AliAODTrack*)fAOD->GetTrack(i);
    if (PassCuts(track))
    {
      Nch++;
      ((TH2F*)(fOutputList->FindObject("fHistdEdxAll")))->Fill(track->Charge()*track->GetDetPid()->GetTPCmomentum(), track->GetDetPid()->GetTPCsignal());
      if (track->GetITSNcls()>0) {((TH2F*)(fOutputList->FindObject("fHistdEdxITS")))->Fill(track->Charge()*track->GetDetPid()->GetTPCmomentum(), track->GetDetPid()->GetTPCsignal());}

      //TMath::Abs(TOFmass^2-7.929) < 3*0.363
      if (TMath::Abs(pow(GetTOFmass(track),2)-7.929)<3*0.363){
      ((TH2F*)(fOutputList->FindObject("fHistdEdxTOF")))->Fill(track->Charge()*track->GetDetPid()->GetTPCmomentum(), track->GetDetPid()->GetTPCsignal());
      if (track->GetITSNcls()>0){
      ((TH2F*)(fOutputList->FindObject("fHistdEdxTOFITS")))->Fill(track->Charge()*track->GetDetPid()->GetTPCmomentum(), track->GetDetPid()->GetTPCsignal());}
    }
  }
}
  ((TH1F*)(fOutputList->FindObject("fHistNch")))->Fill(Nch);

  //loop over v0's
  for (int i = 0; i < fAOD->GetNumberOfV0s(); i++)
  {
    // Get V0 and its positive and negative daughters from input AOD.
    v0 = fAOD->GetV0(i); if (!v0) continue;
    tr1 = (AliAODTrack*)v0->GetDaughter(0); if (!tr1) continue;
    tr2 = (AliAODTrack*)v0->GetDaughter(1); if (!tr2) continue;

    // Offline v0's only
    if (v0->GetOnFlyStatus() == true) continue;

    // Filter loose candidates
    if (v0->CosPointingAngle(prim_vtx) < 0.998 || v0->DcaV0Daughters() > 1.0 || v0->DcaV0ToPrimVertex() > 1.0) continue;

    // Require both daughters to pass cuts
    if (!(PassCuts(tr1) && PassCuts(tr2))) continue;

    // Ask for 3H+pi daughters
    if (IsTritCandidate(tr1) && IsPionCandidate(tr2)) {
      tritTrack = tr1;
      pionTrack = tr2;
    } else if (IsTritCandidate(tr2) && IsPionCandidate(tr1)) {
      tritTrack = tr2;
      pionTrack = tr1;
    } else { continue; }

    // Opposite-charge or like-charge daughters
    chargeProduct = tritTrack->Charge()*pionTrack->Charge();

    //DCA propagation
    track = (AliAODTrack*)tritTrack->Clone(); //clone tritium track for DCA propagation
    track->PropagateToDCA(prim_vtx, fAOD->GetMagneticField(), 20., d0z0, cov);
    if (TMath::Abs(d0z0[1]) >= 1.0) continue;

    // Fill output TTree with the candidate relevant information
    shrd.CosPointingAngle = v0->CosPointingAngle(prim_vtx);
    shrd.DcaV0Daughters = v0->DcaV0Daughters();
    shrd.DcaV0ToPrimVertex = v0->DcaV0ToPrimVertex();
    shrd.DecayLengthV0 = v0->DecayLengthV0(prim_vtx_position);
    shrd.PtArmV0 = v0->PtArmV0();
    shrd.TrigdEdx = chargeProduct*tritTrack->GetDetPid()->GetTPCsignal();
    shrd.TrigInnP = tritTrack->Charge()*tritTrack->GetDetPid()->GetTPCmomentum();
    shrd.TOFmass  = GetTOFmass(tritTrack);
    shrd.ITScls = pionTrack->GetITSNcls() + 100*tritTrack->GetITSNcls();
    shrd.TPCcls = pionTrack->GetTPCNcls() + 1000*tritTrack->GetTPCNcls();
    shrd.TPCNsigma = GetTritNsigma(tritTrack);
    //shrd.Centrality = centralityPercentile;
    if (pionTrack->Charge() == 1)
    {
      shrd.DcaPionToPrimVertex = v0->DcaPosToPrimVertex();
      shrd.DcaTrigToPrimVertex = v0->DcaNegToPrimVertex();

      shrd.PionPx = v0->MomPosX();
      shrd.PionPy = v0->MomPosY();
      shrd.PionPz = v0->MomPosZ();

      shrd.TrigPx = v0->MomNegX();
      shrd.TrigPy = v0->MomNegY();
      shrd.TrigPz = v0->MomNegZ();
    }
    else
    {
      shrd.DcaPionToPrimVertex = v0->DcaNegToPrimVertex();
      shrd.DcaTrigToPrimVertex = v0->DcaPosToPrimVertex();
      
      shrd.PionPx = v0->MomNegX();
      shrd.PionPy = v0->MomNegY();
      shrd.PionPz = v0->MomNegZ();
      
      shrd.TrigPx = v0->MomPosX();
      shrd.TrigPy = v0->MomPosY();
      shrd.TrigPz = v0->MomPosZ();
    }

    fOutputTree->Fill();
  } //end of v0 loop

  PostData(1, fOutputList);
  PostData(2, fOutputTree);
}      

//________________________________________________________________________
void LnnTTreeCreator::Terminate(Option_t *) 
{
  // Called once at the end of the query
}
