#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define EditorQueue 1000
#define ATMQueue 2000
int keyID;
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
	sprintf(toFill, "%s,%s,%.2f,PIN", toSend.info.accountNo, toSend.info.PIN, toSend.info.funds); 

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
			received->funds = i.funds;
			return rowNumber;
 		}
 		rowNumber++;
		
	}
	fclose(file);
	return -1;
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
	keyID = msgget((key_t) 1234, IPC_CREAT | 0600);
	printf("THE ATM ID %d\n", keyID);
	while(cont != 'x' && cont != 'X') {
		while(promptForAccount(&toSend) != 1); 
//do {
			while(promptForPIN(&toSend) != 1);
			strcpy(toSend.message, "PIN");
			toSend.info.funds = 0.00;
			messageToString(mbuf.mtext, toSend);
			printf("%s\n", mbuf.mtext);
			mbuf.mtype = 1;
			if(msgsnd(keyID, &mbuf, 25, 0) == -1) {
				printf("feelsbadd") ;
			} else {
				printf("ATM has sent message\n");
			}
			mbuf2.mtype = 1;
			if(msgrcv(keyID, &mbuf2, 25, 1, 0) >= 0) {
				printf("why you hang");
			} else {
				printf("u no hang!");
			}
/*
				printf("ATM has rec message\n");
				okay = receivedMessage.message[0] == 'O' && receivedMessage.message[1] == 'K';
				if(okay == true) {
					printf("Im TRUE");
					incorrect = 0;
					do{
						choice = promptForFundsOrWithdraw();
					} while(choice == 0);
					if(choice == 1) {;
						strcpy(toSend.message, "FUNDS");
						if(msgsnd(keyID, &mbuf, sizeof(Message) - sizeof(long), 0) == -1) {
							printf("feelsbadd") ;
						} else {
							printf("ATM has sent message\n");
						}
						while(received == false) {}
						receivedMessage.info.funds = 100.00;
						printf("Funds available: %.2f\n", receivedMessage.info.funds);
						choice = 2;
						}
					 if(choice == 2) {
						do {
							while((toSend.info.funds = promptForWithdrawAmount()) == -1){}
							strcpy(toSend.message, "WITH");
							if(msgsnd(keyID, &toSend, sizeof(Message) - sizeof(long), 0) == -1) {
								printf("feelsbadd") ;
							} else {
								printf("ATM has sent message\n");
							}
							while(received == false) {}
							strcpy(receivedMessage.message, "N");
							if(receivedMessage.message[0] == 'N') {
								printf("Not enough funds\n");
							} 
						} while(receivedMessage.message[0] == 'N');
					 printf("Enough funds available\n");
					}
					
					
				} else {
					printf("Im FALSE");
					incorrect++;
					if(incorrect == 3) {
						printf("Account Blocked \n");
						strcpy(toSend.message, "BLOCKED");
						if(msgsnd(keyID, &toSend, sizeof(Message) - sizeof(long), 0) == -1) {
							printf("feelsbadd") ;
						} else {
							printf("ATM has sent message\n");
						}
						
					}
				}
			} else {
				printf("did i get here?");
			}
		} while(okay == false && incorrect != 3); */
	
		printf("Enter X to quit or any another key to continue: ");
		scanf("%s", &cont); 
	}
	
}

void *server(){
	Message receivedMessage;
	infoTuple dbRow;
	int rowNumber;
	message_buf mbuf;
	//probably want to make these constants
	char* updateMessage = "UPDATE DB";
	char* requestFunds = "FUNDS";
	char* withdrawMsg = "WITH";
	char* pinMsg = "PIN";
	keyID = msgget((key_t) 1234, IPC_CREAT | 0600);
	printf("SERVER ID: %d\n", keyID);
	while(true) 
	{	
		if(msgrcv(keyID, &mbuf, 25, 1, 0) >= 0) {
			printf("Message Received: %s\n", mbuf.mtext); 
			receivedMessage = stringToMessage(mbuf.mtext);
			char* message = receivedMessage.message;
			printf("My message: %s", message);
			
			/*if (strcmp(message, updateMessage) == 0){
				printf("Im update");
				updateDatabase(receivedMessage);
			} else if (strcmp(message, requestFunds) == 0) {
				printf("Im rfunds");
				//just send funds from row to ATM thread
			} else if (strcmp(message, withdrawMsg) == 0) {
				printf("Im with");
				//float money = dbRow.funds - receivedMessage.funds;
				float temp = 343.30;
				char update[10];
				sprintf(update, "%.2f", temp);
				FILE* file = fopen("DataBase.txt", "r+");
				fseek(file, 9, SEEK_CUR);
				fputs(update, file);
			} else */if (strcmp(message, pinMsg) == 0) {
				printf("Im pin");
				Message toSend;
				if((rowNumber = checkForAccount(&(toSend.info), "DataBase.txt")) != -1) {;
					//s.message = "OK";
					printf("Im here");
					//strcpy(s.message, "OK");
					//sendMessageS(&s, ID);
					 printf("I SENT IT OMG");
					//sendMessage
				} else {
					//sendbadMessage
					printf("ew");
				}
			} 
		} else {
			printf("Spongebob");
		}
	
	}
}

void main (void){
	//key_t key1 =  EditorQueue;
	//key_t key2 = ATMQueue;
	pthread_t atm, serv;
 	pthread_create(&atm, NULL, ATM, NULL);
	pthread_create(&serv, NULL, server, NULL);
	pthread_join(serv, NULL); 
	pthread_join(atm, NULL);
	

}
