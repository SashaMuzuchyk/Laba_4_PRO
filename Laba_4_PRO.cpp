#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
 int counter, message, myid, numprocs, server;
 int color, remote_leader_rank, i, ICTAG = 0;
 MPI_Status status;
 MPI_Comm oldcommdup, splitcomm, oldcomm, inter_comm;
 MPI_Init(&argc, &argv);
 oldcomm = MPI_COMM_WORLD;
 MPI_Comm_dup(oldcomm, &oldcommdup);
 MPI_Comm_size(oldcommdup, &numprocs);
 MPI_Comm_rank(oldcommdup, &myid);
 server = numprocs-1;
 color = (myid == server);
 MPI_Comm_split(oldcomm, color, myid, &splitcomm);
 if (!color) {
 remote_leader_rank = server; 
 }
 else {
 remote_leader_rank = 0;
 }
 MPI_Intercomm_create(splitcomm, 0, oldcommdup,
remote_leader_rank, ICTAG, &inter_comm);
 MPI_Comm_free(&oldcommdup);
 if (myid == server) {
 for (i = 0; i<server; i++){
 MPI_Recv(&message, 1, MPI_INT, i, MPI_ANY_TAG,
inter_comm, &status);
 printf("Process rank %i received %i from %i\n",
myid, message, status.MPI_SOURCE);
 }
 }
 else{
 counter = myid;
 MPI_Send(&counter, 1, MPI_INT, 0, 0, inter_comm);
 printf("Process rank %i send %i\n", myid, counter);
 }
 MPI_Comm_free(&inter_comm);
 MPI_Finalize();
}