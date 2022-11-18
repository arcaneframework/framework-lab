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

#include <mpiArcane.h>

// #define useMPI

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

MpiArcane* mpiArcane = nullptr;

namespace MPA
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifdef useMPI

#include <mpi.h>

using ::MPI_Comm;

using ::MPI_Request;
using ::MPI_Status;

using ::MPI_Init;
using ::MPI_Initialized;
using ::MPI_Finalize;

using ::MPI_Comm_split;

using ::MPI_Comm_size;
using ::MPI_Comm_rank;

using ::MPI_Send;
using ::MPI_Recv;

using ::MPI_Isend;
using ::MPI_Irecv;

using ::MPI_Type_size;

using ::MPI_Wait;
using ::MPI_Waitall;
using ::MPI_Waitany;

using ::MPI_Test;
using ::MPI_Testall;

using ::MPI_Barrier;

using ::MPI_Bcast;

using ::MPI_Gather;
using ::MPI_Gatherv;

using ::MPI_Allgather;
using ::MPI_Allgatherv;

using ::MPI_Reduce;
using ::MPI_Allreduce;

using ::MPI_Scatter;
using ::MPI_Scatterv;

using ::MPI_Probe;
using ::MPI_Iprobe;

using ::MPI_Get_count;

#else

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

using MPI_Request = Arccore::MessagePassing::Request;
using MPI_Status  = Arccore::MessagePassing::MessageId;
using MPI_Comm = MPA_Comm;

#undef MPI_STATUS_IGNORE
#define MPI_STATUS_IGNORE MPA::MPA_STATUS

#undef MPI_STATUSES_IGNORE
#define MPI_STATUSES_IGNORE MPA::MPA_STATUS

#undef MPI_COMM_WORLD
#define MPI_COMM_WORLD MPA_COMM_WORLD

MPI_Status* MPA_STATUS;

int MPI_Init(int *, char ***);
int MPI_Initialized(int *);
int MPI_Finalize(void);

int MPI_Comm_split(MPI_Comm, int, int, MPI_Comm *);

int MPI_Comm_size(MPI_Comm, int *);
int MPI_Comm_rank(MPI_Comm, int *);

int MPI_Send(const void*, int, MPI_Datatype, int, int, MPI_Comm);
int MPI_Recv(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Status *);

int MPI_Isend(const void *, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *);
int MPI_Irecv(void*, int, MPI_Datatype, int, int, MPI_Comm, MPI_Request *);

int MPI_Type_size(MPI_Datatype , int *);

int MPI_Wait(MPI_Request *, MPI_Status *);
int MPI_Waitall(int, MPI_Request *, MPI_Status *);
int MPI_Waitany(int, MPI_Request *, int *, MPI_Status *);

int MPI_Test(MPI_Request *, int *, MPI_Status *);
int MPI_Testall(int, MPI_Request *, int *, MPI_Status *);

int MPI_Barrier(MPI_Comm);

int MPI_Bcast(void *, int, MPI_Datatype, int, MPI_Comm);

int MPI_Gather(const void *, int, MPI_Datatype, void *, int, MPI_Datatype, int, MPI_Comm);
int MPI_Gatherv(const void *, int, MPI_Datatype, void *, const int *, const int *, MPI_Datatype, int, MPI_Comm);

int MPI_Allgather(const void *, int, MPI_Datatype, void *, int, MPI_Datatype, MPI_Comm);
int MPI_Allgatherv(const void *, int, MPI_Datatype, void *, const int *, const int *, MPI_Datatype, MPI_Comm);

int MPI_Reduce(const void *, void *, int, MPI_Datatype, MPI_Op, int, MPI_Comm);
int MPI_Allreduce(const void *, void *, int, MPI_Datatype, MPI_Op, MPI_Comm);

int MPI_Scatter(const void *, int, MPI_Datatype, void *, int, MPI_Datatype, int, MPI_Comm);
int MPI_Scatterv(const void *, const int *, const int *, MPI_Datatype, void *, int, MPI_Datatype, int, MPI_Comm);

int MPI_Probe(int, int, MPI_Comm, MPI_Status *);
int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status);

int MPI_Get_count(const MPI_Status *, MPI_Datatype, int *);



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPI_Init(int *argc, char ***argv)
{
  int isInit = false;
  MPI_Initialized(&isInit);
  if(isInit){
    return MPI_ERR_OTHER;
  }

  MPA_STATUS = new MPI_Status();
  mpiArcane = new MpiArcane();
  return mpiArcane->MpiArcane_Init(argc, argv);
}

