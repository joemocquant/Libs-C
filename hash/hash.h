#ifndef HASH_H
# define HASH_H

enum e_hash_mode
  {
     hm_no_collision,
     hm_linked,
     hm_linear
  };

struct               s_hash_slot
{
  char               *elt;
  struct s_hash_slot *next;
  int                freed;
};

typedef unsigned int (*t_hash_func)(const char *str);

struct               s_hash
{
  t_hash_func        hfunc;
  struct s_hash_slot *slots;
  enum e_hash_mode   mode;
  unsigned int       size;
};

struct s_hash *hash_new(unsigned int size,
			t_hash_func  hfunc,
			enum e_hash_mode mode);
void          hash_delete(struct s_hash *hash);
int           hash_print_key(const struct s_hash *hash, const char *elt);
int           hash_insert(struct s_hash *hash, const char *elt);
void          hash_dump(const struct s_hash *hash);
int           hash_count(const struct s_hash *hash, const char *elt);
int           hash_erase(struct s_hash *hash, const char *elt, int mode);

#endif /* ! HASH_H */
