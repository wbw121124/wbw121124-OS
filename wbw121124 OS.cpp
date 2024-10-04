// #include<bits/stdc++.h>
// 禁止使用endl
#include<iostream>
#include<fstream>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<windows.h>
#include<Shlobj.h>
#include<thread>
#include<time.h>
#include<map>
#include<vector>
#include<direct.h>
#include<iomanip>
#include<sstream>
#include<cmath>
#include"md5.h"
#define static_castt(x,y) static_cast<x>(y)
using namespace std;
int color;
string username, password;
map<string, string> users;
map<string, void(*)()> commands;
map<string, string> descriptions;
string arg;
string getworkpath();
string path = getworkpath();//getdesktop();
HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
string lower(string s)
{
	for (int i = 0; i < s.size(); i++)
		s[i] = tolower(s[i]);
	return s;
}
string upper(string s)
{
	for (int i = 0; i < s.size(); i++)
		s[i] = toupper(s[i]);
	return s;
}
void changeColor(int color)
{
	//color为颜色值，0-15
	/*
	十六进制颜色代码：
	0 = 黑色	   8 = 灰色
	1 = 蓝色	   9 = 淡蓝色
	2 = 绿色	   A = 淡绿色
	3 = 湖蓝色	 B = 淡浅蓝色
	4 = 红色	   C = 淡红色
	5 = 紫色	   D = 淡紫色
	6 = 黄色	   E = 淡黄色
	7 = 白色	   F = 亮白色
	*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	::color = color;
	return;
}
int getColor()
{
	// 获取当前控制台字体颜色
	return color;
}
void run(string command)
{
	if (command == "")
		return;
	command = lower(command);
	if (commands.find(command) == commands.end())
	{
		int color = getColor();
		changeColor(4);
		cerr << "command '" + command + "' not found\n";
		changeColor(color);
		return;
	}
	commands[command]();
	return;
}
void exit()
{
	changeColor(7);
	exit(0);
	return;//养成return的好习惯
}
void help()
{
	for (auto i : commands)
		cout << i.first << '\t' << descriptions[i.first] << '\n';
	return;
}
//windows系统桌面位置
string getdesktop()
{
	char path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, SHGFP_TYPE_CURRENT, path)))
	{
		// cout << "Desktop path: " << path << '\n';
	}
	else
	{
		int color = getColor();
		changeColor(4);
		cerr << "错误: 获取桌面路径失败\n";
		changeColor(color);
		getcwd(path, sizeof(path));
	}
	return path;
}
string getworkpath()
{
	char path[MAX_PATH];
	getcwd(path, sizeof(path));
	return path;
}
void print(string s, int slp = 10)
{
	for (auto c : s)
	{
		cout << c;
		Sleep(slp);
	}
	return;
}
void addcommand(string name, void(*func)(), string description)
{
	name = lower(name);
	commands[name] = func;
	descriptions[name] = description;
	return;
}
vector<string> split(string command, char c)
{
	vector<string> ans;
	int last = 0, cnt = 0;
	for (int i = 0; i < command.size(); i++)
	{
		// 如果i前面有偶数个'\'或者没有，则可以划分
		if (!(cnt & 1) && command[i] == c)
		{
			ans.push_back(command.substr(last, i - last));
			last = i + 1;
		}
		if (command[i] == '\\')
			cnt++;
		else
			cnt = 0;
	}
	ans.push_back(command.substr(last, command.size() - last));
	return ans;
}
void sign_in()
{
	print("请注册:\n请输入用户名\n\tUser Name:", 50);
	cin >> username;
	print("请输入密码\n\tPassword:", 50);
	DWORD mode;
	GetConsoleMode(hConsole, &mode);
	SetConsoleMode(hConsole, mode & ~ENABLE_ECHO_INPUT);
	cin >> password;
	cin.get();
	SetConsoleMode(hConsole, mode);
	username = lower(username);
	password = lower(password);
	cout << '\n';
	pair<string, string> user;
	ifstream fin("users.wos.txt");
	if (fin.is_open())
	{
		while (fin >> user.first >> user.second)
			if (user.first == username)
			{
				int color = getColor();
				changeColor(4);
				cerr << "错误: 用户名已存在\n";
				changeColor(color);
				return;
			}
	}
	ofstream fout("users.wos.txt", ios::app);
	fout << username << ' ' << MD5::getMD5(password) << '\n';
	return;
}
void init()
{
	addcommand("exit", exit, "退出OS");
	addcommand("help", help, "显示帮助信息");
	addcommand("cls", []() { system("cls"); }, "清屏");
	addcommand("print", []() { print(arg + "\n"); }, "打印字符串");
	addcommand("&", []() {
			system(("powershell " + arg).c_str());
		}, "运行powershell命令");
	addcommand("cd", []() {
		int ret = chdir(arg.c_str());
		if (ret == 0)
		{
			char cwd[PATH_MAX];
			if (getcwd(cwd, sizeof(cwd)) != NULL)
				path = cwd;
			else
			{
				int color = getColor();
				changeColor(4);
				cerr << "错误: 获取工作目录失败\n";
				changeColor(color);
			}
		}
		else
		{
			int color = getColor();
			changeColor(4);
			cerr << "错误: 设置工作目录失败\n错误信息: \n\t";
			perror(strerror(ret));
			changeColor(color);
		}
	}, "切换工作目录");
	addcommand("wos", []() { print("wbw121124 OS 1.1.0\n\tdev by wbw121124\n\t一个命令提示符\n"); }, "输出版本信息");
	addcommand("error", []() {
		if (arg == "")
			for(int i = 0; i <= 42; i++)
				cout << i << ' ' << strerror(i) << '\n';
		else
			cout << arg << ' ' << strerror(atoi(arg.c_str())) << '\n';
	}, "输出STL的错误代码和解释");
	addcommand("md5", []() { print(MD5::getMD5(arg) + '\n'); }, "计算字符串的MD5值");
	addcommand("adduser", []() { sign_in(); }, "添加用户,并登录添加的用户");
	return;
}
signed main()
{
	pair<string, string> user;
	changeColor(10);
	string commands;
	chdir(path.c_str());
	init();
	ifstream fin("users.wos.txt"); // 读取用户名和密码
	if (!fin.is_open())
	{
		goto signin;
		// int color = getColor();
		// changeColor(4);
		// cerr << "错误: 打开用户文件失败\n";
		// changeColor(color);
		// return -1;
	}
	while (fin >> user.first >> user.second)
		users[lower(user.first)] = lower(user.second);
	if (users.empty())
		goto signin;
	print("请输入用户名\n\tUser Name:", 50);
	cin >> username;
	print("请输入密码\n\tPassword:", 50);
	DWORD mode;
	GetConsoleMode(hConsole, &mode);
	SetConsoleMode(hConsole, mode & ~ENABLE_ECHO_INPUT);
	cin >> password;
	cin.get();
	SetConsoleMode(hConsole, mode);
	username = lower(username);
	password = lower(password);
	if (username == "guest")
		goto logined;
	cout << '\n';
	if (users.find(username) == users.end() || users[username] != MD5::getMD5(password))
	{
		int color = getColor();
		changeColor(4);
		cerr << "错误: 用户名或密码错误\n";
		changeColor(color);
		return -1;
	}
	goto logined;
logined:
	print("欢迎使用wbw121124OS, " + username + "!\n", 100);
	while (true)
	{
		changeColor(10);
		cout << username << '@'/*"wbw121124OS "*/ << path << " $ ";
		changeColor(7);
		if(!getline(cin, commands))
		{
			int color = getColor();
			changeColor(4);
			cerr << "错误: 读取命令失败，输入了关闭流操作符\n";
			changeColor(color);
			return -1;
		}
		changeColor(10);
		//将commands按';'划分并运行
		for (auto command : split(commands, ';'))
		{
			//将'\;'替换为';'
			for (int i = 0; i < (int)command.size() - 1; i++)
			    if (command[i] == '\\' && command[i + 1] == ';')
			    	command.erase(i, 1);
			//将'\\'替换为'\'
			for (int i = 0; i < (int)command.size() - 1; i++)
			    if (command[i] == '\\' && command[i + 1] == '\\')
			    	command.erase(i, 1);
			auto space = command.find(' ');
			if (space == command.npos)
			{
				arg = "";
				run(command);
			}
			else
			{
				arg = command.substr(space + 1, command.size() - space);
				run(command.substr(0, space));
			}
		}
	}
	changeColor(7);
	return 0;
signin:
	sign_in();
	goto logined;
	return 0;
}