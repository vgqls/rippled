//------------------------------------------------------------------------------
/*
    Copyright (c) 2011-2013, OpenCoin, Inc.
*/
//==============================================================================

#ifndef RIPPLE_HTTP_DOOR_H_INCLUDED
#define RIPPLE_HTTP_DOOR_H_INCLUDED

namespace ripple {
namespace HTTP {

/** A listening socket. */
class Door
    : public SharedObject
    , public AsyncObject <Door>
    , public List <Door>::Node
    , public LeakChecked <Door>
{
public:
    typedef SharedPtr <Door> Ptr;

    ServerImpl& m_impl;
    acceptor m_acceptor;
    Port m_port;

    Door (ServerImpl& impl, Port const& port)
        : m_impl (impl)
        , m_acceptor (m_impl.get_io_service(), to_asio (port))
        , m_port (port)
    {
        m_impl.add (*this);

        error_code ec;

        m_acceptor.set_option (acceptor::reuse_address (true), ec);
        if (ec)
        {
            m_impl.journal().error <<
                "Error setting acceptor socket option: " << ec.message();
        }

        if (! ec)
        {
            m_impl.journal().info << "Bound to endpoint " <<
                to_string (m_acceptor.local_endpoint());

            async_accept();
        }
        else
        {
            m_impl.journal().error << "Error binding to endpoint " <<
                to_string (m_acceptor.local_endpoint()) <<
                ", '" << ec.message() << "'";
        }
    }

    ~Door ()
    {
        m_impl.remove (*this);
    }

    Port const& port () const
    {
        return m_port;
    }

    void cancel ()
    {
        m_acceptor.cancel();
    }

    void failed (error_code ec)
    {
    }

    void asyncHandlersComplete ()
    {
    }

    void async_accept ()
    {
        Peer* peer (new Peer (m_impl, m_port));
        m_acceptor.async_accept (peer->get_socket(), boost::bind (
            &Door::handle_accept, Ptr(this),
                boost::asio::placeholders::error,
                    Peer::Ptr (peer), CompletionCounter (this)));
    }

    void handle_accept (error_code ec, Peer::Ptr peer, CompletionCounter)
    {
        if (ec == boost::asio::error::operation_aborted)
            return;

        if (ec)
        {
            m_impl.journal().error << "Accept failed: " << ec.message();
            return;
        }

        async_accept();

        peer->handle_accept();
    }
};

}
}

#endif
