#include "HashTable.h"

HashTable::HashTable(int tableSize)
{
    _table.resize(tableSize);

    _crashes = 0;
    _size = 0;
}
  
HashTable::~HashTable() 
{
    for (int i = 0; i < _table.size(); i++)
    {
	Node* node = _table.at(i);

	if (node != NULL) 
	{
	    Node *prevNode = NULL;
	    
	    while (node != NULL) 
	    {
		prevNode = node;

		node = node->next;

		delete node;
		
		--_size;
	    }
	}
    }
}

Node* HashTable::get(const vec3& key) 
{
    int hash = getHash(key);
    
    Node* node = _table.at(hash);
    
    if (node != NULL)
    {
	while (node->next != NULL && (node->key.x != key.x || node->key.z != key.z))
	    node = node->next;
    }

    return node;
}

Chunk* HashTable::getChunk(const vec3& key) 
{
    Node* node = get(key);

    if (node != NULL)
	return node->value;

    return NULL;
}
  
void HashTable::put(const vec3& key, Chunk* value) 
{
    int hash = getHash(key);

    Node* node = _table.at(hash);

    if (node == NULL)
    {
	_table.at(hash) = new Node(key, value);

	++_size;
    }
    else 
    {	
	while (node->next != NULL && (node->key.x != key.x || node->key.z != key.z))
	    node = node->next;
      
	if (node->key.x != key.x || node->key.z != key.z)
	{
	    node->next = new Node(key, value);
	    
	    ++_size;
	    
	    ++_crashes;
	}
    }
}  

void HashTable::remove(const vec3& key)
{
    int hash = getHash(key);

    Node* node = _table.at(hash);

    if (node == NULL)
    {
	return;
    }

    if (node->key.x == key.x && node->key.z == key.z)
    {
	if (node->next != NULL) 
	{
	    _table.at(hash) = node->next;

	    delete node;

	    --_size;

	    return;
	}
	else
	{
	    delete node;
	    
	    _table.at(hash) = NULL;

	    --_size;

	    return;
	}
    }

    while (node->next != NULL)
    {
	if (node->next->key.x == key.x && node->next->key.z == key.z)
	{
	    Node* tmp = node->next;

	    node->next = tmp->next;

	    delete tmp;	  

	    --_size;

	    return;
	}
	else
	{
	    node = node->next;
	}
    }
}
  
int HashTable::getHash(const vec3& key)
{
    long int sum = 0;

    int x = (key.x < 0.0f) ? key.x * -1 : key.x;
    int z = (key.z < 0.0f) ? key.z * -1 : key.z;

    sum = 31 + x;
    sum *= 31 + z;

    return (sum % _table.size());
}

void HashTable::printTable()
{
    for (int i = 0; i < _table.size(); i++)
    {
	if (_table.at(i) == NULL)
	{
	    printf("0\n");
	}
	else
	{
	    printNode(_table.at(i));
	    
	    printf("\n"); 
	}
    }
}

void HashTable::printNode(Node* node)
{
    printf(" (%f, %f) ", node->key.x, node->key.z);

    if (node->next != NULL)
    {
	printNode(node->next);
    }
}

int HashTable::getTableSize()
{
    return _size;
}

int HashTable::getNumberOfCrashes()
{
    return _crashes;
}
