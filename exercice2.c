#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define DEFAULT_FILENAME "mat.bin"
// dim =dimension de la matrice
//mat =matrice
//savatxt =sauvagarde de la matrice texte

void genere(int dim, int **mat){
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
			mat[i][j]=rand() % 100;
			}
		}
	}

void savebin(const char * filename,int dim, int **mat){
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd<0){
		perror("erreur d'ouverture du fichier ");
			exit(-1);
			}
	if(write(fd, &dim, sizeof(int))!=sizeof(int)){
	 	perror("erreur lors de l'ecriture de la dimension ");
	 	close(fd);
			exit(-1);
			}
		for(int i=0;i<dim;i++){
			if(write(fd, mat[i],dim*sizeof(int))!=dim*sizeof(int)){
				perror("erreur lors de l'ecriture desdonnees");
	 			close(fd);
				exit(-1);
			}
			}
			if(close(fd)<0){
			perror("erreur d'ouverture du fichier ");
			exit(-1);
			}
			}
			
void savetxt(const char * filename,int dim, int **mat){
int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd<0){
		perror("erreur d'ouverture du fichier ");
			 exit(-1);
			}		
	char buff[128];
	int len;
	len=snprintf(buff, sizeof(buff), "%d\n", dim);
	if(write(fd,buff,len)!=len){
		perror("erreur lors de l'ecriture de la dimension ");
	 			close(fd);
				exit(-1);
			}
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
				len=snprintf(buff, sizeof(buff), "%d\n", mat[i][j]);
				if(write(fd,buff,len)!=len){
					perror("erreur lors de l'ecriture des donnees ");
	 				close(fd);
					exit(-1);
			}
			}
		if((write(fd, "\n", 1)!=1)){
			perror("erreur lors de l'ecriture du saut de ligne");
			close(fd);
			exit(-1);
			}
		}
		if(close(fd)<0){
			perror("erreur d'ouverture du fichier ");
			exit(-1);
			}
			}
	
/*void free(int **mat,int dim){
	for(int i=0;i<dim;i++){
	free(mat[i]);
	}
	free(mat);
}	*/
void chargebin(const char *filename){
int fd = open(filename, O_WRONLY);
	if(fd<0){
		perror("erreur d'ouverture du fichier ");
		exit(-1);
			}
	int dim;
	if(read(fd, &dim, sizeof(int))!=sizeof(int)){
			perror("erreur lors de la lecture de la dimension ");
	 		close(fd);
			exit(-1);
			}
	int **mat=malloc(dim* sizeof(int));
	for(int i=0;i<dim;i++){
		mat[i]=malloc(dim* sizeof(int));
		if(read(fd, mat[i], dim* sizeof(int))!= dim*sizeof(int)){
			perror("erreur lors de la lecture des donnees ");
	 				close(fd);
					exit(-1);
			}
			}
			close(fd);
	printf("matrice binaire %d*%d \n",dim,dim);
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
			printf("%d",mat[i][j]);
			}
			printf("\n");
		}
		for(int i=0;i<dim;i++){
			free(mat[i]);
			}
		free(mat);
		}

void chargetxt(const char * filename){
int fd = open(filename, O_WRONLY);
	if(fd<0){
		perror("erreur d'ouverture du fichier ");
		 exit(-1);
			}
			
	
					exit(-1);*/
			
	char buff[128];
	int dim, bytes_read;
	bytes_read=read(fd, buff, sizeof(buff));
	if(bytes_read<0){
		perror("erreur lors de la lecture de la dimension ");
		close(fd);
		exit(-1);
			}
	sscanf(buff, "%d", &dim);
		int **mat=malloc(dim* sizeof(int));
		for(int i=0;i<dim;i++){
			mat[i]=malloc(dim* sizeof(int));
			for(int j=0;j<dim;j++){
			if(read(fd, buff, sizeof(buff))<=0){
			perror("erreur lors de la lecture des donnees ");
	 				close(fd);
					exit(-1);
			}
			sscanf(buff, "%d", &mat[i][j]);
		}
		}
		if(close(fd)<0){
		perror("erreur lors de la lecture des donnees ");
					exit(-1);
			}
			
	printf("matrice texte %d*%d \n",dim,dim);
	for(int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
			printf("%d",mat[i][j]);
			}
			printf("\n");
		}
		for(int i=0;i<dim;i++){
			free(mat[i]);
			}
		free(mat);
		}
		
		
		
int main(int argc, char * argv[]){
	int opt;
	int dim=1;
	char *filename=DEFAULT_FILENAME;
	int mode_creer=0, mode_affiche=0, mode_texte=0;
	int **mat;
	
	while((opt=getopt(argc, argv, "cad:f:bt"))!=-1){
		switch(opt){
		case 'c':
			mode_creer=1;
			break;
		case 'a':
			mode_affiche=1;
			break;
		case 'd':
			//char *endptr;
			//dim=strtol(optarg,&endptr,10);
			dim=atoi(optarg);
			break;	
		case 'f':
			filename=optarg;
			break;
		case 't':
			mode_texte=1;
			break;
		default:
			fprintf(stderr, "usage: %s -c|-a -d <dim> -f <filename> [-b|-t]\n", argv[0]);
	 		return 0;
	}
	if(dim<=0){
		fprintf(stderr, "erreur dimension doit etre positif \n");
		return 0;
		}
	if(mode_creer){
		srand(time(NULL));
		int **mat=malloc(dim* sizeof(int));
		for(int i=0;i<dim;i++){
			mat[i]=malloc(dim* sizeof(int));
			}
	genere(dim,mat);
		if(mode_texte){
			savetxt(filename, dim,mat);
		}else{
			savebin(filename, dim,mat);
			}
		printf("matrice creer et enregistree : %s \n",filename);
	
	for(int i=0;i<dim;i++){
			free(mat[i]);
			}
		free(mat);
		}
	else if(mode_affiche){
		if(mode_texte){
			chargetxt(filename);
		}else{
			chargebin(filename);
			}
			}
	else{
		fprintf(stderr, "erreur: vous devez specifier -c(creer) ou -a(afficher) \n");
		}
		}
		return 0;
		}
		
			
		
		
		

			
			
