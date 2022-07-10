#include<iostream>
#include"ast.h"

enum kind{NOT,Head,openbrace,closebrace,openparenthesis,closeparenthesis,semicolon,int_keyword,func_name,return_keyword,identifier,integer,unary_type};
class Token{
	friend class ASTnode;
	public:
		Token* lex(std::string line);
		ASTnode* parse_program(Token* list);
		function* parse_function(Token* list);
		statement* parse_statement(Token* list);
		unary_op* parse_unary(Token* list);
		const_value* parse_exp(Token* list);
		Token* setToken(enum kind,std::string);
		Token* addstrToken(std::string);
		Token* addInteger(std::string);
		void connect(Token*,Token*);
		Token* removeHead(Token*);
		void check();
		Token* CreateAndSwap(std::string, enum kind);
		Token(enum kind);
		~Token();
	private:
		enum kind state=NOT;
		Token *next;
		std::string value="";
};
Token::Token(enum kind stt=NOT){
	state=stt;
	next=NULL;
}
void Token::check(){
	std::cout<<this->state<<" ";
	if(this->next!=nullptr) this->next->check();
}
Token* Token::setToken(enum kind stt,std::string name=""){
	if(stt==this->state && this->value!=""){
		this->value+=name;
		return this;
	}
	this->next=new Token(stt);
	if(name!="") this->next->value=name;
	return this->next;
}
Token* Token::removeHead(Token* list){
	list=list->next;
	return list;
}
void Token::connect(Token* head,Token* now){
	Token* temp=head;
	while(temp->next!=nullptr){
		temp=temp->next;
	}
	temp->next=now;
}
Token* Token::addstrToken(std::string keyword_temp){
	this->next=new Token(NOT);
	this->next->value=keyword_temp;
	return this->next;
}
Token* Token::addInteger(std::string x){
	this->next=new Token(integer);
	this->next->value=x;
	return this->next;
}
const_value* Token::parse_exp(Token* list){
	std::string op_temp="";
	const_value* return_value=new const_value(list->value);
	return return_value;
}
unary_op* Token::parse_unary(Token* list){
	std::string u_op;
	std::string int_temp;
	if(list->state!=unary_type){
		return NULL;
	}
	u_op=list->value;
	list=list->next;
	if(list->state!=integer){
		std::cout<<"missing integer\n";
		return NULL;
	}
	int_temp=list->value;
	unary_op* unary_temp=new unary_op(u_op,int_temp);
	return unary_temp;
}
statement* Token::parse_statement(Token* list){
	if(list->state!=return_keyword){
		std::cout<<"expected \"return\"\n";
		return NULL;
	}
	list=list->next;
	unary_op* u_operator=NULL;
	const_value* exp_temp=NULL;
	if(list->state==unary_type){
		u_operator=parse_unary(list);
		list=list->next;
	}
	else{
		if(list->state!=integer){
			std::cout<<"need a integer\n";
			return NULL;
		}
		exp_temp=parse_exp(list);
	}
	if(exp_temp==NULL && u_operator==NULL){
		std::cout<<"exp is NULL\n";
		return NULL;
	}
	statement* condition;
	if(u_operator!=NULL){
		condition=new statement(u_operator);
	}
	else if(exp_temp!=NULL){
		condition=new statement(exp_temp);
	}
	list=list->next;
	if(list->state!=semicolon){
		std::cout<<"expected \";\"\n";
		return NULL;
	}
	return condition;
}
function* Token::parse_function(Token* list){
	if(list->state!=int_keyword){
		std::cout<<"expected \"int\"\n";
		return NULL;
	}
	list=list->next;
	if(list->state!=func_name){
		std::cout<<"function name shouldn't be empty\n";
		return NULL;
	}
	function* fun_1=new function(list->value);
	list=list->next;
	if(list->state!=openparenthesis){
		std::cout<<"expected \"(\"\n";
		return NULL;
	}
	list=list->next;
	if(list->state!=closeparenthesis){
		std::cout<<"expected \")\"\n";
		return NULL;
	}
	list=list->next;
	if(list->state!=openbrace){
		std::cout<<"expected \"{\"\n";
		return NULL;
	}
	list=list->next;
	statement* condition=parse_statement(list);
	//if(condition->express==NULL) list=list->next;
	if(condition==NULL){
		std::cout<<"statement is NULL\n";
		return NULL;
	}
	if(condition->express==NULL) list=list->next;
	list=list->next;
	list=list->next;
	if(list->next==NULL || list->next->state!=closebrace){
		std::cout<<"expected \"}\"\n";
		std::cout<<list->state<<"\n";
		return NULL;
	}
	fun_1->cond=condition;
	return fun_1;
}
ASTnode* Token::parse_program(Token* list){
	if(list->state==Head) list=list->next;
	if(list->state!=int_keyword){
		std::cout<<"first state is not INT\n";
		return NULL;
	}
	function* func=parse_function(list);
	if(func==NULL){
		std::cout<<"function is NULL\n";
		return NULL;
	}
	program* prog=new program(func);
	if(prog==NULL){
		std::cout<<"program is NULL\n";
		return NULL;
	}
	ASTnode* head=new ASTnode(prog);
	return head;
}
