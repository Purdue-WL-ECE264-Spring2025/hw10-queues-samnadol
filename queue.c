#include "queue.h"
#include "tile_game.h"

#define SOLVED_GAME 81985526993846272

void enqueue(struct queue *q, struct game_state state) {
    insert_at_tail(&q->data, serialize(state));
}

struct game_state dequeue(struct queue *q) {
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

int number_of_moves(struct game_state start) { 
    void (*tile_move[4])(struct game_state *state);
    tile_move[0] = move_down;
    tile_move[1] = move_up;
    tile_move[2] = move_right;
    tile_move[3] = move_left;

    struct queue q;
    q.data.head = NULL;
    enqueue(&q, start);

    while (q.data.head)
    {
        struct game_state current = dequeue(&q);
        if (check_win(current))
        {
            printf("SOLVED in %d\n", current.num_steps);
            free_list(q.data);
            return current.num_steps;
        }
        else
        {
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
