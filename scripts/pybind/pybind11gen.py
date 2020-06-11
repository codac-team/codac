#!/usr/bin/env python3
#
# Apache 2 license
#

import argparse
import collections

import CppHeaderParser
from  pprint import pprint

from pathlib import Path


import random
import string

MethodHookData = collections.namedtuple('MethodHookData', [
    'methname',
    'in_params', 'ret_names',
    'pre', 'post'
])


def _process_enum(enum, clsname=None):

    name = enum['name']
    typename = name
    parent = 'm'
    
    if clsname:
        typename = '%s::%s' % (clsname, typename)
        parent = clsname.lower()
    
    ret = ['py::enum_<%s>(%s, "%s")' % (typename, parent, name)]
    
    for v in enum['values']:
        k = v['name']
        ret.append('  .value("%s", %s::%s)' % (k, typename, k))
    
    ret[-1] = ret[-1] + ';'
    return ret


def _process_fn(fn):
    return [] # TODO


operatorName ={
    'operator+=' :'__iadd__',
    'operator-=' :'__isub__',
    'operator*=' :'__imul__',
    "operator/=": "__itruediv__",
    'operator+' :'__add__',
    'operator-' :'__sub__',
    'operator*' :'__mul__',
    "operator/": "__truediv__",
    'operator==': "__eq__",
    'operator!=': "__ne__",
    'operator|=': "__ior__",
    'operator&=': "__iand__",
    'operator|': "__or__",
    'operator&': "__and__",
    # 'operator=': "__eq__",
    # 'operator()': "__call__",
    # 'operator[]': "__call__",
    # 'operator<<': "__repr__",

}

def process_params(parameters):
    params =[]
    params = [p["type"] for p in parameters]
    # for p in parameters:
    #         # print(p.keys())
    #         val = " const " if p['constant'] is 1 else ""
    #         val += p['raw_type']
    #         val += '&' if p['reference'] else ""
    #         val += '*' if p['pointer'] else ""
    #         params.append(val)
    return ','.join(params) if len(params) > 0 else ""

def process_hint(parameters):
    Lhint = []
    for p in parameters:
        n = f', \"{p["name"]}\"_a'
        if "defaultValue" in p:
            n += "=" + p['default'].replace(" ","")
        Lhint.append(n)
    return ''.join(Lhint)
    
import re
doc_tag_set = {}
def gen_doc_tag_(py_methname, class_name, parameters):
    letters = string.ascii_uppercase
    suffix = ''.join(random.choice(letters) for i in range(1))
    l = [p['raw_type'].replace("ibex::","").replace('std::', "").replace('tubex::', "").replace('<','_').replace(">", "_") for p in parameters]
    # print(l)
    l = [p.upper() for p in l]
    # print(l)
    # l = [p[:min(6, len(p))].upper() for p in l]
    
    l = ("_" +"_".join(l)) if len(l) > 0 else ""
    if py_methname in operatorName:
        py_methname = operatorName[py_methname].upper()
        py_methname = py_methname.replace("__", "")
    # ugly patch !
    py_methname = py_methname.replace("()", "_CALL")
    py_methname = py_methname.replace("=", "")
    py_methname = py_methname.replace("[]", "_INDEX")
    py_methname = py_methname.replace("<<", "")
    
    return f"DOCS_{class_name.upper()}_{py_methname.upper()}{l}"

def gen_doc_tag(py_methname, class_name, parameters):
    suffix = [""] + ["%d"%d for d in range(1,10)  ]
    # print(suffix)
    for s in suffix:
        tag = gen_doc_tag_(py_methname, class_name, parameters)+s
        # print(tag, tag in doc_tag_set)
        if tag in doc_tag_set:
            # print(tag, len(doc_tag_set))
            # print(doc_tag_set.keys())
            # exit()
            continue
        else:
            doc_tag_set[tag] = True
            return tag

