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

#include "exceptions.h"
#include <boost/format.hpp>

using namespace Sem;
using boost::format;


/* class UnknownIndentifier */

UnknownIndentifier::~UnknownIndentifier() throw() {}


/* class UnknownParameter */

UnknownParameter::~UnknownParameter() throw() {}

Error::String UnknownParameter::what() const throw()
{
    return (format("Parameter '%1%' is unknown") % getName()).str();
}


/* class FunctionHasNotDefined */

FunctionHasNotDefined::~FunctionHasNotDefined() throw() {}

Error::String FunctionHasNotDefined::what() const throw()
{
    return (format("Function '%1%' has not defined") % getName()).str();
}
