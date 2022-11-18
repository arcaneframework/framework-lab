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

#define PRINT_CALL true

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

  int MpiArcane_Send(const void *buf, int sizeof_msg, int dest, int tag, MPA_Comm comm, Request *request, bool blocking);
  int MpiArcane_Recv(void *buf, int sizeof_msg, int source, int tag, MPA_Comm comm, Request *request, MessageId *status, bool blocking);

  int MpiArcane_Type_size(MPI_Datatype datatype, int *size);

  int MpiArcane_Wait(Request *request);
  int MpiArcane_Waitall(int count, Request *array_of_requests);
  int MpiArcane_Waitany(int count, Request *array_of_requests, int *index);

  int MpiArcane_Test(Request *request, int *flag);
  int MpiArcane_Testall(int count, Request *array_of_requests, int *flag);

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
  UniqueArray<IMessagePassingMng*> iMPMng;
  UniqueArray<MPI_Comm> commOfMPMng;
  int world_rank;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Init(int *argc, char ***argv)
{
  #ifdef PRINT_CALL
  std::cout << "--------------- MpiArcane_Init()" << std::endl;
  #endif

  int ret = MPI_Init(argc, argv);
  
  if(ret == MPI_SUCCESS){
    iMPMng.resize(1);
    commOfMPMng.resize(1);

    iMPMng[MPA_COMM_WORLD] = Mpi::StandaloneMpiMessagePassingMng::create(MPI_COMM_WORLD);
    commOfMPMng[MPA_COMM_WORLD] = MPI_COMM_WORLD;

    world_rank = iMPMng[MPA_COMM_WORLD]->commRank();

    isInit = true;
  }

  return ret;
}

int MpiArcane::
MpiArcane_Finalize()
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Finalize()" << std::endl;
  #endif

  if(!isInit) return MPI_SUCCESS;
  isInit = false;

  for(int i = 0; i < iMPMng.size(); i++) {
    mpDelete(iMPMng[i]);
  }

  iMPMng.clear();
  commOfMPMng.clear();
  
  return MPI_Finalize();
}

int MpiArcane::
MpiArcane_Abort(MPA_Comm comm, int errorcode)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Abort()" << std::endl;
  #endif

  if(!isInit) return MPI_SUCCESS;
  isInit = false;

  MPI_Comm comm_abort = commOfMPMng[comm];

  for(int i = 0; i < iMPMng.size(); i++) {
    mpDelete(iMPMng[i]);
  }

  iMPMng.clear();
  commOfMPMng.clear();

  return MPI_Abort(comm_abort, errorcode);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Comm_split(MPA_Comm comm, int color, int key, MPA_Comm *newcomm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Comm_split()" << std::endl;
  #endif

  MPI_Comm new_comm;
  MPI_Comm mpi_comm = commOfMPMng[comm];

  int error = MPI_Comm_split(mpi_comm, color, key, &new_comm);
  if(error != MPI_SUCCESS) return error;

  iMPMng.add(Mpi::StandaloneMpiMessagePassingMng::create(new_comm));
  commOfMPMng.add(new_comm);

  *newcomm = commOfMPMng.size() - 1;

  return MPI_SUCCESS;
}


int MpiArcane::
MpiArcane_Comm_dup(MPA_Comm comm, MPA_Comm *newcomm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Comm_dup()" << std::endl;
  #endif
  MPI_Comm mpi_new_comm;
  MPI_Comm mpi_old_comm = commOfMPMng[comm];

  int error = MPI_Comm_dup(mpi_old_comm, &mpi_new_comm);
  if(error != MPI_SUCCESS) return error;

  iMPMng.add(Mpi::StandaloneMpiMessagePassingMng::create(mpi_new_comm));
  commOfMPMng.add(mpi_new_comm);

  *newcomm = commOfMPMng.size() - 1;

  return MPI_SUCCESS;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Comm_size(MPA_Comm comm, int *size)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Comm_size()" << std::endl;
  #endif

  *size = iMPMng[comm]->commSize();
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Comm_rank(MPA_Comm comm, int *rank)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Comm_rank()" << std::endl;
  #endif

  *rank = iMPMng[comm]->commRank();
  return MPI_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Send(const void *buf, int sizeof_msg, int dest, int tag, MPA_Comm comm, Request *request, bool blocking)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Send()" << std::endl;
  #endif

  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  PointToPointMessageInfo p2pMsgInfo(MessageRank(rank), MessageRank(dest), MessageTag(tag), (blocking ? Blocking : NonBlocking));

  ByteArrayView avBuf(sizeof_msg, (Byte*)buf);

  *request = mpSend(iMPMng[comm], avBuf, p2pMsgInfo);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Recv(void *buf, int sizeof_msg, int source, int tag, MPA_Comm comm, Request *request, MessageId *status, bool blocking)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Recv()" << std::endl;
  #endif

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

  *request = mpReceive(iMPMng[comm], avBuf, p2pMsgInfo);

  return MPI_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MpiArcane::
MpiArcane_Type_size(MPI_Datatype datatype, int *size)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Type_size()" << std::endl;
  #endif

  return MPI_Type_size(datatype, size);
}

