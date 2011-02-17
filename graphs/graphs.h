#ifndef GRAPHS_H
# define GRAPHS_H

struct s_mat_graph
{
  int  is_directed; /* Booleen : graphe orient´e ? */
  int  is_value;    /* Booleen : graphe valu´e ? */
  int  nb_top;      /* Nombre de sommets */
  int  **mat;       /* Matrice des arcs du graphe */
};

struct		    s_list_top
{
  struct s_list_top *next_adj; /* Prochain sommet adjacent */
  int               num_top;   /* Num´ero du sommet */
  int               value;     /* Valeur de l’arc */
};

struct			  s_list_first_top
{
  struct s_list_first_top *next_top; /* Prochain sommet de la liste */
  struct s_list_top       *next_adj; /* Prochain sommet adjacent */
  int                     num_top;   /* Num´ero du sommet */
};

struct			  s_list_graph
{
  int			  is_directed; /* Booleen : graphe orient´e ? */
  int			  is_value;    /* Booleen : graphe valu´e ? */
  int			  nb_top;      /* Nombre de sommets */
  struct s_list_first_top *first_top;  /* Premier somment de la liste */
};

struct s_mat_graph  *create_mat_graph(int nb_top,
				      int is_directed,
				      int is_value);

struct s_mat_graph  *add_mat_arc(struct s_mat_graph *graph,
				 int		    num_top1,
				 int                num_top2,
				 int                value);

struct s_list_graph *create_list_graph(int is_directed,
				       int is_value);

struct s_list_graph *add_list_top(struct s_list_graph *graph,
				  int                 num_top);

struct s_list_graph *add_list_arc(struct s_list_graph *graph,
				  int                 num_top1,
				  int                 num_top2,
				  int                 value);

void		    print_mat_graph(struct s_mat_graph *graph);
void                print_list_graph(struct s_list_graph *graph);
void		    width_first_traversal_mat(struct s_mat_graph *graph);
void		    width_first_traversal_list(struct s_list_graph *graph);
void		    depth_first_traversal_mat(struct s_mat_graph *graph);
void		    depth_first_traversal_list(struct s_list_graph *graph);
void		    Dijkstra_mat_graph(struct s_mat_graph *graph);
void		    Dijkstra_list_graph(struct s_list_graph *graph);
void		    Bellman_mat_graph(struct s_mat_graph *graph);
void		    Bellman_list_graph(struct s_list_graph *graph);

#endif /* ! GRAPHS_H */
