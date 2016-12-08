#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include<string.h>

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

}

void sendMessage(Message message) {
	
}
void ATM(){
	Message toSend, receivedMessage;
	bool received = true;
	char cont = 'z';
	int incorrect = 0;
	int choice;
	bool okay;
	while(cont != 'x' && cont != 'X') {
		while(promptForAccount(&toSend) != 1); 
		do {
			while(promptForPIN(&toSend) != 1); 
			strcpy(toSend.message, "PIN");
			sendMessage(toSend);
			while(received == false) {}
			strcpy(receivedMessage.message, "OZ"); //Obviously fix, hardcoded server message
			okay = receivedMessage.message[0] == 'O' && receivedMessage.message[1] == 'K';
			if(okay == true) {
				incorrect = 0;
				while(choice = promptForFunds() != 0)
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
int promptForFunds() {
	printf("Press 1 to display funds, 2 to withdraw");
	int check, size, incorrect, choice;
	bool valid = true;
	valid = true;
	check = scanf("%d", &choice);
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
void editor(){

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
    init();
    pthread_t userThread;
    pthread_t serverThread;
    pthread_t editorThread;
    int i = promptForFunds();
    printf("Number: %d", i);
}
