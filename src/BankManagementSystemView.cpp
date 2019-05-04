/*
 * BankManagementSystemView.cpp
 *
 *  Created on: Apr 16, 2019
 *      Author: Jonathan
 */

#include "BankManagementSystemView.h"
#include "BankManagementSystemModel.h"


#include <iostream>
using namespace std;

BankManagementSystem_View::BankManagementSystem_View() {
	// TODO Auto-generated constructor stub
}

BankManagementSystem_View::~BankManagementSystem_View() {
	// TODO Auto-generated destructor stub
}

void BankManagementSystem_View::printConfirmationMsg(){
	cout << "> Are you sure you would like to delete? (1 = yes / 0 = no): ";
}

// <MENU>
void BankManagementSystem_View::printMenu()
{
	cout << "### MENU ###" << endl;
	cout << "0. Create customer" << endl;
	cout << "1. View all customers" << endl;
	cout << "2. Select customer" << endl;
	cout << "3. Remove customer" << endl;
	cout << "4. View all accounts" << endl;
	cout << "5. Edit an account" << endl;
	cout << "6. Remove an account" << endl;
	cout << "7. Global account transfer" << endl;
	cout << "8. Exit" << endl;
	//cout << "8. (test DB connection)" << endl;
}
// </MENU>

// <Option 0>
void BankManagementSystem_View::printCustomerCreation_Nm()
{
	cout << endl << "### Creating new customer ###" << endl;
	cout << "Creating customer name, (enter -1 to exit)" << endl;
	cout << "> Enter customer's name: ";
}

void BankManagementSystem_View::printCustomerCreation_PINtitle()
{
	cout << "Creating customer PIN, (enter -1 to exit)" << endl;
}

void BankManagementSystem_View::printCustomerCreation_PIN()
{

	cout << "> Enter desired customer's 4 digit PIN, (enter -1 to exit): ";
}

void BankManagementSystem_View::printCustomerCreation_PINErr()
{
	cout << "err: Customer PIN is required to be 4 digits." << endl;
}
// </Option 0>

// <Option 1>
// @param customers: list of customer struct, display all info except PIN
void BankManagementSystem_View::printAllCustomer(std::vector<BankManagementSystem_Model::st_customer> customers)
{
	cout << "### Displaying all current customers ###" << endl;
	for (std::vector<BankManagementSystem_Model::st_customer>::iterator iter = customers.begin(); iter != customers.end(); ++iter)
	{
		cout << "-----" << endl;
		cout << "Customer Name: " << iter.base()->Name << endl;
		cout << "Customer ID: " << iter.base()->ID << endl;
		cout << "Date created: " << iter.base()->dateCreated << endl;
		cout << "Number of accounts: " << iter.base()->num_acc << endl;
		cout << "Number of transactions to date: " << iter.base()->num_trans << endl;
		cout << "-----" << endl;

	}
}

// </Option 1>

// <Option 2>
void BankManagementSystem_View::printSelectingCustomer()
{
	cout << "### Selecting customer to view, (enter -1 to exit) ###" << endl;
	cout << "> Enter desired customer's ID (e.g. 0000): ";
}

void BankManagementSystem_View::printSelectCustInvalid(int selectID){
	cout << "Customer ID: " << selectID << " not found." << endl;
}

//Name, ID, data created, number of accounts, total number of transactions.. Give struct
void BankManagementSystem_View::printCustomerInfo(BankManagementSystem_Model::st_customer customer)
{
	cout << "### Displaying customer Info ###" << endl;

	if(customer.ID == -1){
		cout << "Desired customer was not found." << endl;
	} else {
		cout << "-----" << endl;
		cout << "Customer name: " << customer.Name << endl;
		cout << "Customer ID: " << customer.ID << endl;
		cout << "Customer PIN: hidden" << endl;
		cout << "Date created: " << customer.dateCreated << endl;
		cout << "Number of accounts: " << customer.num_acc << endl;
		cout << "Number of transactions to date: " << customer.num_trans << endl;
		cout << "-----" << endl;
	}
}

void BankManagementSystem_View::printSelectedCustOptions()
{
	cout << "1. Update customer info" << endl;
	cout << "2. Add an account." << endl;
	cout << "3. View all customer's accounts" << endl;
	cout << "4. Perform internal transfer" << endl;
	cout << "5. Exit" << endl;

	cout << "> Enter your choice: ";
}

