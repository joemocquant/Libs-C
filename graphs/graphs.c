#include <stdlib.h>
#include <stdio.h>
#include "graphs.h"

static struct s_list_graph *add_list_arc_bis(struct s_list_graph *graph,
					     int                 num_top1,
					     int                 num_top2,
					     int                 value);

static int get_mat_case(struct s_mat_graph *graph,
			int                row,
			int                col);

struct s_list_graph *add_list_arc(struct s_list_graph *graph,
				  int                 num_top1,
				  int                 num_top2,
				  int                 value);

static void depth_first_traversal_mat_rec(struct s_mat_graph *graph,
					  int                *tab,
					  int                top);

static void depth_first_traversal_list_rec(struct s_list_graph *graph,
					   int                  *tab,
					   int                  top);

static void Dijkstra_mat_graph_rec(struct s_mat_graph *graph,
				   int                *points,
				   int                *count,
				   int                *path);

static void Dijkstra_list_graph_rec(struct s_list_graph *graph,
				    int                 *points,
				    int                 *count,
				    int                 *path);

static void Bellman_mat_graph_rec(struct s_mat_graph *graph,
				  int                *points,
				  int                *count,
				  int                *path);

static void Bellman_list_graph_rec(struct s_list_graph *graph,
				   int                *points,
				   int                *count,
				   int                *path);

struct s_mat_graph   *create_mat_graph(int nb_top,
				       int is_directed,
				       int is_value)
{
  struct s_mat_graph *graph;
  int                i;
  int                j;

  if (NULL == (graph = malloc(sizeof (struct s_mat_graph))))
    return (NULL);
  graph->is_directed = is_directed;
  graph->is_value = is_value;
  graph->nb_top = nb_top;
  if (NULL == (graph->mat = malloc(nb_top * sizeof (int*))))
    return (NULL);
  for (i = 0; i < nb_top; i++)
    if (NULL == (graph->mat[i] = malloc(nb_top * sizeof(int))))
      return (NULL);
  if (is_directed > 0)
    for (i = 0; i < nb_top; i++)
      for (j = 0; j < nb_top; j++)
	graph->mat[i][j] = graph->is_value > 0 ? 42 : 0;
  else
    for (i = 0; i < nb_top; i++)
      for (j = i + 1; j < nb_top; j++)
	graph->mat[i][j] = graph->is_value > 0 ? 42 : 0;
  return (graph);
}

struct s_mat_graph *add_mat_arc(struct s_mat_graph *graph,
				int		   num_top1,
				int                num_top2,
				int                value)
{
  int	           tmp;

  if (num_top1 > graph->nb_top || num_top2 > graph->nb_top)
    return (NULL);
  if ((graph->is_directed <= 0) && (num_top1 > num_top2))
    {
      tmp = num_top1;
      num_top1 = num_top2;
      num_top2 = tmp;
    }
  if (graph->is_value > 0)
    graph->mat[num_top1 - 1][num_top2 - 1] = value;
  else
    graph->mat[num_top1 - 1][num_top2 - 1] = 1;
  return (graph);
}

struct s_list_graph   *create_list_graph(int is_directed,
					 int is_value)
{
  struct s_list_graph *list;

  if (NULL == (list = malloc(sizeof (struct s_list_graph))))
    return (NULL);
  list->is_directed = is_directed;
  list->is_value = is_value;
  list->nb_top = 0;
  list->first_top = NULL;
  return (list);
}

struct s_list_graph       *add_list_top(struct s_list_graph *graph,
					int                 num_top)
{
  struct s_list_first_top *tmp;
  struct s_list_first_top *new;

  for (tmp = graph->first_top; tmp != NULL &&
	 tmp->next_top != NULL; tmp = tmp->next_top)
    if (tmp->num_top == num_top)
      return (NULL);
  if (NULL == (new = malloc(sizeof (struct s_list_top))))
    return (NULL);
  new->next_top = NULL;
  new->next_adj = NULL;
  new->num_top = num_top;
  if (tmp == NULL)
    graph->first_top = new;
  else
    tmp->next_top = new;
  graph->nb_top++;
  return (graph);
}

