#ifndef EXECUTE_REQUEST_HPP
# define EXECUTE_REQUEST_HPP

#include "Response.hpp"
#include "Request.hpp"
#include "Ticket.hpp"
#include "ConfigServer.hpp"
#include "CgiHandler.hpp"
#include <fstream>
#include <iostream>
#include <ios>
#include <cstdio>

#define HTTP_METHOD_NOT_IMPLEMENTED_NB 6

#define MULTIPART "multipart/form-data; boundary="

class ExecuteRequest
{

    private:
        int						_status_code;
		std::map<fd_t, Request> _continue_requests;

		bool
		isAllowedMethod(std::string const &method, std::vector<std::string> method_allowed) const;

        std::string
        autoindexPath(void) const;

        std::string
        matchIndex(ServerLocations const &location, ConfigServer const &config, std::ifstream &ressource);

		bool
		isMultipartProcessing(Ticket const &ticket) const;
		
		void
		processMultipart(Ticket const &ticket);

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
        buildBodyPath(ConfigServer const &config, std::string const& root);

        std::string
        getRedirected(ServerLocations const& location, Response &response);

        // get / delete / post exec

        std::string
        getMethod(std::string const &URI, ConfigServer const &config, ServerLocations const& location);

        std::string
        deleteMethod(std::string const &URI, ConfigServer const &config, ServerLocations const& location);

        std::string
        postMethod(std::string const &URI, ConfigServer const &config,
					ServerLocations const& location, Ticket const& ticket);

		std::string
		execCgi(Request const &request, std::string const &query,
				ConfigServer const &config, ServerLocations const& location);

		std::string
		continueGeneration(Ticket const &ticket);
};

#endif