def _process_method(clsname, meth, hooks, overloaded=False):

    # skip destructor
    if meth.get('destructor', False):
        return None
    
    ret = []
    # print(meth.keys   ())
    py_methname = meth['name']
    parameters = meth['parameters']
    
    doc_tag = gen_doc_tag(py_methname, clsname, parameters)
    # print(doc_tag)
    # doc_ref[doc_tag] = meth["doxygen"]

    # fix types that are enums
    for p in parameters:
        if p.get('enum'):
            p['raw_type'] = p['enum']
            p['type'] = p['enum']
    # constructor
    if py_methname == clsname:
        params = process_params(parameters)
        hint = process_hint(parameters)
        ret.append('  .def(py::init<%s>(),%s%s)' % (params, doc_tag, hint))
    else:
  
        overload = ''
        if overloaded:
            # overloaded method
            params = process_params(parameters)
            # print(py_methname, meth['rtnType'], meth.keys())
            # pprint(meth)
            overload = '(%s (%s::*)(%s) %s)' % (
                # "const" if meth["returns_const"] is 1 else "",
                meth['rtnType'],
                # "*" if meth["returns_pointer"] is 1 else "",
                clsname, params,
                "const" if meth["const"] is True else ""
            )
        hint = process_hint(parameters)
        # doc_tag = gen_doc_tag(py_methname, parameters)
        # doc_ref[doc_tag] = meth["doxygen"]
        if "operator" not in py_methname:
            # doc_tag = f'DOCS_{py_methname.upper()}'
            ret.append('  .def("%s", %s&%s::%s,%s%s)' % (py_methname, overload, clsname, py_methname, doc_tag, hint))
        else:
            params = process_params(parameters)
            # print(py_methname)
            if py_methname in  operatorName:
                s = f'  .def(\"{operatorName[py_methname]}\", []({clsname}& s,{params} o) {{ return s {py_methname[-2:]} o;}}, {doc_tag})'
            elif py_methname  ==  "operator()":
                s = f'  .def(\"__call__\", []({clsname}& s,{params} o) {{ return s(o);}}, {doc_tag})'
            elif py_methname  == "operator[]":
                s = f'  .def(\"{py_methname}\", []({clsname}& s,{params} o) {{ return s[o];}}, {doc_tag})'
            else:
                s = ""    
                # print("*"*80)
                # print(py_methname, overload, clsname, methname, params)
                # print("*"*80)

            ret.append(s)
    assert(len(ret) <= 1)
    if len(ret) == 0:
        return None
    ret = ret[0]
    if len(ret) > 80:
        i1 = ret.find("DOCS")
        p1, p2 = ret[:i1], ret[i1:]
        ret = p1 + '\n          ' + p2
    return {'string': ret, 'doc_tag':doc_tag, "doxygen" : meth.get("doxygen", "")}


def _process_class(cls, hooks):
    
    clsname = cls['name']
    varname = clsname.lower()
        
    ret = [
        {"string": 'py::class_<%s> %s(m, "%s" DOCS_%s);' % (clsname, varname, clsname, clsname.upper()), 
        'doc_tag': "DOCS_%s" % clsname.upper(),
        "doxygen" : cls.get("doxygen", "")
        }
    ]
    # ret = []
    # Collect methods first to determine if there are overloads
    # ... yes, we're ignoring base classes here
    methods = cls['methods']['public']
    if methods:
        # ret.append(varname)
        
        # collapse them to find overloads
        meths = collections.OrderedDict()
        for meth in methods:
            meths.setdefault(meth['name'], []).append(meth)
        
        # process it
        for ml in meths.values():
            if len(ml) == 1:
                v = _process_method(clsname, ml[0], hooks)
                if v is not None:
                    ret += [v]
            else:
                for mh in ml:                    
                    v = _process_method(clsname, mh, hooks, True)
                    if v is not None:
                        ret += [v]
        # exit()
        # ret[-1] = ret[-1] + ';'
    
    # for e in cls['enums']['public']:
    #     ret.append('')
    #     ret += _process_enum(e, clsname=clsname) 
    return ret

# def _process_class(cls, hooks):
    
#     clsname = cls['name']
#     varname = clsname.lower()
        
#     ret = ['py::class_<%s> %s(m, "%s");' % (clsname, varname, clsname)]
    
#     # Collect methods first to determine if there are overloads
#     # ... yes, we're ignoring base classes here
#     methods = cls['methods']['public']
#     if methods:
#         ret.append(varname)
        
#         # collapse them to find overloads
#         meths = collections.OrderedDict()
#         for meth in methods:
#             meths.setdefault(meth['name'], []).append(meth)
        