static struct s_list_graph *add_list_arc_bis(struct s_list_graph *graph,
					     int                 num_top1,
					     int                 num_top2,
					     int                 value)
{
  struct s_list_first_top *list_gen;
  struct s_list_top       *list;
  struct s_list_top       *new;

  list_gen = graph->first_top;
  while (list_gen->num_top != num_top1)
    list_gen = list_gen->next_top;
  for (list = list_gen->next_adj; list != NULL
	 && list->next_adj != NULL; list = list->next_adj);
  if (NULL == (new = malloc(sizeof (struct s_list_top))))
    return (NULL);
  new->next_adj = NULL;
  new->num_top = num_top2;
  if (graph->is_value > 0)
    new->value = value;
  else
    new->value = 1;
  if (list == NULL)
    list_gen->next_adj = new;
  else
    list->next_adj = new;
  return (graph);
}

static int get_mat_case(struct s_mat_graph *graph,
			int                row,
			int                col)
{
  if (row >= graph->nb_top || col >= graph->nb_top)
    exit(1);
  if (graph->is_directed)
    return (graph->mat[row][col]);
  else
    {
      if (row < col)
	return (graph->mat[row][col]);
      else
	return (graph->mat[col][row]);
    }
}

struct s_list_graph *add_list_arc(struct s_list_graph *graph,
				  int                 num_top1,
				  int                 num_top2,
				  int                 value)
{
  graph = add_list_arc_bis(graph, num_top1, num_top2, value);
  if (graph->is_directed <= 0)
    graph = add_list_arc_bis(graph, num_top2, num_top1, value);
  return (graph);
}

static struct s_list_top   *get_list_value(struct s_list_graph *graph,
				     	   int                 row,
					   int                 col)
{
  struct s_list_first_top  *cur_top;
  struct s_list_top        *cur;

  cur_top = graph->first_top;
  while (cur_top->num_top != row + 1)
    cur_top = cur_top->next_top;
  cur = cur_top->next_adj;
  while ((cur != NULL) && (cur->num_top != col + 1))
    cur = cur->next_adj;
  return (cur);
}

void  print_mat_graph(struct s_mat_graph *graph)
{
  int i;
  int j;
  int k;

  for (i = 0; i < graph->nb_top; i++)
    {
      if (graph->is_directed > 0)
	printf("%d->{", i + 1);
      else
	printf("%d--{", i + 1);
      for (k = 0, j = 0; j < graph->nb_top; j++)
	{
	  if (graph->is_value > 0)
	    {
	      if (graph->mat[i][j] != 42)
		{
		  if (k == 0)
		    printf("%d@%d", j + 1, get_mat_case(graph, i , j));
		  else
		    printf(";%d@%d", j + 1, get_mat_case(graph, i, j));
		  k++;
		}
	    }
	  else
	    if (graph->mat[i][j] == 1)
	      {
		if (k == 0)
		  printf("%d", j + 1);
		else
		  printf(";%d", j + 1);
		k++;
	      }
	}
      printf("}\n");
    }
}

void                      print_list_graph(struct s_list_graph *graph)
{
  struct s_list_first_top *list;
  struct s_list_top       *cur;
  int                     k;

  for (list = graph->first_top; list != NULL; list = list->next_top)
    {
      if (graph->is_directed > 0)
	printf("%d->{", list->num_top);
      else
	printf("%d--{", list->num_top);
      for (cur = list->next_adj, k = 0; cur != NULL; cur = cur->next_adj)
	{
	  if (graph->is_value > 0)
	    {
	      if (cur->value != 42)
		{
		  if (k == 0)
		    printf("%d@%d", cur->num_top, cur->value);
		  else
		    printf(";%d@%d", cur->num_top, cur->value);
		  k++;
		}
	    }
	  else
	    if (cur->value == 1)
	      {
		if (k == 0)
		  printf("%d", cur->num_top);
		else
		  printf(";%d", cur->num_top);
		k++;
	      }
	}
      printf("}\n");
    }
}

