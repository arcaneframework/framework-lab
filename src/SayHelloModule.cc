// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
 
#include "SayHelloModule.h"

#include <arcane/IParallelMng.h>
#include <arcane/impl/ArcaneMain.h>
#include "mpA.h"

using namespace Arcane;

#include "main.h"


void SayHelloModule::
beginCompute()
{
  #ifdef PRINT_CALL
  std::cout << "--------------- MPA_Init()" << std::endl;
  #endif

  int isInit = false;
  MPA_Initialized(&isInit);

  parallelMng()->barrier();

  if(parallelMng()->commRank() == 0){
    mpiArcane = new MpiArcane();
    MPA_STATUS = new MPA_Status();
  }
  parallelMng()->barrier();

  mpiArcane->MpiArcane_Init(parallelMng());


  // Test :

  int world_rank = -123;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size = -456;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  if(world_rank != parallelMng()->commRank() || world_size != parallelMng()->commSize()){
    std::cout << "Erreur de rank ou de size : Vrai rank : " << parallelMng()->commRank() << " rank trouvé : " << world_rank << std::endl;
    ARCANE_FATAL("Erreur de rank ou de size...");
  }
}

void SayHelloModule::
compute()
{
  int argc = 0;
  char** argv = NULL;

  int error = mMain(argc, argv);
  IArcaneMain::arcaneMain()->setErrorCode(error);
}

void SayHelloModule::
endCompute()
{
  subDomain()->timeLoopMng()->stopComputeLoop(true);
}
 
ARCANE_REGISTER_MODULE_SAYHELLO(SayHelloModule);
