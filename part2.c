#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
int keyID1, keyID2;
pthread_mutex_t  notEmpty;
pthread_cond_t condition;

typedef struct {
    char accountNo[5];
    float funds;
    char PIN[3];

} infoTuple;

 
typedef struct {
    char message[9];
    infoTuple info;
}Message;

typedef struct msgbuf {
	long mtype;
	char mtext[25];
} message_buf;


void updateDatabase(Message receivedMessage){
	char info[50]; //arbitrary string
	sprintf(info, "\n%5s,%3s,%0.2f", receivedMessage.info.accountNo, receivedMessage.info.PIN, receivedMessage.info.funds);
	FILE* file = fopen("DataBase", "a");
	fputs(info, file);
	fclose(file);
}


int promptForFunds(Message* toSend) {
	int check;
	printf("Enter the funds for this account: ");
	check = scanf("%f", &(toSend->info.funds));
	if(check == 1) return 1;
	return -1;

}

float promptForWithdrawAmount() {
	float withdraw;
	printf("Enter the amount of money to withdaw: ");
	scanf("%f", &withdraw);
	if(withdraw > -1) return withdraw;
	else return -1;
}

int promptForFundsOrWithdraw() {
	printf("Press 1 to display funds or 2 to withdraw funds: ");
	int choice;
	scanf("%d", &choice);
	if(choice == 1) return 1;
	else if(choice == 2) return 2;
	else return 0;
}
int promptForPIN(Message* toSend) {
	int check, size;
	bool valid = true;
	valid = true;
	printf("\nPlease input a valid PIN (3 digits) : ");
	check = scanf("%s", toSend->info.PIN);
	char *p = toSend->info.PIN;
	size = 0;
	for(p; *p != '\0'; p++ ) {
		if(!isdigit(*p)) {
			valid = false;
		}
		size++;
	}
	if(check == 1 && size == 3 && valid == true) {
		return 1;
	}
	return 0;

}
int promptForAccount(Message* toSend) {
	int check, size;
	bool valid = true;
	printf("Please input a valid account number (5 digits) : ");
	check = scanf("%s", toSend->info.accountNo);
	char *p = toSend->info.accountNo;
	size = 0;
	for(p; *p != '\0'; p++ ) {
		if(!isdigit(*p)) {
			valid = false;
		}
		size++;
	}
	if(check == 1 && size == 5 && valid == true) {
		return 1;
	}
	return 0;
	
}
void messageToString(char * toFill, Message toSend) {
	sprintf(toFill, "%s,%s,%.2f,%s", toSend.info.accountNo, toSend.info.PIN, toSend.info.funds, toSend.message); 

}
Message stringToMessage(char* filled) {
	const char s[2] = ",";
	char *token;
	Message msg;
	int colCounter = 0;
	token = strtok(filled, s);
	while( token != NULL )  {
		
		if(colCounter == 0) {
			strcpy(msg.info.accountNo, token);	
		} else if(colCounter == 1) {
			strcpy(msg.info.PIN, token);		
		} else if(colCounter == 2){
			msg.info.funds = atoll(token);	
		} else {
			strncpy(msg.message, token, 10);
			colCounter = -1;
		}
		colCounter++;
		token = strtok(NULL, s);
	}

	return msg;
}

int checkForAccount(infoTuple* received, const char* fileName) {
	FILE* file = fopen(fileName, "r+");
	int colCounter = 0;
	char line[256];
	const char s[2] = ",";
	char *token;
	char values[20]; //idk if anyone is going to be keeping a billion bucks at the bank
	char * vp = values;
	char* front = values;
	infoTuple i;
	int rowNumber;
	while(fgets(line, sizeof(line), file)){
		char *p = line;
		vp = front;
		while(*p){
			*vp = *p;
			vp++;
			p++;
		}
				//vp = front;
		token = strtok(values, s);
		  while( token != NULL )  {	
				if(colCounter == 0) {
					strncpy(i.accountNo, token, 20);
				} else if(colCounter == 1) {
					strncpy(i.PIN, token, 20);	
				} else {
					i.funds = atof(token);
					colCounter = -1;
				}
				colCounter++;
				token = strtok(NULL, s);
		}
		
		if(strcmp(i.accountNo, received->accountNo) == 0 && strcmp(i.PIN, received->PIN) == 0) {
			strcpy(received->accountNo,i.accountNo);
			strcpy(received->PIN,i.PIN);
			received->funds = i.funds;
			return rowNumber;
 		}
 		rowNumber++;
		
	}
	fclose(file);
	return -1;
}
	
