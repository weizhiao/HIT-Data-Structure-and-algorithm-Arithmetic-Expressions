#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int count;//计算括号个数
int Error;//记录输入是否有误
int x;

//定义共用体
union data
{
    char ch;
    float num;
};

//定义栈
struct stack
{
    union data Data;//使用共用体来实现栈的复用
    struct stack* next;//这是个链栈
};

//重命名
typedef struct stack* STACK;
typedef union data DATA;

STACK MakeNull();               //创建一个空栈
void Push(STACK S, DATA data);   //压栈
void Pop(STACK S);              //弹栈
DATA Top(STACK S);              //返回栈顶元素
int Input(char* ch);            //用来读取字符
void Change(char str[], char s[]);//将中缀表达式转化成后缀表达式
int IsPush(char a, char b);      //判断是否弹栈
int IsEmpty(STACK S);           //判断栈是否为空
float Calculate(char s[]);      //通过后缀表达式来计算
int IsError(int pre, int p);     //判断输入是否有误

int main()
{
    char str[50];
    while (1)
    {
    BACK:
        printf("请输入算术表达式(输入q退出)：\n");
        int i = -1;
        count = 0;
        Error = 0;
        int pre = 5, p;
        do
        {
            i++;
            p = Input(&str[i]);//当前位置的返回值
            if (str[0] == 'q') {
                printf("程序退出，欢迎下次使用！\n");
                system("pause");
                return 0;
            }
            if (IsError(pre, p))
            {
            ERROR:
                printf("输入有误，请重新输入！\n");
                fflush(stdin);//清除输入区缓存
                goto BACK;//如果输入有误返回到BACK处
            }
            pre = p;//上一位置的返回值
        } while (str[i] != '\0');//输入算数表达式
        if (count != 0)
        {
            goto ERROR;
        }
        char s[50];
        Change(str, s);
        printf("后缀表达式为:%s\n", s);
        float result = Calculate(s);
        if (Error)
            goto BACK;
        printf("表达式的值为：%.2f\n", result);
    }
    return 0;
}

STACK MakeNull()
{
    STACK S = malloc(sizeof(struct stack));//申请一块内存
    S->next = NULL;
    return S;//返回指向该内存的指针
}

void Push(STACK S, DATA data)
{
    STACK temp = malloc(sizeof(struct stack));
    temp->Data = data;//进行赋值操作
    temp->next = S->next;
    S->next = temp;//插入到栈顶
}

void Pop(STACK S)
{
    STACK temp = S->next;
    S->next = temp->next;
    free(temp);//删除栈顶元素
}

DATA Top(STACK S)
{
    return S->next->Data;
}

int Input(char* ch)
{
    int flag = 1;
    scanf("%c", ch);
    switch (*ch)//判断输入的类型，并根据输入类型返回不同的值
    {
    case '+':
    case '*':
    case '/':
    case '%':
    case '\n':
        if (*ch == '\n')
            *ch = '\0';
        flag = 2;
        break;
    case '.':
        flag = 3;
        break;
    case '-':
        flag = 4;
        break;
    case '(':
        count++;
        flag = 5;
        break;
    case ')':
        count--;
        flag = 6;
        break;
    default:
        if (*ch < '0' || *ch>'9')
        {
            flag = 0;
        }
    }
    return flag;
};

int IsError(int pre, int p)
{
    //根据中缀算术表达式的形式来判断输入正误
    if (pre == 1 || ((pre == 2 || pre == 3 || pre == 4) && p == 1) || (pre == 4 && p == 5) || (pre == 2 && p == 5) || (pre == 5 && (p == 1 || p == 4 || p == 5)) || (pre == 6 && (p == 2 || p == 6)))
        return 0;
    else
        return 1;
}

