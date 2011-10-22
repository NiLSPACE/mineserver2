/*
  Copyright (c) 2011, The Mineserver Project
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
  * Neither the name of the The Mineserver Project nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MINESERVER_NETWORK_CLIENT_H
#define MINESERVER_NETWORK_CLIENT_H

#include <string>
#include <iostream>
#include <list>
#include <vector>

#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include <mineserver/network/parser.h>

namespace Mineserver
{
  class Network_Client : public boost::enable_shared_from_this<Mineserver::Network_Client>
  {
  public:
    typedef boost::shared_ptr<Mineserver::Network_Client> pointer_t;

  private:
    boost::asio::ip::tcp::socket m_socket;
    boost::array<uint8_t, 8192> m_tmp;
    std::vector<uint8_t> m_buffer;
    Mineserver::Network_Parser* m_parser;
    std::list< boost::shared_ptr<Mineserver::Network_Message> > m_incoming;
    std::list< boost::shared_ptr<Mineserver::Network_Message> > m_outgoing;

  public:
    static pointer_t create(boost::asio::io_service& service, Mineserver::Network_Parser* parser)
    {
      return pointer_t(new Mineserver::Network_Client(service, parser));
    }

    boost::asio::ip::tcp::socket& socket()
    {
      return m_socket;
    }

    Mineserver::Network_Parser* parser()
    {
      return m_parser;
    }

    std::list< boost::shared_ptr<Mineserver::Network_Message> >& incoming() { return m_incoming; }
    std::list< boost::shared_ptr<Mineserver::Network_Message> >& outgoing() { return m_outgoing; }

    void start();
    void stop();

  private:
    Network_Client(boost::asio::io_service& service, Mineserver::Network_Parser* parser) : m_socket(service),m_parser(parser)
    {
    }

    void handleRead(const boost::system::error_code& error, size_t transferred);
    void handleWrite(const boost::system::error_code& error);
  };
};

#endif