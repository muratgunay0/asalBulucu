#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void asalYaz(int * asallar, int * asalCount);
int toplamAsal=0;
int threadSayisi=12;

void* routine(void* arg)
{
	int *index = (int*)arg;
	int asalCount=0;
	int i;
	for(i =index[0]; i<=index[1]; i++) {
		int flag=0;
		for(int j=2; j<i; j++) {
			if((i%j)==0) {
				flag=1;
				break;
			}
		}
		if(flag==0)
			++(asalCount);
	}
	toplamAsal+=asalCount;
}
void threadCagir(int ASAL)
{
	int bolum= ASAL /threadSayisi;
	int **bolumler;
	bolumler =(int**)malloc(sizeof(int*)*threadSayisi);
	int toplam=0;
	for(int i=0; i<threadSayisi; i++) {
		bolumler[i]=(int*)malloc(sizeof(int)*2);
		for(int j=0; j<2; j++) {
			toplam+=j*bolum;
			bolumler[i][j]=toplam;
		}

	}
	pthread_t th[threadSayisi];
	printf("\nCoklu Cekirdek Asal Bulucu Calisiyor.\n%d sayisina kadar ki butun asal sayilar bulunacak.\n",ASAL);
	for (int i = 0; i < threadSayisi; i++) {

		if (pthread_create(&th[i], NULL, &routine, (void*)(bolumler[i])) != 0) {
			perror("Failed to created thread");
		}
	}
	for (int i = 0; i < threadSayisi; i++) {
		if (pthread_join(th[i], NULL) != 0) {
			perror("Failed to join thread");
		}
	}
}

int *asalBul(int max, int*asallar,int *asalCount)
{
	*asalCount=0;
	printf("\nTek Cekirdek Asal Bulucu Calisiyor.\n%d sayisina kadar ki butun asal sayilar bulunacak.\n",max);
	for(int i =0; i<max; i++) {
		int flag=0;
		for(int j=2; j<i; j++) {
			if((i%j)==0) {
				flag=1;
				break;
			}
		}
		if(flag==0) {
			++(*asalCount);
			asallar=(int*)realloc(asallar,(*asalCount)*sizeof(int));

			if((asallar==NULL)) {
				printf("Realloc hatasi.");
				return asallar;
			}
			asallar[(*asalCount)-1]=i;
		}
	}
	printf("%d tane asal sayi bulundu.\n",(*asalCount)-2);
	return asallar;
}
void asalYaz(int * asallar, int * asalCount)
{
	for(int i = 0; i<(*asalCount-1); i++) {
		printf("%d %d \n",i,asallar[i]);
	}
}
int main(int argc, char* argv[])
{
	int *asallar = (int*)malloc(sizeof(int));
	int *asalCount =(int*)malloc(sizeof(int));
	int ASAL=100000;
	clock_t start_t1, end_t1,start_t2, end_t2;
	start_t1 = clock();
	int * bulunanlar=asalBul(ASAL,asallar,asalCount);
	end_t1 = clock();
	printf("TEKLI CPU : %f saniyede gorevi tamamladi.\n", (double)(end_t1 - start_t1) / CLOCKS_PER_SEC);
	start_t2 = clock();
	threadCagir(ASAL);
	printf("%d tane asal sayi bulundu.\n",toplamAsal-2);
	end_t2 = clock();
	printf("COKLU CPU  : %f saniyede gorevi tamamladi.\n", (double)(end_t2 - start_t2) / CLOCKS_PER_SEC);
	free(asallar);
	free(asalCount);
	return 0;
}