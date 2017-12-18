#include "stdafx.h"
#include "filetree.h"
#include <windows.h> 
SYSTEMTIME sys;
using namespace std;
typedef struct time
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
};
typedef struct file
{
	int flag;
	int cf;
	char name[20];
	char filetype[10];
	time time;
	struct file *parent, *firstchild, *brother;
	int n;              //����
	int level;           //���
	int size;             //��С
};

filetree::filetree()
{
	root = new file;
	root->level = 0;
	root->cf = 0;
	root->n = 0;
	root->time.year = sys.wYear;
	root->time.month = sys.wMonth;
	root->time.day = sys.wDay;
	root->time.hour = sys.wHour;
	root->time.minute = sys.wMinute;
	root->time.second = sys.wSecond;
	root->filetype[0] = '\0';
	strcpy_s(root->name,"AAA");
	root->brother = NULL;
	root->firstchild = NULL;
	root->brother = NULL;
}


filetree::~filetree()
{
}

void filetree::shownode(file *q)
{
	if (q)
	{
		printf("%s", q->name);
		if (q->cf)
			printf(".%s", q->filetype);
	}
}

void filetree::unfold(file *p)
{
	shownode(p);
	printf("      ������\n");
	if (!p->firstchild)
	{
		printf("û�д�Ŀ¼���ļ�\n");
		return;
	}
	p = p->firstchild;//��һ�����ӽڵ�
	if (p)
	{
		printf("\t");
		shownode(p);
		printf("\n");
		while (p->brother)//�ֵܽڵ�
		{
			p = p->brother;
			printf("\t");
			shownode(p);
			printf("\n");
		}
	}
}

void filetree::addnode(file *p)
{
	file *q = NULL;
	file *w = NULL;
	if (p->cf)
	{
		printf("��������ļ�!\n");
		return;
	}
	q = new file;
	printf("0.Directory\t1.File\n");//����Ŀ¼�����ļ�
	scanf_s("%d", &q->cf);
	printf("��������:");
	scanf_s("%s", q->name, 20);
	getchar();
	if (q->cf)//�����ļ�����
	{
		printf("�����ļ����ͣ�");
		scanf_s("%s", q->filetype, 10);
		printf("����������");
		scanf_s("%d", &q->size);
		getchar();
	}
	if (p->firstchild)
		w = Compare(q->name, p->firstchild);
	if (w)
	{
		if (!strcmp(q->filetype, w->filetype))
			printf("�Ѿ����ڣ��������!\n");
		return;
	}
	if (!q->cf)//����Ŀ¼����
	{
		q->filetype[0] = '\0';
		q->size = 0;
	}
	GetLocalTime(&sys);
	q->time.year = sys.wYear;
	q->time.month = sys.wMonth;
	q->time.day = sys.wDay;
	q->time.hour = sys.wHour;
	q->time.second = sys.wSecond;
	q->flag = 0;
	q->parent = p;
	q->firstchild = NULL;
	q->brother = NULL;
	q->level = p->level + 1;
	q->n = 0;
	p->n++;
	if (!p->firstchild)
	{
		p->firstchild = q;//���ַ������Ŀ¼���νṹ
	}
	else
	{
		p = p->firstchild;
		while (p->brother)
		{
			p = p->brother;
		}
		p->brother = q;
	}
	printf("��ӳɹ���\n");
}

file * filetree::Locate(file * p)
{
	int i = 0;
	char d, c[10], t[10];
	t[0] = '\0';
	scanf_s("%c", &d);
	scanf_s("%c", &d);//���������Ŀ¼·��
	while (d != '*'&&d != '.')//Ŀ¼·����Ϊ���Լ�������.��ѭ�����ҵ�ǰĿ¼�Լ�����Ŀ¼�Ƿ����������Ŀ¼
	{
		if (d == '\\')
		{
			c[i] = '\0';
			scanf_s("%c", &d);
			i = 0;
			p = Compare(c, p);
			if (!p)
			{
				printf("·������\n");
				return NULL;
			}
			p = p->firstchild;//����Ŀ¼���в���
		}
		c[i++] = d;
		scanf_s("%c", &d);
	}
	if (d == '.')
	{
		i = 0;
		scanf_s("%c", &d);
		while (d != '*')
		{
			t[i++] = d;
			scanf_s("%c", &d);
		}
		t[i] = '\0';
	}
	if (d == '*')//·���������
	{
		c[i] = '\0';
		p = Compare(c, p);
		if (!p)
		{
			printf("Ŀ¼·������\n");
			return NULL;
		}
		if (t[0] != '\0'&&p->filetype[0] != '\0')//·�����ƺϷ�
		{
			if (strcmp(t, p->filetype))
			{
				printf("�ļ�·������\n");
				return NULL;
			}
			return p;
		}
		else if (t[0] == '\0'&&p->filetype[0] == '\0')
			return p;
	}
	printf("·������\n");
	return NULL;
}

void filetree::del(file * p)
{
	file *q,*kid;
	if (p->firstchild != NULL)
	{
		cout << "�����ļ�����ɾ��"<<endl;
	}
	else
	{
		q = p->parent;
		kid = q->firstchild;
		if (strcmp(kid->name, p->name) == 0)
			q->firstchild = kid->brother;
		else
		{
			while (strcmp(kid->brother->name, p->name) == 0)
				kid = kid->brother;
			kid->brother = kid->brother->brother;
		}
		cout << "ɾ���ɹ�"<<endl;
	}
}

void filetree::Traverser(file * p)
{
	int i;
	file *q = p;
	if (!p) return;
	for (i = 0; i<p->level; i++)
	{
		printf("    ");//����
	}
	shownode(q);//��ʾ�ڵ���Ϣ
	printf("\n");
	Traverser(p->firstchild);//��ʾ��һ�����ӽڵ�
	Traverser(p->brother);//��ʾ�ֵܽڵ�
}

void filetree::information(file * p)
{
	printf("name:%s\n", p->name);
	if (p->filetype[0] != '\0')
	{
		printf("�ļ����ͣ�%s\n", p->filetype);
		printf("��С��%dK\n", p->size);
	}
	else printf("�˵�\n");
	printf("����ʱ��:");
	printf("%d%d%d\n", p->time.year, p->time.month, p->time.day);
	printf("�������ļ�����:%d\n", p->n);
}

file * filetree::Compare(char * ch, file * p)
{
	int i;
	if (!p) return NULL;
	if (!p->parent)//�����ڵ�Ƚ�
	{
		i = strcmp(ch, p->name);
		if (!i) return p;
		return NULL;
	}
	p = p->parent;
	p = p->firstchild;
	i = strcmp(ch, p->name);//����һ�����ӽڵ�Ƚ�
	if (!i) return p;//��Ŀ¼���棬����Ŀ¼��ַ
	else
	{
		while (p->brother)//���ֵܽڵ�Ƚ�
		{
			p = p->brother;
			if (!strcmp(ch, p->name)) return p;
		}
		return NULL;//����Ŀ¼���棬���ؿ�
	}
}
