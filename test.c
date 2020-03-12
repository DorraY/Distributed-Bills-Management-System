#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h> // for mysql stuff

void database_interrogation(int id,char*** resultat) {
    MYSQL *con = mysql_init(NULL);
        char requete[100];

        // char*** resultat = malloc(1024*sizeof(char***));    
    // char resultat[10][3][1024] =  (char ***) malloc(1024*1024*sizeof(char)) ;
      if (con==NULL) {
                fprintf(stderr, "%s\n" ,mysql_error(con));
                exit(-1);
            }
            if (mysql_real_connect(con,"localhost","dorra","stormborn","SI_ENTR2",0,NULL,0)==NULL) {
                fprintf(stderr,"%s\n",mysql_error(con));
                mysql_close(con);
                exit (1);
            }
            sprintf(requete,"SELECT * FROM Facture where id_client=%d",(id));
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

            MYSQL_ROW row;
            int i=-1;

            while ((row = mysql_fetch_row(result))) 
        { 
            i++;
            for(int j = 0; j < num_fields; j++) 
            { 
                strcpy(resultat[i][j],row[j] ? row[j] : "NULL");
                printf("%s ",resultat[i][j]);
            } 
            printf("\n");
        }
        mysql_free_result(result);
        mysql_close(con);    
        
}

int main(int argc, char** argv) {

    char*** resultat = (char ***) malloc(1024*sizeof(char***));    ;

    database_interrogation(1,resultat);
    // int i,j;
    // for (int i=0;i<10;i++) {
    //     for (int j=0;j<3;j++) {
    //         printf("%s",resultat[i][j]);
    //     }
    //     printf("\n");
    // }
    


        
}