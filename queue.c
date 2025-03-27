#include "queue.h"
#include "tile_game.h"

// #include <time.h>

#define SOLVED_GAME 81985526993846272

void enqueue(struct queue *q, struct game_state state) {
    // insert_at_head(&q->data, serialize(state));
    insert_at_tail(&q->data, serialize(state));
}

struct game_state dequeue(struct queue *q) { 
    // size_t value = remove_from_tail(&q->data);
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

    // int num = 1;
    // int sum = 0;
    // for (int x = 0; x < 4; x++)
    // {
    //     for (int y = 0; y < 4; y++)
    //     {
    //         sum += current.tiles[x][y] == num;
   
    //         num++;
    //         if (num > 15)
    //             num = 0;
    //     }
    // }
    // printf("%d\n", sum);

    // return sum == 16;
}

int number_of_moves(struct game_state start) { 
    // time_t begin = clock();

    void (*tile_move[4])(struct game_state *state);
    tile_move[0] = move_up;
    tile_move[1] = move_down;
    tile_move[2] = move_left;
    tile_move[3] = move_right;

    struct queue q;
    q.data.head = NULL;
    enqueue(&q, start);

    // int it = 0;
    while (q.data.head)
    {
        struct game_state current = dequeue(&q);

        // printf("de %zu\n", serialize(current));
        // print_tile(current);

        if (check_win(current))
        {
            printf("SOLVED in %d\n", current.num_steps);
            printf("serialized %zu\n", serialize(current));
            // print_tile(current);

            free_list(q.data);

            // clock_t end = clock();
            // double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
            // printf("took %lfs\n", time_spent);

            return current.num_steps;
        }
        else
        {
            // printf("%d\n", current.num_steps);
            for (int i = 0; i < 4; i++)
            {
                struct game_state new = current;
                tile_move[i](&new);

                enqueue(&q, new);

                // printf("en %zu\n", serialize(new));
                // print_tile(new);

                // print_list(&q.data);
            }
        }


        // if (it++ == 5)
        //     break;
        // printf("NEW ITERATION\n");
    }

    free_list(q.data);
    return 0;
}
