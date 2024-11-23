#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void send_file(const char *filename, size_t part_size, const char *ip, int port)
{
// creation de sacket
	int sock=socket(AF_INET, SOCK_STREAM,0);
	if(sock<0){
		perror("erreur lors de la creation du socket ");
			 exit(-1);
			}
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_family=htons(port);
	if(inet_pton(AF_INET,ip,&addr.sin_addr)<=0){
		perror("addresse du serveur invalide");
		close(sock);
		exit(-1);
		}
	if(connect (sock, (struct sockaddr *)&addr, sizeof(addr))<0){
		perror("erreur de connexion");
		close(sock);
		exit(-1);
		}
	FILE *f = fopen(filename, "rb");
	if(!f){
		perror("erreur d'ouverture du fichier ");
			close(sock);
			exit(-1);
			}
			
	char buff[BUFFER_SIZE];
	size_t bytes_read;
	size_t total=0;
	
	while((bytes_read = fread(buff,1,sizeof(buff),f))>0){
		if(send(sock,buff,  bytes_read,0)<0){
			perror("erreur lors de l'envoi des donnees ");
			fclose(f);
			close(sock);
			exit(-1);
			}
	total+=bytes_read;
	if(total>=part_size){
		char ack[4];
		recv(sock, ack, sizeof(ack), 0);
		total=0;
		}
		}
	printf("fichier '%s' envoye avec succes .\n", filename);
			fclose(f);			
			exit(-1);
			}
					
int main(int argc, char * argv[]){
if(argc!=5){
	fprintf(stderr, "usage: %s <nomficher> <taille> <ip> <port>\n", argv[0]);
	 exit(-1);
	}
	const char *filename=argv[1];
	size_t part_size=atoi(argv[2]);
	const char *ip=argv[3];
	int port=atoi(argv[4]);
	
	send_file(filename, part_size, ip, port);
	return 0;
	}
	
	

/*void receive_and_join(const char *output_filename , int port){
	int sock=socket(AF_INET, SOCK_STREAM,0);
	if(sock<0){
		perror("erreur lors de la creation du socket ");
			 exit(-1);
			}
	struct sockaddr_in addr, client_addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr=INADDR_ANY;
	addr.sin_family=htons(port);
	
	if(bind( sock, (struct sockaddr *)&addr, sizeof(addr))<0){
		perror("erreur de liaison du socket");
		close(sock);
		exit(-1);
		}	
	if(listen(sock, 5)<0){
		perror("erreur d'ecoute ");
		close(sock);
		exit(-1);
		}
	printf("serveur en attente de connexion sur le port %d\n",port);
	socklen_t client_len=sizeof(client_addr);;
	int client_sock=accept(sock, (struct sockaddr *)&client_addr, &client_len);
	if(client_sock<0){
		perror("erreur de l'acceptation de la connexion ");
		close(sock);
		exit(-1);
		}
	FILE * output_f = fopen(output_filename, "rw");
	if(!output_f){
		perror("erreur d'ouverture du fichier ");
		close(sock);
		close(client_sock);
		exit(-1);
			}
	char buff[BUFFER_SIZE];
	size_t bytes_receive;		

	while((bytes_receive = recv(client_sock,buff,sizeof(buff),0))>0){
		fwrite(buff,1,bytes_receive, output_f);
		send(client_sock, "ACK", 3,0);
		}
		if(bytes_receive,0){
			perror("erreur de reception des donnees ");
			}
	printf("fichier '%s' recu avec succes .\n", output_filename);
		fclose(output_f);
		close(client_sock);
		close(sock);
		}
	
	int main(int argc, char * argv[]){
	if(argc!=3){
	fprintf(stderr, "usage: %s <nomficher_sortie> <port>\n", argv[0]);
	 exit(-1);
	}
	const char *output_filename=argv[1];	
	int port=atoi(argv[2]);
	
	receive_and_join(output_filename,port);
	return 0;
	}*/
