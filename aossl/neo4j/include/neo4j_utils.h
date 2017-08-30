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

#include <neo4j-client.h>

#ifndef AOSSL_NEO4J_INCLUDE_NEO4J_UTILS_H_
#define AOSSL_NEO4J_INCLUDE_NEO4J_UTILS_H_

// Neo4j Query Parameter Types
const int _BOOL_TYPE = 0;
const int _STR_TYPE = 1;
const int _INT_TYPE = 2;
const int _FLT_TYPE = 3;

// Typedefs of Neo4j Functions
typedef neo4j_value_t(*ValueGenerationFunction)(neo4j_value_t);
typedef neo4j_value_t(*KeyGenerationFunction)(neo4j_value_t, neo4j_value_t);
typedef neo4j_value_t(*IndexGenerationFunction)(neo4j_value_t, unsigned int);

#endif  // AOSSL_NEO4J_INCLUDE_NEO4J_UTILS_H_
