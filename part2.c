#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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

void append(infoTuple num)
{
    struct node *temp,*right;
    temp= (struct node *)malloc(sizeof(struct node));
    temp->data=num;
    right=(struct node *)head;
    while(right->next != NULL)
    right=right->next;
    right->next =temp;
    right=temp;
    right->next=NULL;
}
 
 
 
void add(infoTuple num )
{
    struct node *temp;
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    if (head== NULL)
    {
    head=temp;
    head->next=NULL;
    }
    else
    {
    temp->next=head;
    head=temp;
    }
}
 
void  display(struct node *r)
{
    r=head;
    if(r==NULL)
    {
    return;
    }
    while(r!=NULL)
    {
    printf("%0.2f ",r->data.funds);
    r=r->next;
    }
    printf("\n");
}
 
 
int count()
{
    struct node *n;
    int c=0;
    n=head;
    while(n!=NULL)
    {
    n=n->next;
    c++;
    }
    return c;
}

void server(){
	Message receivedMessage;
	infoTuple dbRow;

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
		snprintf(update, "%.2f", temp);
		FILE* file = fopen("DataBase.txt", "r+");
		fseek(file, 9, SEEK_CURR);
		fputs(update, file);
	} else if (strcmp(message, pinMsg) == 0) {
		infoTuple dbRow;

		FILE* file = fopen("DataBase.txt", "r");
		int colCounter = 0;
		char line[50];
		
		char* getValues = malloc(sizeof(char)*10); //idk if anyone is going to be keeping a billion bucks at the bank
		char* front = getValues;
		int i;
		while(fgets(line, sizeof(line), file)){
			char *p = line;
			printf("%s", *p);
			while(*p){
				if(*p == ','){
					if(colCounter == 0){
						getValues = front;
						for(i = 0; i < 5; i++){
							dbRow.accountNo[i] = getValues[i];
						}
					} else if (colCounter == 1) {
						getValues = front;
						for(i = 0; i < 3; i++){
							dbRow.PIN[i] = getValues[i];
						}
					} 

					colCounter++;
					getValues = front;
				} else if (isdigit(*p)) {
					*getValues = *p;
					getValues++;
				} else {
					dbRow.funds = atof(getValues);
					colCounter = 0;
					getValues = front;
				}
				printf("%s, %s, %0.2f\n", dbRow.accountNo, dbRow.PIN, dbRow.funds);
			}
		}
	}
}

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
	check = scanf("%d", &(toSend->funds));
	if(check == 1) return 1;
	return -1;

}

float promptForWithdrawAmount() {
	float withdraw;
	printf("Enter the amount of money to withdaw: ");
	scanf("%d", &withdraw);
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
	int check, size, incorrect;
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
	int check, size, incorrect;
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
void *ATM(){
	Message toSend, receivedMessage;
	bool received = true;
	char cont = 'z';
	toSend.funds = -1;
	int incorrect = 0;
	int choice = 0;
	float withdraw;
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
					while(toSend.funds = promptForWithdrawAmount() == -1){}
					strcpy(toSend.message, "WITH");
					sendMessage(toSend);
					while(received == false) {}
					strcpy(receivedMessage.message, "N");
					if(receivedMessage.message[0] = 'N') {
						printf("Not enough funds\n");
						while(toSend.funds = promptForWithdrawAmount() == -1){}
					} else {
						printf("Enough funds available\n");
					}
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

void editor(){
	Message toSend;
	while(true) {
		while(promptForAccount(&toSend) != 1);
		while(promptForPIN(&toSend) != 1);
		while(promptForFunds(&toSend) != 1);
		strcpy(toSend.message, "UPDATE DB");
		sendMessage(toSend);
	}
}

void init(){
    infoTuple firstRow = {.accountNo = "00001", .PIN = "107", .funds = 3443.22 };
    infoTuple secondRow = {.accountNo = "00011", .PIN = "323", .funds = 10089.97 };
    infoTuple thirdRow = {.accountNo = "00117", .PIN = "259", .funds = 112.00 };
    head = NULL;
    add(firstRow);
    add(secondRow);
    add(thirdRow);
}

int main (void){
    //init();
   	//pthread_t userThread;
   	//pthread_t editorThread;
    //ATM();
	/*
	float temp = 343.30;
	char update[10];
	snprintf(update, "%.2f", temp);
	FILE* file = fopen("DataBase.txt", "r+");
	fseek(file, 9, SEEK_CURR);
	fputs(update, file);*/
	infoTuple dbRow;

	FILE* file = fopen("DataBase.txt", "r");
	int colCounter = 0;
	char line[50];
	char *token;
	infoTuple i;
	char* getValues = malloc(sizeof(char)*20); //idk if anyone is going to be keeping a billion bucks at the bank
	char* front = getValues;
	int i;
	while(fgets(line, sizeof(line), file)){
		char *p = line;
		printf("%s", *p);
		while(*p){
			if(*p == ','){
				if (isdigit(*p) || *p == ','){ //stores value in the array if the value in the line is a digit
					long val = strtol(p, &p, 10);
					*getValues = *p;
					*getValues++;
				} else { //creates a PCB with all the proper values and moves to the next line
					getValues = front;
					token = strtok(getValues, '-');
					p++;
					if(colCounter == 0) {
						printf("ACCOUNT NO: %s, %s", i.accountNo, token);
						i.accountNo = token;
					}
					else if(colCounter == 1) {
						i.PIN = token;
						printf("PIN NO: %s, %s", i.PIN, token);
					}
					else {
						colCounter = 0;
						sscanf(token, "%.2f", &(i.funds));
						printf("Funds NO: %.2f, %s", i.funds, token);
					}
				}
			}
			printf("%.5s, %.3s, %0.2f\n", dbRow.accountNo, dbRow.PIN, dbRow.funds);
		}
	}
	fclose(file);
}
