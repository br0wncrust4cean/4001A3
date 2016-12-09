#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct {
    char accountNo[5];
    char PIN[3];
    float funds;
} infoTuple;

struct node
{
    infoTuple data;
    struct node *next;
}*head;
 
typedef struct {
    char message[9];
    infoTuple info;
	float funds;
}Message;


void sendMessage(Message message) {
	
}

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
	check = scanf("%f", &(toSend->funds));
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
					printf("ACCOUNT NO: %s \n", i.accountNo);
				} else if(colCounter == 1) {
					strncpy(i.PIN, token, 20);
					printf("PIN NO: %s\n", i.PIN);	
				} else {
					i.funds = atof(token);
					printf("Funds NO: %.2f\n", i.funds);
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
	toSend.funds = -1;
	int incorrect = 0;
	int choice = 0;
	bool okay;
	while(cont != 'x' && cont != 'X') {
		while(promptForAccount(&toSend) != 1); 
		do {
			while(promptForPIN(&toSend) != 1); 
			strcpy(toSend.message, "PIN");
			sendMessage(toSend);
			while(received == false) {}
			strcpy(receivedMessage.message, "OK"); //Obviously fix, hardcoded server message
			okay = receivedMessage.message[0] == 'O' && receivedMessage.message[1] == 'K';
			if(okay == true) {
				incorrect = 0;
				do{
					choice = promptForFundsOrWithdraw();
				} while(choice == 0);
				if(choice == 1) {;
					strcpy(toSend.message, "FUNDS");
					sendMessage(toSend);
					while(received == false) {}
					receivedMessage.funds = 100.00;
					printf("Funds available: %.2f\n", receivedMessage.funds);
					choice = 2;
					}
				 if(choice == 2) {
					do {
						while((toSend.funds = promptForWithdrawAmount()) == -1){}
						strcpy(toSend.message, "WITH");
						sendMessage(toSend);
						while(received == false) {}
						strcpy(receivedMessage.message, "N");
						if(receivedMessage.message[0] == 'N') {
							printf("Not enough funds\n");
						} 
					} while(receivedMessage.message[0] == 'N');
				 printf("Enough funds available\n");
				}
				
				
			} else {
				incorrect++;
				if(incorrect == 3) {
					printf("Account Blocked \n");
					strcpy(toSend.message, "BLOCKED");
					sendMessage(toSend);
					
				}
			}
		} while(okay == false && incorrect != 3);
	
		printf("Enter X to quit or any another key to continue: ");
		scanf("%s", &cont);
	}
}

void server(){
	Message receivedMessage;
	infoTuple dbRow;
	int rowNumber;

	//probably want to make these constants
	char* updateMessage = "UPDATE DB";
	char* requestFunds = "FUNDS";
	char* withdrawMsg = "WITH";
	char* pinMsg = "PIN";

	char* message = receivedMessage.message;

	if (strcmp(message, updateMessage) == 0){
		updateDatabase(receivedMessage);
	} else if (strcmp(message, requestFunds) == 0) {
		//just send funds from row to ATM thread
	} else if (strcmp(message, withdrawMsg) == 0) {
		//float money = dbRow.funds - receivedMessage.funds;
		float temp = 343.30;
		char update[10];
		sprintf(update, "%.2f", temp);
		FILE* file = fopen("DataBase.txt", "r+");
		fseek(file, 9, SEEK_CUR);
		fputs(update, file);
	} else if (strcmp(message, pinMsg) == 0) {
		if((rowNumber = checkForAccount(&dbRow, "DataBase.txt")) != -1) {
			//sendMessage
		} else {
			//sendbadMessage
		}
	}
}

void *editor(){
	Message toSend;
	while(true) {
		while(promptForAccount(&toSend) != 1);
		while(promptForPIN(&toSend) != 1);
		while(promptForFunds(&toSend) != 1);
		strcpy(toSend.message, "UPDATE DB");
		sendMessage(toSend);
	}
}


int main (void){
	key_t key =  1234;
	int x = msgget(key, IPC_CREAT | 0600);
 	

}
