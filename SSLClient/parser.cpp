#include "parser.h"

using namespace Sorokin;


Parser::Parser(const char* filename, std::list<std::string>* tempStorage) noexcept : _filename(filename),
                                                                                     _xmlParser(nullptr),
                                                                                     _tempStorage(tempStorage) {}
Parser::~Parser() {
    delete _xmlParser;
    _xmlParser = nullptr;
}

void Parser::Clear() noexcept {
    delete _xmlParser;
    _xmlParser = nullptr;
}

void Parser::loadDocument() noexcept(false) {
    _xmlParser = new xmlpp::DomParser();
    _xmlParser->parse_file(_filename);
}

void Parser::parseDocument() noexcept(false) {
    const xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    if (root == nullptr) {
        throw FileError();
    }
    for (const auto& possibleClient : root->get_children()) {
        if (const xmlpp::Element* client = dynamic_cast<const xmlpp::Element*>(possibleClient)) {
            std::string tmp = "";
            // Get whole string
            for (const auto& possibleChild : client->get_children()) {
                if (const xmlpp::Element* child = dynamic_cast<const xmlpp::Element*>(possibleChild)) {
                    if (child->get_name() == "signature" || child->get_name() == "public_key")
                        continue;
                    tmp += child->get_child_text()->get_content().c_str();
                }
            }
            _tempStorage->push_back(tmp);
            // Get signature and pubkey
            for (const auto& possibleChild : client->get_children()) {
                if (const xmlpp::Element* child = dynamic_cast<const xmlpp::Element*>(possibleChild)) {
                    if (child->get_name() == "signature") {
                        tmp = child->get_child_text()->get_content().c_str();
                        _tempStorage->push_back(tmp);
                        tmp.clear();
                    }
                    else if (child->get_name() == "public_key") {
                        tmp = child->get_child_text()->get_content().c_str();
                        _tempStorage->push_back(tmp);
                        tmp.clear();
                    }

                }
            }
        }
    }
}
