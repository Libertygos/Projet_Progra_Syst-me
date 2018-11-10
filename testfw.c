
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>

#include "testfw.h"
#define TESTSIZE 50
/* ********** STRUCTURES ********** */

struct testfw_t
{
  char *program;
  int timeout;
  char *logfile;
  char *cmd;
  bool silent;
  bool verbose;
  int count;
  test_t * tab[TESTSIZE];
} ;

/* ********** FRAMEWORK ********** */

struct testfw_t *testfw_init(char *program, int timeout, char *logfile, char *cmd, bool silent, bool verbose)
{
    struct testfw_t fw = malloc(sizeof(testfw_t));

    fw -> program = program;
    fw -> timeout = timeout;
    fw -> logfile = logfile;
    fw -> cmd = cmd;
    fw -> silent = silent;
    fw -> verbose = verbose;
    fw -> count = 0;
    for (int i = 0; i < TESTSIZE; i ++){
      fw -> tab[i] = NULL;
    }

    return fw;
}

void testfw_free(struct testfw_t *fw)
{
  for (int i = 0; i < TESTSIZE; i ++){
    free(fw -> tab[i]);
  }
  free(fw);
}

int testfw_length(struct testfw_t *fw)
{
    return fw-> count;
}

struct test_t *testfw_get(struct testfw_t *fw, int k)
{
   if (fw->tab[k] == NULL){
     perror(EXIT_FAILURE);
   }
    return fw->tab[k];
}

/* ********** REGISTER TEST ********** */

struct test_t *testfw_register_func(struct testfw_t *fw, char *suite, char *name, testfw_func_t func)
{

  //ne pas oublier les cas limites
  struct test_t * test = malloc(sizeof(test_t));

  test->suite = suite;
  test->name = name;
  test->func = func;

  fw->tab[fw->count] = test;
  fw->count++;
  return test;
}

struct test_t *testfw_register_symb(struct testfw_t *fw, char *suite, char *name)
{
  //ne pas oublier les cas limites
  struct test_t * test = malloc(sizeof(test_t));

  test->suite = suite;
  test->name = name;

  void *handle;
  testfw_func_t func_test;


  if (!(handle = dlopen ("hello.c", RTLD_LAZY)))
    {
      printf ("Erreur dlopen: %s\n", dlerror ());
      exit (EXIT_FAILURE);
    }
  if (!(func_test = dlsym (handle, strcat(strcat(suite,"_"),name))))
   {
     printf ("Erreur dlsym: %s\n", dlerror ());
     dlclose (handle);
     exit (EXIT_FAILURE);
   }

  dlclose (handle);
  test->func = func_test;
  fw->tab[fw->count] = test;
  fw->count++;
}

int testfw_register_suite(struct testfw_t *fw, char *suite)
{
    return 0;
}

/* ********** RUN TEST ********** */

int testfw_run_all(struct testfw_t *fw, int argc, char *argv[], enum testfw_mode_t mode)
{
    return 0;
}
