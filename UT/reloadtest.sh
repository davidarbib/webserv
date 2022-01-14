#!/bin/zsh
rm -f testcgi
clang++ -Wall -Wextra -Werror -g3 \
	../srcs/Request.cpp \
	../srcs/Response.cpp \
	../srcs/CgiHandler.cpp \
	../srcs/AHttpMessage.cpp \
	../srcs/safe_wrappers.cpp \
	testcgi.cpp \
-o testcgi -I../includes
chmod 755 testcgi
./testcgi
