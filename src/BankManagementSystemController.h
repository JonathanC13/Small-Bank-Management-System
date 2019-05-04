/*
 * BankManagementSystemController.h
 *
 *  Created on: Apr 16, 2019
 *      Author: Jonathan
 */

#include "BankManagementSystemView.h"
#include "BankManagementSystemModel.h"

#ifndef BANKMANAGEMENTSYSTEMCONTROLLER_H_
#define BANKMANAGEMENTSYSTEMCONTROLLER_H_

class BankManagementSystem_Controller {
public:
	//BankManagementSystem_Controller();
	BankManagementSystem_Controller(BankManagementSystem_View BMS_view, BankManagementSystem_Model BMS_model);
	virtual ~BankManagementSystem_Controller();

	void closeProgram();

	void createCustomer_contr(); //0
	int inputValidPIN();
	bool checkDigit(int n);

	void viewAllCustomers();	//1

	void selectCustomer();		//2
	int selectID_IN();
	void selectedCustomerOptions(BankManagementSystem_Model::st_customer sel_cust);
	void createAccount(BankManagementSystem_Model::st_customer sel_cust);
	BankManagementSystem_Model::st_customer updateCustomerInfoOptions(BankManagementSystem_Model::st_customer sel_cust);
	void currentCustomerAccounts(int custID);

	void removeCustomer();		//3

	void viewAllAccounts();		//4

	void editAccount();			//5
	BankManagementSystem_Model::st_account selectedAccountOptions(BankManagementSystem_Model::st_account selAcc);

	void removeAnAccount();		//6

	void transfer(int type, int custID);	//7
};

#endif /* BANKMANAGEMENTSYSTEMCONTROLLER_H_ */
