// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
 
#include "SayHelloModule.h"

#include <arcane/IParallelMng.h>
#include "mpA.h"


using namespace Arcane;

void SayHelloModule::
beginCompute()
{
  MPI_Init(parallelMng());
}

void SayHelloModule::
endCompute()
{
  subDomain()->timeLoopMng()->stopComputeLoop(true);
}
 
void SayHelloModule::
compute()
{
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

  int argc = 0;
  char** argv = NULL;

  MPI_Init(&argc, &argv);

  int world_rank = -123;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size = -456;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  pinfo() << "Hello from " << world_rank << "/" << world_size;


  if(world_rank != parallelMng()->commRank() || world_size != parallelMng()->commSize()){
    error() << "Erreur de rank ou de size : Vrai rank : " << parallelMng()->commRank() << " rank trouvé : " << world_rank;
    ARCANE_FATAL("Erreur de rank ou de size...");
  }

  int root_rank = 0;



  if(world_size != 3) {
    std::cout << "Error: Work only with 3 procs" << std::endl;
    return;
  }


  // Examples from RookieHPC :

  // -------------------- 1 -----------------------





  {
    int number;
    if (world_rank == 0) {
      number = -1;
      MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } 

    else if (world_rank == 1) {
      MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
              MPI_STATUS_IGNORE);
      printf("Process 1 received number %d from process 0\n",
            number);
      if(number != -1)
        ARCANE_FATAL("Erreur 1");
    }
  }





  // -------------------- 2 -----------------------






  {
    switch(world_rank)
    {
      case 0:
      {
        int buffer_sent = 12345;
        MPI_Request request;
        printf("MPI process %d sends value %d.\n", world_rank, buffer_sent);
        MPI_Isend(&buffer_sent, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        
        // Do other things while the MPI_Isend completes
        // <...>

        // Let's wait for the MPI_Isend to complete before progressing further.
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        break;
      }
      case 1:
      {
        int received;
        MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("MPI process %d received value: %d.\n", world_rank, received);
        if(received != 12345)
          ARCANE_FATAL("Erreur 2");
        break;
      }
    }
  }






  // -------------------- 3 -----------------------





  {
    switch(world_rank)
    {
      case 0:
      {
        // The "master" MPI process receives the messages.
        int message_1;
        int message_2;
        MPI_Request requests[2];
        int ready;

        MPI_Irecv(&message_1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(&message_2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &requests[1]);

        // This MPI_Testall is guaranteed to fail since the corresponding MPI_Ssends have not been issued yet.
        MPI_Testall(2, requests, &ready, MPI_STATUSES_IGNORE);
        if(ready)
            printf("[Process 0] First attempt: both receives are complete.\n");
        else
            printf("[Process 0] First attempt: not both receives are complete yet.\n");

        // We can tell other processes to start sending messages
        MPI_Barrier(MPI_COMM_WORLD);
            printf("[Process 0] AAAAAAAAA.\n");

        // TODO : Ligne mise pour éviter un deadlock avec les send des rank 1 et 2.
        MPI_Waitall(2, requests, MPI_STATUSES_IGNORE);

        // We wait for the other processes to tell us their MPI_Ssend is complete
        MPI_Barrier(MPI_COMM_WORLD);

            printf("[Process 0] TTTTTTTTT.\n");


        // This MPI_Testall is guaranteed to succeed since the corresponding MPI_Ssends are all complete.
        //MPI_Testall(2, requests, &ready, MPI_STATUSES_IGNORE);
        //if(ready)
        //    printf("[Process 0] Second attempt: both receives are complete.\n");
        //else
        //    printf("[Process 0] Second attempt: not both receives are complete yet.\n");

        if(message_1 != 12345 || message_2 != 67890)
          ARCANE_FATAL("Erreur 3");
        break;
      }
      case 1:
      case 2:
      {
        // The "slave" MPI processes send the messages.
        int message = (world_rank == 1) ? 12345 : 67890;

        // Wait for the MPI_Testall #1 to be done.
        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            printf("[Process 1&2] MMMMMMMMMM.\n");
        // Tell the sender it can now issue the MPI_Testall #2.
        MPI_Barrier(MPI_COMM_WORLD);

        break;
      }
      default:
      {
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        break;
      }
    }
  }





  // -------------------- 4 -----------------------




  {
    // Determine the rank of the broadcast emitter process
    int broadcast_root = 0;

    int buffer;
    if(world_rank == broadcast_root)
    {
      buffer = 12345;
      printf("[MPI process %d] I am the broadcast root, and send value %d.\n", world_rank, buffer);
    }
    MPI_Bcast(&buffer, 1, MPI_INT, broadcast_root, MPI_COMM_WORLD);
    if(world_rank != broadcast_root)
    {
      printf("[MPI process %d] I am a broadcast receiver, and obtained value %d.\n", world_rank, buffer);
    }
    if(buffer != 12345)
      ARCANE_FATAL("Erreur 4");
  }




  // -------------------- 5 -----------------------





  {
    int result[7] = {100, 0, 0, 101, 0, 102, 103};
    switch(world_rank)
    {
      case 0:
      {
        // Define my value
        int my_value = 100;

        // Define the receive counts
        int counts[3] = {1, 1, 2};

        // Define the displacements
        int displacements[3] = {0, 3, 5};

        int* buffer = (int*)calloc(7, sizeof(int));
        printf("Process %d, my value = %d.\n", world_rank, my_value);
        MPI_Gatherv(&my_value, 1, MPI_INT, buffer, counts, displacements, MPI_INT, root_rank, MPI_COMM_WORLD);
        printf("Values gathered in the buffer on process %d:", world_rank);
        for(int i = 0; i < 7; i++)
        {
          printf(" %d", buffer[i]);
          if(buffer[i] != result[i])
            ARCANE_FATAL("Erreur 5");
        }
        printf("\n");
        free(buffer);
        break;
      }
      case 1:
      {
        // Define my value
        int my_value = 101;

        printf("Process %d, my value = %d.\n", world_rank, my_value);
        MPI_Gatherv(&my_value, 1, MPI_INT, NULL, NULL, NULL, MPI_INT, root_rank, MPI_COMM_WORLD);
        break;
      }
      case 2:
      {
        // Define my values
        int my_values[2] = {102, 103};

        printf("Process %d, my values = %d %d.\n", world_rank, my_values[0], my_values[1]);
        MPI_Gatherv(my_values, 2, MPI_INT, NULL, NULL, NULL, MPI_INT, root_rank, MPI_COMM_WORLD);
        break;
      }
    }
  }







  // -------------------- 6 -----------------------






  {
    // Define the receive counts
    int counts[3] = {1, 1, 2};

    // Define the displacements
    int displacements[3] = {0, 3, 5};

    // Buffer in which receive the data collected
    int buffer[7] = {0};

    // Buffer containing our data to send
    int* my_values;
    int my_values_count;

    int result[7] = {100, 0, 0, 101, 0, 102, 103};

    switch(world_rank)
    {
      case 0:
      {
        // Define my values
        my_values_count = 1;
        my_values = (int*)malloc(sizeof(int) * my_values_count);
        *my_values = 100;
        printf("Value sent by process %d: %d.\n", world_rank, *my_values);
        break;
      }
      case 1:
      {
        // Define my values
        my_values_count = 1;
        my_values = (int*)malloc(sizeof(int) * my_values_count);
        *my_values = 101;
        printf("Value sent by process %d: %d.\n", world_rank, *my_values);
        break;
      }
      case 2:
      {
        // Define my values
        my_values_count = 2;
        my_values = (int*)malloc(sizeof(int) * my_values_count);
        my_values[0] = 102;
        my_values[1] = 103;
        printf("Values sent by process %d: %d and %d.\n", world_rank, my_values[0], my_values[1]);
        break;
      }
    }

    MPI_Allgatherv(my_values, my_values_count, MPI_INT, buffer, counts, displacements, MPI_INT, MPI_COMM_WORLD);

    printf("Values gathered in the buffer on process %d:", world_rank);
    for(int i = 0; i < 7; i++)
    {
      printf(" %d", buffer[i]);
      if(buffer[i] != result[i])
        ARCANE_FATAL("Erreur 6");
    }
    printf("\n");
    free(my_values);
  }
  


  // -------------------- 7 -----------------------





  {
    // Each MPI process sends its rank to reduction, root MPI process collects the result
    int reduction_result = 0;
    int truc = world_rank * 123;

    MPI_Reduce(&truc, &reduction_result, 1, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);

    if(world_rank == root_rank)
    {
      printf("The sum of all ranks is %d.\n", reduction_result);
      int r = 0;
      for(int i = 0; i < world_size; i++)
      {
        r += i * 123;
      }
      if(reduction_result != r)
        ARCANE_FATAL("Erreur 7");
    }
  }



  // -------------------- 8 -----------------------




  {
    // Each MPI process sends its rank to reduction, root MPI process collects the result
    int reduction_result = 0;
    int truc = world_rank * 123;

    MPI_Allreduce(&truc, &reduction_result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    printf("[MPI Process %d] The sum of all ranks is %d.\n", world_rank, reduction_result);
    int r = 0;
    for(int i = 0; i < world_size; i++)
    {
      r += i * 123;
    }
    if(reduction_result != r)
      ARCANE_FATAL("Erreur 8");
  }




  // -------------------- 9 -----------------------



  {
    // Define my value
    int my_value;

    if(world_rank == root_rank)
    {
      int buffer[4] = {11, 111, 211, 311};
      printf("Values to scatter from process %d: %d, %d, %d, %d.\n", world_rank, buffer[0], buffer[1], buffer[2], buffer[3]);
      MPI_Scatter(buffer, 1, MPI_INT, &my_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    }
    else
    {
      MPI_Scatter(NULL, 1, MPI_INT, &my_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    }

    printf("Process %d received value = %d.\n", world_rank, my_value);
    if(my_value != world_rank * 100 + 11)
      ARCANE_FATAL("Erreur 9");
  }



  // -------------------- 10 -----------------------






  {
    switch(world_rank)
    {
      case 0:
      {
        // Define my value
        int my_value;

        // Declare the buffer
        int buffer[7] = {100, 0, 101, 102, 0, 0, 103};

        // Declare the counts
        int counts[3] = {1, 2, 1};

        // Declare the displacements
        int displacements[3] = {0, 2, 6};

        printf("Values in the buffer of root process:");
        for(int i = 0; i < 7; i++)
        {
            printf(" %d", buffer[i]);
        }
        printf("\n");
        MPI_Scatterv(buffer, counts, displacements, MPI_INT, &my_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
        printf("Process %d received value %d.\n", world_rank, my_value);
        if(my_value != 100)
          ARCANE_FATAL("Erreur 10/1");
        break;
      }
      case 1:
      {
        // Declare my values
        int my_values[2];

        MPI_Scatterv(NULL, NULL, NULL, MPI_INT, my_values, 2, MPI_INT, root_rank, MPI_COMM_WORLD);
        printf("Process %d received values %d and %d.\n", world_rank, my_values[0], my_values[1]);
        if(my_values[0] != 101 || my_values[1] != 102)
          ARCANE_FATAL("Erreur 10/2");
        break;
      }
      case 2:
      {
        // Declare my values
        int my_value;

        MPI_Scatterv(NULL, NULL, NULL, MPI_INT, &my_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
        printf("Process %d received value %d.\n", world_rank, my_value);
        if(my_value != 103)
          ARCANE_FATAL("Erreur 10/3");
        break;
      }
    }
  }




  // -------------------- 11 -----------------------



  {
    int buffer0[3] = {123, 456, 789};
    switch(world_rank)
    {
      case 0:
      {
        printf("Process %d: sending a message containing 3 ints (%d, %d, %d), but the receiver is not aware of the length.\n", world_rank, buffer0[0], buffer0[1], buffer0[2]);
        MPI_Send(buffer0, 3, MPI_INT, 1, 1234, MPI_COMM_WORLD);
        break;
      }

      case 1:
      {
        // Retrieve information about the incoming message
        MPI_Status status;
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Process %d: obtained message status by probing it.\n", world_rank);

        printf("Process %d: Status infos -- Source : %i -- Tag : %i\n", world_rank, MPI_Status_source(&status), MPI_Status_tag(&status));

        if(MPI_Status_source(&status) != 0 || MPI_Status_tag(&status) != 1234){
          printf("!!!!!!!!!!!!!!!!!!!! Erreur au niveau du probe !!!!!!!!!!!!!!!!!!!!!!\n");
        }

        // Get the number of integers in the message probed
        int count;
        MPI_Get_count(&status, MPI_INT, &count);

        // Allocate the buffer now that we know how many elements there are
        int* buffer = (int*)malloc(sizeof(int) * count);

        // Finally receive the message
        MPI_Recv(buffer, count, MPI_INT, MPI_Status_source(&status), MPI_Status_tag(&status), MPI_COMM_WORLD, &status);
        printf("Process %d: received message with all %d ints:", world_rank, count);

        for(int i = 0; i < count; i++)
        {
          printf(" %d", buffer[i]);
          if(buffer[i] != buffer0[i])
            ARCANE_FATAL("Erreur 11");
        }
        printf(".\n");
        break;
      }
    }
  }
  // -------------------- 11.5 -----------------------

  {
    int buffer0[3] = {123, 456, 789};
    switch(world_rank)
    {
      case 0:
      {
          printf("Process %d: sending a message containing 3 ints (%d, %d, %d), but the receiver is not aware of the length.\n", world_rank, buffer0[0], buffer0[1], buffer0[2]);
          MPI_Send(buffer0, 3, MPI_INT, 1, 1234, MPI_COMM_WORLD);
          break;
      }

      case 1:
      {
          // Retrieve information about the incoming message
          MPI_Status status;
          MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
          printf("Process %d: obtained message status by probing it.\n", world_rank);

          int source = MPI_Status_source(&status);
          int tag = MPI_Status_tag(&status);

          printf("Process %d: Status infos -- Source : %i -- Tag : %i\n", world_rank, source, tag);



          MPI_Probe(source, tag, MPI_COMM_WORLD, &status);

          // Get the number of integers in the message probed
          int count;
          MPI_Get_count(&status, MPI_INT, &count);

          // Allocate the buffer now that we know how many elements there are
          int* buffer = (int*)malloc(sizeof(int) * count);

          // Finally receive the message
          MPI_Recv(buffer, count, MPI_INT, MPI_Status_source(&status), MPI_Status_tag(&status), MPI_COMM_WORLD, &status);
          printf("Process %d: received message with all %d ints:", world_rank, count);

          for(int i = 0; i < count; i++)
          {
            printf(" %d", buffer[i]);
            if(buffer[i] != buffer0[i])
              ARCANE_FATAL("Erreur 11");
          }
          printf(".\n");
          break;
      }
    }
  }


  // -------------------- 12 -----------------------




  {
    switch(world_rank)
    {
      case 0:
      {    
        // Wait for the other process to issue an MPI_Iprobe in vain
        MPI_Barrier(MPI_COMM_WORLD);

        // Send the message
        int buffer = 12345;
        printf("Process %d: sending the message containing %d.\n", world_rank, buffer);
        MPI_Send(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        break;
      }
      case 1:
      {
        // The send has not been issued yet, this probe is vain but not blocking.
        int flag;
        MPI_Iprobe(0, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
        if(!flag)
        {
          printf("Process %d: no message arrived yet.\n", world_rank);
        }
        else
        {
          // This branching will not happen
          printf("Process %d: message arrived.\n", world_rank);
        }

        // Inform other MPI process that we issued the MPI_Iprobe meant to be vain
        MPI_Barrier(MPI_COMM_WORLD);

        // Actually receive the message
        int buffer;
        MPI_Recv(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d: message received containing %d.\n", world_rank, buffer);
        if(buffer != 12345)
          ARCANE_FATAL("Erreur 12");
        break;
      }
      default:
      {
        MPI_Barrier(MPI_COMM_WORLD);
        break;
      }
    }
  }




  // -------------------- 13 -----------------------





  {
    switch(world_rank)
    {
      case 0:
      {
        // The "master" MPI process sends the messages.
        int buffer[2] = {12345, 67890};
        MPI_Request requests[2];
        int recipient_rank_of_request[2];

        // Send first message to process 1
        printf("[Process %d] Sends %d to process 1.\n", world_rank, buffer[0]);
        MPI_Isend(&buffer[0], 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &requests[0]);
        recipient_rank_of_request[0] = 1;

        // Send second message to process 2
        printf("[Process %d] Sends %d to process 2.\n", world_rank, buffer[1]);
        MPI_Isend(&buffer[1], 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &requests[1]);
        recipient_rank_of_request[1] = 2;

        // Wait for one of non-blocking sends to complete
        int index;
        MPI_Waitany(2, requests, &index, MPI_STATUS_IGNORE);
        printf("[Process %d] The non-blocking send to process %d is complete.\n", world_rank, recipient_rank_of_request[index]);

        // Wait for the other non-blocking send to complete
        MPI_Waitany(2, requests, &index, MPI_STATUS_IGNORE);
        printf("[Process %d] The non-blocking send to process %d is complete too.\n", world_rank, recipient_rank_of_request[index]);
        break;
      }
      case 1:
      case 2:
      {
        // The "slave" MPI processes receive the message.
        int received;
        MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("[Process %d] Received value %d.\n", world_rank, received);
        break;
      }
    }
  }





  // -------------------- 14 -----------------------



  {
    // Determine the colour and key based on whether my rank is even.
    char subcommunicator;
    int colour;
    int key;

    if(world_rank % 2 == 0)
    {
      subcommunicator = 'A';
      colour = 0;
      key = world_rank;
    }
    else
    {
      subcommunicator = 'B';
      colour = 1;
      key = world_size - world_rank;
    }

    // Split de global communicator
    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, colour, key, &new_comm);

    // Get my rank in the new communicator
    int my_new_comm_rank, new_comm_size;
    MPI_Comm_rank(new_comm, &my_new_comm_rank);
    MPI_Comm_size(new_comm, &new_comm_size);

    // Print my new rank and new communicator
    printf("[MPI process %d] I am now MPI process %d/%d in subcommunicator %c.\n", world_rank, my_new_comm_rank, new_comm_size, subcommunicator);
  }
  





  // -------------------- 15 -----------------------



  {
    // Duplicate the MPI_COMM_WORLD communicator; everything is preserved, ranks included
    MPI_Comm duplicated_communicator;
    MPI_Comm_dup(MPI_COMM_WORLD, &duplicated_communicator);

    // The actual communicator duplication with MPI_Comm_dup is complete by now.
    // Below is a use case illustrating the usefulness of MPI_Comm_dup.

    // An MPI_Send issued from the user code
    if(world_rank == 0)
    {
      int some_message_in_user_code = 1234;
      MPI_Send(&some_message_in_user_code, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      printf("[MPI process 0] Sent %d in user code.\n", some_message_in_user_code);
    }

    // An MPI_Send issued by an MPI library running in parallel
    if(world_rank == 0)
    {
      int some_message_in_library_code = 5678;
      MPI_Send(&some_message_in_library_code, 1, MPI_INT, 1, 0, duplicated_communicator);
      printf("[MPI process 0] Sent %d in library code.\n", some_message_in_library_code);
    }

    // Same emitter rank, same receiver rank, same element datatype, same
    // element count but different communicators, they cannot be mismatched.

    // The user code can safely issue its MPI_Recv
    if(world_rank == 1)
    {
      int buffer_for_user_message;
      MPI_Recv(&buffer_for_user_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("[MPI process 1] Received %d in user code.\n", buffer_for_user_message);
    }

    // The MPI library can safely issue its MPI_Recv
    if(world_rank == 1)
    {
      int buffer_for_library_message;
      MPI_Recv(&buffer_for_library_message, 1, MPI_INT, 0, 0, duplicated_communicator, MPI_STATUS_IGNORE);
      printf("[MPI process 1] Received %d in library code.\n", buffer_for_library_message);
    }
  }



  // -------------------- 99 -----------------------
  //MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  MPI_Finalize();

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

}
ARCANE_REGISTER_MODULE_SAYHELLO(SayHelloModule);
