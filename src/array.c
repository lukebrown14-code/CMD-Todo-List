#include "array.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int resize_array(StringList_t *list, int new_capacity) {
  if (new_capacity <= list->capacity)
    return 0;

  char **new_items = realloc(list->item, new_capacity * sizeof(char *));
  if (!new_items) {
    perror("Realloc failed");
    return -1;
  }

  list->item = new_items;
  list->capacity = new_capacity;
  return 0;
}

void free_array(StringList_t *list) {
  if (!list)
    return;
  for (int i = 0; i < list->size; i++) {
    free(list->item[i]);
  }
  free(list->item);
  list->item = NULL;
  list->size = 0;
  list->capacity = 0;
}

int add_array(StringList_t *list, const char *str) {
  if (list->size >= list->capacity) {
    int new_cap = list->capacity ? list->capacity * 2 : 8;
    if (resize_array(list, new_cap) != 0)
      return -1;
  }

  char *copy = strdup(str);
  if (!copy) {
    perror("strdup failed");
    return -1;
  }

  list->item[list->size++] = copy;
  return 0;
}

int write_array_binary(const StringList_t *list, const char *filename ) {
  FILE *file = fopen(filename, "wb");
  if (!file) {
    perror("Failed to open file for writing");
    return -1;
  }

  fwrite(&list->size, sizeof(int), 1, file);

  for (int i = 0; i < list->size; i++) {
    size_t len = strlen(list->item[i]);
    fwrite(&len, sizeof(size_t), 1, file);
    fwrite(list->item[i], 1, len + 1, file);
  }

  fclose(file);
  return 0;
}

StringList_t read_array_binary(const char *filename) {
  StringList_t list = {0};

  FILE *file = fopen(filename, "rb");
  if (!file) {
    perror("Failed to open file");
    return list;
  }

  // Read number of strings;
  int count;
  if (fread(&count, sizeof(int), 1, file) != 1)
    goto corrupt;

  // Pre-allocate exact size
  if (resize_array(&list, count < 8 ? 8 : count) != 0)
    goto corrupt;

  list.size = count;

  for (int i = 0; i < count; i++) {
    size_t len;
    if (fread(&len, sizeof(size_t), 1, file) != 1)
      goto corrupt;

    char *s = malloc(len + 1);
    if (!s)
      goto corrupt;

    if (fread(s, 1, len + 1, file) != len + 1) {
      free(s);
      goto corrupt;
    }

    list.item[i] = s;
  }

  fclose(file);
  return list;

corrupt:
  fprintf(stderr, "File appers to be corrupted\n");
  free_array(&list);
  if (file)
    fclose(file);
  return (StringList_t){0};
}

void print_array(const StringList_t *list) {
  if (list->size == 0) {
    printf("List is empty \n");
    return;
  }
  for (int i = 0; i < list->size; i++) {
    printf("%3d. %s\n", i + 1, list->item[i]);
  }
}
