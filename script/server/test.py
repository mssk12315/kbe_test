# -*- coding: utf-8 -*-
import os
import KBEngine

#"""
class MyScriptA:
	def __init__(self):
		self.s = KBEngine.MyScript()

	def test2(self, a):
		self.s.test("kbengine test " + a);
        #print("test" + a);
#"""

"""
class MyScriptA:
        def test(self, a):
                print("test" + a);
"""

def testScript(isBootstrap):
	print("Hello");
	s = MyScriptA();
	s.test2("nihao");
	s2 = KBEngine.MyScript("");
	s2.test("nihao2");
	s2.test("update ok f56")
	del s2;
#   print("test script");
