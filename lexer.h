#include<string>
#include<cstdlib>
#include"token.h"

class lexer{
	public:
		Token* lex(std::string);
		void addToken(Token* ,enum kind);
		Token* addstring(std::string);
		lexer();
	private:
};
lexer::lexer(){
}
Token* lexer::lex(std::string line){
	std::string keyword_temp,int_value;
	int flag=0;
	Token* now=new Token(Head);
	Token* root=now;
	char i;
	enum kind type=Head;
	for(int j=0;j<line.length();j++){
		type=Head;
		i=line[j];
		//std::cout<<i;
		switch(i){
			case '{':
				type=openbrace;
				flag=0;
				break;
			case '}':
				type=closebrace;
				flag=0;
				break;
			case '(':
				type=openparenthesis;
				flag=0;
				break;
			case ')':
				type=closeparenthesis;
				flag=0;
				break;
			case ';':
				type=semicolon;
				flag=0;
				break;
			case '-':
				flag=3;
				type=unary_type;
				keyword_temp.push_back(i);
				break;
			case '~':
				flag=3;
				type=unary_type;
				keyword_temp.push_back(i);
				break;
			case '!':
				flag=3;
				type=unary_type;
				keyword_temp.push_back(i);
				break;
			case 'a'...'z':
				keyword_temp.push_back(i);
				flag=1;
				break;
			case '0'...'9':
				flag=2;
				keyword_temp.push_back(i);
				break;
			case ' ':
				flag=0;
				break;
		}
		if(j==line.length()-1) flag=0;
		if(flag==0 && !keyword_temp.empty() && keyword_temp[0]<='z' && keyword_temp[0]>='a'){
			if(keyword_temp=="int"){
				now=now->setToken(int_keyword);
			}
			else if(keyword_temp=="main"){
				now=now->setToken(func_name,keyword_temp);
			}
			else if(keyword_temp=="return"){
				now=now->setToken(return_keyword);
			}
			else{
				now=now->addstrToken(keyword_temp);
			}
			keyword_temp.clear();
		}
		else if(flag==0 && !keyword_temp.empty()){
			now=now->setToken(integer,keyword_temp);
		}
		if(type!=Head){
			if(type==unary_type){
				now=now->setToken(type,keyword_temp);
				keyword_temp.clear();
				continue;
			}
			now=now->setToken(type);
		}
	}
	root=root->removeHead(root);
	return root;
}
