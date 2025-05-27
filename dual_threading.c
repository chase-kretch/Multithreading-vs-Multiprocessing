

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void* matrixMultiply(void* arg);

typedef struct {
 int (*A)[];
 int (*B)[];
 int (*C)[];
 int n;
 int start_row;
 int end_row;
} matrixArgs;

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
  int (*C)[n] = (int(*)[n]) calloc(n, sizeof(int[n]));


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

  int rowsPerThread = n / 2;

  pthread_t thread1, thread2;

  matrixArgs thread1Args;
  thread1Args.n = n;
  thread1Args.A = A;
  thread1Args.B = B;
  thread1Args.C = C;
  thread1Args.start_row = 0;
  thread1Args.end_row = rowsPerThread;
  
  matrixArgs thread2Args;
  thread2Args.n = n;
  thread2Args.A = A;
  thread2Args.B = B;
  thread2Args.C = C;
  thread2Args.start_row = rowsPerThread;
  thread2Args.end_row = n;
  int ret;

  ret = pthread_create(&thread1, NULL, matrixMultiply, (void*)&thread1Args);
  if(ret != 0) {
    printf("ERROR: pthread_create() failed");
    return 1; 
  }
  ret = pthread_create(&thread2, NULL, matrixMultiply, (void*)&thread2Args);
  if(ret != 0) {
    printf("ERROR: pthread_create() failed");
    return 1; 
  }

  
//   ret = pthread_create(&thread, NULL, matrixMultiply, (void*)&data);
//   if(ret != 0) {
//     printf("Error: pthread_create() failed");
//   }

  ret = pthread_join(thread1, NULL);
  if(ret != 0) {
        printf("Error: pthread_join()) failed");
        return 1;
    }
  ret = pthread_join(thread2, NULL);
  if(ret != 0) {
        printf("Error: pthread_join()) failed");
        return 1;
    }
  
 


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
  free(C);

  return 0;
}

void* matrixMultiply(void* arg) 
{
    matrixArgs* data = (matrixArgs*) arg;
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
