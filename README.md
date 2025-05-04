# HTTP server

This is a project I've done for learning about an HTTP 1.1 and the Linux socket
API (with the more modern `getaddrinfo()`). Thanks a lot to [Beej's guide to network programming](https://beej.us/guide/bgnet/html/split/index.html).
Also, I've found the [Lwan web server](https://lwan.ws/), which has lots of
very cool things in it (like a coroutine system for concurrency).

This project is very much work in progress and so far only really handles GET
requests. There are some obvious security concerns and a lot of refactoring
remains to be done, but it works!

I'm also planning to add Lua support for configuring the server, though there
are a few (very few) things implemented already.

## Dependencies

This program only depends on LuaJIT (5.1) and
[escape](https://github.com/juacc-dev/escape), a library I've made for printing
ANSI escape sequences more easily. It's used for coloring the output. A clear
limitation of that library is that there is no way of disabling it neither at
compile time nor at run time, so it's needed to compile the server.
