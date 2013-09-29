#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#define MAX_NUM 2000000

// Default number of threads to run.
static uint32_t DEFAULT_THREADS = 2;
static pthread_t *thread_pool = NULL;
void *prime_tester(void *thread_num);

int main(int argc, char **argv) {
  // TODO Read argv.
  uint32_t nthreads = DEFAULT_THREADS;
  thread_pool = malloc(nthreads * sizeof(thread_pool));

  // INIT ---------------------------------------------------------------------
  // Creates the threads pointing to the function in question.
  int res;
  for (long i = 0; i < nthreads; ++i) {
    res = pthread_create(&thread_pool[i], NULL, prime_tester,
        (void *)i);
    if (res) {
      printf("ERR!  return code from pthread_create() is %d\n", res);
      return EXIT_FAILURE;
    }
  }

  // CLEANUP ------------------------------------------------------------------
  // Join all the threads before freeing everything.
  long accumulator = 0;
  long join_res;
  for (long i = 0; i < nthreads; ++i) {
    pthread_join(thread_pool[i], (void **)&join_res);
    accumulator += join_res;
  }
  printf("The sum is %ld\n", accumulator);
  free(thread_pool);
  pthread_exit(NULL);  // Exits the "main" thread.
  return EXIT_SUCCESS;
}

static inline bool prime_test(long n) {
  if (n <= 1L) {
    return false;
  }
  if (n % 2L == 0) {
    return 2L == n;
  }
  if (n % 3L == 0) {
    return 3L == n;
  }
  for (long i = 5L; i * i <= n; i += 6L) {
    if (n % i == 0L)
      return false;
    if (n % (i + 2) == 0L)
      return false;
  }
  return true;
}

void *prime_tester(void *thread_num) {
  const long range = MAX_NUM / DEFAULT_THREADS;
  long thread_id = (long) thread_num;
  long accumulator = 0;
  for (long i = range * thread_id; i < (thread_id * range) + range; ++i) {
    if (prime_test(i)) {
      accumulator += i;
    }
  }
  pthread_exit((void *)accumulator);
}
