

# ft_irc

##what is a socket ?

A socket is an endpoint for sending or receiving data across a network (like the Internet or local LAN).

It’s like a phone jack: two computers plug into it (one as client, one as server), and they talk to each other.

##Why Use Sockets?

Sockets let you build:

    Chat applications
    File transfer systems
    Multiplayer games
    Web servers
Without sockets, programs can’t communicate over the network

##Socket Model: Client & Server

    Role	    Responsibilities
    Server	    Waits for connections, receives & responds
    Client	    Connects to server, sends & receives messages
Imagine:

    The server is a restaurant.
    The client is a customer.
    The socket is the telephone.


There are two main socket types:
    TCP vs UDP

    Type	Protocol	Reliable?	Ordered?	Use Cases
    TCP	    Stream	    Yes	        Yes	        Chat, Web, FTP
    UDP	    Datagram	No	        No	        Gaming, Streaming


##Socket Lifecycle (TCP)

Server Side:

    socket() — Create socket
    bind() — Bind to IP & port
    listen() — Listen for connections
    accept() — Accept connection
    recv() / send() — Exchange data
    close() — Close connection

Client Side:

    socket() — Create socket
    connect() — Connect to server
    send() / recv() — Exchange data
    close() — Close socket

##how to create a socket ?

int socket(int domain, int type, int protocol);

the socket() function is used to create an endpoint for communication. It initializes a new socket that can be used for communication between processes, either locally or across a network.

###domain (or address family):

Specifies the communication domain (or address family) for the socket. It defines the type of protocol and address format used by the socket.

Common values:

AF_INET: IPv4 address family, used for internet communication.

AF_INET6: IPv6 address family, used for internet communication over IPv6.

AF_UNIX or AF_LOCAL: Unix domain sockets, used for communication between processes on the same machine (local IPC).

AF_PACKET: Used for accessing low-level networking, typically for raw socket programming.

###type:

Specifies the socket type or communication semantics.

Common values:

SOCK_STREAM: Stream socket, used for reliable, connection-oriented communication (e.g., TCP).

SOCK_DGRAM: Datagram socket, used for connectionless communication (e.g., UDP).

SOCK_RAW: Raw socket, which provides direct access to lower-layer protocols (e.g., ICMP, custom protocols).

SOCK_SEQPACKET: A socket type that provides reliable, ordered delivery of messages, like SOCK_STREAM, but preserves message boundaries.

###protocol:

Specifies the protocol to be used. Usually, you pass 0 here to allow the system to choose the appropriate protocol based on the domain and type specified.

For example, if you use AF_INET and SOCK_STREAM, the system will default to the TCP protocol, as TCP is the most common protocol for stream sockets.

For SOCK_DGRAM, the system will choose UDP.




how can i bind a socket to a specific port ?
how can i listen for incoming connections ?
how to handle a client connections ?

different types of sockets available ?
what are their use cases ?

The main parts of the project:

    Create all the necessary classes and methods for the project
    Create a socket and handle the server signals.
    Use the poll() function to check if an event has occurred.
    If the event is a new client, register it.
    If the event is data from a registered client, handle it.