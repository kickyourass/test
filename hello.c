#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>


#define POSTFIX_FILE_NAME "postfix.txt"
#define INFIX_FILE_NAME "input.txt"
#define STATE_OPERAND 0
#define STATE_OPERATOR 1

int op_postfix(void){
	int re_fd = open(POSTFIX_FILE_NAME, O_RDONLY);
	unsigned char buf;
	unsigned char state=STATE_OPERATOR;
	int sum=0,temp;
	int stack[10];
	int stack_idx=0;
	while(1){
		read(re_fd,&buf,1);
		if(buf==0x0A || buf==0x00)
			break;
		if( buf>=0x30 && buf <=0x39){
			sum=10*sum+(buf-0x30);
			state=  STATE_OPERAND;
		}
		else{	  
			
			if(buf!=' '){
				sum=stack[--stack_idx];
				temp=stack[--stack_idx];
				switch(buf){

				case '+' :
					stack[stack_idx++]=temp+sum;
					break;
				case '-' :
					stack[stack_idx++]=temp-sum;
					break;
				case '*' :
					stack[stack_idx++]=temp*sum;
					break;
				case '/' :
					stack[stack_idx++]=temp/sum;
					break;
				}
			}
			else if(state==STATE_OPERAND)
				stack[stack_idx++]=sum;
			state=STATE_OPERATOR;
			sum=0;
		}
	}
	close(re_fd);
	return stack[0];
}
int get_ISP(char x){
	switch(x){
		case '+':
			return 5;
		case '-':
			return 5;
		case '*':
			return 10;
		case '/':
			return 10;
		case '(':
			return 0;
		case ')':
			return 5;
	}
}
int get_ICP(char x){
	switch(x){
		case '+':
			return 5;
		case '-':
			return 5;
		case '*':
			return 10;
		case '/':
			return 10;
		case '(':
			return 20;
		case ')':
			return 0;
	}
}
void infix2postfix(){
	int wb_fd = open(POSTFIX_FILE_NAME, O_RDWR|O_CREAT|O_TRUNC);
	int re_fd = open(INFIX_FILE_NAME, O_RDONLY);
	char stack[10];
	char buf;
	int stack_idx=0;
	int state=STATE_OPERATOR;
	while(1){
		read(re_fd,&buf,1);
		if(buf==0x0A)
			break;
		if( buf>=0x30 && buf <=0x39){
			if(state==STATE_OPERATOR)
				write(wb_fd," ",1);
			write(wb_fd,&buf,1);
			state=STATE_OPERAND;
		}
		else{
			while(get_ISP( stack[stack_idx-1] ) >= get_ICP( buf ) && stack_idx >= 1 ){
				if( stack[ --stack_idx]=='(' )
					break;
				write(wb_fd," ",1);
				write( wb_fd, &stack[stack_idx] , 1);
			}
			if( buf!=')' )
				stack[stack_idx++]=buf;
			state=STATE_OPERATOR;
		}	
	}
	while(stack_idx>=1){
		write(wb_fd," ",1);
		write( wb_fd, &stack[--stack_idx] , 1);
	}
	close(re_fd);
	close(wb_fd);
}
int main(void)
{
	int ans;
	infix2postfix();
	ans=op_postfix();
	printf("ANSWER:%d\n",ans);
	return 0;
}