#         # process it
#         for ml in meths.values():
#             if len(ml) == 1:
#                 ret += _process_method(clsname, ml[0], hooks)
#             else:
#                 for mh in ml:                    
#                     ret += _process_method(clsname, mh, hooks, True)
                
#         ret[-1] = ret[-1] + ';'
    
#     for e in cls['enums']['public']:
#         ret.append('')
#         ret += _process_enum(e, clsname=clsname) 
    
#     return ret

def process_header(fname, hooks):
    '''Returns a list of lines'''
    
    header = CppHeaderParser.CppHeader(fname)
    output = []
    
    # for e in header.enums:
    #     output += _process_enum(e)
        # output.append('')
    
    # for fn in header.functions:
    #    output += _process_fn(fn)
    #    output.append('') 
    for cls in sorted(header.classes.values(), key=lambda c: c['line_number']):
        output += _process_class(cls, hooks)
        # print(output)
        # output.append('')
        
    return output

#
# Hooks
#

# Method hook parameters:
#   clsname: name of the class
#   method: a method dictionary from cppheaderparser
#   in_params: copy of method['parameters']
#   ret_names: variables to return
#   pre: statements to insert before function call
#   post: statements to insert after function call
#   .. returns True if method hook did something 

def _reference_hook(clsname, method, hook_data):
    
    parameters = method['parameters']
    refs = [p for p in parameters if p['reference']]
    if refs:
        hook_data.in_params[:] = [p for p in hook_data.in_params if not p['reference']]
        hook_data.pre.extend('%(raw_type)s %(name)s' % p for p in refs)
        hook_data.ret_names.extend(p['name'] for p in refs)
        return True
        
        

def _ctr_hook(clsname, method, hook_data):
    
    if method['returns'] == 'CTR_Code':
        hook_data.ret_names.remove('__ret')
        hook_data.post.append('CheckCTRCode(__ret)')
        return True



doc_header = """
/** 
 *  \file
 *  %s documentation header 
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */
 
 """

cpp_header = """
/** 
 *  \\file
 *  %s binding
 * ----------------------------------------------------------------------------
 *  \date       2020
 *  \\author     Simon Rohou, Benoît Desrochers
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_Tube.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include <pybind11/functional.h>

namespace py = pybind11;
using namespace pybind11::literals;
using py::class_;
using py::init;

using namespace tubex;
using ibex::Interval;
using ibex::IntervalVector;


void export_%s(py::module& m){

"""
# def process_module(module_name, headers, hooks):
#     for header in headers:
#         for k,v in process_header(header, hooks).items():
#             print(k, v)

def process_module(module_name, header, hooks):

    pybind_dict = process_header(header, hooks)
    m_name = Path(header.name.split("_")[-1]).stem
    outModuleFile = Path("/tmp") / f"tubex_py_{m_name}.cpp"
    outModuleDocsFile = Path("/tmp") / f"tubex_py_{m_name}_docs.h"
    
    with outModuleDocsFile.open("w") as fout:
        fout.write(doc_header % m_name )
        for d in pybind_dict:
            # print(d['string'])
            print(d["doc_tag"])
            fout.write('const char* %s=' % d["doc_tag"])
            fout.write(f'R"_docs(%s\n)_docs";\n\n' % d['doxygen'])
    with outModuleFile.open("w") as fout:
        fout.write(cpp_header % (m_name, m_name) )
        
        # fout.write('    py::module %s(m, "%s");' % (module_name, m_name))
        # fout.write('    %s' % module_name)
        # fout.write('\n')

        for d in pybind_dict:
            if "operator" not in d['string']:
                fout.write("    "+d['string'] + "\n")
            # fout.write('const char* %s=' % d["doc_tag"])
            # fout.write(f'R"_docs(%s\n)_docs";' % d['doxygen'])
        fout.write("\n")
        for d in pybind_dict:
            if "operator" in d['string']:
                fout.write("    "+d['string'] + "\n")
        fout.write(';\n')
            
        # fout.write('    return m.ptr();')
        fout.write('}')

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('module_name')
    parser.add_argument('header', type=Path)

    args = parser.parse_args()
    
    hooks = {}
    hooks['method_hooks'] = [_reference_hook, _ctr_hook]
    
    process_module(args.module_name, args.header, hooks)

if __name__ == '__main__':
    main()
