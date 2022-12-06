#include <iostream>
#include <fstream>
#include <errno.h>
#include <string>

using namespace std;

typedef struct cell cell_t;
typedef struct queue queue_t;
typedef int element_t;

struct cell
{
    element_t element;
    cell_t *next;
};

struct queue
{
    cell_t *first, *last;
    unsigned int size;
};

cell_t *newCell();
queue_t *newQueue();
void enqueue(element_t, queue_t *);
element_t dequeue(queue_t *);
bool isEmpty(queue *);
void print(queue_t *);

int main()
{
    queue_t *numbersRead = newQueue();

    int x;

    while (cin >> x)
    {
        enqueue(x, numbersRead);
    }

    print(numbersRead);
}

cell_t *newCell()
{
    cell_t *cell = (cell_t *)malloc(sizeof(cell_t));

    if (!cell)
    {
        perror("Error: Cannot allocate cell");
        exit(-1);
    }

    cell->next = NULL;
    cell->element = -1;

    return cell;
}

queue_t *newQueue()
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));

    if (!queue)
    {
        perror("Error: Cannot allocate queue");
        exit(-1);
    }

    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;

    return queue;
}

bool isEmpty(queue_t *queue)
{
    return queue->size == 0;
}

void enqueue(element_t element, queue_t *queue)
{
    cell_t *cell = newCell();

    if (isEmpty(queue))
        queue->first = cell;
    else
        queue->last->next = cell;

    queue->last = cell;
    queue->size++;
    cell->element = element;
}

element_t dequeue(queue_t *queue)
{
    if (isEmpty(queue))
        return -1;

    cell_t *firstCell = queue->first;
    element_t firstElement = queue->first->element;

    queue->first = firstCell->next;
    queue->size--;

    if (isEmpty(queue))
    {
        queue->first = NULL;
        queue->last = NULL;
    }

    free(firstCell);

    return firstElement;
}

void print(queue_t *queue)
{
    if (isEmpty(queue))
        cout << "Empty list" << endl;

    cell_t *cursor = queue->first;

    while (cursor)
    {
        cout << (cursor->element);

        if (cursor->next)
            cout << " -> ";

        cursor = cursor->next;
    }

    cout << endl;
}