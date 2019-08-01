#include "server.h"
#include "slave.h"
#include "parser.h"
#include "signer.h"


    #define SIGNER_DEBUG
    //#undef SIGNER_DEBUG

    #define SERVER_DEBUG
    //#undef SERVER_DEBUG

int main() {
#ifdef SIGNER_DEBUG
    Sorokin::Signer _Signer;
    _Signer.setKeyLoc("/home/student/C++/SSLServer/pub_key.key", "/home/student/C++/SSLServer/priv_key.key");
    _Signer.createKeyPair(1024);
    if (_Signer.getPwd()) {
        return -1;
    }
#endif

#ifdef SERVER_DEBUG
    Sorokin::Server _Server;
    if (_Server.setSocket()) {
        return -1;
    }

    if (!_Server.setUpListener()) {
        Sorokin::Slave _Slave;
        if (!_Server.getConnection(_Slave.accessSocket())) {
            std::string bufStr;
            if (!_Slave.recvString(bufStr)) {
                Sorokin::Parser _Parser;
                if (!_Parser.loadDocument(bufStr)) {
                    bufStr.clear();

                    std::list<std::string> bufList;
                    _Parser.parseDocument(bufList);

                    std::string pubKey = _Signer.getPubKey();

                    std::list<std::string> signedBufList;
                    for (const auto& iter : bufList) {
                        signedBufList.push_back(_Signer.signString(iter));
                    }
                    bufList.clear();

                    bufStr = _Parser.rebuildDocument(signedBufList, pubKey);
                }

                _Parser.unloadDocument();
                _Slave.sendString(bufStr);
            }

            _Slave.closeConnection();
            _Slave.deleteSocket();
        }

        _Server.closeConnection();
    }

    _Server.deleteSocket();
#endif

    return 0;
}

