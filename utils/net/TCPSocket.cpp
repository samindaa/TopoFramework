// -*-c++-*-

/***************************************************************************
 tcpsocket.cpp  -  A simple tcp socket class
 -------------------
 begin                : 08-JAN-2003
 copyright            : (C) 2003 by The RoboCup Soccer Server
 Maintenance Group.
 email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU LGPL as published by the Free Software  *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/

#include "TCPSocket.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <cerrno>
#include <netinet/in.h>

#ifdef HAVE_WINSOCK2_H
#include "winsock2.h"
#endif

#include <iostream>

namespace rcss
{
namespace net
{
TCPSocket::TCPSocket()
{
  open();
}

TCPSocket::TCPSocket(SocketDesc& s) :
    Socket(s)
{
}

TCPSocket::TCPSocket(const Addr& addr)
{
  open();
  bind(addr);
}

TCPSocket::TCPSocket(const Addr& addr, const Addr& dest)
{
  open();
  bind(addr);
  connect(dest);
}

bool TCPSocket::doOpen(SocketDesc& fd)
{
  close();
  fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  return fd >= 0;
}
}
}
