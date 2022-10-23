#include "LnnTTreeCreator.h"
#include "AliAnalysisTaskPIDResponse.h"

#ifdef __CLING__
    R__ADD_INCLUDE_PATH($ALICE_ROOT)
    #include <ANALYSIS/macros/AddTaskPIDResponse.C>
#endif

#ifdef __ECLIPSE_IDE
    extern AliAnalysisTask *AddTaskPIDResponse(Bool_t, Bool_t,Bool_t,Int_t);
#endif

void runAnalysis()
{
    // Load common libraries
   gSystem->Load("libCore.so");  
   gSystem->Load("libTree.so");
   gSystem->Load("libGeom.so");
   gSystem->Load("libVMC.so");
   gSystem->Load("libPhysics.so");
   gSystem->Load("libMinuit.so"); 
   gSystem->Load("libGui.so");
   gSystem->Load("libXMLParser.so");
   gSystem->Load("libSTEERBase.so");
   gSystem->Load("libESD.so");
   gSystem->Load("libCDB.so");
   gSystem->Load("libAOD.so");
   gSystem->Load("libANALYSIS.so");
   gSystem->Load("libANALYSISalice.so");
    
    // set if you want to run the analysis locally (kTRUE), or on grid (kFALSE)
    Bool_t local = kFALSE;
    // if you run on grid, specify test mode (kTRUE) or full grid model (kFALSE)
    Bool_t gridTest = kFALSE;

        // since we will compile a class, tell root where to look for headers  
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->ProcessLine(".include $ROOTSYS/include");
    gInterpreter->ProcessLine(".include $ALICE_ROOT/include");
#else
    gROOT->ProcessLine(".include $ROOTSYS/include");
    gROOT->ProcessLine(".include $ALICE_ROOT/include");
#endif
    //gROOT->ProcessLine(".include $ALICE_ROOT/ANALYSIS/");
     
    // create the analysis manager
    AliAnalysisManager *mgr = new AliAnalysisManager("AliAnalysisTTreeCreator");
    AliAODInputHandler *aodH = new AliAODInputHandler();
    mgr->SetInputEventHandler(aodH);


#ifndef __CLING__
//load external macros by LoadMacro only in root5
      gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
#endif
  AliAnalysisTaskPIDResponse *pidTask = AddTaskPIDResponse(kFALSE);


    // compile the class and load the add task macro
    // here we have to differentiate between using the just-in-time compiler
    // from root6, or the interpreter of root5
#if !defined (__CINT__) || defined (__CLING__)
    gInterpreter->LoadMacro("LnnTTreeCreator.cxx++g");
    LnnTTreeCreator *task = reinterpret_cast<LnnTTreeCreator*>(gInterpreter->ExecuteMacro("AddMyTask.C"));
#else
    gROOT->LoadMacro("LnnTTreeCreator.cxx++g");
    gROOT->LoadMacro("AddMyTask.C");
    LnnTTreeCreator *task = AddMyTask();
#endif


    if (!mgr->InitAnalysis()) return;
    mgr->SetDebugLevel(2);
    mgr->PrintStatus();
    mgr->SetUseProgressBar(1, 25);

int run_numbers[99] = {297595, 297590, 297588, 297558, 297544, 297542, 297541, 297540, 297537, 297512, 297483, 297481, 297479, 297452, 297451, 297450, 297446, 297442, 297441, 297415, 297414, 297413, 297406, 297405, 297380, 297379, 297372, 297367, 297366, 297363, 297336, 297335, 297333, 297332, 297317, 297315, 297312, 297311, 297310, 297278, 297222, 297221, 297219, 297218, 297196, 297195, 297194, 297193, 297133, /*297132,*/ 297129, 297128, 297124, 297123, 297119, 297118, 297117, 297085, 297035, 297031, 297029, 296966, 296941, 296938, 296935, 296934, 296932, 296931, 296930, 296903, 296900, 296899, 296894, 296890, 296852, 296851, 296850, 296849, 296848, 296839, 296838, 296836, 296835, 296799, 296794, 296793, 296790, 296787, 296786, 296785, 296784, 296781, 296752, 296750, 296749, 296694, 296693, 296691, 296690};

    if (local) {
        // if you want to run locally, we need to define some input
        TChain* chain = new TChain("aodTree");
        // add a few files to the chain (change this so that your local files are added)
        //chain->Add("/sampa/caiolagana/data/AliAOD252.18r.pass3.000296691.root");
        chain->Add("/sampa/caiolagana/data/LHC15o_246994_pass2_9074_AliAOD.root");
        // start the analysis locally, reading the events from the tchain
        mgr->StartAnalysis("local", chain);
    } else {
        // if we want to run on grid, we create and configure the plugin
        AliAnalysisAlien *alienHandler = new AliAnalysisAlien();
        // also specify the include (header) paths on grid
        alienHandler->AddIncludePath("-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include -I$ALICE_PHYSICS/include");
        // make sure your source files get copied to grid
        alienHandler->SetAdditionalLibs("LnnTTreeCreator.cxx LnnTTreeCreator.h");
        alienHandler->SetAnalysisSource("LnnTTreeCreator.cxx");
        // select the aliphysics version. all other packages
        // are LOADED AUTOMATICALLY!
        alienHandler->SetAliPhysicsVersion("vAN-20210315-1");
        //alienHandler->SetAliPhysicsVersion("vAN-20210415_JALIEN-1");
        //alienHandler->SetAliPhysicsVersion("vAN-20220621_ROOT6-1");
        // set the Alien API version
        alienHandler->SetAPIVersion("V1.1x");
        // select the input data
        //alienHandler->SetGridDataDir("/alice/data/2015/LHC15o");
        //alienHandler->SetDataPattern("*pass1/AOD194/*AOD.root");
        alienHandler->SetGridDataDir("/alice/data/2018/LHC18r");
        alienHandler->SetDataPattern("*pass3/AOD252/AOD/*AOD.root");
        // MC has no prefix, data has prefix 000
        alienHandler->SetRunPrefix("000");
        // runnumber
        //alienHandler->AddRunNumber(246994);//LHC15o
        //alienHandler->AddRunNumber(297595);//LHC18r
        for (int i = 0; i < 99; i++)
            alienHandler->AddRunNumber(run_numbers[i]);

        // number of files per subjob
        alienHandler->SetSplitMaxInputFileNumber(40);
        alienHandler->SetExecutable("myTask.sh");
        // specify how many seconds your job may take
        alienHandler->SetTTL(10000);
        alienHandler->SetJDLName("myTask.jdl");

        alienHandler->SetOutputToRunNo(kTRUE);
        alienHandler->SetKeepLogs(kTRUE);
        // merging: run with kTRUE to merge on grid
        // after re-running the jobs in SetRunMode("terminate") 
        // (see below) mode, set SetMergeViaJDL(kFALSE) 
        // to collect final results
        alienHandler->SetMaxMergeStages(1);
        alienHandler->SetMergeViaJDL(kFALSE);

        // define the output folders
        alienHandler->SetGridWorkingDir("LnnWorkingDir2018");
        alienHandler->SetGridOutputDir("LnnOutputDir2018");

        // connect the alien plugin to the manager
        mgr->SetGridHandler(alienHandler);
        if(gridTest) {
            // speficy on how many files you want to run
            alienHandler->SetNtestFiles(1);
            // and launch the analysis
            alienHandler->SetRunMode("test");
            mgr->StartAnalysis("grid");
        } else {
            // else launch the full grid analysis
            alienHandler->SetRunMode("terminate");
            mgr->StartAnalysis("grid");
        }
    }
}
