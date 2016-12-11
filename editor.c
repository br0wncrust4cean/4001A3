#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h> 
int keyID1;

typedef struct {
    char accountNo[5];
    char PIN[3];
    float funds;
} infoTuple;

typedef struct {
    char message[9];
    infoTuple info;
	float funds;
}Message;

//message to receive and send messages to the message queue
typedef struct msgbuf {
	long mtype;
	char mtext[25];
} message_buf;

int promptForFunds(infoTuple* toSend) {
	int check;
	printf("Enter the funds for this account: ");
	check = scanf("%f", &(toSend->funds));
	if(check == 1) return 1;
	return -1;

}

int promptForPIN(infoTuple* toSend) {
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

int promptForAccount(infoTuple* toSend) {
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
	infoTuple info;
	message_buf msg;
	keyID1 = msgget((key_t) 1239, IPC_CREAT | 0600);
	while(true) {
		char *msgStr = malloc(sizeof(char)*25);
		while(promptForAccount(&info) != 1);
		while(promptForPIN(&info) != 1);
		while(promptForFunds(&info) != 1);
		sprintf(msgStr, "%5s,%3s,%0.2f,UPDATE DB", infoTuple.accountNo, infoTuple.PIN, infoTuple.funds);
		strcpy(msg.mtext, msgStr);
		msg.mtype = 1;
		if(msgsnd(keyID1, &msg, 25, 0) == -1){
			printf("Error");
		} else {
			printf("Sent to server");
		}
	}
}

void main() {
	pthread_t editorThread;
	//editor();

	pthread_create(&editorThread, NULL, editor, NULL);
	pthread_join(editorThread, NULL);
}
