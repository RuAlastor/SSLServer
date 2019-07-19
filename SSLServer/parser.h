#ifndef PARSER_H
#define PARSER_H

#include "headers.h"
#include "exceptions.h"

namespace Sorokin {

/*
    class Parser {

    public:
        Parser(std::string* xmlFile, std::list<std::string>* externalList) noexcept;
        ~Parser() noexcept;

        Parser() = delete;
        Parser(const Parser& other) = delete;
        Parser(Parser&& other) = delete;
        Parser& operator=(const Parser& other) = delete;
        Parser& operator=(Parser&& other) = delete;

        void Clear() noexcept;
        void loadDocument() noexcept(false);
        void parseDocument() noexcept(false);
        void rebuildDocument(const  std::list<std::string>& signatures, const std::string& publicKey) noexcept;

    private:
        std::string* _xmlFile;
        xmlpp::DomParser* _xmlParser;
        std::list<std::string>* _tempStorage;

    };
*/

}

#endif // PARSER_H
