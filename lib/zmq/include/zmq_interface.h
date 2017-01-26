/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <string>

#ifndef ZMQ_INTERFACE
#define ZMQ_INTERFACE

const int REQ_RESP = 0;
const int PUB_SUB = 1;

//! An Interface for ZMQIO

//! Defines the methods that the ZMQ Managers must implement
//! send & recv, as well as subscribe
class Zmqio
{
public:
  virtual ~Zmqio() {}
  //! Recieve a message on the port
  virtual std::string recv() = 0;

  //! Send a message on the port
  virtual void send(const char * msg, int msg_size) = 0;

  //! Send a string on the port
  virtual void send(std::string msg) = 0;

  //! Subscribe on a particular filter (only effective for Pub/Sub)
  virtual void subscribe(std::string filter) = 0;
};

//! An Outbound ZMQ Manager

//! Acts as the Requestor (Client) in the ZMQ Sockets
//! Send, then Recieve
class ZmqOut: public Zmqio
{
public:

  virtual ~ZmqOut() {}

  //! Connect to the given conn_str
  virtual void connect(std::string conn_str) = 0;

  //! Send a message on the port
  virtual void send(const char * msg, int msg_size) = 0;

  //! Send a string on the port
  virtual void send(std::string msg) = 0;

  //! Recieve a message on the port
  virtual std::string recv() = 0;

  //! Subscribe on a particular filter (only effective for Pub/Sub)
  virtual void subscribe(std::string filter) = 0;
};

//! An Inbound ZMQ Manager

//! Acts as the Responder (Server) in the ZMQ Sockets
//! Recieve, then Send
class ZmqIn: public Zmqio
{
public:

  virtual ~ZmqIn() {}

  //! Bind on the given conn_str
  virtual void bind(std::string conn_str) = 0;

  //! Recieve a message on the port
  virtual std::string recv() = 0;

  //! Send a message on the port
  virtual void send(const char * msg, int msg_size) = 0;

  //! Send a string on the port
  virtual void send(std::string msg) = 0;

  //! Subscribe on a particular filter (only effective for Pub/Sub)
  virtual void subscribe(std::string filter) = 0;
};
#endif
