#ifndef PARSER_H
#define PARSER_H

#include "headers.h"
#include "exceptions.h"

namespace Sorokin {

    class Parser {

    public:
        Parser(const char* filename, std::list<std::string>* tempStorage) noexcept;
        ~Parser() noexcept;

        Parser() = delete;
        Parser(const Parser& other) = delete;
        Parser(Parser&& other) = delete;
        Parser& operator=(const Parser& other) = delete;
        Parser& operator=(Parser&& other) = delete;

        void Clear() noexcept;
        void loadDocument() noexcept(false);
        void parseDocument() noexcept(false);

    private:
        const char* _filename;
        xmlpp::DomParser* _xmlParser;
        std::list<std::string>* _tempStorage;

    };

}

#endif // PARSER_H
