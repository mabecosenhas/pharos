/****************************************************************************
 * Pharos - A Real-Time Secure Operating System                             *
 * Copyright 2020 Pedro Macara and Filipe Monteiro                          *
 *                                                                          *
 * Licensed under the Apache License, Version 2.0 (the "License");          *
 * you may not use this file except in compliance with the License.         *
 * You may obtain a copy of the License at                                  *
 *                                                                          *
 *     http://www.apache.org/licenses/LICENSE-2.0                           *
 *                                                                          *
 * Unless required by applicable law or agreed to in writing, software      *
 * distributed under the License is distributed on an "AS IS" BASIS,        *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. *
 * See the License for the specific language governing permissions and      *
 * limitations under the License.                                           *
 *                                                                          *
 ****************************************************************************/


#include <pharos/kernel/collection/treemap.h>
#include <pharos/hal/hal.h>
#include <pharos/kernel/assert/assert.h>


/**
 * height of a node
 */
static uint8_t height(ptrTreeNode node);


/**
 * rotate a node to the right
 */
static ptrTreeNode rotateright(ptrTreeNode node);


/**
 * rotate a node to the left
 */
static ptrTreeNode rotateleft(ptrTreeNode node);
static ptrTreeNode RR(ptrTreeNode node);
static ptrTreeNode LL(ptrTreeNode node);
static ptrTreeNode LR(ptrTreeNode node);
static ptrTreeNode RL(ptrTreeNode node);

/**
 * determine the height difference of the right and left part of the node
 */
static int8_t balanceFactor(ptrTreeNode node);

/**
 * insert a node into the tree
 */
static ptrTreeNode treeInsert(ptrTreeMap tree , ptrTreeNode node , const void *key , ptrTreeNode object);

/**
 * remove a node from the tree
 */
static ptrTreeNode treeRemove(ptrTreeMap tree , ptrTreeNode node , const void *key , ptrTreeNode *removed);

/**
 * convert an object to a tree node
 */
INLINE ptrTreeNode castToNode(ptrTreeMap tree , void *object);

/**
 * convert a node to an object
 */
INLINE void *castToObject(ptrTreeMap tree , ptrTreeNode node);

static ptrTreeNode treeMapGetLowerOrEqualNode(const ptrTreeMap tree , const void *key);


void treeMapInitialize(const ptrTreeMap tree , ComparatorMethod comparatorMethod , int16_t offset)
{
    /* there is no root at the beginning */
    tree->root = NULL;

    /* set the comparator method */
    tree->comparatorMethod = comparatorMethod;

    /* set the initial size (0) */
    tree->size = 0U;

    /* set the offset */
    tree->offset = offset;
}


INLINE void *castToObject(ptrTreeMap tree , ptrTreeNode node)
{
    /* cast to a byte address so we can add to the address the offset location */
    uint8_t *address = (uint8_t *) node;


    /* subtract the offset */
    address -= tree->offset;

    /* return the address of the object */
    return (void *) address;
}


INLINE ptrTreeNode castToNode(ptrTreeMap tree , void *object)
{
    /* cast to a byte address so we can add to the address the offset location */
    uint8_t *address = (uint8_t *) object;


    /* add the offset */
    address += tree->offset;

    /* return the address of the node */
    return (ptrTreeNode) (void *) address;
}


void treeMapPut(ptrTreeMap tree , const void *key , void *object)
{
    /* if the tree is empty */
    if(treeMapIsEmpty(tree) == TRUE)
    {
        /* special case when the tree is empty */
        tree->size = 1U;

        /* set the root */
        tree->root = castToNode(tree , object);

        /* no nodes to the right of the root */
        tree->root->right = NULL;

        /* no nodes to the left of the root */
        tree->root->left = NULL;

        /* set the key of the root */
        tree->root->key = key;

        /* height is zero */
        tree->root->treeHeight = 0U;
    }
        /* tree not empty */
    else
    {
        /* insert and update the root */
        tree->root = treeInsert(tree , tree->root , key , castToNode(tree , object));

        /* increment the size of the number of nodes on the tree */
        tree->size++;
    }
}


