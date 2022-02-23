#include "ExecuteRequest.hpp"
#include <cstring>

ExecuteRequest::ExecuteRequest() : _status_code(0)
{}

ExecuteRequest::ExecuteRequest(ExecuteRequest &cpy)
{
    *this = cpy;
}

ExecuteRequest &
ExecuteRequest::operator=(ExecuteRequest const& src)
{
    (void)src;
    return *this;
}

ExecuteRequest::~ExecuteRequest(void)
{
}

bool
ExecuteRequest::isAllowedMethod(std::string const &method, std::vector<std::string> method_allowed) const
{
	for (size_t i = 0; i < method_allowed.size(); i++)
	{
		if (method == method_allowed[i])
			return true;
	}
	return false;
}

bool
ExecuteRequest::isMultipartProcessing(Ticket &ticket) const
{
	std::string content_type = ticket.getRequest().get_header_value("Content-Type");
		return true;
	return false;
}

void
ExecuteRequest::processMultipart(Ticket &ticket)
{
	std::string content_type = ticket.getRequest().get_header_value("Content-Type");
	size_t xpos = content_type.find(MULTIPART);
	if (xpos != 0)
		throw std::exception();
	std::string key = content_type.substr(std::string(MULTIPART).size());
	std::string const &body = ticket.getRequest().getBody();	
	(void)body;
}

int
ExecuteRequest::getStatusCode(void) const
{
    return _status_code;
}

void
ExecuteRequest::setStatusCode(int status_code)
{
    this->_status_code = status_code;
}

bool
ExecuteRequest::isImplemented(std::string const& method) const
{
    for (int i = 0; i < HTTP_METHOD_NOT_IMPLEMENTED_NB; i++)
    {
        if (method == method_not_implemented[i])
            return false;
    }
    return true;
}

bool
ExecuteRequest::isValidRequest(Request const& request, ConfigServer const& config, ServerLocations const& location)
{
    bool valid = true;
    if (request.getStartLine().method_token.empty() || request.getStartLine().request_URI.empty()
    || request.getStartLine().http_version.empty() || request.get_header_value("Host").empty()
    || !request.isContentLengthCorrect() || !request.getValid())
    {
        _status_code = BAD_REQUEST;
        valid = false;
    }
    else if (!isImplemented(request.getStartLine().method_token))
    {
        _status_code = NOT_IMPLEMENTED;
        valid = false;
    }
    else if (request.getStartLine().http_version != "HTTP/1.1")
    {
        _status_code = VERSION_NOT_SUPPORTED;
        valid = false;
    }
    else if (static_cast<int>(request.getBody().size()) > config.getMaxBody())
    {
        _status_code = PAYLOAD_TO_LARGE;
        valid = false;
    }
    else if (!isAllowedMethod(request.getStartLine().method_token, location.getMethods()))
    {
        _status_code = NOT_ALLOWED;
        valid = false;
    }
    else if (request.getStartLine().request_URI.size() > MAX_URI_SIZE)
    {
        _status_code = URI_TO_LONG;
        valid = false;
    }
    return valid;
}

std::string
ExecuteRequest::buildBodyPath(ConfigServer const &config, std::string const& root)
{
    int error_code = 0;
    std::stringstream ss;
    for (size_t i = 0; i < config.getErrorPages().errorCodes.size(); i++)
    {
        ss << config.getErrorPages().errorCodes[i];
        ss >> error_code;
        ss.clear();
        if (error_code == _status_code){
            std::stringstream code_string;
            code_string << _status_code;
            return root + config.getErrorPages().path + code_string.str() + ".html";
        }
    }
    return std::string();
}

std::string
ExecuteRequest::autoindexPath(void) const
{
    return std::string("./autoindex.html");
}

std::string
ExecuteRequest::getRedirected(ServerLocations const& location, Response &response)
{
    _status_code = MOVED_PERMANTLY;
    response.setHeader("Location", location.getRedir().to);
    return std::string();
}

