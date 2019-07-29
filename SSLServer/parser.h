#ifndef PARSER_H
#define PARSER_H

#include "headers.h"

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

    class Parser {

        typedef std::list<std::string> list_str;

    public:
        enum {
            unknownError = -1,
            noError = 0
        };

    public:
        Parser() = default;
        ~Parser() = default;

        std::list<std::string>* parseDocument(const std::string& xmlDocument) noexcept(false);
        inline void tryToCleanUp() noexcept(false) { xmlCleanupParser(); }

    private:
        xmlpp::DomParser* __loadDocument(const std::string& xmlDocument) noexcept;
        std::list<std::string>* __fillList(const xmlpp::DomParser* xmlParser) noexcept(false);
        std::string __getClientText(const xmlpp::Element* client) noexcept(false);

    private:
        Parser(const Parser&) = delete;
        Parser(Parser&&) = delete;
        Parser& operator =(const Parser&) = delete;
        Parser& operator =(Parser&&) = delete;

    };

//-----------------------------------------------------------------------------------------------------------------------------

}

#endif // PARSER_H
