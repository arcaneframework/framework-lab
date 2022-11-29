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

#include "ShMemArcane.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int ShMemArcane::
Init(IParallelMng *iPMng)
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

  iPMng->barrier();
  return MPI_SUCCESS;
}

int ShMemArcane::
Finalize()
{
  if(!m_isInit) return MPI_SUCCESS;

  int rank, size;
  Comm_rank(SMA_COMM_WORLD, &rank);
  Comm_size(SMA_COMM_WORLD, &size);

  m_requests[rank].clear();

  Barrier(SMA_COMM_WORLD);

  if(rank == 0){
    delete[] m_requests;
    m_iPMng.clear();
    m_isInit = false;
  }
  
  return MPI_SUCCESS;
}

int ShMemArcane::
Abort(MPA_Comm comm, int errorcode)
{
  if(!m_isInit) return MPI_SUCCESS;

  int rank;
  Comm_rank(comm, &rank);

  m_requests[rank].clear();

  Barrier(comm);

  if(rank == 0){
    delete[] m_requests;
    m_iPMng.clear();
    m_isInit = false;
  }
  ARCANE_FATAL("MPI_Abort() with error code: ");

  return MPI_SUCCESS;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int ShMemArcane::
Comm_split(MPA_Comm comm, int color, int key, MPA_Comm *newcomm)
{
  SMA_VERIF_COMM_WORLD(comm);

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


int ShMemArcane::
Comm_dup(MPA_Comm comm, MPA_Comm *newcomm)
{
  return Comm_split(comm, 0, 0, newcomm);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int ShMemArcane::
Comm_size(MPA_Comm comm, int *size)
{
  SMA_VERIF_COMM_WORLD(comm);

  *size = m_iPMng[comm]->commSize();
  return MPI_SUCCESS;
}

int ShMemArcane::
Comm_rank(MPA_Comm comm, int *rank)
{
  SMA_VERIF_COMM_WORLD(comm);

  *rank = m_iPMng[comm]->commRank();
  return MPI_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int ShMemArcane::
Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPA_Comm comm, MPA_Request *request, bool blocking)
{
  SMA_VERIF_COMM_WORLD(comm);

  int rank;
  Comm_rank(comm, &rank);

  int sizeof_type;
  int error = Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  int sizeof_msg = sizeof_type * count;

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

int ShMemArcane::
Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPA_Comm comm, MPA_Request *request, MessageId *status, bool blocking)
{
  SMA_VERIF_COMM_WORLD(comm);
    
  int rank;
  Comm_rank(comm, &rank);

  int sizeof_type;
  int error = Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  int sizeof_msg = sizeof_type * count;

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

int ShMemArcane::
Type_size(MPI_Datatype datatype, int *size)
{
  return MPI_Type_size(datatype, size);
}

int ShMemArcane::
Wait(MPA_Request *request)
{
  return Waitall(1, request);
}

int ShMemArcane::
Waitall(int count, MPA_Request *array_of_requests)
{
  int rank;
  Comm_rank(SMA_COMM_WORLD, &rank);

  UniqueArray<Request> arc_reqs(count);
  for(int i = 0; i < count; i++){
    arc_reqs[i] = m_requests[rank][array_of_requests[i]];
  }
  m_iPMng[SMA_COMM_WORLD]->waitAllRequests(arc_reqs);
  return MPI_SUCCESS;
}

int ShMemArcane::
Waitany(int count, MPA_Request *array_of_requests, int *index)
{
  int i = 0;
  for(;;){
    if(i >= count) i = 0;
    int flag;
    Test(&array_of_requests[i], &flag);
    if(flag == 1) break;
    i++;
  }

  *index = i;
  return MPI_SUCCESS;
}

int ShMemArcane::
Test(MPA_Request *request, int *flag)
{
  if(*request == MPA_Request_null){
    *flag = 2;
    return MPI_SUCCESS;
  }
  int rank;
  Comm_rank(SMA_COMM_WORLD, &rank);

  UniqueArray<Request> arc_reqs(1);
  arc_reqs[0] = m_requests[rank][*request];

  UniqueArray<Integer> done_requests = m_iPMng[SMA_COMM_WORLD]->testSomeRequests(arc_reqs);


  if(done_requests.size() == 1){
    *flag = 1;
    *request = MPA_Request_null;
  }
  else{
    *flag = 0;
  }

  return MPI_SUCCESS;
}

int ShMemArcane::
Testall(int count, MPA_Request *array_of_requests, int *flag)
{
  int rank;
  Comm_rank(SMA_COMM_WORLD, &rank);

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

  UniqueArray<Integer> done_requests = m_iPMng[SMA_COMM_WORLD]->testSomeRequests(arc_reqs);

  for(int i = 0; i < done_requests.size(); i++){
    num_of_done_requests++;
    array_of_requests[pos_to_pos[done_requests[i]]] = MPA_Request_null;
  }

  *flag = (num_of_done_requests == count);

  return MPI_SUCCESS;
}

int ShMemArcane::
Barrier(MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  m_iPMng[comm]->barrier();
  return MPI_SUCCESS;
}

int ShMemArcane::
Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  int sizeof_type;
  int error = Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  int sizeof_msg = sizeof_type * count;

  ByteArrayView avBuf(sizeof_msg, (Byte*)buffer);

  m_iPMng[comm]->broadcast(avBuf, root);

  return MPI_SUCCESS;
}

int ShMemArcane::
Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 int root, MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  int sizeof_sendtype;
  int error = Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_sentmsg = sizeof_sendtype * sendcount;
  int sizeof_recvmsg = sizeof_recvtype * recvcount;

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  ByteArrayView avRecvBuf(sizeof_recvmsg, (Byte*)recvbuf);

  m_iPMng[comm]->gather(avSendBuf, avRecvBuf, root);

  return MPI_SUCCESS;
}

int ShMemArcane::
Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, const int *recvcounts, const int *displs,
                  MPI_Datatype recvtype, int root, MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  int sizeof_sendtype;
  int error = Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;


  int sizeof_sentmsg = sizeof_sendtype * sendcount;

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  UniqueArray<Byte> uaRecv;

  m_iPMng[comm]->gatherVariable(avSendBuf, uaRecv, root);

  int rank;
  Comm_rank(comm, &rank);

  if(rank == root){
    int size;
    Comm_size(comm, &size);

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

int ShMemArcane::
Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                    void *recvbuf, int recvcount, MPI_Datatype recvtype,
                    MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  int sizeof_sendtype;
  int error = Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_sentmsg = sizeof_sendtype * sendcount;
  int sizeof_recvmsg = sizeof_recvtype * recvcount;

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  ByteArrayView avRecvBuf(sizeof_recvmsg, (Byte*)recvbuf);

  m_iPMng[comm]->allGather(avSendBuf, avRecvBuf);

  return MPI_SUCCESS;
}

int ShMemArcane::
Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                     void *recvbuf, const int *recvcounts, const int *displs,
                     MPI_Datatype recvtype, MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  int sizeof_sendtype;
  int error = Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_sentmsg = sizeof_sendtype * sendcount;

  ByteArrayView avSendBuf(sizeof_sentmsg, (Byte*)sendbuf);
  UniqueArray<Byte> uaRecv;

  m_iPMng[comm]->allGatherVariable(avSendBuf, uaRecv);

  int size;
  Comm_size(comm, &size);

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

int ShMemArcane::
Allreduce(const void *sendbuf, void *recvbuf, int sizeof_msg,
                     MPI_Datatype datatype, MPI_Op op, MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  eReduceType rtype;
  if(op == MPI_MIN)
    rtype = ReduceMin;
  else if(op == MPI_MAX)
    rtype = ReduceMax;
  else if(op == MPI_SUM)
    rtype = ReduceSum;

  else {
    if(op == MPI_OP_NULL)
      std::cout << "[MPI_ERR_TYPE] MPI_OP_NULL not supported yet" << std::endl;
    else if(op == MPI_PROD)
      std::cout << "[MPI_ERR_TYPE] MPI_PROD not supported yet" << std::endl;
    else if(op == MPI_LAND)
      std::cout << "[MPI_ERR_TYPE] MPI_LAND not supported yet" << std::endl;
    else if(op == MPI_BAND)
      std::cout << "[MPI_ERR_TYPE] MPI_BAND not supported yet" << std::endl;
    else if(op == MPI_LOR)
      std::cout << "[MPI_ERR_TYPE] MPI_LOR not supported yet" << std::endl;
    else if(op == MPI_BOR)
      std::cout << "[MPI_ERR_TYPE] MPI_BOR not supported yet" << std::endl;
    else if(op == MPI_LXOR)
      std::cout << "[MPI_ERR_TYPE] MPI_LXOR not supported yet" << std::endl;
    else if(op == MPI_BXOR)
      std::cout << "[MPI_ERR_TYPE] MPI_BXOR not supported yet" << std::endl;
    else if(op == MPI_MINLOC)
      std::cout << "[MPI_ERR_TYPE] MPI_MINLOC not supported yet" << std::endl;
    else if(op == MPI_MAXLOC)
      std::cout << "[MPI_ERR_TYPE] MPI_MAXLOC not supported yet" << std::endl;
    else if(op == MPI_REPLACE)
      std::cout << "[MPI_ERR_TYPE] MPI_REPLACE not supported yet" << std::endl;
    else
      std::cout << "[MPI_ERR_TYPE] Invalid op" << std::endl;
    return MPI_ERR_TYPE;
  }

  if(datatype == MPI_CHAR || datatype == MPI_SIGNED_CHAR)
    return _allReduce((const char*)sendbuf, (char*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_SHORT)
    return _allReduce((const short*)sendbuf, (short*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_INT)
    return _allReduce((const int*)sendbuf, (int*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_LONG)
    return _allReduce((const long*)sendbuf, (long*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_LONG_LONG)
    return _allReduce((const long long*)sendbuf, (long long*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_UNSIGNED_CHAR)
    return _allReduce((const unsigned char*)sendbuf, (unsigned char*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_UNSIGNED_SHORT)
    return _allReduce((const unsigned short*)sendbuf, (unsigned short*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_UNSIGNED)
    return _allReduce((const unsigned int*)sendbuf, (unsigned int*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_UNSIGNED_LONG)
    return _allReduce((const unsigned long*)sendbuf, (unsigned long*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_UNSIGNED_LONG_LONG)
    return _allReduce((const unsigned long long*)sendbuf, (unsigned long long*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_FLOAT)
    return _allReduce((const float*)sendbuf, (float*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_DOUBLE)
    return _allReduce((const double*)sendbuf, (double*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_LONG_DOUBLE)
    return _allReduce((const long double*)sendbuf, (long double*)recvbuf, sizeof_msg, rtype, comm);
  else if(datatype == MPI_BYTE)
    return _allReduce((const Byte*)sendbuf, (Byte*)recvbuf, sizeof_msg, rtype, comm);
  else
    std::cout << "[MPI_ERR_TYPE] Invalid datatype" << std::endl;
  return MPI_ERR_TYPE;
}

int ShMemArcane::
Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                            void *recvbuf, int recvcount, MPI_Datatype recvtype,
                            int root, MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  int sizeof_recvtype;
  int error = Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvmsg = sizeof_recvtype * recvcount;

  int rank;
  Comm_rank(comm, &rank);

  if(rank == root) {
    int size;
    Comm_size(comm, &size);

    for(int i = 0; i < size; i++){
      if(i == root) continue;
      MPA_Request req;
      Send(((Byte*)sendbuf) + (i*sizeof_recvmsg), recvcount, recvtype, i, 0, comm, &req, false);
    }
    for(int i = 0; i < sizeof_recvmsg; i++){
      ((Byte*)recvbuf)[i] = *(((Byte*)sendbuf) + (rank*sizeof_recvmsg+i));
    }
  }
  else{
    MPA_Request req;
    MessageId mId;
    Recv(recvbuf, recvcount, recvtype, root, 0, comm, &req, &mId, true);
  }
  // Barrier car root se desync sinon.
  Barrier(comm);

  return MPI_SUCCESS;
}

int ShMemArcane::
Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, MPI_Datatype sendtype, 
                       void *recvbuf, int recvcount, MPI_Datatype recvtype,
                       int root, MPA_Comm comm)
{
  SMA_VERIF_COMM_WORLD(comm);

  int sizeof_sendtype;
  int error = Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvmsg = sizeof_recvtype * recvcount;


  int rank;
  Comm_rank(comm, &rank);

  if(rank == root) {
    int size;
    Comm_size(comm, &size);

    int pos = 0;
    for(int i = 0; i < size; i++){
      if(i == root) continue;
      MPA_Request req;
      Send(((Byte*)sendbuf) + (displs[i]*sizeof_sendtype), sendcounts[i], sendtype, i, 0, comm, &req, false);
    }
    for(int i = 0; i < sizeof_recvmsg; i++){
      ((Byte*)recvbuf)[i] = *(((Byte*)sendbuf) + (displs[rank]*sizeof_recvmsg+i));
    }
  }
  else{
    MPA_Request req;
    MessageId mId;
    Recv(recvbuf, recvcount, recvtype, root, 0, comm, &req, &mId, true);
  }

  // Barrier car root se desync sinon.
  Barrier(comm);

  return MPI_SUCCESS;
}


int ShMemArcane::
Probe(int source, int tag, MPA_Comm comm, MessageId *status)
{
  SMA_VERIF_COMM_WORLD(comm);

  status->reset();

  PointToPointMessageInfo p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), NonBlocking);

  MessageSourceInfo msi;

  do{
    msi = mpLegacyProbe(m_iPMng[comm]->messagePassingMng(), p2pMsgInfo);
  } while(!msi.isValid());

  status->setSourceInfo(msi);
  
  return MPI_SUCCESS;
}


int ShMemArcane::
Iprobe(int source, int tag, MPA_Comm comm, int *flag, MessageId *status)
{
  SMA_VERIF_COMM_WORLD(comm);

  status->reset();

  PointToPointMessageInfo p2pMsgInfo = PointToPointMessageInfo(MessageRank(source), MessageTag(tag), NonBlocking);

  status->setSourceInfo(mpLegacyProbe(m_iPMng[comm]->messagePassingMng(), p2pMsgInfo));
  
  return MPI_SUCCESS;
}

int ShMemArcane::
Get_count(const MessageId *status, MPI_Datatype datatype, int *count)
{
  int sizeof_type;
  int error = Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  *count = int(status->sourceInfo().size() / sizeof_type);
  return MPI_SUCCESS;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
