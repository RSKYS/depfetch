/*

Copyright 2025 Pouria Rezaei <Pouria.rz@outlook.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License at <http://www.gnu.org/licenses/> for
more details.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN_DEPS 512
#define LEN_NAMES 512
typedef unsigned short int intS;

typedef struct {
    char queue[LEN_DEPS][LEN_NAMES];
    intS front, rear;
} Queue;

void initQue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

intS isQueEmpty(Queue *q) {
    return q->front == q->rear;
}

void enqueue(Queue *q, const char *item) {
    if (q->rear < LEN_DEPS - 1 && item && *item) { 
        strncpy(q->queue[q->rear], item, LEN_NAMES - 1);
        q->queue[q->rear][LEN_NAMES - 1] = '\0';
        q->rear++;
    }
}

char *dequeue(Queue *q) {
    return isQueEmpty(q) ? NULL : q->queue[q->front++];
}

void getDeps(const char *package, intS is_list) {
    char dependencies[LEN_DEPS][LEN_NAMES] = {0};
    intS visited_count = 0;
    Queue queue;
    initQue(&queue);
    enqueue(&queue, package);

    while (!isQueEmpty(&queue)) {
        char *current_package = dequeue(&queue);
        if (!current_package || !*current_package) continue;

        intS already_visited = 0;
        for (intS i = 0; i < visited_count; i++) {
            if (strcmp(dependencies[i], current_package) == 0) {
                already_visited = 1;
                break;
            }
        }
        if (already_visited) continue;

        strncpy(dependencies[visited_count], current_package, LEN_NAMES - 1);
        dependencies[visited_count][LEN_NAMES - 1] = '\0';
        visited_count++;

        char command[LEN_NAMES + 50];
        snprintf(command, sizeof(command),
                 "paru -Si %s 2>/dev/null | grep 'Depends On' | sed \
                 -e 's/.*: //' -e 's/>.*//' -e 's/=.*//' \
                  | sort -u", current_package);

        FILE *fp = popen(command, "r");
        if (fp) {
            char line[LEN_NAMES * 10] = {0};
            if (fgets(line, sizeof(line), fp)) {
                line[strcspn(line, "\n")] = '\0';

                char *token = strtok(line, " ");
                while (token) {
                    if (strcmp(token, "None") != 0) {
                        enqueue(&queue, token);
                    }
                    token = strtok(NULL, " ");
                }
            }
            pclose(fp);
        }
    }

    if (visited_count > 0) {
        for (intS i = 0; i < visited_count; i++) {
            printf("%s%s", dependencies[i], is_list ? "\n" : (i == visited_count - 1 ? "\n" : " "));
        }
    }
}

void printUsage() {
    printf("\
Usage: depfetch [OPTIONS] package1 package2 ...\n\
Retrieve and display package dependencies.\n\n\
Options:\n\
  --list       Print dependencies one per line.\n\
  --help, -h   Show this help message and exit.\n\n\
Examples:\n\
  depfetch package1 package2      # Default: Print dependencies in a single line\n\
  depfetch package1 --list        # Print dependencies one per line\n\
  depfetch --help                 # Show this help message\n\n");
}

intS main(intS argc, char *argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    intS is_list = 0;
    char *packages[LEN_DEPS];
    intS pack_count = 0;

    for (intS i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--list") == 0) is_list = 1;
        
        else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printUsage();
            return 0; }
        else {
            packages[pack_count++] = argv[i];
        }
    }

    if (pack_count == 0) {
        printUsage();
        return 1;
    }

    for (intS i = 0; i < pack_count; i++) {
        getDeps(packages[i], is_list);
    }

    return 0;
}
