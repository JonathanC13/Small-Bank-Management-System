/*
 * BankManagementSystemView.h
 *
 *  Created on: Apr 16, 2019
 *      Author: Jonathan
 */

#include "BankManagementSystemModel.h"

using namespace std;
#include <string>

#ifndef BANKMANAGEMENTSYSTEMVIEW_H_
#define BANKMANAGEMENTSYSTEMVIEW_H_

/* MENU
 * 0. Create customer
 * 1. View all customers
 * 2. Select customer
 * 	2.1. update selected customers info
 * 		2.1.1. selected customer options
 * 			2.1.1 print update options
 * 			2.1.1.1. update name
 * 			2.1.1.2. update PIN
 * 		2.1.2 view customer accounts
 * 		2.1.3. conduct customer internal transfer
 * 3. Remove customer
 * 4. View all accounts
 * 5. Edit account
 * 6. Remove an account
 * 7. Global account transfer
 * 8. Exit
 */

class BankManagementSystem_View {
public:
	BankManagementSystem_View();
	virtual ~BankManagementSystem_View();

	void printMenu();														// print Welcome and displays management options
	void printCustomerCreation_Nm();										// option 0: print customer creation instructions for Name
	void printCustomerCreation_PINtitle();
	void printCustomerCreation_PIN();										// option 0: print customer creation instructions for PIN
	void printCustomerCreation_PINErr();
	void printAllCustomer(std::vector<BankManagementSystem_Model::st_customer> customers);		// option 1: print all current customers
	void printSelectingCustomer();											// option 2: print screen that asks for customer name with ID, e.g. Jon21342
	void printSelectCustInvalid(int selectID);
	void printCustomerInfo(BankManagementSystem_Model::st_customer customer);//	2.1: Name, ID, data created, number of accounts, total number of transactions.
	void printSelectedCustOptions();										// 2.1.1. selected customer options
	void printUpdateCustomer(string name);									//		2.1.1:  print options for the current customer.
	void printModNameOption();												//			2.1.1.1 mod name msg
	void printModPINOption();												//			2.1.1.2 mod PIN msg
										// use printAllAccounts(...)			//		2.1.2: owner ID, Account ID, date created, total amount. transaction history later
	void printInterTransfers();												//		2.1.3: Instructions for internal transfers for customer accounts.
	void printInterTransfersFROM();
	void printInterTransfersTO();
	void printInterTransfersAMT();
	void printRemoveCustomer();												// Option 3: Remove customer entirely
	void printAllAccounts(std::vector<BankManagementSystem_Model::st_account> accounts);// Option 4: account owner, Account Number, date created, total amount
	void printAccountsInfo(BankManagementSystem_Model::st_account account);
	void printSelectingAccount();											// op 5
	void printSelectedAccOptions(int owner);
	void printOwnerChange();
	void printAmtChange();
	void printRemoveAcc();												// op 6
	void printSelectAccInvalid(int selectID);
	void printGlobTransfers();												// Option 7: global account transfer
	void printConfirmationMsg();											// For deleting customers or accounts

};


#endif /* BANKMANAGEMENTSYSTEMVIEW_H_ */
