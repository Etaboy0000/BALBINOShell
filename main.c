#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

char *buff;
char *cmd[100];

int interpreter_executer() {
    if (!strcmp(cmd[0], "exit")) {
        exit(0);
    }
    else if (!strcmp(cmd[0], "help")) {
        printf("Commandes disponibles : exit, help, clear, ls\n");
    }
    else if (!strcmp(cmd[0], "clear")) {
        system("clear");
    }
    else {
        // Commande externe
        int pid = fork();
        if (pid == 0) {
            // Exécuter la commande
            execvp(cmd[0], cmd);
            // Si exec échoue
            perror("Erreur d'exécution");
            exit(EXIT_FAILURE);
        }
        else if (pid > 0) {
            int status;
            wait(&status);
        }
        else {
            perror("Erreur de fork");
        }
    }
    return 0;
}

void organiser_cmd() {
    char *token = strtok(buff, " ");
    int i = 0;
    while (token != NULL) {
        cmd[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    cmd[i] = NULL;  // Fin du tableau pour execvp
}

int main(int argc, char **argv) {
    buff = (char *)malloc(1024);  // Allocation dynamique du buffer
    while (1) {
        printf("Balbino_Prompt >> ");
        memset(buff, 0, 1024);  // Remise à zéro du buffer

        // Lecture de l'entrée utilisateur
        fgets(buff, 1024, stdin);

        // Supprimer le retour à la ligne final
        buff[strcspn(buff, "\n")] = 0;

        organiser_cmd();  // Organisation de la commande
        interpreter_executer();  // Exécution de la commande
    }

    free(buff);  // Libération de la mémoire
    return 0;
}
