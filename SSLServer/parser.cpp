#include "parser.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

xmlpp::DomParser* Parser::__loadDocument(const std::string& xmlDocument) noexcept {
    xmlpp::DomParser* xmlParser;
    try {
        xmlParser = new xmlpp::DomParser;
        xmlParser->parse_memory(xmlDocument.c_str());
    }
    catch (std::bad_alloc& error) {
        std::cout << error.what() << '\n';
        return nullptr;
    }
    catch (std::exception& error) {
        std::cout <<  error.what() << '\n';
        delete xmlParser;
        return nullptr;
    }
    catch (...) {
        std::cout << "Unknown error!\n";
        delete xmlParser;
        return nullptr;
    }
    return xmlParser;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::list<std::string>* Parser::parseDocument(const std::string& xmlDocument) noexcept(false) {
    xmlpp::DomParser* xmlParser = __loadDocument(xmlDocument);
    if (xmlParser == nullptr) {
        return nullptr;
    }

    list_str* returnList = __fillList(xmlParser);

    delete xmlParser;
    xmlParser = nullptr;
    return returnList;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::list<std::string>* Parser::__fillList(const xmlpp::DomParser* xmlParser) noexcept(false) {
    const xmlpp::Element* root = xmlParser->get_document()->get_root_node();
    list_str* returnList = nullptr;
    if (root != nullptr) {
        returnList = new list_str;
        for (const auto& possibleClient : root->get_children()) {
            if (const xmlpp::Element* client = dynamic_cast<const xmlpp::Element*>(possibleClient)) {
                returnList->push_back(__getClientText(client));
            }
        }
    }
    return returnList;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Parser::__getClientText(const xmlpp::Element *client) noexcept(false) {
    std::string answerStr = "";
    for (const auto& possibleText : client->get_children()) {
        if (const xmlpp::Element* text = dynamic_cast<const xmlpp::Element*>(possibleText)) {
            answerStr += text->get_child_text()->get_content().c_str();
        }
    }
#ifdef DEBUG
    std::cout << "Client str: " << answerStr << '\n';
#endif
    return answerStr;
}

//-----------------------------------------------------------------------------------------------------------------------------


/*
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
            newNode = client->add_child("public_key");
            newNode->add_child_text(publicKey);
            client->add_child_text("\n\t");
        }
    }

    _xmlFile->clear();
    *_xmlFile = _xmlParser->get_document()->write_to_string().c_str();
}
*/

