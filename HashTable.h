#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "MathUtils.h"
#include "Chunk.h"
#include <stdio.h>

class Node 
{
public:
    vec3 key;
    Chunk* value;
    Node* next;
  
    Node(const vec3& key, Chunk* value) 
    {
	this->key = key;
	this->value = value;
	this->next = NULL;
    }

    ~Node()
    {
	delete value;
	value = NULL;
    }
};
 
class HashTable 
{
public:  
    HashTable(int tableSize);
    ~HashTable();

    Node* get(const vec3& key);    
    Chunk* getChunk(const vec3& key); 

    void put(const vec3& key, Chunk* value);

    void remove(const vec3& key);

    int getHash(const vec3& key);

    void printTable();
    void printNode(Node* node);

    int getTableSize();
    int getNumberOfCrashes();

private:
    std::vector<Node*> _table;

    int _crashes;
    int _size;

};

#endif
