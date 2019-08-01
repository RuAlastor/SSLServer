#include "parser.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Parser::err Parser::loadDocument(const std::string& xmlDocument) noexcept {
    try {
        _xmlParser = new xmlpp::DomParser;
        _xmlParser->parse_memory(xmlDocument.c_str());
    }
    catch (std::exception& error) {
        std::cout <<  error.what() << '\n';
        return undefinedError;
    }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Parser::err Parser::unloadDocument() noexcept {
    try {
        delete _xmlParser;
        xmlCleanupParser();
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
        return undefinedError;
    }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Parser::err Parser::parseDocument(list_str& answerList) noexcept {
    if (_xmlParser == nullptr) {
        return undefinedError;
    }
    try {
         __fillList(answerList);
    }
    catch (std::exception& error) {
        std::cout << error.what() << '\n';
    }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Parser::err Parser::__fillList(list_str& answerList) noexcept(false) {
    const xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    if (root != nullptr) {
        for (const auto& possibleClient : root->get_children()) {
            if (const xmlpp::Element* client = dynamic_cast<const xmlpp::Element*>(possibleClient)) {
                answerList.push_back(__getClientText(client));
            }
        }
        return noError;
    }
    return undefinedError;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Parser::__getClientText(const xmlpp::Element *client) noexcept(false) {
    std::string answerStr = "";
    for (const auto& possibleText : client->get_children()) {
        if (const xmlpp::Element* text = dynamic_cast<const xmlpp::Element*>(possibleText)) {
            answerStr += text->get_child_text()->get_content().c_str();
        }
    }
#ifdef PARSER_DEBUG
    std::cout << "Client str: " << answerStr << '\n';
#endif
    return answerStr;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Parser::rebuildDocument(const std::list<std::string>& sigList, const std::string& pubKey) noexcept(false) {
    xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    root->set_name("response");
    auto iter = sigList.begin();
    for (auto& possibleClient : root->get_children()) {
        if (xmlpp::Element* client = dynamic_cast<xmlpp::Element*>(possibleClient)) {
            xmlpp::Element* newNode = client->add_child("signature");
            newNode->add_child_text(*iter);
            client->add_child_text_before(newNode, "\t");
            client->add_child_text("\n\t\t");
            iter++;
            newNode = client->add_child("public_key");
            newNode->add_child_text(pubKey);
            client->add_child_text("\n\t");
        }
    }

    return std::string(_xmlParser->get_document()->write_to_string().c_str());
}

//-----------------------------------------------------------------------------------------------------------------------------


