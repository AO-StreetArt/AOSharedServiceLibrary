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
#include <exception>

#include "neo4j_interface.h"
#include "neo4j_result_tree.h"
#include "neo4j_connection_pool.h"

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_RESULT_ITERATOR_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_RESULT_ITERATOR_H_

// Results Iterator, accepts a result stream as an input,
// Returns Results Trees for each entry that matches in the query
// Deletes it when finished
// Also responsible for releasing the database connection after usage
class ResultsIterator: public ResultsIteratorInterface {
  neo4j_result_stream_t *results;
  Neo4jQuerySession *s;
  Neo4jConnectionPool *p;
  inline void clear_results() {
    if (neo4j_close_results(results) != 0) {
      const char * err_string = strerror(errno);
      std::string err_msg(err_string);
      err_msg = "Error clearing results from memory: " + err_msg;
      throw Neo4jException(err_msg);
    }
  }

 public:
  inline ResultsIterator(neo4j_result_stream_t *result_stream, \
    Neo4jQuerySession *session, Neo4jConnectionPool *pool) {
    results = result_stream;
    if ( !(results) || neo4j_nfields(results) < 0 ) {
      std::string err_msg(strerror(errno));
      err_msg = "Failed to fetch results" + err_msg;
      p->release_connection(s);
      throw Neo4jException(err_msg);
    }
    s = session;
    p = pool;
  }
  ~ResultsIterator() {clear_results(); p->release_connection(s);}
  void clear() {clear_results();}
  bool empty() {if (!results) {return true;} return false;}
  unsigned int length() {return neo4j_nfields(results);}
  inline ResultTreeInterface* next() {
    neo4j_result_t *res = neo4j_fetch_next(results);
    if (!res) {return NULL;} else {return new ResultTree(res);}
  }
};

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_RESULT_ITERATOR_H_
