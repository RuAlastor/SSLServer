#include "parser.h"

using namespace Sorokin;


Parser::Parser(const char *filename,
               std::list<std::string>* externalList) noexcept : _filename(filename),
                                                                _xmlParser(nullptr),
                                                                _tempStorage(externalList) {}
Parser::~Parser() {
    delete _xmlParser;
}


void Parser::loadDocument() noexcept(false) {
    _xmlParser = new xmlpp::DomParser();
    _xmlParser->parse_file(_filename);
}

int Parser::parseDocument() noexcept(false) {
    const xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    if (root == nullptr) {
        std::cout << "File is empty. Nothing to sign";
        return emptyDoc;
    }
    for (const auto& client : root->get_children()) {
        if (const xmlpp::Element* clientChild = dynamic_cast<const xmlpp::Element*>(client)) {
            std::string tmp = "";
            for (const auto& info : clientChild->get_children()) {
                if (const xmlpp::Element* childText = dynamic_cast<const xmlpp::Element*>(info)) {
                    tmp += childText->get_child_text()->get_content().c_str();
                }
            }
            _tempStorage->push_back(tmp);
        }
    }
    return noError;
}





