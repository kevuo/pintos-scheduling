#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "devices/timer.h"
#include <string.h>

static void pruebahilos(int, int, char*, int);

int thread_type;
int cpu_bounded =0;
int io_bounded =0;
char log [256];

void
test_prueba_hilo (void) 
{
  pruebahilos (8,0,"log.txt",45);
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
    int duration;               /* Execution time. */
    int iterations;             /* Iterations counted so far. */
  };
static void bound_operation (void *);

/* t: cantidad de hilos*/
/* b: 0 si es I/O bounded, 1 si es CPU bounded */
/* l: Archive donde se escribiran los logs */
/* p: Procentaje de hilos I/O bounded */
static void 
pruebahilos (int t, int b, char* l, int p)
{
  ASSERT(!(t>25));
  
  int time_execution=0;

  struct bound_test test;
  struct bound_thread *threads;
  int iterations = 5;
  int *output, *op;
  int product;
  int i;

  msg ("Creando %d threads con %d iteraciones.", t, iterations);
  msg ("Creando log en %s", l);
  
  thread_type = b;
  if (p != -1)
  {
    float divide = (float)p/100.0;
    io_bounded = t* divide;
    cpu_bounded = t* (1-divide);
    thread_type = 0;
  }

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

  snprintf (log, sizeof log, "Log del set de hilos. Creando %d hilos. %d I/O bounded y %d CPU bounded.", 
    t, io_bounded, cpu_bounded);

  /* Start threads. */
  ASSERT (output != NULL);
  for (i = 0; i < t; i++)
    {
      struct bound_thread *t = threads + i;
      char name[16];
      
      t->test = &test;
      t->id = i;
      t->iterations = 0;
      snprintf(log + strlen(log), sizeof log, " El id del hilo %d es %d." ,i, t->id);

      /* Save thread name in the name buffer */
      snprintf (name, sizeof name, "thread %d", i);
      thread_create (name, PRI_DEFAULT, bound_operation, t);
    }
  

  timer_sleep (t * iterations * 10+ 100);

  /* Acquire the output lock in case some rogue thread is still
     running. */
  lock_acquire (&test.output_lock);
      printf("", *output);

  /* Print completion order. */
  product = 0;
  for (op = output; op < test.output_pos; op++) 
    {
      printf("", *op);
      struct bound_thread *t;
      int new_prod;

      ASSERT (*op >= 0 && *op < t);
      t = threads + *op;

      ++t->iterations;
      time_execution += t->duration;

      msg ("thread %d: iteration=%d",
           t->id,  t->iterations);
    }

  lock_release (&test.output_lock);
  free (output);
  free (threads);

  /* Save total execution time to log*/
  snprintf(log + strlen(log), sizeof log, " El tiempo de ejecucion promedio de los hilos fue: %d.",
    time_execution);

  printf("%s\n", log);

}

/* Bound thread. */
static void
bound_operation (void *t_) 
{
  struct bound_thread *t  = t_;
  struct bound_test *test = t->test;
  int i;
  printf("El hilo %d ha comenzado.\n", t->id);
  int64_t tiempo_comienzo_hilo = timer_ticks ();
  timer_sleep (3);

  /* Case where the p (percentage) is given*/
  if ( io_bounded > 0 || cpu_bounded > 0 ) 
  {
    /* If there are I/O bound operations remaining */
    if (io_bounded >0)
    {
      for (i =1; i <= test->iterations; i++)
      {
        char* io_operation;
        io_operation =  malloc(16384);
        printf("Allocated %lu bytes of thread %d. \n", sizeof(*io_operation)*16384, t->id );
        
        lock_acquire (&test->output_lock);
        *test->output_pos++ = t->id;
        lock_release (&test->output_lock);
      }
      io_bounded--;  
  
      /* Compute time of execution, save it in thre thread member: duration */
      int64_t tiempo_finalizacion = timer_elapsed (tiempo_comienzo_hilo);
      t->duration = tiempo_finalizacion;
   
      /* Store log data in the log buffer "log" */
      snprintf(log + strlen(log), sizeof log, " El hilo %d ha durado: %d ticks.", 
      t->id, tiempo_finalizacion);
      return;    
    }

    /* If there are CPU bound operations remaining */
    if (cpu_bounded >0)
    {
      for (i =1; i <= test->iterations; i++)
      {
        unsigned long long var1 = 19230986656;
        unsigned long long var2 = 19230981231;
        unsigned long long var3 = 19230981231;
        unsigned long long var4 = var1*var2/var3;
        unsigned long long var5 = var4*99992312312;
        printf("Operation result: %llu of thread %d.\n",var5, t->id );

        lock_acquire (&test->output_lock);
        *test->output_pos++ = t->id;
        lock_release (&test->output_lock);
      }    
      cpu_bounded--; 
    } 

    /* Compute time of execution, save it in thre thread member: duration */
    int64_t tiempo_finalizacion = timer_elapsed (tiempo_comienzo_hilo);
    t->duration = tiempo_finalizacion;
    
    /* Store log data in the log buffer "log" */
    snprintf(log + strlen(log), sizeof log, " El hilo %d ha durado: %d ticks.", 
    t->id, tiempo_finalizacion);
    return;
  }
  
  /* Case where the b is given */
  /* All operations are CPU bounded*/
  if (thread_type ==1)
  { 
    for (i =1; i <= test->iterations; i++)
    {
      unsigned long long var1 = 19230986656;
      unsigned long long var2 = 19230981231;
      unsigned long long var3 = 19230981231;
      unsigned long long var4 = var1*var2/var3;
      unsigned long long var5 = var4*99992312312;
      printf("Operation result: %llu of thread %d.\n",var5, t->id );

      lock_acquire (&test->output_lock);
      *test->output_pos++ = t->id;
      lock_release (&test->output_lock);
    }
  } 

  /* All operations are I/O bounded*/
  if (thread_type ==0)
  { 
    for (i =1; i <= test->iterations; i++)
    {
      char* io_operation;
      io_operation =  malloc(16384);
      printf("Allocated %lu bytes of thread %d.\n", sizeof(*io_operation)*16384, t->id );

      lock_acquire (&test->output_lock);
      *test->output_pos++ = t->id;
      lock_release (&test->output_lock);
    }
  }
  /* Compute time of execution, save it in thre thread member: duration */
  int64_t tiempo_finalizacion = timer_elapsed (tiempo_comienzo_hilo);
  t->duration = tiempo_finalizacion;

  /* Store log data in the log buffer "log" */
  snprintf(log + strlen(log), sizeof log, " El hilo %d ha durado: %d ticks.", 
    t->id, tiempo_finalizacion);
}
