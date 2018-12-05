#include <stdio.h>
#include <string.h>
#include <sstream>
#include <iostream>

using namespace std;

struct TASK {
	long long uCountTask;
	long long startID;
	long long stopID;
	int index[256];
	long long mask[256];
	char * text[256][256];
	bool includeNull;
};

void DeleteText(char* (text[256][256]));

int makeTask(TASK *);

int makeTask(TASK *thisTask, bool includeNull);

int makeTask(TASK *, long long startID, long long stopID);

int makeTask(TASK *, long long startID, long long stopID, bool includeNull);

int makeText(char* (text[256][256]), int index[256], bool includeNull);

void setMask(TASK * thisTask);


void printText(char* (text[256][256]));

inline void getPassString(TASK *thisTask,long long taskNum, char *);

inline int passNum2index(TASK *thisTask,long long taskNum, int index[256]);

long long calcMaxTasknum(TASK * thisTask);


inline int passwdTest(char * passwd);

inline int passwdTest(char * cmd, char * passwd, char * option, char * check);


long long bruteForce(TASK * thisJob,char *cmd, char *option, char * checkString, char * outFile);

int testJob(TASK * thisJob, long long job);

void usage(char * cmd);

int main(int argc, char* argv[]){

	long long startID = 0;
	long long stopID = 0;
	long long jobID = 0;
	long long result=0;

	char cmd[100] = {0,};
	char option[100] = {0,};
	char check[100] = {0,};
	char outFile[100] ={0,};

	bool bIncludeNull = NULL;
	bool bBruteForce = true;

	TASK thisJob ={0,};


	//printText(thisJob.text);

	for(int i=1; i<argc; i++){
		if(argv[i][0]!='-')
			continue;
		switch(argv[i][1]){
		case 'n':
			if(i==argc-1){
				usage(argv[0]);
				return -1;
			}else{
				jobID = atol(argv[i+1]);
				bBruteForce = NULL;
			}
			break;
		case 't':
			bBruteForce = NULL;
			break;
		case 's':
			if(i==argc-1){
				usage(argv[0]);
				return -1;
			}else{
				startID = atol(argv[i+1]);
			}
			break;
		case 'e':
			if(i==argc-1){
				usage(argv[0]);
				return -1;
			}else{
				stopID = atol(argv[i+1]);
			}
			break;
		case 'z':
			bIncludeNull=true;
			break;
		case 'c':
			//printf("check cmd\n");
			if(i==argc-1){
				usage(argv[0]);
				return -1;
			}else{
				strncpy(cmd, argv[i+1],99);
			}
			break;
		case 'o':
			//printf("check option\n");
			if(i==argc-1){
				usage(argv[0]);
				return -1;
			}else{
				strncpy(option, argv[i+1],99);
			}
			break;
		case 'k':
			//printf("check define false response\n");
			if(i==argc-1){
				usage(argv[0]);
				return -1;
			}else{
				strncpy(check, argv[i+1],99);
			}
			break;
		case 'f':
			printf("check f\n");
			if(i==argc-1){
				usage(argv[0]);
				return -1;
			}else{
				strncpy(outFile,argv[i+1],99);
			}
			break;
		case 'h':
			usage(argv[0]);
			return 1;
		}
	}

	makeTask(&thisJob, startID, stopID, bIncludeNull);

	//printText(thisJob.text);

	if(	bBruteForce == NULL){
		printf("job ID %u\n", jobID);
		result = testJob(&thisJob, jobID);
	}
	else
		result = bruteForce(&thisJob, cmd, option, check, outFile);

	DeleteText(thisJob.text);

	getchar();
	return 0;
}

void usage(char * cmd){
	printf("a.exe [option][...]\n");
	printf("[option]\n");
	printf("\t-t : validate wordlist.txt\n\n");
	printf("\t-z : include empty word set(default : not include)\n");
	printf("\t-n <job number>: print out the passwd that jobnumber at the wordList\n");
	printf("\t-s <job number>: bruteForce start <job number> (default : 0)\n");
	printf("\t-e <job number>: bruteForce stop <job number>  (default : end of word space)\n\n");
	printf("\t-c <command>: bruteForce command with qute\n");
	printf("\t-o <command option>: bruteForce command with qute\n");
	printf("\t-k <check string>: bruteForce check strinhg first 3char \n");
}


