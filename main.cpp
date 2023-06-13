#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>

class MyRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
    {
        std::string method = request.getMethod();
        std::string uri = request.getURI();

        // Check if the requested resource exists
        bool resourceExists = checkResourceExists(uri);

        if (resourceExists)
        {
            // Resource exists, send the response
            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html");

            std::ostream& ostr = response.send();
            ostr << "<html>";
            ostr << "<head><title>Simple HTTP Server</title></head>";
            ostr << "<body>";
            ostr << "<h1>Welcome to the Simple HTTP Server With Poco!!!</h1>";
            ostr << "<p>This is a response from the server.</p>";
            ostr << "<p>Request Method: " << method << "</p>";
            ostr << "<p>Request URI: " << uri << "</p>";
            ostr << "</body>";
            ostr << "</html>";
        }
        else
        {
            // Resource does not exist, send a 404 response with custom 404 page
            response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND, "Not Found");
            response.setChunkedTransferEncoding(true);
            response.setContentType("text/html");

            std::ostream& ostr = response.send();
            ostr << "<html>";
            ostr << "<head><title>404 Not Found</title></head>";
            ostr << "<body>";
            ostr << "<h1>404 Not Found</h1>";
            ostr << "<p>The requested resource was not found.</p>";
            ostr << "</body>";
            ostr << "</html>";
        }
    }

private:
    bool checkResourceExists(const std::string& uri)
    {
        // 在这里根据URI检查资源是否存在
        // 根据您的应用程序需求和上下文实现资源存在性检查的逻辑
        // 返回true表示资源存在，返回false表示资源不存在

        // 示例：假设只有根目录"/"的资源存在，其他资源都不存在
        if (uri == "/")
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest& request)
    {
        return new MyRequestHandler;
    }
};

class MyServerApp : public Poco::Util::ServerApplication
{
protected:
    int main(const std::vector<std::string>&)
    {
        Poco::Net::ServerSocket socket(8080);
        Poco::Net::HTTPServerParams::Ptr params(new Poco::Net::HTTPServerParams);
        params->setMaxThreads(10); // 设置线程数为10
        Poco::Net::HTTPServer server(new MyRequestHandlerFactory, socket, params);
        server.start();
        waitForTerminationRequest();
        server.stop();
        return Poco::Util::Application::EXIT_OK;
    }
};

int main(int argc, char** argv)
{
    MyServerApp app;
    return app.run(argc, argv);
}

