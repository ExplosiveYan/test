#pragma once
class filetree
{
public:
	filetree();
	~filetree();
	void shownode(file *q);
	void unfold(file *p);
	void addnode(file *p);
	file *Locate(file *p);
	void del(file *p);
	void Traverser(file *p);
	void information(file *p);
	file * Compare(char *ch, file *p);
	file *root;
private:

};

