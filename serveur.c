#include <stdio.h>      // pour printf() et fprintf()
#include <sys/socket.h> // pour socket(), bind()
#include <arpa/inet.h>  // pour htons et htonl
#include <stdlib.h>     // pour atoi(), exist() 
#include <string.h>     // pour memset(), strcpy()  
#include <unistd.h>     // pour close()  
#include <netinet/in.h> // pour struct sockaddr_in
#include <netdb.h> // pour hostent
#define MAX   1024       // longueur du Buffer

// le port du serveur proxy est 4200

int socketDialogue;

void appelServeur1(int id,char* adresse) {
   int socketServeur1;
   char aEnvoyer[MAX] = {0};
   char aRecevoir[MAX] = {0};
   int countr; // longueur du message envoyé
   unsigned short port ;
   struct sockaddr_in adresseServeur1 = {0};
   struct hostent *h;

   port = atoi("4200");

   socketServeur1 = socket(AF_INET,SOCK_STREAM,0);
   adresseServeur1.sin_family = AF_INET;
   adresseServeur1.sin_port = htons(port);
   adresseServeur1.sin_addr.s_addr = inet_addr(adresse);

   socketServeur1 = socket(AF_INET,SOCK_DGRAM,0);

   if (socketServeur1<0) {
      perror("Problème de création de socket\n");
      exit(-1);
   }
   printf("Socket créé avec succès (%d)\n",socketServeur1);
   h = gethostbyname(adresse);

   if (h==NULL) {
      herror("gethostbyname(): ");
      printf("Serveur inconnu %s",adresse);
      exit(1);
   }
   adresseServeur1.sin_family = h->h_addrtype;

   memcpy((char *)&adresseServeur1.sin_addr.s_addr,h->h_addr_list[0],h->h_length);

   adresseServeur1.sin_port = htons(port);
   memset(&adresseServeur1.sin_zero,0,8);

   memset(aEnvoyer,0x00,MAX*sizeof(char));

   // traitement 

   countr = sendto(socketServeur1,aEnvoyer,strlen(aEnvoyer),0,(struct sockaddr *)&adresseServeur1,sizeof(adresseServeur1));
   switch (countr)
   {
   case -1:
      perror("send to");
      close(socketServeur1);
      exit(-3);
   case 0:
      fprintf(stderr,"Aucune donnée à envoyer\n");
      close(socketServeur1);
      exit(0);
   default:
      if (countr!=strlen(aEnvoyer))
         fprintf(stderr,"Erreur dans l'envoi\n");
      else
         printf("Message envoyé avec succés.");
   }

   shutdown(socketServeur1,SHUT_RDWR);
   close(socketServeur1);

}

// port 3030
void appelServeur2(int id)
    {
   int countr,socketClientServeur2;
   struct sockaddr_in Serv2Addr = {0};      // @ du serveur

   char  BufferEnvoi[MAX],BufferReception[MAX]; 
   unsigned short Serv2Port;

   socketClientServeur2 = socket(AF_INET,SOCK_STREAM,0);
   if (socketClientServeur2<0) {
      perror("Problème de création de la socket\n");
      exit(-1);
   }
   Serv2Port = atoi("3030");
   Serv2Addr.sin_family = AF_INET;
   Serv2Addr.sin_port =htons(Serv2Port) ;
   Serv2Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   
   while(connect(socketClientServeur2,(struct sockaddr* )&Serv2Addr,sizeof(Serv2Addr))==-1) sleep(1);

   sprintf(BufferEnvoi,"%d",id);
   countr = sendto(socketClientServeur2,BufferEnvoi,strlen(BufferEnvoi),0,(struct sockaddr *)&Serv2Addr,sizeof(Serv2Addr));

   send(socketClientServeur2,BufferEnvoi,sizeof(char)*MAX,0);


   countr = read(socketClientServeur2,BufferReception,strlen(BufferReception));

   write(socketDialogue,BufferReception,strlen(BufferReception));

   shutdown(socketClientServeur2,SHUT_RDWR);
   close(socketClientServeur2);

} ;
void appelServeur1et2(int id, char* adresse) {

   appelServeur1(id,adresse);
   appelServeur2(id);
}
int  main(int argc, char * argv[]) 
{ 
   int  socketServeur;               // descripteur du socket d'écoute et descripteur du socket de dialogue
   struct sockaddr_in ServAddr;      // @ du serveur
   struct sockaddr_in ClientAddr;    // @ du client
   socklen_t   longueurAdresse; 
   char  Buffer[MAX];                // Buffer de réception
   unsigned short ServPort;          // port du serveur
   int  countr;                      // count receive: longueur du message reçu
   
   int  codeOperation,idFacture;              
   
   if(argc < 2) {
		fprintf(stderr, "Usage: %s ServPort\n", argv[0]);
		exit(1);
	}
   ServPort=atoi(argv[1]);
    socketServeur = socket(AF_INET, SOCK_STREAM,0);  
	/*** 0 indique que l’on utilisera le protocole par défaut associé à SOCK_STREAM soit TCP ***/ 
    if(socketServeur < 0)  /*** échec ? ***/
    { 
       perror("Probleme de creation de socket\n ");  /***  Affiche le message d ’erreur ***/  
       exit(-1);   /*** On sort en indiquant un code erreur ***/
    } 
   
   /* Création de l'adresse externe de socket serveur */
   longueurAdresse = sizeof(struct sockaddr_in); 
   memset(&ServAddr, 0x00, longueurAdresse); 
   ServAddr.sin_family = AF_INET; 
   ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);    
   ServAddr.sin_port = htons(ServPort); 

   if((bind(socketServeur, (struct  sockaddr *)&ServAddr, longueurAdresse))< 0) 
   { 
      perror("bind"); 
       exit(-1); 
   } 
   printf("Socket attachee avec  succes !\n"); 

   if(listen(socketServeur,5)  < 0) 
   { 
      perror("listen"); 
      exit(-1); 
   } 
while(1)
{   
   printf("Serveur proxy en attente de demande de conenxion\n\n"); 
   
   socketDialogue = accept(socketServeur,(struct sockaddr *)&ClientAddr, &longueurAdresse); 
   if (socketDialogue <0) 
       { 
          perror("accept"); 
          close(socketDialogue); 
          exit(-1); 
       } 
	
    memset(Buffer, 0x00, MAX*sizeof(char)); 
		
    countr =  recv(socketDialogue, Buffer, MAX*sizeof(char),0);  
    switch(countr) 
    { 
       case -1:  
             perror("recv"); /***  une  erreur  ***/ 
             close(socketDialogue); 
             exit(-1); 
       case 0: 
             fprintf(stderr,"La socket a été fermée par le client!\n\n"); 
             close(socketDialogue); 
             return  0; 
       default:  /***  réception de  n octets   ***/
             printf("Message reçu: %s (%d octets)\n\n", Buffer, countr); 
             //send(socketDialogue,"Bien reçu, le serveur proxy traitera votre demande.\n",100,0);

            sscanf(Buffer,"%d:%d",&codeOperation,&idFacture);

           switch (codeOperation) {
            case 1:
               appelServeur1(idFacture,argv[1]);
               break;
            case 2:
               appelServeur2(idFacture);
               break;
            case 3:
               appelServeur1et2(idFacture,argv[1]);
            default:
               printf("Etat érronée");
               exit(-1);
         }
       }     

       close(socketDialogue);  
}
   close(socketServeur); 

   return   0; 
} 