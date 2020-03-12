#include <stdio.h>      // pour printf() et fprintf()
#include <sys/socket.h> // pour socket(), bind()
#include <arpa/inet.h>  // pour htons et htonl
#include <stdlib.h>     // pour atoi(), exist() 
#include <string.h>     // pour memset(), strcpy()  
#include <unistd.h>     // pour close()  
#include <netinet/in.h> // pour struct sockaddr_in
#include <netdb.h> // pour hostent
#include <my_global.h>
#include <mysql.h> // for mysql stuff

#define MAX   1024       // longueur du Buffer

// compile with this command gcc -o Serveur2 $(mysql_config --cflags) Serveur2.c $(mysql_config --libs)


// serveur de port 3030

int socketDialogue;

const char* interrogation_bd(int id) {
    char* aEnvoyer;

    sprintf(aEnvoyer, "Il n'existe pas de client portant l'identifiant: %d",id);
    MYSQL *con = mysql_init(NULL);
    char requete[100];
    if (con==NULL) {
                fprintf(stderr, "%s\n" ,mysql_error(con));
                exit(-1);
            }
            if (mysql_real_connect(con,"localhost","dorra","stormborn","SI_ENTR2",0,NULL,0)==NULL) {
                fprintf(stderr,"%s\n",mysql_error(con));
                mysql_close(con);
                exit(1);
            }
            sprintf(requete,"SELECT * FROM Facture where id_client=%d",id);
            if (mysql_query(con,requete)) {
                fprintf(stderr, "%s\n" ,mysql_error(con));
                exit(-1);
            }
            
            MYSQL_RES *result = mysql_store_result(con);
            if (result==NULL) {
                fprintf(stderr, "%s\n" ,mysql_error(con));
                exit(-1);
            }
            int num_fields = mysql_num_fields(result);
            printf("%d",num_fields);

            MYSQL_ROW row;

            row = mysql_fetch_row(result);

            if (row==NULL) {
                send(socketDialogue,aEnvoyer,100*MAX,0);

            }

            printf("Les factures du client ayant pour id %d:",id);
            printf("\n");
            while (row = mysql_fetch_row(result)) 
            {
            for (int i=0;i<num_fields;i++) {

                printf("%s ", row[i] ? row[i] : "NULL"); // if row not null print row else print NULL

            }
                printf("\n");   
            }
        
        mysql_free_result(result);
        mysql_close(con);
        return aEnvoyer;
}

int main(int argc, char*argv[]) {
    
    char Buffer[MAX];
    socklen_t  longueurAdresse; 
    int socketServeur2,socketDialogue;
    struct sockaddr_in ServAddr;
    struct sockaddr_in ClientAddr;
    unsigned short ServPort;
    int countr,id;

    ServPort = atoi("3030");
    socketServeur2 = socket(AF_INET,SOCK_STREAM,0);
    if (socketServeur2<0) {
        perror("Problème de création de socket\n");
        return(-1);
    }
    longueurAdresse = sizeof(struct sockaddr_in);
    memset(&ServAddr,0x00,longueurAdresse);
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServAddr.sin_port = htons(ServPort);

    if ((bind(socketServeur2,(struct sockaddr*)&ServAddr,longueurAdresse)) <0) {
        perror("bind");
        return -1;
    }

       if(listen(socketServeur2,5)  < 0) 
   { 
      perror("listen"); 
      exit(-1); 
   } 

    
    while (1) {
        printf("\nServeur Entr2 en attente de demande de connexion\n");
        socketDialogue = accept(socketServeur2,(struct sockaddr*)&ClientAddr,&longueurAdresse);
        if (socketDialogue<0) {
            perror("accept");
            close(socketDialogue);
            return(-1);
        }
        memset(Buffer,0x00,MAX*sizeof(char));
        countr = recv(socketDialogue,Buffer,MAX*sizeof(char),0);
        switch (countr)
        {
        case -1:
            perror("recv");
            close(socketDialogue);
            return -1;
            break;
        case 0:
            close(socketDialogue);
            return 0;
        default:
            printf("message reçu %s\n",Buffer);
            id = atoi(Buffer);
            printf("%d\n",id);
            interrogation_bd(12835081);
            send(socketDialogue,interrogation_bd(id),sizeof(int)*MAX,0);
        }
        close(socketDialogue);
    }
    close(socketServeur2);

    return 0 ; 
}