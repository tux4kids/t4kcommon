#include <stdio.h>
#include "CUnit/Basic.h"
#include "t4k_common.h"
#include "test_public_functions.h"



void test_T4K_inRect(void)
{
  SDL_Rect rect;
  rect.x = 100;
  rect.y = 100;
  rect.w = 500;
  rect.h = 100;
  
  CU_ASSERT_EQUAL(T4K_inRect(rect,100,100), 1);
  CU_ASSERT_EQUAL(T4K_inRect(rect,350,150), 1);
  CU_ASSERT_EQUAL(T4K_inRect(rect,600,200), 1);
  
  CU_ASSERT_EQUAL(T4K_inRect(rect,99,99), 0);
  CU_ASSERT_EQUAL(T4K_inRect(rect,601,201), 0);
  CU_ASSERT_EQUAL(T4K_inRect(rect,-100,-100), 0);
}



void test_T4K_CheckFile(void)
{
  FILE * f = NULL;
  int ret = EOF;
  
  CU_ASSERT_EQUAL(T4K_CheckFile("unexistant_file"), 0);
  
  f = fopen("temp_file", "a");
  if (f == NULL)
  {
    perror("fopen: ");
    fprintf(stderr, "T4K_CheckFile() test aborted\n");
    return;
  }
  CU_ASSERT_EQUAL(T4K_CheckFile("temp_file"), 1);
  
  ret = fclose(f);
  if (ret == EOF)
  {
    perror("fclose: ");
    ret = remove("temp_file");
    if (ret == -1)
    {
      perror("remove: ");
    }
    return;
  }
  
  ret = remove("temp_file");
  if (ret == -1)
  {
    perror("remove: ");
  }
  return;
}



void test_T4K_RemoveSlash(void)
{
  char winpath[] = "C:\\my\\windows\\path\\";
  char unixpath[] = "/home/my/unix/path/";
  //char * nopath = NULL;
  
  /*
  //TODO: T4K_RemoveSlash should check first if path is not NULL
  //      and then strlen(path) ! but it's the contrary for now
  //      so this test make CUnit crash
  T4K_RemoveSlash(nopath);
  CU_ASSERT_PTR_NULL(nopath);
  */
  
  T4K_RemoveSlash(winpath);
  CU_ASSERT_STRING_EQUAL(winpath, "C:\\my\\windows\\path");
  
  T4K_RemoveSlash(unixpath);
  CU_ASSERT_STRING_EQUAL(unixpath, "/home/my/unix/path");
}
