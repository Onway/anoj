import os
import random
import select
import traceback
import subprocess
import commands

class Compiler:
    def __init__(self, lang, code, idstr, workdir='/tmp/', timeout=5):
        self.lang = lang
        self.code = code
        self.workdir = workdir
        self.timeout = timeout
        self.idstr = idstr

    def run(self):
        curdir = os.curdir

        os.chdir(self.workdir)
        if self.lang == "c":
            srcfile = self.idstr + ".c"
            binfile = self.idstr
            cmd = "gcc -o %s %s --static" % (binfile, srcfile)
        elif self.lang == "c++":
            srcfile = self.idstr + ".cpp"
            binfile = self.idstr
            cmd = "g++ -o %s %s --static" % (binfile, srcfile)
        elif self.lang == "java":
            javadir = self.idstr
            srcfile = "Main.java"
            binfile = "Main"
            os.mkdir(javadir)
            os.chdir(javadir)
            cmd = "javac %s" % srcfile
        elif self.lang == "python":
            srcfile = self.idstr + ".py"
            cmd = "pyflakes %s" % srcfile
        else:
            os.chdir(curdir)
            return {"result": "Compile Error", "debug": "Unsupported language"}

        srcfd = open(srcfile, "w", 0644)
        srcfd.write(self.code)
        srcfd.close()

        try:
            popen = subprocess.Popen(cmd.split(), stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT)
        except OSError:
            os.chdir(curdir)
            return {"result": "Internal Error",
                    "debug": traceback.format_exc()}
        else:
            os.chdir(curdir)

        fs = select.select([popen.stdout], [], [], self.timeout)
        if popen.stdout in fs[0]:
            msg = popen.stdout.read()
        else:
            popen.terminate()
            return {"result": "Compile Error", "msg": "Compile too long time"}
        
        if popen.poll() != 0:
            return {"result": "Compile Error", "msg": msg}

class Judger:
    def __init__(self, time, memory, outsize, lang, datadir, workdir, idstr,
                javapolicy="/etc/wyuoj/java.policy"):
        self.time = time
        self.memory = memory
        self.outsize = outsize
        self.lang = lang
        self.datadir = datadir
        self.workdir = workdir
        self.idstr = idstr

    def run(self):
        cmd = "judger -t %s -f %s -l %s -d %s " % (self.time, self.outsize,
                self.lang, self.datadir)

        if self.lang != "java":
            cmd += "-m %s " % self.memory
            gotodir = self.workdir
        else:
            gotodir = os.path.join(self.workdir, self.idstr)
        cmd += "-w %s -- " % gotodir

        if self.lang == "c" or self.lang == "c++":
            cmd += "./%s" % self.idstr
        elif self.lang == "python":
            cmd += "python -S %s.py" % self.idstr
        elif self.lang == "java":
            cmd += "java -Xms%dk -Xmx%dk -Djava.security.manager -Djava.security.policy=%s Main" % (int(self.memory), int(self.memory) * 2, self.javapolicy)
        else:
            pass
            
        status, output = commands.getstatusoutput(cmd)
        if status == 0:
            kdict = {}
            for i in output.split("\n"):
                if i != "":
                    t = i.split(":")
                    kdict[t[0]] = t[1]
            return kdict
        else:
            return {"result": "Internal Error", "debug": "%s error" % cmd}

class Tasker:
    def __init__(self, workdir):
        self.workdir = workdir

    def start(self):
        self.idstr = self.__randstr()

    def finish(self):
        curdir = os.curdir
        os.chdir(self.workdir)
        os.system("rm -rf %s*" % self.idstr)
        os.chdir(curdir)

    @staticmethod
    def __randstr(size=16):
        st = ""
        for i in range(size + 1):
            st = st + chr(97 + random.randint(0, 25))
        return st

class Networker:
    def __init__(cgi_form, config_parser):
        self.form = cgi_form
        self.cf = config_parser

    def revc_task(self):
        self.rid = self.form.getvalue("rid", "")
        self.pid = self.form.getvalue("pid", "")
        self.time = self.form.getvalue("time", "1000")
        self.memory = self.form.getvalue("memory", "32768")
        self.outsize = self.form.getvalue("outsize", "1024")
        self.lang = self.form.getvalue("lang", "c")
        self.code = self.form.getvalue("code", "")

        self.allow_ip = self.cf.get("DEFAULT", "FROM").split(",")
        self.to_ip = self.cf.get("DEFAULT", "TO")
        self.workdir = self.cf.get("DEFAULT", "WORKDIR")
        self.datadir = self.cf.get("DEFAULT", "DATADIR")

        if "REMOTE_ADDR" in os.environ:
            remote_addr = os.environ["REMOTE_ADDR"]
        else:
            remote_addr = ""
        
        if remote_addr not in self.allow_ip or\
                self.pid == "" or self.rid == "" or self.code == "":
            return False
        return True

    def send_result(**kdict):
        kdict["rid"] = self.rid 
        kdict["pid"] = self.pid

        if not kdict.has_key("time"):
            kdict["time"] = 0
        if not kdict.has_key("memory"):
            kdict["memory"] = 0
        if not kdict.has_key("codelen"):
            kdict["codelen"] = len(self.code)
        if not kdict.has_key("result"):
            kdict["result"] = "Internal Error"
        if not kdict.has_key("msg"):
            kdict["msg"] = ""
        if not kdict.has_key("debug"):
            kdict["debug"] = ""
        urllib2.urlopen(urllib2.Request(self.to_ip, urllib.urlencode(kdict)))

if __name__ == "__main__":
#    codefd = open("/tmp/code.c", "r")
#    code = codefd.read()
#    print Compiler("c", code, "123456").run()

#    time = 1
#    memory = 10000
#    outsize = 1000
#    lang = "c"
#    datadir = "/home/wyuojer/data/1000"
#    workdir = "/tmp"
#    idstr = "123456"
#
#    judger = Judger(time, memory, outsize, lang, datadir, workdir, idstr)
#    print judger.run()

#    tasker = Tasker("/tmp")
#    tasker.start()
#    print tasker.idstr
#    tasker.finish()
