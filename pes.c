#include "pes.h"
#include "index.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int cmd_init() {
    mkdir(".pes", 0755);
    mkdir(".pes/objects", 0755);

    printf("Initialized empty PES repository in .pes/\n");
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: pes <command>\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0) {
        return cmd_init();
    }

    if (strcmp(argv[1], "add") == 0) {
        if (argc < 3) {
            printf("Usage: pes add <files>\n");
            return 1;
        }

        Index index = {0};
        index_load(&index);

        for (int i = 2; i < argc; i++) {
            if (index_add(&index, argv[i]) != 0) {
                fprintf(stderr, "error: failed to add %s\n", argv[i]);
            }
        }

        return 0;
    }

    if (strcmp(argv[1], "status") == 0) {
        Index index = {0};
        index_load(&index);
        return index_status(&index);
    }

    printf("Unknown command\n");
    return 1;
}