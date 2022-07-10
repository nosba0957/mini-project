#include<string>

class const_value{
	friend class Token;
	friend class ASTnode;
	public:
		const_value(std::string x){
			exp=x;
		}
		std::string re_value(){
			return exp;
		}
	private:
		std::string exp;
};
class unary_op{
	friend class ASTnode;
	public:
		unary_op(std::string x,std::string y){
			u_oper=x;
			exp=y;
		}
		std::string re_value(){
			return u_oper+exp;
		}
	private:
		std::string u_oper;
		std::string exp;
};
class statement{
	friend class Token;
 	friend class ASTnode;
	public:
		statement(const_value* x){
			express=x;
		}
		statement(unary_op* x){
			u_op=x;
		}
	private:
		const_value* express=NULL;
		unary_op* u_op=NULL;
};
class function{
	friend class Token;
	friend class ASTnode;
	public:
		function(std::string name);
	private:
		std::string function_name;
		statement* cond=NULL;
};
function::function(std::string name){
	function_name=name;
}
class program{
	friend class Token;
	friend class ASTnode;
	public:
		program(function* func){
			this->func=func;
		}
		std::string return_exp();
	private:
		function* func=NULL;
};
class ASTnode{
	friend class Token;
	public:
		ASTnode(program* prog1){
			prog=prog1;
		}
		std::string return_exp();
	private:
		program* prog=NULL;
};
std::string ASTnode::return_exp(){
	const_value* exp_temp=this->prog->func->cond->express;
	unary_op* unary_temp=this->prog->func->cond->u_op;
	if(exp_temp==NULL) return unary_temp->re_value();
	else return exp_temp->re_value();
}
