clang++ -Wall -Wextra -Werror fakephp.cpp -o fakephp
sudo docker rm fake
sudo docker build -t fakephp:latest .
sudo docker run -ti -p 8003:8003 -p 8002:8002 -p 8001:8001 -p 80:80 -p 22:22 --name fake \
fakephp:latest
