
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "testfw.h"
#include <fcntl.h>      /* Définitions des constantes O_*   */
#include <unistd.h>
#include <sys/types.h>
 #include <sys/wait.h>

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
  struct test_t * tab[TESTSIZE];
} ;

/* ********** FRAMEWORK ********** */

struct testfw_t *testfw_init(char *program, int timeout, char *logfile, char *cmd, bool silent, bool verbose)
{
    struct testfw_t *fw = malloc(sizeof(struct testfw_t));

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
    free(fw -> tab[i]);// ne pas free un pointeur NULL
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
     exit(EXIT_FAILURE);
   }
    return fw->tab[k];
}

/* ********** REGISTER TEST ********** */

struct test_t *testfw_register_func(struct testfw_t *fw, char *suite, char *name, testfw_func_t func)
{

  //ne pas oublier les cas limites
  struct test_t * test = malloc(sizeof(struct test_t));

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
  struct test_t * test = malloc(sizeof(struct test_t));

  test->suite = suite;
  test->name = name;

  void *handle;
  testfw_func_t func_test;


  if (!(handle = dlopen (NULL, RTLD_LAZY)))
    {
      printf ("Erreur dlopen: %s\n", dlerror ());
      exit (EXIT_FAILURE);
    }

  if (!(func_test = dlsym (handle, strcat(strcat(suite,"_"),name)))) // Utiliser grep en redirigeant sa sortie avec un pipe.
   {
     printf ("Erreur dlsym: %s\n", dlerror ());
     dlclose (handle);
     exit (EXIT_FAILURE);
   }

  dlclose (handle);
  test->func = func_test;
  fw->tab[fw->count] = test;
  fw->count++;

  return test;
}


int testfw_register_suite(struct testfw_t *fw, char *suite)


{
  FILE * f_in = popen(strcat("grep sample ", suite), "w");
  FILE * f_out = popen(strcat("cut -d _ -f 2 ", suite), "r");

//  dup2(f, 1);

  if (!fork()){
    execlp("cut","cut -d _ -f 2 " , NULL);
  }else{
    wait(NULL);
  }
  return 0;
}

/* ********** RUN TEST ********** */

int testfw_run_all(struct testfw_t *fw, int argc, char *argv[], enum testfw_mode_t mode)
{
  int fail = 0;
  if (argc == 1){
    for (int i = 0; i < fw->count; i++){ //toutes les fonctions ont les mêmes arguments. bcp plus simple
      if (!  ((fw->tab[i])->func )(argc, argv))  {
        fail++;
      }
    }
  }
  return fail;
}
