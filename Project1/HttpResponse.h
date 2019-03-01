#include <string>
using namespace std;

class HttpResponse{
    private:
        string body;
        string header;
        string statusMessage;
        // 200 = OK
        // 400 = Bad request
        // 404 = Not found
        int status;

    public:
        HttpResponse(){
        }
        string getBody() {
            return body;
        }
        void setBody(string body){
            this->body = body;
        }
        string getHeader(){
            return header;
        }
        void setHeader(string header){
            this->header = header;
        }
        string getStatusMessage(){
            return statusMessage;
        }
        void setStatusMessage(string message){
            this->statusMessage = message;
        }
        int getStatus(){
            return status;
        }
        void setStatus(int status){
            this->status = status;
        }
        
        string encode(){
            return "HTTP/1.0 " + 
                to_string(status) + " " 
                + statusMessage + "\n"
                + "Content-Type: text/plain\n"   
                + "Content-Length: 12\n"
                + "\nHello world!";
        }
};