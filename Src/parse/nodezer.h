
#ifndef DEF_NODEZER_H
#define DEF_NODEZER_H

#include <string>
#include <vector>
#include <exception>

#include "tokenizer.h"
#include "node.h"

using namespace std;
using namespace def::token;
using namespace def::node;


namespace def {
namespace node {

class Nodezer {

	public:

	Nodezer(vector<Word>&);

	void Read(){
		prev = cur;
		cur = next;
		if(i==0){
			cur = words[0];
		}
		try{
			next = words.at(i);
		}catch(const exception& e){
			next = nullword;
		}
	};

	void Clear(){
		i = 0;
		nullword = Word{0,0,Token::State::Null,""};
		prev = cur = next = nullword;
		tn_stk.clear();
		tn_stk.push_back(TypeNode::Expression);
	};


	Node* Scan(); // 扫描构建节点树

	private:

	unsigned int i; // 当前单词位置
	Word prev;  // 上一个单词
	Word cur;   // 当前单词
	Word next;  // 下一个单词

	Word nullword;  // 空单词

	vector<Word>& words; // 单词
	vector<TypeNode> tn_stk; // 当前节点类型栈

}; // --end-- class Nodezer

} // --end-- namespace node
} // --end-- namespace def



#endif
// --end-- DEF_NODEZER_H

