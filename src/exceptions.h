#ifndef __LIBBOOLFUNCCOMPILER_EXCEPTIONS_H__
#define __LIBBOOLFUNCCOMPILER_EXCEPTIONS_H__

/*
 * Copyright (c) Anton Liaukevich 2009-2010 <leva.dev@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "types.h"
#include <polishboolfunctions/exceptions.h>

namespace Sem {


struct Error: public PolishBoolFunctions::Error {};

class UnknownIndentifier: public Error {
public:

    inline UnknownIndentifier(const Sem::String& name):
        name_(name) {}

    virtual ~UnknownIndentifier() throw();

    inline Sem::String getName() const
    {
        return name_;
    }

private:
    Sem::String name_;

}; //class UnknownIndentifier

struct UnknownParameter: public UnknownIndentifier {

    inline UnknownParameter(const String& name):
        UnknownIndentifier(name) {}

    virtual ~UnknownParameter() throw();

    virtual String what() const throw();

}; //struct UnknownParameter

struct FunctionHasNotDefined: public UnknownIndentifier {

    inline FunctionHasNotDefined(const String& name):
        UnknownIndentifier(name) {}

    virtual ~FunctionHasNotDefined() throw();

    virtual String what() const throw();

}; //struct FunctionHasNotDefined


} //namespace Sem

#endif //__LIBBOOLFUNCCOMPILER_EXCEPTIONS_H__
