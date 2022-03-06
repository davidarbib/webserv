#include "ExecuteRequest.hpp"
#include <cstring>

ExecuteRequest::ExecuteRequest() : _status_code(0)
{
}

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
ExecuteRequest::isMultipartProcessing(Ticket const &ticket) const
{
	std::string content_type = ticket.getRequest().getHeaderValue("Content-Type");
	size_t xpos = content_type.find(MULTIPART);
	if (xpos != 0)
		return false;
	return true;
}

void
ExecuteRequest::processMultipartHeaders(std::string &headers_part,
											t_headers *headers)
{
	std::map<std::string, std::string> mp_headers;

	size_t i = 0;
	while (i < headers_part.size())
	{
		size_t colon_pos = headers_part.find(":");
		size_t endline_pos = headers_part.find(CRLF_S);
		
		mp_headers[headers_part.substr(0, colon_pos)] =
		headers_part.substr(colon_pos + 1,  endline_pos - (colon_pos + 1));
		headers_part = headers_part.substr(endline_pos + 2,
										headers_part.size() - (endline_pos + 2));
		i = endline_pos + CRLF_LEN;
	}
	
	//for (std::map<std::string, std::string>::iterator it = mp_headers.begin();
	//		it != mp_headers.end();
	//		it++)
	//{
	//	std::cout << "key : " << it->first << ", value : " << it->second;
	//	std::cout << std::endl;
	//}

	std::string fname_mark("filename=");
	std::string content_disposition = mp_headers["Content-Disposition"];
	size_t fpos = content_disposition.find(fname_mark);
	size_t quote_pos = fpos + fname_mark.size();
	std::string fname = content_disposition.substr(quote_pos + 1,
			content_disposition.size() - (quote_pos + 1));
	fname.assign(fname.substr(0, fname.find("\"")));
	headers->filename = fname;
	headers->content_type = mp_headers["Content-Type"]; 
	headers->charset = mp_headers["charset"]; 
}

void
split_parts(std::vector<std::vector<char> > &v_parts, Ticket const &ticket)
{
	std::string content_type = ticket.getRequest().getHeaderValue("Content-Type");
	size_t xpos = content_type.find(MULTIPART);
	if (xpos != 0)
		throw std::exception();

	std::string key = "--";
	key += content_type.substr(std::string(MULTIPART).size());
	std::string last_boundary = key + '-' + '-';

	AHttpMessage::body_type::const_iterator body_cursor =
		ticket.getRequest().getBody().begin();
	AHttpMessage::body_type::const_iterator body_end =
		ticket.getRequest().getBody().end();
	AHttpMessage::body_type::const_iterator multipart_end =
		search(body_cursor, body_end, last_boundary.begin(), last_boundary.end());
	AHttpMessage::body_type::const_iterator it = search(body_cursor, body_end, key.begin(), key.end());

	while (it != multipart_end)
	{
		it += key.size();
		it += CRLF_LEN;

		AHttpMessage::body_type::const_iterator next_boundary =
			search(it, body_end, key.begin(), key.end());
		std::vector<char> part;
		part.assign(it, next_boundary);
		v_parts.push_back(part);
		it = next_boundary;
	}
}

std::string
cutExtension(std::string filename, std::string &extension)
{
	size_t dotpos = filename.find_last_of(".");
	if (dotpos == std::string::npos)
	{
		extension = "";
		return filename;
	}
	extension = filename.substr(dotpos + 1);
	filename = filename.substr(0, dotpos);
	return filename;
}

std::string
add_timestamp(std::string &filename)
{
	std::stringstream stream;
	std::string s;
	std::string extension;

	 std::time_t tm = time(NULL); //time since epoch
	 stream << static_cast<long>(tm) << std::endl;
	 stream >> s;
	 filename = cutExtension(filename, extension);
	 filename = filename + s;
	 if (extension.size() != 0)
			 filename += "." + extension;
	 return filename;
}

