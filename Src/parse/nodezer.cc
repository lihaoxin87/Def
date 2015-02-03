
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>

#include "nodezer.h"

#define S Token::State
#define T TypeNode

using namespace std;

using namespace def::token;
using namespace def::node;

// Log::log
#define ERR(str) cerr<<str<<endl;exit(1);

/**
 * 构造
 */
Nodezer::Nodezer(vector<Word>& w):
    words(w)
{
    Clear();
}



/**
 * 判断第一个参数（类型）是否为后面的类型任意之一
 */

bool Nodezer::IsType(T c,
    T t0=T::Null, T t1=T::Null, T t2=T::Null, T t3=T::Null, T t4=T::Null,
    T t5=T::Null, T t6=T::Null, T t7=T::Null, T t8=T::Null, T t9=T::Null)
{
    if(c==T::Null){ // 不可对比Null
        return false;
    }
    if(c==t0||c==t1||c==t2||c==t3||c==t4||
       c==t5||c==t6||c==t7||c==t8||c==t9){
        return true;
    }else{
        return false;
    }
}

/**
 * 判断当前的节点类型
 */
void Nodezer::CurTypeNode()
{
    ctn = GetTypeNode(cur);
}


/**
 * 获得当前的节点类型
 */
TypeNode Nodezer::GetTypeNode(Word &cur)
{
    S ct = cur.type;
    string cv = cur.value;
    //S nt = next.type;
    //string nv = next.value;

    //cout<<(int)ct<<"->"<<cv<<endl;

    if(ct==S::Symbol){

        return T::Variable; // 变量名

    }else if(ct==S::Sign){

        if(cv=="="){
            return T::Assign; //赋值 =
        }else if(cv=="+"){
            return T::Add; //加 +
        }else if(cv=="*"){
            return T::Mul; // 乘 *
        }else if(cv=="/"){
            return T::Mul; // 乘 *
        }

    }else if(ct==S::Null){

        return T::Null; // 终止符
    }


    // 无匹配
    return T::Normal;

}


/**
 * 从当前单词新建节点
 */
Node* Nodezer::CreatNode(int mv=1, Node*l=NULL, Node*r=NULL)
{
    //Read();
    if(mv!=0) Move(mv); //移动指针

    Node *p = NULL;

    switch(ctn){
    case T::Variable: // 变量
        return new NodeVariable(cur);
    case T::Add: // 加 +
        p = new NodeAdd(cur); break;
    case T::Sub: // 减 -
        p = new NodeSub(cur); break;
    case T::Mul: // 乘 *
        p = new NodeMul(cur); break;
    case T::Div: // 除 /
        p = new NodeDiv(cur); break;
    }

    if(p&&l) p->Left(l);
    if(p&&r) p->Right(r);

    return p;
}


/**
 * 扫描单词 构建单条表达式
 * @param 
 */
Node* Nodezer::Express(Node *p=NULL, T tt=T::Start)
{
    //T t = T::Start; // 记录状态
    //string cv = cur.value; // 当前值

    while(tt!=T::Down){

        Read();
        CurTypeNode(); // 当前类型
        T t = ctn;
        cout<<(int)t<<"->"<<cur.value<<endl;

        //// Start
        if(tt==T::Start){ //开始状态

            cout << "-Start-" << endl;
            if(t==T::Variable){
                p = CreatNode(1);
                tt = t;
            }else if(t==T::Null){ //结束
                tt = T::Down;
            }

        //// Variable
        }else if(tt==T::Variable){

            cout << "-Variable-" << endl;
            if(IsType(t,T::Variable,T::Null) ){ //语句结束 完成返回
                tt = T::Down;
            // 如果是加减乘除操作
            }else if( IsType(t,T::Add,T::Sub,T::Mul,T::Div) ){
                p = CreatNode(1, p);
                tt = t;
            }else{ 
                tt = T::Down;
            }

        //// Add Sub
        }else if( IsType(tt,T::Add,T::Sub) ){ // 加法 减法 + -

            cout << "-Add,Sub-" << endl;
            if( IsType(t,T::Variable) ){
                p->Right(CreatNode(1));
            // 同级左结合算符 + -
            }else if( IsType(t,T::Add,T::Sub) ){
                p = CreatNode(1, p);
                tt = t;
            // 优先算符 * /
            }else if( IsType(t,T::Mul,T::Div) ){
                Node *pn = CreatNode(1);
                Node *r = p->Right();
                pn->Left(r);
                pn = Express(pn, t);
                p->Right(pn);
            }else{ 
                tt = T::Down;
            }

        //// Mul Div
        }else if( IsType(tt,T::Mul,T::Div) ){ // 乘法 除法 * /

            cout << "-Mul,Div-" << endl;

            if( IsType(t,T::Variable) ){
                p->Right(CreatNode(1));
            // 同级左结合算符
            }else if( IsType(t,T::Mul,T::Div) ){
                p = CreatNode(1, p);
                tt = t;
            }else{ //遇到加减法等优先级更低的返回节点
                tt = T::Down;
            }

        //// Null
        }else if(tt==T::Null){ // 终止

            cout << "-Null-" << endl;
            tt = T::Down;

        }else{

        }

    }

    return p;
}



#undef S // Token::State
#undef T // TypeNode


/****************** 语法分析器测试 *******************/


int main()
{
    cout << "\n";


    // 词法分析结果
    vector<Word> words;

    // 初始化词法分析器
    Tokenizer T(true, "./test.d", words);

    // 执行词法分析
    T.Scan();

    // 初始化语法分析器
    Nodezer N(words);

    // 解析得到语法树（表达式）
    Node *node = N.Express();

    cout << "\n\n";

    //delete node;

    cout << 
    node->Left()->Right()->Right()->GetName()
    << endl;

    /*
    */
    

    cout << "\n\n";


    // 打印词法分析结果
    for(int i=0; i<words.size(); i++){
    	Word wd = words[i];
		cout << wd.line << ","<< wd.posi << "  " << (int)wd.type << "  " << wd.value << endl;
    }
    
    



    cout << "\n\n";


    /*

    Tokenizer TK("./Parser/test.d", true);
    vector <Word> words = TK.Scan();

    for(int i=0; i<words.size(); i++){
    	Word wd = words[i];
		cout << wd.line << ","<< wd.pos << "  " << (int)wd.type << "  " << wd.value << endl;
    }

    */

}




/**************** 语法分析器测试结束 *****************/