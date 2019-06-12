#pragma once
#include "stdafx.h"
#include <stack>


class Command
{
public:
	Command() {}
	virtual ~Command() {}

	virtual bool execute() = 0;
	virtual void redo() = 0;
	virtual void undo() = 0;
};


class CommandManager
{
private:
	std::stack<Command*> m_UndoStack;//撤销栈
	std::stack<Command*> m_RedoStack;//重做栈

	CommandManager();
	~CommandManager();

	static CommandManager* m_Instance;

public:

	static CommandManager* getInstance();
	static void release();

	//存储命令
	void StoreCommand(Command* cmd);

	//清除所有命令
	void ClearAllCommand();

	//清除命令
	void ClearCommand(std::stack<Command*>& stack);

	//执行撤销命令
	void undo();

	//执行重做命令
	void redo();

	//获取撤销命名个数
	int GetUndoSize();

	//获取重做命名个数
	int GetRedoSize();
};

