#pragma once
#include "vectors.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
  void *key, *value;
} KV;

// Throw awat for vector declare completion
static inline char eq_kv(KV p1, KV p2) { return p1.key == p2.key; };

DECLARE_VECTOR_TYPE(KV, vkv, eq_kv)

typedef struct {
  vkv *buckets;
  size_t size;
  int (*eq)(void *a, void *b);
  long (*hash)(void *key);
} HashMap;

HashMap *hm_create(size_t size, int (*eq)(void *, void *),
                   long (*hash)(void *)) {
  assert(size > 0);
  HashMap *map = (HashMap *)malloc(sizeof(HashMap));

  map->size = size;
  map->buckets = (vkv *)calloc(size, sizeof(vkv));
  map->eq = eq;
  map->hash = hash;

  return map;
}

char hm_insert(HashMap *map, void *key, void *value) {
  long index = map->hash(key) % map->size;
  vkv *bucket = &map->buckets[index];
  if (bucket->count == 0) {
    vkv_init(bucket, 100);
  }

  KV pair = {key, value};
  for (int i = 0; i < bucket->count; i++) {
    if (map->eq(bucket->items[i].key, key)) {
      // Key already present
      return -1;
    }
  }

  vkv_append(bucket, (KV){key, value});
  return 0;
}

void *hm_at(HashMap *map, void *key) {
  long index = map->hash(key) % map->size;
  vkv *bucket = &map->buckets[index];
  if (map->buckets[index].count == 0)
    return NULL;

  for (int i = 0; i < bucket->count; i++) {
    if (map->eq(key, bucket->items[i].key))
      return bucket->items[i].value;
  }
  return NULL;
}

char hm_contains(HashMap *map, void *key) {
  long index = map->hash(key) % map->size;
  vkv *bucket = &map->buckets[index];
  if (map->buckets[index].count == 0)
    return 0;

  for (int i = 0; i < bucket->count; i++) {
    if (map->eq(key, bucket->items[i].key))
      return 1;
  }
  return 0;
}

void hm_remove(HashMap *map, void *key) { assert("Not implemented"); }
