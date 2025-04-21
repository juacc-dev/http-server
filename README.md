# HTTP server

This is an HTTP 1.1 server that uses the Linux socket API (with the more modern
`getaddrinfo()`). The project is very much work in progress and so far only
implements GET requests. 

I'm planning to add Lua support for configuring the server.
