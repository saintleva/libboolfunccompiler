#ifndef __LIBBOOLFUNCCOMPILER_SEMANTICDATA_H__
#define __LIBBOOLFUNCCOMPILER_SEMANTICDATA_H__

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
#include "exceptions.h"
#include <map>
#include <polishboolfunctions/Rpn.h>
#include <polishboolfunctions/RpnFunction.h>

namespace Sem {


struct FunctionHeader {

    typedef PolishBoolFunctions::Rpn::size_type Arity;
    typedef std::map<String,Arity> ParamMap;

    inline Arity getArity() const
    {
        return params.size();
    }

    inline Arity findParameter(const String& parameter) const
    {
        ParamMap::const_iterator iter = params.find(parameter);
        if (iter != params.end())
            return iter->second;
        else
            throw UnknownParameter(parameter);
    }

    String name;
    ParamMap params;

}; //struct FunctionHeader

struct FunctionDefinition {

    FunctionHeader header;
    PolishBoolFunctions::Rpn rpn;

}; //struct FunctionDefinition

struct FunctionContainer {

    typedef std::list<FunctionDefinition> List;
    typedef std::map<String,FunctionDefinition*> Map;
    typedef Map::value_type Pair;

    inline FunctionDefinition& addUnspecifiedFunc()
    {
        list.push_back(FunctionDefinition());
        return list.back();
    }

    inline void indexLastFunc(const String& name)
    {
        map.insert(Pair(name, &list.back()));
    }

    inline bool isDefined(const String& name) const
    {
        return map.find(name) != map.end();
    }

    inline PolishBoolFunctions::RpnFunction findRpnFunction(const String& name) const
    {
        Map::const_iterator iter = map.find(name);
        if (iter != map.end())
        {
            FunctionDefinition* func = iter->second;
            return PolishBoolFunctions::RpnFunction(&func->rpn, func->header.getArity());
        }
        else
            throw FunctionHasNotDefined(name);
    }

    List list;
    Map map;

}; //struct FunctionContainer


} //namespace Sem

#endif //__LIBBOOLFUNCCOMPILER_SEMANTICDATA_H__
