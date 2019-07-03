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
    return noError;
}


int Parser::rebuildDocument(const std::list<unsigned char*>& signatures, const std::string& publicKey) noexcept(false) {
    xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    root->set_name("response");
    auto iter = signatures.begin();
    for (auto& possibleClient : root->get_children()) {
        if (xmlpp::Element* client = dynamic_cast<xmlpp::Element*>(possibleClient)) {
            xmlpp::Element* newNode = client->add_child("signature");
            newNode->add_child_text(reinterpret_cast<const char*>(*iter));
            iter++;
            newNode = client->add_child("public key");
            newNode->add_child_text(publicKey);
        }
    }

    _xmlParser->get_document()->write_to_file(_filename);
    return noError;
}


