#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>
#include <sstream>

#define NUM_ARG 4

// Program call:  web-server [hostname] [port] [file-dir]
int main(int argc, char* argv[])
{
  if (argc < NUM_ARG){
    std::cout << "Bad function call" << std::endl;
    return 0;
  }

  int status;
  

  return 0;
} 