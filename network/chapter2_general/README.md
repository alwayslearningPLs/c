# Sockets

A socket is one *endpoint* of a communication link between systems.

There are different socket APIs (Application Programming InterfaceS).

*Berkeley sockets* (released in 1983 with 4.3BSD UNIX).

## Two types of sockets

Sockets come in two basic types:

- *Connection-oriented* (TCP or Transmission Control Procotol)
- *Connectionless* (UDP or User Datagram Protocol)

## Functions

- `socket()`: creates and initializes a new *socket*.
- `bind()`: associates a *socket* with a particular local *IP address* and port number.
- `listen()`: is used on the server to cause a TCP socket to listen for new connections.
- `connect()`: is used on the client to set the remote address and port. In the case of TCP, it also establishes a connection.
- `accept()`: is used on the server to create a new socket for an incomming TCP connection.
- `send()`: Send data with a socket.
- `recv()`: Receive data with a socket.
- `sendto()`: Send data from sockets without a bound remote address.
- `recvfrom()`: Receive data from sockets without a bound remote address.
- `close()`: is used to close a socket.
- `shutdown()`: is used to close one side of a TCP connection. It is useful to ensure an orderly connection teardown.
- `select()`: is used to wait for an event on one or more sockets.
- `getnameinfo()`: provide protocol-independent manner of working with hostnames.
- `getaddrinfo()`: provide protocol-independent manner of working with addresses.
- `setsockopt()`: is used to change some socket options.
- `fcntl()`: is used to get and set some socket options.

## Reference

- POSIX means Portable Operating System Interface
- The terms *Berkeley sockets*, *BSD sockets*, *Unix sockets* and *POSIX* sockets are the same(?).

