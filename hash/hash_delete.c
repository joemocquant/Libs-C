#include <stdlib.h>
#include "hash.h"

void                 hash_delete(struct s_hash *hash)
{
  unsigned int       i;
  struct s_hash_slot *list;
  struct s_hash_slot *tmp;
  
  for (i = 0; i < hash->size; i++)
    if (hash->slots[i].elt != NULL)
      {
	free(hash->slots[i].elt);	  
	list = hash->slots[i].next;
	while (list != NULL)
	  { 
	    free(list->elt);
	    tmp = list;
	    list = list->next;
	    free(tmp);
	  }
      }
  free(hash->slots);
  free(hash);
}
