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

#include "include/uuid_admin.h"

//Constructor & Destructor
//uuidAdmin::uuidAdmin() {}
//uuidAdmin::~uuidAdmin() {}

//Generate a new UUID
UuidContainer uuidAdmin::generate()
{
UuidContainer cont;
int uuid_gen_result = 0;
uuid_t uuid;
uuid_gen_result = uuid_generate_time_safe(uuid);

if (uuid_gen_result == -1) {
	cont.err = "UUID Generated in an unsafe manner that exposes a potential security risk : http://linux.die.net/man/3/uuid_generate";
}
else {
	cont.err = "";
}

char uuid_str[37];
uuid_unparse_lower(uuid, uuid_str);

std::string str (uuid_str);
cont.id = str;
return cont;
}