ptrTreeNode treeInsert(ptrTreeMap tree , ptrTreeNode node , const void *key , ptrTreeNode object)
{
    /* comparison of keys */
    intptr_t comparasion;


    /* if there is no node where to insert (leaf of the tree) */
    if(node == NULL)
    {
        /* object is the node */
        node = object;

        /* update the left to NULL */
        node->left = NULL;

        /* update the right to NULL */
        node->right = NULL;

        /* update the height to 0 */
        node->treeHeight = 0U;

        /* set the key */
        node->key = key;

        /* return the node */
        return node;
    }

    /* compare both keys */
    comparasion = tree->comparatorMethod(key , node->key);

    /* if the object key is greater than the node key */
    if(comparasion > 0)
    {
        /* insert to "right" size of the node */
        node->right = treeInsert(tree , node->right , key , object);

        /* check if rotation are required */
        if(balanceFactor(node) == -2)
        {
            /* balance is required since there is one side of the node which has a greater height by 2 than the other */
            comparasion = tree->comparatorMethod(key , node->right->key);

            /* check which rotation is required to re-balance the tree */
            if(comparasion > 0)
            {
                /* update the head node of this sub-tree */
                node = RR(node);
            }
            else
            {
                /* update the head node of this sub-tree */
                node = RL(node);
            }
        }
    }
        /* if key is smaller */
    else if(comparasion < 0)
    {
        /* insert to the left side */
        node->left = treeInsert(tree , node->left , key , object);

        /* check if rotation are required */
        if(balanceFactor(node) == 2)
        {
            /* balance is required since there is one side of the node which has a greater height by 2 than the other */
            comparasion = tree->comparatorMethod(key , node->left->key);

            /* check which rotation is required to re-balance the tree */
            if(comparasion < 0)
            {
                /* update the head node of this sub-tree */
                node = LL(node);
            }
            else
            {
                /* update the head node of this sub-tree */
                node = LR(node);
            }
        }
    }
        /* if keys are equal */
    else
    {
        /* else, keys are equal -> object is the node */
        node = object;

        /* set the key */
        node->key = key;

        /* set the right node */
        object->right = node->right;

        /* set the left node */
        object->left = node->left;

        /* set the height */
        object->treeHeight = node->treeHeight;

        /* return the object added */
        return object;
    }

    /* update the height of the node */
    node->treeHeight = height(node);

    /* return head node of the sub-tree */
    return node;
}


void *treeMapRemoveLowerOrEqual(ptrTreeMap tree , const void *key)
{
    /* result of the method */
    void *result;

    /* removed node (initialize a NULL for none removed) */
    ptrTreeNode lower = treeMapGetLowerOrEqualNode(tree , key);


    /* if there is a lower node */
    if(lower != NULL)
    {
        /* then remove it */
        result = treeMapRemove(tree , lower->key);
    }
    else
    {
        /* no node was removed */
        result = NULL;
    }

    /* return the node removed (or NULL if none) */
    return result;
}


void * treeMapGetLowerOrEqual(const ptrTreeMap tree , const void *key)
{
    /* return value of this method */
    void *result;

    /* try to find a node with a lower than or equal to key */
    ptrTreeNode node = treeMapGetLowerOrEqualNode(tree , key);


    /* if there is lower node */
    if(node != NULL)
    {
        /* then cast it to the object*/
        result = castToObject(tree , node);
    }
    else
    {
        /* return NULL */
        result = NULL;
    }

    /* return the object address or NULL */
    return result;
}


ptrTreeNode treeMapGetLowerOrEqualNode(const ptrTreeMap tree , const void *key)
{
    /* result of the method */
    ptrTreeNode result = NULL;

    /* iterator to search in the map */
    ptrTreeNode iterator = tree->root;

    /* comparison of the keys */
    intptr_t comparasion;


    /* search the map */
    while(iterator != NULL)
    {
        /* compare keys */
        comparasion = tree->comparatorMethod(iterator->key , key);

        /* if greater */
        if(comparasion > 0)
        {
            /* continue to search to the left */
            iterator = iterator->left;
        }
        else
        {
            /* found the lower */
            result = iterator;

            /* stop searching */
            break;
        }
    }

    /* could not find value */
    return result;
}


