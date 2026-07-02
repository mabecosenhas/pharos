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


#ifndef PHAROS_COLLECTION_DECLARATIONS_H
#define PHAROS_COLLECTION_DECLARATIONS_H

#include <pharos/hal/hal.h>

/**
 * Forward declaration of the list node and its pointer
 */
typedef struct LinkedListNode LinkedListNode , *ptrLinkedListNode;

/**
 * Forward declaration of the list and its pointer
 */
typedef struct LinkedList LinkedList , *ptrLinkedList;

/**
 * Forward declaration of TreeMap structure
 */
typedef struct TreeMap TreeMap , *ptrTreeMap;

/**
 * Forward declaration of TreeNode structure
 */
typedef struct TreeNode TreeNode , *ptrTreeNode;

/**
 * Forward declaration of SimpleList structure
 */
typedef struct SimpleList SimpleList , *ptrSimpleList;

/**
 * Forward declaration of SimpleListNode structure
 */
typedef struct SimpleListNode SimpleListNode , *ptrSimpleListNode;

/**
 * Forward declaration of DoubleLinkedSimpleListNode structure
 */
typedef struct DoubleLinkedSimpleListNode DoubleLinkedSimpleListNode , *ptrDoubleLinkedSimpleListNode;


#endif /* PHAROS_COLLECTION_DECLARATIONS_H */
