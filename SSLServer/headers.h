#ifndef HEADERS_H
#define HEADERS_H

// General libs
#include <iostream>
#include <errno.h>          // Needed to check errors in C-functions
#include <string.h>         // Needed to transform error into readable state
#include <stdlib.h>

// Server libs
#include <sys/socket.h>     /// Main posix-socket library
#include <sys/types.h>      /// Main posix-socket library
#include <unistd.h>         /// Needed for close()
#include <fcntl.h>          /// Needed to make sockets non-block
#include <netinet/in.h>     /// Needed for proper ip and port reading

/*
// General libs
#include <string.h>
#include <fstream>
#include <exception>
#include <iomanip>
#include <sstream>

// Containers and iterators
#include <string>
#include <list>
#include <iterator>

// Parser part
#include <libxml++/libxml++.h>

// Security part
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
*/

#endif // HEADERS_H
