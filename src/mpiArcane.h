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

#ifndef MPIARCANE_H
#define MPIARCANE_H

#include <arccore/message_passing_mpi/StandaloneMpiMessagePassingMng.h>
#include <arccore/message_passing/IMessagePassingMng.h>
#include <arccore/message_passing/Messages.h>
#include <arccore/message_passing/MessageRank.h>
#include <arccore/message_passing/MessageTag.h>
#include <arccore/message_passing/PointToPointMessageInfo.h>

#include <arccore/base/ArrayView.h>
#include <arccore/base/Span.h>
#include <arccore/collections/Array.h>
#include <arcane/utils/IThreadMng.h>
#include <arcane/utils/Ref.h>

#include <thread>
#include <map>

#define MPA_Comm int
#define MPA_COMM_WORLD -1
#define MPA_Request unsigned long
#define MPA_Request_null 0

using namespace Arccore;
using namespace Arccore::MessagePassing;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class MpiArcane
{
 public:
  MpiArcane()
  : m_isInit(false)
  {};

  virtual ~MpiArcane(){};

 public:
  int MpiArcane_Init(IParallelMng *iMPMng);
  int MpiArcane_Finalize();
  int MpiArcane_Abort(MPA_Comm comm, int errorcode);

  int MpiArcane_Comm_split(MPA_Comm comm, int color, int key, MPA_Comm *newcomm);
  int MpiArcane_Comm_dup(MPA_Comm old_comm, MPA_Comm* new_comm);

  int MpiArcane_Comm_size(MPA_Comm comm, int *size);
  int MpiArcane_Comm_rank(MPA_Comm comm, int *rank);

  int MpiArcane_Send(const void *buf, int sizeof_msg, int dest, int tag, MPA_Comm comm, MPA_Request *request, bool blocking);
  int MpiArcane_Recv(void *buf, int sizeof_msg, int source, int tag, MPA_Comm comm, MPA_Request *request, MessageId *status, bool blocking);

  int MpiArcane_Type_size(MPI_Datatype datatype, int *size);

  int MpiArcane_Wait(MPA_Request *request);
  int MpiArcane_Waitall(int count, MPA_Request *array_of_requests);
  int MpiArcane_Waitany(int count, MPA_Request *array_of_requests, int *index);

  int MpiArcane_Test(MPA_Request *request, int *flag);
  int MpiArcane_Testall(int count, MPA_Request *array_of_requests, int *flag);

  int MpiArcane_Barrier(MPA_Comm comm);

  int MpiArcane_Bcast(void *buffer, int sizeof_msg, int root, MPA_Comm comm);

  int MpiArcane_Gather(const void *sendbuf, int sizeof_sentmsg,
                             void *recvbuf, int sizeof_recvmsg,
                             int root, MPA_Comm comm);

  int MpiArcane_Gatherv(const void *sendbuf, int sizeof_sentmsg,
                              void *recvbuf, const int *recvcounts, const int *displs,
                              int sizeof_recvtype, int root, MPA_Comm comm);

  int MpiArcane_Allgather(const void *sendbuf, int sizeof_sentmsg,
                                void *recvbuf, int sizeof_recvmsg,
                                MPA_Comm comm);

  int MpiArcane_Allgatherv(const void *sendbuf, int sizeof_sentmsg,
                                 void *recvbuf, const int *recvcounts, const int *displs,
                                 int sizeof_recvtype, MPA_Comm comm);

  template<class T>
  int MpiArcane_Allreduce(const T *sendbuf, T *recvbuf, int sizeof_msg,
                          MPI_Op op, MPA_Comm comm);

  int MpiArcane_Scatter(const void *sendbuf, int sizeof_sentmsg,
                              void *recvbuf, int sizeof_recvmsg,
                              int root, MPA_Comm comm);

  int MpiArcane_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, int sizeof_sendtype, 
                               void *recvbuf, int sizeof_recvmsg,
                               int root, MPA_Comm comm);

  int MpiArcane_Probe(int source, int tag, MPA_Comm comm, MessageId *status);
  int MpiArcane_Iprobe(int source, int tag, MPA_Comm comm, int *flag, MessageId *status);

  int MpiArcane_Get_count(const MessageId *status, int sizeof_type, int *count);



 protected:
  bool m_isInit;
  UniqueArray<Ref<IParallelMng>> m_iPMng;
  UniqueArray<Request>* m_requests;
  std::map<std::thread::id, int> m_tids;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Init(IParallelMng *iPMng)
{
  if(m_isInit) return MPI_SUCCESS;

  iPMng->barrier();

  if(iPMng->commRank() == 0){
    m_requests = new UniqueArray<Request>[iPMng->commSize()];
    m_iPMng.resize(iPMng->commSize());
    m_isInit = true;
  }

  iPMng->barrier();

  m_requests[iPMng->commRank()].resize(1);
  m_requests[iPMng->commRank()][0].reset();

  iPMng->threadMng()->beginCriticalSection();
  m_iPMng[iPMng->commRank()] = makeRef(iPMng);
  m_tids[std::this_thread::get_id()] = iPMng->commRank();
  iPMng->threadMng()->endCriticalSection();

  std::cout << "Tid : " << std::this_thread::get_id() << " id : " << m_tids[std::this_thread::get_id()] << std::endl;

  iPMng->barrier();
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Finalize()
{
  if(!m_isInit) return MPI_SUCCESS;

  int rank, size;
  MpiArcane_Comm_rank(MPA_COMM_WORLD, &rank);
  MpiArcane_Comm_size(MPA_COMM_WORLD, &size);

  m_requests[rank].clear();

  MpiArcane_Barrier(MPA_COMM_WORLD);

  if(rank == 0){

    // for(int i = size; i < m_iPMng.size(); i++) {
    //   delete(m_iPMng[i].get());
    //   m_iPMng[i].reset();
    // }

    delete[] m_requests;
    m_iPMng.clear();
    m_isInit = false;
  }

  
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Abort(MPA_Comm comm, int errorcode)
{
  if(!m_isInit) return MPI_SUCCESS;
  m_isInit = false;

  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  m_requests[rank].clear();

  MpiArcane_Barrier(comm);

  if(rank == 0){

    // for(int i = 1; i < m_iPMng.size(); i++) {
    //   delete(m_iPMng[i]);
    // }

    delete[] m_requests;
    m_iPMng.clear();
  }
  ARCANE_FATAL("MPI_Abort() with error code: ");

  return MPI_SUCCESS;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Comm_split(MPA_Comm comm, int color, int key, MPA_Comm *newcomm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  int min = m_iPMng[comm]->reduce(ReduceMin, color);
  int max = m_iPMng[comm]->reduce(ReduceMax, color);

  UniqueArray<Integer> truc(1);
  truc[0] = (color == min ? m_iPMng[comm]->commRank() : -1);

  UniqueArray<Integer> truc2(m_iPMng[comm]->commSize());

  m_iPMng[comm]->allGather(truc, truc2);

  UniqueArray<Integer> final_min;
  for(Integer elem : truc2){
    if(elem != -1)
      final_min.add(elem);
  }

  truc[0] = (color == max ? m_iPMng[comm]->commRank() : -1);
  m_iPMng[comm]->allGather(truc, truc2);

  UniqueArray<Integer> final_max;
  for(Integer elem : truc2){
    if(elem != -1)
      final_max.add(elem);
  }

  Ref<IParallelMng> new_iPMng;

  if(color == min){
    new_iPMng = m_iPMng[comm]->createSubParallelMngRef(final_min);
    m_iPMng[comm]->createSubParallelMngRef(final_max);
  }
  else{
    m_iPMng[comm]->createSubParallelMngRef(final_min);
    new_iPMng = m_iPMng[comm]->createSubParallelMngRef(final_max);
  }

  m_iPMng[comm]->threadMng()->beginCriticalSection();
  m_iPMng.add(new_iPMng);
  *newcomm = m_iPMng.size() - 1;
  m_iPMng[comm]->threadMng()->endCriticalSection();
  
   return MPI_SUCCESS;
}


int MpiArcane::
MpiArcane_Comm_dup(MPA_Comm comm, MPA_Comm *newcomm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  m_iPMng[comm]->threadMng()->beginCriticalSection();
  m_iPMng.add(Ref<IParallelMng>(m_iPMng[comm]));
  *newcomm = m_iPMng.size() - 1;
  m_iPMng[comm]->threadMng()->endCriticalSection();

  return MPI_SUCCESS;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Comm_size(MPA_Comm comm, int *size)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];
  *size = m_iPMng[comm]->commSize();
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Comm_rank(MPA_Comm comm, int *rank)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];
  *rank = m_iPMng[comm]->commRank();
  return MPI_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Send(const void *buf, int sizeof_msg, int dest, int tag, MPA_Comm comm, MPA_Request *request, bool blocking)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  PointToPointMessageInfo p2pMsgInfo(MessageRank(dest), MessageTag(tag), (blocking ? Blocking : NonBlocking));

  ByteArrayView avBuf(sizeof_msg, (Byte*)buf);

  Request arc_request = m_iPMng[comm]->send(avBuf, p2pMsgInfo);

  // Pas besoin de save la request si bloquant.
  if(!blocking){
    m_requests[rank].add(arc_request);
    *request = m_requests[rank].size()-1;
  }
  //else{
  //  UniqueArray<Request> reqs(1);
  //  reqs[0] = arc_request;
  //  m_iPMng[comm]->freeRequests(reqs);
  //}

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Recv(void *buf, int sizeof_msg, int source, int tag, MPA_Comm comm, MPA_Request *request, MessageId *status, bool blocking)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];
    
  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  PointToPointMessageInfo p2pMsgInfo;
  
  if(status->isValid()) {
   p2pMsgInfo = PointToPointMessageInfo(*status);
  }
  else{
   p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), (blocking ? Blocking : NonBlocking));
  }

  ByteArrayView avBuf(sizeof_msg, (Byte*)buf);

  Request arc_request = m_iPMng[comm]->receive(avBuf, p2pMsgInfo);

  // Pas besoin de save la request si bloquant.
  if(!blocking){
    m_requests[rank].add(arc_request);
    *request = m_requests[rank].size()-1;
  }
  //else{
    //UniqueArray<Request> reqs(1);
    //reqs[0] = arc_request;
    //m_iPMng[comm]->freeRequests(reqs);
  //}

  return MPI_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Type_size(MPI_Datatype datatype, int *size)
{
  return MPI_Type_size(datatype, size);
}

