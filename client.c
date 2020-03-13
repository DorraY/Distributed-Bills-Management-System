#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX 1024

int main(int argc , char* argv[]) {
    int socketClient,codeOperation,idClient;
    char aEnvoyer[MAX] = {0};
    char aRecevoir[MAX] = {0};
    int countr; // taille du message reçu
    struct sockaddr_in adresseServeur = {0} ; // adresse du serveur

    socketClient = socket(AF_INET,SOCK_STREAM,0);
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(atoi(argv[2])); // récuperer le port de la ligne de commande
    adresseServeur.sin_addr.s_addr = inet_addr(argv[1]);

    while (connect(socketClient,(struct sockaddr *) &adresseServeur,sizeof(adresseServeur)) == -1) sleep(1);
            printf("1: Récupérer l'information à  partir du sytème 1\n2: Récupérer l'information à partir du sytème 2 \n3: Récupérer à partir des 2 systèmes\n") ;
            scanf("%d",&codeOperation);
            while (codeOperation!= 1 && codeOperation!=2 && codeOperation!=3) {
                printf("Code non valide!"); 
                scanf("%d",&codeOperation);
            }
            printf("Donner l'id du client   ");
            scanf("%d",&idClient);
            while (idClient<0) {
                printf("Id doit être un entier positif.");
                printf("Donner l'id du client   ");
                scanf("%d",&idClient);
            }

            sprintf(aEnvoyer,"%d:%d",codeOperation,idClient);
            send(socketClient,aEnvoyer,sizeof(int)*MAX,0);
            countr = read(socketClient,aRecevoir,strlen(aRecevoir));
            printf("%s",aRecevoir);


    shutdown(socketClient, SHUT_RDWR);
    close(socketClient);
    return 0;
}