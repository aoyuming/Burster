#pragma once
#include "stdafx.h"
#include <stack>
#include <vector>
using namespace std;

//����ӿ�
class Command
{
public:
	Command() {}
	virtual ~Command() {}

	virtual bool execute() = 0;
	virtual void redo() = 0;
	virtual void undo() = 0;
};

//��������
class ComplexCommand : public Command
{
private:
	//�������
	vector<Command*> m_ComVector;
public:
	ComplexCommand(int count, ...);
	virtual ~ComplexCommand();
	virtual bool execute();
	virtual void redo();
	virtual void undo();

	//�������
	void addCommand(Command* com);
};

//���������
class CommandManager
{
private:
	std::stack<Command*> m_UndoStack;//����ջ
	std::stack<Command*> m_RedoStack;//����ջ

	CommandManager();
	~CommandManager();

	static CommandManager* m_Instance;

public:

	static CommandManager* getInstance();
	static void release();

	//�洢����
	void StoreCommand(Command* cmd);

	//�����������
	void ClearAllCommand();

	//�������
	void ClearCommand(std::stack<Command*>& stack);

	//ִ�г�������
	void undo();

	//ִ����������
	void redo();

	//��ȡ������������
	int GetUndoSize();

	//��ȡ������������
	int GetRedoSize();
};

