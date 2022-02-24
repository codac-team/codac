#!/usr/bin/env python3
#
# Apache 2 license
#

import argparse
import collections

import CppHeaderParser
from  pprint import pprint

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
    'operator!=': "__neq__",
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
    for p in parameters:
            print(p.keys())
            val = " const " if p['constant'] is 1 else ""
            val += p['raw_type']
            val += '&' if p['reference'] else ""
            val += '*' if p['pointer'] else ""
            params.append(val)
    return ','.join(params) if len(params) > 0 else ""

def process_hint(parameters):
    Lhint = []
    for p in parameters:
        n = f', \"{p["name"]}\"_a'
        if "defaultValue" in p:
            n += "=" + p['default'].replace(" ","")
        Lhint.append(n)
    return ''.join(Lhint)
    

def _process_method(clsname, meth, hooks, overloaded=False):

    # skip destructor
    if meth.get('destructor', False):
        return []
    
    ret = []
    doc_ref = {}
    # print(meth.keys   ())
    methname = meth['name']
    parameters = meth['parameters']
    # if not (methname == clsname):
        # return ret
    # fix types that are enums
    for p in parameters:
    #     for k,v in p.items():
    #         if k in ['method']:
    #             continue
    #         print("*"*80)
    #         print(k)
    #         if isinstance(v, dict):
    #             pprint(v)
    #         else:
    #             print(type(k))
    #     print(p.keys())
    #     print(p.get("defaultValue"))
    #     print(p['raw_type'], p['desc'])
        if p.get('enum'):
            p['raw_type'] = p['enum']
            p['type'] = p['enum']
    # exit()
    # constructor
    if methname == clsname:
        # params = ','.join(p['raw_type'] for p in parameters)
        params = process_params(parameters)
        hint = process_hint(parameters)
        # args = ','.join(p['name']+"_a")
        ret.append('  .def(py::init<%s>()%s)' % (params, hint))
        # return ret
    else:
        # return ret
        
        # pre = []
        # post = []
        # ret_names = []
        # if meth['returns'] != 'void':
        #     ret_names.append('__ret')
        
        # in_params = parameters[:]
        
        # modified = False
        
        # # data that hooks can modify
        # hook_data = MethodHookData(methname,
        #                            in_params, ret_names,
        #                            pre, post)
        
        # for hook in hooks.get('method_hooks', []):
        #     if hook(clsname, meth, hook_data):
        #         modified = True
        
        # py_methname = hook_data.methname
        py_methname = methname

        # modified = False
        # if modified:
            
        #     in_args = ''
        #     if in_params:
        #         in_args = ', ' + ', '.join('%(type)s %(name)s' % p for p in in_params)
            
        #     ret.append('  .def("%(py_methname)s", [](%(clsname)s &__inst%(in_args)s) {' % locals())
            
        #     if pre:
        #         ret.append('    ' + '; '.join(pre) + ';')
            
        #     meth_params = ', '.join(p['name'] for p in meth['parameters'])
            
        #     fnret = 'auto __ret = '
            
        #     if '__ret' not in ret_names:
        #         fnret = ''
            
        #     if not post:
        #         if ret_names == ['__ret']:
        #             ret_names = []
        #             fnret = 'return '
            
        #     ret.append('    %(fnret)s__inst.%(methname)s(%(meth_params)s);' % locals())
            
        #     if post:
        #         ret.append('    ' + '; '.join(post) + ';')
                        
        #     if len(ret_names) == 0:
        #         pass
        #     elif len(ret_names) == 1:
        #         ret.append('    return %s;' % ret_names[0])
        #     else:
        #         ret.append('    return std::make_tuple(%s);' % ', '.join(ret_names))
                
        #     ret.append('  })')
            
        # else:
        overload = ''
        if overloaded:
            # overloaded method
            params = process_params(parameters)
            print(py_methname, meth['rtnType'], meth.keys())
            # pprint(meth)
            overload = '(%s (%s::*)(%s) %s)' % (
                # "const" if meth["returns_const"] is 1 else "",
                meth['rtnType'],
                # "*" if meth["returns_pointer"] is 1 else "",
                clsname, params,
                "const" if meth["const"] is True else ""
            )
        hint = process_hint(parameters)
        doc_tag = gen_doc_tag(py_methname, parameters)
        doc_ref[doc_tag] = meth["doxygen"]
        if "operator" not in py_methname:
            # doc_tag = f'DOCS_{py_methname.upper()}'
            ret.append('  .def("%s", %s&%s::%s,%s%s)' % (py_methname, overload, clsname, methname, doc_tag, hint))
        else:
            params = process_params(parameters)
            if py_methname in  operatorName:
                s = f'//  .def(\"{operatorName[py_methname]}\", []({clsname}& s,{params} o) {{ return s {py_methname[-2:]} o;}})'
                # print("*"*80)
                # print(py_methname, overload, clsname, methname, params)
                # print("*"*80)

                ret.append(s)
        
    return ret

def gen_doc_tag(py_methname, parameters):
    letters = string.ascii_uppercase
    suffix = ''.join(random.choice(letters) for i in range(4))
    l = [p['raw_type'][:3].upper() for p in parameters]
    l = ("_" +"_".join(l)) if len(l) > 0 else ""
    return f"DOCS_{py_methname.upper()}{l}"

def _process_class(cls, hooks):
    
    clsname = cls['name']
    varname = clsname.lower()
        
    ret = ['py::class_<%s> %s(m, "%s");' % (clsname, varname, clsname)]
    
    # Collect methods first to determine if there are overloads
    # ... yes, we're ignoring base classes here
    methods = cls['methods']['public']
    if methods:
        ret.append(varname)
        
        # collapse them to find overloads
        meths = collections.OrderedDict()
        for meth in methods:
            meths.setdefault(meth['name'], []).append(meth)
        
        # process it
        for ml in meths.values():
            if len(ml) == 1:
                ret += _process_method(clsname, ml[0], hooks)
            else:
                for mh in ml:                    
                    ret += _process_method(clsname, mh, hooks, True)
                
        ret[-1] = ret[-1] + ';'
    
    for e in cls['enums']['public']:
        ret.append('')
        ret += _process_enum(e, clsname=clsname) 
    
    return ret

def process_header(fname, hooks):
    '''Returns a list of lines'''
    
    header = CppHeaderParser.CppHeader(fname)
    output = []
    
    for e in header.enums:
        output += _process_enum(e)
        output.append('')
    
    #for fn in header.functions:
    #    output += _process_fn(fn)
    #    output.append('') 
    
    for cls in sorted(header.classes.values(), key=lambda c: c['line_number']):
        output += _process_class(cls, hooks)
        output.append('')
        
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


def process_module(module_name, headers, hooks):

    print()
    print('#include <pybind11/pybind11.h>')
    print('namespace py = pybind11;')
    print()
    
    for header in headers:
        print('#include <%s>' % header) # TODO, not usually the actual path
        
    print()
    print('PYBIND11_PLUGIN(%s) {' % module_name)
    print()
    print('    py::module m("%s");' % module_name)
    print()

    for header in headers:
        print('    ' + '\n    '.join(process_header(header, hooks)))
        print()
        
    print('    return m.ptr();')
    print('}')

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('module_name')
    parser.add_argument('headers', nargs='+')
    
    args = parser.parse_args()
    
    hooks = {}
    hooks['method_hooks'] = [_reference_hook, _ctr_hook]
    
    process_module(args.module_name, args.headers, hooks)

if __name__ == '__main__':
    main()
