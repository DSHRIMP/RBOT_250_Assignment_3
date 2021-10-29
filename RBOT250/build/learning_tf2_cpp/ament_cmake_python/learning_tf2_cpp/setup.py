import os
from setuptools import find_packages
from setuptools import setup

setup(
    name='learning_tf2_cpp',
    version='0.0.0',
    packages=find_packages(
        include=('learning_tf2_cpp', 'learning_tf2_cpp.*')),
)
