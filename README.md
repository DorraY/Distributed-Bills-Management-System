# SAR

compile Serveur2 wih : gcc -o Serveur2 $(mysql_config --cflags) Serveur2.c $(mysql_config --libs)
compile Serveur1, Serveur and myclient with standard gcc.

launch the servers first
Serveur2 : port 3030
Serveur1 : port 2058
Serveur : port 4200
