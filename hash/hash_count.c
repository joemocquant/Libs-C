#include <stdlib.h>
#include "hash.h"

static int hash_count_no_col(const struct s_hash *hash,
			     unsigned int        key,
			     const char          *elt);

static int hash_count_linked(const struct s_hash *hash,
			     unsigned int        key,
			     const char          *elt);

static int hash_count_linear(const struct s_hash *hash,
			     unsigned int        key,
			     const char          *elt);

static int my_strcmp(const char *s1, const char *s2);

int            hash_count(const struct s_hash *hash,
                          const char          *elt)
{
  unsigned int key;

  if (elt == NULL)
    return (0);
  key = hash->hfunc(elt);
  if (key > hash->size)
    return (0);
  if (hash->mode == hm_no_collision)
    return (hash_count_no_col(hash, key, elt));
  if (hash->mode == hm_linked)
    return (hash_count_linked(hash, key, elt));
  if (hash->mode == hm_linear)
    return (hash_count_linear(hash, key, elt));
  return (0);
}

static int     hash_count_no_col(const struct s_hash *hash,
				 unsigned int        key,
				 const char          *elt)
{  
  if (my_strcmp(hash->slots[key].elt, elt))
    return (1);
  return (0);
}

static int           hash_count_linked(const struct s_hash *hash,
				       unsigned int        key,
				       const char          *elt)
{
  int                count;
  struct s_hash_slot *list;
  
  count = 0;
  if (my_strcmp(hash->slots[key].elt, elt))
    count++;
  for (list = hash->slots[key].next; list != NULL; list = list->next)
    if (my_strcmp(list->elt, elt))
      count++;
  return (count);
}

static int     hash_count_linear(const struct s_hash *hash,
				 unsigned int        key,
				 const char          *elt)
{
  int          count;
  unsigned int i;
  unsigned int j;
 
  for (count = 0, i = key; i < hash->size; i++)
    if ((hash->slots[i].freed == 0) && (my_strcmp(hash->slots[i].elt, elt)))
      count++;
  for (j = 0; j < key; j++)
    if ((hash->slots[j].freed == 0) && (my_strcmp(hash->slots[j].elt, elt)))
      count++;
  return (count);
  
}

static int my_strcmp(const char *s1, const char *s2)
{
  int i;
 
  if ((s1 == NULL) || (s2 == NULL))
    return (0);
  i = 0;
  while ((s1[i] != '\0') && (s2[i] != '\0') && (s1[i] == s2[i]))
    i++;
  if (s1[i] == s2[i])
    return (1);
  else
    return (0);
}
