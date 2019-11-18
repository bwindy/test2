from ctypes import CDLL
lib = CDLL("./split_test.so")
lib.test("-pcm=../data/wakeup.pcm  -model=../model/wakeup_ja.fuet  -record")

