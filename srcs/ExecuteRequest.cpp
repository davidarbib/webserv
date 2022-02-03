#include "ExecuteRequest.hpp"

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
    else if (static_cast<int>(request.getBody().size()) > config.getMaxBody())
    {
        _status_code = PAYLOAD_TO_LARGE;
        valid = false;
    }
    else if (!isAllowedMethod(request.getStartLine().method_token, config.getLocations()[0].getMethods()))
    {
        _status_code = NOT_ALLOWED;
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
ExecuteRequest::getMethod(std::string const& URI, ConfigServer const& config)
{
    std::string uri = config.getLocations()[0].getRoot() + URI;
    std::cout << "URI :" << uri << std::endl;
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
ExecuteRequest::deleteMethod(std::string const& URI, ConfigServer const& config)
{
    std::string uri = config.getLocations()[0].getRoot() + URI;
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