void  width_first_traversal_mat(struct s_mat_graph *graph)
{
  int *tab;
  int *file;
  int i;
  int j;
  int nb;

  tab = malloc(graph->nb_top * sizeof (int));
  for (i = 0; i < graph->nb_top; i++)
    tab[i] = 0;
  file = malloc(graph->nb_top * sizeof (int));
  nb = 0;
  tab[0] = 1;
  file[nb++] = 0;
  while (nb != 0)
    {
      for (i = file[0], j = 0; j < graph->nb_top; j++)
	if ((graph->is_value > 0 && get_mat_case(graph, i, j) != 42)
	    || (graph->is_value <= 0 && get_mat_case(graph, i, j) != 0))
	  if (tab[j] == 0)
	    {
	      tab[j] = 1;
	      file[nb++] = j;
	    }
      printf("%d;", file[0] + 1);
      nb--;
      for (i = 0; i < nb; i++)
	file[i] = file[i + 1];
    }
  printf("\n");
  free(tab);
  free(file);
}

void                width_first_traversal_list(struct s_list_graph *graph)
{
  int               *tab;
  int               *file;
  int               i;
  int               j;
  int		    nb;
  struct s_list_top *cur;

  tab = malloc(graph->nb_top * sizeof (int));
  for (i = 0; i < graph->nb_top; i++)
    tab[i] = 0;
  file = malloc(graph->nb_top * sizeof (int));
  nb = 0;
  tab[0] = 1;
  file[nb++] = 0;

  while (nb != 0)
    {
      for (i = file[0], j = 0; j < graph->nb_top; j++)
	{
	  cur = get_list_value(graph, i, j);
	  if (cur != NULL)
	    if (tab[j] == 0)
	      {
		tab[j] = 1;
		file[nb++] = j;
	      }
	}
      printf("%d;", file[0] + 1);
      nb--;
      for (i = 0; i < nb; i++)
	file[i] = file[i + 1];
    }
  printf("\n");
  free(tab);
  free(file);
}

void  depth_first_traversal_mat(struct s_mat_graph *graph)
{
  int *tab;
  int i;
  int nb;

  tab = malloc(graph->nb_top * sizeof (int));
  for (i = 0; i < graph->nb_top; i++)
    tab[i] = 0;
  nb = 0;
  while (nb != graph->nb_top)
    {
      tab[nb] = 1;
      printf("%d;", nb + 1);
      depth_first_traversal_mat_rec(graph, tab, nb);
      for (nb = 0; nb < graph->nb_top && tab[nb] == 1; nb++);
    }
  printf("\n");
  free(tab);
}

static void depth_first_traversal_mat_rec(struct s_mat_graph *graph,
					  int                *tab,
					  int                top)
{
  int	    j;

  for (j = 0; j < graph->nb_top; j++)
    if ((graph->is_value > 0 && get_mat_case(graph, top ,j) != 42)
	|| (graph->is_value <= 0 && get_mat_case(graph, top, j) != 0))
      if (tab[j] == 0)
	{
	  tab[j] = 1;
	  printf("%d;", j + 1);
	  depth_first_traversal_mat_rec(graph, tab, j);
	}
}

void  depth_first_traversal_list(struct s_list_graph *graph)
{
  int *tab;
  int i;
  int nb;

  tab = malloc(graph->nb_top * sizeof (int));
  for (i = 0; i < graph->nb_top; i++)
    tab[i] = 0;
  nb = 0;
  while (nb != graph->nb_top)
    {
      tab[nb] = 1;
      printf("%d;", nb + 1);
      depth_first_traversal_list_rec(graph, tab, nb);
      for (nb = 0; nb < graph->nb_top && tab[nb] == 1; nb++);
    }
  printf("\n");
  free(tab);
}

static void         depth_first_traversal_list_rec(struct s_list_graph *graph,
						   int                  *tab,
						   int                  top)
{
  int               j;
  struct s_list_top *cur;

  for (j = 0; j < graph->nb_top; j++)
    {
      cur = get_list_value(graph, top, j);
      if (cur != NULL)
	{
	  if (tab[j] == 0)
	    {
	      tab[j] = 1;
	      printf("%d;", j + 1);
	      depth_first_traversal_list_rec(graph, tab, j);
	    }
	}
    }
}

