// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* mpA.h                                                       (C) 2000-2022 */
/*                                                                           */
/* Fichier à inclure pour pouvoir utiliser le MessagePassing de Arcane.      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef MPA_H
#define MPA_H

#include <IPMngArcane.h>
IPMngArcane* mpiArcane = nullptr;

// Permet l'utilisation de MPI sans Arcane (pour tester).
//#define useMPI

// Permet d'afficher les appels MPI.
#define PRINT_CALL



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifdef useMPI

#include <mpi.h>

#ifdef PRINT_CALL
int world_rank = -1;
#endif

using MPA_Status = MPI_Status;

// Macro permettant de récupérer des infos à partir d'un status.
#define MPI_Status_sizeof() sizeof(MPI_Status)
#define MPI_Status_source(a) ((a)->MPI_SOURCE)
#define MPI_Status_error(a) ((a)->MPI_ERROR)
#define MPI_Status_tag(a) ((a)->MPI_TAG)


int MPA_Init(int *argc, char ***argv)
{
  #ifdef PRINT_CALL

  std::cout << "--------------- MPA_Init(argc, argv)" << std::endl;


  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  

  return MPI_SUCCESS;

  #else
  return MPI_SUCCESS;
  #endif
}


int MPA_Initialized(int *flag)
{
  #ifdef PRINT_CALL
  std::cout << "--------------- MPA_Initialized()" << std::endl;
  #endif

  return MPI_Initialized(flag);
}

int MPA_Finalize(void)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Finalize()" << std::endl;
  #endif

  return MPI_SUCCESS;
}

int MPA_Abort(MPI_Comm comm, int errorcode)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Abort()" << std::endl;
  #endif

  return MPI_Abort(comm, errorcode);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPA_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Comm_split()" << std::endl;
  #endif

  return MPI_Comm_split(comm, color, key, newcomm);
}

int MPA_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Comm_dup()" << std::endl;
  #endif

  return MPI_Comm_dup(comm, newcomm);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPA_Comm_size(MPI_Comm comm, int *size)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Comm_size()" << std::endl;
  #endif

  return MPI_Comm_size(comm, size);
}

int MPA_Comm_rank(MPI_Comm comm, int *rank)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Comm_rank()" << std::endl;
  #endif

  return MPI_Comm_rank(comm, rank);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPA_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
  #ifdef PRINT_CALL
  int rank;
  MPI_Comm_rank(comm, &rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Send("
            << "source=" << rank
            << ", dest=" << dest
            << ", tag=" << tag 
            << ")" << std::endl;
  #endif

  return MPI_Send(buf, count, datatype, dest, tag, comm);
}

int MPA_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
  #ifdef PRINT_CALL
  int rank;
  MPI_Comm_rank(comm, &rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Recv("
            << "source=" << source
            << ", dest=" << rank
            << ", tag=" << tag 
            << ")" << std::endl;
  #endif

  return MPI_Recv(buf, count, datatype, source, tag, comm, status);
}



int MPA_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  #ifdef PRINT_CALL
  int rank;
  MPI_Comm_rank(comm, &rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Isend("
            << "source=" << rank
            << ", dest=" << dest
            << ", tag=" << tag 
            << ")" << std::endl;
  #endif

  return MPI_Isend(buf, count, datatype, dest, tag, comm, request);
}

int MPA_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request)
{
  #ifdef PRINT_CALL
  int rank;
  MPI_Comm_rank(comm, &rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Irecv("
            << "source=" << source
            << ", dest=" << rank
            << ", tag=" << tag 
            << ")" << std::endl;
  #endif

  return MPI_Irecv(buf, count, datatype, source, tag, comm, request);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPA_Type_size(MPI_Datatype datatype, int *size)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Type_size()" << std::endl;
  #endif

  return MPI_Type_size(datatype, size);
}

int MPA_Wait(MPI_Request *request, MPI_Status *status)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Wait()" << std::endl;
  #endif
  
  return MPI_Wait(request, status);
}

int MPA_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Waitall()" << std::endl;
  #endif

  return MPI_Waitall(count, array_of_requests, array_of_statuses);
}

int MPA_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Waitany()" << std::endl;
  #endif

  return MPI_Waitany(count, array_of_requests, index, status);
}

int MPA_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Test()" << std::endl;
  #endif

  return MPI_Test(request, flag, status);
}

int MPA_Testall(int count, MPI_Request *array_of_requests, int *flag, MPI_Status *array_of_statuses)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Testall()" << std::endl;
  #endif

  return MPI_Testall(count, array_of_requests, flag, array_of_statuses);
}

int MPA_Barrier(MPI_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Barrier()" << std::endl;
  #endif

  return MPI_Barrier(comm);
}

int MPA_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
  #ifdef PRINT_CALL
  int sizeof_datatype;
  MPI_Type_size(datatype, &sizeof_datatype);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Bcast("
            << "count=" << count 
            << ", sizeof_datatype=" << sizeof_datatype 
            << ")" << std::endl;
  #endif

  return MPI_Bcast(buffer, count, datatype, root, comm);
}

int MPA_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm)
{
  #ifdef PRINT_CALL
  int sizeof_sendtype, sizeof_recvtype;
  MPI_Type_size(sendtype, &sizeof_sendtype);
  MPI_Type_size(recvtype, &sizeof_recvtype);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Gather("
            << "sendcount=" << sendcount 
            << ", sizeof_sendtype=" << sizeof_sendtype 
            << ", recvcount=" << recvcount 
            << ", sizeof_recvtype=" << sizeof_recvtype 
            << ")" << std::endl;
  #endif

  return MPI_Gather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
}


int MPA_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, const int *recvcounts, const int *displs,
                MPI_Datatype recvtype, int root, MPI_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Gatherv()" << std::endl;
  #endif

  return MPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm);
}

int MPA_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPI_Comm comm)
{
  #ifdef PRINT_CALL
  int sizeof_sendtype, sizeof_recvtype;
  MPI_Type_size(sendtype, &sizeof_sendtype);
  MPI_Type_size(recvtype, &sizeof_recvtype);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Allgather("
            << "sendcount=" << sendcount 
            << ", sizeof_sendtype=" << sizeof_sendtype 
            << ", recvcount=" << recvcount 
            << ", sizeof_recvtype=" << sizeof_recvtype 
            << ")" << std::endl;
  #endif

  return MPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
}

int MPA_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const int *recvcounts, const int *displs,
                   MPI_Datatype recvtype, MPI_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Allgatherv()" << std::endl;
  #endif

  return MPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
}

int MPA_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Reduce()" << std::endl;
  #endif

  return MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
}

int MPA_Allreduce(const void *sendbuf, void *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Allreduce()" << std::endl;
  #endif

  return MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}

int MPA_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
               MPI_Comm comm)
{
  #ifdef PRINT_CALL
  int sizeof_sendtype, sizeof_recvtype;
  MPI_Type_size(sendtype, &sizeof_sendtype);
  MPI_Type_size(recvtype, &sizeof_recvtype);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Scatter("
            << "sendcount=" << sendcount 
            << ", sizeof_sendtype=" << sizeof_sendtype 
            << ", recvcount=" << recvcount 
            << ", sizeof_recvtype=" << sizeof_recvtype 
            << ")" << std::endl;
  #endif

  return MPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
}

int MPA_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs,
                 MPI_Datatype sendtype, void *recvbuf, int recvcount,
                 MPI_Datatype recvtype,
                 int root, MPI_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Scatterv()" << std::endl;
  #endif

  return MPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm);
}

int MPA_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Probe(" 
            << "provenance=" << source
            << ", tag=" << tag 
            << ")" << std::endl;
  #endif

  return MPI_Probe(source, tag, comm, status);
}

int MPA_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Iprobe(" 
            << "provenance=" << source
            << ", tag=" << tag 
            << ")" << std::endl;
  #endif

  return MPI_Iprobe(source, tag, comm, flag, status);
}

int MPA_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Get_count()" << std::endl;
  #endif

  return MPI_Get_count(status, datatype, count);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#else

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


// Le MPI_Status devient un MessageId.
using MPA_Status  = Arccore::MessagePassing::MessageId;

// Pour ces deux macros, on doit utiliser une variable
// globale pour éviter des problèmes aux probes.
#undef MPI_STATUS_IGNORE
#define MPI_STATUS_IGNORE MPA_STATUS

#undef MPI_STATUSES_IGNORE
#define MPI_STATUSES_IGNORE MPA_STATUS

// Ces macros MPA sont dans le fichier IPMngArcane.h.
#undef MPI_COMM_WORLD
#define MPI_COMM_WORLD MPA_COMM_WORLD

#define MPI_Request MPA_Request
#define MPI_Status MPA_Status
#define MPI_Comm MPA_Comm

// Pour éviter une incompatibilité avec MPICH
// (vu que ces macros sont récup dans le mpi.h).
#undef MPI_ANY_SOURCE
#define MPI_ANY_SOURCE -1

#undef MPI_ANY_TAG
#define MPI_ANY_TAG -1

// Macros permettant de récupérer des infos à partir d'un MPA_Status.
#define MPI_Status_sizeof() sizeof(MPA_Status)
#define MPI_Status_source(a) ((a)->sourceInfo().rank().value())
#define MPI_Status_error(a) (MPI_SUCCESS)
#define MPI_Status_tag(a) ((a)->sourceInfo().tag().value())

MPA_Status* MPA_STATUS;


int MPA_Init(int *argc, char ***argv)
{
  #ifdef PRINT_CALL
  std::cout << "--------------- MPA_Init(argc, argv)" << std::endl;
  #endif

  return MPI_SUCCESS;
}

int MPA_Initialized(int *flag)
{
  *flag = (mpiArcane != nullptr);
  return MPI_SUCCESS;
}

int MPA_Finalize(void)
{
  int isInit = false;
  MPA_Initialized(&isInit);
  if(!isInit){
    return MPI_SUCCESS;
  }

  #ifdef PRINT_CALL
  int rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &rank);
  std::cout << "[World Rank " << rank << "] --------------- MPA_Finalize()" << std::endl;
  #endif

  mpiArcane->Barrier(MPA_COMM_WORLD);
  mpiArcane->Finalize();

  return MPI_SUCCESS;
}

int MPA_Abort(MPA_Comm comm, int errorcode)
{
  #ifdef PRINT_CALL
  std::cout << "--------------- MPA_Abort()" << std::endl;
  #endif

  return mpiArcane->Abort(comm, errorcode);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPA_Comm_split(MPA_Comm comm, int color, int key, MPA_Comm *newcomm)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Comm_split()" << std::endl;
  #endif

  return mpiArcane->Comm_split(comm, color, key, newcomm);
}

int MPA_Comm_dup(MPA_Comm comm, MPA_Comm *newcomm)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Comm_dup()" << std::endl;
  #endif

  return mpiArcane->Comm_dup(comm, newcomm);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPA_Comm_size(MPA_Comm comm, int *size)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Comm_size()" << std::endl;
  #endif

  return mpiArcane->Comm_size(comm, size);
}

int MPA_Comm_rank(MPA_Comm comm, int *rank)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  #ifdef PRINT_CALL
  std::cout << "--------------- MPA_Comm_rank()" << std::endl;
  #endif

  return mpiArcane->Comm_rank(comm, rank);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPA_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  int rank;
  mpiArcane->Comm_rank(comm, &rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Send("
            << "source=" << rank
            << ", dest=" << dest
            << ", tag=" << tag 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  // Avec "blocking" = true, le MPA_Request n'est pas utilisé.
  MPA_Request req;
  return mpiArcane->Send(buf, count, datatype, dest, tag, comm, &req, true);
}

int MPA_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPA_Comm comm, MPA_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  int rank;
  mpiArcane->Comm_rank(comm, &rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Recv("
            << "source=" << source
            << ", dest=" << rank
            << ", tag=" << tag 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  // Avec "blocking" = true, le MPA_Request n'est pas utilisé.
  MPA_Request req;
  return mpiArcane->Recv(buf, count, datatype, source, tag, comm, &req, status, true);
}



int MPA_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPA_Comm comm, MPA_Request *request)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  int rank;
  mpiArcane->Comm_rank(comm, &rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Isend("
            << "source=" << rank
            << ", dest=" << dest
            << ", tag=" << tag 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  return mpiArcane->Send(buf, count, datatype, dest, tag, comm, request, false);
}

int MPA_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPA_Comm comm, MPA_Request *request)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  int rank;
  mpiArcane->Comm_rank(comm, &rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Irecv("
            << "source=" << source
            << ", dest=" << rank
            << ", tag=" << tag 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  MessageId mId;
  return mpiArcane->Recv(buf, count, datatype, source, tag, comm, request, &mId, false);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPA_Type_size(MPI_Datatype datatype, int *size)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Type_size()" << std::endl;
  #endif

  return mpiArcane->Type_size(datatype, size);
}

int MPA_Wait(MPA_Request *request, MPA_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Wait()" << std::endl;
  #endif

  return mpiArcane->Wait(request);
}

int MPA_Waitall(int count, MPA_Request *array_of_requests, MPA_Status *array_of_statuses)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Waitall()" << std::endl;
  #endif

  return mpiArcane->Waitall(count, array_of_requests);
}

int MPA_Waitany(int count, MPA_Request *array_of_requests, int *index, MPA_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Waitany()" << std::endl;
  #endif

  return mpiArcane->Waitany(count, array_of_requests, index);
}

int MPA_Test(MPA_Request *request, int *flag, MPA_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Test()" << std::endl;
  #endif

  return mpiArcane->Test(request, flag);
}

int MPA_Testall(int count, MPA_Request *array_of_requests, int *flag, MPA_Status *array_of_statuses)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Testall()" << std::endl;
  #endif

  return mpiArcane->Testall(count, array_of_requests, flag);
}

int MPA_Barrier(MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Barrier()" << std::endl;
  #endif

  return mpiArcane->Barrier(comm);
}

int MPA_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  int sizeof_datatype;
  mpiArcane->Type_size(datatype, &sizeof_datatype);

  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Bcast("
            << "count=" << count 
            << ", sizeof_datatype=" << sizeof_datatype 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  return mpiArcane->Bcast(buffer, count, datatype, root, comm);
}

int MPA_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  int sizeof_sendtype, sizeof_recvtype;
  mpiArcane->Type_size(sendtype, &sizeof_sendtype);
  mpiArcane->Type_size(recvtype, &sizeof_recvtype);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Gather("
            << "sendcount=" << sendcount 
            << ", sizeof_sendtype=" << sizeof_sendtype 
            << ", recvcount=" << recvcount 
            << ", sizeof_recvtype=" << sizeof_recvtype 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  return mpiArcane->Gather(
    sendbuf, sendcount, sendtype,
    recvbuf, recvcount, recvtype,
    root, comm);
}


int MPA_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, const int *recvcounts, const int *displs,
                MPI_Datatype recvtype, int root, MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Gatherv()" << std::endl;
  #endif

  return mpiArcane->Gatherv(
    sendbuf, sendcount, sendtype,
    recvbuf, recvcounts, displs, recvtype,
    root, comm);
}

int MPA_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  int sizeof_sendtype, sizeof_recvtype;
  mpiArcane->Type_size(sendtype, &sizeof_sendtype);
  mpiArcane->Type_size(recvtype, &sizeof_recvtype);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Allgather("
            << "sendcount=" << sendcount 
            << ", sizeof_sendtype=" << sizeof_sendtype 
            << ", recvcount=" << recvcount 
            << ", sizeof_recvtype=" << sizeof_recvtype 
            << ", comm=" << comm 
            << ")" << std::endl;

  #endif

  return mpiArcane->Allgather(
    sendbuf, sendcount, sendtype,
    recvbuf, recvcount, recvtype,
    comm);
}

int MPA_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const int *recvcounts, const int *displs,
                   MPI_Datatype recvtype, MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Allgatherv()" << std::endl;
  #endif
  
  return mpiArcane->Allgatherv(
    sendbuf, sendcount, sendtype,
    recvbuf, recvcounts, displs, recvtype,
    comm);
}

int MPA_Allreduce(const void *sendbuf, void *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Allreduce()" << std::endl;
  #endif
  
  return mpiArcane->Allreduce(sendbuf, recvbuf, count, datatype, op, comm);

}

int MPA_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPA_Comm comm)
{
  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Reduce()" << std::endl;
  #endif

  return MPA_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}

int MPA_Scatter(const void *sendbuf, int sendcount,
                MPI_Datatype sendtype,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int root, MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  int sizeof_sendtype, sizeof_recvtype;
  mpiArcane->Type_size(sendtype, &sizeof_sendtype);
  mpiArcane->Type_size(recvtype, &sizeof_recvtype);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Scatter("
            << "sendcount=" << sendcount 
            << ", sizeof_sendtype=" << sizeof_sendtype 
            << ", recvcount=" << recvcount 
            << ", sizeof_recvtype=" << sizeof_recvtype 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  return mpiArcane->Scatter(
    sendbuf, sendcount, sendtype,
    recvbuf, recvcount, recvtype,
    root, comm);
}

int MPA_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs,
                 MPI_Datatype sendtype, 
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 int root, MPA_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Scatterv()" << std::endl;
  #endif

  return mpiArcane->Scatterv(
    sendbuf, sendcounts, displs, sendtype,
    recvbuf, recvcount, recvtype,
    root, comm);
}

int MPA_Probe(int source, int tag, MPA_Comm comm, MPA_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Probe(" 
            << "provenance=" << source
            << ", tag=" << tag 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  return mpiArcane->Probe(source, tag, comm, status);
}

int MPA_Iprobe(int source, int tag, MPA_Comm comm, int *flag, MPA_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Iprobe(" 
            << "provenance=" << source
            << ", tag=" << tag 
            << ", comm=" << comm 
            << ")" << std::endl;
  #endif

  return mpiArcane->Iprobe(source, tag, comm, flag, status);
}

int MPA_Get_count(const MPA_Status *status, MPI_Datatype datatype, int *count)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  #ifdef PRINT_CALL
  int world_rank;
  mpiArcane->Comm_rank(MPA_COMM_WORLD, &world_rank);
  std::cout << "[World Rank " << world_rank << "] --------------- MPA_Get_count()" << std::endl;
  #endif
  
  return mpiArcane->Get_count(status, datatype, count);
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

// On redirige tous les appels MPI vers nos fonctions.
#define MPI_Init MPA_Init
#define MPI_Initialized MPA_Initialized
#define MPI_Finalize MPA_Finalize
#define MPI_Abort MPA_Abort

#define MPI_Comm_split MPA_Comm_split
#define MPI_Comm_dup MPA_Comm_dup

#define MPI_Comm_size MPA_Comm_size
#define MPI_Comm_rank MPA_Comm_rank

#define MPI_Send MPA_Send
#define MPI_Recv MPA_Recv

#define MPI_Isend MPA_Isend
#define MPI_Irecv MPA_Irecv

#define MPI_Type_size MPA_Type_size

#define MPI_Wait MPA_Wait
#define MPI_Waitall MPA_Waitall
#define MPI_Waitany MPA_Waitany

#define MPI_Test MPA_Test
#define MPI_Testall MPA_Testall

#define MPI_Barrier MPA_Barrier

#define MPI_Bcast MPA_Bcast

#define MPI_Gather MPA_Gather
#define MPI_Gatherv MPA_Gatherv

#define MPI_Allgather MPA_Allgather
#define MPI_Allgatherv MPA_Allgatherv

#define MPI_Reduce MPA_Reduce
#define MPI_Allreduce MPA_Allreduce

#define MPI_Scatter MPA_Scatter
#define MPI_Scatterv MPA_Scatterv

#define MPI_Probe MPA_Probe
#define MPI_Iprobe MPA_Iprobe

#define MPI_Get_count MPA_Get_count

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