void
ExecuteRequest::processMultipart(Ticket const &ticket, std::string const& path)
{
	std::vector<std::vector<char> > v_parts;

	std::string end_section(CRLFCRLF_S);
	split_parts(v_parts, ticket);
	//std::cout << "----debug split parts-----" << std::endl;
	//for (std::vector<std::vector<char> >::iterator it = v_parts.begin();
	//		it != v_parts.end(); it++)
	//{
	//	std::cout << "----New file----"	<< std::endl;
	//	for (std::vector<char>::iterator it2 = it->begin();
	//			it2 != it->end(); it2++)
	//		std::cout << *it2;
	//	std::cout << std::endl;
	//}
	//std::cout << "--------------------" << std::endl;
	for (std::vector<std::vector<char> >::iterator part = v_parts.begin();
			part != v_parts.end(); part++)
	{
		std::string header_part;
		t_headers headers;
		AHttpMessage::body_type::iterator headers_end =
			search(part->begin(), part->end(), end_section.begin(), end_section.end());

		header_part.assign(part->begin(), headers_end);
		processMultipartHeaders(header_part, &headers);
		
		//std::cout << "content_type" << headers.content_type << std::endl;
		//std::cout << "filename" << headers.filename << std::endl;
		//std::cout << "charset" << headers.charset << std::endl;

		//std::cout << "--------body------------" << std::endl;

		//for (AHttpMessage::body_type::iterator it = headers_end + CRLF_LEN * 2;
		//		it != part->end(); it++)
		//	std::cout << *it;
		//std::cout << "------------------------" << std::endl;
        add_timestamp(headers.filename);
        if (access(headers.filename.c_str(), F_OK) == 0)
	        headers.filename += "_";
		headers.filename = path + headers.filename;
		SmartFile file(headers.filename, "w");
		int body_size = part->end() - (headers_end + CRLF_LEN * 2); 
		char *buf = new char [body_size];
		int i = 0;
		for (AHttpMessage::body_type::iterator it = headers_end + CRLF_LEN * 2;
				it != part->end(); it++)
			buf[i++] = *it;
		//buffer with body
		//write buffer on file
		file.puts(buf, body_size);
		delete [] buf;

	}	
	//if (isItEndSection(it))
	//{
	//	if (flags & HEADERS)
	//		flags |= BODY;
	//}
	//else if (isItEndLine(it))
	//{
	//	(void)it;
	//}

	//detect end line or end section
	//if end line
	//	parse headers
	//if end section
	//  store body on disk
	//

	//std::string const &body = ticket.getRequest().getBody();	
	//(void)body;
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
    || request.getStartLine().http_version.empty() || request.getHeaderValue("Host").empty()
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
ExecuteRequest::buildBodyPath(ConfigServer const &config)
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
            return config.getErrorPages().path + code_string.str() + ".html";
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
ExecuteRequest::getMethod(std::string const& uri, ConfigServer const& config, ServerLocations const& location, std::string const &resolved_uri)
{
    if (uri[uri.size() - 1] == '/' && location.getAutoIndex() == 1)
    {
       _status_code = OK;
       return autoindexPath();
    }
    std::ifstream ressource;
    std::string complete_uri = location.getRoot() + uri;
    if (uri == location.getpath())
		complete_uri = resolved_uri;
    ressource.open(complete_uri.c_str(), std::ifstream::in);
    if (ressource.is_open() && complete_uri[complete_uri.size() - 1] != '/')
    {
        _status_code = OK;
        ressource.close();
        return complete_uri;
    }
    else
        _status_code = NOT_FOUND;
    ressource.close();
    return buildBodyPath(config);
}

std::string
ExecuteRequest::deleteMethod(std::string const& uri, ConfigServer const& config, ServerLocations const& location, std::string const &resolved_uri)
{
    if (uri[uri.size() - 1] == '/' && location.getAutoIndex() == 1)
    {
       _status_code = NOT_ALLOWED;
       return buildBodyPath(config);
    }
    std::string complete_uri = location.getRoot() + uri;
    if (uri == location.getpath())
        complete_uri = resolved_uri;
    std::string deleted_path("./trash/");
    std::ifstream in(complete_uri.c_str(), std::ios::in | std::ios::binary);
    if (in)
    {
        std::ofstream out(deleted_path.append(complete_uri).c_str(), std::ios::out | std::ios::binary);
        out << in.rdbuf();
        std::remove(complete_uri.c_str());
        _status_code = OK;
    }
    else
        _status_code = NOT_FOUND;
    return buildBodyPath(config);
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
ExecuteRequest::postMethod(std::string const &uri, ConfigServer const &config,
					ServerLocations const& location, Ticket const& ticket)
{
	(void)config;
    std::string complete_uri = location.getRoot() + uri;
	//check multipart marks in headers
	if (isMultipartProcessing(ticket))
	{
	      processMultipart(ticket, complete_uri);
		  _status_code = CREATED; 
		  return EMPTY_STR;
	}
	_status_code = NO_CONTENT;
	return EMPTY_STR;
}

AHttpMessage::body_type
ExecuteRequest::execCgi(Request const &request,
							std::string const &original_uri,
							std::string const &resolved_uri,
							std::string const &query,
							ServerLocations const& location,
							int index_page_idx)
{	
	std::string ressource;
	if (index_page_idx == -1)
		ressource = original_uri;
	else
		ressource = resolved_uri;
	ressource = location.getRoot() + ressource;
	CgiHandler handler(request, location.getCgiPath(), ressource, query);
	handler.sendCgi();
	handler.getCgiResponse();

	char line[FGET_SIZE + 1];
    bzero(line, FGET_SIZE + 1);
    AHttpMessage::body_type cgi_response;
	while (fgets(line, FGET_SIZE, handler.getCgiResponse()))
    {
		for (int i = 0; line[i] && i < FGET_SIZE; i++)
        	cgi_response.push_back(line[i]);
    }
	if (cgi_response.empty())
		throw std::exception();
	return cgi_response;
}

std::string
ExecuteRequest::continueGeneration(Ticket const &ticket)
{
	ticket.getConnection().expectFullBodyNextRequest();
	_continue_requests[ticket.getConnection().getSocketFd()]
		= ticket.getRequest();
	setStatusCode(CONTINUE);
	return std::string(EMPTY_STR);
}

std::string ExecuteRequest::method_not_implemented[HTTP_METHOD_NOT_IMPLEMENTED_NB];
