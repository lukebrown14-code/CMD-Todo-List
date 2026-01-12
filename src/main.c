#include "array.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "file.bin"

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

  StringList_t list = read_array_binary(FILE_NAME);

  const char *cmd = argv[1];

  if (strcmp(cmd, "print") == 0) {
    print_array(&list);
  }

  else if (strcmp(cmd, "add") == 0 && argc >= 3) {

    size_t total_len = 0;
    for (int i = 0; i < argc; i++) {
      total_len = strlen(argv[i]) + 1;
    }

    char *task = malloc(total_len + 1);
    if (!task) {
      perror("malloc failed");
      return 1;
    }

    task[0] = '\0';
    for(int i = 2; i < argc; i++){
      if(i > 2) strcat(task, " ");
      strcat(task, argv[i]);
    }

    add_array(&list, task);
    printf("Added: %s\n", task);

    free(task);
    write_array_binary(&list, FILE_NAME);

  }

  else if (strcmp(cmd, "remove") == 0 && argc >= 3) {
    int idx = atoi(argv[2]) - 1;
    if (idx >= 0 && idx < list.size) {
      free(list.item[idx]);
      memmove(&list.item[idx], &list.item[idx + 1],
              (list.size - idx - 1) * sizeof(char *));
      list.size--;
      write_array_binary(&list, FILE_NAME);
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