int MPI_Initialized(int *flag)
{
  *flag = (mpiArcane != nullptr);
  return MPI_SUCCESS;
}

int MPI_Finalize(void)
{
  int isInit = false;
  MPI_Initialized(&isInit);
  if(!isInit){
    return MPI_SUCCESS;
  }

  mpiArcane->MpiArcane_Finalize();

  free(mpiArcane);
  mpiArcane = nullptr;

  free(MPA_STATUS);
  MPA_STATUS = nullptr;

  return MPI_SUCCESS;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;
  return mpiArcane->MpiArcane_Comm_split(comm, color, key, newcomm);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPI_Comm_size(MPI_Comm comm, int *size)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;
  return mpiArcane->MpiArcane_Comm_size(comm, size);
}

int MPI_Comm_rank(MPI_Comm comm, int *rank)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;
  return mpiArcane->MpiArcane_Comm_rank(comm, rank);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  int sizeof_type;
  int error = mpiArcane->MpiArcane_Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  Request req;

  return mpiArcane->MpiArcane_Send(buf, sizeof_type*count, dest, tag, comm, &req, true);
}

int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  int sizeof_type;
  int error = mpiArcane->MpiArcane_Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  Request req;

  error = mpiArcane->MpiArcane_Recv(buf, sizeof_type*count, source, tag, comm, &req, status, true);
  return error;
}



int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  int sizeof_type;
  int error = mpiArcane->MpiArcane_Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Send(buf, sizeof_type*count, dest, tag, comm, request, false);
}

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request)
{
  if(mpiArcane == nullptr) return MPI_ERR_COMM;

  int sizeof_type;
  int error = mpiArcane->MpiArcane_Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;
  MessageId mId;

  return mpiArcane->MpiArcane_Recv(buf, sizeof_type*count, source, tag, comm, request, &mId, false);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

int MPI_Type_size(MPI_Datatype datatype, int *size)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  return mpiArcane->MpiArcane_Type_size(datatype, size);
}

int MPI_Wait(MPI_Request *request, MPI_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  return mpiArcane->MpiArcane_Wait(request);
}

int MPI_Waitall(int count, MPI_Request *array_of_requests, MPI_Status *array_of_statuses)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  return mpiArcane->MpiArcane_Waitall(count, array_of_requests);
}

int MPI_Waitany(int count, MPI_Request *array_of_requests, int *index, MPI_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  return mpiArcane->MpiArcane_Waitany(count, array_of_requests, index);
}

int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  return mpiArcane->MpiArcane_Test(request, flag);
}

int MPI_Testall(int count, MPI_Request *array_of_requests, int *flag, MPI_Status *array_of_statuses)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  return mpiArcane->MpiArcane_Testall(count, array_of_requests, flag);
}

int MPI_Barrier(MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  return mpiArcane->MpiArcane_Barrier(comm);
}

int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  int sizeof_type;
  int error = mpiArcane->MpiArcane_Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Bcast(buffer, sizeof_type*count, root, comm);
}

int MPI_Gather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype,
               int root, MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  int sizeof_sendtype;
  int error = mpiArcane->MpiArcane_Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = mpiArcane->MpiArcane_Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Gather(
    sendbuf, sizeof_sendtype*sendcount, 
    recvbuf, sizeof_recvtype*recvcount, 
    root, comm);
}


int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                void *recvbuf, const int *recvcounts, const int *displs,
                MPI_Datatype recvtype, int root, MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  int sizeof_sendtype;
  int error = mpiArcane->MpiArcane_Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = mpiArcane->MpiArcane_Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Gatherv(
    sendbuf, sizeof_sendtype*sendcount, 
    recvbuf, recvcounts, displs, sizeof_recvtype,
    root, comm);
}

int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  int sizeof_sendtype;
  int error = mpiArcane->MpiArcane_Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = mpiArcane->MpiArcane_Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Allgather(
    sendbuf, sizeof_sendtype*sendcount, 
    recvbuf, sizeof_recvtype*recvcount, 
    comm);
}

int MPI_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                   void *recvbuf, const int *recvcounts, const int *displs,
                   MPI_Datatype recvtype, MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  int sizeof_sendtype;
  int error = mpiArcane->MpiArcane_Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = mpiArcane->MpiArcane_Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Allgatherv(
    sendbuf, sizeof_sendtype*sendcount, 
    recvbuf, recvcounts, displs, sizeof_recvtype,
    comm);
}

