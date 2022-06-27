#include<stdio.h>
#include<stdlib.h>
#include<math.h>

int count;//�������Ÿ���
int Error;//��¼�����Ƿ�����
int x;

//���干����
union data
{
    char ch;
    float num;
};

//����ջ
struct stack
{
    union data Data;//ʹ�ù�������ʵ��ջ�ĸ���
    struct stack* next;//���Ǹ���ջ
};

//������
typedef struct stack* STACK;
typedef union data DATA;

STACK MakeNull();               //����һ����ջ
void Push(STACK S, DATA data);   //ѹջ
void Pop(STACK S);              //��ջ
DATA Top(STACK S);              //����ջ��Ԫ��
int Input(char* ch);            //������ȡ�ַ�
void Change(char str[], char s[]);//����׺���ʽת���ɺ�׺���ʽ
int IsPush(char a, char b);      //�ж��Ƿ�ջ
int IsEmpty(STACK S);           //�ж�ջ�Ƿ�Ϊ��
float Calculate(char s[]);      //ͨ����׺���ʽ������
int IsError(int pre, int p);     //�ж������Ƿ�����

int main()
{
    char str[50];
    while (1)
    {
    BACK:
        printf("�������������ʽ(����q�˳�)��\n");
        int i = -1;
        count = 0;
        Error = 0;
        int pre = 5, p;
        do
        {
            i++;
            p = Input(&str[i]);//��ǰλ�õķ���ֵ
            if (str[0] == 'q') {
                printf("�����˳�����ӭ�´�ʹ�ã�\n");
                system("pause");
                return 0;
            }
            if (IsError(pre, p))
            {
            ERROR:
                printf("�����������������룡\n");
                fflush(stdin);//�������������
                goto BACK;//����������󷵻ص�BACK��
            }
            pre = p;//��һλ�õķ���ֵ
        } while (str[i] != '\0');//�����������ʽ
        if (count != 0)
        {
            goto ERROR;
        }
        char s[50];
        Change(str, s);
        printf("��׺���ʽΪ:%s\n", s);
        float result = Calculate(s);
        if (Error)
            goto BACK;
        printf("���ʽ��ֵΪ��%.2f\n", result);
    }
    return 0;
}

STACK MakeNull()
{
    STACK S = malloc(sizeof(struct stack));//����һ���ڴ�
    S->next = NULL;
    return S;//����ָ����ڴ��ָ��
}

void Push(STACK S, DATA data)
{
    STACK temp = malloc(sizeof(struct stack));
    temp->Data = data;//���и�ֵ����
    temp->next = S->next;
    S->next = temp;//���뵽ջ��
}

void Pop(STACK S)
{
    STACK temp = S->next;
    S->next = temp->next;
    free(temp);//ɾ��ջ��Ԫ��
}

DATA Top(STACK S)
{
    return S->next->Data;
}

int Input(char* ch)
{
    int flag = 1;
    scanf("%c", ch);
    switch (*ch)//�ж���������ͣ��������������ͷ��ز�ͬ��ֵ
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
    //������׺�������ʽ����ʽ���ж���������
    if (pre == 1 || ((pre == 2 || pre == 3 || pre == 4) && p == 1) || (pre == 4 && p == 5) || (pre == 2 && p == 5) || (pre == 5 && (p == 1 || p == 4 || p == 5)) || (pre == 6 && (p == 2 || p == 6)))
        return 0;
    else
        return 1;
}

void Change(char str[], char s[])
{
    STACK S = MakeNull();//����һ���ַ�ջ
    int j = 0;
    int i = 0;
    if (str[0] == '-' && str[1] == '(') {
        s[j++] = '0';
    }
    else if (str[0] == '-' && str[1] != '(') {
        s[j++] = '-';
        i++;
    }
    for (; str[i] != '\0'; i++)//ɨ���ַ���str
    {

        //�����������ֱ�Ӵ����ַ���s
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
                if (str[i] == ')')//���Ϊ')'��ջ��ֱ������'('Ϊֹ
                {
                    while (Top(S).ch != '(')
                    {
                        s[j++] = ' ';//�Կո���Ϊ��ͬԪ�صķָ���
                        s[j++] = Top(S).ch;
                        Pop(S);
                    }
                }
                else
                {
                    while (!IsEmpty(S) && IsPush(Top(S).ch, ch.ch))//���Ҫ��ջ���򵯵�����Ҫ��ʱֹͣ
                    {
                        s[j++] = ' ';
                        s[j++] = Top(S).ch;
                        Pop(S);
                    }
                    s[j++] = ' ';
                }
            }
            Push(S, ch);//ѹջ
        }
    }
    while (!IsEmpty(S))//ɨ��������ж�ջ�Ƿ�Ϊ�գ�����Ϊ���򵯳���'(',')'�������Ԫ��
    {
        if (Top(S).ch != '(' && Top(S).ch != ')')
        {
            s[j++] = ' ';
            s[j++] = Top(S).ch;
        }
        Pop(S);
    }
    s[j++] = ' ';
    s[j] = '\0';//�ַ���������
}

int IsPush(char a, char b)//����������������ȼ����ж��Ƿ�ջ
{
    if (((a == '+' || a == '-') && (b == '*' || b == '/' || b == '%')) || a == '(' || a == ')')
    {
        return 0;
    }
    return 1;
}

int IsEmpty(STACK S)//�ж�ջ�Ƿ�Ϊ��
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
    STACK S = MakeNull();//����һ������ջ
    while (s[i] != '\0')
    {
        //�������ַ�תΪ��������
        if ((s[i] >= '0' && s[i] <= '9') || (s[i] == '-' && s[i + 1] >= '0' && s[i + 1] <= '9'))
        {
            int point = -1;
            if (s[i] == '-')
            {
                i++;//��������λ
            }
            int j = i;
            DATA sum;
            sum.num = 0;
            for (; s[j] != ' '; j++)
            {
                if (s[j] == '.')
                {
                    point = j;//�ҵ�С�����λ��
                }
            }
            if (point == -1)
                point = j;
            for (int k = i; k < j; k++)//������λ��ʼ
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
            i = j + 1;//ָ����һ��Ԫ��
            Push(S, sum);//������ѹջ
        }
        else//����������������ջ�������ֽ�������
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
                    printf("��������Ϊ0�����������룡\n");
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
                    a.num = (int)c % (int)b;//��floatǿתΪint
                }
                else
                {
                    printf("���ܶ�С��ȡ��,���������룡\n");
                    Error = 1;
                }
                break;
            }
            Push(S, a);
            i = i + 2;//ָ����һ��Ԫ��
        }
    }
    return Top(S).num;//����ջ��Ԫ�أ������ʽ��ֵ
}