void *treeMapGet(const ptrTreeMap tree , const void *key)
{
    /* start at the root node */
    ptrTreeNode iterator = tree->root;

    /* comparison of the keys */
    intptr_t comparasion;


    /* check that the tree pointer is OK */
    PHAROS_ASSERT(tree != NULL , "Tree is invalid");

    /* check that the comparator method is OK */
    PHAROS_ASSERT(tree->comparatorMethod != NULL , "Tree comparator method is invalid");

    /* while we are searching down the tree */
    while(iterator != NULL)
    {
        /* compare our iterator node key */
        comparasion = tree->comparatorMethod(iterator->key , key);

        /* if the iterator key is greater */
        if(comparasion > 0)
        {
            /* go to left */
            iterator = iterator->left;
        }
            /* if the iterator key is smaller */
        else if(comparasion < 0)
        {
            /* go to the right */
            iterator = iterator->right;
        }
            /* if the keys are equal */
        else
        {
            /* found our object with the same value */
            iterator = castToObject(tree , iterator);

            /* leave the search cycle */
            break;
        }
    }

    /* could not find value */
    return iterator;
}


void *treeMapGetFirst(ptrTreeMap map)
{
    /* result of the function */
    void *result;

    /* start with the root */
    ptrTreeNode iterator = map->root;


    /* if there is a root */
    if(iterator != NULL)
    {
        /* loop always to the left until no node is found */
        while(iterator->left != NULL)
        {
            /* go left */
            iterator = iterator->left;
        }

        /* cast to the object */
        result = castToObject(map , iterator);
    }
    else
    {
        /* there is no first (no nodes on the tree) */
        result = NULL;
    }

    /* return the iterator */
    return result;
}


void *treeMapGetLast(ptrTreeMap map)
{
    /* result of the function */
    void *result;

    /* start with the root */
    ptrTreeNode iterator = map->root;


    /* if there is a root */
    if(iterator != NULL)
    {
        /* loop always to the right until no node is found */
        while(iterator->right != NULL)
        {
            /* go right */
            iterator = iterator->right;
        }

        /* cast to the object */
        result = castToObject(map , iterator);
    }
    else
    {
        /* there is no first (no nodes on the tree) */
        result = NULL;
    }

    /* return the iterator */
    return result;
}


void *treeMapGetNext(ptrTreeMap tree , void *previous)
{
    /* result of the method */
    void *result;

    /* iterator on the map */
    ptrTreeNode iterator = tree->root;

    /* previous node */
    ptrTreeNode previousNode = castToNode(tree , previous);

    /* last know next */
    ptrTreeNode lastKnown = NULL;

    /* comparison of the keys */
    intptr_t comparasion;


    /* search the map */
    while(iterator != NULL)
    {
        /* compare the keys */
        comparasion = tree->comparatorMethod(iterator->key , previousNode->key);

        /* if greater */
        if(comparasion > 0)
        {
            /* update the last know next node */
            lastKnown = iterator;

            /* continue to search to the left */
            iterator = iterator->left;
        }
        else
        {
            /* continue to search to the right */
            iterator = iterator->right;
        }
    }

    /* if there is no next node */
    if(lastKnown == NULL)
    {
        /* then return NULL */
        result = NULL;
    }
        /* if there is a next node */
    else
    {
        /* cast it to the object */
        result = castToObject(tree , lastKnown);
    }

    /* return the next object (or NULL if none) */
    return result;
}


void *treeMapGetPrevious(ptrTreeMap tree , void *next)
{
    /* result of the method */
    void *result;

    /* iterator on the map */
    ptrTreeNode iterator = tree->root;

    /* previous node */
    ptrTreeNode previousNode = castToNode(tree , next);

    /* last know next */
    ptrTreeNode lastKnown = NULL;

    /* comparison of the keys */
    intptr_t comparasion;


    /* search the map */
    while(iterator != NULL)
    {
        /* compare the keys */
        comparasion = tree->comparatorMethod(iterator->key , previousNode->key);

        /* if smaller */
        if(comparasion < 0)
        {
            /* update the last know next node */
            lastKnown = iterator;

            /* continue to search to the left */
            iterator = iterator->left;
        }
        else
        {
            /* continue to search to the right */
            iterator = iterator->right;
        }
    }

    /* if there is no next node */
    if(lastKnown == NULL)
    {
        /* then return NULL */
        result = NULL;
    }
        /* if there is a next node */
    else
    {
        /* cast it to the object */
        result = castToObject(tree , lastKnown);
    }

    /* return the next object (or NULL if none) */
    return result;
}