std::string
ExecuteRequest::matchIndex(ServerLocations const &location, ConfigServer const &config, std::ifstream &ressource)
{
    std::string uri;    
    for (size_t i = 0; i < location.getIndex().size(); i++)
    {
        uri = location.getRoot() + "/" + location.getIndex()[i];
        ressource.open(uri.c_str(), std::ifstream::in);
        if (ressource.is_open())
        {
            _status_code = OK;
            ressource.close();
            return uri;
        }
    }
    if (ressource.is_open() == false)
        _status_code = NOT_FOUND;
    ressource.close();
    return buildBodyPath(config, location.getRoot());
}

std::string
ExecuteRequest::getMethod(std::string const& URI, ConfigServer const& config, ServerLocations const& location)
{
    if (URI[URI.size() - 1] == '/' && location.getAutoIndex() == 1)
    {
        _status_code = OK;
        return autoindexPath();
    }
    std::ifstream ressource;
    std::string uri = location.getRoot() + URI;
    if (URI == location.getpath())
        return matchIndex(location, config, ressource);
    else
        ressource.open(uri.c_str(), std::ifstream::in);
    if (ressource.is_open() && uri[uri.size() - 1] != '/')
    {
        _status_code = OK;
        ressource.close();
        return uri;
    }
    else
        _status_code = NOT_FOUND;
    ressource.close();
    return buildBodyPath(config, location.getRoot());
}

std::string
ExecuteRequest::deleteMethod(std::string const& URI, ConfigServer const& config, ServerLocations const& location)
{
    std::string uri = location.getRoot() + URI;
    std::string deleted_path("./trash/");
    std::ifstream in(uri.c_str(), std::ios::in | std::ios::binary);
    if (in)
    {
        std::ofstream out(deleted_path.append(uri).c_str(), std::ios::out | std::ios::binary);
        out << in.rdbuf();
        std::remove(uri.c_str());
        _status_code = OK;
    }
    else
        _status_code = NOT_FOUND;
    return buildBodyPath(config, location.getRoot());
}

void
ExecuteRequest::fillMethodNotImplemented(void)
{
    method_not_implemented[0] = "HEAD";
    method_not_implemented[1] = "PUT";
    method_not_implemented[2] = "CONNECT";
    method_not_implemented[3] = "OPTIONS";
    method_not_implemented[4] = "TRACE";
    method_not_implemented[5] = "PATCH";
}

std::string
ExecuteRequest::postMethod(std::string const &URI, ConfigServer const &config,
					ServerLocations const& location)
{
	(void)config;
	(void)location;
	std::cout << "POST METHOD" << std::endl;
    std::string uri = "./" + URI;
	//check multipart marks in headers
	//if (isMultipartProcessing(ticket))
	//	processMultipart(ticket);
	
	//ticket.getRequest();
	//if multipart :
	//	process multipart
    //return buildBodyPath(ticket.get, location.getRoot());
	return "OK"; //TODO not OK
}

#define FGET_SIZE 42

std::string
ExecuteRequest::execCgi(Request const &request,
							std::string const &query,
							ConfigServer const &config,
							ServerLocations const& location)
{	
	std::cout << "------------------New Cgi handling------------------" << std::endl;
	(void)config; // TODO
	std::string ressource = location.getRoot() + request.getStartLine().request_URI;
	CgiHandler handler(request, location.getCgiPath(), ressource, query);
	handler.sendCgi();
	handler.getCgiResponse();
	std::cout << "New Cgi response obtained" << std::endl;

	char line[FGET_SIZE + 1];
    bzero(line, FGET_SIZE + 1);
    std::string cgi_response;
	while (fgets(line, FGET_SIZE, handler.getCgiResponse()))
    {
        cgi_response += std::string(line);
    }
    std::cout << "OUR CGI RESPONSE :" << cgi_response << std::endl;
	return cgi_response;
}

std::string ExecuteRequest::method_not_implemented[HTTP_METHOD_NOT_IMPLEMENTED_NB];
