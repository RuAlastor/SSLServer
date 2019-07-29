#ifndef PARSER_H
#define PARSER_H

#include "headers.h"

namespace Sorokin {

//-----------------------------------------------------------------------------------------------------------------------------

    /**
     * @brief The Parser class - libxml++ wrapper
     */
    class Parser {

    public:
        /// @brief Return error types
        enum err {
            unknownError = -1,
            noError = 0
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
         * @return list of concatinated fileds of client-nodes if no error occured, <nullptr> else
         */
        std::list<std::string>* parseDocument() noexcept(false);
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
         * @return list of concatinated fields of client-nodes
         */
        std::list<std::string>* __fillList() noexcept(false);
        /**
         * @brief __getClientText parses client-node
         * @param[in] client - client node to parse
         * @return concatinated fields of the node
         */
        std::string __getClientText(const xmlpp::Element* client) noexcept(false);
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