long long bruteForce(TASK *thisJob,char *command, char *opt, char *check_string, char *outFile){
	long long uCount = 0;
	long long uN = 0;
	char buf[200] = {0,};
	char cmd[100] = {0,};
	char option[100] = {0,};
	char check[100] = {0,};
	char passwd[100] = {0,};
	char aJobID[100] = {0,};
	FILE * fdOutFile = NULL;

	uN = thisJob->startID;
	if(thisJob->stopID ==0)
		uCount = thisJob->uCountTask;
	else uCount = thisJob->stopID;


	printf("start ID : %u\n", thisJob->startID);
	printf("stop ID (0 is last ID) : %u\n", thisJob->stopID);
	printf("last ID : %u\n", uCount);
	printf("Null set : %d\n", thisJob->includeNull);
	printf("----------------------------------\n");

	if(command[0] == NULL){
	printf("cmd, passwd, option, true_response\n");
	printf("command : ");
	scanf("%99[^\n]s", cmd);
	getchar();
	}else{ strncpy(cmd, command, 99);}

	if(opt[0] == NULL){
	printf("option : ");
	scanf("%99[^\n]s", option);
	getchar();
	}else{ strncpy(option, opt, 99);}

	if(check_string[0] == NULL){
	printf("false response : ");
	scanf("%99[^\n]s", check);
	getchar();
	}else{strncpy(check, check_string, 99);}

	do{
		getPassString(thisJob, uN, passwd);
		///////////////////////////////
		//printf("passwd : %s\n", passwd);
		if(passwdTest(cmd, passwd, option, check)!=1)
		{
			if(outFile[0]==NULL)
				printf("%u, Find passwd : %s\n", uN, passwd);
			else{
				fdOutFile = fopen(outFile, "w+");
				fscanf(fdOutFile, "%u, Find passwd : %s\n", uN, passwd);
				fclose(fdOutFile);
			}
			return uN;
		}
		if(uN%1000==0)
			printf("%u\n", uN);
		uN++;
	}while(uCount != uN);

	printf("Done! but, there are no password in word list space");
	return 0;
}

int testJob(TASK * thisJob, long long job){
	long long jobID = 0;
	char passwd[256]={0,};

	if(job ==0){
		printf("password string generating number : ");
		scanf("%u", & jobID);
	} else
		jobID = job;
	getPassString(thisJob, jobID, passwd);
	printf("passwd space %u is %s\n", job, passwd);

	return 0;
}

int makeTask(TASK *thisTask, long long startID, long long stopID, bool includeNull){
	if(makeText(thisTask->text, thisTask->index, includeNull) == -1)
		return -1;
	thisTask->includeNull = includeNull;
	setMask(thisTask);
	thisTask->startID    = startID;
	thisTask->stopID     = stopID;
	thisTask->uCountTask = calcMaxTasknum(thisTask);
}

int makeTask(TASK *thisTask, long long startID, long long stopID){
	if(makeTask(thisTask) == -1)
		return -1;
	thisTask->startID    = startID;
	thisTask->stopID     = stopID;

}

int makeTask(TASK *thisTask, bool includeNull){
	if(makeText(thisTask->text, thisTask->index, includeNull) == -1)
		return -1;
	setMask(thisTask);
	thisTask->includeNull = includeNull;
	thisTask->startID    = 0;
	thisTask->stopID     = 0;
	thisTask->uCountTask = calcMaxTasknum(thisTask);
}

int makeTask(TASK *thisTask){
	if(makeText(thisTask->text, thisTask->index, false) == -1)
		return -1;
	setMask(thisTask);
	thisTask->startID    = 0;
	thisTask->stopID     = 0;
	thisTask->uCountTask = calcMaxTasknum(thisTask);
}

int makeText(char* (text[256][256]), int index[256], bool includeNull){

	int mask=0;
	int nCount=0;

	unsigned int len=0;
	char buf[256]={0,};
	int i=0,j=0,x=0,y=0;

	FILE * fd=NULL;
	nCount=256;

	fd=fopen("wordlist.txt","r+");
	if(fd==NULL){
		printf("\"wordlist.txt\" file open failed!!");
		gets(buf);
		return 0;
	}

	fgets(buf, 256, fd);
	if(buf==NULL)
		return 0;

	if(strcmp(buf,"<word>\n")==0){
		//printf("check first \"<word>\"\n");
	}else {
		printf("buf : %s", buf);
		printf("At the first part of the file, fill the \"<word>\" in wordlist.txt file \n");
		gets(buf);
		return -1;
	}


	for(j=0;j<256 && feof(fd)==0;j++){
		fgets(buf, 256, fd);

		if(buf==NULL) return 0;

		if(strcmp(buf,"<word>\n")==0)
		{
			index[i]=j;
			i++;
			j=-1;
			continue;
		}

		len = strlen(buf);
		if(buf[len-1]=='\n'){
			buf[len-1]='\n';
			len=len-1;
		}
		buf[len]='\0';

		if(len){
			text[i][j]=new char[len+1];
			strncpy(text[i][j], buf, len+1);
			//printf("insert[%d][%d] : %s\n", i, j, buf);
		}else break;
		index[i]=j;
		//if(includeNull) //캐릭터 셋 비어있는 경우 추가
			//index[i]=j+1;
	}
}