int MpiArcane::
MpiArcane_Wait(MPA_Request *request)
{
  return MpiArcane_Waitall(1, request);
}

int MpiArcane::
MpiArcane_Waitall(int count, MPA_Request *array_of_requests)
{
  int rank;
  MpiArcane_Comm_rank(MPA_COMM_WORLD, &rank);

  UniqueArray<Request> arc_reqs(count);
  for(int i = 0; i < count; i++){
    arc_reqs[i] = m_requests[rank][array_of_requests[i]];
  }
  m_iPMng[m_tids[std::this_thread::get_id()]]->waitAllRequests(arc_reqs);
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Waitany(int count, MPA_Request *array_of_requests, int *index)
{
  int i = 0;
  for(;;){
    if(i >= count) i = 0;
    int flag;
    MpiArcane_Test(&array_of_requests[i], &flag);
    if(flag == 1) break;
    i++;
  }

  *index = i;
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Test(MPA_Request *request, int *flag)
{
  if(*request == MPA_Request_null){
    *flag = 2;
    return MPI_SUCCESS;
  }
  int rank;
  MpiArcane_Comm_rank(MPA_COMM_WORLD, &rank);

  UniqueArray<Request> arc_reqs(1);
  arc_reqs[0] = m_requests[rank][*request];

  UniqueArray<Integer> done_requests = m_iPMng[m_tids[std::this_thread::get_id()]]->testSomeRequests(arc_reqs);


  if(done_requests.size() == 1){
    *flag = 1;
    *request = MPA_Request_null;
  }
  else{
    *flag = 0;
  }

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Testall(int count, MPA_Request *array_of_requests, int *flag)
{
  int rank;
  MpiArcane_Comm_rank(MPA_COMM_WORLD, &rank);

  Integer num_of_done_requests = 0;

  UniqueArray<Request> arc_reqs;
  UniqueArray<Integer> pos_to_pos;

  for(int i = 0; i < count; i++){
    if(array_of_requests[i] == MPA_Request_null){
      num_of_done_requests++;
    }
    else{
      arc_reqs.add(m_requests[rank][array_of_requests[i]]);
      pos_to_pos.add(i);
    }
  }

  UniqueArray<Integer> done_requests = m_iPMng[m_tids[std::this_thread::get_id()]]->testSomeRequests(arc_reqs);

  for(int i = 0; i < done_requests.size(); i++){
    num_of_done_requests++;
    array_of_requests[pos_to_pos[done_requests[i]]] = MPA_Request_null;
  }

  *flag = (num_of_done_requests == count);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Barrier(MPA_Comm comm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];
  m_iPMng[comm]->barrier();
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Bcast(void *buffer, int sizeof_msg, int root, MPA_Comm comm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  ByteArrayView avBuf(sizeof_msg, (Byte*)buffer);

  m_iPMng[comm]->broadcast(avBuf, root);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Gather(const void *sendbuf, int sizeof_sentmsg,
                 void *recvbuf, int sizeof_recvmsg,
                 int root, MPA_Comm comm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  ByteArrayView avRecvBuf(sizeof_recvmsg, (Byte*)recvbuf);

  m_iPMng[comm]->gather(avSendBuf, avRecvBuf, root);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Gatherv(const void *sendbuf, int sizeof_sentmsg,
                  void *recvbuf, const int *recvcounts, const int *displs,
                  int sizeof_recvtype, int root, MPA_Comm comm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  UniqueArray<Byte> uaRecv;

  m_iPMng[comm]->gatherVariable(avSendBuf, uaRecv, root);

  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  if(rank == root){
    int size;
    MpiArcane_Comm_size(comm, &size);

    ByteArrayView avRecvBuf(uaRecv.size(), (Byte*)recvbuf);
    int pos = 0;
    for(int i = 0; i < size; i++){
      for(
        int j = displs[i]*sizeof_recvtype; 
        j < (displs[i]+recvcounts[i]) * sizeof_recvtype; 
        j++)
      {
        avRecvBuf[j] = uaRecv[pos++];
      }
    }
  }

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Allgather(const void *sendbuf, int sizeof_sentmsg,
                    void *recvbuf, int sizeof_recvmsg,
                    MPA_Comm comm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  ByteArrayView avRecvBuf(sizeof_recvmsg, (Byte*)recvbuf);

  m_iPMng[comm]->allGather(avSendBuf, avRecvBuf);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Allgatherv(const void *sendbuf, int sizeof_sentmsg,
                                       void *recvbuf, const int *recvcounts, const int *displs,
                                       int sizeof_recvtype, MPA_Comm comm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  UniqueArray<Byte> uaRecv;

  m_iPMng[comm]->allGatherVariable(avSendBuf, uaRecv);

  int size;
  MpiArcane_Comm_size(comm, &size);

  ByteArrayView avRecvBuf(uaRecv.size(), (Byte*)recvbuf);
  int pos = 0;
  for(int i = 0; i < size; i++){
    for(
      int j = displs[i]*sizeof_recvtype; 
      j < (displs[i]+recvcounts[i]) * sizeof_recvtype; 
      j++)
    {
      avRecvBuf[j] = uaRecv[pos++];
    }
  }
  
  return MPI_SUCCESS;
}

template<class T>
int MpiArcane::
MpiArcane_Allreduce(const T *sendbuf, T *recvbuf, int sizeof_msg,
                     MPI_Op op, MPA_Comm comm)
{
  eReduceType rtype;
  if(op == MPI_MIN)
    rtype = ReduceMin;
  else if(op == MPI_MAX)
    rtype = ReduceMax;
  else if(op == MPI_SUM)
    rtype = ReduceSum;
  else if(op == MPI_OP_NULL){
    std::cout << "[MPI_ERR_TYPE] MPI_OP_NULL not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_PROD){
    std::cout << "[MPI_ERR_TYPE] MPI_PROD not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_LAND){
    std::cout << "[MPI_ERR_TYPE] MPI_LAND not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_BAND){
    std::cout << "[MPI_ERR_TYPE] MPI_BAND not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_LOR){
    std::cout << "[MPI_ERR_TYPE] MPI_LOR not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_BOR){
    std::cout << "[MPI_ERR_TYPE] MPI_BOR not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_LXOR){
    std::cout << "[MPI_ERR_TYPE] MPI_LXOR not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_BXOR){
    std::cout << "[MPI_ERR_TYPE] MPI_BXOR not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_MINLOC){
    std::cout << "[MPI_ERR_TYPE] MPI_MINLOC not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_MAXLOC){
    std::cout << "[MPI_ERR_TYPE] MPI_MAXLOC not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else if(op == MPI_REPLACE){
    std::cout << "[MPI_ERR_TYPE] MPI_REPLACE not supported yet" << std::endl;
    return MPI_ERR_TYPE;
  }
  else {
    std::cout << "[MPI_ERR_TYPE] Invalid op" << std::endl;
    return MPI_ERR_TYPE;
  }

  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  ArrayView<T> avSendBuf(sizeof_msg, (T*)sendbuf);
  ArrayView<T> avRecvBuf(sizeof_msg, (T*)recvbuf);

  avRecvBuf.copy(avSendBuf);

  m_iPMng[comm]->reduce(rtype, avRecvBuf);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Scatter(const void *sendbuf, int sizeof_sentmsg,
                            void *recvbuf, int sizeof_recvmsg,
                            int root, MPA_Comm comm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  if(rank == root) {
    int size;
    MpiArcane_Comm_size(comm, &size);

    for(int i = 0; i < size; i++){
      if(i == root) continue;
      MPA_Request req;
      MpiArcane_Send(((Byte*)sendbuf) + (i*sizeof_recvmsg), sizeof_recvmsg, i, 0, comm, &req, false);
    }
    for(int i = 0; i < sizeof_recvmsg; i++){
      ((Byte*)recvbuf)[i] = *(((Byte*)sendbuf) + (rank*sizeof_recvmsg+i));
    }
  }
  else{
    MPA_Request req;
    MessageId mId;
    MpiArcane_Recv(recvbuf, sizeof_recvmsg, root, 0, comm, &req, &mId, true);
  }
  // Barrier car root se desync sinon.
  MpiArcane_Barrier(comm);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, int sizeof_sendtype, 
                       void *recvbuf, int sizeof_recvmsg,
                       int root, MPA_Comm comm)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];

  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  if(rank == root) {
    int size;
    MpiArcane_Comm_size(comm, &size);

    int pos = 0;
    for(int i = 0; i < size; i++){
      if(i == root) continue;
      MPA_Request req;
      MpiArcane_Send(((Byte*)sendbuf) + (displs[i]*sizeof_sendtype), sendcounts[i]*sizeof_sendtype, i, 0, comm, &req, false);
    }
    for(int i = 0; i < sizeof_recvmsg; i++){
      ((Byte*)recvbuf)[i] = *(((Byte*)sendbuf) + (displs[rank]*sizeof_recvmsg+i));
    }
  }
  else{
    MPA_Request req;
    MessageId mId;
    MpiArcane_Recv(recvbuf, sizeof_recvmsg, root, 0, comm, &req, &mId, true);
  }

  // Barrier car root se desync sinon.
  MpiArcane_Barrier(comm);

  return MPI_SUCCESS;
}


int MpiArcane::
MpiArcane_Probe(int source, int tag, MPA_Comm comm, MessageId *status)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];
  PointToPointMessageInfo p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), NonBlocking);

  MessageSourceInfo msi;

  do{
    msi = mpLegacyProbe(m_iPMng[comm]->messagePassingMng(), p2pMsgInfo);
  } while(!msi.isValid());

  status->setSourceInfo(msi);
  
  return MPI_SUCCESS;
}


int MpiArcane::
MpiArcane_Iprobe(int source, int tag, MPA_Comm comm, int *flag, MessageId *status)
{
  if(comm == MPA_COMM_WORLD)
    comm = m_tids[std::this_thread::get_id()];
  PointToPointMessageInfo p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), NonBlocking);

  status->setSourceInfo(mpLegacyProbe(m_iPMng[comm]->messagePassingMng(), p2pMsgInfo));
  
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Get_count(const MessageId *status, int sizeof_type, int *count)
{
  *count = int(status->sourceInfo().size() / sizeof_type);
  return MPI_SUCCESS;
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
