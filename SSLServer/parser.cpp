#include "parser.h"

using namespace Sorokin;


Parser::Parser(std::string* xmlFile,
               std::list<std::string>* externalList) noexcept : _xmlFile(xmlFile),
                                                                _xmlParser(nullptr),
                                                                _tempStorage(externalList) {}
Parser::~Parser() {
    delete _xmlParser;
    _xmlParser = nullptr;
}

void Parser::Clear() noexcept {
    _xmlFile->clear();
    delete _xmlParser;
    _xmlParser = nullptr;
}

void Parser::loadDocument() noexcept(false) {
    _xmlParser = new xmlpp::DomParser();
    _xmlParser->parse_memory(_xmlFile->c_str());
    _xmlFile->clear();
}

void Parser::parseDocument() noexcept(false) {
    const xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    if (root == nullptr) {
        throw EmptyFileError();
    }
    for (const auto& possibleClient : root->get_children()) {
        if (const xmlpp::Element* client = dynamic_cast<const xmlpp::Element*>(possibleClient)) {
            std::string tmp = "";
            for (const auto& possibleChild : client->get_children()) {
                if (const xmlpp::Element* child = dynamic_cast<const xmlpp::Element*>(possibleChild)) {
                    tmp += child->get_child_text()->get_content().c_str();
                }
            }
            _tempStorage->push_back(tmp);
        }
    }
}


void Parser::rebuildDocument(const std::list<std::string>& signatures, const std::string& publicKey) noexcept {
    xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    root->set_name("response");
    auto iter = signatures.begin();
    for (auto& possibleClient : root->get_children()) {
        if (xmlpp::Element* client = dynamic_cast<xmlpp::Element*>(possibleClient)) {
            xmlpp::Element* newNode = client->add_child("signature");
            newNode->add_child_text(*iter);
            client->add_child_text_before(newNode, "\t");
            client->add_child_text("\n\t\t");
            iter++;
            newNode = client->add_child("public key");
            newNode->add_child_text(publicKey);
            client->add_child_text("\n\t");
        }
    }

    _xmlFile->clear();
    *_xmlFile = _xmlParser->get_document()->write_to_string().c_str();
}


