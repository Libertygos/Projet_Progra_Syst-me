
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
#define TESTFILE "sample.c"
#define TESTMAXCHAR 50
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
  //We are using a tab for the test_t pointers
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
    //Initialisation of the test_t pointers
    for (int i = 0; i < TESTSIZE; i ++){
      fw -> tab[i] = NULL;
    }

    return fw;
}

void testfw_free(struct testfw_t *fw)
{
  for (int i = 0; i < TESTSIZE; i ++){
    if ( (testfw_get(fw, i))->name != NULL){
      free((testfw_get(fw, i))->name);
    }

   if ( (testfw_get(fw, i))->suite != NULL){
      free((testfw_get(fw, i))->suite);
    }
    free(testfw_get(fw, i));
  }
  free(fw);
}

int testfw_length(struct testfw_t *fw)
{
    return fw->count;
}

struct test_t *testfw_get(struct testfw_t *fw, int k)
{
   if (fw->tab[k] == NULL)
   {
     exit(EXIT_FAILURE);
   }
   else
   {
     return fw->tab[k];
   }
}

/* ********** REGISTER TEST ********** */

struct test_t *testfw_register_func(struct testfw_t *fw, char *suite, char *name, testfw_func_t func)
{

  struct test_t * test = malloc(sizeof(struct test_t));


  test->suite  = (char *)malloc(sizeof(char) * strlen(suite));
  test->name =  (char *)malloc(sizeof(char) * strlen(name));

  for (int i = 0; i < strlen(suite); i++){
    ( test->suite )[i] = suite[i];
  }

  for (int i = 0; i < strlen(name); i++){
    ( test->name )[i] = name[i];
  }

  test->func = func;

  fw->tab[ fw->count ] = test;

  //We are adding a test_t pointer so the size is increased
  fw->count++;
  return test;
}


struct test_t *testfw_register_symb(struct testfw_t *fw, char *suite, char *name)
{

  struct test_t * test = malloc(sizeof(struct test_t));

  char *suite_name;

  suite_name = malloc((strlen(suite) + strlen(name) + 1)*sizeof(char));
  strcat(suite_name, suite);
  strcat(suite_name, "_");
  strcat(suite_name, name);


  //Treatment of dlopen and dlsym
  void *handle;
  testfw_func_t func_test;

  if ( (handle = dlopen (NULL, RTLD_LAZY)) == NULL)
    {
      printf ("Erreur dlopen: %s\n", dlerror ());
      dlclose(handle);
      exit (EXIT_FAILURE);
    }


  else if(( (func_test = dlsym (handle, suite_name)) == NULL))
   {
     printf ("Erreur dlsym: %s\n", dlerror ());
     dlclose (handle);
     exit (EXIT_FAILURE);
   }

  //The function is registered
  testfw_register_func(fw, suite, name, func_test);

  test->suite = suite;
  test->name = name;
  test->func = func_test;
  dlclose (handle);

  return test;
}


int testfw_register_suite(struct testfw_t *fw, char *suite){

  char buf[1000];

  sprintf(buf, "nm --defined-only %s | cut -d '' -f 3 | grep test | cut -d ' ' -f 3 | grep \"^%s_\" | cut -d '_' -f 2", fw->program, suite);

  FILE *f = popen(buf, "r");

  if (f == NULL){
     return EXIT_FAILURE;
  }

  char c[1000];
  while( fgets(c, 1000, f) != NULL){
    char * name = strtok(c, "\n");
    //char * new_name = (char *)malloc(sizeof(char) * (strlen(name) - 1));


      //printf("%s\n", name[strlen(name)]);

    // printf("%s\n", new_name[strlen(name) - 1]);
  //  new_name[strlen(name)] = '\0';
    // printf("%s\n",new_name[strlen(new_name)]);

    testfw_register_symb(fw, suite, name);
  }

  return EXIT_SUCCESS;
}

/* ********** RUN TEST ********** */

int testfw_run_all(struct testfw_t *fw, int argc, char *argv[], enum testfw_mode_t mode)
{
  int fail = 0;


  void hello(int sig){
     printf("[%s] run test %s.%s in x ms (status %d)", strsignal(sig), "test", "hello", sig);
  }
  /*
    sigset_t fullmask;
    sigfillset(&fullmask);
    sigprocmask(SIG_SETMASK, &fullmask, NULL);*/


      if (!fork()){

   for (int i = 0; i < fw->count; i++){
     // char *suite = (testfw_get(fw, i))->suite;
     //char *name = (testfw_get(fw, i))->name;
     if ( ( *(( fw->tab[i] )->func ) )(argc, argv) == EXIT_SUCCESS ){
       kill(getppid(), 0);
       printf("OK\n");
   }
     /*
        printf("I will kill daddy in 3 sec...\n");
    sleep(3);
    kill(getppid(), 9);*/
     return 1;

      }


      }else{
  /*

      sigset_t emptymask;
      sigemptyset(&emptymask);*/

      struct sigaction act_hello;
      act_hello.sa_handler = hello;
      act_hello.sa_flags = 0;
      sigemptyset(&act_hello.sa_mask);

      // sigprocmask(SIG_SETMASK, &emptymask, NULL);

      sigaction(0, &act_hello, NULL);

      wait(NULL);

    }


    return fail;
  }

  /*
  int fail = 0;
  if (fw == NULL){
    return EXIT_SUCCESS;
  }
  for (int i = 0; i < fw->count; i++){

      char * suite = ( fw->tab[i] )->suite;
      char * name = ( fw->tab[i] )->name;



      if ( (*(( fw->tab[i] )->func ))(argc, argv) == EXIT_FAILURE)  {
        fail++;

	    printf("------------------------\n");

	    printf("FAIL of %s.%s\n",suite, name );

      }
     //When the function test returns EXIT_SUCCESS
      else{
	    printf("------------------------\n");

	    printf("SUCCESS of %s.%s\n",suite, name );
      }
    }
  return fail;*/
