#pragma once

//检查更新的方式
enum UpdateMode
{
	INVARIABLY,//总是检查更新
	WEEKLY,//每周检查更新
	NEVER//从不检查更新
};

//当前更新的状态
enum UpdateState
{
	NEWEST,//最新版本
	ERROR_1//更新出错
};