void *treeMapRemoveFirst(ptrTreeMap tree)
{
    /* first object on the map */
    void *first;

    /* cast the object to the node */
    ptrTreeNode node;


    /* check that the tree pointer is OK */
    PHAROS_ASSERT(tree != NULL , "Tree is invalid");

    /* check that the comparator method is OK */
    PHAROS_ASSERT(tree->comparatorMethod != NULL , "Tree comparator method is invalid");

    /* get the first object on the map */
    first = treeMapGetFirst(tree);

    /* cast the object to the node */
    node = castToNode(tree , first);

    /* remove the first node */
    treeMapRemove(tree , node->key);

    /* return the first object */
    return first;
}


void *treeMapRemove(ptrTreeMap tree , const void *key)
{
    /* removed node (initialize a NULL for none removed) */
    ptrTreeNode removed = NULL;


    /* check that the tree pointer is OK */
    PHAROS_ASSERT(tree != NULL , "Tree is invalid");

    /* check that the comparator method is OK */
    PHAROS_ASSERT(tree->comparatorMethod != NULL , "Tree comparator method is invalid");

    /* does the tree have any element */
    if(tree->root == NULL)
    {
        /* no node removed from an empty tree */
        return NULL;
    }

    /* remove the node and calculate new root for the tree */
    tree->root = treeRemove(tree , tree->root , key , &removed);

    /* if there was any node removed */
    if(removed != NULL)
    {
        /* then decrease the number of nodes */
        tree->size--;

        /* return the node removed */
        return castToObject(tree , removed);
    }

    /* no node was removed */
    return NULL;
}


ptrTreeNode treeRemove(ptrTreeMap tree , ptrTreeNode node , const void *key , ptrTreeNode *removed)
{
    /* iterator node */
    ptrTreeNode p;

    /* comparison of keys */
    intptr_t comparasion;


    /* if the node does not exist */
    if(node == NULL)
    {
        /* then cannot remove the node */
        return NULL;
    }

    /* compare the keys */
    comparasion = tree->comparatorMethod(key , node->key);

    /* if greater */
    if(comparasion > 0)
    {
        /* remove from the node to the right */
        node->right = treeRemove(tree , node->right , key , removed);

        /* check if needs re-balancing */
        if(balanceFactor(node) == 2)
        {
            /* if it is the left balance too great */
            if(balanceFactor(node->left) >= 0)
            {
                /* rotate to left to re-balance the tree */
                node = LL(node);
            }
            else
            {
                /* rotate to right to re-balance the tree */
                node = LR(node);
            }
        }
    }
        /* if the key is smaller */
    else if(comparasion < 0)
    {
        /* remove from the node to the left */
        node->left = treeRemove(tree , node->left , key , removed);

        /* check if needs re-balancing */
        if(balanceFactor(node) == -2)
        {
            /* if the right is off-balance */
            if(balanceFactor(node->right) <= 0)
            {
                /* rotate to right to re-balance the tree */
                node = RR(node);
            }
            else
            {
                /* rotate to left to re-balance the tree */
                node = RL(node);
            }
        }
    }
        /* if keys are equal */
    else
    {
        /* data to be deleted is found */
        *removed = node;

        if(node->right != NULL)
        {
            ptrTreeNode aux = node;
            ptrTreeNode removedAux;

            /* delete its in order successor */
            p = node->right;

            /* find the smallest node that is larger than the one to remove */
            while(p->left != NULL)
            {
                /* continue to search to the left */
                p = p->left;
            }

            /* replace the node to remove with the smallest one that is larger */
            node = p;

            /* remove the smallest node from the right side of the original node and replace the right value with the node removed */
            node->right = treeRemove(tree , aux->right , p->key , &removedAux);

            /* also update the left */
            node->left = aux->left;

            /* if the balance factor is too great */
            if(balanceFactor(node) == 2)
            {
                /* if it is the left balance too great */
                if(balanceFactor(node->left) >= 0)
                {
                    /* rotate left */
                    node = LL(node);
                }
                else
                {
                    /* rotate right */
                    node = LR(node);
                }
            }
        }
        else
        {
            return node->left;
        }
    }

    /* update with correct height */
    node->treeHeight = height(node);
    return node;
}


bool treeMapContains(ptrTreeMap tree , const void *key)
{
    /* return value */
    bool result = FALSE;

    /* iterator on the tree */
    ptrTreeNode iterator = tree->root;

    /* comparison of the keys */
    intptr_t comparasion;


    /* while there are items to search */
    while(iterator != NULL)
    {
        /* compare the key with the iterator key */
        comparasion = tree->comparatorMethod(iterator->key , key);

        /* if greater */
        if(comparasion > 0)
        {
            /* then continue to search to the left */
            iterator = iterator->left;
        }
            /* if smaller */
        else if(comparasion < 0)
        {
            /* continue to search to the right */
            iterator = iterator->right;
        }
            /* if equal */
        else
        {
            /* found our object with the same value */
            result = TRUE;

            /* stop searching */
            break;
        }
    }

    /* return if could find (or not) the key on the map */
    return result;
}