int MpiArcane::
MpiArcane_Wait(Request *request)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Wait()" << std::endl;
  #endif
  
  mpWait(iMPMng[MPA_COMM_WORLD], *request);
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Waitall(int count, Request *array_of_requests)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Waitall()" << std::endl;
  #endif

  ArrayView<Request> reqs(count, array_of_requests);
  mpWaitAll(iMPMng[MPA_COMM_WORLD], reqs);
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Waitany(int count, Request *array_of_requests, int *index)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Waitany()" << std::endl;
  #endif

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
MpiArcane_Test(Request *request, int *flag)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Test()" << std::endl;
  #endif

  ArrayView<Request> reqs(1, request);
  bool bFlag;
  ArrayView<bool> avFlag(1, &bFlag);

  mpTestSome(iMPMng[MPA_COMM_WORLD], reqs, avFlag);
  *flag = bFlag;

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Testall(int count, Request *array_of_requests, int *flag)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Testall()" << std::endl;
  #endif

  ArrayView<Request> reqs(count, array_of_requests);
  UniqueArray<bool> flags(count);

  mpTestSome(iMPMng[MPA_COMM_WORLD], reqs, flags);

  for(int i = 1; i < count; i++){
    flags[0] += flags[i];
  }
  *flag = flags[0];

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Barrier(MPA_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Barrier()" << std::endl;
  #endif

  mpBarrier(iMPMng[comm]);
  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Bcast(void *buffer, int sizeof_msg, int root, MPA_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Bcast()" << std::endl;
  #endif

  ByteArrayView avBuf(sizeof_msg, (Byte*)buffer);

  mpBroadcast(iMPMng[comm], avBuf, root);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Gather(const void *sendbuf, int sizeof_sentmsg,
                 void *recvbuf, int sizeof_recvmsg,
                 int root, MPA_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Gather()" << std::endl;
  #endif

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  ByteArrayView avRecvBuf(sizeof_recvmsg, (Byte*)recvbuf);

  mpGather(iMPMng[comm], avSendBuf, avRecvBuf, root);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Gatherv(const void *sendbuf, int sizeof_sentmsg,
                  void *recvbuf, const int *recvcounts, const int *displs,
                  int sizeof_recvtype, int root, MPA_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Gatherv()" << std::endl;
  #endif

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  UniqueArray<Byte> uaRecv;
  mpGatherVariable(iMPMng[comm], avSendBuf, uaRecv, root);

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
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Allgather()" << std::endl;
  #endif

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  ByteArrayView avRecvBuf(sizeof_recvmsg, (Byte*)recvbuf);

  mpAllGather(iMPMng[comm], avSendBuf, avRecvBuf);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Allgatherv(const void *sendbuf, int sizeof_sentmsg,
                                       void *recvbuf, const int *recvcounts, const int *displs,
                                       int sizeof_recvtype, MPA_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Allgatherv()" << std::endl;
  #endif

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  UniqueArray<Byte> uaRecv;
  mpAllGatherVariable(iMPMng[comm], avSendBuf, uaRecv);

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
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Allreduce()" << std::endl;
  #endif

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

  mpAllReduce(iMPMng[comm], rtype, avRecvBuf);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Scatter(const void *sendbuf, int sizeof_sentmsg,
                            void *recvbuf, int sizeof_recvmsg,
                            int root, MPA_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Scatter()" << std::endl;
  #endif

  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  if(rank == root) {
    int size;
    MpiArcane_Comm_size(comm, &size);

    int pos = 0;
    for(int i = 0; i < size; i++){        
      Request req;
      MpiArcane_Send(((Byte*)sendbuf) + (i*sizeof_recvmsg), sizeof_recvmsg, i, 0, comm, &req, false);
    }
  }
  // else {
    Request req;
    MessageId mId;
    MpiArcane_Recv(recvbuf, sizeof_recvmsg, root, 0, comm, &req, &mId, false);
  // }

  MpiArcane_Barrier(comm);

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, int sizeof_sendtype, 
                       void *recvbuf, int sizeof_recvmsg,
                       int root, MPA_Comm comm)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Scatterv()" << std::endl;
  #endif

  int rank;
  MpiArcane_Comm_rank(comm, &rank);

  if(rank == root) {
    int size;
    MpiArcane_Comm_size(comm, &size);

    int pos = 0;
    for(int i = 0; i < size; i++){
      Request req;
      MpiArcane_Send(((Byte*)sendbuf) + (displs[i]*sizeof_sendtype), sendcounts[i]*sizeof_sendtype, i, 0, comm, &req, false);
    }
  }
  // else {
    Request req;
    MessageId mId;
    MpiArcane_Recv(recvbuf, sizeof_recvmsg, root, 0, comm, &req, &mId, false);
  // }

  MpiArcane_Barrier(comm);

  return MPI_SUCCESS;
}


int MpiArcane::
MpiArcane_Probe(int source, int tag, MPA_Comm comm, MessageId *status)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Probe()" << std::endl;
  #endif

  PointToPointMessageInfo p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), Blocking);
  *status = mpProbe(iMPMng[comm], p2pMsgInfo);

  return MPI_SUCCESS;
}


int MpiArcane::
MpiArcane_Iprobe(int source, int tag, MPA_Comm comm, int *flag, MessageId *status)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Iprobe()" << std::endl;
  #endif

  PointToPointMessageInfo p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), NonBlocking);
  *status = mpProbe(iMPMng[comm], p2pMsgInfo);
  *flag = status->isValid();

  return MPI_SUCCESS;
}

int MpiArcane::
MpiArcane_Get_count(const MessageId *status, int sizeof_type, int *count)
{
  #ifdef PRINT_CALL
  std::cout << "[World Rank " << world_rank << "] --------------- MpiArcane_Get_count()" << std::endl;
  #endif

  *count = int(status->sourceInfo().size() / sizeof_type);

  return MPI_SUCCESS;
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