int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype,
               MPI_Op op, int root, MPI_Comm comm)
{
  return MPA::MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
}

int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count,
                  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  
  if(datatype == MPI_CHAR || datatype == MPI_SIGNED_CHAR)
    return mpiArcane->MpiArcane_Allreduce((const char*)sendbuf, (char*)recvbuf, count, op, comm);
  else if(datatype == MPI_SHORT)
    return mpiArcane->MpiArcane_Allreduce((const short*)sendbuf, (short*)recvbuf, count, op, comm);
  else if(datatype == MPI_INT)
    return mpiArcane->MpiArcane_Allreduce((const int*)sendbuf, (int*)recvbuf, count, op, comm);
  else if(datatype == MPI_LONG)
    return mpiArcane->MpiArcane_Allreduce((const long*)sendbuf, (long*)recvbuf, count, op, comm);
  else if(datatype == MPI_LONG_LONG)
    return mpiArcane->MpiArcane_Allreduce((const long long*)sendbuf, (long long*)recvbuf, count, op, comm);
  else if(datatype == MPI_UNSIGNED_CHAR)
    return mpiArcane->MpiArcane_Allreduce((const unsigned char*)sendbuf, (unsigned char*)recvbuf, count, op, comm);
  else if(datatype == MPI_UNSIGNED_SHORT)
    return mpiArcane->MpiArcane_Allreduce((const unsigned short*)sendbuf, (unsigned short*)recvbuf, count, op, comm);
  else if(datatype == MPI_UNSIGNED)
    return mpiArcane->MpiArcane_Allreduce((const unsigned int*)sendbuf, (unsigned int*)recvbuf, count, op, comm);
  else if(datatype == MPI_UNSIGNED_LONG)
    return mpiArcane->MpiArcane_Allreduce((const unsigned long*)sendbuf, (unsigned long*)recvbuf, count, op, comm);
  else if(datatype == MPI_UNSIGNED_LONG_LONG)
    return mpiArcane->MpiArcane_Allreduce((const unsigned long long*)sendbuf, (unsigned long long*)recvbuf, count, op, comm);
  else if(datatype == MPI_FLOAT)
    return mpiArcane->MpiArcane_Allreduce((const float*)sendbuf, (float*)recvbuf, count, op, comm);
  else if(datatype == MPI_DOUBLE)
    return mpiArcane->MpiArcane_Allreduce((const double*)sendbuf, (double*)recvbuf, count, op, comm);
  else if(datatype == MPI_LONG_DOUBLE)
    return mpiArcane->MpiArcane_Allreduce((const long double*)sendbuf, (long double*)recvbuf, count, op, comm);
  else if(datatype == MPI_BYTE)
    return mpiArcane->MpiArcane_Allreduce((const Byte*)sendbuf, (Byte*)recvbuf, count, op, comm);
  else
    return MPI_ERR_TYPE;
}

int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
               void *recvbuf, int recvcount, MPI_Datatype recvtype, int root,
               MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  int sizeof_sendtype;
  int error = mpiArcane->MpiArcane_Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = mpiArcane->MpiArcane_Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Scatter(
    sendbuf, sizeof_sendtype*sendcount, 
    recvbuf, sizeof_recvtype*recvcount, 
    root, comm);
}

int MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs,
                 MPI_Datatype sendtype, void *recvbuf, int recvcount,
                 MPI_Datatype recvtype,
                 int root, MPI_Comm comm)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;

  int sizeof_sendtype;
  int error = mpiArcane->MpiArcane_Type_size(sendtype, &sizeof_sendtype);
  if(error != MPI_SUCCESS) return error;

  int sizeof_recvtype;
  error = mpiArcane->MpiArcane_Type_size(recvtype, &sizeof_recvtype);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Scatterv(
    sendbuf, sendcounts, displs, sizeof_sendtype,
    recvbuf, sizeof_recvtype*recvcount,
    root, comm);
}

int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  status->reset();
  return mpiArcane->MpiArcane_Probe(source, tag, comm, status);
}

int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  status->reset();
  return mpiArcane->MpiArcane_Iprobe(source, tag, comm, flag, status);
}

int MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count)
{
  if(mpiArcane == nullptr) return MPI_ERR_ARG;
  
  int sizeof_type;
  int error = mpiArcane->MpiArcane_Type_size(datatype, &sizeof_type);
  if(error != MPI_SUCCESS) return error;

  return mpiArcane->MpiArcane_Get_count(status, sizeof_type, count);
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

}; // End namespace MPA

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
