
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <time.h>



typedef struct {
 int (*A)[];
 int (*B)[];
 int (*C)[];
 int n;
 int start_row;
 int end_row;
} matrixArgs;

void matrixMultiply(matrixArgs* args);

const int MAX_SIZE = 4;

//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s [matrix_size] [optional_seed]\n", argv[0]);
    return 1;
  }

  int n = atoi(argv[1]);

  int seed = 0;
  if (argc == 3)
  {
    seed = atoi(argv[2]);
  }

  int (*A)[n] = (int(*)[n]) calloc(n, sizeof(int[n]));
  int (*B)[n] = (int(*)[n]) calloc(n, sizeof(int[n]));
  int (*C)[n] = mmap(NULL, n*n*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);





  // Initialize input matrices with random values
  if (n <= MAX_SIZE)
  {
    srand(seed);
  }
  else
  {
    srand(time(NULL));
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      A[i][j] = rand() % 10;
      B[i][j] = rand() % 10;
    }
  }

  if (n <= MAX_SIZE)
  {
    // Print the input matrices
    printf("Input matrices:\n");
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        printf("%d ", A[i][j]);
      }
      printf("\n");
    }
    printf("\n");
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        printf("%d ", B[i][j]);
      }
      printf("\n");
    }
  }

//   matrixArgs data;
//   data.n = n;
//   data.A = A;
//   data.B = B;
//   data.C = C;

  int rowsPerProcess = n / 2;

  matrixArgs childArgs;
  childArgs.n = n;
  childArgs.A = A;
  childArgs.B = B;
  childArgs.C = C;
  childArgs.start_row = 0;
  childArgs.end_row = rowsPerProcess;
  
  matrixArgs parentArgs;
  parentArgs.n = n;
  parentArgs.A = A;
  parentArgs.B = B;
  parentArgs.C = C;
  parentArgs.start_row = rowsPerProcess;
  parentArgs.end_row = n;
  
  pid_t pid = fork();

  if(pid == 0) {
    matrixMultiply(&childArgs);
    msync(C, n*n*sizeof(int), MS_SYNC);
    exit(0);
  } 
  else if (pid > 0) {
    matrixMultiply(&parentArgs);
    wait(NULL);
  } else {
    printf("Error: fork() failed");
  }
  
//   ret = pthread_create(&thread, NULL, matrixMultiply, (void*)&data);
//   if(ret != 0) {
//     printf("Error: pthread_create() failed");
//   }


  if (n <= MAX_SIZE)
  {
    // Print the result matrix
    printf("\nResultant matrix is: \n");
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        printf("%d ", C[i][j]);
      }
      printf("\n");
    }
  }

  printf("\nSUCCESSFULLY DONE\n");

  
  free(A);
  free(B);
  munmap(C, n*n*sizeof(int));

  return 0;
}

void matrixMultiply(matrixArgs* args) 
{
    matrixArgs* data = (matrixArgs*) args;
    int n = data->n;
    int (*A)[n] = data-> A;
    int (*B)[n] = data -> B;
    int (*C)[n] = data -> C;
    int start_row = data-> start_row;
    int end_row = data-> end_row;
  // Perform matrix multiplication
  for (int i = start_row; i < end_row; i++)
  {
    for (int j = 0; j < n; j++)
    {
      C[i][j] = 0;
      for (int k = 0; k < n; k++)
      {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
  
}
