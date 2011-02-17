#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

void                 hash_dump(const struct s_hash *hash)
{
  unsigned int       i;
  struct s_hash_slot *list;
  
  for (i = 0; i < hash->size; i++)
    {
      if ((hash->slots[i].elt != NULL) && (hash->slots[i].freed == 0))
	{
	  printf("%s", hash->slots[i].elt);
	  for (list = hash->slots[i].next; list != NULL; list = list->next)
	    printf(" %s", list->elt);
	  printf("\n");
	}
    }
}
