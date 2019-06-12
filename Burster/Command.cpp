#include "stdafx.h"
#include "Command.h"

CommandManager* CommandManager::m_Instance = NULL;
CommandManager* CommandManager::getInstance()
{
	if (NULL == m_Instance)
		m_Instance = new CommandManager;
	return m_Instance;
}

void CommandManager::release()
{
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

CommandManager::CommandManager()
{

}

CommandManager::~CommandManager()
{
	ClearAllCommand();
}

//�洢����
void CommandManager::StoreCommand(Command* cmd)
{
	m_UndoStack.push(cmd);
	ClearCommand(m_RedoStack);//�����������
}

//�����������
void CommandManager::ClearAllCommand()
{
	ClearCommand(m_UndoStack);
	ClearCommand(m_RedoStack);
}

//�������
void CommandManager::ClearCommand(std::stack<Command*>& stack)
{
	if (stack.empty()) 
		return;

	while (!stack.empty())
	{
		delete stack.top();
		stack.pop();
	}
}

//ִ�г�������
void CommandManager::undo()
{
	if (m_UndoStack.empty())
		return;

	Command* cmd = m_UndoStack.top();
	cmd->undo();
	m_RedoStack.push(cmd);
	m_UndoStack.pop();
}

//ִ����������
void CommandManager::redo()
{
	if (m_RedoStack.empty())
		return;

	Command* cmd = m_RedoStack.top();
	cmd->redo();
	m_UndoStack.push(cmd);
	m_RedoStack.pop();
}


//��ȡ������������
int CommandManager::GetUndoSize()
{
	return m_UndoStack.size();
}

//��ȡ������������
int CommandManager::GetRedoSize()
{
	return m_RedoStack.size();
}