void BankManagementSystem_View::printUpdateCustomer(string name)
{
	cout << "### Updating customer info ###" << endl;
	cout << "1. Modify name [currently: " << name << "]" << endl;
	cout << "2. Modify PIN" << endl;
	cout << "3. Save and Exit" << endl;

	cout << "> Enter your choice: ";
}

void BankManagementSystem_View::printModNameOption()
{
	cout << "> Enter desired new name (-1 to exit): ";
}

void BankManagementSystem_View::printModPINOption()
{
	cout << "> Enter desired new PIN (-1 to exit): ";
}

void BankManagementSystem_View::printAccAmt(){
	cout << "> Enter desired account amount (-1 to exit): ";
}

void BankManagementSystem_View::printInterTransfers()
{
	cout << "Internal customer account transfer , (enter -1 to exit)" << endl;
}
void BankManagementSystem_View::printInterTransfersFROM()
{
	cout << "> From account, (-1 to exit): ";
}

void BankManagementSystem_View::printInterTransfersTO()
{
	cout << "> To account, (-1 to exit): ";
}

void BankManagementSystem_View::printInterTransfersAMT()
{
	cout << "> Amount to transfer, (-1 to exit): ";
}

// </Option 2>

// <Option 3>
void BankManagementSystem_View::printRemoveCustomer()
{
	cout << "### Selecting customer to remove, (enter -1 to exit) ###" << endl;
	cout << "> Enter desired customer's ID (e.g. 0): ";
}
// </Option 3>

// <Option 4>
// need shared struct
void BankManagementSystem_View::printAllAccounts(std::vector<BankManagementSystem_Model::st_account> accounts)
{

	cout << "### Displaying all current accounts open ###" << endl;
	for (std::vector<BankManagementSystem_Model::st_account>::iterator iter = accounts.begin(); iter != accounts.end(); ++iter)
	{

		cout << "-----" << endl;
		cout << "Owner ID: " << iter.base()->ownerID << endl;
		cout << "Account ID: " << iter.base()->accountID << endl;
		cout << "Date created: " << iter.base()->dateCreated << endl;
		cout << "Total amount: " << iter.base()->tot_amount << endl;
		cout << "-----" << endl;

	}
}

void BankManagementSystem_View::printSelectAccInvalid(int selectID){
	cout << "Account ID: " << selectID << " not found." << endl;
}
// </Option 4>

// <Option 5>
void BankManagementSystem_View::printSelectingAccount()
{
	cout << "### Selecting account to view, (enter -1 to exit) ###" << endl;
	cout << "> Enter desired account's ID (e.g. 0): ";
}

// @params structs for each account, need shared structure (account owner, Account Number, date created, total amount) in model header file?
void BankManagementSystem_View::printAccountsInfo(BankManagementSystem_Model::st_account account)
{
	cout << "### Displaying account Info ###" << endl;

	if(account.ownerID == -1){
		cout << "Desired Account was not found." << endl;
	} else {
		cout << "-----" << endl;
		cout << "Account owner: " << account.ownerID << endl;
		cout << "Account ID: " << account.accountID << endl;
		cout << "Account date created: " << account.dateCreated << endl;
		cout << "Account amount: " << account.tot_amount << endl;

		cout << "-----" << endl;
	}
}

void BankManagementSystem_View::printSelectedAccOptions(int owner){
	cout << "### Updating account info ###" << endl;
	cout << "1. Change owner ID [currently: " << owner << "]" << endl;
	cout << "2. Change amount" << endl;
	cout << "3. Save and Exit" << endl;

	cout << "> Enter your choice: ";
}

void BankManagementSystem_View::printOwnerChange(){
	cout << "> Enter desired new owner (-1 to exit): ";
}

void BankManagementSystem_View::printAmtChange(){
	cout << "> Enter desired new amount (-1 to exit): ";
}

void BankManagementSystem_View::printRemoveAcc(){
	cout << "### Selecting account to remove, (enter -1 to exit) ###" << endl;
	cout << "> Enter desired account's ID (e.g. 0): ";
}
// </Option 5>

// <Option 7>
void BankManagementSystem_View::printGlobTransfers()
{
	cout << "Global customer account transfer , (enter -1 to exit)" << endl;
}

// </Option 6>
