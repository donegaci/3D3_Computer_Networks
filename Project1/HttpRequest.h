
#include <string>
using namespace std;

class HttpRequest {
    private:
        string method;
        string resource;
        string body;

    public:
        HttpRequest(){
        }
        void setMethod(string method){
            this->method = method;
        }
        string getMethod(){
            return method;
        }
        void setResource(string resource){
            this->resource = resource;
        }
        string getResource(){
            return resource;
        }
        void setBody(string body){
            this->body = body;
        }
        string getBody(){
            return body;
        }
        
        string encode(){
            return  getMethod() + " " 
            + getResource() + " "
            + "HTTP/1.0\n";
        }
};