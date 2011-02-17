#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

static unsigned int hash_function(const char *str)
{
  unsigned int      i;

  for (i = 0; str[i] != '\0'; i++);
  return (i);
}



int             main(void)
{
  struct s_hash *hash;
  t_hash_func   hfunc;
  
  hfunc = hash_function;
  hash = hash_new(6, hfunc, hm_linear);
  
  hash_insert(hash, "CC");
  hash_insert(hash, "CC");
  hash_delete(hash);
  /*hash_dump(hash);*/

  return (0);
}

