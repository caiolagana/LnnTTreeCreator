void runGrid()
{
  // Load common libraries
  gSystem->Load("libCore.so");  
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libPhysics.so");
  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");   
  // Use AliRoot includes to compile our task
  gROOT->ProcessLine(".include $ALICE_ROOT/include");

  // Grid informations
  gROOT->LoadMacro("CreateAlienHandler.C");
  AliAnalysisGrid *alienHandler = CreateAlienHandler();
  if (!alienHandler) return;

  // Create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("AliAnalysisTTreeCreator");
  mgr->SetGridHandler(alienHandler);

  // Make AOD input handler
  AliAODInputHandler *aodHandler = new AliAODInputHandler();
  mgr->SetInputEventHandler(aodHandler);

  // PID response task. Works with Aliroot>= v5-05-19-AN
  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPIDResponse.C");
  AliAnalysisTaskPIDResponse *taskPID = AddTaskPIDResponse(kFALSE);

  // Physics Selection Event
  //gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
  //AliPhysicsSelectionTask *physSel = AddTaskPhysicsSelection();

  // Create lpn task
  //gROOT->LoadMacro("LpnTTreeCreator.cxx++g");   
  //AliAnalysisTaskSE *lpntask = new LpnTTreeCreator("TaskLpnTreeCreator");
  //lpntask->SelectCollisionCandidates(AliVEvent::kINT7);
  //lpntask->SelectCollisionCandidates(0xffffffff);
  //mgr->AddTask(lpntask);

  // Create lnn task
  gROOT->LoadMacro("LnnTTreeCreator.cxx++g");   
  AliAnalysisTaskSE *lnntask = new LnnTTreeCreator("TaskLnnTreeCreator");
  lnntask->SelectCollisionCandidates(AliVEvent::kCentral | AliVEvent::kSemiCentral);
  //lnntask->SelectCollisionCandidates(0xffffffff);
  mgr->AddTask(lnntask);

  // Create containers for input/output
  cout << "============ creating input/output containers... ======================" << endl;
  AliAnalysisDataContainer *cinput   = mgr->GetCommonInputContainer();
  //AliAnalysisDataContainer *lhistout = mgr->CreateContainer("lhist", TList::Class(), AliAnalysisManager::kOutputContainer, "Grid.LpnHistOutput.root");
  //AliAnalysisDataContainer *ltreeout = mgr->CreateContainer("ltree", TTree::Class(), AliAnalysisManager::kOutputContainer, "Grid.LpnTreeOutput.root");
  AliAnalysisDataContainer *shistout = mgr->CreateContainer("shist", TList::Class(), AliAnalysisManager::kOutputContainer, "Grid.LnnHistOutput.root");
  AliAnalysisDataContainer *streeout = mgr->CreateContainer("stree", TTree::Class(), AliAnalysisManager::kOutputContainer, "Grid.LnnTreeOutput.root");
  cout << "input/output containers created!" << endl;

  // Connect input/output
  //mgr->ConnectInput(lpntask,  0, cinput);
  mgr->ConnectInput(lnntask,  0, cinput);
  //mgr->ConnectOutput(lpntask, 1, lhistout);
  //mgr->ConnectOutput(lpntask, 2, ltreeout);
  mgr->ConnectOutput(lnntask, 1, shistout);
  mgr->ConnectOutput(lnntask, 2, streeout);
  cout << "I/O connected" << endl;

  // Enable debug printouts
  mgr->SetDebugLevel(10);

  if (!mgr->InitAnalysis())
    return;

  mgr->PrintStatus();
  mgr->StartAnalysis("grid");

};
