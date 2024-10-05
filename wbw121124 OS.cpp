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
#include"rsa.h"
using namespace std;
int color;
string username, password;
map<string, string> users;
map<string, void(*)()> commands;
map<string, string> descriptions;
map<string, string> tempstrings;
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
void colorchange(int color1, void(*func)())
{
	int color = getColor();
	changeColor(color1);
	func();
	changeColor(color);
	return;
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
int help_ts;
void help()
{
	for (auto i : commands)
	{
		string t;
		for (int j = i.first.size(); j < help_ts * 8; j++)
			t += " ";
		cout << i.first << t << descriptions[i.first] << '\n';
	}
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
	help_ts = max(help_ts, (int)ceil(name.size() / 4.00));
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
	cout << '\n';
	pair<string, string> user;
	ifstream fin("users.wos.txt");
	if (fin.is_open())
	{
		while (fin >> user.first >> user.second)
			if (user.first == username)
			{
				colorchange(4, []() {
					cerr << "����: �û����Ѵ���\n";
				});
				return;
			}
	}
	ofstream fout("users.wos.txt", ios::app);
	fout << username << ' ' << MD5::getMD5(password) << '\n';
	users[username] = MD5::getMD5(password);
	return;
}
void change_password()
{
	string old, now;
	DWORD mode;
	GetConsoleMode(hConsole, &mode);
	SetConsoleMode(hConsole, mode & ~ENABLE_ECHO_INPUT);
	print("�����������\n\tUser Name:", 50);
	cin >> old;
	cin.get();
	if (password != old)
	{
		colorchange(4, []() {
			cerr << "����: �������\n";
		});
		return;
	}
	print("\n������������\n\tPassword:", 50);
	cin >> now;
	cin.get();
	SetConsoleMode(hConsole, mode);
	cout << '\n';
	users[username] = MD5::getMD5(now);
	password = now;
	ofstream fout("users.wos.txt", ios::trunc);
	for (auto i : users)
		fout << i.first << ' ' << i.second << '\n';
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
				colorchange(4, []() {
					cerr << "����: ���ù���Ŀ¼ʧ��\n";
				});
			}
		}
		else
		{
			tempstrings["cderror"] = strerror(ret);
			colorchange(4, []() {
				cerr << "����: ���ù���Ŀ¼ʧ��\n������Ϣ: \n\t";
				perror(tempstrings["cderror"].c_str());
			});
		}
	}, "�л�����Ŀ¼");
	addcommand("wos", []() { print("wbw121124 OS 1.1.5\n\tdev by wbw121124\n\tһ��������ʾ��\n"); }, "����汾��Ϣ");
	addcommand("error", []() {
		if (arg == "")
			for(int i = 0; i <= 42; i++)
				cout << i << ' ' << strerror(i) << '\n';
		else
			cout << arg << ' ' << strerror(atoi(arg.c_str())) << '\n';
	}, "���STL�Ĵ������ͽ���");
	addcommand("md5", []() { print(MD5::getMD5(arg) + '\n'); }, "�����ַ�����MD5ֵ");
	addcommand("adduser", []() { sign_in(); }, "����û�,����¼��ӵ��û�");
	addcommand("password", []() { change_password(); }, "�޸��û�����");
	addcommand("rsa", []() {
		string mode, text;
		while (true)
		{
			cout << "����������\n\t(sc, ja, jie, exit)>";
			if (!(cin >> mode))
				break;
			if (mode == "sc")
			{
			    cout << "�������Ƿ���������ͬ�ġ����ǲ�ͬ������������\n\t(y, n)>";
				cin >> mode;
				if (mode == "y")
				{
				    long long p, q, n, e, d;
					cout << "������������ͬ�ġ����ǲ�ͬ������������\n\tp, q>";
					cin >> p >> q;
					RSA::generateRSAKeys(p, q, n, e, d, true);
					cout << "��Կ: e, n(" << e << ", " << n << ")\n˽Կ: d, n(" << d << ", " << n << ")\n";
				}
				else
				{
					long long p, q, n, e, d;
					RSA::generateRSAKeys(p, q, n, e, d);
					cout << "��Կ: e, n(" << e << ", " << n << ")\n˽Կ: d, n(" << d << ", " << n << ")\n";
				}
				colorchange(6, []() {cout << "�뱣�ܺ�������Կ\n";});
			}
			else if (mode == "ja")
			{
				long long e, n;
				cout << "������˽Կ\n\t(e, n)>";
				cin >> e >> n;
				cin.get();
				cout << "������Ҫ���ܵ��ַ���\n\t>";
				getline(cin, text);
				cout << "���ܺ���ַ���: " << RSA::rsaEncryptstring(text, e, n) << '\n';
			}
			else if (mode == "jie")
			{
				long long d, n;
				cout << "�����빫Կ\n\t(d, n)>";
				cin >> d >> n;
				cin.get();
				cout << "������Ҫ���ܵ��ַ���\n\t>";
				getline(cin, text);
				cout << "���ܺ���ַ���: " << RSA::rsaDecryptstring(text, d, n) << '\n';
			}
			else if (mode == "exit")
				break;
		}
	}, "RSA���ܽ���");
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
		users[lower(user.first)] = user.second;
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
	if (username == "guest")
		goto logined;
	cout << '\n';
	if (users.find(username) == users.end() || users[username] != MD5::getMD5(password))
	{
		colorchange(6, []() {cerr << "����: �û������������\n";});
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
			colorchange(6, []() {cerr << "����: ��ȡ����ʧ�ܣ������˹ر���������\n";});
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