#include<iostream>
#include<string>
#include<fstream>
#include<cstring>
#include"lexer.h" 
using namespace std;

string generate(ASTnode* tree){
	string func_name="main";
	string content="	.globl "+func_name+"\n"+func_name+":\n"+"	movl	$";
	string int_temp=tree->return_exp();
	string operator_temp;
	while(int_temp[0]=='-'||int_temp[0]=='~'||int_temp[0]=='!'){
		operator_temp.push_back(int_temp[0]);
		int_temp.erase(int_temp.begin());
	}
	int comp=0,neg=0,notS=0,index=1;
	int value=stoi(int_temp);
	if(!operator_temp.empty()){
		for(auto i:operator_temp){
			if(i=='-'){
				if(neg>0) neg=0;
				else if(neg==0) neg=index;
			}
			if(i=='~'){
				if(comp>0) comp=0;
				else if(comp==0) comp=index;
			}
			if(i=='!'){
				if(notS>0) notS=0;
				else if(notS==0) notS=index;
			}
			index++;
		}
	}
	content=content+int_temp+", %eax\n";
	while(notS>0 || comp>0 || neg>0){
		if(notS>0 && notS>comp && notS>neg){
			content=content+"	cmpl	$0, %eax\n"+"	movl	$0,%eax\n"+"	sete	%al\n";
			notS=0;
		}
		if(comp>0 && comp>notS && comp>neg){
			content=content+"	neg	%eax\n"+"	dec	%eax\n";
			comp=0;
		}
		if(neg>0 && neg>comp && neg>notS){
			content+="	neg	%eax\n";
			neg=0;
		}
	}
	content=content+"	ret\n";
	return content;
}
int main(int argc,char **argv){
	string fileIn=argv[1];
	ifstream file;
	file.open(fileIn);

	ofstream fileOut;
	string OutFileName(argv[1]);
	OutFileName[OutFileName.length()-1]='s';
	fileOut.open(OutFileName,std::ios_base::out);
	string content="";

	string lines;
	Token* token=new Token();
	Token* head=new Token(Head);
	lexer* lexr=new lexer();
	while(getline(file,lines)){
		token=lexr->lex(lines);
		//token->check();
		//std::cout<<"\n";
		head->connect(head,token);
	}
	//head->check();
	ASTnode* tree=token->parse_program(head);
	if(tree==NULL){
		cout<<"somthing wrong";
		file.close();
		fileOut.close();
		return 0;
	}
	content=generate(tree);
	fileOut<<content;
	file.close();
	fileOut.close();
	return 0;
}
