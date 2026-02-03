#include "array.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Usage:\n");
    printf("  print                    - show current list\n");
    printf("  add \"Buy milk\"           - add new item\n");
    printf("  remove 2                 - remove item (1-based index)\n");
    printf("  clear                    - delete all\n");
    return 1;
  }

  const char *home = getenv("HOME");
  if (!home) {
    fprintf(stderr, "HOME environment variable not set\n");
    return 1;
  }

  char dir_path[PATH_MAX];
  snprintf(dir_path, sizeof(dir_path), "%s/.todo", home);
  mkdir(dir_path, 0755);

  char file_path[PATH_MAX];
  snprintf(file_path, sizeof(file_path), "%s/.todo/data.bin", home);

  StringList_t list = read_array_binary(file_path);

  const char *cmd = argv[1];

  if (strcmp(cmd, "print") == 0) {
    print_array(&list);
  }

  else if (strcmp(cmd, "add") == 0 && argc >= 3) {

    size_t total_len = 0;
    for (int i = 2; i < argc; i++) {
      total_len += strlen(argv[i]) + 1;
    }

    char *task = malloc(total_len);
    if (!task) {
      perror("malloc failed");
      return 1;
    }

    size_t offset = 0;
    for (int i = 2; i < argc; i++) {
      if (i > 2)
        task[offset++] = ' ';
      size_t len = strlen(argv[i]);
      memcpy(task + offset, argv[i], len);
      offset += len;
    }
    task[offset] = '\0';

    add_array(&list, task);
    printf("Added: %s\n", task);

    free(task);
    write_array_binary(&list, file_path);

  }

  else if (strcmp(cmd, "remove") == 0 && argc >= 3) {
    int idx = atoi(argv[2]) - 1;
    if (idx >= 0 && idx < list.size) {
      free(list.item[idx]);
      memmove(&list.item[idx], &list.item[idx + 1],
              (list.size - idx - 1) * sizeof(char *));
      list.size--;
      write_array_binary(&list, file_path);
      printf("Item %d removed \n", idx + 1);
    } else {
      printf("Invalid index \n");
    }
  }

  else if (strcmp(cmd, "clear") == 0) {
    free_array(&list);
    remove(file_path);
    printf("List cleared\n");
  }

  else {
    printf("Unknown command: %s\n", cmd);
  }

  free_array(&list);

  return 0;
}
