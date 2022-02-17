#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define FIRST 1000
#define STEP 10000
#define MAX 10000000

int main(){
	int *ans = (int *)calloc(FIRST,sizeof(int));
	if(ans == NULL) return -1;
	int n = 1;
	int len = FIRST;
	int anssize = FIRST;
	ans[0] = 1;

	FILE *fp;
	fp = fopen("Calc.csv","w");
	if(fp == NULL) return -1;
	long total = 0;
	while(n<=MAX){
		long cputime = clock();
		for(int i=0;i<len;i++){
			ans[i]*=n;
		}
		n++;
		int carry = 0;
		for(int i=0;i<len;i++){
			ans[i]+=carry;
			carry =0;
			if(ans[i]>9){
				carry= ans[i]/10;
				ans[i]= ans[i]%10;
			}
		}
		if(carry!=0){
			int add = 0,atmp = carry;
			while(atmp>0){
				atmp/=10;
				add++;
			}
			
			if(len+add > anssize){
				//reallocがうまくできないので代用
				int *tmp = (int *)calloc(anssize+STEP,sizeof(int));
				if(tmp == NULL){
					free(ans);
					return -1;
				}else{
					for(int i = 0;i<len;i++) tmp[i] = ans[i];
					free(ans);
					ans=tmp;
					anssize+=STEP;
				}
			}
			for(int i = len;i<len+add;i++){
				ans[i] = carry % 10;
				carry /= 10;
			}
			len+=add;
		}
		cputime=clock()-cputime;
		total+=cputime;
		printf("\r%.2fKB  %.2f%%",(float)sizeof(ans[0]) * anssize/1024,(float)n/MAX*100);
		fprintf(fp,"%d!,%ld,%ld\n",n,cputime,total);
	}
	printf("\n");
	int start = len -1;
	while(ans[start] == 0) start--;

	FILE *ansfp;	
	ansfp = fopen("ans.txt","w");
	int zero = 0;
	for(int i=start;i>=0;i--){
		if(i >= start - 10) printf("%d",ans[i]);
		if(i == start) printf(".");
		if(ans[i] == 0) zero++;
		else zero = 0;
		fprintf(ansfp,"%d",ans[i]);
	}
	fprintf(ansfp,"\n%d! Zero:%d\n",n-1,zero);
	printf("*10^%d\n%d! Zero:%d\n",start,n-1,zero);
	free(ans);
	fclose(fp);
	fclose(ansfp);
	return 0;
}
