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

#define BUFFER_SIZE 50000

vector<string> parseURL(string url);
void writeToFile(char buffer[], string fileName);

int main(int argc, char *argv[])
{
    vector<string> parsedURL; // parsedURL[0] = hostname, parsedURL[1] = port, parsedURL[2] = resource name
    HttpRequest request;

    if (argc <= 1){
        cout << "Not enough arguments in function call" << endl;
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        parsedURL = parseURL(argv[i]);
        request.setMethod("GET");
        request.setResource(parsedURL[2]);

        string hostname = parsedURL[0];
        string portNo = parsedURL[1];
        string fileDir = parsedURL[2];
        
        struct addrinfo hints, *serverInfo;
        int status, sockFD;
        // struct sockaddr_storage their_addr;
        // socklen_t addr_size;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE; // use my IP



        // DNS lookup and fills out the serverInfo struct
        if ((status = getaddrinfo(hostname.c_str(), portNo.c_str(), &hints, &serverInfo)) != 0)
        {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
            return 1;
        }
        
        //returns a socket file descriptor to be used in later system calls
        if ((sockFD = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol)) < 0){
            perror("socket"); 
            exit(EXIT_FAILURE); 
        }

        if (connect(sockFD, serverInfo->ai_addr, serverInfo->ai_addrlen) < 0){
            perror("connect"); 
            exit(EXIT_FAILURE); 
        }

        // send data to connection
        if (send(sockFD, request.encode().c_str(), request.encode().length(), 0) < 0){
            perror("send"); 
            exit(EXIT_FAILURE); 
        } 

        //read the message into a buffer
        // recv will block until there is some data to read
        char buffer[BUFFER_SIZE] = {};
        if (recv(sockFD, buffer, BUFFER_SIZE, 0) < 0){
            perror("recv"); 
            exit(EXIT_FAILURE); 
        }
        string temp(buffer);
        int space = temp.find(" ")+1;
        temp.erase(0, space);
        string statusCode = temp.substr(0, temp.find(" "));
        if (statusCode == "200")
            writeToFile(buffer, fileDir);
        else if (statusCode == "404")
            cout << endl << "404 Not Found" << endl;
        

        close(sockFD);

    }

    return 0;

}


// returns a vector containing host, port and resource name
vector<string> parseURL(string url)
{
    vector<string> output;
    url = url.substr(7, url.length() - 1); // remove http://

    size_t found = url.find_first_of(":");
    string host = url.substr(0, found);
    output.push_back(host);
    url = url.erase(0,found);

    size_t found1 = url.find_first_of("/");
    string port =url.substr(1,found1-1);
    output.push_back(port);
    url = url.erase(1,found1-1);

    string resource =url.substr(2);;
    output.push_back(resource);

    return output;
}


void writeToFile(char buffer[], string fileName){

    // Parse out the body of the http response
    string str(buffer);
    string messageBody = str.substr(str.find("\n\n")+1, str.length());

    ofstream outfile ("client_" + fileName);

    if( !outfile )
        cout << endl <<  "Opening file failed" << endl;
    else{
        outfile << messageBody;
        outfile.close();
    }
    return;
}