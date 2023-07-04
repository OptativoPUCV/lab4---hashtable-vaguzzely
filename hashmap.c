#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2)
{
  if(key1==NULL || key2==NULL) return 0;
  if(strcmp((char*)key1,(char*)key2) == 0) return 1;
  return 0;
}

void insertMap(HashMap* map, char* key, void* value) {
    long codigoHash = hash(key, map->capacity);
    int contIteracion = 0;

    if (map->size > (map->capacity * 0.7)) {
        enlarge(map);
    }

    while (map->buckets[codigoHash] != NULL && contIteracion < map->capacity) {
        if (is_equal(map->buckets[codigoHash]->key, key)) {
            return;
        }
        codigoHash = (codigoHash + 1) % map->capacity;
        contIteracion++;
    }

    if (contIteracion == map->capacity) {
        return;
    }

    map->buckets[codigoHash] = createPair(key, value);
    map->current = codigoHash;
    map->size++;
}


Pair* searchMap(HashMap* map, char* key) {
    if (map == NULL || key == NULL)
        return NULL;

    long index = hash(key, map->capacity);

    while (map->buckets[index] != NULL) {
        if (strcmp(map->buckets[index]->key, key) == 0) {
            map->current = index;
            return map->buckets[index];
        }
        index = (index + 1) % map->capacity;
    }
  return NULL;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair ** anteriorBuckets = map->buckets; 
    int anteriorCapac = map->capacity; 

    map->capacity *= 2;
    map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *));

    
    map->size = 0;

    
    for (int i = 0; i < anteriorCapac; i++) {
        if (anteriorBuckets[i] != NULL && anteriorBuckets[i]->key != NULL) {
            insertMap(map, anteriorBuckets[i]->key, anteriorBuckets[i]->value);
        }
    }

    free(anteriorBuckets);


}


HashMap * createMap(long capacity) 
{
  HashMap* map = malloc(sizeof(HashMap));
  map->buckets = malloc(capacity * sizeof(Pair*));
  memset(map->buckets, 0, capacity * sizeof(Pair*));
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;
    return map;
}

void eraseMap(HashMap * map,  char * key) 
{
  if (map == NULL || key == NULL)
        return;


    Pair* pair = searchMap(map, key);

    if (pair != NULL) {
        pair->key = NULL;
        map->size--;
    }
}


Pair* firstMap(HashMap* map) 
{
  map->current = -1;
  
  while (map->current < map->capacity - 1) 
  {
    map->current++;
    if (map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL) 
    {
      return map->buckets[map->current];
    }
  }
  return NULL;
}


Pair* nextMap(HashMap* map) 
{
  while (map->current < map->capacity - 1) 
  {
    map->current++;
    if (map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL) 
    {
      return map->buckets[map->current];
    }
  }
  return NULL;
}
