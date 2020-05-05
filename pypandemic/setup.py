from setuptools import setup, Extension
import os
from Cython.Build import cythonize

#os.environ["CXX"] = "/usr/bin/g++"
#os.environ["C"] = "/usr/bin/g++"

#setup(ext_modules=cythonize("entities.pyx", language='c++', extra_compile_args=["-std=c++11"]))
ext_module = Extension(
"entities",
["entities.pyx"],
#extra_objects=["libpandemicentities.a"],
#library_dirs=["core/"],
extra_compile_args=["-std=c++11"],
extra_link_args=["-std=c++11"]
)
setup(ext_modules=cythonize(ext_module))
