#!/usr/bin/env python
#coding:utf-8

import threading
import time
import random

def worker_func():
    print('working thread is %s' % threading.current_thread())
    random.seed()
    time.sleep(random.random())
    print('finished thread is %s' % threading.current_thread())


def simple_thread_demo():
    for i in range(10):
        threading.Thread(target=worker_func).start()


glock = threading.Lock()
gsem = threading.Semaphore(3)

#
def work_func_lock(Lock):
    Lock.acquire()
    worker_func()
    Lock.release()


gMoney = 1000
gCondition = threading.Condition()

class Producer(threading.Thread):
    def run(self):
        print("")
        while True:

            global gMoney
            global gCondition


            gCondition.acquire()
            random.seed()
            p = random.randint(500, 1000)
            gMoney += p;
            time.sleep(random.random())
            print("%s: produce %s, left %s"% (threading.current_thread(), p, gMoney))
            gCondition.notify_all()
            gCondition.release()


class Consumer(threading.Thread):
    def run(self):
        print("")
        while True:
            global gMoney
            global gCondition

            gCondition.acquire()
            random.seed()
            c = random.randint(100, 200)
            print("%s: try take %s, left %s"% (threading.current_thread(), c, gMoney))
            while gMoney < c:
                gCondition.wait()
            gMoney -= c
            print("%s: take %s, left %s"% (threading.current_thread(), c, gMoney))
            time.sleep(random.random())
            gCondition.release()

def consumer_producer_demo():
    for i in range(10):
        Consumer().start()

    for i in range(1):
        Producer().start()


def thread_func():
    for i in range (10):
        threading.Thread(target=work_func_lock, args=[gsem]).start()

if __name__ == '__main__':
    #simple_thread_demo()
    #thread_func()
    consumer_producer_demo()