// File with MPI examples to test mpA.

#include <iostream>
#include <mpA.h>

int main(int argc, char* argv[])
{
  MPA::MPI_Init(&argc, &argv);

  int world_rank;
  MPA::MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPA::MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  std::cout << "Hello from " << world_rank << "/" << world_size << std::endl; 


  int root_rank = 0;




  // Examples from RookieHPC :

  // -------------------- 1 -----------------------






  int number;
  if (world_rank == 0) {
    number = -1;
    MPA::MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
  } 

  else if (world_rank == 1) {
    MPA::MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,
            MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n",
          number);
  }






  // -------------------- 2 -----------------------







  switch(world_rank)
  {
      case 0:
      {
          int buffer_sent = 12345;
          MPA::MPI_Request request;
          printf("MPI process %d sends value %d.\n", world_rank, buffer_sent);
          MPA::MPI_Isend(&buffer_sent, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
          
          // Do other things while the MPI_Isend completes
          // <...>

          // Let's wait for the MPI_Isend to complete before progressing further.
          MPA::MPI_Wait(&request, MPI_STATUS_IGNORE);
          break;
      }
      case 1:
      {
          int received;
          MPA::MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          printf("MPI process %d received value: %d.\n", world_rank, received);
          break;
      }
  }







  // -------------------- 3 -----------------------






  switch(world_rank)
  {
    case 0:
    {
        // The "master" MPI process receives the messages.
        int message_1;
        int message_2;
        MPA::MPI_Request requests[2];
        int ready;

        MPA::MPI_Irecv(&message_1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &requests[0]);
        MPA::MPI_Irecv(&message_2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &requests[1]);

        // This MPI_Testall is guaranteed to fail since the corresponding MPI_Ssends have not been issued yet.
        MPA::MPI_Testall(2, requests, &ready, MPI_STATUSES_IGNORE);
        if(ready)
            printf("[Process 0] First attempt: both receives are complete.\n");
        else
            printf("[Process 0] First attempt: not both receives are complete yet.\n");

        // We can tell other processes to start sending messages
        MPA::MPI_Barrier(MPI_COMM_WORLD);

        // We wait for the other processes to tell us their MPI_Ssend is complete
        MPA::MPI_Barrier(MPI_COMM_WORLD);

        // This MPI_Testall is guaranteed to succeed since the corresponding MPI_Ssends are all complete.
        MPA::MPI_Testall(2, requests, &ready, MPI_STATUSES_IGNORE);
        if(ready)
            printf("[Process 0] Second attempt: both receives are complete.\n");
        else
            printf("[Process 0] Second attempt: not both receives are complete yet.\n");
        break;
    }
    case 1:
    case 2:
    {
        // The "slave" MPI processes send the messages.
        int message = (world_rank == 1) ? 12345 : 67890;

        // Wait for the MPI_Testall #1 to be done.
        MPA::MPI_Barrier(MPI_COMM_WORLD);

        MPA::MPI_Send(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

        // Tell the sender it can now issue the MPI_Testall #2.
        MPA::MPI_Barrier(MPI_COMM_WORLD);
        break;
    }
    default:
    {
      MPA::MPI_Barrier(MPI_COMM_WORLD);
      MPA::MPI_Barrier(MPI_COMM_WORLD);
      break;
    }
  }






  // -------------------- 4 -----------------------





  // Determine the rank of the broadcast emitter process
  int broadcast_root = 0;

  int buffer;
  if(world_rank == broadcast_root)
  {
      buffer = 12345;
      printf("[MPI process %d] I am the broadcast root, and send value %d.\n", world_rank, buffer);
  }
  MPA::MPI_Bcast(&buffer, 1, MPI_INT, broadcast_root, MPI_COMM_WORLD);
  if(world_rank != broadcast_root)
  {
      printf("[MPI process %d] I am a broadcast receiver, and obtained value %d.\n", world_rank, buffer);
  }





  // -------------------- 5 -----------------------






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
          MPA::MPI_Gatherv(&my_value, 1, MPI_INT, buffer, counts, displacements, MPI_INT, root_rank, MPI_COMM_WORLD);
          printf("Values gathered in the buffer on process %d:", world_rank);
          for(int i = 0; i < 7; i++)
          {
              printf(" %d", buffer[i]);
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
          MPA::MPI_Gatherv(&my_value, 1, MPI_INT, NULL, NULL, NULL, MPI_INT, root_rank, MPI_COMM_WORLD);
          break;
      }
      case 2:
      {
          // Define my values
          int my_values[2] = {102, 103};

          printf("Process %d, my values = %d %d.\n", world_rank, my_values[0], my_values[1]);
          MPA::MPI_Gatherv(my_values, 2, MPI_INT, NULL, NULL, NULL, MPI_INT, root_rank, MPI_COMM_WORLD);
          break;
      }
  }








  // -------------------- 6 -----------------------







  // Define the receive counts
  int counts[3] = {1, 1, 2};

  // Define the displacements
  int displacements[3] = {0, 3, 5};

  // Buffer in which receive the data collected
  int buffer2[7] = {0};

  // Buffer containing our data to send
  int* my_values;
  int my_values_count;

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

  MPA::MPI_Allgatherv(my_values, my_values_count, MPI_INT, buffer2, counts, displacements, MPI_INT, MPI_COMM_WORLD);

  printf("Values gathered in the buffer on process %d:", world_rank);
  for(int i = 0; i < 7; i++)
  {
      printf(" %d", buffer2[i]);
  }
  printf("\n");
  free(my_values);
 
  


  // -------------------- 7 -----------------------






  // Each MPI process sends its rank to reduction, root MPI process collects the result
  int reduction_result = 0;
  int truc = world_rank * 123;

  MPA::MPI_Reduce(&truc, &reduction_result, 1, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);

  if(world_rank == root_rank)
  {
    printf("The sum of all ranks is %d.\n", reduction_result);
  }




  // -------------------- 8 -----------------------





  // Each MPI process sends its rank to reduction, root MPI process collects the result
  reduction_result = 0;
  MPA::MPI_Allreduce(&truc, &reduction_result, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

  printf("[MPI Process %d] The sum of all ranks is %d.\n", world_rank, reduction_result);
 




  // -------------------- 9 -----------------------





  // Define my value
  int my_value;

  if(world_rank == root_rank)
  {
      int buffer[4] = {0, 100, 200, 300};
      printf("Values to scatter from process %d: %d, %d, %d, %d.\n", world_rank, buffer[0], buffer[1], buffer[2], buffer[3]);
      MPA::MPI_Scatter(buffer, 1, MPI_INT, &my_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
  }
  else
  {
      MPA::MPI_Scatter(NULL, 1, MPI_INT, &my_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
  }

  printf("Process %d received value = %d.\n", world_rank, my_value);






  // -------------------- 10 -----------------------






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
          MPA::MPI_Scatterv(buffer, counts, displacements, MPI_INT, &my_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
          printf("Process %d received value %d.\n", world_rank, my_value);
          break;
      }
      case 1:
      {
          // Declare my values
          int my_values[2];

          MPA::MPI_Scatterv(NULL, NULL, NULL, MPI_INT, my_values, 2, MPI_INT, root_rank, MPI_COMM_WORLD);
          printf("Process %d received values %d and %d.\n", world_rank, my_values[0], my_values[1]);
          break;
      }
      case 2:
      {
          // Declare my values
          int my_value;

          MPA::MPI_Scatterv(NULL, NULL, NULL, MPI_INT, &my_value, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
          printf("Process %d received value %d.\n", world_rank, my_value);
          break;
      }
  }





  // -------------------- 11 -----------------------




  switch(world_rank)

  {
    case 0:
    {
        int buffer[3] = {123, 456, 789};
        printf("Process %d: sending a message containing 3 ints (%d, %d, %d), but the receiver is not aware of the length.\n", world_rank, buffer[0], buffer[1], buffer[2]);
        MPA::MPI_Send(buffer, 3, MPI_INT, 1, 0, MPI_COMM_WORLD);
        break;
    }

    case 1:
    {
        // Retrieve information about the incoming message
        MPA::MPI_Status status;
        MPA::MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
        printf("Process %d: obtained message status by probing it.\n", world_rank);

        // Get the number of integers in the message probed
        int count;
        MPA::MPI_Get_count(&status, MPI_INT, &count);

        // Allocate the buffer now that we know how many elements there are
        int* buffer = (int*)malloc(sizeof(int) * count);

        // Finally receive the message
        MPA::MPI_Recv(buffer, count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process %d: received message with all %d ints:", world_rank, count);

        for(int i = 0; i < count; i++)
        {
            printf(" %d", buffer[i]);
        }
        printf(".\n");
        break;
    }
  }



  // -------------------- 12 -----------------------





  switch(world_rank)
  {
    case 0:
    {    
      // Wait for the other process to issue an MPI_Iprobe in vain
      MPA::MPI_Barrier(MPI_COMM_WORLD);

      // Send the message
      int buffer = 12345;
      printf("Process %d: sending the message containing %d.\n", world_rank, buffer);
      MPA::MPI_Send(&buffer, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      break;
    }
    case 1:
    {
      // The send has not been issued yet, this probe is vain but not blocking.
      int flag;
      MPA::MPI_Iprobe(0, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
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
      MPA::MPI_Barrier(MPI_COMM_WORLD);

      // Actually receive the message
      int buffer;
      MPA::MPI_Recv(&buffer, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("Process %d: message received containing %d.\n", world_rank, buffer);
      break;
    }
    default:
    {
      MPA::MPI_Barrier(MPI_COMM_WORLD);
      break;
    }
  }



  // -------------------- 13 -----------------------








  switch(world_rank)
  {
    case 0:
    {
      // The "master" MPI process sends the messages.
      int buffer[2] = {12345, 67890};
      MPA::MPI_Request requests[2];
      int recipient_rank_of_request[2];

      // Send first message to process 1
      printf("[Process %d] Sends %d to process 1.\n", world_rank, buffer[0]);
      MPA::MPI_Isend(&buffer[0], 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &requests[0]);
      recipient_rank_of_request[0] = 1;

      // Send second message to process 2
      printf("[Process %d] Sends %d to process 2.\n", world_rank, buffer[1]);
      MPA::MPI_Isend(&buffer[1], 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &requests[1]);
      recipient_rank_of_request[1] = 2;

      // Wait for one of non-blocking sends to complete
      int index;
      MPA::MPI_Waitany(2, requests, &index, MPI_STATUS_IGNORE);
      printf("[Process %d] The non-blocking send to process %d is complete.\n", world_rank, recipient_rank_of_request[index]);

      // Wait for the other non-blocking send to complete
      MPA::MPI_Waitany(2, requests, &index, MPI_STATUS_IGNORE);
      printf("[Process %d] The non-blocking send to process %d is complete too.\n", world_rank, recipient_rank_of_request[index]);
      break;
    }
    case 1:
    case 2:
    {
      // The "slave" MPI processes receive the message.
      int received;
      MPA::MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("[Process %d] Received value %d.\n", world_rank, received);
      break;
    }
  }






  // -------------------- 14 -----------------------




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
  MPA::MPI_Comm new_comm;
  MPA::MPI_Comm_split(MPI_COMM_WORLD, colour, key, &new_comm);

  // Get my rank in the new communicator
  int my_new_comm_rank, new_comm_size;
  MPA::MPI_Comm_rank(new_comm, &my_new_comm_rank);
  MPA::MPI_Comm_size(new_comm, &new_comm_size);

  // Print my new rank and new communicator
  printf("[MPI process %d] I am now MPI process %d/%d in subcommunicator %c.\n", world_rank, my_new_comm_rank, new_comm_size, subcommunicator);
  




  MPA::MPI_Finalize();
  return 0;
}
