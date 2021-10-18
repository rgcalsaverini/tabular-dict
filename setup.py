import pathlib

from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig
import sys
import setuptools
import tempfile
import os
import subprocess


class CMakeExtension(Extension):

    def __init__(self, name):
        # don't invoke the original build_ext for this special extension
        super().__init__(name, sources=[])


class build_ext(build_ext_orig):

    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        super().run()

    def build_cmake(self, ext):
        cwd = pathlib.Path().absolute()
        subprocess.run(['cmake', '.'], check=True, cwd=cwd, capture_output=True)
        subprocess.run(['cmake', '--build', '.'], check=True, cwd=cwd, capture_output=True)


setup(
    name='flatten_json',
    version='0.0.1',
    author='Celus',
    packages=[''],
    package_dir={'': '.'},
    ext_modules=[CMakeExtension('flatten_json')],
    cmdclass={'build_ext': build_ext},
    package_data={'': ['flatten_json.cpython-38-x86_64-linux-gnu.so']},
    zip_safe=False,
)