void  Dijkstra_mat_graph(struct s_mat_graph *graph)
{
  int *points;
  int *count;
  int *path;
  int i;

  points = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    points[i] = 1;
  points[0] = 0;
  count = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    count[i] = get_mat_case(graph, 0, i);
  count[0] = 0;
  path = malloc(graph->nb_top * sizeof (int));
  for (i = 0; i < graph->nb_top; i++)
    path[i] = 1;
  for (i = 0; i < graph->nb_top && (points[i] == 0 || count[i] == 42); i++);
  if (i != graph->nb_top)
    Dijkstra_mat_graph_rec(graph, points, count, path);
  else
    {
      for (i = 0; i < graph->nb_top; i++)
	printf("point %d: %d, father: %d\n", i + 1, count[i], path[i]);
    }
  free(points);
  free(count);
  free(path);
}

static void Dijkstra_mat_graph_rec(struct s_mat_graph *graph,
				   int                *points,
				   int                *count,
				   int                *path)
{
  int       i;
  int       min;

  for (i = 1; points[i] == 0; i++);
  min = i;
  for (i = min + 1; i < graph->nb_top; i++)
    if ((points[i] == 1) && (count[i] < count[min]))
      min = i;
  points[min] = 0;
  for (i = 0; i < graph->nb_top; i++)
    if ((graph->is_value > 0 && get_mat_case(graph, min, i) != 42)
	|| (graph->is_value <= 0 && get_mat_case(graph, min, i) != 0))
      if (count[i] > get_mat_case(graph, min ,i) + count[min])
	{
	  path[i] = min + 1;
	  count[i] = get_mat_case(graph, min ,i) + count[min];
	}
  for (i = 0; i < graph->nb_top && (points[i] == 0 || count[i] == 42); i++);
  if (i != graph->nb_top)
    Dijkstra_mat_graph_rec(graph, points, count, path);
  else
    {
      for (i = 0; i < graph->nb_top; i++)
	printf("point %d: %d, father: %d\n", i + 1, count[i], path[i]);
    }
}

void                Dijkstra_list_graph(struct s_list_graph *graph)
{
  int               *points;
  int               *count;
  int               *path;
  int               i;
  struct s_list_top *cur;

  points = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    points[i] = 1;
  points[0] = 0;
  count = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    {
      cur = get_list_value(graph, 0, i);
      if (cur != NULL)
	count[i] = cur->value;
      else
	count[i] = 42;
    }
  count[0] = 0;
  path = malloc(graph->nb_top * sizeof (int));
  for (i = 0; i < graph->nb_top; i++)
    path[i] = 1;
  for (i = 0; i < graph->nb_top && (points[i] == 0 || count[i] == 42); i++);
  if (i != graph->nb_top)
    Dijkstra_list_graph_rec(graph, points, count, path);
  else
    {
      for (i = 0; i < graph->nb_top; i++)
	printf("point %d: %d, father: %d\n", i + 1, count[i], path[i]);
    }
  free(points);
  free(count);
  free(path);
}

static void         Dijkstra_list_graph_rec(struct s_list_graph *graph,
					    int                 *points,
					    int                 *count,
					    int                 *path)
{
  int               i;
  int               min;
  struct s_list_top *cur;

  for (i = 1; points[i] == 0; i++);
  min = i;
  for (i = min + 1; i < graph->nb_top; i++)
    if ((points[i] == 1) && (count[i] < count[min]))
      min = i;
  points[min] = 0;
  for (i = 0; i < graph->nb_top; i++)
    {
      cur = get_list_value(graph, min, i);
      if (cur != NULL)
	if (count[i] > cur->value + count[min])
	  {
	    path[i] = min + 1;
	    count[i] = cur->value + count[min];
	  }
    }
  for (i = 0; i < graph->nb_top && (points[i] == 0 || count[i] == 42); i++);
  if (i != graph->nb_top)
    Dijkstra_list_graph_rec(graph, points, count, path);
  else
    {
      for (i = 0; i < graph->nb_top; i++)
	printf("point %d: %d, father: %d\n", i + 1, count[i], path[i]);
    }
}