void DeleteText(char* (text[256][256])){
	int i=0;
	int j=0;
	if(text[i][j]==NULL){
		printf("text is empty");
		return;
	}

	for(;i<256&&(text[i][0]!=NULL);i++)
	{
		for(j=0;j<256&&(text[i][j]!=NULL);j++)
		{
			//printf("[ %d, %d ] : %s\n", i, j, text[i][j]);
			delete(text[i][j]);
		}
	}
	printf("clear the wordlist!! bye~!\n");
	return;
}

void printText(char* (text[256][256])){
	int i=0;
	int j=0;
	if(text[i][j]==NULL){
		printf("text is empty");
		return;
	}

	for(i=0;i<256&&(text[i][0]!=NULL);i++)
	{
		for(j=0;j<256&&(text[i][j]!=NULL);j++)
		{
			printf("[ %d, %d ] : %s\n", i, j, text[i][j]);
		}
	}
	return;
}

inline void getPassString(TASK *thisTask,long long taskNum, char *passString){
	int len = 0;
	int tmpIndex[256]={0,};
	len = passNum2index(thisTask, taskNum, tmpIndex);

	memset(passString, '\x0', 4);

	for(int i=0; i<len ; i++){
		if(thisTask->text[i][tmpIndex[i]])
			strcat(passString, thisTask->text[i][tmpIndex[i]]);
	}

}

inline int passNum2index(TASK * thisTask, long long taskNum, int index[256]){
	int i = 0;
	long long volume = 1;
	long long uN = taskNum;

	for(i=0 ; thisTask->index[i] != 0 ; i++){
		if(thisTask->includeNull){
			index[i] = (int)(uN % (long long)(thisTask->index[i]+1));
			uN=uN/(thisTask->index[i]+1);
		}
		else{
			index[i] = (int)(uN % (long long)thisTask->index[i]);
			uN=uN/thisTask->index[i];
		}
	}

	return i;
//		uN=taskNum%thisTask->mask[i];

}

void setMask(TASK * thisTask){
	long long volume = 1;
	long long uN =0;

	for(int i=0; thisTask->index[i]!=0; i++){
		//taskNum = index[0]*index[1]*index[2]*..... not every time ... move the out of loop.
		if(thisTask->includeNull)
			volume = volume * (long long)thisTask->index[i]+1;
		else
			volume = volume * (long long)thisTask->index[i];
		thisTask->mask[i] = volume;
	}
}

inline int passwdTest(char * passwd){

	char buf[256]="uage -P ";
	strcat(buf, passwd);
	strcat(buf, " ");
	strcat(buf, "last option");

	FILE* stream = _popen( buf, "r" );
	int bytesRead = fread( buf, 1, 128, stream );

	printf("\n%s\n", buf);
	//output check code

	return 0;
}

inline int passwdTest(char * ori_cmd, char * passwd, char * option, char * check){
	char buf[1024]={0,};
	char cmd[1024]={0,};
	char pw[100]={0,};
	int i=0;

	strncpy(pw,passwd,99);

	strncpy(cmd, ori_cmd,99);
	strncat(cmd, " \"",2);
	strncat(cmd, pw,99);
	strncat(cmd, "\" ",2);
	strncat(cmd, option,99);

	FILE* stream = _popen( cmd, "r" );
	//int bytesRead = fread( buf, 1, 128, stream );
	//printf("%s\n", buf);
	//_pclose(stream);
	while(fgets(buf, 128, stream)!=NULL)
	{
		if(strncmp(buf, check, 3)==0){
			_pclose(stream);
			return 1;
		}else{
			_pclose(stream);
			return 0;
		}
//		printf("%s\n", buf);
	}

	if(feof(stream)){
		//printf(" done 1job : %s %d\n", cmd, _pclose(stream));
		_pclose(stream);
	}	

	return 0;
}

long long calcMaxTasknum(TASK * thisTask){
	int i=0;
	long long num=1;
	while(thisTask->index[i]!=0){
		if(thisTask->includeNull)
			num = num * (thisTask->index[i]+1);
		else
			num = num * thisTask->index[i];
		i++;
	}

	return num;
}
