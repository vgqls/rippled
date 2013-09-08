//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef BEAST_ASIO_HTTPPARSER_H_INCLUDED
#define BEAST_ASIO_HTTPPARSER_H_INCLUDED

class HTTPParserImpl;

/** A parser for HTTPRequest and HTTPResponse objects. */
class HTTPParser
{
public:
    enum Type
    {
        typeRequest,
        typeResponse
    };

    /** Construct a new parser for the specified HTTPMessage type. */
    explicit HTTPParser (Type type);

    /** Destroy the parser. */
    ~HTTPParser ();

    /** Returns a non zero error code if parsing fails. */
    unsigned char error () const;

    /** Returns the error message text when error is non zero. */
    String message () const;

    /** Parse the buffer and return the amount used.
        Typically it is an error when this returns less than
        the amount passed in.
    */
    std::size_t process (void const* buf, std::size_t bytes);

    /** Notify the parser that eof was received.
    */
    void process_eof ();

    /** Returns `true` when parsing is successful and complete. */
    bool finished () const;

    /** Return the HTTPResponse object produce from the parsing.
        Only valid after finished returns `true`.
    */
    SharedPtr <HTTPResponse> const& response ();

    //SharedPtr <HTTPRequest> const& request ();

private:
    Type m_type;
    ScopedPointer <HTTPParserImpl> m_impl;
    SharedPtr <HTTPResponse> m_response;
    //SharedPtr <HTTPRequest> m_request;
};

#endif