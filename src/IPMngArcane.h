// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* mpiArcane.h                                                 (C) 2000-2022 */
/*                                                                           */
/* Fichier contenant la classe MpiArcane permettant de faire les appels vers */
/* Arcane à partir des infos obtenus des fonctions de mpA.h.                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef IPMNGARCANE_H
#define IPMNGARCANE_H

#include <arccore/message_passing_mpi/StandaloneMpiMessagePassingMng.h>
#include <arccore/message_passing/IMessagePassingMng.h>

#include <arccore/collections/Array.h>

#include <arcane/IParallelMng.h>

#define MPA_Comm int
#define MPA_COMM_WORLD 0
#define MPA_Request unsigned long
#define MPA_Request_null 0

using namespace Arcane;
using namespace Arccore::MessagePassing;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class IPMngArcane
{
 public:
  virtual ~IPMngArcane() = default;

 public:
  virtual int Init(IParallelMng *iMPMng) = 0;
  virtual int Finalize() = 0;
  virtual int Abort(MPA_Comm comm, int errorcode) = 0;

  virtual int Comm_split(MPA_Comm comm, int color, int key, MPA_Comm *newcomm) = 0;
  virtual int Comm_dup(MPA_Comm old_comm, MPA_Comm* new_comm) = 0;

  virtual int Comm_size(MPA_Comm comm, int *size) = 0;
  virtual int Comm_rank(MPA_Comm comm, int *rank) = 0;

  virtual int Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPA_Comm comm, MPA_Request *request, bool blocking) = 0;
  virtual int Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPA_Comm comm, MPA_Request *request, MessageId *status, bool blocking) = 0;

  virtual int Type_size(MPI_Datatype datatype, int *size) = 0;

  virtual int Wait(MPA_Request *request) = 0;
  virtual int Waitall(int count, MPA_Request *array_of_requests) = 0;
  virtual int Waitany(int count, MPA_Request *array_of_requests, int *index) = 0;

  virtual int Test(MPA_Request *request, int *flag) = 0;
  virtual int Testall(int count, MPA_Request *array_of_requests, int *flag) = 0;

  virtual int Barrier(MPA_Comm comm) = 0;

  virtual int Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPA_Comm comm) = 0;

  virtual int Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                             void *recvbuf, int recvcount, MPI_Datatype recvtype,
                             int root, MPA_Comm comm) = 0;

  virtual int Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                              void *recvbuf, const int *recvcounts, const int *displs,
                              MPI_Datatype recvtype, int root, MPA_Comm comm) = 0;

  virtual int Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                                MPA_Comm comm) = 0;

  virtual int Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                 void *recvbuf, const int *recvcounts, const int *displs,
                                 MPI_Datatype recvtype, MPA_Comm comm) = 0;

  virtual int Allreduce(const void *sendbuf, void *recvbuf, int sizeof_msg,
                                  MPI_Datatype datatype, MPI_Op op, MPA_Comm comm) = 0;

  virtual int Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                              void *recvbuf, int recvcount, MPI_Datatype recvtype,
                              int root, MPA_Comm comm) = 0;

  virtual int Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, MPI_Datatype sendtype, 
                               void *recvbuf, int recvcount, MPI_Datatype recvtype,
                               int root, MPA_Comm comm) = 0;

  virtual int Probe(int source, int tag, MPA_Comm comm, MessageId *status) = 0;
  virtual int Iprobe(int source, int tag, MPA_Comm comm, int *flag, MessageId *status) = 0;

  virtual int Get_count(const MessageId *status, MPI_Datatype datatype, int *count) = 0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
