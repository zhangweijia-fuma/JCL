#coding:utf-8
from libs.web.Base  import WebRequestHandler,BaseError,operator_except
from system.menu import menuManage
import time

# 获取用户菜单
# 参数：
# 	id： 用户ID
class Restful(WebRequestHandler):

	# 获取用户菜单

	@operator_except
	def get(self):

		uid = self.get_argument('id', default='') 

		if uid == '':
			raise BaseError(801)  #参数错误

		cur  = self.db.getCursor()
		user = self.objUserInfo 

		sql = """
			select distinct(ugmu.menu_item_id) 
			 from system.user_group_member ugm,system.user_group_menu ugmu
			 where ugm.user_id=%s and ugm.group_id = ugmu.group_id
			"""%(uid)
		cur.execute(sql)
		rows = cur.fetchall()
	
		tree = []
		if rows:
			cur.execute('select id,code,name,sort from system.menu  where tier=0 order by sort')
			rows = cur.fetchall()
			li 	 = self.tupleToList(rows)
			li 	 = self.getChildren(cur, li, 1, uid)                
			
			# 清洗空的父菜单
			for obj in li:
				if obj['children']:
					lic=obj['children']
					tempc=[]
					for objc in lic:
						if objc['isItem'] or objc['children']:
							tempc.append(objc)  

					if tempc:
						obj['children'] = tempc
						tree.append(obj)

		else:
			cur.execute('select id,code,name,sort from system.menu  where tier=0 order by sort')
			rows = cur.fetchall()
			li = self.tupleToList(rows)
			li = self.getChildren2(cur, li, 1)  


			# 清洗空的父菜单
			for obj in li:
				if obj['children']:
					lic=obj['children']
					tempc=[]
					for objc in lic:
						if objc['isItem'] or objc['children']:
							tempc.append(objc)  

					if tempc:
						obj['children']=tempc
						tree.append(obj)			
		
		self.response(tree)

	def getChildren2(self,cur,li,tier):
		self.menuIdsStr = None
		
		if len(li)==0:
			return li
		temp=[]
		for obj in li:
			# 判断是否有子菜单
			sql="select id,code,name,sort from system.menu  where tier=%s and parent_code='%s' order by sort"%(tier,obj['code'])
			cur.execute(sql)
			rows = cur.fetchall()
			lic=self.tupleToList(rows) 
			obj['children']=lic
			if tier==1 and len(lic)==0:
				pass
			elif tier>1 and len(lic)==0:
			# 获取菜单项
				sql1="select mi.id, mi.path  from "
				sql1+="system.menu_item mi where mi.menu_id=%s"%(obj['id'])
				# if self.menuIdsStr:
				# 	sql1 += " and mi.id in (%s)" % self.menuIdsStr

				cur.execute(sql1)
				items = cur.fetchall()
				if items:
					# 菜单项存在
					obj['id']=items[0][0]
					obj['isItem']=True
					obj['path']=items[0][1]
					temp.append(obj);
				else:
					# 菜单项不存在
					# obj['isHidden']=True
					# li.remove(obj)
					pass
			else:
				obj['children']=self.getChildren2(cur, lic, tier+1)
				temp.append(obj);
		return temp;    
	
	def getChildren(self,cur,li,tier,uid):

		if len(li) == 0:
			return li

		temp = []
		for obj in li:
			# 判断是否有子菜单
			sql="select id,code,name,sort from system.menu  where tier=%s and parent_code='%s' order by sort"%(tier,obj['code'])
			cur.execute(sql)
			rows = cur.fetchall()
			lic=self.tupleToList(rows) 
			obj['children']=lic
			if tier==1 and len(lic)==0:
				pass
			elif tier>1 and len(lic)==0:
				# 获取菜单项
				sql1="select mi.id, mis.menu_item_id, mi.path  from "
				sql1+="system.menu_item mi inner join (select distinct(ugmu.menu_item_id) "
				sql1+="from system.user_group_member ugm,system.user_group_menu ugmu "
				sql1+="where ugm.user_id=%s and ugm.group_id=ugmu.group_id) mis "%(uid)
				sql1+="on mi.id=mis.menu_item_id where mi.menu_id=%s" % (obj['id'])

				cur.execute(sql1)
				items = cur.fetchall()

				if items:
					# 菜单项存在
					obj['id']=items[0][0]
					obj['isItem']=True
					obj['path']=items[0][2]
					temp.append(obj);
				else:
					# 菜单项不存在
					# obj['isHidden']=True
					# li.remove(obj)
					pass
			else:
				obj['children']=self.getChildren(cur, lic, tier+1, uid)
				temp.append(obj);
		return temp;    
	
	def tupleToList(self,rows):
		li=[]
		for row in rows:
			obj={}
			obj['id'] 		= row[0]
			obj['code']		= row[1]
			obj['name']		= row[2]
			obj['sort']		= row[3]
			obj['children'] = []
			obj['isItem'] 	= False
			obj['path'] 	= ''
			li.append(obj)
		return li        

