#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

int hash_print_key(const struct s_hash *hash,
                   const char          *elt)
{
  if (elt == NULL)
    return (1);
  printf("%d\n", hash->hfunc(elt));
  return (0);
}
