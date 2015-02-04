
#ifndef DEF_NODE_H
#define DEF_NODE_H

#include <string>
#include <vector>

#include "../Util/str.h"

using namespace std;
using namespace def::util;


namespace def {
namespace node {

// 节点类型
enum class TypeNode
{
	Normal,   // 默认状态
	Start,    // 开始状态
	Down,     // 完成状态

	// 枝节点

	Group,    // 组合表达式

	Assign,   // 赋值语句

	Add,   // + 加法计算              // 5
	Sub,   // - 减法计算
	Mul,   // * 乘法计算
	Div,   // / 除法计算

	FuncDefine,   // 函数定义
	FuncCall,     // 函数调用

	// 叶节点

	Variable,   // 变量符号           // 11

	None,    // 值 none
	Bool,    // 值 布尔
	Int,     // 值 整数
	Float,   // 值 浮点数
	String,  // 值 字符串

	// 终止
	Null
	
}; // --end-- enum class TypeNode


// 节点
struct Node{
	unsigned int line;   // 代码行
	unsigned int posi;   // 所属位置
	TypeNode type;           // 节点类型
	// 构造方法
	Node(TypeNode t, Word &w)
		: type(t)
	{
		line = w.line;
		posi = w.posi;
	}
	virtual ~Node()=0;
	virtual void AddChild(Node*){};
	virtual Node* Child(size_t i){};
	virtual Node* Left(Node*n=NULL){};
	virtual Node* Right(Node*n=NULL){};
	virtual bool GetBool(){};
	virtual long GetInt(){};
	virtual double GetFloat(){};
	virtual string GetName(){};
	virtual string GetString(){};
};
Node::~Node(){} // 纯虚析构函数的定义


// 多叉节点
struct NodeTree : Node{
	vector<Node*> childs; // 子节点指针列表
	NodeTree(TypeNode t, Word &w)
		: Node(t, w){}
	virtual ~NodeTree(){
		size_t sz = childs.size();
		for(int i=0; i<sz; i++)
		{
        	//cout<<"delete child "<<i<<endl;
			delete childs[i];
		}
		childs.clear();
	};
	inline void AddChild(Node* n){
        //cout<<"add child "<<(int)n->type<<endl;
		childs.push_back(n);
	}
	inline Node* Child(size_t i){
		return i<childs.size() ? childs[i] : NULL;
	}
};


// 组合表达式
struct NodeGroup : NodeTree{
	NodeGroup(Word &w)
		: NodeTree(TypeNode::Group, w){}
};


// 二叉节点
struct NodeTwinTree : Node{
	Node* left;   // 左子节点
	Node* right;  // 右子节点
	NodeTwinTree(TypeNode t, Word &w, Node* lf=NULL, Node* rt=NULL)
		: Node(t, w), left(lf), right(rt){}
	virtual ~NodeTwinTree(){
        //cout<<"delete left "<<(int)left->type<<endl;
		delete left;
        //cout<<"delete right "<<(int)right->type<<endl;
		delete right;
	};
	inline Node* Left(Node*n=NULL){
        //cout<<"left child "<<(int)n->type<<endl;
        if(n!=NULL){
			left = n; //设置
        }
        return left;
	}
	inline Node* Right(Node*n=NULL){
        //cout<<"right child "<<(int)n->type<<endl;
        if(n!=NULL){
			right = n; //设置
        }
        return right;
	}
};


// = 赋值节点
struct NodeAssign: NodeTwinTree{
	NodeAssign(Word &w)
		: NodeTwinTree(TypeNode::Assign, w){}
};


// + 加操作节点
struct NodeAdd: NodeTwinTree{
	NodeAdd(Word &w)
	: NodeTwinTree(TypeNode::Add, w){}
};


// - 减操作节点
struct NodeSub: NodeTwinTree{
	NodeSub(Word &w)
	: NodeTwinTree(TypeNode::Sub, w){}
};

// * 乘操作节点
struct NodeMul: NodeTwinTree{
	NodeMul(Word &w)
	: NodeTwinTree(TypeNode::Mul, w){}
};

// / 除操作节点
struct NodeDiv: NodeTwinTree{
	NodeDiv(Word &w)
	: NodeTwinTree(TypeNode::Div, w){}
};


// variable 节点
struct NodeVariable : Node{
	string name;
	NodeVariable(Word &w)
	: Node(TypeNode::Variable, w){
		name = w.value;
	}
	inline string GetName(){
		return name;
	};
};


// none 节点
struct NodeNone : Node{
	NodeNone(Word &w)
	: Node(TypeNode::None, w){}
};


// bool 节点
struct NodeBool : Node{
	bool value;
	NodeBool(Word &w)
	: Node(TypeNode::Bool, w){
		value = w.value=="true" ? true : false;
	}
	inline bool GetBool(){
		return value;
	};
};


// int 节点
struct NodeInt : Node{
	long value;
	NodeInt(Word &w)
	: Node(TypeNode::Int, w){
		value = Str::s2l(w.value);
	}
	inline long GetLong(){
		return value;
	};
};








} // --end-- namespace node
} // --end-- namespace def



#endif
// --end-- DEF_NODE_H
