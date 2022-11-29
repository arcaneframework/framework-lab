// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
 
#include "LauncherModule.h"

#include <arcane/IParallelMng.h>
#include <arcane/impl/ArcaneMain.h>
#include <arcane/utils/ApplicationInfo.h>
#include "mpA.h"
#include "ShMemArcane.h"
#include "MpiArcane.h"

using namespace Arcane;

#include "main.h"


void LauncherModule::
beginCompute()
{
  info() << "MPA_Init()";

  if(parallelMng()->isThreadImplementation())
  {
    info() << "Mode ShMem";
    parallelMng()->barrier();
    if(parallelMng()->commRank() == 0){
      mpiArcane = new ShMemArcane();
      MPA_STATUS = new MPA_Status();
    }
    parallelMng()->barrier();
  }
  else{
    info() << "Mode Mpi";
    mpiArcane = new MpiArcane();
    MPA_STATUS = new MPA_Status();
  }

  mpiArcane->Init(parallelMng());
}

void LauncherModule::
compute()
{
  int argc = 0;
  char** argv = NULL;

  //StringList arguments;
  //IArcaneMain::arcaneMain()->applicationInfo().args(arguments);

  int error = mMain(argc, argv);
  IArcaneMain::arcaneMain()->setErrorCode(error);
}

void LauncherModule::
endCompute()
{
  info() << "MPA_Finalize()";

  if(parallelMng()->isThreadImplementation())
  {
    if(parallelMng()->commRank() == 0){
      delete mpiArcane;
      delete MPA_STATUS;
    }
  }
  else{
    delete mpiArcane;
    delete MPA_STATUS;
  }


  subDomain()->timeLoopMng()->stopComputeLoop(true);
}
 
ARCANE_REGISTER_MODULE_LAUNCHER(LauncherModule);
