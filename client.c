#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define MAX 1024

void initServerAddress(struct sockaddr_in * adresseServeur, char * argv[]){
    adresseServeur->sin_family = AF_INET;
    adresseServeur->sin_port = htons(atoi(argv[2])); // récuperer le port de la ligne de commande
    adresseServeur->sin_addr.s_addr = inet_addr(argv[1]);
}

int getOperationCode(){
    int codeOperation;
    printf(
           "0: Quiter le programme\n"
           "1: Récupérer l'information à  partir du sytème 1\n"
           "2: Récupérer l'information à partir du sytème 2 \n"
           "3: Récupérer à partir des 2 systèmes\n") ;

    scanf("%d",&codeOperation);
    while (codeOperation!= 1 && codeOperation!=2 && codeOperation!=3 && codeOperation!=0) {
        printf("Code non valide!\n"); 
        printf(
           "0: Quiter le programme\n"
           "1: Récupérer l'information à  partir du sytème 1\n"
           "2: Récupérer l'information à partir du sytème 2 \n"
           "3: Récupérer à partir des 2 systèmes\n") ;
        scanf("%d",&codeOperation);
    }
}

int getClientID(){
    int idClient;
    printf("Donner l'id du client   ");
    scanf("%d",&idClient);
    while (idClient<0) {
        printf("Id doit être un entier positif.");
        printf("Donner l'id du client   ");
        scanf("%d",&idClient);
    }
    return idClient;
}

int main(int argc , char* argv[]) {
    int socketClient,codeOperation,idClient;
    char aEnvoyer[MAX] = {0};
    char aRecevoir[MAX] = {0};
    int countr; // taille du message reçu
    struct sockaddr_in adresseServeur = {0} ; // adresse du serveur

   
    initServerAddress(&adresseServeur, argv);
    socketClient = socket(AF_INET,SOCK_STREAM,0);
    while (connect(socketClient,(struct sockaddr *) &adresseServeur,sizeof(adresseServeur)) == -1){
            sleep(1);
        } 
    
    for (codeOperation = getOperationCode(); codeOperation!=0; codeOperation = getOperationCode()){

        
        idClient = getClientID();
        sprintf ( aEnvoyer, "%d:%d", codeOperation, idClient);
        send ( socketClient, aEnvoyer, sizeof(int)*MAX, 0);
        countr = read( socketClient, aRecevoir, MAX*sizeof(char*));
        printf ("%s\n", aRecevoir);
        
    }
    shutdown(socketClient, SHUT_RDWR);
    close(socketClient);

    
    return 0;
}