void Change(char str[], char s[])
{
    STACK S = MakeNull();//建立一个字符栈
    int j = 0;
    int i = 0;
    if (str[0] == '-' && str[1] == '(') {
        s[j++] = '0';
    }
    else if (str[0] == '-' && str[1] != '(') {
        s[j++] = '-';
        i++;
    }
    for (; str[i] != '\0'; i++)//扫描字符串str
    {

        //如果是数字则直接存入字符串s
        if ((str[i] <= '9' && str[i] >= '0') || str[i] == '.' || (!(str[i - 1] <= '9' && str[i - 1] >= '0') && str[i] == '-') && i != 0)
        {
            s[j++] = str[i];
        }
        else
        {
            DATA ch;
            ch.ch = str[i];
            if (str[i] != '(')
            {
                if (str[i] == ')')//如果为')'弹栈，直到遇到'('为止
                {
                    while (Top(S).ch != '(')
                    {
                        s[j++] = ' ';//以空格作为不同元素的分隔符
                        s[j++] = Top(S).ch;
                        Pop(S);
                    }
                }
                else
                {
                    while (!IsEmpty(S) && IsPush(Top(S).ch, ch.ch))//如果要弹栈，则弹到符合要求时停止
                    {
                        s[j++] = ' ';
                        s[j++] = Top(S).ch;
                        Pop(S);
                    }
                    s[j++] = ' ';
                }
            }
            Push(S, ch);//压栈
        }
    }
    while (!IsEmpty(S))//扫描结束后判断栈是否为空，若不为空则弹出除'(',')'外的所有元素
    {
        if (Top(S).ch != '(' && Top(S).ch != ')')
        {
            s[j++] = ' ';
            s[j++] = Top(S).ch;
        }
        Pop(S);
    }
    s[j++] = ' ';
    s[j] = '\0';//字符串结束符
}

int IsPush(char a, char b)//根据四则运算的优先级来判断是否弹栈
{
    if (((a == '+' || a == '-') && (b == '*' || b == '/' || b == '%')) || a == '(' || a == ')')
    {
        return 0;
    }
    return 1;
}

int IsEmpty(STACK S)//判断栈是否为空
{
    if (S->next == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

float Calculate(char s[])
{
    int i = 0;
    STACK S = MakeNull();//建立一个数字栈
    while (s[i] != '\0')
    {
        //将数字字符转为浮点类型
        if ((s[i] >= '0' && s[i] <= '9') || (s[i] == '-' && s[i + 1] >= '0' && s[i + 1] <= '9'))
        {
            int point = -1;
            if (s[i] == '-')
            {
                i++;//跳过符号位
            }
            int j = i;
            DATA sum;
            sum.num = 0;
            for (; s[j] != ' '; j++)
            {
                if (s[j] == '.')
                {
                    point = j;//找到小数点的位置
                }
            }
            if (point == -1)
                point = j;
            for (int k = i; k < j; k++)//从数字位开始
            {
                if (k < point)
                {
                    sum.num += (s[k] - '0') * pow(10, point - k - 1);
                }
                else if (k > point)
                {
                    sum.num += (s[k] - '0') * pow(10, point - k);
                }
            }
            if (s[i - 1] == '-')
            {
                sum.num = -sum.num;
            }
            i = j + 1;//指向下一个元素
            Push(S, sum);//将数字压栈
        }
        else//如果碰到运算符，则弹栈两个数字进行运算
        {
            DATA a;
            a.num = 0;
            float b = Top(S).num;
            Pop(S);
            float c = Top(S).num;
            Pop(S);
            switch (s[i])
            {
            case '+':
                a.num = b + c;
                break;
            case '-':
                a.num = c - b;
                break;
            case '*':
                a.num = c * b;
                break;
            case '/':
                if (b < 1e-6)
                {
                    printf("除数不能为0，请重新输入！\n");
                    Error = 1;
                }
                else
                {
                    a.num = c / b;
                }
                break;
            default:
                if ((c - (int)c) < 1e-4 && (b - (int)b) < 1e-4)
                {
                    a.num = (int)c % (int)b;//将float强转为int
                }
                else
                {
                    printf("不能对小数取余,请重新输入！\n");
                    Error = 1;
                }
                break;
            }
            Push(S, a);
            i = i + 2;//指向下一个元素
        }
    }
    return Top(S).num;//返回栈顶元素，即表达式的值
}
