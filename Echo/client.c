#include "socksLib.h"

int main(int argc, char *argv[]){

    int sockFd = 0; //file descripter for socket
    struct sockaddr_in servAddr = {0}; //address for client
    char *sendLine = NULL; //for sending
    char *buf = NULL; //for receiving

    //create a local copy of bufLen for getline to change as necessary
    size_t bufLen = BUFF_LEN;

    //Print usage if cmd line error
    if(argc != 3){
        printf("Usage: ./Client <Server IP> <Server port>\n");
        exit(EXIT_FAILURE);
    }
    //Check IP
    if((isValidIP(argv[1])) <= 0){
        printf("Error: Not a valid IP address\n");
        exit(EXIT_FAILURE);
    }
    //Check Port
    if((isValidPort(argv[2])) <= 0){
        exit(EXIT_FAILURE);
    }

    //Create socket
    if((sockFd = SocketDemoUtils_createTcpSocket()) == -1){
        exit(EXIT_FAILURE);
    }

    //Ensure servAddr memmory space is clean
    memset(&servAddr, 0, sizeof(servAddr));

    //Populate address info
    if((SocketDemoUtils_populateAddrInfo(argv[1], argv[2], &servAddr)) == -1){
        exit(EXIT_FAILURE);
    }

    //Connect to server
    if((connect(sockFd, (struct sockaddr *) &servAddr, sizeof(servAddr))) == -1){
        perror("Error connecting to server ");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server.\n");

    //Allocate memory for outgoing message
    sendLine = (char *) malloc(BUFF_LEN);

    //Send message
    while(1){
        //Get stdin
        printf("Type your message [type 'quit' to quit]:\n");
        if((getline(&sendLine, &bufLen, stdin)) == -1){
            perror("getline error ");
            exit(EXIT_FAILURE);
        }

        //Send to the server
        if((SocketDemoUtils_send(sockFd, sendLine, strlen(sendLine))) == -1){
            exit(EXIT_FAILURE);
        }

        //User wants to quit
        if((strcmp(sendLine, "quit\n") == 0) || (strcmp(sendLine, "Quit\n") == 0)){
            printf("Goodbye.\n");
            close(sockFd);
            break;
        } else { //the user receives what they sent
            if((SocketDemoUtils_recv(sockFd, &buf)) == -1){
                exit(EXIT_FAILURE);
            }
            printf("Message received: %s\n", buf);
            free(buf);
        }
    }
    return 0; //exit cleanly
}
