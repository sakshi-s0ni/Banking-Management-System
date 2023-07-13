#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>   // Internet Protocol family
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>

typedef struct normalUser{
	int userID;
	char name[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
}normalUser;

typedef struct jointUser{
	int userID;
	char name1[30];
	char name2[30];
	char password[10];
	int account_no;
	float balance;
	char status[20];
	//int active;
}jointUser;

typedef struct admin{
	int userID;
	char username[30];
	char password[10];
}admin;

normalUser getNormalUser(int ID){
	int i=ID-1000;
	normalUser currUser;
	int fd=open("NUfile",O_RDONLY,0744);

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    	     //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(normalUser));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return currUser;
}

jointUser getJointUser(int ID){
	int i=ID-1000;
	jointUser currUser;
	int fd=open("JUfile",O_RDONLY,0744);

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(jointUser));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return currUser;
}

admin getAdmin(int ID){
	int i=ID-1000;
	admin currUser;
	int fd=open("ADfile",O_RDONLY,0744);
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     //nth record
	lock.l_len=sizeof(admin);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(admin));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return currUser;
}

bool checkNormalUser(normalUser currUser){
	int i=currUser.userID-1000;
	int fd=open("NUfile",O_RDONLY,0744);
	bool result;
	normalUser temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    	     //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(normalUser));
	if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkJointUser(jointUser currUser){
	int i=currUser.userID-1000;
	int fd=open("JUfile",O_RDONLY,0744);
	bool result;
	jointUser temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(jointUser));
	if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE"))
		result=true;
	else
		result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkAdmin(admin currUser){
	int i=currUser.userID-1000;
	int fd=open("ADfile",O_RDONLY,0744);
	bool result;
	admin temp;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     //nth record
	lock.l_len=sizeof(admin);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(admin));
	if(!strcmp(temp.password,currUser.password))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool depositMoney(int accType,int ID,float amt){
	int i=ID-1000;
	//printf("hiii\n");
	if(accType==1){
		int fd=open("NUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    //nth record
		lock.l_len=sizeof(normalUser);	             //sizeof(record)
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normalUser));

		if(!strcmp(currUser.status,"ACTIVE")){
			currUser.balance+=amt;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
		}
		else	result=false;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(jointUser));

		if(!strcmp(currUser.status,"ACTIVE")){
			currUser.balance+=amt;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		// printf("hiii\n");
		sleep(10);
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

bool withdrawMoney(int accType,int ID,float amt){
	int i=ID-1000;
	if(accType==1){
		int fd=open("NUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    //nth record
		lock.l_len=sizeof(normalUser);	             //sizeof(record)
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normalUser));

		if(!strcmp(currUser.status,"ACTIVE") && currUser.balance>=amt){
			currUser.balance-=amt;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(jointUser));

		if(!strcmp(currUser.status,"ACTIVE") && currUser.balance>=amt){
			currUser.balance-=amt;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

float getBalance(int accType,int ID){
	int i=ID-1000;
	float result;
	if(accType==1){
		int i=ID-1000;
		int fd=open("NUfile",O_RDONLY,0744);
		normalUser temp;

		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    	     //nth record
		lock.l_len=sizeof(normalUser);	             //sizeof(record)
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&temp,sizeof(normalUser));
		if(!strcmp(temp.status,"ACTIVE"))	result=temp.balance;
		else					result=0;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2){
		int i=ID-1000;
		int fd=open("JUfile",O_RDONLY,0744);
		jointUser temp;

		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    	     //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&temp,sizeof(jointUser));
		if(!strcmp(temp.status,"ACTIVE"))	result=temp.balance;
		else					result=0;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return 0;
}

bool alterPassword(int accType,int ID,char newPwd[10]){
	int i=ID-1000;
	if(accType==1){
		int fd=open("NUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    //nth record
		lock.l_len=sizeof(normalUser);	             //sizeof(record)
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normalUser));

		if(!strcmp(currUser.status,"ACTIVE")){
			strcpy(currUser.password,newPwd);
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(jointUser));

		if(!strcmp(currUser.status,"ACTIVE")){
			strcpy(currUser.password,newPwd);
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

bool addNormalUser(normalUser record){
	int fd=open("NUfile",O_RDWR,0744);
	bool result;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(normalUser);    //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	normalUser endUser;
	lseek(fd,(-1)*sizeof(normalUser),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(normalUser));

	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");

	int j=write(fd,&record,sizeof(normalUser));
	if(j!=0)	result=true;
	else	result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool addJointUser(jointUser record){
	int fd=open("JUfile",O_RDWR,0744);
	bool result;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser endUser;
	lseek(fd,(-1)*sizeof(jointUser),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(jointUser));

	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");

	int j=write(fd,&record,sizeof(jointUser));
	if(j!=0)	result=true;
	else	result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool deleteNormalUser(int ID){
	int i=ID-1000;
	int fd=open("NUfile",O_RDWR,0744);
	bool result;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	normalUser currUser;
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(normalUser));

	if(!strcmp(currUser.status,"ACTIVE")){
		strcpy(currUser.status,"CLOSED");
		currUser.balance=0;

		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR);
		int j=write(fd,&currUser,sizeof(normalUser));
		if(j!=0)	result=true;
		else		result=false;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool deleteJointUser(int ID){
	int i=ID-1000;
	int fd=open("JUfile",O_RDWR,0744);
	bool result;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser currUser;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(jointUser));

	if(!strcmp(currUser.status,"ACTIVE")){
		strcpy(currUser.status,"CLOSED");
		currUser.balance=0;

		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR);
		int j=write(fd,&currUser,sizeof(jointUser));
		if(j!=0)	result=true;
		else		result=false;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool modifyNormalUser(normalUser modUser){
	int i=modUser.userID-1000;
	int fd=open("NUfile",O_RDWR,0744);
	bool result=false;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	normalUser currUser;
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(normalUser));

	if(!strcmp(currUser.status,"ACTIVE") && (modUser.account_no==currUser.account_no)){
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR);
		int j=write(fd,&modUser,sizeof(normalUser));
		if(j!=0)	result=true;
		else		result=false;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool modifyJointUser(jointUser modUser){
	int i=modUser.userID-1000;
	int fd=open("JUfile",O_RDWR,0744);
	bool result=false;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser currUser;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(jointUser));

	if(!strcmp(currUser.status,"ACTIVE")  && (modUser.account_no==currUser.account_no)){
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR);
		int j=write(fd,&modUser,sizeof(jointUser));
		if(j!=0)	result=true;
		else		result=false;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

void serverTask(int nsd){
	int msgLength,select,type,option,accType,currUserID;
	bool result;
	while(1){
		read(nsd,&option,sizeof(option));
		printf("Option : %d\n",option);

		if(option==1){
			normalUser currUser1;
			accType=1;
			msgLength=read(nsd,&currUser1,sizeof(normalUser));
			printf("Username : %d\n",currUser1.userID);
			printf("Password : %s\n",currUser1.password);
			currUserID=currUser1.userID;
			result=checkNormalUser(currUser1);
			write(nsd,&result,sizeof(result));
		}
		else if(option==2){
			jointUser currUser2;
			accType=2;
			msgLength=read(nsd,&currUser2,sizeof(jointUser));
			currUserID=currUser2.userID;
			printf("Username : %d\n",currUser2.userID);
			printf("Password : %s\n",currUser2.password);
			result=checkJointUser(currUser2);
			write(nsd,&result,sizeof(result));
		}
		else if(option==3){
			admin currUser3;
			accType=3;
			msgLength=read(nsd,&currUser3,sizeof(admin));
			currUserID=currUser3.userID;
			printf("Username : %d\n",currUser3.userID);
			printf("Password : %s\n",currUser3.password);
			result=checkAdmin(currUser3);
			write(nsd,&result,sizeof(result));
		}
		else{
			result=false;
			write(nsd,&result,sizeof(result));
		}
		if(result)	break;
	}

	while(1){
		read(nsd,&select,sizeof(int));
		if(option==1 || option==2){
			if(select==1){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=depositMoney(accType,currUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==2){
				float amt;
				read(nsd,&amt,sizeof(float));
				result=withdrawMoney(accType,currUserID,amt);
				write(nsd,&result,sizeof(result));
			}
			else if(select==3){
				float amt;
				amt=getBalance(accType,currUserID);
				write(nsd,&amt,sizeof(float));
			}
			else if(select==4){
				char pwd[10];
				read(nsd,pwd,sizeof(pwd));
				result=alterPassword(accType,currUserID,pwd);
				write(nsd,&result,sizeof(result));
			}
			else if(select==5){
				if(option==1){
					normalUser user1=getNormalUser(currUserID);
					write(nsd,&user1,sizeof(normalUser));
				}
				else if(option==2){
					jointUser user2=getJointUser(currUserID);
					write(nsd,&user2,sizeof(jointUser));
				}
			}
			else if(select==6)	break;
		}
		else if(option==3){
			read(nsd,&type,sizeof(int));
			if(select==1){
				if(type==1){
					normalUser newUser1;
					read(nsd,&newUser1,sizeof(normalUser));
					result=addNormalUser(newUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					jointUser newUser2;
					read(nsd,&newUser2,sizeof(jointUser));
					result=addJointUser(newUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==2){
				if(type==1){
					int delUserID1;
					read(nsd,&delUserID1,sizeof(int));
					result=deleteNormalUser(delUserID1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
					read(nsd,&delUserID2,sizeof(int));
					result=deleteJointUser(delUserID2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==3){
				if(type==1){
					normalUser modUser1;
					read(nsd,&modUser1,sizeof(normalUser));
					result=modifyNormalUser(modUser1);
					write(nsd,&result,sizeof(result));
				}
				else if(type==2){
					jointUser modUser2;
					read(nsd,&modUser2,sizeof(jointUser));
					result=modifyJointUser(modUser2);
					write(nsd,&result,sizeof(result));
				}
			}
			else if(select==4){
				if(type==1){
					normalUser searchUser1;
					int userID1;
					read(nsd,&userID1,sizeof(int));
					searchUser1=getNormalUser(userID1);
					write(nsd,&searchUser1,sizeof(normalUser));
				}
				else if(type==2){
					jointUser searchUser2;
					int userID2;
					read(nsd,&userID2,sizeof(int));
					searchUser2=getJointUser(userID2);
					write(nsd,&searchUser2,sizeof(jointUser));
				}
			}
			else if(select==5)	break;
		}
	}
	close(nsd);
	write(1,"Ended client session.....\n",sizeof("Ended client session.....\n"));
	return;
}

int main(){
	struct sockaddr_in server,client;   // The <netinet/in.h> header defines the sockaddr_in structure
	int sd,nsd,clientLen;
	bool result;
	sd=socket(AF_INET,SOCK_STREAM,0);   // int socket (int family, int type, int protocol);  // returns socket descriptor
	//TCP transport protocol,0 for default// type − It specifies the kind of socket you want. // if protocol = 0 then it's systems default
	server.sin_family=AF_INET;          // family − It specifies the protocol family i.e if family = AF_INET then it's IPv4 protocol
	server.sin_addr.s_addr=INADDR_ANY;  // INADDR_ANY value for IP address means the server's IP address will be assigned automatically.
	server.sin_port=htons(5555);
	// bind returns 0 on success and -1 on failure
	bind(sd,(struct sockaddr *)&server,sizeof(server));
	 // The bind function assigns a local protocol address to a socket.
	 // With the Internet protocols, the protocol address is the combination of either
	 // a 32-bit IPv4 address or a 128-bit IPv6 address, along with a 16-bit TCP or UDP port number.
	 // This function is called by TCP server only.
	listen(sd,5);	                      // listen function converts an unconnected socket into a passive socket, indicating that the kernel should accept incoming connection requests directed to this socket. // maximum number of connections the kernel should queue for this socket i.e max queue length :: 5 clients per connection
  // listen returns 0 on success and -1 on error.
	write(1,"Waiting for the client.....\n",sizeof("Waiting for the client.....\n"));
	while(1){
		clientLen = sizeof(client);
		nsd=accept(sd,(struct sockaddr *)&client,&clientLen);

		write(1,"Connected to the client.....\n",sizeof("Connected to the client.....\n"));
		if(!fork()){
			close(sd);	// child doesn't require sd
			serverTask(nsd);
			exit(0);
		}
		else{
			close(nsd);	// child has nsd and parent doesn't require it
		}
	}
	return 0;
}
