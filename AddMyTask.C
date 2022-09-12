LnnTTreeCreator* AddMyTask(TString name = "LnnTTreeCreator")
{
    // get the manager via the static access member. since it's static, you don't need
    // to create an instance of the class here to call the function
    AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
    if (!mgr) {
        return 0x0;
    }
    // get the input event handler, again via a static method. 
    // this handler is part of the managing system and feeds events
    // to your task
    if (!mgr->GetInputEventHandler()) {
        return 0x0;
    }
    // create an instance of your task
    LnnTTreeCreator* task = new LnnTTreeCreator(name.Data());   
    if(!task) return 0x0;
    task->SelectCollisionCandidates(AliVEvent::kCentral | AliVEvent::kSemiCentral);
    // add your task to the manager
    mgr->AddTask(task);
    // Create containers for input/output
    cout << "============ creating input/output containers... ======================" << endl;
    AliAnalysisDataContainer *cinput   = mgr->GetCommonInputContainer();
    AliAnalysisDataContainer *shistout = mgr->CreateContainer("shist", TList::Class(), AliAnalysisManager::kOutputContainer, "Grid.LnnHistOutput.root");
    AliAnalysisDataContainer *streeout = mgr->CreateContainer("stree", TTree::Class(), AliAnalysisManager::kOutputContainer, "Grid.LnnTreeOutput.root");
    cout << "input/output containers created!" << endl;
    // Connect input/output
    mgr->ConnectInput(task,  0, cinput);
    mgr->ConnectOutput(task, 1, shistout);
    mgr->ConnectOutput(task, 2, streeout);
    cout << "I/O connected" << endl;
    // in the end, this macro returns a pointer to your task. this will be convenient later on
    // when you will run your analysis in an analysis train on grid
    return task;
}
