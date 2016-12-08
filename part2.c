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

void ATM(){
	Message toSend;
	int check, size, i;
	bool valid = true;
	while(true) {
		do {
			printf("Please input your account number: ");
			check = scanf("%s", toSend.info.accountNo);
			size = sizeof(toSend.info.accountNo)/sizeof(char);
			printf("\nTHIS IS NOW THE ACCOUNT NUMBER: %s, size: %d", toSend.info.accountNo,size);
			char *p = toSend.info.accountNo;
			for(p; *p != '\0'; p++ ) {
				if(!isdigit(*p)) {
					valid = false;
				}
			}
		} while(check != 1 || size != 5 || valid == true); /*
		check = printf("\nPlease input your PIN: ");
		check = scanf("%s", toSend.info.PIN);
		printf("\n THIS IS NOW YOUR PIN: %s, DIGITS: %d", toSend.info.PIN, check);
		strcpy(toSend.message, "PIN");
		printf("\n THIS IS NOW YOUR MESSAGE: %s \n", toSend.message); */
		
		
		
	}
	
	

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

    display(head);
	ATM();
    
}
