#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h> // for mysql stuff

int main(int argc, char** argv) {

    MYSQL *con = mysql_init(NULL);
    char* requete;
    int id = atoi(argv[1]);
    char* resultat_de_requete[100];
    sprintf(resultat_de_requete,"Les factures du client ayant pour id %d:",id);
    strcat(resultat_de_requete,"\n");
    char* tableau_resultat;
    strcat(tableau_resultat,resultat_de_requete);
    puts(tableau_resultat);

      if (con==NULL) {
                fprintf(stderr, "%s\n" ,mysql_error(con));
                exit(-1);
            }
            if (mysql_real_connect(con,"localhost","dorra","stormborn","SI_ENTR2",0,NULL,0)==NULL) {
                fprintf(stderr,"%s\n",mysql_error(con));
                mysql_close(con);
                exit (1);
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

            MYSQL_ROW row=mysql_fetch_row(result);

            if (row==NULL) {
                printf("Pas de résultat pour le client portant l'id:%d",id);
                exit(0);
            }

            while ((row = mysql_fetch_row(result))) 
        { 
            for(int i = 0; i < num_fields; i++) 
            { 
                strcat(tableau_resultat, row[i] ? strcat(row[i],":") : "NULL:");      
            } 
            strcat(tableau_resultat,"\n");
        }
        mysql_free_result(result);
        mysql_close(con);    
        
}
        