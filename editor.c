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

void *editor(){
	Message toSend;
	while(true) {
		while(promptForAccount(&toSend) != 1);
		while(promptForPIN(&toSend) != 1);
		while(promptForFunds(&toSend) != 1);
		strcpy(toSend.message, "UPDATE DB");
	}
}

void main() {
	pthread_t editorThread;
	//editor();

	pthread_create(&editorThread, NULL, editor, NULL);
	pthread_join(editorThread, NULL);
}
