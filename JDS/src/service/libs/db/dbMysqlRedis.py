#coding:utf-8
import web.dbRedis

class MysqlToRedis :
    
    def __init__(db,rdb) :
        self.db=db
        self.rdb=rdb

    def add(self,table,data,commit=True):
        
        #1.加入到数据库
        id=self.db.insert(table,data,commit)
        if id<0 : raise BaseError(703) # SQL执行错误
        
        #2.加入到Redis,id取自数据库
        key=table+':%s'+id
        self.rdb.save(key,data)
        return id
    
    def update(self,table,data,ids,pk='id',commit=True,lock=True):
        #1.根据id列表更新数据
        rw=db.updateByPk(table,data,'{{ in (%s)}}'%(ids),pk,commit,lock)      
        if id<0 : raise BaseError(705) # SQL执行错误

        #2.数据更新到Redis
        key=table+':'
        self.rdb.update(key,data,ids,pk)
        return rw
    
    def delete(self,table,data,ids,pk='id',commit=True,lock=True):
        #1.数据更新到数据库
        rw=db.updateByPk(table,
                         {'isDelete':'Y','deleteTime':'{{now()}}'},
                         '{{ in (%s)}}'%(ids),pk,commit,lock)      
        if rw<0 : raise BaseError(705) # SQL执行错误

        #2.数据更新到Redis
        key=table+':'
        self.rdb.delete(key,data,ids,pk)
        return rw
    
    def getToObjectByPk(self,table,id) :
        return self.rdb.get(table+':'+id)
