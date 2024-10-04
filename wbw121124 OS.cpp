// #include<bits/stdc++.h>
// ��ֹʹ��endl
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
	//colorΪ��ɫֵ��0-15
	/*
	ʮ��������ɫ���룺
	0 = ��ɫ	   8 = ��ɫ
	1 = ��ɫ	   9 = ����ɫ
	2 = ��ɫ	   A = ����ɫ
	3 = ����ɫ	 B = ��ǳ��ɫ
	4 = ��ɫ	   C = ����ɫ
	5 = ��ɫ	   D = ����ɫ
	6 = ��ɫ	   E = ����ɫ
	7 = ��ɫ	   F = ����ɫ
	*/
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	::color = color;
	return;
}
int getColor()
{
	// ��ȡ��ǰ����̨������ɫ
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
	return;//����return�ĺ�ϰ��
}
void help()
{
	for (auto i : commands)
		cout << i.first << '\t' << descriptions[i.first] << '\n';
	return;
}
//windowsϵͳ����λ��
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
		cerr << "����: ��ȡ����·��ʧ��\n";
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
		// ���iǰ����ż����'\'����û�У�����Ի���
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
	print("��ע��:\n�������û���\n\tUser Name:", 50);
	cin >> username;
	print("����������\n\tPassword:", 50);
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
				cerr << "����: �û����Ѵ���\n";
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
	addcommand("exit", exit, "�˳�OS");
	addcommand("help", help, "��ʾ������Ϣ");
	addcommand("cls", []() { system("cls"); }, "����");
	addcommand("print", []() { print(arg + "\n"); }, "��ӡ�ַ���");
	addcommand("&", []() {
			system(("powershell " + arg).c_str());
		}, "����powershell����");
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
				cerr << "����: ��ȡ����Ŀ¼ʧ��\n";
				changeColor(color);
			}
		}
		else
		{
			int color = getColor();
			changeColor(4);
			cerr << "����: ���ù���Ŀ¼ʧ��\n������Ϣ: \n\t";
			perror(strerror(ret));
			changeColor(color);
		}
	}, "�л�����Ŀ¼");
	addcommand("wos", []() { print("wbw121124 OS 1.1.0\n\tdev by wbw121124\n\tһ��������ʾ��\n"); }, "����汾��Ϣ");
	addcommand("error", []() {
		if (arg == "")
			for(int i = 0; i <= 42; i++)
				cout << i << ' ' << strerror(i) << '\n';
		else
			cout << arg << ' ' << strerror(atoi(arg.c_str())) << '\n';
	}, "���STL�Ĵ������ͽ���");
	addcommand("md5", []() { print(MD5::getMD5(arg) + '\n'); }, "�����ַ�����MD5ֵ");
	addcommand("adduser", []() { sign_in(); }, "����û�,����¼��ӵ��û�");
	return;
}
signed main()
{
	pair<string, string> user;
	changeColor(10);
	string commands;
	chdir(path.c_str());
	init();
	ifstream fin("users.wos.txt"); // ��ȡ�û���������
	if (!fin.is_open())
	{
		goto signin;
		// int color = getColor();
		// changeColor(4);
		// cerr << "����: ���û��ļ�ʧ��\n";
		// changeColor(color);
		// return -1;
	}
	while (fin >> user.first >> user.second)
		users[lower(user.first)] = lower(user.second);
	if (users.empty())
		goto signin;
	print("�������û���\n\tUser Name:", 50);
	cin >> username;
	print("����������\n\tPassword:", 50);
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
		cerr << "����: �û������������\n";
		changeColor(color);
		return -1;
	}
	goto logined;
logined:
	print("��ӭʹ��wbw121124OS, " + username + "!\n", 100);
	while (true)
	{
		changeColor(10);
		cout << username << '@'/*"wbw121124OS "*/ << path << " $ ";
		changeColor(7);
		if(!getline(cin, commands))
		{
			int color = getColor();
			changeColor(4);
			cerr << "����: ��ȡ����ʧ�ܣ������˹ر���������\n";
			changeColor(color);
			return -1;
		}
		changeColor(10);
		//��commands��';'���ֲ�����
		for (auto command : split(commands, ';'))
		{
			//��'\;'�滻Ϊ';'
			for (int i = 0; i < (int)command.size() - 1; i++)
			    if (command[i] == '\\' && command[i + 1] == ';')
			    	command.erase(i, 1);
			//��'\\'�滻Ϊ'\'
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