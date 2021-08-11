#coding=utf-8
import os
import sys
Env = Environment()
#Env = Environment(CXXFLAGS="-std=c++11")

currentPath = os.path.abspath('.')

Env.Append(CPPPATH=[currentPath])
Export('Env')
SConscript('tests/SConscript')
