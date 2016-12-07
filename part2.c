#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct infoTuple{
    char accountNo[5];
    char PIN[3];
    float funds;
} infoTuple;

struct node
{
    infoTuple data;
    struct node *next;
}*head;
 
typedef struct editorMessage{
    char message[9];
    infoTuple toUpdate;
}editorMessage;

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

/*
void addafter(int num, int loc)
{
    int i;
    struct node *temp,*left,*right;
    right=head;
    for(i=1;i<loc;i++)
    {
    left=right;
    right=right->next;
    }
    temp=(struct node *)malloc(sizeof(struct node));
    temp->data=num;
    left->next=temp;
    left=temp;
    left->next=right;
    return;
}*/
 
 
 
/*void insert(infoTuple num)
{
    int c=0;
    struct node *temp;
    temp=head;
    if(temp==NULL)
    {
    add(num);
    }
    else
    {
    while(temp!=NULL)
    {
        if(temp->data<num)
        c++;
        temp=temp->next;
    }
    if(c==0)
        add(num);
    else if(c<count())
        addafter(num,++c);
    else
        append(num);
    }
}*/
 
 
/*
int delete(infoTuple num)
{
    struct node *temp, *prev;
    temp=head;
    while(temp!=NULL)
    {
    if(temp->data==num)
    {
        if(temp==head)
        {
        head=temp->next;
        free(temp);
        return 1;
        }
        else
        {
        prev->next=temp->next;
        free(temp);
        return 1;
        }
    }
    else
    {
        prev=temp;
        temp= temp->next;
    }
    }
    return 0;
}*/
 
 
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
    
}