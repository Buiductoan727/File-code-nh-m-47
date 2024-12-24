#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <atomic>
#include <fstream>

#define Left_Click 0x5A
#define Right_Click 0x58

#define ConsoleWidth 156
#define ConsoleHeight 35

extern short SPage;
extern short SIndex;
extern short SAll;

struct CauTrucBang {//cấu trúc của bảng
	short SDong = 0;
	short SCot = 0;
	short SSoBom = 0;
	short SSoODaMo = 0;
	short SSoCo = 0;
};
struct CauTrucO {//cấu trúc của các ô
	bool BCoBom = false; // Ô có bom
	bool BDaMo = false; // Những ô đã được mở
	bool BDaCamCo = false; // Những ô đã được cắm cờ
	short SBomXungQuanh = false; // Những quả bom xung quanh một ô
};