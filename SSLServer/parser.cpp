#include "parser.h"

using namespace Sorokin;

//-----------------------------------------------------------------------------------------------------------------------------

Parser::err Parser::loadDocument( const std::string& xmlDocument ) noexcept
{
    try
    {
        _xmlParser = new xmlpp::DomParser;
        _xmlParser->parse_memory(xmlDocument.c_str());
    }
    catch ( const std::exception& error )
    {
        PARSER_DEBUG_PRINTER( error.what() )
        return loadingError;
    }
    catch (...) { return undefinedError; }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Parser::err Parser::unloadDocument() noexcept {
    try
    {
        delete _xmlParser;
        xmlCleanupParser();
    }
    catch ( const std::exception& error )
    {
        PARSER_DEBUG_PRINTER( error.what() )
        return unloadingError;
    }
    catch (...) { return undefinedError; }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Parser::err Parser::parseDocument( list_str& answerList ) const noexcept {
    if ( _xmlParser == nullptr ) { return loadingError; }

    try { __fillList( answerList ); }
    catch ( const std::exception& error )
    {
        PARSER_DEBUG_PRINTER( error.what() )
        return undefinedError;
    }
    catch (...) { return undefinedError; }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

Parser::err Parser::__fillList(list_str& answerList) const noexcept(false) {
    const xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    if ( root == nullptr )
    {
        PARSER_DEBUG_PRINTER( "Empty document" )
        return documentError;
    }

    for ( const auto& possibleClient : root->get_children() )
    {
        if ( const xmlpp::Element* client = dynamic_cast<const xmlpp::Element*>( possibleClient ) )
        { answerList.push_back( __getClientText( client ) ); }
    }

    return noError;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Parser::__getClientText( const xmlpp::Element* const client ) const noexcept( false )
{
    std::string answerStr = "";
    for ( const auto& possibleText : client->get_children() )
    {
        if ( const xmlpp::Element* text = dynamic_cast<const xmlpp::Element*>( possibleText ) )
        { answerStr += text->get_child_text()->get_content().c_str(); }
    }
    PARSER_DEBUG_PRINTER( std::string( "Client std: " ) + answerStr )

    return answerStr;
}

//-----------------------------------------------------------------------------------------------------------------------------

std::string Parser::rebuildDocument( const std::list<std::string>& sigList,
                                     const std::string& pubKey ) noexcept
{
    if ( _xmlParser == nullptr )
    {
        PARSER_DEBUG_PRINTER( "Parser is not loaded!" )
        return "";
    }

    xmlpp::Element* root = _xmlParser->get_document()->get_root_node();
    if ( root == nullptr )
    {
        PARSER_DEBUG_PRINTER( "Empty document!" )
        return "";
    }

    try
    {
        root->set_name("response");

        auto iter = sigList.begin();
        for ( auto& possibleClient : root->get_children() )
        {
            if ( xmlpp::Element* client = dynamic_cast<xmlpp::Element*>( possibleClient) )
            {
                xmlpp::Element* newNode = client->add_child( "signature" );
                newNode->add_child_text(*iter);
                client->add_child_text_before(newNode, "\t");
                client->add_child_text("\n\t\t");
                iter++;

                newNode = client->add_child("public_key");
                newNode->add_child_text(pubKey);
                client->add_child_text("\n\t");
            }
        }
    }
    catch ( const std::exception& error )
    {
        PARSER_DEBUG_PRINTER( error.what() )
        return "";
    }
    catch (...)
    {
        PARSER_DEBUG_PRINTER( "Undefined error" )
        return "";
    }

    return std::string( _xmlParser->get_document()->write_to_string().c_str() );
}

//-----------------------------------------------------------------------------------------------------------------------------


