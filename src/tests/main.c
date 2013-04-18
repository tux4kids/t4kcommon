#include <stdio.h>
#include <stdlib.h>
#include "CUnit/Basic.h"
#include "test_public_functions.h"



//TODO: export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$HOME/test/CUnit/lib



int main(int argc, char ** argv)
{
  (void) argc;
  (void) argv;
  
  CU_pSuite suite;
  CU_pTest test;
  CU_ErrorCode err;
  
  err = CU_initialize_registry(); 
  if (err != CUE_SUCCESS)
  {
    fprintf(stderr, "CU_initialize_registry: memory allocation failed\n");
    return EXIT_FAILURE;
  }
  
  suite = CU_add_suite("T4K_common test suite", NULL, NULL);
  if (suite == NULL)
  {
    fprintf(stderr, "CU_add_suite: %s\n", CU_get_error_msg());
    return EXIT_FAILURE;
  }
  
  //TODO: maybe, later, use CU_register_suites() and CU_TestInfo structures
  
  test = CU_ADD_TEST(suite, test_T4K_inRect);
  if (test == NULL)
  {
    fprintf(stderr, "CU_add_test: %s\n", CU_get_error_msg());
    return EXIT_FAILURE;
  }
  test = CU_ADD_TEST(suite, test_T4K_CheckFile);
  if (test == NULL)
  {
    fprintf(stderr, "CU_add_test: %s\n", CU_get_error_msg());
    return EXIT_FAILURE;
  }
  test = CU_ADD_TEST(suite, test_T4K_RemoveSlash);
  if (test == NULL)
  {
    fprintf(stderr, "CU_add_test: %s\n", CU_get_error_msg());
    return EXIT_FAILURE;
  }
  
  err = CU_basic_run_suite(suite);
  if (err != CUE_SUCCESS)
  {
    fprintf(stderr, "CU_basic_run_suite: error during the test run: %s\n", CU_get_error_msg());
    return EXIT_FAILURE;
  }
  
  CU_cleanup_registry();
  
  return EXIT_SUCCESS;
}

