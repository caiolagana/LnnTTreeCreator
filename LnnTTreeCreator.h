#ifndef LnnTTreeCreator_H
#define LnnTTreeCreator_H

#include <TChain.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TList.h>
#include <TH1.h>
#include <TF1.h>
#include <TClonesArray.h>

#include "TMath.h"

#include "AliAnalysisTaskSE.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliLog.h"
#include "AliAODTrack.h"
#include "AliAODInputHandler.h"
#include "AliAODHeader.h"

class LnnTTreeCreator : public AliAnalysisTaskSE {
 public:
  LnnTTreeCreator() : AliAnalysisTaskSE(),
  fAOD(0),
  fOutputList(0),
  fOutputTree(0),
  track(0),
  tr1(0),
  tr2(0),
  tritTrack(0),
  pionTrack(0),
  v0(0),
  //fPIDResponse(0),
  aodCentrality(0),
  Nch(0),
  chargeProduct(0)
  {}

  LnnTTreeCreator(const char *name);
  virtual ~LnnTTreeCreator();
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t *option);
  virtual void   Terminate(Option_t *);
  bool PassCuts(AliAODTrack*);
  bool IsPionCandidate(AliAODTrack*);
  bool IsTritCandidate(AliAODTrack*);
  Double_t GetTOFmass(AliAODTrack*);
  Double_t GetTritNsigma(AliAODTrack*);

 private:
  AliAODEvent    *fAOD;        //! AOD object
  TList          *fOutputList; //! Output list
  TTree          *fOutputTree; //! Output tree
  AliAODTrack    *track;       //! general track
  AliAODTrack    *tr1;         //! first  daughter of V0
  AliAODTrack    *tr2;         //! second daughter of V0
  AliAODTrack    *tritTrack;   //! tritium track from V0
  AliAODTrack    *pionTrack;   //! pion track from V0
  AliAODv0       *v0;          //! V0
  AliAODVertex   *prim_vtx;    //! event primary vertex
  //AliPIDResponse *fPIDResponse;//! pid response
  AliCentrality *aodCentrality;//! event centrality
  Int_t Nch;                   //!number of charged particles
  TF1 *center;                 //!central dedx line for 3H
  TF1 *lower;                  //!upper dedx line for 3H
  TF1 *upper;                  //!lower dedx line for 3H
  int chargeProduct;           //!flag matter

  /// \cond CLASSDEF
  ClassDef(LnnTTreeCreator, 1);
  /// \endcond
};

#endif
