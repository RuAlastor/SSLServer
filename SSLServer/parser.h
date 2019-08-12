#ifndef PARSER_H
#define PARSER_H

// General libs
#include <string>
#include <list>
#include <cstring>                      // Needed to transform error into readable state

#include <errno.h>                      // Needed to check errors in C-functions

// Parser part
#include <libxml++/libxml++.h>          /// Main xml-parser library
#include <libxml2/libxml/parser.h>      /// Needed to clean up memory partially

#define PARSER_DEBUG
//#undef PARSER_DEBUG

#ifdef PARSER_DEBUG
    #include <iostream>
    #define PARSER_DEBUG_PRINTER( msg ) std::cout << msg << '\n';
    #define PARSER_DEBUG_C_PRINTER( preErrorMsg ) __printCError( preErrorMsg );
#else
    #define PARSER_DEBUG_PRINTER(msg)
    #define PARSER_DEBUG_C_PRINTER(preErrorMsg)
#endif

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

    /**
     * @brief The Parser class - libxml++ wrapper
     */
    class Parser {

    public:
        /// @brief Return error types
        enum err
        {
            documentError   = -5,
            unloadingError  = -4,
            loadingError    = -2,
            undefinedError  = -1,
            noError         = 0
        };

    public:
        /// @defgroup Parser_setup_methods
        /// @{
        /// @brief Default constructor
        Parser() noexcept : _xmlParser(nullptr) {}
        /**
         * @brief loadDocument - loads document into the memory and parses it
         * @param[in] xmlDocument - document-string to parse
         * @return 0 if no error occured, -1 otherwise
         */
        err loadDocument(const std::string& xmlDocument) noexcept;
        /// @}

        /// @defgroup Parser_usage_methods
        /// @{
        /**
         * @brief parseDocument - parses loaded document
         * @param[out] answerList list of concatinated fileds of client-nodes if no error occured
         * @return 0 if no error occured, -1 otherwise
         */
        err parseDocument(std::list<std::string>& answerList) const noexcept;
        std::string rebuildDocument(const std::list<std::string>& sigList,
                                    const std::string& pubKey
                                    ) noexcept;
        /// @}

        /// @defgroup Parser_deleting_methods
        /// @{
        /**
         * @brief unloadDocument - unloads document from memory
         * @return 0 if no error occured, -1 otherwise
         */
        err unloadDocument() noexcept;
        /// @brief Default destructor
        ~Parser() = default;
        /// @}

    private:
        typedef std::list<std::string> list_str;

    private:
        xmlpp::DomParser* _xmlParser;

    private:
        /// @defgroup Parser_additional_methods
        /// @{
        /**
         * @brief __fillList - reads xml-document and gets concatinated fields of a client-node
         * @param[out] list of concatinated fields of client-nodes
         * @return 0 if no error occured, -1 otherwise
         */
        err __fillList(std::list<std::string>& answerList) const noexcept(false);
        /**
         * @brief __getClientText parses client-node
         * @param[in] client - client node to parse
         * @return concatinated fields of the node
         */
        std::string __getClientText(const xmlpp::Element* const client) const noexcept(false);
        /// @}

    private:
        /// @defgroup Parser_deleted_methods
        /// @{
        /// @brief Deleted copy constructor
        Parser(const Parser&) = delete;
        /// @brief Deleted move constructor
        Parser(Parser&&) = delete;
        /// @brief Deleted copy assigner
        Parser& operator =(const Parser&) = delete;
        /// @brief Deleted move assigner
        Parser& operator =(Parser&&) = delete;
        /// @}

    };

//-----------------------------------------------------------------------------------------------------------------------------

}

#endif // PARSER_H
