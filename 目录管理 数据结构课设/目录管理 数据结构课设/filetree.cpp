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
	int n;              //数量
	int level;           //层次
	int size;             //大小
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
	printf("      包括：\n");
	if (!p->firstchild)
	{
		printf("没有此目录或文件\n");
		return;
	}
	p = p->firstchild;//第一个孩子节点
	if (p)
	{
		printf("\t");
		shownode(p);
		printf("\n");
		while (p->brother)//兄弟节点
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
		printf("不能添加文件!\n");
		return;
	}
	q = new file;
	printf("0.Directory\t1.File\n");//创建目录还是文件
	scanf_s("%d", &q->cf);
	printf("输入名字:");
	scanf_s("%s", q->name, 20);
	getchar();
	if (q->cf)//创建文件步骤
	{
		printf("输入文件类型：");
		scanf_s("%s", q->filetype, 10);
		printf("输入数量：");
		scanf_s("%d", &q->size);
		getchar();
	}
	if (p->firstchild)
		w = Compare(q->name, p->firstchild);
	if (w)
	{
		if (!strcmp(q->filetype, w->filetype))
			printf("已经存在，不能添加!\n");
		return;
	}
	if (!q->cf)//创建目录步骤
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
		p->firstchild = q;//存地址，建立目录树形结构
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
	printf("添加成功！\n");
}

file * filetree::Locate(file * p)
{
	int i = 0;
	char d, c[10], t[10];
	t[0] = '\0';
	scanf_s("%c", &d);
	scanf_s("%c", &d);//接收输入的目录路径
	while (d != '*'&&d != '.')//目录路径不为空以及不包含.，循环查找当前目录以及其子目录是否存在所输入目录
	{
		if (d == '\\')
		{
			c[i] = '\0';
			scanf_s("%c", &d);
			i = 0;
			p = Compare(c, p);
			if (!p)
			{
				printf("路径有误！\n");
				return NULL;
			}
			p = p->firstchild;//对子目录进行查找
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
	if (d == '*')//路径结束标记
	{
		c[i] = '\0';
		p = Compare(c, p);
		if (!p)
		{
			printf("目录路径有误！\n");
			return NULL;
		}
		if (t[0] != '\0'&&p->filetype[0] != '\0')//路径名称合法
		{
			if (strcmp(t, p->filetype))
			{
				printf("文件路径有误！\n");
				return NULL;
			}
			return p;
		}
		else if (t[0] == '\0'&&p->filetype[0] == '\0')
			return p;
	}
	printf("路径有误！\n");
	return NULL;
}

void filetree::del(file * p)
{
	file *q,*kid;
	if (p->firstchild != NULL)
	{
		cout << "有子文件不能删除"<<endl;
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
		cout << "删除成功"<<endl;
	}
}

void filetree::Traverser(file * p)
{
	int i;
	file *q = p;
	if (!p) return;
	for (i = 0; i<p->level; i++)
	{
		printf("    ");//缩进
	}
	shownode(q);//显示节点信息
	printf("\n");
	Traverser(p->firstchild);//显示第一个孩子节点
	Traverser(p->brother);//显示兄弟节点
}

void filetree::information(file * p)
{
	printf("name:%s\n", p->name);
	if (p->filetype[0] != '\0')
	{
		printf("文件类型：%s\n", p->filetype);
		printf("大小：%dK\n", p->size);
	}
	else printf("菜单\n");
	printf("创建时间:");
	printf("%d%d%d\n", p->time.year, p->time.month, p->time.day);
	printf("包含的文件数量:%d\n", p->n);
}

file * filetree::Compare(char * ch, file * p)
{
	int i;
	if (!p) return NULL;
	if (!p->parent)//跟父节点比较
	{
		i = strcmp(ch, p->name);
		if (!i) return p;
		return NULL;
	}
	p = p->parent;
	p = p->firstchild;
	i = strcmp(ch, p->name);//跟第一个孩子节点比较
	if (!i) return p;//在目录里面，返回目录地址
	else
	{
		while (p->brother)//跟兄弟节点比较
		{
			p = p->brother;
			if (!strcmp(ch, p->name)) return p;
		}
		return NULL;//不在目录里面，返回空
	}
}
