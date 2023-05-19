from setuptools import setup, Extension
from torch.utils import cpp_extension

setup( name='linear_function_cpp',
       ext_modules=[cpp_extension.CppExtension('linear_function_cpp', ['Function.cpp'])],
       cmdclass={'build_ext': cpp_extension.BuildExtension})