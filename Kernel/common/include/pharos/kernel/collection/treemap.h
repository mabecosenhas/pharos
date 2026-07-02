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


#ifndef PHAROS_TREEMAP_H
#define PHAROS_TREEMAP_H


#include <pharos/kernel/collection/declarations.h>


/**
 * generic comparator method used to sort arrays, in tree maps, hash tables, etc
 */
typedef intptr_t(*ComparatorMethod)(const void *argA , const void *argB);

/**
 * Tree node
 */
struct TreeNode
{
    /**
     * smaller node
     */
    ptrTreeNode left;

    /**
     * bigger node
     */
    ptrTreeNode right;

    /**
     * address where the unique key of the node is
     */
    const void *key;

    /**
     * height of the node of the tree used to make the tree balanced
     * 2^8 of height allows 2^(256) number of elements.
     */
    uint8_t treeHeight;
};

/**
 * Tree map
 */
struct TreeMap
{
    /**
     * root node of the tree map
     */
    ptrTreeNode root;

    /**
     * method used to compare keys
     */
    ComparatorMethod comparatorMethod;

    /**
     * number of elements of the tree
     */
    uint32_t size;

    /**
     * used to determine the tree node of the object [bytes]
     */
    int16_t offset;
};


/**
 * initialize a tree map. This method should be the first one to be called before using a tree map.
 *
 * Example:
 *
 * structure to place on a TreeMap
 * typedef struct
 * {
 *
 *   node must be the 1st field of any structure to insert on the TreeMap
 *   TreeNode nodeForMapA
 *
 *   this must be the 2nd node. It is used to map this structure in a second map
 *   TreeNode nodeForMapB
 *
 *   this must be the 3rd node. It is used to map this structure in a third map
 *   TreeNode nodeForMapC
 *
 *    key to compare with (shared in all maps in this case) - note that the key does not even have to be on this structure
 *   uint16_t key
 *
 *    any values that may be used
 *   int value
 *    .
 *    .
 *    .
 * } Xpto , *ptrXpto
 *
 * method used to compare keys
 * int8_t comparator(uint16_t *keyA, uint16_t *keyB)
 * {
 *    if(*keyA > *keyB)
 *    {
 *       return 1
 *    }
 *    else if(*keyA < *keyB)
 *    {
 *       return -1
 *    }
 *
 *    return 0
 * }
 *
 * int main()
 * {
 *    TreeMap mapA , mapB , mapC
 *    Xpto x , y
 *
 *    x.key = 12
 *    x.value = 10
 *    y.key = 33322
 *    y.value = 111132
 *
 *    initialize the trees: mapA uses the first node, mapB uses the second node, etc
 *    treeMapInitialize(&mapA , comparator , offsetof(Xpto , nodeForMapA))
 *    treeMapInitialize(&mapB , comparator , offsetof(Xpto , nodeForMapB))
 *    treeMapInitialize(&mapC , comparator , offsetof(Xpto , nodeForMapC))
 *
 *    on mapA place both x and y
 *    treeMapPut(&mapA , &(x.key) , &x)
 *    treeMapPut(&mapA , &(y.key) , &y)
 *
 *    on mapB place only x
 *    treeMapPut(&mapB , &(x.key) , &x)
 *
 *    on mapC place only y
 *    treeMapPut(&mapB , &(y.key) , &y)
 * }
 *
 * @constraint a node can only be once, and only once in a tree, even if with different keys
 * 
 * @param tree tree map to initialize 
 * @param comparatorMethod method used to compare keys
 * @param offset offset of the number of bytes of the node inside the object
 */
void treeMapInitialize(const ptrTreeMap tree , ComparatorMethod comparatorMethod , int16_t offset);


/**
 * put an object on the specified tree with a given key and comparator method (used to compare keys)
 */
void treeMapPut(ptrTreeMap tree , const void *key , void *object);


/**
 * remove the first entry with key lower to or equal to the specified key
 */
void *treeMapRemoveLowerOrEqual(ptrTreeMap tree , const void *key);


/**
 * Get the entry with a key lower than or equal to the specified key
 * 
 * @param tree tree map
 * @param key address of the key to search for
 * 
 * @return returns the entry with a key lower than or equal to the specified key or NULL if none exists
 */
void *treeMapGetLowerOrEqual(ptrTreeMap tree , const void *key);


/**
 * get an object from the specified tree with a given key and comparator method (used to compare keys)
 *
 * @param tree tree map
 * @param key address of the key of the object to get
 * 
 * @return returns the object with the specified key or NULL if none
 */
void *treeMapGet(const ptrTreeMap tree , const void *key);


/**
 * remove an object from the map
 * 
 * @param tree tree map
 * @param key address of the key of the object to remove
 * 
 * @return returns the object removed or NULL if none
 */
void *treeMapRemove(ptrTreeMap tree , const void *key);


/**
 * remove the first node (has lowest key) on the tree
 * 
 * @param tree tree map
 * 
 * @return returns the the object or NULL if the tree is empty
 */
void *treeMapRemoveFirst(ptrTreeMap tree);


/**
 * remove an object from the specified tree with a given key and comparator method (used to compare keys)
 *
 * @return returns the object removed or NULL if none
 */
void *treeMapRemoveUnprotected(ptrTreeMap tree , const void *key);


/**
 * Get the element with the lowest key on the map
 * 
 * @param map map where to search the lowest key
 * 
 * @return returns NULL if the map is empty and the address of the object on the map with the lowest key otherwise
 */
void *treeMapGetFirst(ptrTreeMap map);


/**
 * Get the element with the highest key on the map
 * 
 * @param map map where to search the highest key
 * 
 * @return returns NULL if the map is empty and the address of the object on the map with the highest key otherwise
 */
void *treeMapGetLast(ptrTreeMap map);


/**
 * Get the element with lowest key on the map higher than the key of element
 * 
 * @param map map where to search
 * @param element element
 * 
 * @return returns the previous element to the element
 */
void *treeMapGetNext(ptrTreeMap tree , void *element);


/**
 * Get the element with highest key on the map lower than the key of element
 * 
 * @param map map where to search
 * @param element element
 * 
 * @return returns the previous element to the element
 */
void *treeMapGetPrevious(ptrTreeMap map , void *element);


/**
 * determines if the tree contains the specified key
 *
 * @return returns TRUE if the map contains the key and false otherwise
 */
bool treeMapContains(ptrTreeMap tree , const void *key);


/**
 * determines the height of the tree
 *
 * @return returns the height of the nodes (~log2(size))
 */
uint8_t treeMapHeight(const ptrTreeMap tree);


/**
 * Clears the tree map
 * 
 * @param tree tree map to clear
 */
INLINE void treeMapClear(const ptrTreeMap tree)
{
    /* reset the root */
    tree->root = NULL;

    /* reset the size */
    tree->size = 0U;
}


/**
 * determines if the tree contains any element
 *
 * @return returns TRUE if there is at least one element and FALSE otherwise
 */
INLINE bool treeMapIsEmpty(const ptrTreeMap tree)
{
    /* check the size to see if the map is empty */
    return tree->size == 0U ? TRUE : FALSE;
}


/**
 * determines the size of a tree
 *
 * @return returns the number of nodes (elements) of the tree
 */
INLINE uint32_t treeMapSize(const ptrTreeMap tree)
{
    /* return the size */
    return tree->size;
}

#endif /* TREEMAP_H */
