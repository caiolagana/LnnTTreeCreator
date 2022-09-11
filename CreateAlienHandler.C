AliAnalysisGrid *CreateAlienHandler()
{
	
//**********************************************************************   
	AliAnalysisAlien *plugin = new AliAnalysisAlien();
	plugin->SetOverwriteMode();
	
//Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
	plugin->SetRunMode("test");
//**********************************************************************

//**********************************************************************	
//Set versions of used packages
//more informatio see: https://alimonitor.cern.ch/packages/
plugin->SetAPIVersion("V1.1x");
plugin->SetROOTVersion("v5-34-08-7");
plugin->SetAliROOTVersion("vAN-20220619-1");
//**********************************************************************

//=================== PbPb 11h
plugin->SetGridDataDir("/alice/data/2011/LHC11h_2");
plugin->SetRunPrefix("000");
plugin->SetDataPattern("*ESDs/pass2/AOD145/*/AliAOD.root");
//plugin->SetDataPattern("*ESDs/pass2/AOD115/*/AliAOD.root");
//=================== PbPb 11h

// https://alimonitor.cern.ch/prod/jobs.jsp?t=5163
//    Int_t run_numbers[169]= {//AOD145
//167693, 167706, 167711, 167713, 167806, 167807, 167808, 167813, 167814, 167818, 167902, 167903, 167915, 167920, 167921, 167985, 167986, 167987, 167988, 168066, 168068, 168069, 168076, 168103, 168104, 168105, 168107, 168108, 168115, 168171, 168172, 168173, 168175, 168181, 168203, 168204, 168205, 168206, 168207, 168208, 168212, 168213, 168310, 168311, 168318, 168322, 168325, 168341, 168342, 168356, 168361, 168362, 168458, 168460, 168461, 168464, 168467, 168511, 168512, 168514, 168777, 168826, 168984, 168988, 168992, 169035, 169040, 169044, 169045, 169091, 169094, 169099, 169138, 169143, 169144, 169145, 169148, 169156, 169160, 169167, 169238, 169411, 169415, 169417, 169418, 169419, 169420, 169475, 169498, 169504, 169506, 169512, 169515, 169550, 169553, 169554, 169555, 169557, 169584, 169586, 169587, 169588, 169590, 169591, 169628, 169835, 169837, 169838, 169846, 169855, 169858, 169859, 169918, 169919, 169920, 169922, 169923, 169924, 169956, 169961, 169965, 169969, 169975, 169981, 170027, 170036, 170038, 170040, 170081, 170083, 170084, 170085, 170088, 170089, 170091, 170152, 170155, 170159, 170163, 170193, 170195, 170203, 170204, 170205, 170207, 170208, 170228, 170230, 170264, 170267, 170268, 170269, 170270, 170306, 170308, 170309, 170311, 170312, 170313, 170315, 170387, 170388, 170389, 170390, 170546, 170552, 170556, 170572, 170593
//};

//http://alimonitor.cern.ch/configuration/index.jsp?partition=LHC11h&pass=2&raw_run=&filling_scheme=&filling_config=&fillno=&energy=&intensity_per_bunch=&mu=&interacting_bunches=&noninteracting_bunches_beam_1=&noninteracting_bunches_beam_2=&interaction_trigger=&rate=&beam_empty_trigger=&empty_empty_trigger=&muon_trigger=&high_multiplicity_trigger=&emcal_trigger=&calibration_trigger=&quality=&muon_quality=&comment=&field=&det_aco=&det_emc=&det_fmd=&det_hlt=&det_hmp=&det_mch=&det_mtr=&det_phs=&det_pmd=&det_spd=&det_sdd=&det_ssd=&det_tof=&det_tpc=&det_trd=&det_t00=&det_v00=&det_zdc=&hlt_mode=&changedon=
//
//runs with good global quality
//
Int_t run_numbers[108]= {170593//good runs
//!! 170593, 170572, 170388, 170387, 170315, 170313, 170312, 170311, 170309, 170308, 170306, 170270, 170269, 170268, 170230, 170228, 170207, 170204, 170203, 170193, 170163, 170159, 170155, 170091, 170089, 170088, 170085, 170084, 170083, 170081, 170040, 170027, 169965, 169923, 169859, 169858, 169855, 169846, 169838, 169837, 169835, 169591, 169590, 169588, 169587, 169586, 169557, 169555, 169554, 169553, 169550, 169515, 169512, 169506, 169504, 169498, 169475, 169420, 169419, 169418, 169417, 169415, 169411, 169238, 169167, 169160, 169156, 169148, 169145, 169144, 169138, 169099, 169094, 169091, 169045, 169044, 169040, 169035, 168992, 168988, 168826, 168777, 168514, 168512, 168511, 168467, 168464, 168460, 168458, 168362, 168361, 168342, 168341, 168325, 168322, 168311, 168310, 168115, 168108, 168107, 168105, 168076, 168069, 167988, 167987, 167985, 167920, 167915
};

  for (Int_t i=0;i<108;i++)
    plugin->AddRunNumber(run_numbers[i]);

//semi-good runs (NOT USED)
Int_t run_semigood[16]={
167813, 168066, 168068, 168104, 168212, 168461, 169143, 169956, 170036, 169975, 169981, 170038, 170152, 170195, 170205, 170264
};
//!!  for (Int_t i=0;i<0;i++)
//!!    plugin->AddRunNumber(run_semigood[i]);

//complementary runs (NOT USED)
Int_t run_complementary[44]={
167693, 167706, 167711, 167713, 167806, 167807, 167808, 167814, 167818, 167902, 167903, 167921, 167986, 168103, 168171, 168172, 168173, 168175, 168181, 168203, 168204, 168205, 168206, 168207, 168208, 168213, 168318, 168356, 168984, 169584, 169628, 169918, 169919, 169920, 169922, 169924, 169961, 170208, 170267, 170389, 170390, 170546, 170552, 170556
};
//!!  for (Int_t i=0;i<0;i++)
//!!    plugin->AddRunNumber(run_complementary[i]);


//plugin->SetMergeViaJDL(kTRUE);
plugin->SetMergeViaJDL(kFALSE);
//plugin->SetCheckCopy(kTRUE);
plugin->SetCheckCopy(kFALSE);//do not copy files to aliensh

//**********************************************************************
//Define alien work directory where all files will be copied. Relative to alien $HOME.
	plugin->SetOutputToRunNo(1);
        plugin->SetOverwriteMode(kTRUE);
	plugin->SetGridWorkingDir("11h_AOD145_TreeCreation_fitPID");
//Declare alien output directory. Relative to working directory.
	plugin->SetGridOutputDir(Form("output"));
//Declare the analysis source files names separated by blancs. To be compiled runtime
//using ACLiC on the worker nodes.
	plugin->SetAnalysisSource("LnnTTreeCreator.cxx");
//Declare all libraries (other than the default ones for the framework. These will be
//loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
	plugin->SetAdditionalLibs("LnnTTreeCreator.cxx LnnTTreeCreator.h");
	//plugin->SetAdditionalLibs("LnnTTreeCreator.cxx LpnTTreeCreator.cxx LnnTTreeCreator.h LpnTTreeCreator.h HyperRecoDecay.h");
        plugin->SetNtestFiles(1);
	plugin->SetDefaultOutputs();
	plugin->SetAnalysisMacro("plugin_11h_AOD145_TreeCreation_fitPID.C");

//Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
	plugin->SetSplitMaxInputFileNumber(100);
//Optionally modify the executable name (default analysis.sh)
	plugin->SetExecutable("plugin_11h_AOD145_TreeCreation_fitPID.sh");
//Optionally set time to live (default 30000 sec)
	plugin->SetTTL(30000);
//Optionally set input format (default xml-single)
	plugin->SetInputFormat("xml-single");
//Optionally modify the name of the generated JDL (default analysis.jdl)
	plugin->SetJDLName("plugin_11h_AOD145_TreeCreation_fitPID.jdl");
//Optionally modify job price (default 1)
	plugin->SetPrice(1);      
//Optionally modify split mode (default 'se')    
	plugin->SetSplitMode("se");
//**********************************************************************
	
	return plugin;
}
