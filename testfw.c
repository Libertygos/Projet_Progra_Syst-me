
#define _GNU_SOURCE
#define _XOPEN_SOURCE
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
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

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

  pid_t fpid;//variable global
  bool timeout = false;
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
  for (int i = 0; i < fw->count; i ++){
    if ( (testfw_get(fw, i))->name != NULL && testfw_get(fw, i) != NULL){
      free((testfw_get(fw, i))->name);
    }

   if ( (testfw_get(fw, i))->suite != NULL && testfw_get(fw, i) != NULL){
      free((testfw_get(fw, i))->suite);
    }
/*
  if ((testfw_get(fw, i) -> func != NULL)){
    free((testfw_get(fw, i))->func);
  }*/
  if (testfw_get(fw, i) != NULL){
    free(testfw_get(fw, i));
    }
  }
  if (fw == NULL){
    perror("The fw in NULL in testfw_free ");
  }else{
    free(fw);
  }
}

int testfw_length(struct testfw_t *fw)
{
  if (fw->count < 0){
    perror("Negative count in testfw_length");
    exit(EXIT_FAILURE);
  }
    return fw->count;
  }

struct test_t *testfw_get(struct testfw_t *fw, int k)
{
   if (fw->tab[k] == NULL)
   {
     perror("The pointer is NULL in testfw_get");
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


  if (test->suite == NULL){
    perror("Malloc failed in testfw_register_func");
    exit(EXIT_FAILURE);
  }
  if (test->name == NULL){
    perror("Malloc failed in testfw_register_func");
    exit(EXIT_FAILURE);
  }

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


  if (fw == NULL || suite == NULL || name == NULL){
    perror("Invalid structure fw in testfw_register_symb");
    exit(EXIT_FAILURE);
  }

  struct test_t * test = malloc(sizeof(struct test_t));

  if (test == NULL){
    perror("Malloc failed in testfw_register_symb");
    exit(EXIT_FAILURE);
  }

  char *suite_name;

  suite_name = malloc((strlen(suite) + strlen(name) + 1)*sizeof(char));

  if (suite_name == NULL){
    perror("Malloc failed in testfw_register_symb");
    exit(EXIT_FAILURE);
  }

  strcat(suite_name, suite);
  strcat(suite_name, "_");
  strcat(suite_name, name);


  //Treatment of dlopen and dlsym
  void *handle;
  testfw_func_t func_test;

  if ( (handle = dlopen (NULL, RTLD_LAZY)) == NULL)
    {
      //printf ("Erreur dlopen: %s\n", dlerror ());
      dlerror ();
      dlclose(handle);
      exit (EXIT_FAILURE);
    }


  else if(( (func_test = dlsym (handle, suite_name)) == NULL))
   {
     //printf ("Erreur dlsym: %s\n", dlerror ());
     dlerror ();
     dlclose (handle);
     exit (EXIT_FAILURE);
   }
   free(suite_name);
  //The function is registered
   test = testfw_register_func(fw, suite, name, func_test);

  dlclose (handle);

  return test;
}


int testfw_register_suite(struct testfw_t *fw, char *suite){

  char buf[1000];
  int i = 0;//return value wich is the number that increments for each registred test


  if (fw == NULL || suite == NULL){
    perror("Invalid structure fw in testfw_register_symb");
    exit(EXIT_FAILURE);
  }
  sprintf(buf, "nm --defined-only %s | grep %s | cut -d ' ' -f 3 | grep \"^%s_\" | cut -d '_' -f 2", fw->program, suite ,suite);

  FILE *f = popen(buf, "r");

  if (f == NULL){
    perror("popen failed in testfw_register_suite");
     return EXIT_FAILURE;
  }
  char c[1000];
  while( fgets(c, 1000, f) != NULL){
    char * name = strtok(c, "\n");
    testfw_register_symb(fw, suite, name);
    i++;
  }
//  printf("%d\n", i);
  return i;
}

/* ********** RUN TEST ********** */
void myhandler(int sig) {
  timeout = true;
  kill(fpid,9);
}

int testfw_run_all(struct testfw_t *fw, int argc, char *argv[], enum testfw_mode_t mode) //PRENDRE EN COMPTE LES OPTIONS QUI SONT INIT DANS testfw_main.c
{

  int status;
  struct sigaction act;
  act.sa_handler = myhandler;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  struct timeval tv_before;
  struct timeval tv_after;

  int fail = 0;
  if (fw == NULL){
    return EXIT_SUCCESS;
  }

  for (int i = 0; i < fw->count; i++){

    if (close(2) == -1){
      perror("Error with close(2)");
    }
      char * suite = ( fw->tab[i] )->suite;
      char * name = ( fw->tab[i] )->name;

      gettimeofday(&tv_before,NULL);

      if((fpid = fork() )== 0){
        //int fd = open("/tmp/res.log", O_WRONLY | O_CREAT, 0644);
        //int fd_err = open("/tmp/error.log", O_WRONLY | O_CREAT, 0644);
        //dup2(fd, 1); dup2(fd, 2);
        if (close(1) == -1){
          perror("Error with close(1)");
        }
        exit((*(( fw->tab[i] )->func ))(argc, argv));
      }

      sigaction(SIGALRM, &act, NULL);
      alarm(fw->timeout);
      waitpid(fpid, &status, WUNTRACED);
      gettimeofday(&tv_after,NULL);

      if( timeout == true){
        printf("[TIMEOUT] run test \"%s.%s\" in %.2f ms(status 124)\n",suite , name ,(float)(((tv_after.tv_sec)-(tv_before.tv_sec))*1000 +(1.0*((tv_after.tv_usec)-(tv_before.tv_usec))/1000)));
        timeout = false;
        fail++;
      }
      else if(WIFEXITED(status)){//le fils a renvyer une valeur de retour
        if(WEXITSTATUS(status) == EXIT_FAILURE)
        {
          fail++;
          printf("[FAILURE] run test \"%s.%s\" in %.2f ms(status 1)\n",suite , name ,(float)(((tv_after.tv_sec)-(tv_before.tv_sec))*1000 + (1.0*((tv_after.tv_usec)-(tv_before.tv_usec))/1000)));
        }
        else{
          printf("[SUCCESS] run test \"%s.%s\" in %.2f ms(status 0)\n",suite , name ,(float)(((tv_after.tv_sec)-(tv_before.tv_sec))*1000 + (1.0*((tv_after.tv_usec)-(tv_before.tv_usec))/1000)));
            }
      }
      else if(WIFSIGNALED(status))//Le fils est tuee par un signal
      {
        fail++;
        printf("[KILLED] run test \"%s.%s\" in %.2f ms (signal \"%s\")\n",suite , name ,(float)(((tv_after.tv_sec)-(tv_before.tv_sec))*1000 + (1.0*((tv_after.tv_usec)-(tv_before.tv_usec))/1000)),strsignal(WTERMSIG(status)));
      }
      else {
        perror("Status du test non reconnu ligne 255 - testfw.c !");
      }

    }
      return fail;
  }


          /*if ( (*(( fw->tab[i] )->func ))(argc, argv) == EXIT_FAILURE)  {
    =======
      for (int i = 0; i < fw->count; i++){

          char * suite = ( fw->tab[i] )->suite;
          char * name = ( fw->tab[i] )->name;



          if ( (*(( fw->tab[i] )->func ))(argc, argv) == EXIT_FAILURE)  {
    >>>>>>> aca29d32ada8d8265b40578ba9620e6c087d481e
            fail++;

    	    printf("------------------------\n");

    	    printf("FAIL of %s.%s\n",suite, name );

          }
         //When the function test returns EXIT_SUCCESS
          else{
    	    printf("------------------------\n");

    	    printf("SUCCESS of %s.%s\n",suite, name );
    <<<<<<< HEAD
        }*/
