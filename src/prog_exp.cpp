#include <iostream>

using namespace std;

int avl_rotations = 0;
int vp_rotations = 0;

typedef struct int_cell int_cell_t;

typedef struct node_cell node_cell_t;
typedef struct node_queue node_queue_t;

typedef int tree_element_t;
typedef struct node node_t;

enum NodeColor
{
    BLACK_NODE,
    RED_NODE
};
struct node
{
    node_t *left, *right, *parent;
    tree_element_t element;
    NodeColor color;
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

bool isEmpty(node_queue_t *queue)
{
    return queue->size == 0;
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

node_t *dequeue(node_queue_t *queue)
{
    if (isEmpty(queue))
        return NULL;

    node_cell_t *firstCell = queue->first;
    node_t *firstElement = queue->first->element;

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

void enqueue(node_t *element, node_queue_t *queue)
{
    node_cell_t *cell = newNodeCell();

    if (isEmpty(queue))
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
    node->parent = NULL;
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
    if (y->right)
        y->right->parent = x;

    y->right = x;
    y->parent = x->parent;
    x->parent = y;

    return y;
}

node_t *rotateLeft(node_t *x)
{
    node_t *y = x->right;

    x->right = y->left;

    if (y->left)
        y->left->parent = x;

    y->left = x;
    y->parent = x->parent;
    x->parent = y;

    return y;
}

node_t *avl_rebalance(node_t *root)
{
    int leftHeight = height(root->left);
    int rightHeight = height(root->right);
    int balanceFactor = leftHeight - rightHeight;

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

node_t *getUncle(node_t *node)
{
    node_t *parent = node->parent;
    node_t *grandparent = node->parent->parent;

    if (grandparent->left == parent)
        return grandparent->right;

    return grandparent->left;
}

void vp_recolor_node(node_t *node)
{
    if (node->color == BLACK_NODE)
        node->color = RED_NODE;
    else
        node->color = BLACK_NODE;
}

node_t *vp_rebalance(node_t *root, node_t *insertedNode)
{
    node_queue_t *nodeQueue = newQueue();
    enqueue(insertedNode, nodeQueue);

    node_t *z;
    while (!isEmpty(nodeQueue))
    {
        z = dequeue(nodeQueue);

        // CASE 0
        if (!z->parent)
            continue;

        // NO VIOLATION
        if (z->color == BLACK_NODE || z->parent->color == BLACK_NODE || !(z->parent->parent))
            continue;

        node_t *uncle = getUncle(z);

        bool rightTriangle = z->parent->left == z && z->parent->parent->right == z->parent;
        bool leftTriangle = z->parent->right == z && z->parent->parent->left == z->parent;
        bool isTriangle = leftTriangle || rightTriangle;

        // CASE 1
        if (uncle && uncle->color == RED_NODE)
        {
            vp_recolor_node(z->parent);
            vp_recolor_node(uncle);
            vp_recolor_node(z->parent->parent);

            if (z->parent->parent->color == RED_NODE)
                enqueue(z->parent->parent, nodeQueue);
        }
        else
        {
            // CASE 2
            if (isTriangle)
            {
                if (rightTriangle)
                {
                    z->parent->parent->right = rotateRight(z->parent);
                    vp_rotations++;
                    z = z->right;
                }
                else
                {
                    z->parent->parent->left = rotateLeft(z->parent);
                    vp_rotations++;
                    z = z->left;
                }
            }

            // CASE 3
            node_t *originalParent = z->parent, *originalGrandparent = z->parent->parent;
            bool isLeftLine = z->parent->left == z && z->parent->parent->left == z->parent;

            if (isLeftLine)
            {
                if (root == z->parent->parent)
                    root = rotateRight(z->parent->parent);
                else if (z->parent->parent->parent->right == z->parent->parent)
                    z->parent->parent->parent->right = rotateRight(z->parent->parent);
                else
                    z->parent->parent->parent->left = rotateRight(z->parent->parent);

                vp_rotations++;
            }
            else
            {
                if (root == z->parent->parent)
                    root = rotateLeft(z->parent->parent);
                else if (z->parent->parent->parent->right == z->parent->parent)
                    z->parent->parent->parent->right = rotateLeft(z->parent->parent);
                else
                    z->parent->parent->parent->left = rotateLeft(z->parent->parent);
                vp_rotations++;
            }

            vp_recolor_node(originalParent);
            vp_recolor_node(originalGrandparent);
        }
    }

    root->color = BLACK_NODE;

    return root;
}

node_t *vp_insert(tree_element_t element, node_t *root)
{
    if (root == NULL)
    {
        root = newNode(element);
        root->color = BLACK_NODE;
        return root;
    }

    node_t *cursor = root;

    node_t *insertedNode = newNode(element);
    insertedNode->color = RED_NODE;

    while (cursor)
    {
        if (element > cursor->element)
        {
            if (cursor->right)
            {
                cursor = cursor->right;
            }
            else
            {
                cursor->right = insertedNode;
                insertedNode->parent = cursor;
                break;
            }
        }
        else if (element < cursor->element)
        {
            if (cursor->left)
            {
                cursor = cursor->left;
            }
            else
            {
                cursor->left = insertedNode;
                insertedNode->parent = cursor;
                break;
            }
        }
    }

    return vp_rebalance(root, insertedNode);
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

    while (!isEmpty(queue))
    {
        node_t *node = dequeue(queue);

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
    node_t *vp_root = NULL;

    int insertionCount = 0;
    while (cin >> x)
    {
        insertionCount++;
        avl_root = avl_insert(x, avl_root);
        vp_root = vp_insert(x, vp_root);
    }

    printf("DONE: insertionCount: %d\n", insertionCount);
    printf("-----------------\n");
    printf("AVL Tree:\n");
    printf("balance level: %f\n", balanceLevel(avl_root));
    printf("avg rotations: %f\n", (float)avl_rotations / insertionCount);
    printf("-----------------\n");
    printf("Red-Black Tree:\n");
    printf("balance level: %f\n", balanceLevel(vp_root));
    printf("avg rotations: %f\n", (float)vp_rotations / insertionCount);
    printf("-----------------\n");
}
