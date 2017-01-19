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

#include "factory/couchbase_interface.h"
#include "couchbase_admin.h"

#ifndef AOSSL_FACTORY_COUCHBASE
#define AOSSL_FACTORY_COUCHBASE

//! The Couchbase Service Component Factory

//! The Service Component Factory tracks the Couchbase
//! objects exposed by the framework and passes back
//! instances of interfaces.  This allows for the publicly exposed methods
//! to be independent of the implementations.
class CouchbaseComponentFactory
{
public:

  //! Create a new Service Component Factory
  CouchbaseComponentFactory() {}

  //! Delete a Service Component Factory
  ~CouchbaseComponentFactory() {}

  //! Get a Couchbase Interface instance
  CouchbaseInterface* get_couchbase_interface( const char * conn ) {return new CouchbaseAdmin( conn );}

  //! Get a Couchbase Interface instance for a password protected DB
  CouchbaseInterface* get_couchbase_interface( const char * conn, const char * pswd ) {return new CouchbaseAdmin( conn, pswd );}

};

#endif
