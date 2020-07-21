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

#include "predefined-operations.h"
#include <polishboolfunctions/NotOperation.h>
#include <polishboolfunctions/BinaryOperation.h>

using namespace PolishBoolFunctions;


AbstractOperation* Sem::predefinedOperation(const String& name)
{
    using namespace UnaryOperators;
    using namespace BinaryOperators;

    if (name == "Not")
        return Not.clone();
    else if (name == "BinaryIdenticalFalse")
        return BinaryOperators::IdenticalFalse.clone();
    else if (name == "NotOr")
        return NotOr.clone();
    else if (name == "AndNot")
        return Greater.clone();
    else if (name == "NotSecond")
        return NotSecond.clone();
    else if (name == "Less")
        return Less.clone();
    else if (name == "NotFirst")
        return NotFirst.clone();
    else if (name == "Xor")
        return Xor.clone();
    else if (name == "NotAnd")
        return NotAnd.clone();
    else if (name == "And")
        return And.clone();
    else if (name == "Equal")
        return Equal.clone();
    else if (name == "First")
        return First.clone();
    else if (name == "ReverseImplies")
        return ReverseImplies.clone();
    else if (name == "Second")
        return Second.clone();
    else if (name == "Implies")
        return Implies.clone();
    else if (name == "Or")
        return Or.clone();
    else if (name == "BinaryIdenticalTrue")
        return BinaryOperators::IdenticalTrue.clone();
    else
        return 0;
}
