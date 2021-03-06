#ifndef EXECUTE_REQUEST_HPP
# define EXECUTE_REQUEST_HPP

#include "Response.hpp"
#include "Request.hpp"
#include "Ticket.hpp"
#include "ConfigServer.hpp"
#include "CgiHandler.hpp"
#include "parsing_tools.hpp"
#include "SmartFile.hpp"
#include <fstream>
#include <iostream>
#include <ios>
#include <cstdio>
#include <ctime>
#include <cctype>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define HTTP_METHOD_NOT_IMPLEMENTED_NB 6
#define CRLFCRLF_S "\r\n\r\n"
#define CRLF_S "\r\n"
#define CRLF_LEN 2

#define MULTIPART "multipart/form-data; boundary="

typedef struct s_headers
{
	std::string content_type;
	std::string filename;
	std::string charset;
} 				t_headers;


class ExecuteRequest
{

    private:
        int						_status_code;
		std::map<fd_t, Request> _continue_requests;

		bool
		isAllowedMethod(std::string const &method, std::vector<std::string> method_allowed) const;

        std::string
        autoindexPath(std::string const &uri) const;

		bool
		isMultipartProcessing(Ticket const &ticket) const;
		
		void
		processMultipart(Ticket const &ticket, std::string const &path);

		void
		processMultipartHeaders(std::string &headers_part, t_headers *headers);

    public:
        static std::string method_not_implemented[HTTP_METHOD_NOT_IMPLEMENTED_NB];
        ExecuteRequest(void);
        ExecuteRequest(ExecuteRequest &cpy);
        ExecuteRequest &
        operator=(ExecuteRequest const &src);
        virtual ~ExecuteRequest(void);

        static void
        fillMethodNotImplemented(void);

        int
        getStatusCode(void) const;

        void
        setStatusCode(int status_code);

        bool
        isValidRequest(Request const &request, ConfigServer const &config, ServerLocations const& location);

        bool
        isImplemented(std::string const &method) const;

        std::string
        buildBodyPath(ConfigServer const &config);

        std::string
        getRedirected(ServerLocations const& location, Response &response);

        // get / delete / post exec

        std::string
        getMethod(std::string const &uri, ConfigServer const &config, ServerLocations const& location, std::string const &resolved_uri);

        std::string
        deleteMethod(std::string const &uri, ConfigServer const &config, ServerLocations const& location, std::string const &resolved_uri);

        std::string
        postMethod(std::string const &uri, ConfigServer const &config,
					ServerLocations const& location, Ticket const& ticket);

	        AHttpMessage::body_type	
		execCgi(Request const &request, std::string const &original_uri,
				std::string const &resolved_uri, std::string const &query, ServerLocations const& location,
				int index_page_idx);

		std::string
		continueGeneration(Ticket const &ticket);
};

#endif
