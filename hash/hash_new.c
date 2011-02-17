#include <stdlib.h>
#include "hash.h"

struct s_hash   *hash_new(unsigned int size,
		       	  t_hash_func  hfunc,
			  enum e_hash_mode mode)
{
  struct s_hash *hash;
  unsigned int  i;

  if (NULL == (hash = malloc(sizeof (struct s_hash))))
    return (NULL);
  if (NULL == (hash->slots = malloc(size * sizeof (struct s_hash_slot))))
    {
      free(hash);
      return (NULL);
    }
  hash->hfunc = hfunc;
  hash->mode = mode;
  hash->size = size;
  for (i = 0; i < size; i++)
    {
      hash->slots[i].elt = NULL;
      hash->slots[i].next = NULL;
      hash->slots[i].freed = 0;
    }
  return (hash);
}
