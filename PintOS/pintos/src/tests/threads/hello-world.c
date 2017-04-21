

/* Tests timer_sleep(0), which should return immediately. */

#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"

static void pruebahilos(int, int, int, int);
int thread_type =0;

void
test_hello_world (void) 
{
  pruebahilos (5,2,3,4);
  thread_type = 0;
}

/* Information about the test. */
struct bound_test 
  {
    int64_t start;              /* Current time at start of test. */
    int iterations;             /* Number of iterations per thread. */

    /* Output. */
    struct lock output_lock;    /* Lock protecting output buffer. */
    int *output_pos;            /* Current position in output buffer. */
  };

/* Information about an individual thread in the test. */
struct bound_thread 
  {
    struct bound_test *test;     /* Info shared between all threads. */
    int id;                     /* Bound ID. */
    int duration;               /* Number of ticks to sleep. */
    int iterations;             /* Iterations counted so far. */
  };
static void bound_operation (void *);

/* t: cantidad de hilos*/
/* b: 1 si es I/O bounded, 0 si es CPU bounded */
/* l: Archive donde se escribiran los logs */
/* p: Procentaje de hilos I/O bounded */
static void 
pruebahilos (int t, int b, int l, int p)
{
  ASSERT(!(t>25))

  struct bound_test test;
  struct bound_thread *threads;
  int iterations = 5;
  int *output, *op;
  int product;
  int i;

  /* This test does not work with the MLFQS. */
  /*  ASSERT (!thread_mlfqs); */

  msg ("Creando %d threads que dormiran %d veces cada uno.", t, iterations);
  msg ("Thread 0 duerme 10 ticks cada vez,");
  msg ("thread 1 duerme 20 ticks cada vez, y asi en adelante.");
  msg ("Si es exitoso, el producto de la cuenta de iteraciones y ");
  msg ("duracion de dormido aparecera en orden no descendiente.");

  /* Allocate memory. */
  threads = malloc (sizeof *threads * t);
  output = malloc (sizeof *output * iterations * t * 2);
  if (threads == NULL || output == NULL)
    PANIC ("couldn't allocate memory for test");

  /* Initialize test. */
  test.start = timer_ticks () + 100;
  test.iterations = iterations;
  lock_init (&test.output_lock);
  test.output_pos = output;

  /* Start threads. */
  ASSERT (output != NULL);
  for (i = 0; i < t; i++)
    {
      struct bound_thread *t = threads + i;
      char name[16];
      
      t->test = &test;
      t->id = i;
      t->duration = (i + 1) * 10;
      t->iterations = 0;
      
      snprintf (name, sizeof name, "thread %d", i);
      thread_create (name, PRI_DEFAULT, bound_operation, t);
    }

  timer_sleep (100 + t * iterations * 10 + 100);

  /* Acquire the output lock in case some rogue thread is still
     running. */
  lock_acquire (&test.output_lock);
      printf("ha entrado aqui, %d\n", *output);

  /* Print completion order. */
  product = 0;
  for (op = output; op < test.output_pos; op++) 
    {
      printf("ha entrado,%d\n", *op);
      struct bound_thread *t;
      int new_prod;

      ASSERT (*op >= 0 && *op < t);
      t = threads + *op;

      new_prod = ++t->iterations * t->duration;
        
      msg ("thread %d: duration=%d, iteration=%d, product=%d",
           t->id, t->duration, t->iterations, new_prod);
      
      // if (new_prod >= product)
      //   product = new_prod;
      // else
      //   fail ("thread %d woke up out of order (%d > %d)!",
      //         t->id, product, new_prod);
    }

  lock_release (&test.output_lock);
  free (output);
  free (threads);

}

/* Bound thread. */
static void
bound_operation (void *t_) 
{
  struct bound_thread *t  = t_;
  struct bound_test *test = t->test;
  int i;
  printf("living is easy with %d\n", thread_type);
  if (thread_type ==1)
  { 
    for (i =1; i <= test->iterations; i++)
    {
      long double var1 = 19230986656.19230981231;
      long double var2 = 19230981231.19230981231;
      long double var3 = 19230981231.123123;
      long double var4 = var1*var2/var3;
      long double var5 = var4*99992312312.3453458907879;
      printf("Operation result: %lu\n",var5);

      lock_acquire (&test->output_lock);
      *test->output_pos++ = t->id;
      lock_release (&test->output_lock);
    }
  } 
  if (thread_type ==0)
  { 
    for (i =1; i <= test->iterations; i++)
    {
    char* io_bound =  malloc(16384 *90);

      lock_acquire (&test->output_lock);
      *test->output_pos++ = t->id;
      lock_release (&test->output_lock);
    }
  }  
}