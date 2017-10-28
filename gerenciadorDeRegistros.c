#include <stdio.h>
#include <stdlib.h>

int main(){
	
	FILE *lf;
	lf = fopen("arquivo.bin", "wb");
	
	if(lf == NULL){
		printf("Erro na criacao do arquivo.\n\n");
		system("PAUSE");
		return 0;
	}
	
	fflush(lf);
	fclose(lf);
	
	return 0;
}