uint8_t height(ptrTreeNode node)
{
    /* return value */
    uint8_t result;

    /* height of left and right nodes */
    uint8_t lh , rh;


    /* if there is no node */
    if(node == NULL)
    {
        /* height is 0 */
        result = 0U;
    }
        /* if there is a node */
    else
    {
        /* if the left does not exist */
        if(node->left == NULL)
        {
            /* left height is 0 */
            lh = 0U;
        }
        else
        {
            /* calculate left height */
            lh = 1U + node->left->treeHeight;
        }

        /* if the right does not exist */
        if(node->right == NULL)
        {
            /* right height is 0 */
            rh = 0U;
        }
        else
        {
            /* calculate right height */
            rh = 1U + node->right->treeHeight;
        }

        /* if the left height is greater than the right */
        if(lh > rh)
        {
            /* return the left height */
            result = lh;
        }
            /* if the right height is greater */
        else
        {
            /* return the right height */
            result = rh;
        }
    }

    /* return the height */
    return result;
}


ptrTreeNode rotateright(ptrTreeNode node)
{
    /* previous left node */
    ptrTreeNode previousLeft;

    /* get the current left node */
    previousLeft = node->left;

    /* update the left node */
    node->left = previousLeft->right;

    /* update the previous left right node */
    previousLeft->right = node;

    /* update the nodes height */
    node->treeHeight = height(node);

    /* update the previous node height */
    previousLeft->treeHeight = height(previousLeft);

    /* return the previous left node */
    return previousLeft;
}


ptrTreeNode rotateleft(ptrTreeNode node)
{
    /* previous right node */
    ptrTreeNode previousRight;

    /* get the current right node */
    previousRight = node->right;

    /* update the right node */
    node->right = previousRight->left;

    /* update the previous right left node */
    previousRight->left = node;

    /* update the node height */
    node->treeHeight = height(node);

    /* update the previous node height */
    previousRight->treeHeight = height(previousRight);

    /* return the previous right */
    return previousRight;
}


ptrTreeNode RR(ptrTreeNode node)
{
    /* rotate left */
    node = rotateleft(node);

    /* return the result of the rotation */
    return node;
}


ptrTreeNode LL(ptrTreeNode node)
{
    /* rotate right */
    node = rotateright(node);

    /* return the result of the rotation */
    return node;
}


ptrTreeNode LR(ptrTreeNode node)
{
    /* rotate left (the left node) */
    node->left = rotateleft(node->left);

    /* rotate right */
    node = rotateright(node);

    /* return the result of the rotation */
    return node;
}


ptrTreeNode RL(ptrTreeNode node)
{
    /* rotate right (the right node) */
    node->right = rotateright(node->right);

    /* rotate left */
    node = rotateleft(node);

    /* return the result of the rotation */
    return node;
}


int8_t balanceFactor(ptrTreeNode node)
{
    /* return value */
    int8_t result;

    uint16_t lh , rh;

    /* if the node does not exist */
    if(node == NULL)
    {
        /* then it is balanced */
        result = 0U;
    }
    else
    {
        /* if the left does not exist */
        if(node->left == NULL)
        {
            /* set left height to 0 */
            lh = 0U;
        }
        else
        {
            /* calculate left height */
            lh = 1U + node->left->treeHeight;
        }

        /* if the right does not exist */
        if(node->right == NULL)
        {
            /* set right height to 0 */
            rh = 0U;
        }
        else
        {
            /* calculate right height */
            rh = 1U + node->right->treeHeight;
        }

        /* calculate the difference between heights of left and right */
        result = lh - rh;

    }

    /* return the balance */
    return result;
}


uint8_t treeMapHeight(const ptrTreeMap tree)
{
    /* result of the method */
    uint8_t result;


    /* if there are no elements */
    if(tree->root == NULL)
    {
        /* then the height is 0 */
        result = 0U;
    }
    else
    {
        /* otherwise, get the height of the root */
        result = tree->root->treeHeight;
    }

    /* return the height */
    return result;
}
