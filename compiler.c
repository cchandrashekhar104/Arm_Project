#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R0 0x9999

struct stack{
int data;
struct stack* next;
};
struct node{
int num;
int isOp;
struct node *next;
};
void insert(int data,int isOp,struct node **head,struct node **tail)
{
struct node *temp=(struct node *) malloc(sizeof(struct node));
temp->num=data;
temp->isOp=isOp;
temp->next=NULL;
if((*tail)==NULL)
{
(*tail)=temp;
(*head)=temp;
}
else
{
(*tail)->next=temp;
(*tail)=(*tail)->next;
}
}
void printList(struct node **head)
{
struct node *temp=*head;
while((temp)!=NULL)
{
if(temp->isOp==0)
printf("%d ",temp->num);
else
printf("%c ",temp->num);
temp=temp->next;
}
}

void push(int data,struct stack **st1)
{
struct stack *temp=(struct stack *) malloc(sizeof(struct stack));
temp->data=data;
temp->next=*st1;
*st1=temp;
}
int pop(struct stack **st1)
{
if(*st1==NULL)
	return -1;
struct stack *temp=*st1;
*st1=(*st1)->next;
int data=temp->data;
free(temp);
return data;
}
char peak(struct stack **st1)
{
return (*st1)->data;
}
int isEmpty(struct stack **st1)
{
	if((*st1)==NULL)
	return 1;
	else
	return 0;
}
int precedenceof(char ch)
{
if(ch=='(')
return 1;
if(ch=='+'||ch=='-')
return 2;
if(ch=='*'||ch=='/')
return 3;
}
void printAssembly(struct node **head)
{
struct stack **stack2=(struct stack **) malloc(sizeof(struct stack *));
struct node *temp=*head;
char c;
int d1,d2,reg=0;
FILE *fp=fopen("a.s","w");
if(fp==NULL)
{
printf("Error opening file\n");
exit(1);
}
fprintf(fp,"\tPRESERVE8\n\tTHUMB\n\tAREA appcode, CODE, READONLY\n\tEXPORT __main\n\tENTRY\n__main FUNCTION\n");
while((temp)!=NULL)
{
if(temp->isOp==0)
push(temp->num,stack2);
else
{
c=temp->num;
switch(c)
{
	case '+':
		if((d1=pop(stack2))<R0)
		fprintf(fp,"\t\tMOV R%d,#%d\n",reg++,d1);
		if((d2=pop(stack2))<R0)
		fprintf(fp,"\t\tMOV R%d,#%d\n",reg++,d2);
		//printf("d1=%d,d2=%d\n",d1,d2);
		if(d1<R0 && d2<R0)
		{
		fprintf(fp,"\t\tADD R%d,R%d\n",reg-2,reg-1);
		reg--;
		push(R0+(reg-1),stack2);
		}
		if(d1<R0 && d2>=R0)
		{
		fprintf(fp,"\t\tADD R%d,R%d\n",d2-R0,reg-1);
		reg--;
		push(d2,stack2);
		}
                if(d1>=R0 && d2<R0)
                {
                fprintf(fp,"\t\tADD R%d,R%d\n",d1-R0,reg-1);
                reg--;
                push(d1,stack2);
                }
		if(d1>=R0 && d2>=R0)
		{
		fprintf(fp,"\t\tADD R%d,R%d\n",d2-R0,d1-R0);
		reg--;
		push(d2,stack2);
		}
		break;
	case '-':
		d1=pop(stack2);
		d2=pop(stack2);
		if(d2<R0)
		fprintf(fp,"\t\tMOV R%d,#%d\n",reg++,d2);
		if(d1<R0)
		fprintf(fp,"\t\tMOV R%d,#%d\n",reg++,d1);
		if(d1<R0 && d2<R0)
		{
		fprintf(fp,"\t\tSUB R%d,R%d\n",reg-2,reg-1);
		reg--;
		push(R0+(reg-1),stack2);
		}
		if(d1<R0 && d2>=R0)
		{
		fprintf(fp,"\t\tSUB R%d,R%d\n",d2-R0,reg-1);
		reg--;
		push(d2,stack2);
		}
                if(d1>=R0 && d2<R0)
                {
                fprintf(fp,"\t\tSUB R%d,R%d\n",reg-1,d1-R0);
		fprintf(fp,"\t\tMOV R%d,R%d\n",d1-R0,reg-1);
                reg--;
                push(d1,stack2);
                }
		if(d1>=R0 && d2>=R0)
		{
		fprintf(fp,"\t\tSUB R%d,R%d\n",d2-R0,d1-R0);
		reg--;
		push(d2,stack2);
		}
		break;
	case '*':
		if((d1=pop(stack2))<R0)
		fprintf(fp,"\t\tMOV R%d,#%d\n",reg++,d1);
		if((d2=pop(stack2))<R0)
		fprintf(fp,"\t\tMOV R%d,#%d\n",reg++,d2);
		if(d1<R0 && d2<R0)
		{
		fprintf(fp,"\t\tMUL R%d,R%d\n",reg-2,reg-1);
		reg--;
		push(R0+(reg-1),stack2);
		}
		if(d1<R0 && d2>=R0)
		{
		fprintf(fp,"\t\tMUL R%d,R%d\n",d2-R0,reg-1);
		reg--;
		push(d2,stack2);
		}
                if(d1>=R0 && d2<R0)
                {
                fprintf(fp,"\t\tMUL R%d,R%d\n",d1-R0,reg-1);
                reg--;
                push(d1,stack2);
                }
		if(d1>=R0 && d2>=R0)
		{
		fprintf(fp,"\t\tMUL R%d,R%d\n",d2-R0,d1-R0);
		reg--;
		push(d2,stack2);
		}
		break;
	case '/':
		d1=pop(stack2);
		d2=pop(stack2);
		if(d2<R0)
		fprintf(fp,"\t\tMOV R%d,#%d\n",reg++,d2);
		if(d1<R0)
		fprintf(fp,"\t\tMOV R%d,#%d\n",reg++,d1);
		if(d1<R0 && d2<R0)
		{
		fprintf(fp,"\t\tSDIV R%d,R%d\n",reg-2,reg-1);
		reg--;
		push(R0+(reg-1),stack2);
		}
		if(d1<R0 && d2>=R0)
		{
		fprintf(fp,"\t\tSDIV R%d,R%d\n",d2-R0,reg-1);
		reg--;
		push(d2,stack2);
		}
                if(d1>=R0 && d2<R0)
                {
                fprintf(fp,"\t\tSDIV R%d,R%d\n",reg-1,d1-R0);
		fprintf(fp,"\t\tMOV R%d,R%d\n",d1-R0,reg-1);
                reg--;
                push(d1,stack2);
                }
		if(d1>=R0 && d2>=R0)
		{
		fprintf(fp,"\t\tSDIV R%d,R%d\n",d2-R0,d1-R0);
		reg--;
		push(d2,stack2);
		}
		break;
}
}
temp=temp->next;
}
fprintf(fp,"stop B stop\n\tENDFUNC\n\tEND\n");
fclose(fp);
}
int main()
{
struct stack **stack1=(struct stack **)malloc(sizeof(struct stack*));
struct node **list_tail=(struct node **)malloc(sizeof(struct node*));
struct node **list_head=(struct node **)malloc(sizeof(struct node*));

printf("Enter expression\n");
char exp[100],c;
int i,num=0,j;
scanf("%s",exp);
//printf("%s\n",exp);
for(i=0;i<strlen(exp);i++)
{
	if((exp[i]>='A'&&exp[i]<='Z')||(exp[i]>='a'&&exp[i]<='z'))
	{
		printf("%c",exp[i]);
		insert(exp[i],1,list_head,list_tail);
	}
	if((exp[i]>='0'&&exp[i]<='9'))
	{
		j=i;
		while((exp[j]>='0'&&exp[j]<='9'))
		{
			num=(num*10)+(exp[j]-48);
			j++;
		}
		i=j-1;
		printf("%d",num);
		insert(num,0,list_head,list_tail);		
		num=0;
		continue;
	}
	if(exp[i]=='(')
		push('(',stack1);
	if(exp[i]==')')
	{
		while((c=pop(stack1))!='(')
			{
			printf("%c",c);
			insert(c,1,list_head,list_tail);
			}
	}
	if((exp[i]=='+')||(exp[i]=='-')||(exp[i]=='*')||(exp[i]=='/'))
	{
		if(!isEmpty(stack1))
		while(precedenceof(peak(stack1))>=precedenceof(exp[i]))
		{
		c=pop(stack1);
		printf("%c",c);
		insert(c,1,list_head,list_tail);
		if(isEmpty(stack1))
		break;
		}
		push(exp[i],stack1);
	}
}
while((c=pop(stack1))!=-1)
{
printf("%c",c);
insert(c,1,list_head,list_tail);
}
printf("\n");
printList(list_head);
printAssembly(list_head);
return 0;
}
