
#include <string>
using namespace std;

class HttpRequest {
    private:
        string method;
        string url;
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
        void setUrl(string url){
            this->url = url;
        }
        string getUrl(){
            return url;
        }
        void setBody(string body){
            this->body = body;
        }
        string getBody(){
            return body;
        }
        
        string encode(){
            return  getMethod() + " " 
            + getUrl() + " "
            + "HTTP/1.0\n";
        }
};