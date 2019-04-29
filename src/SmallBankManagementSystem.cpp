//============================================================================
// Name        : SmallBankManagementSystem.cpp
// Author      : Jonathan Chan
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "BankManagementSystemView.h"
#include "BankManagementSystemModel.h"
#include "BankManagementSystemController.h"

using namespace std;

int main() {

	string arr_str[2];
	arr_str[0] = "abc";
	arr_str[1] = "bbb";

	//Create a view : to display bank system information on console
	BankManagementSystem_View BMS_view;

	BankManagementSystem_Model BMS_model;

	BankManagementSystem_Controller BMS_controller(BMS_view, BMS_model);

	return 0;
}
