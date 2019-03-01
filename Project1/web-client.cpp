#include "HttpRequest.h"
#include "HttpResponse.h"
#include <iostream>
#include <sstream>

// web-client should be called as
// ./web-cient [URL] [URL] ...
int main(int argc, char* argv[])
{

  HttpRequest request;

  request.setMethod("GET");
  request.setUrl(argv[1]);
  request.setBody("Body");
  std::cout << request.encode() << endl << endl;


  HttpResponse response;

  response.setStatus(200);
  response.setStatusMessage("OK");
  response.setBody("BODY of response");

  std::cout << response.encode();


}
