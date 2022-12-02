#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>

#define BILLION 1000000000L;
FILE *fp;

void processA(int *pid,char temp){
	
	char root_folder[1000];
	getcwd(root_folder,sizeof(root_folder));

	if(*pid==0){
		if(temp=='a'){
			strcat(root_folder,"/temp1.sh");
			execl("/bin/sh","sh",root_folder,NULL);
		}else if(temp=='b'){
			strcat(root_folder,"/temp2.sh");
			execl("/bin/sh","sh",root_folder,NULL);
		}else if(temp=='c'){
			strcat(root_folder,"/temp3.sh");
			execl("/bin/sh","sh",root_folder,NULL);
		}
	}
}

int main(){
	int pid1,pid2,pid3;
	double a,b,c;
	struct timespec start,stop;
	clock_gettime(CLOCK_REALTIME,&start);
	pid1 = fork();
	if(pid1==0){
		struct sched_param param;
		param.sched_priority = 0;
		sched_setscheduler(pid1,SCHED_OTHER,&param);
		processA(&pid1,'a');
	}else{
		pid2 = fork();
		if(pid2==0){
			struct sched_param paramb;
			paramb.sched_priority = 50;
			sched_setscheduler(pid1,SCHED_FIFO,&paramb);
			processA(&pid2,'b');
		}else{
			pid3 = fork();
			if(pid3==0){
				struct sched_param paramc;
				paramc.sched_priority = 50;
				sched_setscheduler(pid1,SCHED_RR,&paramc);
				processA(&pid3,'c');
			}
		}
	}
	int status1,status2,status3;
	waitpid(pid1,&status1,0);
	clock_gettime(CLOCK_REALTIME,&stop);
	a = (stop.tv_sec-start.tv_sec) + (double)(stop.tv_nsec-start.tv_sec)/BILLION;
	waitpid(pid2,&status2,0);
	clock_gettime(CLOCK_REALTIME,&stop);
	b = (stop.tv_sec-start.tv_sec) + (double)(stop.tv_nsec-start.tv_sec)/BILLION;
	waitpid(pid3,&status3,0);
	clock_gettime(CLOCK_REALTIME,&stop);
	c = (stop.tv_sec-start.tv_sec) + (double)(stop.tv_nsec-start.tv_sec)/BILLION;

	fp = fopen("temp.txt","w");
	fprintf(fp,"a->%lf\nb->%lf\nc->%lf\n",a,b,c);
	fclose(fp);

	printf("a->%lf\n",a);
	printf("b->%lf\n",b);
	printf("c->%lf\n",c);
}