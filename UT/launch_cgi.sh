#!/bin/zsh
echo "Aloha" \
| env SCRIPT_NAME=index.php \
		PATH_INFO=/home/daav/webserv/UT/index.php \
		PATH_TRANSLATED=/home/daav/webserv/UT/index.php \
		CONTENT_LENGTH=12 \
		REQUEST_METHOD=POST \
		REDIRECT_STATUS=200 \
/usr/bin/php-cgi 
