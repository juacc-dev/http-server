# HTTP server

This is a project I've done for learning about an HTTP 1.1 and the Linux socket
API (with the more modern `getaddrinfo()`). Thanks a lot to [Beej's guide to network programming](https://beej.us/guide/bgnet/html/split/index.html).
Also, I've found the [Lwan web server](https://lwan.ws/), which has lots of
very cool things in it (like a coroutine system for concurrency).

This project is very much work in progress and so far only implements GET
requests, but it works! There are some obvious security concerns and a lot of
refactoring remains to be done. I'm also planning to add Lua support for
configuring the server.
