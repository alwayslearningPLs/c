# Programming in Depth TCP

We have three different methods of handling TCP requests in a non-blocking manner:

- Using fork() to create another process to handle that request.
- Polling non-blocking sockets, so we have a list of sockets and we are looping across all of them watching for the state changes.
- Using select() function.

## Synchronous multiplexing with select()

Given a set of _sockets_, it can be used to block until any of the sockets in that set is ready to be _read from_. It can also be configured to return if a socket is ready to be _written_ to or if a socket has an _error_.

`int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)`

