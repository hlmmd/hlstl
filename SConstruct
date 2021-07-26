#coding=utf-8
import os
import sys
Env = Environment()

currentPath = os.path.abspath('.')

Env.Append(CPPPATH=[currentPath])
Export('Env')
SConscript('hlstl_unittest/SConscript')