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

#define MPA_Comm int
#define MPA_COMM_WORLD 0
#define MPA_Request unsigned long

using namespace Arccore;
using namespace Arccore::MessagePassing;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class MpiArcane
{
 public:
  MpiArcane()
  : isInit(false)
  {};

  virtual ~MpiArcane(){};

 public:
  int MpiArcane_Init(int *argc, char ***argv);
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
  bool isInit;
  UniqueArray<IMessagePassingMng*> m_iMPMng;
  UniqueArray<MPI_Comm> m_commOfMPMng;
  UniqueArray<Request> m_requests;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Init(int *argc, char ***argv)
{
  int ret = MPI_Init(argc, argv);
  
  if(ret == MPI_SUCCESS){
    m_requests.resize(1);
    m_iMPMng.resize(1);
    m_commOfMPMng.resize(1);

    m_iMPMng[MPA_COMM_WORLD] = Mpi::StandaloneMpiMessagePassingMng::create(MPI_COMM_WORLD);
    m_commOfMPMng[MPA_COMM_WORLD] = MPI_COMM_WORLD;

    isInit = true;
  }

  return ret;
}

int MpiArcane::
MpiArcane_Finalize()
{
  if(!isInit) return MPI_SUCCESS;
  isInit = false;

  for(int i = 0; i < m_iMPMng.size(); i++) {
    mpDelete(m_iMPMng[i]);
  }

  m_requests.clear();
  m_iMPMng.clear();
  m_commOfMPMng.clear();
  
  return MPI_Finalize();
}

int MpiArcane::
MpiArcane_Abort(MPA_Comm comm, int errorcode)
{
  if(!isInit) return MPI_SUCCESS;
  isInit = false;

  MPI_Comm comm_abort = m_commOfMPMng[comm];

  for(int i = 0; i < m_iMPMng.size(); i++) {
    mpDelete(m_iMPMng[i]);
  }

  m_requests.clear();
  m_iMPMng.clear();
  m_commOfMPMng.clear();

  return MPI_Abort(comm_abort, errorcode);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Comm_split(MPA_Comm comm, int color, int key, MPA_Comm *newcomm)
{
  MPI_Comm new_comm;
  MPI_Comm mpi_comm = m_commOfMPMng[comm];

  int error = MPI_Comm_split(mpi_comm, color, key, &new_comm);
  if(error != MPI_SUCCESS) return error;

  m_iMPMng.add(Mpi::StandaloneMpiMessagePassingMng::create(new_comm));
  m_commOfMPMng.add(new_comm);

  *newcomm = m_commOfMPMng.size() - 1;

  return MPI_SUCCESS;
}


int MpiArcane::
MpiArcane_Comm_dup(MPA_Comm comm, MPA_Comm *newcomm)
{
  MPI_Comm mpi_new_comm;
  MPI_Comm mpi_old_comm = m_commOfMPMng[comm];

  int error = MPI_Comm_dup(mpi_old_comm, &mpi_new_comm);
  if(error != MPI_SUCCESS) return error;

  m_iMPMng.add(Mpi::StandaloneMpiMessagePassingMng::create(mpi_new_comm));
  m_commOfMPMng.add(mpi_new_comm);

  *newcomm = m_commOfMPMng.size() - 1;

  return MPI_SUCCESS;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Comm_size(MPA_Comm comm, int *size)
{
  *size = m_iMPMng[comm]->commSize();
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Comm_rank(MPA_Comm comm, int *rank)
{
  *rank = m_iMPMng[comm]->commRank();
  return MPI_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Send(const void *buf, int sizeof_msg, int dest, int tag, MPA_Comm comm, MPA_Request *request, bool blocking)
{
  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  PointToPointMessageInfo p2pMsgInfo(MessageRank(rank), MessageRank(dest), MessageTag(tag), (blocking ? Blocking : NonBlocking));

  ByteArrayView avBuf(sizeof_msg, (Byte*)buf);

  Request arc_request = mpSend(m_iMPMng[comm], avBuf, p2pMsgInfo);

  // Pas besoin de save la request si bloquant.
  if(!blocking){
    m_requests.add(arc_request);
    *request = m_requests.size()-1;
  }

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Recv(void *buf, int sizeof_msg, int source, int tag, MPA_Comm comm, MPA_Request *request, MessageId *status, bool blocking)
{
  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  PointToPointMessageInfo p2pMsgInfo;
  
  if(status->isValid()) {
    p2pMsgInfo = PointToPointMessageInfo(*status);
  }
  else{
    p2pMsgInfo = PointToPointMessageInfo(MessageRank(rank), MessageRank(source), MessageTag(tag), (blocking ? Blocking : NonBlocking));
  }

  ByteArrayView avBuf(sizeof_msg, (Byte*)buf);

  Request arc_request = mpReceive(m_iMPMng[comm], avBuf, p2pMsgInfo);

  // Pas besoin de save la request si bloquant.
  if(!blocking){
    m_requests.add(arc_request);
    *request = m_requests.size()-1;
  }

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
  mpWait(m_iMPMng[MPA_COMM_WORLD], m_requests[*request]);
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Waitall(int count, MPA_Request *array_of_requests)
{
  UniqueArray<Request> arc_reqs(count);
  for(int i = 0; i < count; i++){
    arc_reqs[i] = m_requests[array_of_requests[i]];
  }
  mpWaitAll(m_iMPMng[MPA_COMM_WORLD], arc_reqs);
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Waitany(int count, MPA_Request *array_of_requests, int *index)
{
  // TODO : Faire mieux.
  int i = 0;
  for(;;){
    int flag;
    MpiArcane_Test(&array_of_requests[i], &flag);
    if(flag) break;
    if(i++ >= count) i = 0;
  }

  

  *index = i;
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Test(MPA_Request *request, int *flag)
{
  UniqueArray<Request> arc_reqs(1);
  arc_reqs[0] = m_requests[*request];

  bool bFlag;
  ArrayView<bool> avFlag(1, &bFlag);

  mpTestSome(m_iMPMng[MPA_COMM_WORLD], arc_reqs, avFlag);
  *flag = bFlag;

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Testall(int count, MPA_Request *array_of_requests, int *flag)
{
  UniqueArray<Request> arc_reqs(count);
  for(int i = 0; i < count; i++){
    arc_reqs[i] = m_requests[array_of_requests[i]];
  }

  UniqueArray<bool> flags(count);

  mpTestSome(m_iMPMng[MPA_COMM_WORLD], arc_reqs, flags);

  for(int i = 1; i < count; i++){
    flags[0] += flags[i];
  }
  *flag = flags[0];

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Barrier(MPA_Comm comm)
{
  mpBarrier(m_iMPMng[comm]);
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Bcast(void *buffer, int sizeof_msg, int root, MPA_Comm comm)
{
  ByteArrayView avBuf(sizeof_msg, (Byte*)buffer);

  mpBroadcast(m_iMPMng[comm], avBuf, root);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Gather(const void *sendbuf, int sizeof_sentmsg,
                 void *recvbuf, int sizeof_recvmsg,
                 int root, MPA_Comm comm)
{
  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  ByteArrayView avRecvBuf(sizeof_recvmsg, (Byte*)recvbuf);

  mpGather(m_iMPMng[comm], avSendBuf, avRecvBuf, root);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Gatherv(const void *sendbuf, int sizeof_sentmsg,
                  void *recvbuf, const int *recvcounts, const int *displs,
                  int sizeof_recvtype, int root, MPA_Comm comm)
{
  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  UniqueArray<Byte> uaRecv;
  mpGatherVariable(m_iMPMng[comm], avSendBuf, uaRecv, root);

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
  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  ByteArrayView avRecvBuf(sizeof_recvmsg, (Byte*)recvbuf);

  mpAllGather(m_iMPMng[comm], avSendBuf, avRecvBuf);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Allgatherv(const void *sendbuf, int sizeof_sentmsg,
                                       void *recvbuf, const int *recvcounts, const int *displs,
                                       int sizeof_recvtype, MPA_Comm comm)
{
  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  UniqueArray<Byte> uaRecv;
  mpAllGatherVariable(m_iMPMng[comm], avSendBuf, uaRecv);

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

  ArrayView<T> avSendBuf(sizeof_msg, (T*)sendbuf);
  ArrayView<T> avRecvBuf(sizeof_msg, (T*)recvbuf);

  avRecvBuf.copy(avSendBuf);

  mpAllReduce(m_iMPMng[comm], rtype, avRecvBuf);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Scatter(const void *sendbuf, int sizeof_sentmsg,
                            void *recvbuf, int sizeof_recvmsg,
                            int root, MPA_Comm comm)
{
  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  if(rank == root) {
    int size;
    MpiArcane_Comm_size(comm, &size);

    int pos = 0;
    for(int i = 0; i < size; i++){        
      MPA_Request req;
      MpiArcane_Send(((Byte*)sendbuf) + (i*sizeof_recvmsg), sizeof_recvmsg, i, 0, comm, &req, false);
    }
  }
  MPA_Request req;
  MessageId mId;
  MpiArcane_Recv(recvbuf, sizeof_recvmsg, root, 0, comm, &req, &mId, false);

  MpiArcane_Barrier(comm);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, int sizeof_sendtype, 
                       void *recvbuf, int sizeof_recvmsg,
                       int root, MPA_Comm comm)
{
  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  if(rank == root) {
    int size;
    MpiArcane_Comm_size(comm, &size);

    int pos = 0;
    for(int i = 0; i < size; i++){
      MPA_Request req;
      MpiArcane_Send(((Byte*)sendbuf) + (displs[i]*sizeof_sendtype), sendcounts[i]*sizeof_sendtype, i, 0, comm, &req, false);
    }
  }

  MPA_Request req;
  MessageId mId;
  MpiArcane_Recv(recvbuf, sizeof_recvmsg, root, 0, comm, &req, &mId, false);

  MpiArcane_Barrier(comm);

  return MPI_SUCCESS;
}


int MpiArcane::
MpiArcane_Probe(int source, int tag, MPA_Comm comm, MessageId *status)
{
  // PointToPointMessageInfo p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), Blocking);
  // *status = mpProbe(m_iMPMng[comm], p2pMsgInfo);

  // return MPI_SUCCESS;

  // TODO : Faire autrement.

  MPI_Comm mpi_comm = m_commOfMPMng[comm];
  MPI_Status mpi_status;

  int error = MPI_Probe(source, tag, mpi_comm, &mpi_status);

  if(error != MPI_SUCCESS) return error;

  MessageId::SourceInfo sourceInfo(MessageRank(mpi_status.MPI_SOURCE), MessageTag(mpi_status.MPI_TAG), mpi_status._ucount);
  status->setSourceInfo(sourceInfo);

  return error;
}


int MpiArcane::
MpiArcane_Iprobe(int source, int tag, MPA_Comm comm, int *flag, MessageId *status)
{
  // TODO : Faire autrement.
  // PointToPointMessageInfo p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), NonBlocking);
  // *status = mpProbe(m_iMPMng[comm], p2pMsgInfo);
  // *flag = status->isValid();

  // return MPI_SUCCESS;

  MPI_Comm mpi_comm = m_commOfMPMng[comm];
  MPI_Status mpi_status;

  int error = MPI_Iprobe(source, tag, mpi_comm, flag, &mpi_status);

  if(error != MPI_SUCCESS) return error;

  MessageId::SourceInfo sourceInfo(MessageRank(mpi_status.MPI_SOURCE), MessageTag(mpi_status.MPI_TAG), mpi_status._ucount);
  status->setSourceInfo(sourceInfo);

  return error;
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
