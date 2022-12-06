// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
 //-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* LauncherModule.cc                                           (C) 2000-2022 */
/*                                                                           */
/* Module permettant de créer l'objet gérant le message passing et de lancer */
/* le programme qui utilise des appels MPI.                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "LauncherModule.h"

#include <arcane/IParallelMng.h>
#include <arcane/impl/ArcaneMain.h>
#include <arcane/utils/ApplicationInfo.h>
#include <arcane/parallel/mpithread/HybridParallelMng.h>
#include <arcane/parallel/mpi/MpiParallelMng.h>
#include "mpA.h"
#include "HybridArcane.h"
#include "ShMemArcane.h"
#include "MpiArcane.h"

using namespace Arcane;
using namespace Arcane::MessagePassing;

#include "main.h"


void LauncherModule::
beginCompute()
{
  info() << "MPA_Init()";

  // On choisit le bon mode.
  if(parallelMng()->isHybridImplementation())
  {
    info() << "Mode Hybride";

    parallelMng()->barrier();
    if(parallelMng()->commRank() % parallelMng()->messagePassingMng()->commSize() == 0){
      mpiArcane = new HybridArcane();
      MPA_STATUS = new MPA_Status();
    }
    parallelMng()->barrier();
  }
  else if(parallelMng()->isThreadImplementation())
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

  // On lance le "vrai" main du fichier main.h.
  int error = mMain(argc, argv);

  // Le code erreur de mMain() est le code retourné par Arcane.
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
