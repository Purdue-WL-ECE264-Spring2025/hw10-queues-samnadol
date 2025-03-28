#include "queue.h"
#include "tile_game.h"

#include <stdlib.h>

#define SOLVED_GAME 81985526993846272

struct checked_node
{
    struct game_state state;
    struct checked_node *next;
};

void enqueue(struct queue *q, struct game_state state)
{
    insert_at_tail(&q->data, serialize(state));
}

struct game_state dequeue(struct queue *q)
{
    size_t value = remove_from_head(&q->data);
    return deserialize(value);
}

void print_tile(struct game_state state)
{
    printf("[\n");
    for (int x = 0; x < 4; x++)
    {
        printf("  ");
        for (int y = 0; y < 4; y++)
        {
            printf("%2d ", state.tiles[x][y]);
        }
        printf("\n");
    }
    printf("]\n");
}

int check_win(struct game_state current)
{
    return ((serialize(current) - current.num_steps) == SOLVED_GAME);
}

void add_checked(struct checked_node **ll_head, struct game_state current)
{
    struct checked_node *new = (struct checked_node *)malloc(sizeof(struct checked_node));
    new->state = current;
    new->next = NULL;

    if (*ll_head == NULL)
    {
        *ll_head = new;
    }
    else
    {
        struct checked_node *ll_current = *ll_head;
        while (ll_current->next)
            ll_current = ll_current->next;
        ll_current->next = new;
    }
}

int checked(struct checked_node **ll_head, struct game_state current)
{
    if (*ll_head == NULL)
        return 0;
    
    struct checked_node *ll_current = *ll_head;
    while (ll_current->next)
    {
        if (serialize(ll_current->state) - ll_current->state.num_steps == serialize(current) - current.num_steps)
            return 1;
        ll_current = ll_current->next;
    }
    return 0;
}

void free_checked(struct checked_node **ll_head)
{
    if (*ll_head == 0)
        return;

    struct checked_node *ll_current = *ll_head;
    struct checked_node *ll_prev;

    while (ll_current->next)
    {
        ll_prev = ll_current;
        ll_current = ll_current->next;
        free(ll_prev);
    }
}

int number_of_moves(struct game_state start)
{
    void (*tile_move[4])(struct game_state *state) = {move_down, move_up, move_right, move_left};
    struct checked_node *checked_list = NULL;

    struct queue q;
    q.data.head = NULL;
    enqueue(&q, start);

    while (q.data.head)
    {
        struct game_state current = dequeue(&q);
        if (checked(&checked_list, current))
            continue;

        if (check_win(current))
        {
            printf("SOLVED in %d\n", current.num_steps);
            free_list(q.data);

            free_checked(&checked_list);
            return current.num_steps;
        }
        else
        {
            add_checked(&checked_list, current);
            for (int i = 0; i < 4; i++)
            {
                struct game_state new = current;
                tile_move[i](&new);
                enqueue(&q, new);
            }
        }
    }

    free_list(q.data);
    return 0;
}
