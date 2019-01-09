#include <string>

using namespace std;

void Fun4All_Cosmic(int nEvents = 20000,
                      // const char *input_file = "/sphenix/data/data02/sphenix/caladc/hcallab/ihcal_2018-01-12__17_09_38.prdf",
                      // const char *output_file = "/sphenix/user/xusun/software/data/cosmic/beamtest/Cosmic_HCALIN_col_0_0.root")
                      // const char *input_file = "/sphenix/data/data02/sphenix/caladc/hcallab/ihcal_2018-01-15__08_00_58.prdf",
                      // const char *output_file = "/sphenix/user/xusun/software/data/cosmic/beamtest/Cosmic_HCALIN_col_0_1.root")
                      // const char *input_file = "/sphenix/data/data02/sphenix/t1044/rcdaq-00000074-0000.prdf",
                      // const char *output_file = "/sphenix/user/xusun/software/data/cosmic/beamtest/Cosmic_HCALIN_col_1.root")
                      // const char *input_file = "/sphenix/data/data02/sphenix/t1044/rcdaq-00000088-0000.prdf",
                      // const char *output_file = "/sphenix/user/xusun/software/data/cosmic/beamtest/Cosmic_HCALIN_col_2_0.root")
                      // const char *input_file = "/sphenix/data/data02/sphenix/t1044/rcdaq-00000089-0000.prdf",
                      // const char *output_file = "/sphenix/user/xusun/software/data/cosmic/beamtest/Cosmic_HCALIN_col_2_1.root")
                      // const char *input_file = "/sphenix/data/data02/sphenix/t1044/rcdaq-00000124-0000.prdf",
                      // const char *output_file = "/sphenix/user/xusun/software/data/cosmic/beamtest/Cosmic_HCALIN_col_3.root")
                      const char *input_file = "/sphenix/data/data02/sphenix/t1044/rcdaq-00000162-0000.prdf",
                      const char *output_file = "/sphenix/user/xusun/software/data/cosmic/beamtest/Cosmic_HCALOUT_col_0.root")
{
  gSystem->Load("libfun4all");
  gSystem->Load("libPrototype4.so");

  const bool do_cemc = true;
  const bool do_hcal = true;
  //  const bool do_cemc = false;
  //  const bool do_hcal = false;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(Fun4AllServer::VERBOSITY_SOME);

  recoConsts *rc = recoConsts::instance();
  //rc->set_IntFlag("RUNNUMBER",0);

  // ------------------- HCal and EMcal -------------------

  CaloCalibration *calib = NULL;

  if (do_cemc)
  {
    SubsysReco *unpack = new CaloUnpackPRDF();
    //  unpack->Verbosity(1);
    se->registerSubsystem(unpack);

    calib = new CaloCalibration("CEMC");
    //    calib->Verbosity(4);
    //    calib->GetCalibrationParameters().set_double_param("calib_const_scale", 8. / 3000);
    //    calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 0);
    calib->GetCalibrationParameters().ReadFromFile("CEMC", "xml", 0, 0,
                                                   string(getenv("CALIBRATIONROOT")) + string("/Prototype4/Calibration/"));  // calibration database
    se->registerSubsystem(calib);
  }

  if (do_hcal)
  {
    // leading order energy scale from Xu Sun
    // const double cin_cali = 0.00270145;
    // const double cout_cali = 0.0065718;

    calib = new CaloCalibration("HCALIN");
    calib->set_calib_tower_node_prefix("CALIB_LG");
    calib->set_raw_tower_node_prefix("RAW_LG");
    calib->GetCalibrationParameters().set_name("hcalin_lg");
    // calib->GetCalibrationParameters().set_double_param("calib_const_scale", cin_cali);
    // calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 0);
     calib->GetCalibrationParameters().ReadFromFile("hcalin_lg", "xml", 0, 0, string(getenv("CALIBRATIONROOT")) + string("/Prototype4/Calibration/"));  // calibration database
    se->registerSubsystem(calib);

    calib = new CaloCalibration("HCALIN");
    calib->set_calib_tower_node_prefix("CALIB_HG");
    calib->set_raw_tower_node_prefix("RAW_HG");
    calib->GetCalibrationParameters().set_name("hcalin_hg");
    calib->GetCalibrationParameters().ReadFromFile("hcalin_hg", "xml", 0, 0, string(getenv("CALIBRATIONROOT")) + string("/Prototype4/Calibration/"));  // calibration database
    se->registerSubsystem(calib);

    calib = new CaloCalibration("HCALOUT");
    calib->set_calib_tower_node_prefix("CALIB_LG");
    calib->set_raw_tower_node_prefix("RAW_LG");
    calib->GetCalibrationParameters().set_name("hcalout_lg");
    // calib->GetCalibrationParameters().set_double_param("calib_const_scale", cout_cali);
    // calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 0);
     calib->GetCalibrationParameters().ReadFromFile("hcalout_lg", "xml", 0, 0, string(getenv("CALIBRATIONROOT")) + string("/Prototype4/Calibration/"));  // calibration database
    se->registerSubsystem(calib);

    calib = new CaloCalibration("HCALOUT");
    calib->set_calib_tower_node_prefix("CALIB_HG");
    calib->set_raw_tower_node_prefix("RAW_HG");
    calib->GetCalibrationParameters().set_name("hcalout_hg");
    // calib->GetCalibrationParameters().set_double_param("calib_const_scale", cout_cali / 32);
    // calib->GetCalibrationParameters().set_int_param("use_chan_calibration", 0);
    calib->GetCalibrationParameters().ReadFromFile("hcalout_hg", "xml", 0, 0, string(getenv("CALIBRATIONROOT")) + string("/Prototype4/Calibration/"));  // calibration database
    se->registerSubsystem(calib);
  }

  // -------------------  Output -------------------
  //main DST output
  Fun4AllDstOutputManager *out_Manager = new Fun4AllDstOutputManager("DSTOUT",
                                                                     output_file);
  se->registerOutputManager(out_Manager);

  //alternatively, fast check on DST using DST Reader:
  Prototype4DSTReader *reader = new Prototype4DSTReader(
      string(output_file) + string("_DSTReader.root"));

  if (do_hcal)
  {
    reader->AddTower("RAW_LG_HCALIN");
    //  reader->AddTower("RAW_HG_HCALIN");
    reader->AddTower("RAW_LG_HCALOUT");
    reader->AddTower("RAW_HG_HCALOUT");

    reader->AddTower("CALIB_LG_HCALIN");
    //  reader->AddTower("CALIB_HG_HCALIN");
    reader->AddTower("CALIB_LG_HCALOUT");
    reader->AddTower("CALIB_HG_HCALOUT");
  }

  if (do_cemc)
  {
    reader->AddTower("RAW_CEMC");
    reader->AddTower("CALIB_CEMC");
  }

  se->registerSubsystem(reader);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(input_file);
  se->registerInputManager(in);

  se->run(nEvents);

  se->End();
}
