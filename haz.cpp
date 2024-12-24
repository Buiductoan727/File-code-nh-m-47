#include "H.h";
#include "scd.h";
#include "haz.h";
using namespace std;
short SPage;
short SIndex;
short SAll;

int main() {
	// xoá con trỏ
	Cursor(false);
	// thay đổi kích thước màn hình
	resizeConsole(ConsoleWidth, ConsoleHeight);
	// đổi tên tiêu đề
	SetConsoleTitle(TEXT("game do min"));
	XacNhan();
	string sign;
	cin >> sign;
	for (int i = 0; i < sign.size(); i++) {
		sign[i] = toupper(sign[i]);
	}
	if (sign == "YES") {
		setColor(7);
		deleteRow(2, 1);
		deleteRow(3, 1);
		deleteRow(8, 1);
		veThongBao();
		string ok;
		cin >> ok;
		for (int i = 0; i < ok.size(); i++) {
			ok[i] = toupper(ok[i]);
		}
		if (ok == "OK") {
			deleteRow(2, 15);
			deleteRow(2, 1);
			deleteRow(5, 1);
			deleteRow(6, 1);
			thietKe();
			veMenu(0);
			SPage = 1;
			xuLySuKien();
		}
		else {//không nhập đúng OK
			deleteRow(0, 30);
			TextColor(14);
			gotoxy(ConsoleWidth / 2 - 15, 15); printf("Cam on ban da choi tro choi cua toi !!");
			setBackgroundColor(0);
			std::cout << std::endl; std::cout << std::endl; std::cout << std::endl; std::cout << std::endl;
			Sleep(2000);
			exit(0);
		}
	}
	else {//không nhập dúng YES
		deleteRow(0, 30);
		TextColor(14);
		gotoxy(ConsoleWidth / 2 - 15, 15); printf("Cam on ban da choi tro choi cua toi !!");
		setBackgroundColor(0);
		std::cout << std::endl; std::cout << std::endl; std::cout << std::endl; std::cout << std::endl;
		Sleep(2000);
		exit(0);
	}
	return 0;
}