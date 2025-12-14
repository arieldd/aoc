#pragma once
#include "vectors.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  void *key, *value;
  size_t next_collision_index;
} KV;

DECLARE_VECTOR_OF_TYPE(vkv, KV)

typedef struct {
  KV *buckets;
  vkv collisions;
  size_t size;
  int (*eq)(void *a, void *b);
  long (*hash)(void *key);
} HashMap;

HashMap hm_create(size_t size, int (*eq)(void *, void *),
                  long (*hash)(void *)) {
  if (!size)
    size = 1000;
  HashMap map = {0};

  map.size = size;
  map.buckets = calloc(size, sizeof(KV));
  map.collisions = vkv_init(size);
  map.collisions.count = 1; // index 0 is a special value for not initialised KV
  map.eq = eq;
  map.hash = hash;

  return map;
}

void hm_clear(HashMap *map) {
  memset(map->buckets, 0, sizeof(KV) * map->size);
  map->collisions.count = 1;
}

char hm_insert(HashMap *map, void *key, void *value) {
  long index = map->hash(key) % map->size;
  KV *entry = &map->buckets[index];

  while (entry->next_collision_index) {
    if (map->eq(entry->key, key)) {
      // Key already present
      return -1;
    }
    entry = vkv_at(&map->collisions, entry->next_collision_index);
  }

  if (!entry->key) {
    entry->key = key;
    entry->value = value;
  } else {
    entry->next_collision_index = map->collisions.count;
    vkv_append(&map->collisions, (KV){key, value, 0});
  }

  return 0;
}

void *hm_at(HashMap *map, void *key) {
  long index = map->hash(key) % map->size;
  KV *entry = &map->buckets[index];

  if (!entry->key)
    return NULL;

  for (;;) {
    if (map->eq(entry->key, key)) {
      return entry->value;
    }
    if (!entry->next_collision_index)
      break;

    entry = vkv_at(&map->collisions, entry->next_collision_index);
  }

  return NULL;
}

char hm_contains(HashMap *map, void *key) { return hm_at(map, key) != NULL; }

void *hm_remove(HashMap *map, void *key) { // probably wrong
  long index = map->hash(key) % map->size;
  KV *entry = &map->buckets[index], //
      *prev = entry;

  if (!entry->key)
    return NULL;

  for (;;) {
    if (map->eq(entry->key, key)) {
      if (prev->next_collision_index)
        prev->next_collision_index = entry->next_collision_index;
      return entry->value;
    }
    if (!entry->next_collision_index)
      break;

    prev = entry;
    entry = vkv_at(&map->collisions, entry->next_collision_index);
  }

  return NULL;
}

void hm_free(HashMap *map) {
  free(map->buckets);
  vkv_free(map->collisions);
}
