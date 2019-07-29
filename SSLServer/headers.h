#ifndef HEADERS_H
#define HEADERS_H

#define DEBUG
// #undef DEBUG

// General libs
#include <iostream>
#include <errno.h>          // Needed to check errors in C-functions
#include <string.h>         // Needed to transform error into readable state

// Server libs
#include <sys/socket.h>     /// Main posix-socket library
#include <sys/types.h>      /// Main posix-socket library
#include <unistd.h>         /// Needed for close()
#include <fcntl.h>          /// Needed to make sockets non-block
#include <netinet/in.h>     /// Needed for proper ip and port reading

// Parser part
#include <libxml++/libxml++.h>
#include <libxml2/libxml/parser.h>

// Containers and iterators
#include <string>
#include <list>

/*
// General libs
#include <string.h>
#include <fstream>
#include <exception>
#include <iomanip>
#include <sstream>

#include <iterator>

// Security part
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
*/

#endif // HEADERS_H
