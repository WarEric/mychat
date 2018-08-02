# This is a simple chat tool using tcp and udp.
The reason why I do it is just for enhancing my socket code ability. It's a central model of client and service structure. First, by using the tcp protocol, client establish a connection to a central server and acquire a local connect udp sockfd. After that, users need to upload their client udp address info (ip address and port) and account info (name and password)  from their client to the server. The server will record client address info unless the account info is illegal. Second, users can upload their words through udp protocol, which will be broadcast by udp protocol to user that signed in the central service.

### This project is DEVELOPING, NOT finished, DON'T use it.
