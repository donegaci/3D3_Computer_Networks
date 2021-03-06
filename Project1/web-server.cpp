#include "HttpRequest.h"
#include "HttpResponse.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <pthread.h>

#define BACKLOG 10       // how many pending connections queue will hold
#define BUFFER_SIZE 50000 // max message buffer size

vector<string> parseMessage(string buffer);
int getFile(string filename, string &file, int &fileLength);
void *socketThread(void *arg);

// Program call:  web-server [hostname] [portNo] [fileDir]
int main(int argc, char *argv[])
{

    char *hostname, *fileDir, *portNo;
    struct addrinfo hints, *serverInfo;
    int status, sockFD, new_sockFD;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    // Wrong number of arguments use default values
    if (argc < 4)
    {
        cout << "Using default values\n";
        hostname = (char *)"localhost";
        portNo = (char *)"4000";
        fileDir = (char *)".";
    }
    else
    {
        hostname = argv[1];
        portNo = argv[2];
        fileDir = argv[3];
    }

    // DNS lookup and fills out the serverInfo struct
    if ((status = getaddrinfo(hostname, portNo, &hints, &serverInfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    //returns a socket file descriptor to be used in later system calls
    sockFD = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

    // Bind to port that was passed to getaddrinfo
    // All ports below 1024 are RESERVED
    bind(sockFD, serverInfo->ai_addr, serverInfo->ai_addrlen);

    // listen for incoming connections
    if (listen(sockFD, BACKLOG) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t threadID[60];
    int i = 0;
    while (1)
    {
        // accept an incoming connection:
        addr_size = sizeof(their_addr);
        if ((new_sockFD = accept(sockFD, (struct sockaddr *)&their_addr, &addr_size)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //for each new client request, create a thread and assign the handler to process it
        if (pthread_create(&threadID[i], NULL, socketThread, &new_sockFD) != 0)
            printf("Failed to create thread\n");
    }

    return 0;
}

// Splits a string input by spaces and returns a vector of words
vector<string> parseMessage(string buffer)
{
    stringstream ss(buffer);
    istream_iterator<string> begin(ss);
    istream_iterator<string> end;
    vector<string> words(begin, end);
    copy(words.begin(), words.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    return words;
}

// Function takes as arguments the name of file, string to write output to and filelength
// returns an int 200, 400, 404
int getFile(string filename, string &file, int &fileLength)
{
    ifstream f;
    f.open("./" + filename);

    // check the file exists
    if (f)
    {
        stringstream sstr;
        sstr << f.rdbuf();
        file = sstr.str();
        fileLength = file.length();
        return 200;
    }
    else
    {
        cout << "404" << endl;
        return 404; // NOT FOUND
    }
}

void *socketThread(void *arg)
{
    int newSocket = *((int *)arg);

    //read the message into a buffer
    char buffer[BUFFER_SIZE] = {};
    recv(newSocket, buffer, BUFFER_SIZE, 0);

    vector<string> temp = parseMessage(buffer);
    string method, fileName;
    // HTTP request format
    // GET URL HTTP_V
    method = temp[0];
    fileName = temp[1];


    string f;
    int fileLength;
    if (method == "GET")
    {
        int status = getFile(fileName, f, fileLength);
        HttpResponse response;
        if (status == 200){
            // create response message
            response.setStatus(200);
            response.setStatusMessage("OK");
            response.setBody(f);
            // send response message
        }
        else if (status == 404){
            // create response message
            response.setStatus(404);
            response.setStatusMessage("Not Found");
            response.setBody(f);
        }
        send(newSocket, (response.encode()).c_str(), fileLength+20, 0);

        close(newSocket);
    }

}