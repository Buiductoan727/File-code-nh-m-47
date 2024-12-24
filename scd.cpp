#include "scd.h"
#include "H.h";
#include "haz.h";

CauTrucBang CTBang;
CauTrucO** CTO;
CauTrucO CCTO[25][25];

// biến cho cin.ignore
int check = 0;
// biến cho name save
std::string nameDe;
std::string nameThuong;
std::string nameKho;
// biến cho level
int capDo = 0;
// biến cho thời gian hiện tại 
int ctimer = 0;
int cctime = 0;
int ctimer3 = 500000;
int ctimer4 = 1000000;
int ctimer5 = 10000000;
// biến cho timer
int total_time;
// toạ độ x, y để vẽ bảng
short StoaDoX;
short StoaDoY;
// cập nhập tình hình hiện tại
bool theoDoiConTro = false;
//tạo và xoá mảng 2 chiều động
void taoMang2chieuDong() {

	CTO = new CauTrucO *[CTBang.SCot]; // khai báo con trỏ ở đầu dòng điều khiển cả dòng

	for (int i = 0; i < CTBang.SCot; i++) {
		CTO[i] = new CauTrucO[CTBang.SDong]; //ở cột tạo 1 mảng mới điều khiển từng ô trong cột.
	}
}
//xoá mảng
void xoaMang2chieuDong() {

	for (int i = 0; i < CTBang.SCot; i++) {
		delete[] CTO[i]; // xoá từng ô liên tiếp
	}

	delete[] CTO; // xoá con trỏ đầu dòng
}
// lưu toạ độ bảng
void luuBang() {
	StoaDoX = ((ConsoleWidth / 2) - CTBang.SCot);
	StoaDoY = (((ConsoleHeight - 6) - (CTBang.SDong)) / 2 + 7);
}
// The current pointer
COORD CViTriConTro;
// Whether the user use the key?
bool BUsingKey = false;
// tạo map
void khoitao(short SDong, short SCot, short SSoBom) {

	CTBang.SDong = SDong;
	CTBang.SCot = SCot;
	CTBang.SSoBom = SSoBom;
	CTBang.SSoODaMo = 0;
	CTBang.SSoCo = 0;

	taoMang2chieuDong();
	randomBomb();
	luuBang();
	CViTriConTro = { 0,0 };
	theoDoiConTro = true;
	veBang();
	veTrangThai(1, 0, 0);

}
// nhận toạ độ X
short toaDoX(short SX) {
	return (SX * 2 + StoaDoX);
}
// nhận toạ độ Y
short toaDoY(short SY) {
	return (SY + StoaDoY);
}
// Draw Cell
void veO(short SX, short SY, short SKieu) {
	switch (SKieu) {
	case 0: // ô rỗng - Green
		setColorCell(toaDoX(SX), toaDoY(SY), 0, 10, "  ");
		break;
	case 1: // số 1 (chữ mà xanh trời). từ 1 -> 8: trắng
		setColorCell(toaDoX(SX) , toaDoY(SY)  , 9, 15, "1 ");
		break;
	case 2: // xanh lá
		setColorCell(toaDoX(SX) , toaDoY(SY) , 2, 15, "2 ");
		break;
	case 3: // đỏ
		setColorCell(toaDoX(SX) , toaDoY(SY) , 12, 15, "3 ");
		break;
	case 4: // xanh trong
		setColorCell(toaDoX(SX) , toaDoY(SY) , 1, 15, "4 ");
		break;
	case 5: // đỏ
		setColorCell(toaDoX(SX) , toaDoY(SY) , 4, 15, "5 ");
		break;
	case 6: // Strong CYAN
		setColorCell(toaDoX(SX)  , toaDoY(SY) , 3, 15, "6 ");
		break;
	case 7: // đen
		setColorCell(toaDoX(SX)  , toaDoY(SY) , 0, 15, "7 ");
		break;
	case 8: // hồng
		setColorCell(toaDoX(SX) , toaDoY(SY)  , 13, 15, "8 ");
		break;
	case 9: // Bomb
		setColorCell(toaDoX(SX), toaDoY(SY), 0, 12, "B ");
		break;
	case 10: // ô chẵn
		setColorCell(toaDoX(SX)  , toaDoY(SY) , 0, 8, "  ");
		break;
	case 11: // ô lẻ
		setColorCell(toaDoX(SX) , toaDoY(SY) , 0, 7, "  ");
		break;
	case 12: // Theo con trỏ
		setColorCell(toaDoX(SX) + 1, toaDoY(SY), 0, 13, " ");
		break;
	case 13: // cắm cờ
		setColorCell(toaDoX(SX), toaDoY(SY), 12, 14, "F ");
		break;
	case 14: // cắm cờ sai
		setColorCell(toaDoX(SX), toaDoY(SY), 15, 6, "sai");
		break;
	case 15: // cắm cờ đúng ô có bom
		setColorCell(toaDoX(SX), toaDoX(SY), 12, 14, "B ");
		break;
	}
}
// vẽ bảng (map của game)
void veBang() {
	for (int i = 0; i < CTBang.SDong; i++) {
		for (int j = 0; j < CTBang.SCot; j++) {
			// vẽ cờ
			if (CTO[i][j].BDaCamCo) {
				veO(j, i, 13);
			}
			// vẽ bom
			else if (CTO[i][j].SBomXungQuanh) {
				veO(j, i, CTO[i][j].SBomXungQuanh);
			}
			// ô trống
			else if (CTO[i][j].BDaMo) {
				veO(j, i, 0);
			}
			// ô chẵn
			else if ((i % 2 && j % 2) || !(i % 2 || j % 2)) {
				veO(j, i, 10);
			}
			// ô lẻ
			else {
				veO(j, i, 11);
			}
			if (BUsingKey) {
				veO(CViTriConTro.X, CViTriConTro.Y, 12);
			}
		}
	}
}
//tạo bom ngẫu nhiên
void randomBomb() {
	short SSoBom = CTBang.SSoBom;
	short SI, SJ; // SI: Dòng, SJ: cột (ngẫu nhiên)
	srand(time(NULL));
	while (SSoBom) {
		SI = rand() % CTBang.SDong;
		SJ = rand() % CTBang.SCot;
		if (CTO[SI][SJ].BCoBom) {
			continue;
		}
		CTO[SI][SJ].BCoBom = true;
		SSoBom--;
	}
}
// xuất bom
void xuatBom() {
	for (int i = 0; i < CTBang.SDong; i++) {
		for (int j = 0; j < CTBang.SCot; j++) {
			std::cout << CTO[i][j].BCoBom << " ";
		}
		std::cout << std::endl;
	}
}
// xử lý RightClick - đặt cờ
void RightClick(short SX, short SY) {
	if (SPage == 3) {
		if (!CTO[SX][SY].BDaMo) {
			if (CTO[SX][SY].BDaCamCo) {
				CTO[SX][SY].BDaCamCo = false;
				CTBang.SSoCo--;
			}
			else {
				CTO[SX][SY].BDaCamCo = true;
				CTBang.SSoCo++;
			}
		}
		veBang();
		// cập nhập số bom còn lại
		if (CTBang.SSoBom - CTBang.SSoCo >= 0) {
			deleteRow(4, 1);
			veTrangThai(1, 0, 0);
		}
	}
}
// đếm bom lân cận
short DemBomLanCan(short SX, short SY) {
	short SDem = 0;
	for (int i = SX - 1; i <= SX + 1; i++) {
		for (int j = SY - 1; j <= SY + 1; j++) {
			if (i < 0 || i >= CTBang.SDong || j < 0 || j >= CTBang.SCot || (i == SX && j == SY)) {
				continue;
			}
			// kiểm tra bom
			if (CTO[i][j].BCoBom) {
				SDem++;
			}
		}
	}
	return SDem;
}
// mở chức năng ô
void moO(short SX, short SY) {
	if (!CTO[SX][SY].BDaMo && !CTO[SX][SY].BDaCamCo) {
		CTO[SX][SY].BDaMo = true;
		// ô có bom
		if (CTO[SX][SY].BCoBom) {
			thua(); // thua
		}
		else {
			CTBang.SSoODaMo++;
			short SBomXungQuanh = DemBomLanCan(SX, SY);
			if (SBomXungQuanh) {
				CTO[SX][SY].SBomXungQuanh = SBomXungQuanh;
			}
			// ô trống
			else {
				// Kiểm tra xung quanh các ô và gọi đệ quy
				for (int i = SX - 1; i <= SX + 1; i++) {
					for (int j = SY - 1; j <= SY + 1; j++) {
						if (i < 0 || i >= CTBang.SDong || j < 0 || j >= CTBang.SCot || (i == SX && j == SY)) {
							continue;
						}
						// goi đệ quy
						moO(i, j);
					}
				}
			}
		}
	}
}
// phân tích thắng, thua
static bool Compare() {
	return ((CTBang.SSoODaMo + CTBang.SSoBom) == (CTBang.SCot * CTBang.SDong));
}
// xử lý phím LeftClick - mở ô
void LeftClick(short SX, short SY) {
	if (SPage == 3 && !CTO[SX][SY].BDaMo && !CTO[SX][SY].BDaCamCo) {
		moO(SX, SY);
		if (theoDoiConTro) {
			veBang();
			if (Compare()) {
				thang();
			}
		}
	}
}
/*--------------------------------------------------------------------xử-lý-thắng-và-thua-------------------------------------------------------------------------*/
// thắng
void thang() {
	theoDoiConTro = false;
	xoaMang2chieuDong();
	SPage = 5; SIndex = 0;
	deleteRow(4, 1);
	veTrangThai(2, 2, 0); // cập nhập trạng thái chiến thắng
}
// thua
void thua() {
	SIndex = 0;
	// hiển thị Bomb và cờ đúng hoặc sai
	for (int i = 0; i < CTBang.SDong; i++) {
		for (int j = 0; j < CTBang.SCot; j++) {
			if (CTO[i][j].BDaCamCo) {
				if (CTO[i][j].BCoBom) {
					veO(j, i, 15); //  flag đúng (có Bomb)
				}
				else {
					veO(j, i, 14); // cờ sai
				}
			}
			else {
				// không có cờ
				if (CTO[i][j].BCoBom) {
					veO(j, i, 9); // hiển thị bom
				}
			}
		}
	}

	theoDoiConTro = false;
	xoaMang2chieuDong(); // xoá mảng
	SPage = 4;
	deleteRow(4, 1);
	veTrangThai(3, 3, 0); // trạng thái thua
}
/*-------------------------------------------------------------------------bàn-phím---------------------------------------------------------------------------------*/
// xử lý đầu vào các phím
static void xuLyPhim(KEY_EVENT_RECORD key) {
	// Kiểm tra xem người dùng có nhập khóa không
	if (key.bKeyDown) {
		switch (key.wVirtualKeyCode) {
			// The up key
		case VK_UP:
			switch (SPage) {
			case 1: // menu chính
				if (SAll == 5) {
					if (SIndex == 0) {
						SIndex = SAll - 1;
					}
					else {
						SIndex -= 1;
					}
					veMenu(SIndex);
				}
				break;
			case 2: // Level 
				if (SAll == 4) {
					if (SIndex == 0) {
						SIndex = SAll - 1;
					}
					else {
						SIndex -= 1;
					}
					veCapDoChoi(SIndex);
				}
				break;
			case 3: // Trang chơi
				if (theoDoiConTro) {
					BUsingKey = true;
					CViTriConTro.Y = ((CViTriConTro.Y == 0) ? CTBang.SDong - 1 : CViTriConTro.Y - 1);
					veBang();
				}
				break;
			case 4: // thua
				if (SAll == 2) {
					if (SIndex == 0) {
						SIndex = SAll - 1;
					}
					else {
						SIndex -= 1;
					}
					veTrangThai(3, 3, SIndex);
				}
				break;
			case 5: // thắng
				if (SAll == 2) {
					if (SIndex == 0) {
						SIndex = SAll - 1;
					}
					else {
						SIndex -= 1;
					}
					veTrangThai(2, 2, SIndex);
				}
				break;
			case 6: // lưu trang
				if (SAll == 2) {
					if (SIndex == 0) {
						SIndex = SAll - 1;
					}
					else {
						SIndex -= 1;
					}
					veTrangThai(1, 1, SIndex);
				}
				break;
			}
			break;
			// The down key
		case VK_DOWN:
			switch (SPage) {
			case 1: // menu chính
				if (SAll == 5) {
					if (SIndex == SAll - 1) {
						SIndex = 0;
					}
					else {
						SIndex += 1;
					}
					veMenu(SIndex);
				}
				break;
			case 2: // Level 
				if (SAll == 4) {
					if (SIndex == SAll - 1) {
						SIndex = 0;
					}
					else {
						SIndex += 1;
					}
					veCapDoChoi(SIndex);
				}
				break;
			case 3: // trang chơi
				if (theoDoiConTro) {
					BUsingKey = true;
					CViTriConTro.Y = ((CViTriConTro.Y == CTBang.SDong - 1) ? 0 : CViTriConTro.Y + 1);
					veBang();
				}
				break;
			case 4: // thua
				if (SAll == 2) {
					if (SIndex == SAll - 1) {
						SIndex = 0;
					}
					else {
						SIndex += 1;
					}
					veTrangThai(3, 3, SIndex);
				}
				break;
			case 5: // thắng
				if (SAll == 2) {
					if (SIndex == SAll - 1) {
						SIndex = 0;
					}
					else {
						SIndex += 1;
					}
					veTrangThai(2, 2, SIndex);
				}
				break;
			case 6: // lưu trang
				if (SAll == 2) {
					if (SIndex == 0) {
						SIndex = SAll - 1;
					}
					else {
						SIndex -= 1;
					}
					veTrangThai(1, 1, SIndex);
				}
				break;
			}
			break;
			// The left key
		case VK_LEFT:
			if (theoDoiConTro) {
				BUsingKey = true;
				CViTriConTro.X = ((CViTriConTro.X == 0) ? CTBang.SCot - 1 : CViTriConTro.X - 1);
				veBang();
			}
			break;
			// The right key
		case VK_RIGHT:
			if (theoDoiConTro) {
				BUsingKey = true;
				CViTriConTro.X = ((CViTriConTro.X == CTBang.SCot - 1) ? 0 : CViTriConTro.X + 1);
				veBang();
			}
			break;
			// The enter key
		case VK_RETURN:
			switch (SPage) {
			case 1: // menu chính
				// chọn level
				if (SIndex == 0) {
					SPage = 2;
					deleteRow(4, 10);
					veCapDoChoi(0);
				}
				// tiếp tục chơi 
				else if (SIndex == 1) {
					check++;
					taiGame();
					deleteRow(3, 30);
					SPage = 3;
					CViTriConTro = { 0,0 };
					theoDoiConTro = true;
					veBang();
					veTrangThai(1, 0, 0);
				}
				// trang kỉ lục
				else if (SIndex == 2) {
					taiDiemGame();
					taiThoiGianchoi();
					taiThoiGianchoi4();
					taiThoiGianchoi5();
					Diem();
				}
				// Irang thông tin về game
				else if (SIndex == 3) {
					deleteRow(3, 30);
					thongtin();
				}
				else {
					deleteRow(0, 30);
					TextColor(14);
					gotoxy(ConsoleWidth / 2 - 15, 15); printf("Cam on ban da choi tro choi cua toi !!");
					setBackgroundColor(0);
					std::cout << std::endl; std::cout << std::endl; std::cout << std::endl; std::cout << std::endl;
					Sleep(2000);
					exit(0);
				}
				break;
			case 2: // mức độ
				// dễ
				ctimer = 0;
				if (SIndex == 0) {
					capDo = 1; check++;
					SPage = 3;
					deleteRow(4, 50);
					khoitao(9, 9, 10); // tạo map 9x9, 10 bom
				}
				// thường
				else if (SIndex == 1) {
					capDo = 2; check++;
					SPage = 3;
					deleteRow(4, 50);
					khoitao(16, 16, 40); //tạo map 16x16, 40 bom
				}
				// khó
				else if (SIndex == 2) {
					capDo = 3; check++;
					SPage = 3;
					deleteRow(4, 50);
					khoitao(24, 24, 99); //tạo map 24x24, 99 bom
				}
				// thoát
				else {
					SPage = 1;
					SIndex = 0;
					deleteRow(4, 10);
					veMenu(0);
				}
				break;
			case 4: // Lose page
				if (SIndex) {
					SPage = 1;
					deleteRow(3, ConsoleHeight - 3);
					SIndex = 0;
					veMenu(0);
				}
				else {
					SPage = 3; ctimer = 0;
					deleteRow(3, ConsoleHeight - 3);
					khoitao(CTBang.SDong, CTBang.SCot, CTBang.SSoBom);
				}
				break;
			case 5: // thắng
				if (SIndex) {
					SPage = 1; SIndex = 0;
					deleteRow(3, ConsoleHeight - 3);
					veMenu(0);
				}
				else {
					deleteRow(3, 40);
					TextColor(11);
					if (check == 1) std::cin.ignore();
					if (capDo == 1) {
						taiThoiGianchoi();
						if (cctime < ctimer3) {
							gotoxy(ConsoleWidth / 2 - 40, 12); printf("ki luc moi :) ban lam tot lam");
							gotoxy(ConsoleWidth / 2 - 20, 14); printf("nhap ten cua ban: ");
							TextColor(3);
							std::string temp;
							std::getline(std::cin, temp);
							nameDe = temp;
							if (nameDe != "") {
								SPage = 1; SIndex = 0;
								deleteRow(3, ConsoleHeight - 3);
								veMenu(0);
							}
							luuThoiGianChoi();
						}
						else {
							gotoxy(ConsoleWidth / 2 - 35, 14); printf("chua pha ki luc");
							gotoxy(ConsoleWidth / 2 - 20, 16); printf("Co len ban se lam duoc ma");
							Sleep(6000);
							SPage = 1; SIndex = 0;
							deleteRow(3, ConsoleHeight - 3);
							veMenu(0);
						}

					}
					else if (capDo == 2) {
						taiThoiGianchoi4();
						if (cctime < ctimer4) {
							gotoxy(ConsoleWidth / 2 - 40, 12); printf("ki luc moi :) ban lam tot lam");
							gotoxy(ConsoleWidth / 2 - 20, 14); printf("nhap ten cua ban: ");
							TextColor(3);
							std::string temp;
							std::getline(std::cin, temp);
							nameThuong = temp;
							if (nameThuong != "") {
								SPage = 1; SIndex = 0;
								deleteRow(3, ConsoleHeight - 3);
								veMenu(0);
							}
							luuThoiGianChoi4();
						}
						else {
							gotoxy(ConsoleWidth / 2 - 35, 14); printf("chua pha ki luc");
							gotoxy(ConsoleWidth / 2 - 20, 16); printf("Co len");
							Sleep(6000);
							SPage = 1; SIndex = 0;
							deleteRow(3, ConsoleHeight - 3);
							veMenu(0);
						}

					}
					else if (capDo == 3) {
						taiThoiGianchoi5();
						if (cctime < ctimer5) {
							gotoxy(ConsoleWidth / 2 - 40, 12); printf("ki luc moi :) ban lam tot lam");
							gotoxy(ConsoleWidth / 2 - 20, 14); printf("nhap ten cua ban: ");
							TextColor(3);
							/*if (check == 1) std::cin.ignore();*/
							std::string temp;
							std::getline(std::cin, temp);
							nameKho = temp;
							if (nameKho != "") {
								SPage = 1; SIndex = 0;
								deleteRow(3, ConsoleHeight - 3);
								veMenu(0);
							}
							luuThoiGianChoi5();
						}
						else {
							gotoxy(ConsoleWidth / 2 - 35, 14); printf("chua pha ki luc");
							gotoxy(ConsoleWidth / 2 - 20, 16); printf("Co len");
							Sleep(6000);
							SPage = 1; SIndex = 0;
							deleteRow(3, ConsoleHeight - 3);
							veMenu(0);
						}

					}
					luuDiemGame();
				}
				break;
			case 6: // Lưu trang trò chơi (nhấn ESC khi muốn chơi tiếp)
				if (SIndex) {
					luuGame();
					SPage = 1; SIndex = 0;
					deleteRow(3, ConsoleHeight - 3);
					veMenu(0);
				}
				else {
					luuGame();
					SPage = 1; SIndex = 0;
					deleteRow(3, ConsoleHeight - 3);
					veMenu(0);
				}
				break;
			}
			break;
			// The ESC key
		case VK_ESCAPE:
			switch (SPage) {
			case 1: // nenu chính
				deleteRow(0, 30);
				TextColor(14);
				gotoxy(ConsoleWidth / 2 - 15, 15); printf("Cam on ban da choi tro choi cua toi !!");
				setBackgroundColor(0);
				std::cout << std::endl; std::cout << std::endl; std::cout << std::endl; std::cout << std::endl;
				Sleep(2000);
				exit(0);
				break;
			case 2: // Level 
				SPage = 1;
				deleteRow(4, 10);
				veMenu(0);
				break;
			case 3: // trang chơi
				SIndex = 0;
				SPage = 6; // trang lưu tên
				theoDoiConTro = false;
				veTrangThai(1, 1, 0);
				break;
			case 4: // thua
				SPage = 2;
				deleteRow(3, ConsoleHeight - 3);
				veCapDoChoi(0);
				break;
			case 5: // thắng
				SPage = 1; SIndex = 0;
				deleteRow(3, ConsoleHeight - 3);
				veMenu(0);
			case 7: // thông tin
				SPage = 1; SIndex = 0;
				deleteRow(3, 40);
				veMenu(0);
				break;
			case 8: // diểm cao nhất
				SPage = 1; SIndex = 0;
				deleteRow(3, 40);
				veMenu(0);
				break;
			}
			break;
			// mở ô (nhấn Z)
		case Left_Click:
			LeftClick(CViTriConTro.Y, CViTriConTro.X);
			break;
			// cắm cờ (nhấn X)
		case Right_Click:
			RightClick(CViTriConTro.Y, CViTriConTro.X);
			break;
		}
	}
}
// xử lý đầu vào các phím
void xuLySuKien() {
	while (1) {
		DWORD DWNumberOfEvents = 0; // lưu các sự kiện
		//DWord : kiểu số nguyên không dấu 32 bit
		DWORD DWNumberOfEventsRead = 0; // Lưu số lượng sự kiện đã được lọc
		// dùng hàm GetNumberOfConsoleInputEvents để gán những cái sự kiện đã được lọc cho 2 giá trị trên
		HANDLE HConsoleInput = GetStdHandle(STD_INPUT_HANDLE); // thiết bị đầu vào
		//HANDLE : handle của 1 đối tượng
		GetNumberOfConsoleInputEvents(HConsoleInput, &DWNumberOfEvents); // đặt sự kiện đầu vào của giao diện cho biến DWNumberOfEvents
		if (DWNumberOfEvents) {
			INPUT_RECORD* IREventBuffer = new INPUT_RECORD[DWNumberOfEvents]; // khai báo con trỏ EvenBuffer
			ReadConsoleInput(HConsoleInput, IREventBuffer, DWNumberOfEvents, &DWNumberOfEventsRead); // đặt các sự kiện được lưu vào con trỏ EvenBuffer
			// đọc các sự kiện
			for (DWORD i = 0; i < DWNumberOfEvents; i++) {
				// Key handle
				if (IREventBuffer[i].EventType == KEY_EVENT) {
					xuLyPhim(IREventBuffer[i].Event.KeyEvent);
				}
			}
		}

		if (SPage != 3) {
			total_time = clock();
		}
		if (SPage == 3) {
			Time(total_time);
		}
	}
}
// ------------------------------------------------------------------------thiết-kế-map-----------------------------------------------------------------------------
void thietKe() {
	short i;
	setColorCell(0, 0, 15, 0, "------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	setColor(7);
	for (i = 0; i < ConsoleWidth; i++) {
		printf("%c", 45);
	}
}
// Design the status
void veTrangThai(short trangthai, short SMode, short SIndex) {
	SIndex = SIndex;
	SAll = 2;
	setColorBGTextXY(0, 3, 15, 0, "Map: %d & %d", CTBang.SDong, CTBang.SCot);
	setColorBGTextXY(0, 4, 15, 0, "So bom: %d", CTBang.SSoBom - CTBang.SSoCo);
	if (SMode == 1) {
		std::string SMode = "  luu  ";
		setColorBGTextXY((ConsoleWidth / 2) - (SMode.length() / 2), 4, 15, ((SIndex == 0) ? 2 : 0), "  luu  ");
	}
	if (SMode == 2) {
		std::string SWin = "  luu ten  ";
		setColorBGTextXY((ConsoleWidth / 2) - (SWin.length() / 2), 4, 15, ((SIndex == 0) ? 2 : 0), "  luu ten  ");
	}
	if (SMode == 3) {
		std::string SLose = "  choi lai  ";
		setColorBGTextXY((ConsoleWidth / 2) - (SLose.length() / 2), 4, 15, ((SIndex == 0) ? 2 : 0), "  choi lai  ");
	}
	if (SMode >= 1) {
		std::string SExit = "  thoat  ";
		setColorBGTextXY((ConsoleWidth / 2) - (SExit.length() / 2), 5, 15, ((SIndex == 1) ? 2 : 0), "  thoat  ");
	}
	// trạng thái khi chơi + thời gian chơi
	// đang chơi
	if (trangthai == 1) {
		setColorBGTextXY(ConsoleWidth - 16, 4, 15, 0, "trang thai: %s", "Dang choi");
		gotoxy(0, 5); std::cout << "Thoi gian: ";
	}
	// thắng
	if (trangthai == 2) {
		setColorBGTextXY(ConsoleWidth - 16, 4, 15, 0, "trang thai: %s", "thang");
		gotoxy(0, 5); std::cout << "Thoi gian: ";
	}
	// thua
	if (trangthai == 3) {
		setColorBGTextXY(ConsoleWidth - 16, 4, 15, 0, "trang thai: %s", "thua");
		gotoxy(0, 5); std::cout << "Thoi gian: ";
	}
	std::cout << std::endl;
	short i;
	for (i = 0; i < ConsoleWidth; i++) {
		printf("%c", 45);
	}

	if (SPage == 3) {
		gotoxy(1, 15); TextColor(2); std::cout << "Z  : mo o";
		gotoxy(1, 16); TextColor(2); std::cout << "X  : cam co/huy cam co";
		gotoxy(1, 17); TextColor(2); std::cout << "ESC: thoat and luu";
		gotoxy(137, 15); TextColor(2); std::cout << "UP KEY   : len";
		gotoxy(137, 16); TextColor(2); std::cout << "DOWN KEY : xuong";
		gotoxy(137, 17); TextColor(2); std::cout << "LEFT KEY : trai";
		gotoxy(137, 18); TextColor(2); std::cout << "RIGHT KEY: phai";
	}
}
// vẽ menu chính
void veMenu(short SIndex) {
	// cập nhập
	SIndex = SIndex;
	SAll = 5;
	for (int i = 0; i < 40; i++)//vẽ hàng nhiều màu
	{
		if (i % 5 == 0) {
			TextColor(9); gotoxy(55 + i, 5);     printf("%c", 220);
		}
		else if (i % 5 == 1) {
			TextColor(10);  gotoxy(55 + i, 5);     printf("%c", 220);
		}
		else if (i % 5 == 2) {
			TextColor(11);  gotoxy(55 + i, 5);   printf("%c", 220);
		}
		else if (i % 5 == 3) {
			TextColor(12);   gotoxy(55 + i, 5);    printf("%c", 220);
		}
		else if (i % 5 == 4) {
			TextColor(13);   gotoxy(55 + i, 5);    printf("%c", 220);
		}
	}
	for (int i = 0; i < 40; i++)
	{
		if (i % 5 == 0) {
			TextColor(9); gotoxy(55 + i, 13);     printf("%c", 220);
		}
		else if (i % 5 == 1) {
			TextColor(10);  gotoxy(55 + i, 13);    printf("%c", 220);
		}
		else if (i % 5 == 2) {
			TextColor(11);  gotoxy(55 + i, 13);    printf("%c", 220);
		}
		else if (i % 5 == 3) {
			TextColor(12);   gotoxy(55 + i, 13);   printf("%c", 220);
		}
		else if (i % 5 == 4) {
			TextColor(13);   gotoxy(55 + i, 13);    printf("%c", 220);
		}
	}
	// vẽ Menu
	std::string STGamemoi = "  game moi  ";
	setColorBGTextXY((ConsoleWidth / 2) - (STGamemoi.length() / 2) - 2, 7, 15, ((SIndex == 0) ? 1 : 0), "  game moi  ");

	std::string STtieptuc = "  tiep tuc  ";
	setColorBGTextXY((ConsoleWidth / 2) - (STtieptuc.length() / 2) - 2, 8, 15, ((SIndex == 1) ? 1 : 0), "  tiep tuc  ");

	std::string STDiem = "  diem cao nhat  ";
	setColorBGTextXY((ConsoleWidth / 2) - (STDiem.length() / 2) - 2, 9, 15, ((SIndex == 2) ? 1 : 0), "  diem cao nhat  ");

	std::string STThongtin = "  thong tin  ";
	setColorBGTextXY((ConsoleWidth / 2) - (STThongtin.length() / 2) - 2, 10, 15, ((SIndex == 3) ? 1 : 0), "  thong tin  ");

	std::string STThoat = "  thoat  ";
	setColorBGTextXY((ConsoleWidth / 2) - (STThoat.length() / 2) - 2, 11, 15, ((SIndex == 4) ? 1 : 0), "  thoat  ");
}
// vẽ menu các cấp độ game
void veCapDoChoi(short SIndex) {
	// cập nhập
	SIndex = SIndex;
	SAll = 4;

	std::string STLevel = "  chon level  ";
	setColorBGTextXY((ConsoleWidth / 2) - (STLevel.length() / 2), 4, 15, 2, "  chon level  ");

	// vẽ menu
	std::string STDe = "  de  ";
	setColorBGTextXY((ConsoleWidth / 2) - (STDe.length() / 2), 7, 15, ((SIndex == 0) ? 1 : 0), "  de ");

	std::string STThuong = "  thuong   ";
	setColorBGTextXY((ConsoleWidth / 2) - (STThuong.length() / 2), 8, 15, ((SIndex == 1) ? 1 : 0), "  thuong  ");

	std::string STKho = "  kho   ";
	setColorBGTextXY((ConsoleWidth / 2) - (STKho.length() / 2), 9, 15, ((SIndex == 2) ? 1 : 0), "  kho   ");

	std::string STQuayLai = "   quay lai  ";
	setColorBGTextXY((ConsoleWidth / 2) - (STQuayLai.length() / 2) - 1, 10, 15, ((SIndex == 3) ? 1 : 0), "   quay lai  ");
}
// xác nhận rằng bạn muốn chơi
void XacNhan() {
	using namespace std;

	string SCheck = "     hay nhap YES de choi";
	setColorBGTextXY((ConsoleWidth / 2) - (SCheck.length() / 2) - 20, 3, 9, 0, "     hay nhap YES de choi ");
	cout << endl;

}
// vẽ thông báo
void veThongBao() {
	using namespace std;

	string SNotice = "**vao trang thong tin de xem huong dan va cach choi**";
	setColorBGTextXY((ConsoleWidth / 2) - (SNotice.length() / 2), 2, 12, 0, "**vao trang thong tin de xem huong dan va cach choi**");
	cout << endl;
	cout << endl;
	string SOk = " hay nhap OK de choi ";
	setColorBGTextXY((ConsoleWidth / 2) - (SOk.length() / 2) - 6, 9, 10, 0, " hay nhap OK de choi ");
	cout << endl;
	string SCheckok = "nhap OK: ";
	setColorBGTextXY((ConsoleWidth / 2) - (SCheckok.length() / 2) - 5, 15, 14, 0, "  nhap OK:  ");

}
// vẽ hướng dẫn và thông tin cần thiết
void thongtin() {
	SPage = 7;
	using namespace std;

	setColorBGTextXY(0, 3, 6, 0, "---------------------------------------------------------------------thong-tin------------------------------------------------------------------------------");
	cout << endl;

	setColorBGTextXY(1, 5, 3, 0, "- xin chao nhom toi gom 2 nguoi la Le Xuan Loc va Bui Duc Toan, dang hoc o lop ET1-08 tai dai hoc bach khoa ha noi ");
	cout << endl;

	setColorBGTextXY(1, 6, 3, 0, " hien tai chung toi la sinh vien nam 1 tai dai hoc bach khoa ha noi");
	cout << endl;

	setColorBGTextXY(1, 7, 3, 0, "day la tua game dau tien cua nhom va dang con nhieu khuyet diem mong cac ban thong cam");
	cout << endl;

	setColorBGTextXY(0, 12, 6, 0, "---------------------------------------------------------------------huong-dan-choi-----------------------------------------------------------------------------");
	cout << endl;

	setColorBGTextXY(1, 15, 11, 0, "nhan UP, DOWN, LEFT, RIGHT de di chuyen");
	cout << endl;
	setColorBGTextXY(1, 16, 11, 0, "nhan Z de mo o, X de dat co");
	cout << endl;
	setColorBGTextXY(1, 17, 11, 0, "neu da dat co thi ban khong the mo o, hay nhan X de xoa co truoc sau do roi mo o");
	cout << endl;
	setColorBGTextXY(1, 18, 11, 0, "khi mo o, thay so 1 nghia la 8 o xung quanh co 1 trai bom và tuong tu voi cac so khac");
	cout << endl;
	setColorBGTextXY(1, 19, 11, 0, "neu chac chan o nao co bom hay cam co de danh dau");
	cout << endl;
	setColorBGTextXY(1, 20, 11, 0, "neu mo phai o co bom nguoi choi se thua cuoc, de chien thang can mo het tat ca cac o khong chua bom, chung ta co the dung co de danh dau nhung o ta nghi ngo neu co bom");
	cout << endl;
	setColorBGTextXY(1, 22, 11, 0, "de thoat va luu hay nhan phim ESC");
	cout << endl;

	setColorBGTextXY(0, 30, 6, 0, "----------------------------------------------------------------------HET--------------------------------------------------------------------------------");
	cout << endl;
	setColorBGTextXY(14, 36, 14, 0, "***------------------------cam on da doc, nhan ESC de thoat, chuc ban choi game vui ve----------------------***");
	cout << endl;
	SIndex = 0;
}

// chức năng khác--------------------------------------------------------------------------------------------------------------------------------------------------
void TextColor(int x)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, x);
}
void gotoxy(int x, int y)//x,y là tọa điểm x,y trên màn hình
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD a = { x, y };    SetConsoleCursorPosition(h, a);
}
void Time(int total_time)//tính thời gian chơi game
{
	int time = (clock() - total_time + ctimer); cctime = time; time /= 1000;
	int gio, phut, giay;
	gio = time / 3600; phut = (time / 60) % 60, giay = time % 60;
	gotoxy(6, 5); TextColor(15);
	if (giay < 10 && gio < 10 && phut < 10) printf("0%d:0%d:0%d", gio, phut, giay);
	else if (giay < 10 && gio < 10) printf("0%d:%d:0%d", gio, phut, giay);
	else if (phut < 10 && gio < 10) printf("0%d:0%d:%d", gio, phut, giay);
	else if (giay < 10 && phut < 10) printf("%d:%d:0%d", gio, phut, giay);
	else if (giay < 10) printf("%d:%d:0%d", gio, phut, giay);
	else if (gio < 10) printf("0%d:%d:%d", gio, phut, giay);
	else if (phut < 10)printf("%d:0%d:%d", gio, phut, giay);
}
void Time3()
{
	int time = ctimer3; time /= 1000;
	int gio, phut, giay;
	gio = time / 3600; phut = (time / 60) % 60, giay = time % 60;

	if (giay < 10 && gio < 10 && phut < 10) printf("0%d:0%d:0%d", gio, phut, giay);
	else if (giay < 10 && gio < 10) printf("0%d:%d:0%d", gio, phut, giay);
	else if (phut < 10 && gio < 10) printf("0%d:0%d:%d", gio, phut, giay);
	else if (giay < 10 && phut < 10) printf("%d:%d:0%d", gio, phut, giay);
	else if (giay < 10) printf("%d:%d:0%d", gio, phut, giay);
	else if (gio < 10) printf("0%d:%d:%d", gio, phut, giay);
	else if (phut < 10)printf("%d:0%d:%d", gio, phut, giay);
}
void Time4()
{
	int time = ctimer4; time /= 1000;
	int gio, phut, giay;
	gio = time / 3600; phut = (time / 60) % 60, giay = time % 60;

	if (giay < 10 && gio < 10 && phut < 10) printf("0%d:0%d:0%d", gio, phut, giay);
	else if (giay < 10 && gio < 10) printf("0%d:%d:0%d", gio, phut, giay);
	else if (phut < 10 && gio < 10) printf("0%d:0%d:%d", gio, phut, giay);
	else if (giay < 10 && phut < 10) printf("%d:%d:0%d", gio, phut, giay);
	else if (giay < 10) printf("%d:%d:0%d", gio, phut, giay);
	else if (gio < 10) printf("0%d:%d:%d", gio, phut, giay);
	else if (phut < 10)printf("%d:0%d:%d", gio, phut, giay);
}
void Time5()
{
	int time = ctimer5; time /= 1000;
	int gio, phut, giay;
	gio = time / 3600; phut = (time / 60) % 60, giay = time % 60;

	if (giay < 10 && gio < 10 && phut < 10) printf("0%d:0%d:0%d", gio, phut, giay);
	else if (giay < 10 && gio < 10) printf("0%d:%d:0%d", gio, phut, giay);
	else if (phut < 10 && gio < 10) printf("0%d:0%d:%d", gio, phut, giay);
	else if (giay < 10 && phut < 10) printf("%d:%d:0%d", gio, phut, giay);
	else if (giay < 10) printf("%d:%d:0%d", gio, phut, giay);
	else if (gio < 10) printf("0%d:%d:%d", gio, phut, giay);
	else if (phut < 10)printf("%d:0%d:%d", gio, phut, giay);
}

