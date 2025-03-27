#include "linked_list.h"

#include <stdlib.h>
#include <stdint.h>

struct list_node *new_node(size_t value)
{
  struct list_node *node = (struct list_node *)malloc(sizeof(struct list_node));
  if (!node)
    return NULL;

  node->value = value;
  node->next = NULL;
  return node;
}

void insert_at_head(struct linked_list *list, size_t value)
{
  struct list_node *node = new_node(value);
  if (!node)
    return;

  node->next = list->head;
  list->head = node;
}

void insert_at_tail(struct linked_list *list, size_t value)
{
  struct list_node *node = new_node(value);
  if (!node)
    return;

  struct list_node *current = list->head;
  if (current)
  {
    while (current->next)
    {
      current = current->next;
    }
    current->next = node;
  }
  else
  {
    list->head = node;
  }
}

size_t remove_from_head(struct linked_list *list)
{
  size_t old_value = list->head->value;

  struct list_node *current = list->head;
  list->head = current->next;
  free(current);

  return old_value;
}

size_t remove_from_tail(struct linked_list *list)
{
  struct list_node *current = list->head;
  struct list_node *newlast;

  while (current->next)
  {
    newlast = current;
    current = current->next;
  }

  size_t old_value = current->value;
  free(current);

  if (list->head == current)
  {
    list->head = NULL;
  }
  else
  {
    newlast->next = NULL;
  }

  return old_value;
}

void free_list(struct linked_list list)
{
  struct list_node *node = list.head;
  struct list_node *next_node = list.head;
  while (node)
  {
    next_node = node->next;
    free(node);
    node = next_node;
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list)
{
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next)
  {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
