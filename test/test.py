from ctypes import cdll
lib = cdll.LoadLibrary('./split_test.so')

ping = "-pcm=../data/wakeup.pcm  -model=../model/wakeup_ja.fuet  -record"
lib.test(ping.encode("UTF-8"))