// lưu game --------------------------------------------------------------------------------------------------------------------------------------------
void luuGame() {
	for (int i = 0; i < CTBang.SCot; i++) {
		for (int j = 0; j < CTBang.SDong; j++) {
			CCTO[i][j] = CTO[i][j];
		}
	}
	std::ofstream outFile("K.txt", std::ios::binary);
	if (outFile.is_open()) {
		outFile.write(reinterpret_cast<const char*>(CCTO), sizeof(CCTO));
		outFile.close();
	}
	else {
		// xử lý tệp lỗi
	}
	std::ofstream outFile1("K1.txt", std::ios::binary);
	if (outFile1.is_open()) {
		outFile1.write(reinterpret_cast<const char*>(&CTBang), sizeof(CTBang));
		outFile1.close();
	}
	else {
		// xử lý tệp lỗi
	}
	std::ofstream outFile2("K2.txt", std::ios::binary);
	if (outFile2.is_open()) {
		outFile2.write(reinterpret_cast<const char*>(&cctime), sizeof(cctime));
		outFile2.close();
	}
	else {
		// xử lý tệp lỗi
	}
	std::ofstream outFile3("K3.txt", std::ios::binary);
	if (outFile3.is_open()) {
		outFile3.write(reinterpret_cast<const char*>(&capDo), sizeof(capDo));
		outFile3.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
void taiGame() {
	std::ifstream inFile1("K1.txt", std::ios::binary);
	if (inFile1.is_open()) {
		inFile1.read(reinterpret_cast<char*>(&CTBang), sizeof(CTBang));
		inFile1.close();
	}
	else {
		// xử lý tệp lỗi
	}
	std::ifstream inFile("K.txt", std::ios::binary);
	if (inFile.is_open()) {
		inFile.read(reinterpret_cast<char*>(&CCTO), sizeof(CCTO));
		inFile.close();
	}
	else {
		// xử lý tệp lỗi
	}
	std::ifstream inFile2("K2.txt", std::ios::binary);
	if (inFile2.is_open()) {
		inFile2.read(reinterpret_cast<char*>(&ctimer), sizeof(ctimer));
		inFile2.close();
	}
	else {
		// xử lý tệp lỗi
	}
	taoMang2chieuDong();
	luuBang();
	for (int i = 0; i < CTBang.SCot; i++) {
		for (int j = 0; j < CTBang.SDong; j++) {
			CTO[i][j] = CCTO[i][j];
		}
	}
	std::ifstream inFile3("K3.txt", std::ios::binary);
	if (inFile3.is_open()) {
		inFile3.read(reinterpret_cast<char*>(&capDo), sizeof(capDo));
		inFile3.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
//Điểm ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void luuDiemGame() {
	std::ofstream outFileDiem("Diem.txt", std::ios::binary | std::ios::trunc);
	if (outFileDiem.is_open()) {
		outFileDiem << nameDe;
		outFileDiem.close();
	}
	else {
		// xử lý tệp lỗi
	}

	std::ofstream ngoaiFileDiem1("Diem1.txt", std::ios::binary | std::ios::trunc);
	if (ngoaiFileDiem1.is_open()) {
		ngoaiFileDiem1 << nameThuong;
		ngoaiFileDiem1.close();
	}
	else {
		// xử lý tệp lỗi
	}

	std::ofstream ngoaiFileDiem2("Diem2.txt", std::ios::binary | std::ios::trunc);
	if (ngoaiFileDiem2.is_open()) {
		ngoaiFileDiem2 << nameKho;
		ngoaiFileDiem2.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
void taiDiemGame() {
	std::ifstream trongFileDiem("Diem.txt", std::ios::binary);
	if (trongFileDiem.is_open()) {

		// Đọc dữ liệu chuỗi
		std::getline(trongFileDiem, nameDe);

		trongFileDiem.close();
	}
	else {
		// xử lý tệp lỗi
	}

	std::ifstream trongFileDiem1("Diem1.txt", std::ios::binary);
	if (trongFileDiem1.is_open()) {

		// Đọc dữ liệu chuỗi
		std::getline(trongFileDiem1, nameThuong);

		trongFileDiem1.close();
	}
	else {
		// xử lý tệp lỗi
	}

	std::ifstream trongFileDiem2("Diem2.txt", std::ios::binary);
	if (trongFileDiem2.is_open()) {

		// Đọc dữ liệu chuỗi
		std::getline(trongFileDiem2, nameKho);

		trongFileDiem2.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
// lưu thời gian cho cấp độ dễ
void luuThoiGianChoi() {
	std::ofstream ngoaiFileDiem3("Diem3.txt", std::ios::binary | std::ios::trunc);
	if (ngoaiFileDiem3.is_open()) {
		ngoaiFileDiem3 << cctime;
		ngoaiFileDiem3.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
void taiThoiGianchoi() {
	std::ifstream trongFileDiem3("Diem3.txt", std::ios::binary);
	if (trongFileDiem3.is_open()) {

		// Đọc dữ liệu chuỗi
		trongFileDiem3 >> ctimer3;

		trongFileDiem3.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
// lưu thời gian cho cấp độ thường
void luuThoiGianChoi4() {
	std::ofstream ngoaiFileDiem4("Diem4.txt", std::ios::binary | std::ios::trunc);
	if (ngoaiFileDiem4.is_open()) {
		ngoaiFileDiem4 << cctime;
		ngoaiFileDiem4.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
void taiThoiGianchoi4() {
	std::ifstream trongFileDiem4("Diem4.txt", std::ios::binary);
	if (trongFileDiem4.is_open()) {

		// Đọc dữ liệu chuỗi
		trongFileDiem4 >> ctimer4;
		trongFileDiem4.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
// lưu thời gian cho cấp độ khó
void luuThoiGianChoi5() {
	std::ofstream ngoaiFileDiem5("Diem5.txt", std::ios::binary | std::ios::trunc);
	if (ngoaiFileDiem5.is_open()) {
		ngoaiFileDiem5 << cctime;
		ngoaiFileDiem5.close();
	}
	else {
		// xử lý tệp lỗi
	}
}
void taiThoiGianchoi5() {
	std::ifstream trongFileDiem5("Diem5.txt", std::ios::binary);
	if (trongFileDiem5.is_open()) {

		// Đọc dữ liệu chuỗi
		trongFileDiem5 >> ctimer5;

		trongFileDiem5.close();
	}
	else {
	}
}
//đưa ra điểm cao nhất
void Diem()// cập nhập điểm cao nhất
{
	SPage = 8;
	deleteRow(3, 30);
	gotoxy(ConsoleWidth / 2 - 7, 3); TextColor(160); printf(" diem cao nhat ");
	TextColor(0);
	for (int i = 0; i < 45; i++)
	{
		gotoxy(18 + i, 11); printf(" ");
	}
	for (int i = 0; i < 45; i++)
	{
		gotoxy(18 + i, 21); printf(" ");
	}
	gotoxy(ConsoleWidth / 2 - 5.01, 6); TextColor(158); printf("thoi gian nhanh nhat");
	gotoxy(25, 10); TextColor(11); printf("ten : ");       gotoxy(41, 10); TextColor(3); std::cout << nameDe;
	gotoxy(25, 11); TextColor(11); printf("level         : ");       gotoxy(41, 11); TextColor(3); std::cout << "de";
	gotoxy(25, 12); TextColor(11); printf("time        : ");		 gotoxy(41, 12); TextColor(3); Time3();

	gotoxy(25, 17); TextColor(11); printf("ten : ");       gotoxy(41, 17); TextColor(3); std::cout << nameThuong;
	gotoxy(25, 18); TextColor(11); printf("level         : ");       gotoxy(41, 18); TextColor(3); std::cout << "thuong";
	gotoxy(25, 19); TextColor(11); printf("time         : ");       gotoxy(41, 19); TextColor(3); Time4();

	gotoxy(25, 24); TextColor(11); printf("ten : ");       gotoxy(41, 24); TextColor(3); std::cout << nameKho;
	gotoxy(25, 25); TextColor(11); printf("level         : ");       gotoxy(41, 25); TextColor(3); std::cout << "kho";
	gotoxy(25, 26); TextColor(11); printf("time         : ");       gotoxy(41, 26); TextColor(3); Time5();

	setColorBGTextXY(14, 36, 14, 0, "*****Neu ban muon quay lai de choi tro choi cua toi, xin vui long nhan phim ESC! Cam on ban da doc! Hy vong ban choi tot!*****");
	std::cout << std::endl;

	TextColor(0);
}