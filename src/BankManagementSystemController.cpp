/*
 * BankManagementSystemController.cpp
 *
 *  Created on: Apr 16, 2019
 *      Author: Jonathan
 */
#include <math.h>       /* floor */
#include <limits>
#include <iostream>
using namespace std;

#include "BankManagementSystemController.h"
#include "BankManagementSystemView.h"
#include "BankManagementSystemModel.h"
#include "BankManagementSystemUDPcontroller.h"



//BankManagementSystem_Controller::BankManagementSystem_Controller() {
	// TODO Auto-generated constructor stub
//}

BankManagementSystem_Controller::~BankManagementSystem_Controller() {
	// TODO Auto-generated destructor stub
}

BankManagementSystem_View view;
BankManagementSystem_Model model;
BankManagementSystemUDP_controller UDP_contr;

BankManagementSystem_Controller::BankManagementSystem_Controller(BankManagementSystem_View BMS_view, BankManagementSystem_Model BMS_model) {
	view = BMS_view;
	model = BMS_model;

	int exit = 0;

	while(exit == 0)
	{
		int menu_choice;
		view.printMenu();
		cout << "> Enter your choice: ";
		//cin.ignore(); //will flush the input stream
		cin >> menu_choice;
		if (cin.fail())
		{
			cout << "ERROR -- You did not enter an integer" << endl;

			// get rid of failure state
			cin.clear();

			// From Eric's answer (thanks Eric)
			// discard 'bad' character(s)
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		cout << endl;

		switch(menu_choice)
		{
			case 0:
				createCustomer_contr();
				break;
			case 1:
				viewAllCustomers();
				break;
			case 2:
				selectCustomer();	// in the add account option of selectCustomer it updates customer info when it can (customer's number of accounts)
				break;
			case 3:
				removeCustomer();	// removing a customer that has accounts will change the owner to value like -5 to indicate that all funds now belong to the bank
				break;
			case 4:
				viewAllAccounts();
				break;
			case 5:
				editAccount();
				break;
			case 6:
				removeAnAccount();	// updates customer info when it can (customer's number of accounts)
				break;
			case 7:
				transfer(1, 0);	// successful transfer increases the number of transfers of the affected customers
				break;
			case 8:
				exit = 1;
				closeProgram();
				cout << "Thank you, exiting ...";
				break;

			//case 9:
				//cout << "testing connection" << endl;
				//UDP_contr.testConn();
				//break;
			default:
				cout << "Please enter a value 0 to 8" << endl;
				break;
		}

		cout << endl;
	}

}

void BankManagementSystem_Controller::closeProgram(){
	BankManagementSystem_Model::st_customer placeholder_cust_end;
	BankManagementSystem_Model::st_account placeholder_acc_end;
	string retJSON_end = UDP_contr.createUDPPacket(50, placeholder_cust_end, placeholder_acc_end, 0, 0, 0);
}

// <op 0, create new customer>
// tests:
/*
 * 1 digit
 * enter
 * 1 letter
 * >4 digits
 * 12a0
 * 4 digits
 */
void BankManagementSystem_Controller::createCustomer_contr()
{
	view.printCustomerCreation_Nm();
	string createNm;
	cin >> createNm;
	if (cin.fail())
	{
		cout << "ERROR -- ";

		// get rid of failure state
		cin.clear();

		// From Eric's answer (thanks Eric)
		// discard 'bad' character(s)
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	}

	if(createNm == "-1")
	{
		return;
	}

	view.printCustomerCreation_PINtitle();
	int createPIN = inputValidPIN();

	BankManagementSystem_Model::st_customer createdCust = model.setNewCustomerInfo(createNm, createPIN);
	BankManagementSystem_Model::st_account placeholder_acc;
	// request to server
	// the passed createdCust is parsed into a packet
	string retJSON = UDP_contr.createUDPPacket(1, createdCust, placeholder_acc, 0, 0, 0);
	//cout << "ret JSON " << retJSON << endl;
	// model to parse the JSON
	std::vector<BankManagementSystem_Model::st_customer> viewingCustomers = model.getAllCustomers(retJSON);

	// view
	// 1. pass the array to view and it prints
	view.printAllCustomer(viewingCustomers);

}

int BankManagementSystem_Controller::inputValidPIN()
{
	int createPIN;
	while(1)
	{
		view.printCustomerCreation_PIN();

		cin >> createPIN;
		if (cin.fail())
		{
			cout << "ERROR -- You did not enter an integer" << endl;
			cout << "Enter a valid PIN: ";
			// get rid of failure state
			cin.clear();

			// From Eric's answer (thanks Eric)
			// discard 'bad' character(s)
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if(createPIN == -1)
		{
			break;
		}

		if(std::to_string(createPIN).length() != 4)
		{
			view.printCustomerCreation_PINErr();
			continue;
		} else {
			break;
		}

		// if 4 chars long, check if only numbers
		if(checkDigit(createPIN) == false)
		{
			view.printCustomerCreation_PINErr();
			continue;
		} else {
			break;
		}

	}

	return createPIN;
}
// </op 0>

bool BankManagementSystem_Controller::checkDigit(int n)
{
	int min_rg = 48;
	int max_rg = 57;

	 while (n != 0) {
		   int asciiVal = (int)(n % 10 + '0');
		   cout << asciiVal << endl;
		   if(asciiVal < min_rg || asciiVal > max_rg)
		   {
			   return false;
		   }
		   n = floor(n / 10);
	  }
	 return true;

}

// <op 1 View all customers>
void BankManagementSystem_Controller::viewAllCustomers()
{

	// 1. ask UDP controller to retrieve all info from DB
	// 2. return full json (string) to model.
	BankManagementSystem_Model::st_customer emptyCust;
	BankManagementSystem_Model::st_account placeholder_acc;
	emptyCust.ID = -2;
	string retJSON = UDP_contr.createUDPPacket(2, emptyCust, placeholder_acc, 0, 0, 0);
	// 3. model creates an array of st_customers from parsing the JSON
	std::vector<BankManagementSystem_Model::st_customer> viewingCustomers = model.getAllCustomers(retJSON);

	// view
	// 1. pass the array to view and it prints
	view.printAllCustomer(viewingCustomers);


}
// </op 1>

// <op 2 Select customer>
void BankManagementSystem_Controller::selectCustomer()
{
	view.printSelectingCustomer();
	BankManagementSystem_Model::st_account placeholder_acc;
	// input from user, the customer ID
	int selectID = selectID_IN();
	if(selectID != -1){
		BankManagementSystem_Model::st_customer sel_cust;
		sel_cust.ID = selectID;
		// request search of customer with the ID
		string retJSON = UDP_contr.createUDPPacket(3, sel_cust, placeholder_acc, 0, 0, 0);

		// parse the JSON
		std::vector<BankManagementSystem_Model::st_customer> viewingCustomers = model.getAllCustomers(retJSON);
		BankManagementSystem_Model::st_customer selectedCust = viewingCustomers[0];

		// check if found
		if(selectedCust.ID != -1)
		{
			// if ID not -1, then it was found in the DB and returned
			// options available when a customer is selected.
			selectedCustomerOptions(selectedCust);
		} else {
			view.printSelectCustInvalid(selectID);
		}
	}

}

//returns valid input
int BankManagementSystem_Controller::selectID_IN()
{
	int selectID;
	while(1)
	{

		// get user input
		cin >> selectID;
		if (cin.fail())
		{
			cout << "ERROR -- You did not enter an integer" << endl;
			cout << "Enter a valid value: ";
			// get rid of failure state
			cin.clear();

			// From Eric's answer (thanks Eric)
			// discard 'bad' character(s)
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		} else {
			break;
		}
	}
	return selectID;
}

void BankManagementSystem_Controller::selectedCustomerOptions(BankManagementSystem_Model::st_customer sel_cust)
{

	int selectOp = 0;
	//BankManagementSystem_Model::st_customer select_C = sel_cust;

	while(1)
	{
		view.printCustomerInfo(sel_cust);
		view.printSelectedCustOptions();


		// get user input
		cin >> selectOp;
		if (cin.fail())
		{
			cout << "ERROR -- You did not enter an integer" << endl;
			cout << "Enter a valid option: ";
			// get rid of failure state
			cin.clear();

			// From Eric's answer (thanks Eric)
			// discard 'bad' character(s)
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if(selectOp < 1 || selectOp > 5)
		{
			cout << "Please enter a value 0 to 5" << endl;
			continue;
		}

		if(selectOp == 5)
		{
			break;
		}

		switch(selectOp)
		{
			case 1:
				//1. update customer info
				sel_cust = updateCustomerInfoOptions(sel_cust);
				break;
			case 2:
				// 2. create an account for this customer
				createAccount(sel_cust);
				break;
			case 3:
				currentCustomerAccounts(sel_cust.ID);
				// view all accounts
				break;
			case 4:
				// perform internal transfer
				transfer(0, sel_cust.ID);
				break;
			default:
				cout << "Please enter a value 0 to 5" << endl;
				break;
		}
	}

}

// 2.1
BankManagementSystem_Model::st_customer BankManagementSystem_Controller::updateCustomerInfoOptions(BankManagementSystem_Model::st_customer sel_cust)
{
	int selectOp;
	string pickName;
	int pickPIN;
	BankManagementSystem_Model::st_customer sel_custcurr = sel_cust;
	BankManagementSystem_Model::st_account placeholder_acc;
	while (1)
	{
		view.printUpdateCustomer(sel_custcurr.Name);

		// get user input
		cin >> selectOp;
		if (cin.fail())
		{
			cout << "ERROR -- You did not enter an integer" << endl;
			cout << "Enter a valid option: ";
			// get rid of failure state
			cin.clear();

			// From Eric's answer (thanks Eric)
			// discard 'bad' character(s)
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if(selectOp < 1 || selectOp > 3)
		{
			cout << "Please enter a value 1 to 3" << endl;
			continue;
		}

		if(selectOp == 3)
		{
			// on exit commit to DB - even if no changes.

			// sel_cust is given to be parsed into a packet to update the selected customer
			string retJSON = UDP_contr.createUDPPacket(4, sel_custcurr, placeholder_acc, 0, 0, 0);
			//cout << "update: " << retJSON << endl;

			if(retJSON.compare(0,1,"1") == 0)
			{
				// rows affected = 1, therefore assume the desired customer row was updated.
				cout << "Customer updated!" << endl;

				// update current customer struct
				// select customer row
				string retJSON_sel = UDP_contr.createUDPPacket(3, sel_custcurr, placeholder_acc, 0, 0, 0);
				// parse the JSON
				std::vector<BankManagementSystem_Model::st_customer> viewingCustomers = model.getAllCustomers(retJSON_sel);
				sel_custcurr = viewingCustomers[0];
			}
			break;
		}

		switch(selectOp)
		{
			case 1:
				// modify current customer name
				while(1){
					view.printModNameOption();

					cin >> pickName;
					if (cin.fail())
					{
						cout << "ERROR -- name" << endl;
						cout << "Enter a valid input: ";
						// get rid of failure state
						cin.clear();

						// From Eric's answer (thanks Eric)
						// discard 'bad' character(s)
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						continue;
					} else {
						if(pickName == "-1"){
							break;
						}
					}
					break;
				}
				//model.modName(sel_custcurr, pickName);
				sel_custcurr.Name = pickName;
				break;
			case 2:
				// modify PIN

				pickPIN = inputValidPIN();
				//model.modPIN(sel_custcurr, pickPIN);
				sel_custcurr.PIN = pickPIN;
				break;

			default:
				cout << "Please enter a value 1 to 3" << endl;
				break;

		}

	}
	return sel_custcurr;
}

void BankManagementSystem_Controller::createAccount(BankManagementSystem_Model::st_customer sel_cust)
{
	double accAmt = 0;
	while(1){
		view.printAccAmt();
		cin >> accAmt;
		if (cin.fail())
		{
			cout << "ERROR --  Account amount" << endl;

			// get rid of failure state
			cin.clear();

			// From Eric's answer (thanks Eric)
			// discard 'bad' character(s)
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		if(accAmt == -1 ){
			return;
		}
		if(accAmt < 0){
			cout << "Please enter a positive value for amount." << endl;
			continue;
		} else {
			break;
		}
	}


	BankManagementSystem_Model::st_customer sel_custcurr;
	sel_custcurr.ID = sel_cust.ID;
	BankManagementSystem_Model::st_account new_acc = model.createAccount(accAmt);

	string retJSON = UDP_contr.createUDPPacket(10, sel_custcurr, new_acc, 0, 0, 0);

	// parse json
	std::vector<BankManagementSystem_Model::st_account> viewingAcc = model.getAllAccounts(retJSON);

	//cout << "NUM ACC RET = " << viewingAcc.size() << endl;
	if(viewingAcc.size() == 1){
		// change customer account count
		BankManagementSystem_Model::st_customer update_custcurr = sel_cust;
		update_custcurr.num_acc = update_custcurr.num_acc + 1;
		sel_cust.num_acc = sel_cust.num_acc + 1;

		// update
		string retJSON = UDP_contr.createUDPPacket(4, update_custcurr, new_acc, 0, 0, 0);
	} // else no account so dont update a customer or somehow too many accounts affected and returned

	// view
	// 1. pass the array to view and it prints
	view.printAllAccounts(viewingAcc);
}

void BankManagementSystem_Controller::currentCustomerAccounts(int custID)
{

	BankManagementSystem_Model::st_customer sel_custcurr;
	BankManagementSystem_Model::st_account placeholder_acc;
	sel_custcurr.ID = custID;
	string retJSON = UDP_contr.createUDPPacket(7, sel_custcurr, placeholder_acc, 0, 0, 0);
	// 3. model creates an array of st_customers from parsing the JSON
	std::vector<BankManagementSystem_Model::st_account> viewingAccounts = model.getAllAccounts(retJSON);

	// view
	// 1. pass the array to view and it prints
	view.printAllAccounts(viewingAccounts);

}

// </op 2>

// <op 3 Remove customer>
void BankManagementSystem_Controller::removeCustomer()
{
	view.printRemoveCustomer();

	// user input customer ID
	int selectID = selectID_IN();
	BankManagementSystem_Model::st_customer sel_custcurr;
	BankManagementSystem_Model::st_account placeholder_acc;
	sel_custcurr.ID = selectID;

	string retJSON = UDP_contr.createUDPPacket(3, sel_custcurr, placeholder_acc, 0, 0, 0);
	// parse the JSON
	std::vector<BankManagementSystem_Model::st_customer> viewingCustomers = model.getAllCustomers(retJSON);
	sel_custcurr = viewingCustomers[0];
	view.printCustomerInfo(sel_custcurr); // display the selected customer to be deleted

	if(sel_custcurr.ID != -1){
		// confirmation msg
		view.printConfirmationMsg();

		int confirm_del;
		while(1){
			cin >> confirm_del;
			if (cin.fail())
			{
				cout << "ERROR -- confirm_del" << endl;

				// get rid of failure state
				cin.clear();

				// From Eric's answer (thanks Eric)
				// discard 'bad' character(s)
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
			} else {
				if(confirm_del == 1){
					// delete row
					//req UDP to remove specified account
					string retJSON = UDP_contr.createUDPPacket(5, sel_custcurr, placeholder_acc, 0, 0, 0);

					if(retJSON.compare(0,1,"1") == 0)
					{
						cout << "Customer deleted!" << endl;

						//get all accounts that belong to this deleted customer and change the owner value to -5
						BankManagementSystem_Model::st_customer emptyCust;
						BankManagementSystem_Model::st_account placeholder_acc;
						placeholder_acc.accountID = -2;
						string retJSON_accnts = UDP_contr.createUDPPacket(6, emptyCust, placeholder_acc, 0, 0, 0);

						std::vector<BankManagementSystem_Model::st_account> viewingAccounts = model.getAllAccounts(retJSON_accnts);

						for (std::vector<BankManagementSystem_Model::st_account>::iterator iter = viewingAccounts.begin(); iter != viewingAccounts.end(); ++iter)
						{
							// if the selectID matches the id_owner of an account then update the owner to value "-5" to indicate funds default forfeited to the bank
							if (selectID == iter.base()->ownerID){
								placeholder_acc = (*iter);
								placeholder_acc.ownerID = -5;

								string retJSON_updAcc = UDP_contr.createUDPPacket(8, emptyCust, placeholder_acc, 0, 0, 0);
							}
						}

						break;

					} else {
						cout << "That customer ID does not exist, no customer deleted." << endl;
						break;
					}

				} else if(confirm_del == 0){
					break;
				} else {
					cout << "Please enter a valid option:" << endl;
				}
			}
		}
	} else {
		cout << "No customer deleted." << endl;
	}



}
// </op 3>

// <op 4 View all accounts>
void BankManagementSystem_Controller::viewAllAccounts()
{
	// 1. ask UDP controller to retrieve all info from DB
	// 2. return full json (string) to model.
	BankManagementSystem_Model::st_customer emptyCust;
	BankManagementSystem_Model::st_account placeholder_acc;
	placeholder_acc.accountID = -2;
	string retJSON = UDP_contr.createUDPPacket(6, emptyCust, placeholder_acc, 0, 0, 0);
	// 3. model creates an array of st_customers from parsing the JSON
	std::vector<BankManagementSystem_Model::st_account> viewingAccounts = model.getAllAccounts(retJSON);

	// view
	// 1. pass the array to view and it prints
	view.printAllAccounts(viewingAccounts);

}
// </op 4>

// <op 5 edit account>
void BankManagementSystem_Controller::editAccount()
{
	view.printSelectingAccount();
	BankManagementSystem_Model::st_account placeholder_acc;
	// input from user, the customer ID
	int selectID = selectID_IN();
	if(selectID != -1){
		BankManagementSystem_Model::st_customer null_cust;
		placeholder_acc.accountID = selectID;
		// request search of customer with the ID
		string retJSON = UDP_contr.createUDPPacket(6, null_cust, placeholder_acc, 0, 0, 0);

		// parse the JSON
		std::vector<BankManagementSystem_Model::st_account> viewingAccounts = model.getAllAccounts(retJSON);
		BankManagementSystem_Model::st_account selectedAcc = viewingAccounts[0];

		// check if found
		if(selectedAcc.accountID != -1)
		{
			// if ID not -1, then it was found in the DB and returned
			// options available when a customer is selected.
			selectedAccountOptions(selectedAcc);
		} else {
			cout << "Account with ID: " << selectID << " does not exist." << endl;

		}
	}

}

BankManagementSystem_Model::st_account BankManagementSystem_Controller::selectedAccountOptions(BankManagementSystem_Model::st_account selAcc){

	int sel_op = 0;
	BankManagementSystem_Model::st_account sel_acccurr = selAcc;
	BankManagementSystem_Model::st_customer null_cust;
	while(1)
	{
		view.printAccountsInfo(sel_acccurr);
		view.printSelectedAccOptions(sel_acccurr.ownerID);


		// get user input
		cin >> sel_op;
		if (cin.fail())
		{
			cout << "ERROR -- You did not enter an integer" << endl;
			cout << "Enter a valid option: ";
			// get rid of failure state
			cin.clear();

			// From Eric's answer (thanks Eric)
			// discard 'bad' character(s)
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}

		if(sel_op < 1 || sel_op > 3)
		{
			continue;
		}

		// save and exit
		if(sel_op == 3)
		{
			// update
			string retJSON = UDP_contr.createUDPPacket(8, null_cust, sel_acccurr, 0, 0, 0);
			break;
		}

		switch(sel_op)
		{
			case 1:
				int newOwner;
				//1. change owner
				while(1){
					view.printOwnerChange();
					cin >> newOwner;
					if (cin.fail())
					{
						cout << "ERROR -- newOwner" << endl;

						// get rid of failure state
						cin.clear();

						// From Eric's answer (thanks Eric)
						// discard 'bad' character(s)
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						continue;
					} else {
						break;
					}
				}
				if(newOwner == -1){
					break;
				} else {

					// check if valid new owner
					BankManagementSystem_Model::st_customer sel_cust;
					sel_cust.ID = newOwner;
					// request search of customer with the ID
					string retJSON = UDP_contr.createUDPPacket(3, sel_cust, sel_acccurr, 0, 0, 0);

					// parse the JSON
					std::vector<BankManagementSystem_Model::st_customer> viewingCustomers = model.getAllCustomers(retJSON);
					BankManagementSystem_Model::st_customer selectedCust = viewingCustomers[0];

					// check if found
					if(selectedCust.ID != -1)
					{
						// if ID not -1, then it was found in the DB and returned
						//set the new ownerID
						cout << "newOwner: " << newOwner << endl;
						sel_acccurr.ownerID = newOwner;

					} else {
						cout << "Account with ID: " << newOwner << " does not exist." << endl;
					}


					break;
				}
			case 2:
				//2. change amount
				double newAmt;
				while(1){
					view.printAmtChange();

					cin >> newAmt;
					if (cin.fail())
					{
						cout << "ERROR -- newAmt" << endl;
						cout << "Enter a valid value: ";
						// get rid of failure state
						cin.clear();

						// From Eric's answer (thanks Eric)
						// discard 'bad' character(s)
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
						break;
					} else {
						if(newAmt < -1){
							cout << "No negative values for amount." << endl;
							continue;
						} else {
							break;
						}
					}
				}
				if(newAmt == -1){
					break;
				} else {

					sel_acccurr.tot_amount = newAmt;

					break;
				}

			default:
				cout << "Please enter a value 0 to 3" << endl;
				break;
		}
	}

	return sel_acccurr;

}
// <op 5 edit account>

// <op 6 Remove an account>
void BankManagementSystem_Controller::removeAnAccount()
{
	view.printRemoveAcc();

	// user input account ID
	int selectID = selectID_IN();
	if(selectID == -1){
		return;
	}
	BankManagementSystem_Model::st_customer null_cust;
	BankManagementSystem_Model::st_account placeholder_acc;
	placeholder_acc.accountID = selectID;
	// select it first
	string retJSON = UDP_contr.createUDPPacket(6, null_cust, placeholder_acc, 0, 0, 0);
	// parse the JSON
	std::vector<BankManagementSystem_Model::st_account> viewingAcc = model.getAllAccounts(retJSON);
	BankManagementSystem_Model::st_account sel_acccurr = viewingAcc[0];
	view.printAccountsInfo(sel_acccurr); // display the selected account to be deleted

	int acc_owner = sel_acccurr.ownerID;	// get owner of th account

	if(sel_acccurr.accountID != -1){
		// confirmation msg
		view.printConfirmationMsg();

		int confirm_del;
		while(1){
			cin >> confirm_del;
			if (cin.fail())
			{
				cout << "ERROR -- confirm_del" << endl;
				cout << "Enter a valid value: ";
				// get rid of failure state
				cin.clear();

				// From Eric's answer (thanks Eric)
				// discard 'bad' character(s)
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				continue;
			} else {
				if(confirm_del == 1){
					// delete row
					//req UDP to remove specified account
					string retJSON = UDP_contr.createUDPPacket(9, null_cust, placeholder_acc, 0, 0, 0);

					if(retJSON.compare(0,1,"1") == 0)
					{
						cout << "Account deleted!" << endl;

						BankManagementSystem_Model::st_customer modCust;
						modCust.ID = acc_owner;
						string retJSON_custUpdate = UDP_contr.createUDPPacket(3, modCust, placeholder_acc, 0, 0, 0);

						// parse the JSON
						std::vector<BankManagementSystem_Model::st_customer> viewingCustomers = model.getAllCustomers(retJSON_custUpdate);
						modCust = viewingCustomers[0];

						// check if found
						if(modCust.ID != -1)
						{
							// if ID not -1, then it was found in the DB and returned
							int new_num_acc = 0;
							if (modCust.num_acc > 0){
								new_num_acc = modCust.num_acc - 1;
							}
							modCust.num_acc = new_num_acc;
							// update customer
							string retJSON_final = UDP_contr.createUDPPacket(3, modCust, placeholder_acc, 0, 0, 0);


						} else {
							// if owner does not exist
							cout << "Account owner does not exist, therefore no customer updated." << endl;
						}

						break;

					} else {
						cout << "That account ID does not exist, no account deleted." << endl;
						break;
					}

				} else if(confirm_del == 0){
					break;
				} else {
					cout << "Enter a valid option: ";
				}
			}
		}
	} else {
		cout << "No account deleted." << endl;
	}


}
// </op 6>

// <op 7 Global account transfer>
// type: local = 0, global = 1
// update num of transfers when successful transfer is completed
void BankManagementSystem_Controller::transfer(int type, int custID)
{
	BankManagementSystem_Model::st_customer custTrans;

	BankManagementSystem_Model::st_account SRC_acc;
	while(1){
		view.printInterTransfersFROM();
		int selectAccID_FROM = selectID_IN();
		if(selectAccID_FROM == -1){
			return;
		}
		SRC_acc.accountID = selectAccID_FROM;
		string retJSON_src = UDP_contr.createUDPPacket(6, custTrans, SRC_acc, 0, 0, 0);

		std::vector<BankManagementSystem_Model::st_account> viewingAccounts_src = model.getAllAccounts(retJSON_src);
		SRC_acc = viewingAccounts_src[0];

		if(SRC_acc.accountID == -1){
			cout << "That account does not exit" << endl;
			continue;
		} else {
			view.printAccountsInfo(SRC_acc);
			if (type == 0){
				if(SRC_acc.ownerID != custID){
					cout << "This account does not belong to the current customer for a local transfer." << endl;
					continue;
				}
			}
			break;
		}
	}

	BankManagementSystem_Model::st_account DEST_acc;
	while(1){
		view.printInterTransfersTO();
		int selectAccID_TO = selectID_IN();
		if(selectAccID_TO == -1){
			return;
		}
		DEST_acc.accountID = selectAccID_TO;
		string retJSON_dest = UDP_contr.createUDPPacket(6, custTrans, DEST_acc, 0, 0, 0);

		std::vector<BankManagementSystem_Model::st_account> viewingAccounts_dest = model.getAllAccounts(retJSON_dest);
		DEST_acc = viewingAccounts_dest[0];
		if(DEST_acc.accountID == -1){
			cout << "That account does not exist." << endl;
			continue;
		} else {
			view.printAccountsInfo(DEST_acc);
			if (type == 0){
				if(DEST_acc.ownerID != SRC_acc.ownerID){
					cout << "Owners of both accounts do not match for a local transfer." << endl;
					continue;
				}
			}

			break;
		}
	}

	double selectAmt = 0;
	while(1){
		view.printInterTransfersAMT(); //+double means remove from SRC add to DEST. Only positive values
		selectAmt = selectID_IN();
		if(selectAmt == -1){
			return;	// exit
		} else if(selectAmt < 0){
			cout << "Enter a positive value for the amount to be transferred." << endl;
			continue;
		}

		// now determine if amount is valid for transfer. accountFrom - amount.
		if(SRC_acc.tot_amount - selectAmt < 0){
			// invalid amount, too much
			cout << "The amount to be transferred is too much from the source." << endl;
			continue;
		} else {
			break;
		}
	}
	// at this point both accounts at least exist


	//at this point, only update requests are required.
	// last check to ensure local, check ownerID of both accounts are the same
	if(type == 0){
		if((SRC_acc.ownerID != DEST_acc.ownerID) && (SRC_acc.ownerID != custID)){
			cout << "Selected accounts are not valid for an internal transfer. " << endl;
			return;
		}
	}

	BankManagementSystem_Model::st_customer up_cust;
	up_cust.ID = custID;
	// update account structs
	cout << "SRC: " << SRC_acc.tot_amount << "-" << selectAmt << endl;
	SRC_acc.tot_amount = SRC_acc.tot_amount - selectAmt;
	cout << "SRC total: " << SRC_acc.tot_amount << endl;
	string retJSON_upSRC = UDP_contr.createUDPPacket(8, up_cust, SRC_acc, 0, 0, 0);
	if(retJSON_upSRC.compare(0,1,"1") == 0)
	{
		cout << "Account updated!" << endl;

		// update customer's number of transaction if they exist
		up_cust.ID = SRC_acc.ownerID;
		// select customer row
		string retJSON_custSRC = UDP_contr.createUDPPacket(3, up_cust, SRC_acc, 0, 0, 0);
		std::vector<BankManagementSystem_Model::st_customer> updCustSRC = model.getAllCustomers(retJSON_custSRC);
		up_cust = updCustSRC[0];
		up_cust.num_trans = up_cust.num_trans + 1;

		string retJSON_updcustSRC = UDP_contr.createUDPPacket(4, up_cust, SRC_acc, 0, 0, 0);

	} else {
		cout << "ERR: account update failed." << endl;

	}
	cout << "DEST: " << DEST_acc.tot_amount << "+" << selectAmt << endl;
	DEST_acc.tot_amount = DEST_acc.tot_amount + selectAmt;
	cout << "DEST total: " << DEST_acc.tot_amount << endl;
	string retJSON_upDEST = UDP_contr.createUDPPacket(8, up_cust, DEST_acc, 0, 0, 0);

	if(retJSON_upDEST.compare(0,1,"1") == 0)
	{
		cout << "Account updated!" << endl;
		// update customer's number of transaction if they exist
		up_cust.ID = DEST_acc.ownerID;
		// select customer row
		string retJSON_custDEST = UDP_contr.createUDPPacket(3, up_cust, DEST_acc, 0, 0, 0);
		std::vector<BankManagementSystem_Model::st_customer> updCustDEST = model.getAllCustomers(retJSON_custDEST);
		up_cust = updCustDEST[0];
		up_cust.num_trans = up_cust.num_trans + 1;

		string retJSON_updcustSRC = UDP_contr.createUDPPacket(4, up_cust, DEST_acc, 0, 0, 0);

	} else {
		cout << "ERR: account update failed." << endl;

	}


}
// </op 7>
