
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
<<<<<<< HEAD
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>
=======
 #include <sys/wait.h>
>>>>>>> aca29d32ada8d8265b40578ba9620e6c087d481e

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
void myhandler(int sig) {
  printf("timeout avant = %d\n",timeout );
  timeout = true;
  printf("timeout apres = %d\n",timeout );
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

  clock_t clock_timer_before, clock_timer_after;
  clock_t time_difference;


  //for(int sig = 1 ; sig < 32 ; sig++) {
  //
  //}

  int fail = 0;
  if (fw == NULL){
    return EXIT_SUCCESS;
  }
<<<<<<< HEAD

  for (int i = 0; i < fw->count; i++){


      char * suite = ( fw->tab[i] )->suite;
      char * name = ( fw->tab[i] )->name;

      int p[2];
      int e = pipe(p);
      if (e !=0) {
        printf("ERREUR lors de la creation d'un pipe\n" );
        exit(EXIT_FAILURE);
      }

      gettimeofday(&tv_before,NULL);

      if((fpid = fork() )== 0){
        close(p[0]);
        int value = (*(( fw->tab[i] )->func ))(argc, argv);
        write(p[1],&value,sizeof(int));

        if(value== EXIT_SUCCESS){
          return EXIT_SUCCESS;
        }else{
          return EXIT_FAILURE;
        }
      }
      sigaction(SIGALRM, &act, NULL);
      alarm(4);
      close(p[1]);
      //alarm(2);

      waitpid(fpid, &status, WUNTRACED);
      gettimeofday(&tv_after,NULL);

      int valuepere;
      read(p[0],&valuepere,sizeof(int));


      printf("WTERMSIG : %d \n", WIFSIGNALED(status));
      printf("timeout = %d\n",timeout );
      if( timeout == true){
        printf("[TIMEOUT] run test \"%s.%s\" in %ld s %ld ms(status %d)\n",suite , name ,(tv_after.tv_sec)-(tv_before.tv_sec), (tv_after.tv_usec)-(tv_before.tv_usec),status);
        timeout = false;
      }
      else if(WIFEXITED(status)){//le fils a renvyer une valeur de retour
        if(valuepere == 1)
        {
          printf("[FAILURE] run test \"%s.%s\" in %ld s %ld ms (status %d)\n", suite, name,(tv_after.tv_sec)-(tv_before.tv_sec), (tv_after.tv_usec)-(tv_before.tv_usec),status );
        }
        else{
          printf("[SUCCESS] run test \"%s.%s\" in %ld s %ld ms(status %d)\n",suite , name ,(tv_after.tv_sec)-(tv_before.tv_sec), (tv_after.tv_usec)-(tv_before.tv_usec),status);
            }
      }
      else if(WIFSIGNALED(status))//Le fils est tuee par un signal
      {

        printf("WTERMSIG = : %d\n", WTERMSIG(status));
        switch(WTERMSIG(status)){ //ne pas oublier les 29 autres cas... SAUF QUE Y'A UNE FONCTION A UTILIS2 POUR PAS AVOIR A LE FAIRE!!!
          case 6 :
            printf("[KILLED] run test \"%s.%s\" in %ld s %ld ms (signal \"Aborted\")\n",suite , name ,(tv_after.tv_sec)-(tv_before.tv_sec), (tv_after.tv_usec)-(tv_before.tv_usec));
            break;
          case 11 :
            printf("[KILLED] run test \"%s.%s\" in %ld s %ld ms (signal \"Segmentation fault\")\n",suite , name ,(tv_after.tv_sec)-(tv_before.tv_sec), (tv_after.tv_usec)-(tv_before.tv_usec));
          break;
          case 14 :
            printf("[KILLED] run test \"%s.%s\" in %ld s %ld ms (signal \"Alarm clock\")\n",suite , name ,(tv_after.tv_sec)-(tv_before.tv_sec), (tv_after.tv_usec)-(tv_before.tv_usec));
          break;
        }
      }
      else {
        perror("Status du test non reconnu ligne 255 - testfw.c !");
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
    }
    return fail;
  }
=======
      }
    }
  return fail;
}
>>>>>>> aca29d32ada8d8265b40578ba9620e6c087d481e
