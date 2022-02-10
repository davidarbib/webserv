#include "ExecuteRequest.hpp"

ExecuteRequest::ExecuteRequest() : _status_code(0)
{
    initMethodList();
}

ExecuteRequest::ExecuteRequest(ExecuteRequest &cpy)
{
    initMethodList();
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

void
ExecuteRequest::initMethodList(void)
{
	this->_method_list[0] = "GET";
	this->_method_list[1] = "POST";
	this->_method_list[2] = "DELETE";
}

bool
ExecuteRequest::isAllowedMethod(std::string const &method) const
{
	for (int i = 0; i < METHOD_NB; i++)
	{
		if (method == this->_method_list[i])
			return true;
	}
	return false;
}

bool
ExecuteRequest::isValidMethod(std::string const &method) const
{
	for (size_t i = 0; i < method.length(); i++)
	{
		if (std::isupper(method[i]) == 0)
			return false;
	}
	return true;
}

bool
ExecuteRequest::isMultipartProcessing(Ticket const &ticket) const
{
	std::string content_type = ticket.getRequest().get_header_value("Content-Type");
		return true;
	return false;
}

void
ExecuteRequest::processMultipart(Ticket const &ticket)
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
ExecuteRequest::isValidRequest(Request const& request, ConfigServer const& config)
{
    bool valid = true;
    if (request.getStartLine().method_token.empty() || request.getStartLine().request_URI.empty()
    || request.getStartLine().http_version.empty() || request.get_header_value("Host").empty())
    {
        _status_code = BAD_REQUEST;
        valid = false;
    }
    else if (request.getStartLine().http_version != "HTTP/1.1")
    {
        _status_code = VERSION_NOT_SUPPORTED;
        valid = false;
    }
    else if ( static_cast<int>(request.getBody().size()) > config.getMaxBody())
    {
        _status_code = PAYLOAD_TO_LARGE;
        valid = false;
    }
    if (!valid)
        buildBodyPath();
    return valid;
}

std::string
ExecuteRequest::buildBodyPath(void)
{
    std::stringstream body_path;
    body_path << "./srcs/html/";
	body_path << _status_code;
	body_path << ".html";
    return body_path.str();
}

std::string
ExecuteRequest::getMethod(std::string const& URI)
{
    std::string uri = "./" + URI;
    std::ifstream ressource(uri.c_str());
    if (ressource)
    {
        _status_code = OK;
        return uri;       
    }
    else
        _status_code = NOT_FOUND;
    return buildBodyPath();
}

std::string
ExecuteRequest::deleteMethod(std::string const& URI)
{
    std::string uri = "./" + URI;
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
    return buildBodyPath();
}

std::string
ExecuteRequest::postMethod(std::string const& URI, Ticket const &ticket)
{
	std::cout << "POST METHOD" << std::endl;
    std::string uri = "./" + URI;
	//check multipart marks in headers
	if (isMultipartProcessing(ticket))
		processMultipart(ticket);
	
	//ticket.getRequest();
	//if multipart :
	//	process multipart
    return buildBodyPath();
}