int checkSizeOfNum(char* num){
	char *p;
	p=num;
	int returnVal = 0;
	for(p; *p != '\0'; p++){
		returnVal++;
	}
	printf("****return val: %d\n", returnVal);
	return returnVal;
	
}

void *ATM(){
	Message toSend, receivedMessage;
	bool received = true;
	char cont = 'z';
	toSend.info.funds = -1;
	message_buf mbuf, mbuf2;
	int incorrect = 0;
	int choice = 0;
	bool okay;
	keyID1 = msgget((key_t) 1239, IPC_CREAT | 0600);
	//printf("THE ATM ID %d\n", keyID);
	while(cont != 'x' && cont != 'X') {

		while(promptForAccount(&toSend) != 1); 
		do {
			while(promptForPIN(&toSend) != 1);
			strcpy(toSend.message, "PIN");
			toSend.info.funds = 0.00;
			messageToString(mbuf.mtext, toSend);
			printf("Sending this to server: %s\n", mbuf.mtext);
			mbuf.mtype = 1;
			if(msgsnd(keyID1, &mbuf, 25, 0) == -1) {
				printf("feelsbadd") ;
			} else {
				pthread_cond_broadcast(&condition);
				sleep(1);
				printf("Sent from client\n");			
			} 
			pthread_mutex_lock(&notEmpty);
			while(msgrcv(keyID1, &mbuf, 25, 2, 0) == -1) {
				pthread_cond_wait(&condition, &notEmpty); 			
			}
		
				printf("Received from server: %s\n", mbuf.mtext);
				receivedMessage = stringToMessage(mbuf.mtext);
				okay = receivedMessage.message[0] == 'O' && receivedMessage.message[1] == 'K';
				if(okay == true) {
					incorrect = 0;
					do{
						choice = promptForFundsOrWithdraw();
					} while(choice == 0);
					if(choice == 1) {
						strcpy(receivedMessage.message, "FUNDS");
						mbuf.mtype = 1;
						if(msgsnd(keyID1, &mbuf, 25, 0) == -1) {
							printf("ATM could not send message: Funds \n") ;
						} else {
							printf("ATM has sent message\n");
						}
						while(msgrcv(keyID1, &mbuf, 25, 2, 0) == -1) {
							printf("Im from AT, waiting on you to signal!");
							pthread_cond_wait(&condition, &notEmpty); 			
						}
						receivedMessage = stringToMessage(mbuf.mtext);
						printf("Funds available: %.2f\n", receivedMessage.info.funds);
						choice = 2; //Needed to move on, hence why its not an if else!
					}
					if(choice == 2) {
						do {
							while((receivedMessage.info.funds = promptForWithdrawAmount()) == -1){}
							strcpy(receivedMessage.message, "WITH");
							mbuf.mtype = 1;
							messageToString(mbuf.mtext, receivedMessage);
							if(msgsnd(keyID1, &mbuf,25 , 0) == -1) {
								printf("feelsbadd") ;
							} else {
								printf("ATM has sent message: WITH\n");
								pthread_cond_wait(&condition, &notEmpty);
							}
							printf("Check if we made it past the conditional wait\n");
							while(msgrcv(keyID1, &mbuf, 25, 2, 0) == -1) {
								pthread_cond_wait(&condition, &notEmpty); 			
							}
							if(receivedMessage.message[0] == 'N') {
								printf("Not enough funds\n");
							} 
						} while(receivedMessage.message[0] == 'N');
					 printf("Enough funds available\n");
					}
					
					
				} else {
					printf("Im FALSE\n");
					incorrect++;
					if(incorrect == 3) {
						printf("Account Blocked \n");
						strcpy(receivedMessage.message, "BLOCKED");
						messageToString(mbuf.mtext, receivedMessage);
						if(msgsnd(keyID1, &mbuf, 25, 0) == -1) {
							printf("feelsbadd") ;
						} else {
							printf("ATM has sent message\n");
						}
						
					}
				}
			} while(okay == false && incorrect != 3); 
			printf("Enter X to quit or any another key to continue: ");
			scanf("%s", &cont); 
		} 
}



