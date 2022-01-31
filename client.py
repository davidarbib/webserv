import socket

HOST = '127.0.0.1'
CRLF = '\r\n'
PORT = 8003
RECVSIZE = 1024

def createMessage():
    message = 'GET / HTTP/1.1'
    message += CRLF
    message += 'Host : webserv.com'
    message += CRLF
    message += CRLF
    return message

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((HOST, PORT))
print('connected to ' + HOST + ':' + str(PORT))
client.send(bytes(createMessage(), 'ascii'))
response = client.recv(RECVSIZE)
print(response)
client.close()

#create connection
#send data
#receive and prompt response
