from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "main_cpp",
        ["main_cpp.cpp"],
        include_dirs=[pybind11.get_include()],
        language="c++"
    ),
]

setup(
    name="main_cpp",
    ext_modules=ext_modules,
)
