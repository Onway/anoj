import os
import signal

s = raw_input()
os.kill(os.getpid(), signal.SIGUSR1)
print s
