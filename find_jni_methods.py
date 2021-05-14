import sys
import os

from binaryninja import (
    SymbolType, Endianness, Type, BackgroundTaskThread,
    Symbol, SymbolType, Architecture)

SCRIPTDIR  = os.path.dirname(os.path.realpath(__file__))
JNI_ONLOAD = "JNI_OnLoad"

def print_err(msg):
    sys.stderr.write(f"{msg}\n")

class FindJNIFunctionAnalysis(BackgroundTaskThread):
    def __init__(self, bv):
        BackgroundTaskThread.__init__(self, "Finding JNI Functions", False)
        self.bv = bv

        self.jni_functions = list()
        self.jni_onload = None

    def update_progress(self, phase_name, curr, total):
        self.progress = f"Finding JNI Functions ({phase_name}): {curr} / {total}"

    def is_pointer(self, addr):
        return self.bv.read(addr, 1) != b""

    def read_pointer(self, addr):
        data = self.bv.read(addr, self.bv.arch.address_size)
        value = int.from_bytes(data, 'little' if self.bv.arch.endianness == Endianness.LittleEndian else 'big')
        return value

    def get_string(self, address):
        i = 0
        s = ""
        s_ptr = address
        while 1:
            b = self.bv.read(s_ptr + i, 1)
            if b == b"\x00":
                break
            if b == b"" or not b.isascii():
                return None
            s += b.decode("ascii")
            i += 1
        return s

    def is_address_of_function(self, address, function):
        funcs = self.bv.get_functions_at(address)
        for fun in funcs:
            if fun == function:
                return True
        return False

    def define_JNI_types(self):
        with open(os.path.join(SCRIPTDIR, "jni.h"), "r") as fin:
            data = fin.read()

        jni_types = self.bv.platform.parse_types_from_source(data)
        for type_name in jni_types.types:
            type_obj = jni_types.types[type_name]
            self.bv.define_user_type(type_name, type_obj)

    def find_dynamic_jni(self):
        if JNI_ONLOAD not in self.bv.symbols:
            print_err(f"[!] \"{JNI_ONLOAD}\" not in symbols")
            return

        jni_onload_symbol = self.bv.symbols[JNI_ONLOAD][0]
        if jni_onload_symbol.type != SymbolType.FunctionSymbol:
            print_err(f"[!] \"{JNI_ONLOAD}\" is not a function")
            return

        funcs = self.bv.get_functions_at(jni_onload_symbol.address)
        if len(funcs) > 0:
            self.jni_onload = funcs[0]

        sections = [".data", ".rodata"]
        for section_name in sections:
            if section_name not in self.bv.sections:
                print_err("[+] \"%s\" is not a section" % section_name)
                continue
            data_section = self.bv.sections[section_name]

            i = 0
            n_addresses = data_section.end - data_section.start
            for addr in range(data_section.start, data_section.end):
                i += 1
                self.update_progress("dynamic", i, n_addresses)

                if not self.is_pointer(addr):
                    continue
                method_name_ptr = self.read_pointer(addr)
                method_name = self.get_string(method_name_ptr)
                if method_name is None:
                    continue

                if not self.is_pointer(addr + self.bv.arch.address_size):
                    continue
                method_signature_ptr = self.read_pointer(addr + self.bv.arch.address_size)
                method_signature = self.get_string(method_signature_ptr)
                if method_signature is None or len(method_signature) == 0:
                    continue
                if method_signature[0] != "(" or ")" not in method_signature:
                    continue

                if not self.is_pointer(addr + self.bv.arch.address_size*2):
                    continue
                method_ptr = self.read_pointer(addr + self.bv.arch.address_size*2)

                funcs = self.bv.get_functions_at(method_ptr)
                if len(funcs) == 0:
                    # Create the function
                    plat = None
                    if self.bv.arch.name == "armv7" and method_ptr % 2 == 1:
                        thumb2 = Architecture["thumb2"]
                        plat   = self.bv.platform.get_related_platform(thumb2)
                        method_ptr -= 1
                    self.bv.create_user_function(method_ptr, plat)
                    funcs = self.bv.get_functions_at(method_ptr)

                fun = funcs[0]
                self.jni_functions.append(fun)

                self.bv.define_user_symbol(Symbol(SymbolType.DataSymbol, addr, f"{method_name}_struct"))
                self.bv.define_user_data_var(addr, self.bv.types["JNINativeMethod"])
                self.bv.define_user_data_var(method_name_ptr,
                    Type.array(Type.char(), len(self.get_string(method_name_ptr)) + 1))
                self.bv.define_user_data_var(method_signature_ptr,
                    Type.array(Type.char(), len(self.get_string(method_signature_ptr)) + 1))

    def find_static_jni(self):
        n_functions = len(self.bv.functions)
        for i, fun in enumerate(self.bv.functions):
            self.update_progress("static", i, n_functions)
            if fun.name.startswith("Java_"):
                self.jni_functions.append(fun)

    @staticmethod
    def _build_function_type(fun, params: dict, out=None):
        out_type    = out or fun.return_type
        param_types = list()
        for param_idx, param_var in enumerate(fun.parameter_vars):
            if param_idx in params:
                param_types.append(params[param_idx])
            else:
                if param_var.type is None:
                    param_types.append(Type.int(4))
                else:
                    param_types.append(param_var.type)

        return Type.function(out_type, param_types)

    def apply_types(self):
        jnienv_ptr_type = Type.pointer(self.bv.arch, self.bv.types["JNIEnv"])
        javavm_ptr_type = Type.pointer(self.bv.arch, self.bv.types["JavaVM"])

        n_functions = len(self.jni_functions)
        for i, fun in enumerate(self.jni_functions):
            self.update_progress("applying types", i, n_functions)
            if len(fun.parameter_vars) == 0:
                continue

            fun_type = FindJNIFunctionAnalysis._build_function_type(fun, {0: jnienv_ptr_type})
            fun.set_user_type(fun_type)

        if self.jni_onload is not None:
            fun_type = FindJNIFunctionAnalysis._build_function_type(self.jni_onload, {0: javavm_ptr_type})
            self.jni_onload.set_user_type(fun_type)

    def run(self):
        self.bv.update_analysis_and_wait()
        self.define_JNI_types()

        self.find_dynamic_jni()
        self.find_static_jni()

        self.apply_types()

        print("Found %d JNI functions" % len(self.jni_functions))
