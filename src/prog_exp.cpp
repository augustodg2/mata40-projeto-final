#include <iostream>

using namespace std;

int avl_rotations = 0;
int vp_rotations = 0;

typedef struct int_cell int_cell_t;
typedef struct int_queue int_queue_t;

typedef struct node_cell node_cell_t;
typedef struct node_queue node_queue_t;

typedef struct int_queue int_queue_t;

struct int_cell
{
    int element;
    int_cell_t *next;
};

struct int_queue
{
    int_cell_t *first, *last;
    unsigned int size;
};

typedef int tree_element_t;
typedef struct node node_t;
struct node
{
    node_t *left, *right;
    tree_element_t element;
    int height;
};

struct node_cell
{
    node_t *element;
    node_cell_t *next;
};

struct node_queue
{
    node_cell_t *first, *last;
    unsigned int size;
};

node_queue_t *newQueue()
{
    node_queue_t *queue = (node_queue_t *)malloc(sizeof(node_queue_t));

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

bool isEmptyIntQueue(int_queue_t *queue)
{
    return queue->size == 0;
}

bool isEmptyNodeQueue(node_queue_t *queue)
{
    return queue->size == 0;
}

int dequeueInt(int_queue_t *queue)
{
    if (isEmptyIntQueue(queue))
        return -1;

    int_cell_t *firstCell = queue->first;
    int firstElement = queue->first->element;

    queue->first = firstCell->next;
    queue->size--;

    if (isEmptyIntQueue(queue))
    {
        queue->first = NULL;
        queue->last = NULL;
    }

    free(firstCell);

    return firstElement;
}

int_cell_t *newIntCell()
{
    int_cell_t *cell = (int_cell_t *)malloc(sizeof(int_cell_t));

    if (!cell)
    {
        perror("Error: Cannot allocate cell");
        exit(-1);
    }

    cell->next = NULL;
    cell->element = -1;

    return cell;
}

node_cell_t *newNodeCell()
{
    node_cell_t *cell = (node_cell_t *)malloc(sizeof(node_cell_t));

    if (!cell)
    {
        perror("Error: Cannot allocate cell");
        exit(-1);
    }

    cell->next = NULL;
    cell->element = NULL;

    return cell;
}

node_t *dequeueNode(node_queue_t *queue)
{
    if (isEmptyNodeQueue(queue))
        return NULL;

    node_cell_t *firstCell = queue->first;
    node_t *firstElement = queue->first->element;

    queue->first = firstCell->next;
    queue->size--;

    if (isEmptyNodeQueue(queue))
    {
        queue->first = NULL;
        queue->last = NULL;
    }

    free(firstCell);

    return firstElement;
}

void enqueue(node_t *element, node_queue_t *queue)
{
    node_cell_t *cell = newNodeCell();

    if (isEmptyNodeQueue(queue))
        queue->first = cell;
    else
        queue->last->next = cell;

    queue->last = cell;
    queue->size++;
    cell->element = element;
}

void enqueue(int element, int_queue_t *queue)
{
    int_cell_t *cell = newIntCell();

    if (isEmptyIntQueue(queue))
        queue->first = cell;
    else
        queue->last->next = cell;

    queue->last = cell;
    queue->size++;
    cell->element = element;
}

node_t *newNode(int element)
{
    node_t *node = (node_t *)malloc(sizeof(node_t));

    if (!node)
    {
        perror("Cannot create new node");
        exit(-1);
    }

    node->left = NULL;
    node->right = NULL;
    node->element = element;

    return node;
}

node_t *bst_insert(tree_element_t element, node_t *root)
{
    if (root == NULL)
        return newNode(element);

    if (root->element > element)
        root->left = bst_insert(element, root->left);
    else if (root->element < element)
        root->right = bst_insert(element, root->right);

    return root;
}

int height(node_t *node)
{
    if (!node)
        return -1;

    return 1 + max(height(node->left), height(node->right));
}

node_t *rotateRight(node_t *x)
{
    node_t *y = x->left;

    x->left = y->right;
    y->right = x;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

node_t *rotateLeft(node_t *x)
{
    node_t *y = x->right;

    x->right = y->left;
    y->left = x;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

node_t *avl_rebalance(node_t *root)
{
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    int balanceFactor = leftHeight - rightHeight;

    root->height = 1 + max(leftHeight, rightHeight);

    if (balanceFactor > 1)
    {
        if (height(root->left->right) > height(root->left->left))
        {
            avl_rotations++;
            root->left = rotateLeft(root->left);
        }

        avl_rotations++;
        root = rotateRight(root);
    }
    else if (balanceFactor < -1)
    {
        if (height(root->right->left) > height(root->right->right))
        {
            avl_rotations++;
            root->right = rotateRight(root->right);
        }

        avl_rotations++;
        root = rotateLeft(root);
    }

    return root;
}

node_t *avl_insert(tree_element_t element, node_t *root)
{
    root = bst_insert(element, root);

    return avl_rebalance(root);
}

int balanceFactor(node_t *node)
{
    return height(node->left) - height(node->right);
}

float balanceLevel(node_t *root)
{
    node_queue_t *queue = newQueue();
    enqueue(root, queue);

    float factorSum = 0;
    int nodeCount = 0;

    while (!isEmptyNodeQueue(queue))
    {
        node_t *node = dequeueNode(queue);

        if (node->left)
            enqueue(node->left, queue);
        if (node->right)
            enqueue(node->right, queue);

        nodeCount++;
        factorSum += abs(balanceFactor(node));
    }

    return factorSum / nodeCount;
}

int main()
{
    int x;

    node_t *avl_root = NULL;

    int insertionCount = 0;
    while (cin >> x)
    {
        insertionCount++;
        avl_root = avl_insert(x, avl_root);
    }

    printf("DONE: insertionCount: %d\n\n", insertionCount);

    printf("AVL Tree:\n");
    printf("balance level: %f\n", balanceLevel(avl_root));
    printf("avg rotations: %f\n", (float)avl_rotations / insertionCount);
}
