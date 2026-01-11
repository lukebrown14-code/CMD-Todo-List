#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "file.bin"

typedef struct {
  char **item;
  int capacity;
  int size;
} StringList_t;

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

int write_array_binary(const StringList_t *list) {
  FILE *file = fopen(FILE_NAME, "wb");
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

// StringList_t create_array_binary(int initial_capacity) {
//   StringList_t list = {0};

//   list.capacity = initial_capacity > 0 ? initial_capacity : 4;
//   list.size = 0;
//   list.item = malloc(list.capacity * sizeof(char *));

//   if (!list.item) {
//     perror("Initial allocation failed\n");
//     list.capacity = 0;
//   }

//   FILE *file = fopen(FILE_NAME, "wb");
//   if (!file) {
//     perror("Cannot create file");
//   }

//   fclose(file);
//   return list;
// }

StringList_t read_array_binary() {
  StringList_t list = {0};

  FILE *file = fopen(FILE_NAME, "rb");
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

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Usage:\n");
    printf("  %s print                    - show current list\n", argv[0]);
    printf("  %s add \"Buy milk\"           - add new item\n", argv[0]);
    printf("  %s remove 2                 - remove item (1-based index)\n",
           argv[0]);
    printf("  %s clear                    - delete all\n", argv[0]);
    return 1;
  }

  // Read File
  StringList_t list = read_array_binary();

  const char *cmd = argv[1];

  if (strcmp(cmd, "print") == 0) {
    print_array(&list);
  }

  else if (strcmp(cmd, "add") == 0 && argc >= 3) {
    if (add_array(&list, argv[2]) == 0) {
      write_array_binary(&list);
      printf("Added: %s\n", argv[2]);
    }
  }

  else if (strcmp(cmd, "remove") == 0 && argc >= 3) {
    int idx = atoi(argv[2]) - 1;
    if (idx >= 0 && idx < list.size) {
      free(list.item[idx]);
      memmove(&list.item[idx], &list.item[idx + 1],
              (list.size - idx - 1) * sizeof(char *));
      list.size--;
      write_array_binary(&list);
      printf("Item %d removed \n", idx + 1);
    } else {
      printf("Invalid index \n");
    }
  }

  else if (strcmp(cmd, "clear") == 0) {
    free_array(&list);
    remove(FILE_NAME);
    printf("List cleared\n");
  }

  else {
    printf("Unknow command: %s\n", cmd);
  }

  free_array(&list);

  return 1;
}
