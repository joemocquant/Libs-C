#include <stdlib.h>
#include "hash.h"

static int hash_erase_linked(const struct s_hash *hash,
			     unsigned int        key,
			     const char          *elt,
			     int                 mode);

static void hash_erase_linked_mode_on(const struct s_hash *hash,
				      unsigned int        key,
				      const char          *elt);

static int hash_erase_linear(const struct s_hash *hash,
			     unsigned int        key,
			     const char          *elt,
			     int                 mode);

static int my_strcmp(const char *s1, 
                     const char *s2);

int            hash_erase(struct s_hash *hash,
                          const char    *elt,
                          int           mode)
{
  unsigned int key;

  if (elt == NULL)
    return (1);
  key = hash->hfunc(elt);
  if (key > hash->size)
    return (1);
  if (hash->mode == hm_no_collision)
    {
      if (my_strcmp(hash->slots[key].elt, elt))
	{
	  free(hash->slots[key].elt);
	  hash->slots[key].elt = NULL;
	}
      return (0);
    }
  if (hash->mode == hm_linked)
    return (hash_erase_linked(hash, key, elt, mode));
  if (hash->mode == hm_linear)
    return (hash_erase_linear(hash, key, elt, mode));
  return (1); 
}

static int           hash_erase_linked(const struct s_hash *hash,
				       unsigned int        key,
				       const char          *elt,
				       int                 mode)
{
  struct s_hash_slot *tmp;

  if (my_strcmp(hash->slots[key].elt, elt))
    {
      free(hash->slots[key].elt);
      if (hash->slots[key].next != NULL)
	{
	  hash->slots[key].elt = hash->slots[key].next->elt;
	  tmp = hash->slots[key].next;
	  hash->slots[key].next = hash->slots[key].next->next;
	  free(tmp);
	}
      else
	hash->slots[key].elt = NULL;
    }
  if (mode)
    {
      while (my_strcmp(hash->slots[key].elt, elt))
	hash_erase_linked(hash, key, elt, 0);
      hash_erase_linked_mode_on(hash, key, elt);
    }
  return(0);
}

static void          hash_erase_linked_mode_on(const struct s_hash *hash,
					       unsigned int        key,
					       const char          *elt)
{
  struct s_hash_slot *list;
  struct s_hash_slot *tmp;
  struct s_hash_slot *old;

  old = NULL;
  for (list = hash->slots[key].next; list != NULL; list = list->next)
    if (my_strcmp(list->elt, elt))
      {
	free(list->elt);
	if (old == NULL)
	  {
	    hash->slots[key].next = list->next;
	    free(list);
	    list = &hash->slots[key];
	  }
	else
	  {
	    tmp = list;
	    old->next = list->next;
	    free(tmp);
	  }
      }
    else
      old = list;
}

static int     hash_erase_linear(const struct s_hash *hash,
				 unsigned int        key,
				 const char          *elt,
				 int                 mode)
{
  unsigned int i;
  
  for (i = key; i < hash->size; i++) 
    if ((!hash->slots[i].freed) && (my_strcmp(hash->slots[i].elt, elt)))
      {
	hash->slots[i].freed = 1;
	if (!mode)
	  return (0);
      }
  for (i = 0; i < key; i++)
    if ((!hash->slots[i].freed) && (my_strcmp(hash->slots[i].elt, elt)))
      {
	hash->slots[i].freed = 1;
	if (!mode)
	  return (0);
      }
  return (0);
}

static int my_strcmp(const char *s1,
                     const char *s2)
{
  int      i;
 
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
