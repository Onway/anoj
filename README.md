# anoj
Online Judge的评测程序

## 基本功能
* 支持语言：c，c++，java，python
* 资源限制：时间，内存，输出
* 多组输入
* Special Judge

## 功能结构
* 评测请求通过HTTP提交至python脚本
* 脚本负责对源码进行编译，然后调用judger程序
* 脚本读取judger的结果输出，发送至指定的http页面

## 依赖
    sudo apt-get install pkg-config  
    sudo apt-get install libglib2.0-dev 
    sudo apt-get install openjdk-7-jdk  
    sudo apt-get install python2.7
    sudo apt-get install g++-4.6
    sudo apt-get install gcc-4.6
    sudo apt-get install pyflakes
    sudo apt-get install mini-httpd

## 单独运行judger程序
    git clone https://github.com/Onway/anoj.git
    cd anoj/judge
    make
    sudo make install
    
    # 测试目录，需要包含.in输入文件和.ans答案文件
    mkdir /tmp/test 
    touch /tmp/test/a.in
    date +%D > /tmp/test/a.ans

    # 注意是judger不是./judger
    judger -t 1000 -m 65536 -f 4096 -l c++ -w /tmp -d /tmp/test -- date +%D
    
    sudo make uninstall
    make clean
    rm -rf /tmp/test

## 安装测试
    git clone https://github.com/Onway/anoj.git
    cd anoj
    sudo ./install.sh

    # 运行http服务器，可查看demo测试页面：http://127.0.0.1:8888/
    sudo mini_httpd -C /etc/mini-httpd.conf

    # 以anojer用户运行测试
    sudo su anojer
    
    # 在~/data目录生成输入和答案文件
    cd ~/test
    ./datalist.py datalist.xml ~/data
    
    # 运行自动测试
    ./solution.py solution.xml
    # 输出自动测试结果页面
    ./solution.py solution.xml ~/history/result > /tmp/result.html

## 卸载
    # 先停止http服务器
    sudo kill `cat /var/run/mini-httpd.pid`

    cd anoj
    sudo ./uninstall.sh
    # 或者直接删掉
    rm -f /usr/bin/judger
    rm -rf /etc/anoj
    userdel -r anojer

## 注意
* judger程序的沙箱功能主要是通过降权、设置资源限制和拦截系统调用
* 没有使用chroot，因为链接库不好处理
* 对java程序没有拦截系统调用，而是使用了policy文件

## BUGS
* 目前只在64位的ubuntu 14.04测试过，32位的系统会编译失败
（user_regs_struct.orig_eax的问题）
* 测试用例中发现有失败的（T_T）
