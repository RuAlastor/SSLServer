#ifndef PARSER_H
#define PARSER_H

#include "headers.h"

namespace Sorokin {

    class Parser {

    public:
        Parser(const char* filename, std::list<std::string>* externalList) noexcept;
        ~Parser() noexcept;

        Parser() = delete;
        Parser(const Parser& other) = delete;
        Parser(Parser&& other) = delete;
        Parser& operator=(const Parser& other) = delete;
        Parser& operator=(Parser&& other) = delete;

        int loadDocument() noexcept(false);
        int parseDocument() noexcept(false);
        int rebuildDocument(const  std::list<std::string>& signatures, const std::string& publicKey) noexcept(false);

    private:
        enum ParserErrors { noError, incorrectDoc, emptyDoc };

        const char* _filename;
        xmlpp::DomParser* _xmlParser;
        std::list<std::string>* _tempStorage;

    };

}

#endif // PARSER_H
