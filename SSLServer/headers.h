#ifndef HEADERS_H
#define HEADERS_H



// Defines
#define DEBUG
#undef DEBUG

// General libs
#include <iostream>
#include <errno.h>                      // Needed to check errors in C-functions
#include <string.h>                     // Needed to transform error into readable state
#include <sstream>                      // Needed to properly turn raw data into hex
#include <iomanip>                      // Needed to properly turn raw data into hex
#include <fstream>                      // Needed to properly read public key

// Server libs
#include <sys/socket.h>                 /// Main posix-socket library
#include <sys/types.h>                  /// Main posix-socket library
#include <unistd.h>                     /// Needed for close()
#include <fcntl.h>                      /// Needed to make sockets non-block
#include <netinet/in.h>                 /// Needed for proper ip and port reading

// Parser part
#include <libxml++/libxml++.h>          /// Main xml-parser library
#include <libxml2/libxml/parser.h>      /// Needed to clean up memory partially

// Containers and iterators
#include <string>
#include <list>

// Security part
#include <stdio.h>                      /// Needed to use RSA functions
#include <memory>                       /// Needed to properly work with OpenSSL ptr's
#include <openssl/rsa.h>                /// Main OpenSSL libraries
#include <openssl/md5.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/bn.h>

#endif // HEADERS_H
