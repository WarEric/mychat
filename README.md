# This is a simple chat tool using tcp and udp.
The reason why I do it is just for enhancing my socket code ability. It's a central model of client and service structure. First, by using the tcp protocol, client establish a connection to a central server and acquire a local connect udp sockfd. After that, users need to upload their client udp address info (ip address and port) and account info (name and password)  from their client to the server. The server will record client address info unless the account info is illegal. Second, users can upload their words through udp protocol, which will be broadcast by udp protocol to user that signed in the central service.

# ADVANTAGE
1. You can choose anyone as a server, a server represent a chat family. （a mechaine can run only one server because it will run out of the fixed port）
2. It still works well in a local area network or an internal network environment.
3. What we need is just a server's ipv4 address, dns and others things are not necessary because we depend on ipv4 address only, which is used to find the central server.
4. If your machaine's os system is unix like, I think it could works.(I haven't test this program in other systems except my ubuntu18.0 system.)
5. The server can decide the access of a client by it's account info which contians name and passwd.(server use the default configure file shoadows to make a decision, the configure file can be changed dynamically)

# HOW TO USE ?
1. Download this code.
2. Run make. (it will generate two programs: client and server)
3. Chose a mechaine as a central server.

# SERVER (only one)
1. Run ./server
2. Lookup this machaine's ipv4 address xxx.xxx.xxx.xxx and inform it to clients which want to join in. 

# CLINET
1. Run ./client xxx.xxx.xxx.xxx
2. Input your account name and passwd
3. If you succeed, you can see the welcome information. If not, please follow the guideline. 

# CHAT
- People who attended the chat family can communicate with others by typing words, this message will be broadcast to clients which are in it.
- You can choose to leave present chatting or shutdown this program by using Ctrl+c.

# ATENTIONS
- I test it in my ubuntu18.0(server) kvm system only.
- If there is any problem or bug, please let me know. I will try my best to fix it as soon as possible.
- I would be very grateful if you fix one or more bugs and willing to contribute it generously.
