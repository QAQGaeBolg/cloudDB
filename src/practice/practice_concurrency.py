from multiprocessing import managers
import pymysql
import traceback
from multiprocessing import Pool, Manager, cpu_count
from multiprocessing.managers import BaseManager
import os, sys, time
import random

def creatTable():
    conn = pymysql.connect(
        host = 'localhost',
        port = 3306,
        user = 'zyf',
        passwd = 'DJzyf1528'
    )
    cur = conn.cursor()
    sql_database = f"""
    create database if not exists practice1 default charset utf8 collate utf8_general_ci;"""
    sql_table = f"""
    create table student(
        id int not null auto_increment,
        name varchar(20) not null,
        age int default 0
        tel varcher(13) unique not null,
        primary key(id)
    ) engine = innodb character set utf8;"""
    try:
        cur.execute(sql_database)
        conn.select_db('practice1')
        cur.execute("drop table if exists student;")
        cur.execute(sql_table)
    except Exception as e:
        print(e)
    else :
        print("successfully create database and table")
    cur.close()
    conn.close()

class MyMysql(object):
    def __init__(self):
        self.conn = pymysql.connect(
            host = 'localhost',
            port = 3306,
            user = 'zyf',
            passwd = 'DJzyf1528',
            db = "practice1",
            charset = 'utf8'
        )
        self.cur = self.conn.cursor()
    
    def executeSql(self, sql):
        try:
            res = self.cur.execute(sql)
            print("执行sql受影响的行数： ", res)
            self.conn.commit()
        except:
            traceback.print_exc()
    
    def quit(self):
        self.cur.close()
        self.conn.commit()
        self.conn.close()

class MyManager(BaseManager): pass

def my_Manager():
    m = MyManager()
    m.start()
    return m

MyManager.register('MyMysql', MyMysql)

def run(my_sql):
    print('subprocess is', os.getpid())
    name = 'ha' + str(random.randomint(1, 100)) + '_' + str(os.getpid())
    age = random.randint(1, 100)
    tel = '1' + str(random.choice([3, 5, 7, 8])) + str(random.random())[2 : 11]
    sql = f"""
    insert intostudent(name, age, tel) values('{name}', '{age}', '{tel}');
    """
    my_sql.executeSql(sql)

if __name__ == '__main__':
    creatTable()
    manager = my_Manager()
    my_mysql = manager.MyMysql()
    print('Parent process', os.getpid())
    p = Pool(cpu_count())
    n = 10
    for i in range(n):
        p.apply(run, args = (my_mysql, ))
        p.close()
        p.join()
    print('All subprocess done!')
    my_mysql.quit()
        

        
