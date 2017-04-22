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
int duracion_hilos [5];

void
test_hello_world (void) 
{
  pruebahilos (5,0,"log.txt",30);
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
/* b: 0 si es I/O bounded, 1 si es CPU bounded */
/* l: Archive donde se escribiran los logs */
/* p: Procentaje de hilos I/O bounded */
static void 
pruebahilos (int t, int b, char* l, int p)
{
  ASSERT(!(t>25));
  
//  duracion_hilos[t];
  int time_execution=0;

  struct bound_test test;
  struct bound_thread *threads;
  int iterations = 5;
  int *output, *op;
  int product;
  int i;

  /* This test does not work with the MLFQS. */
  /*  ASSERT (!thread_mlfqs); */

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

 // printf("size of duration %d\n", sizeof(duracion_hilos));
 // for (i = 0; i< t; i++)
  //{
  //  time_execution += duracion_hilos[i];
  //  printf("tiempo ejecucion individual  %d\n", duracion_hilos[i]);
 // }
//  float average_time_execution = (int) time_execution / (float)t;
  snprintf(log + strlen(log), sizeof log, " El tiempo de ejecucion promedio de los hilos fue: %f.",
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

  int64_t tiempo_comienzo_hilo = timer_ticks ();
  timer_sleep (3);

  if ( io_bounded > 0 || cpu_bounded > 0 ) 
  {
    if (io_bounded >0)
    {
      for (i =1; i <= test->iterations; i++)
      {
        char* io_operation;
        io_operation =  malloc(16384);
        printf("Allocated %lu bytes.\n", sizeof(*io_operation)*16384 );
        lock_acquire (&test->output_lock);
        *test->output_pos++ = t->id;
        lock_release (&test->output_lock);
      }
      io_bounded--;  

      int64_t tiempo_finalizacion = timer_elapsed (tiempo_comienzo_hilo);
      duracion_hilos[t->id] = tiempo_finalizacion;
  printf("Impresion de duracionhilos: %d y %d\n ", t->id, duracion_hilos[t->id]);
      t->duration = tiempo_finalizacion;
      snprintf(log + strlen(log), sizeof log, " El hilo %d ha durado: %d ticks.", 
      t->id, tiempo_finalizacion);
      return;    
    }
    if (cpu_bounded >0)
    {
      for (i =1; i <= test->iterations; i++)
      {
        long double var1 = 19230986656.19230981231;
        long double var2 = 19230981231.19230981231;
        long double var3 = 19230981231.123123;
        long double var4 = var1*var2/var3;
        long double var5 = var4*99992312312.3453458907879;
        printf("Operation result: %ld\n",var5);

        lock_acquire (&test->output_lock);
        *test->output_pos++ = t->id;
        lock_release (&test->output_lock);
      }    
      cpu_bounded--; 
    } 
    int64_t tiempo_finalizacion = timer_elapsed (tiempo_comienzo_hilo);
    duracion_hilos[t->id] = tiempo_finalizacion;
  printf("Impresion de duracionhilos: %d y %d\n ", t->id, duracion_hilos[t->id]);
t->duration = tiempo_finalizacion;
    snprintf(log + strlen(log), sizeof log, " El hilo %d ha durado: %d ticks.", 
    t->id, tiempo_finalizacion);
    return;
  }

  if (thread_type ==1)
  { 
    for (i =1; i <= test->iterations; i++)
    {
      long double var1 = 19230986656.19230981231;
      long double var2 = 19230981231.19230981231;
      long double var3 = 19230981231.123123;
      long double var4 = var1*var2/var3;
      long double var5 = var4*99992312312.3453458907879;
      printf("Operation result: %ld\n",var5);

      lock_acquire (&test->output_lock);
      *test->output_pos++ = t->id;
      lock_release (&test->output_lock);
    }
  } 
  if (thread_type ==0)
  { 
    for (i =1; i <= test->iterations; i++)
    {
      char* io_operation;
      io_operation =  malloc(16384);
      printf("Allocated %lu bytes.\n", sizeof(*io_operation)*16384 );

      lock_acquire (&test->output_lock);
      *test->output_pos++ = t->id;
      lock_release (&test->output_lock);
    }
  }
  int64_t tiempo_finalizacion = timer_elapsed (tiempo_comienzo_hilo);
  duracion_hilos[t->id] = tiempo_finalizacion;
  printf("Impresion de duracionhilos: %d y %d\n ", t->id, duracion_hilos[t->id]);
  t->duration = tiempo_finalizacion;
  snprintf(log + strlen(log), sizeof log, " El hilo %d ha durado: %d ticks.", 
    t->id, tiempo_finalizacion);
}