void *server(){
	Message receivedMessage, toSend;
	infoTuple dbRow;
	int rowNumber;
	message_buf mbuf;
	//probably want to make these constants
	char* updateMessage = "UPDATE DB";
	char* requestFunds = "FUNDS";
	char* withdrawMsg = "WITH";
	char* pinMsg = "PIN";
	keyID1= msgget((key_t) 1239, IPC_CREAT | 0600);
	//printf("SERVER ID: %d\n", keyID);
	while(true) 
	{	
		while(msgrcv(keyID1, &mbuf, 25, 1, IPC_NOWAIT) == -1) {
		
		}
			//pthread_mutex_lock(&notEmpty);
			printf("Received from atm: %s\n", mbuf.mtext);
			//pthread_mutex_lock(&notEmpty);
			receivedMessage = stringToMessage(mbuf.mtext);	
			printf("do i make it here, after converting?\n");
			if (strcmp(receivedMessage.message, updateMessage) == 0){
				printf("Im update");
				updateDatabase(receivedMessage);
			} else if (strcmp(receivedMessage.message, requestFunds) == 0) {
				printf("Im rfunds");
				//just send funds from row to ATM thread
			} else if (strcmp(receivedMessage.message, withdrawMsg) == 0) {
				printf("Im with");
				if(receivedMessage.info.funds < dbRow.funds){
					float money = dbRow.funds - receivedMessage.info.funds;
					strcpy(receivedMessage.message, "Y");
					messageToString(mbuf.mtext,receivedMessage);
					char update[10];
					char currMoney[10];
					char line[256];
					char *values = malloc(sizeof(char) * 25);
					char* front = values;
					const char s[2] = ",";
					char *currFunds = malloc(sizeof(char) * 10);
					sprintf(update, "%.2f", money);
					int iterations = 0, col = 0;
					sprintf(currMoney, "%.2f", dbRow.funds);
					FILE* file = fopen("DataBase.txt", "r+");
					while(fgets(line, sizeof(line), file)){
						if(rowNumber = iterations){
							int i;
							int numOfDigits = checkSizeOfNum(currMoney);
							char *zeroes = malloc(sizeof(char) * numOfDigits);
							char *zeroesFront = zeroes;
							printf("zero stuff\n");
							for (i = 0; i < numOfDigits; i++){
								printf("bloop\n");
								zeroes[i] = '0';
							}
							zeroes = zeroesFront;
							printf("%s\n", zeroes);
							fseek(file, 10, SEEK_CUR);
							fputs(zeroes, file);
						}
						iterations++;
					}
					fclose(file);
				}
			} else if(strcmp(receivedMessage.message, pinMsg)== 0) {
				printf("do i make it here, its a pin\n");
				if((rowNumber = checkForAccount(&(dbRow), "DataBase.txt")) != -1) {
					sleep(1);
					strcpy(receivedMessage.message, "OK");
					messageToString(mbuf.mtext, receivedMessage);
					printf("Sending this to atm: %s\n", mbuf.mtext);
					mbuf.mtype = 2;
					if(msgsnd(keyID1, &mbuf, 25, 0) == -1) {
						printf("feelsbadd") ;
					} else {
						printf("Sever has sent message\n");
						pthread_cond_broadcast(&condition);
						pthread_mutex_unlock(&notEmpty);
					} 
				} else {
				}
			}
		} 
	
	}



void main (void){
	//key_t key1 =  EditorQueue;
	//key_t key2 = ATMQueue;
	pthread_t atm, serv;
	pthread_mutex_init(&notEmpty, NULL);
	pthread_cond_init(&condition, NULL); 	
 	pthread_create(&atm, NULL, ATM, NULL);
	pthread_create(&serv, NULL, server, NULL);
	pthread_join(serv, NULL); 
	pthread_join(atm, NULL);
	

}
