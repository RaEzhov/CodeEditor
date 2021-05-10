#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define length 100

void editNameOutFile(int argn, char* name, char* argc[]){
    strcpy(name, "out");
    strcpy(name+3, argc[argn]);
}

int max(int a, int b){
    if(a>b){
        return a;
    }
    return b;
}

int MaxCycles(FILE** fw, int mode) {
    int level = 0;
    int maxLevel = 0;
    int temp = 0;
    char buf[length] = {0};
    while (1) {
        fscanf(*fw, "%s", buf);
        if (!(strcmp(buf, "if"))) {
            temp = MaxCycles(fw, 1) + level;
        }
        for (int i = 0; i < strlen(buf); i++) {
            if (buf[i] == '{') {
                level++;
            } else if (buf[i] == '}') {
                maxLevel = max(maxLevel, level);
                maxLevel = max(maxLevel, temp);
                level--;
                if (level == 0) {
                    return maxLevel - mode;
                }
            }
        }
    }
}

int main(int argv, char* argc[]) {
    char outName[20] = {0};
    editNameOutFile(1, outName, argc);
    FILE *fl = fopen(argc[1], "rt");
    FILE *fw = fopen(outName, "wt");
    char *buffer = (char *) malloc(sizeof(char) * length);
    int status = 0;
    int level = 0;
    int space = 0;
    int quotes = 0;
    int endl = 0;
    char c = 0;
    char prevC = 0;
    int swtc = 0;
    while (!feof(fl)) {
        fscanf(fl, "%c", &c);
        if (level == 0 && c == 'i' && prevC == 10) {
            prevC = '\n';
            fprintf(fw, "\n");
        }
        if (c == '/' && prevC == '/') {
            fprintf(fw, "/");
            while (c != 10 && !feof(fl)) {
                fscanf(fl, "%c", &c);
                fprintf(fw, "%c", c);
            }
        }
        if (c == '}' && prevC == '}') {
            continue;
        }
        if (c == ':') {
            swtc++;
            level++;
        }
        if (swtc > 0 && c == ';') {
            level--;
            swtc--;
        }
        if (c == '(' && (prevC == 'r' || prevC == 'e')){
            fprintf(fw, " ");
        }
        if (c == '{') {
            level++;
        }
        if (prevC == 'e' && (c == '<' || c == '"')) {
            prevC = ' ';
            fprintf(fw, " ");
        }
        if (c == '}') {
            level--;
            if (prevC == ';' || prevC == 10 || prevC == '}' || prevC == ' ') {
                putchar('\b');
                putchar('\b');
                putchar('\b');
                putchar('\b');
            }
        }
        if (c != ' ' && c != 10 && c != 9) {
            fprintf(fw, "%c", c);
            if (c == '}') {
                fprintf(fw, "\n");

                for (int i = 0; i < level; i++) {
                    prevC = ' ';
                    fprintf(fw, "    ");
                }
                prevC = '\n';
            }
        }
        if (c == ' ' && (prevC != ' ' && prevC != 10 && prevC != '#')) {
            prevC = ' ';
            fprintf(fw, " ");
        }
        if (c == 10 && prevC != 10 && prevC != '(' && prevC != ')' && (prevC < 'a' || prevC > 'z') &&
            (prevC < '1' || prevC > '9')) {
            fprintf(fw, "\n");
            for (int i = 0; i < level; i++) {
                prevC = ' ';
                fprintf(fw, "    ");
            }
        }
        prevC = c;
    }

    int k = 0;
    for (int i = 0; i < 5; i++)
        while (k > 2)
            k++;

    free(buffer);
    fclose(fl);
    fclose(fw);


    //open output and count vlojennost cyclov

    fw = fopen(outName, "rt");
    char buf[length] = {0};
    int cycle = 0;
    int maxCycle = -1;
    while (!feof(fw)) {
        fscanf(fw, "%s", buf);
        if ((!(strcmp(buf, "for")) || !(strcmp(buf, "while")))) {
            cycle = MaxCycles(&fw, 0);
        }
        if (!(strcmp(buf, "if"))) {
            cycle = MaxCycles(&fw, 1);
        }
        maxCycle = max(cycle, maxCycle);
    }
    printf("%d", maxCycle);
    fclose(fw);

    fw = fopen(outName, "rw");
    return 0;
}