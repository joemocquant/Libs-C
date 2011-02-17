#include <stdlib.h>
#include "hash.h"

static char *my_strcpy(const char *src);

static int hash_insert_elt_no_col(struct s_hash *hash,
				  unsigned int  key,
				  char          *elt);

static int hash_insert_elt_linked(struct s_hash *hash,
				  unsigned int key,
				  char         *elt);

static int hash_insert_elt_linear(struct s_hash *hash,
				  unsigned int  key,
				  char          *elt);

int            hash_insert(struct s_hash *hash,
                           const char    *elt)
{
  unsigned int key;
  char         *real_elt;
  
  real_elt = my_strcpy(elt);
  if ((hash == NULL) || (real_elt == NULL))
    return (1);
  key = hash->hfunc(elt);
  if (key >= hash->size)
    return (1);
  if (hash->mode == hm_no_collision)
    return (hash_insert_elt_no_col(hash, key, real_elt));
  if (hash->mode == hm_linked)
    return (hash_insert_elt_linked(hash, key, real_elt));
  if (hash->mode == hm_linear)
    return (hash_insert_elt_linear(hash, key, real_elt));
  return (1);
}

static char *my_strcpy(const char *src)
{
  char      *res;
  int       i;
  int       j;

  if (src == NULL)
    return (NULL);
  for (i = 0; src[i] != '\0'; i++);
  if (NULL == (res = malloc((i + 1) * sizeof (char))))
    return (NULL);
  for (j = 0; j < i; j++)
    res[j] = src[j];
  res[j] = '\0';
  return (res);
}

static int hash_insert_elt_no_col(struct s_hash *hash,
				  unsigned int  key, 
				  char          *elt)
{
  if (hash->slots[key].elt == NULL)
    {
      hash->slots[key].elt = elt;
      return (0);
    }
  return (1);
}
 
static int           hash_insert_elt_linked(struct s_hash *hash, 
					    unsigned int  key, 
					    char          *elt)
{
  struct s_hash_slot *new;

  if (hash->slots[key].elt == NULL)
    hash->slots[key].elt = elt;
  else
    {
      if (NULL == (new = malloc(sizeof (struct s_hash_slot))))
	return (1);
      new->elt = elt;
      new->next = hash->slots[key].next;
      hash->slots[key].next = new;
    }
  return (0);
}

static int     hash_insert_elt_linear(struct s_hash *hash, 
				      unsigned int  key, 
				      char          *elt)
{
  unsigned int i;
  unsigned int j;
  
  for (i = key; i < hash->size && hash->slots[i].elt != NULL
	 && hash->slots[i].freed == 0; i++);
  if (i != hash->size)
    {
      if (hash->slots[i].freed == 1)
	free(hash->slots[i].elt);
      hash->slots[i].elt = elt;
      hash->slots[i].freed = 0;
      return (0);
    }
  for (j = 0; j < key && hash->slots[j].elt != NULL; j++);
  if (j != key)
    {
      if (hash->slots[j].freed == 1)
	free(hash->slots[i].elt);
      hash->slots[j].elt = elt;
      hash->slots[j].freed = 0;
      return (0);
    } 
  return (1);
}
