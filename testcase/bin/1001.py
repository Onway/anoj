import os
import signal

s = raw_input()
os.kill(os.getpid(), signal.SIGRTMIN+1)
print s