void  Bellman_mat_graph(struct s_mat_graph *graph)
{
  int *points;
  int *count;
  int *path;
  int i;

  points = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    points[i] = 1;
  points[0] = 0;
  count = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    count[i] = 42;
  count[0] = 0;
  path = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    path[i] = 0;
  path[0] = 1;
  for (i = 0; i < graph->nb_top && points[i] == 0; i++);
  if (i != graph->nb_top)
    Bellman_mat_graph_rec(graph, points, count, path);
  else
    for (i = 0; i < graph->nb_top; i++)
      printf("point %d: %d, father: %d\n", i + 1, count[i], path[i]);
  free(points);
  free(count);
  free(path);
}

static void Bellman_mat_graph_rec(struct s_mat_graph *graph,
				  int                *points,
				  int                *count,
				  int                *path)
{
  int       i;
  int       j;
  int       find;
  int       min;
  int       tmp1;
  int       tmp2;

  for (find = -1, i = 0; i < graph->nb_top && find == -1; i++)
    if (points[i] == 1)
      {
	for (find = 1, j = 0; j < graph->nb_top && find == 1; j++)
	  if (get_mat_case(graph, j, i) != 42)
	    {
	      if (points[j] == 1)
		find = -1;
	      else
		min = j;
	    }
      }
  if (find != -1)
    {
      i--;
      for (j = 0; j < graph->nb_top; j++)
	if (points[j] == 0)
	  if ((graph->is_value > 0 && get_mat_case(graph, j, i) != 42)
	      || (graph->is_value <= 0 && get_mat_case(graph, j, i) != 0))
	    {
	      tmp1 = get_mat_case(graph, min, i) + count[min];
	      tmp2 = get_mat_case(graph, j, i) + count[j];
	    if (tmp1 > tmp2)
	      min = j;
	    }
      points[i] = 0;
      path[i] = min + 1;
      count[i] = get_mat_case(graph, min, i) + count[min];
      Bellman_mat_graph_rec(graph, points, count, path);
    }
  else
    for (i = 0; i < graph->nb_top; i++)
      printf("point %d: %d, father: %d\n", i + 1, count[i], path[i]);
}

void  Bellman_list_graph(struct s_list_graph *graph)
{
  int *points;
  int *count;
  int *path;
  int i;

  points = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    points[i] = 1;
  points[0] = 0;
  count = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    count[i] = 42;
  count[0] = 0;
  path = malloc(graph->nb_top * sizeof (int));
  for (i = 1; i < graph->nb_top; i++)
    path[i] = 0;
  path[0] = 1;
  for (i = 0; i < graph->nb_top && points[i] == 0; i++);
  if (i != graph->nb_top)
    Bellman_list_graph_rec(graph, points, count, path);
  else
    for (i = 0; i < graph->nb_top; i++)
      printf("point %d: %d, father: %d\n", i + 1, count[i], path[i]);
  free(points);
  free(count);
  free(path);
}

static void         Bellman_list_graph_rec(struct s_list_graph *graph,
					   int                 *points,
					   int                 *count,
					   int                 *path)
{
  int               i;
  int               j;
  int               find;
  int               min;
  int               tmp1;
  int               tmp2;
  struct s_list_top *cur;

  for (find = -1, i = 0; i < graph->nb_top && find == -1; i++)
    if (points[i] == 1)
      {
	for (find = 1, j = 0; j < graph->nb_top && find == 1; j++)
	  {
	    cur = get_list_value(graph, j, i);
	    if (cur != NULL)
	      {
		if (points[j] == 1)
		  find = -1;
		else
		  min = j;
	      }
	  }
      }
  if (find != -1)
    {
      i--;
      for (j = 0; j < graph->nb_top; j++)
	if (points[j] == 0)
	  {
	    cur = get_list_value(graph, j, i);
	    if (cur != NULL)
	      {
		tmp2 = cur->value + count[j];
		cur = get_list_value(graph, min, i);
		tmp1 = cur->value + count[min];
		if (tmp1 > tmp2)
		  min = j;
	      }
	  }
      points[i] = 0;
      path[i] = min + 1;
      count[i] = cur->value + count[min];
      Bellman_list_graph_rec(graph, points, count, path);
    }
  else
    for (i = 0; i < graph->nb_top; i++)
      printf("point %d: %d, father: %d\n", i + 1, count[i], path[i]);
}
