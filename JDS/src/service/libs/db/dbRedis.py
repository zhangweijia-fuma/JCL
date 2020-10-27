#coding:utf-8
import config,ujson,redis,msgpack

'''
Mysql 主
Redis 从
 基本结构应该是：
	1. 存入MySQL-- > 复制到Redis
	2. 查询Redis -- > 查询MySQL
'''
def operator_status(func):
	#得到操作状态
	def gen_status(*args, **kwargs):
		error, result = None, None
		try:
			result = func(*args, **kwargs)
		except Exception as e:
			error = str(e)

		return {'result': result, 'error':  error}

	return gen_status

class RedisCache(object):
	def __init__(self):
		if not hasattr(RedisCache, 'pool'):  # 如果RedisCache中无属性'pool'，则创建
			RedisCache.create_pool(self,config.RedisConfig)
		self._connection = redis.Redis(connection_pool = RedisCache.pool)

	@staticmethod
	def create_pool(self,config):
		RedisCache.pool = redis.ConnectionPool(**config)

	def set_object(self, key, obj, to=None):
		return self.set_data(key, ujson.dumps(obj), to)

	def get_object(self, key):
		r = self.get_data(key);
		if not r:
			return r
		if not r or r['error'] or not r['result']:
			return None;
		r = ujson.loads(r['result']);
		return r

	@operator_status
	def set_data(self, key, value, to=None):
		#设置KV数据(key, value)
		r = self._connection.set(key, value)
		if not r:
			return r
		if to:
			self._connection.expire(key, to)
		return r

	def get_ttl(self, key):
		#获取key超时时间
		try:
			r = self._connection.ttl(key)
			return r
		except Exception as e:
			return 0

	@operator_status
	def set_expire(self, key, timeout):
		#设置key超时时间
		return self._connection.expire(key, timeout)

	@operator_status
	def get_data(self, key):
		#根据key得到value
		return self._connection.get(key)

	@operator_status
	def del_data(self, key):
		#删除key
		return self._connection.delete(key)

	@operator_status
	def jsonToString(self,jsonData) :
		return ' '.join('%s %s' % (k,v) for k,v in jsonData.items())

	@operator_status
	def hmset(self,key,data) :
		return 'HMSET %s '+ RedisCache.jsonToString(self, data)

	@operator_status
	def save_list(self,table,data,id=None,RL='L'):
		if id is None : id = ''
		luaScript_Save = '''
			if #ARGV[1]==0 or ARGV[1]='{}' then
			   return 0
			end

			local key
			local data=ARGV[1]
			if KEYS[2]=='' then
			   key=KEYS[1]
			else
			   key = KEYS[1]..':'..KEYS[2]
			end
			if KEYS[3]=='R' then
			   redis.call("rpush", key,data)
			else
			   redis.call("lpush", key,data)
			end
			return 1
		'''
		ls=self._connection.register_script(luaScript_Save)
		return ls(keys=[table,id,RL],args=[ujson.dumps(data)])


	@operator_status
	def save(self,table,data,id='id') :
		luaScript = '''
			local tab = {}
			local id=KEYS[2]

			local key = KEYS[1]..':'..id
			--local id  = 1  -- 如果是修改记录，则返回的值为1

			local value=cjson.decode(ARGV[1])

			for k,v in pairs(value) do
				if  type(v)=='userdata'  then
					v=''
				end
				tab[#tab + 1] = k
				tab[#tab + 1] = v
			end

			redis.call("SELECT", KEYS[3])
			if redis.call("EXISTS", key) ~= 1 then
			   -- 如果是新增记录，COUNT 计数器+1
			   if KEYS[2]=='id' then
				  id=redis.call("INCR", KEYS[1]..':AUTOID')
			   end
			   redis.call("INCR", KEYS[1]..':COUNT')
			   -- 将id添加到list列表左侧
			   redis.call("LPUSH", KEYS[1]..":LIST",id)
			   key=KEYS[1]..':'..id
			end

			-- 新增或更改记录
			redis.call("HMSET", key,unpack(tab))
			return id
		'''
		ls=self._connection.register_script(luaScript)
		r=ls(keys=[table,id,config.TableToRedisNo[table]],args=[ujson.dumps(data)])
		return r

	@operator_status
	def delete(self,table,id) :
		luaScript = '''
			local key = KEYS[1]..':'..KEYS[2]

			local rw=redis.call("DEL",key)
			if rw==1 then
			   redis.call("DECR",KEYS[1]..':COUNT')
			   redis.call("LREM",KEYS[1]..':LIST',1,KEYS[2])
			end

			return rw
		'''
		#self._connection.eval(luaScript,2,table,id,data)
		ls=self._connection.register_script(luaScript)
		return ls(keys=[table,id])


	@operator_status
	def get(self,table,id) :
		luaScript= '''
			local key = KEYS[1]..':'..KEYS[2]
			local r=redis.call("HGETALL",key)
			local t={}
			for i=1,#r,2 do
				t[r[i]]=r[i+1]
			end
			return cmsgpack.pack(t)
		'''
		ls=self._connection.register_script(luaScript)
		info=msgpack.unpackb(ls(keys=[table,id]),encoding='utf-8')
		return info