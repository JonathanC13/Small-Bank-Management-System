/*
 * BankManagementSystemModel.h
 *
 *  Created on: Apr 16, 2019
 *      Author: Jonathan
 */

using namespace std;
#include <string>
#include <bits/stdc++.h>

#ifndef BANKMANAGEMENTSYSTEMMODEL_H_
#define BANKMANAGEMENTSYSTEMMODEL_H_

class BankManagementSystem_Model {

	// class variables
public:
	// new customer struct
	struct st_customer {
		int ID;
		string Name;
		int PIN;
		string dateCreated;
		int num_acc;
		int num_trans;
	};
	// pointer for array of customer structs
	//st_customer* pt_customers;
	vector<st_customer> pt_customers;
	int pt_customers_Size = 0;

	// accounts struct. owner ID, Account ID, date created, total amount
	struct st_account {
		int ownerID;
		int accountID;
		string dateCreated;
		double tot_amount;
	};
	// pointer for array of account structs
	//st_account* pt_accounts;
	vector<st_account> pt_accounts;
	int pt_accounts_Size = 0;


	// functions
public:
	BankManagementSystem_Model();
	virtual ~BankManagementSystem_Model();

	BankManagementSystem_Model::st_customer setNewCustomerInfo(string newCustNm, int newPIN);		//0
	vector<BankManagementSystem_Model::st_customer> getAllCustomers(string buf);	//1
	void setNumCustomers(int newSize);
	int getNumCustomers();

	BankManagementSystem_Model::st_customer selectCustomer_m(string retJSON);	//2
	void modName(BankManagementSystem_Model::st_customer customer, string newNm);
	void modPIN(BankManagementSystem_Model::st_customer customer, int newPIN);
	BankManagementSystem_Model::st_account* getCurrCustomerAccounts(int custID);

	BankManagementSystem_Model::st_customer removeCustomer_m(int selID);	//3

	vector<BankManagementSystem_Model::st_account> getAllAccounts(string buf); //4
	int getNumAccounts();
};

#endif /* BANKMANAGEMENTSYSTEMMODEL_H_ */
