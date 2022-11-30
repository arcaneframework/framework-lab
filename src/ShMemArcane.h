// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* mpiArcane.h                                                 (C) 2000-2022 */
/*                                                                           */
/* Fichier contenant la classe ShMemArcane permettant de faire les appels vers */
/* Arcane à partir des infos obtenus des fonctions de mpA.h.                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifndef SHMEMARCANE_H
#define SHMEMARCANE_H


#include <arccore/message_passing_mpi/StandaloneMpiMessagePassingMng.h>
#include <arccore/message_passing/IMessagePassingMng.h>
#include <arccore/message_passing/Messages.h>
#include <arccore/message_passing/MessageRank.h>
#include <arccore/message_passing/MessageTag.h>
#include <arccore/message_passing/PointToPointMessageInfo.h>

#include <arccore/base/String.h>
#include <arccore/base/FatalErrorException.h>
#include <arccore/base/ArrayView.h>
#include <arccore/base/Span.h>

#include <arccore/collections/Array.h>

#include <arcane/utils/IThreadMng.h>
#include <arcane/utils/Ref.h>

#include <arcane/IParallelMng.h>

#include <thread>
#include <map>

#include "IPMngArcane.h"

using namespace Arcane;
using namespace Arccore::MessagePassing;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#define TID m_tids[std::this_thread::get_id()]

class ShMemArcane : 
public IPMngArcane
{
 public:
  ShMemArcane()
  : m_isInit(false)
  {};

  virtual ~ShMemArcane(){};

 public:
  int Init(IParallelMng *iMPMng) override;
  int Finalize() override;
  int Abort(MPA_Comm comm, int errorcode) override;

  int Comm_split(MPA_Comm comm, int color, int key, MPA_Comm *newcomm) override;
  int Comm_dup(MPA_Comm old_comm, MPA_Comm* new_comm) override;

  int Comm_size(MPA_Comm comm, int *size) override;
  int Comm_rank(MPA_Comm comm, int *rank) override;

  int Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPA_Comm comm, MPA_Request *request, bool blocking) override;
  int Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPA_Comm comm, MPA_Request *request, MessageId *status, bool blocking) override;

  int Type_size(MPI_Datatype datatype, int *size) override;

  int Wait(MPA_Request *request) override;
  int Waitall(int count, MPA_Request *array_of_requests) override;
  int Waitany(int count, MPA_Request *array_of_requests, int *index) override;

  int Test(MPA_Request *request, int *flag) override;
  int Testall(int count, MPA_Request *array_of_requests, int *flag) override;

  int Barrier(MPA_Comm comm) override;

  int Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPA_Comm comm) override;

  int Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                             void *recvbuf, int recvcount, MPI_Datatype recvtype,
                             int root, MPA_Comm comm) override;

  int Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                              void *recvbuf, const int *recvcounts, const int *displs,
                              MPI_Datatype recvtype, int root, MPA_Comm comm) override;

  int Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                                MPA_Comm comm) override;

  int Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                                 void *recvbuf, const int *recvcounts, const int *displs,
                                 MPI_Datatype recvtype, MPA_Comm comm) override;

  int Allreduce(const void *sendbuf, void *recvbuf, int sizeof_msg,
                          MPI_Datatype datatype, MPI_Op op, MPA_Comm comm) override;


  int Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                              void *recvbuf, int recvcount, MPI_Datatype recvtype,
                              int root, MPA_Comm comm) override;

  int Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, MPI_Datatype sendtype, 
                               void *recvbuf, int recvcount, MPI_Datatype recvtype,
                               int root, MPA_Comm comm) override;

  int Probe(int source, int tag, MPA_Comm comm, MessageId *status) override;
  int Iprobe(int source, int tag, MPA_Comm comm, int *flag, MessageId *status) override;

  int Get_count(const MessageId *status, MPI_Datatype datatype, int *count) override;

 private:
  template<class T>
  int _allReduce(const T *sendbuf, T *recvbuf, int sizeof_msg,
                          eReduceType op, MPA_Comm comm);

 protected:
  bool m_isInit;
  UniqueArray<Ref<IParallelMng>>* m_iPMng;
  UniqueArray<Request>* m_requests;
  std::map<std::thread::id, int> m_tids;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

template<class T>
int ShMemArcane::
_allReduce(const T *sendbuf, T *recvbuf, int sizeof_msg,
           eReduceType reduceType, MPA_Comm comm)
{
  ArrayView<T> avSendBuf(sizeof_msg, (T*)sendbuf);
  ArrayView<T> avRecvBuf(sizeof_msg, (T*)recvbuf);

  avRecvBuf.copy(avSendBuf);

  m_iPMng[TID][comm]->reduce(reduceType, avRecvBuf);

  return MPI_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
