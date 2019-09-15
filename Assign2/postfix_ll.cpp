#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cmath>
#include<cstring>

using namespace std;

struct Node  // definition of a node in linked list
{
    float a;
    Node* next;
};

class Stack  // definition of a stack
{
    Node* head;

    public:

    Stack()  // constructor 
    {
        head = NULL;
    }

    bool empty()  // check if stack is empty
    {
        if (head==NULL)
            return true;
        
        else
            return false;
    }

    float top()  // return top elem
    {
        if (empty())
        {    
            cerr<<"Stack is empty"<<endl;
            return 0;
        }
        else
            return head->a; 
    }

    void pop()  // pop the top elem
    {
        if (empty())
            cerr<<"Stack is empty"<<endl;
        else
            head = head->next;
    }

    void push(float x)  // push to top
    {
        Node* tmp = new Node;
        tmp->a = x;
        tmp->next = head;
        head = tmp;        
        
    }
    
};

bool isInteger(float val) // to check if a float var has int values
{
    int truncated = (int)val;
    return (val == truncated);
}

int isOperator(char* c) // to check if a string is an operator 
{
    if (!strcmp(c,"+"))
        return 1;
    else if (!strcmp(c,"*"))
        return 2;
    else if (!strcmp(c,"-"))
        return 3;
    else if (!strcmp(c,"/"))
        return 4;
    else 
        return 0; 
    
}

void eval_postfix(Stack& s,char* c)  // func to eval postfix by taking in one elem at a time
{
    int i = 0;

    int op = isOperator(c);
    
    if (op)  // check if operation
    {
        float a = s.top(); s.pop();  // pop 1st operand
        
        if (s.empty())  // empty stack before completion of operation
        { 
            cerr<<"Invalid expr. \""<<c<<"\" does not have enough operands"<<endl;
            exit(0);
        }
        float b = s.top(); s.pop();  // pop second operand
        
        float res;
        if (op == 1)
            res = a+b;
        else if (op == 2)
            res = a*b;
        else if (op == 3)
            res = a-b;
        else if (op == 4)
            res = a/b;
    
        s.push(res);  // push evaluated result
    }

    else  // push operands if not operation
    {   
        float a;
        float b; 
        
        char *token = strtok(c, "e");  // split by "e"
        
        a = atof(token);
        token = strtok(NULL, "e");
        if (token != NULL)
        {
            b = atof(token);
            if (isInteger(b))  // check if float following "e" is float 
                a*=pow(10,b);
            else
            {
                cerr<<"Invalid expr. Illegal tag: "<<a<<"e"<<b<<endl;
                exit(0);
            }
        } 
        s.push(a);
    }
}

int main()
{
    Stack s;
    char post_exp [50];
    char tmp[5][10];
    cout<<"Enter the postfix expression: ";
    cin.getline(post_exp,50);
    char * split = strtok (post_exp," ");  // split by space and store all inputs from exp
    
    int i = 0;
    while (split != NULL) 
    {
        strcpy(tmp[i],split);
        split = strtok (NULL, " ");
        i++;
    }
    
    for(int j=0;j<i;j++)
    {
        eval_postfix(s,tmp[j]);  // evaluate exp by passing one input a time
    }

    float val = s.top();  // pop final result
    s.pop();

    if (s.empty())  // stack should be empty at the end after popping the final result
        cout<<val<<endl;
    else
        cerr<<"Invalid expr. incomplete"<<endl;
    
    return 0; 
}