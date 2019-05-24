/*
 * BankManagementSystemModel.cpp
 *
 *  Created on: Apr 16, 2019
 *      Author: Jonathan
 */

#include "BankManagementSystemModel.h"
#include "BankManagementSystemUDPcontroller.h"

#include <iostream>
#include <cstdlib>
#include <ctime> //for current date and time

#include <bits/stdc++.h>

#include "json.hpp"
// for convenience
using json = nlohmann::json;


BankManagementSystem_Model::BankManagementSystem_Model() {
	// TODO Auto-generated constructor stub

}

BankManagementSystem_Model::~BankManagementSystem_Model() {
	// TODO Auto-generated destructor stub
	// STACKDUMP ERROR HERE
	// soln, remove static arr
	/*
	if (pt_customers != NULL)
	{
		//delete [] pt_customers;
		//pt_customers = NULL;
	}
	*/

}

// <op 0>
// gets the customer name and PIN from the user (controller), then it generates the ID = given by DB, data created = lib, number of accounts = 0, total number of transactions = 0.
BankManagementSystem_Model::st_customer BankManagementSystem_Model::setNewCustomerInfo(string newCustNm, int newPIN)
{
	//<Get the current time>
	// current date/time based on current system
	time_t now = time(0);

	// convert now to string form
	char* dt = ctime(&now);
	//cout << "Time: " << dt << endl;
	//</Get the current time>

	// create a customer struct
	st_customer newCust;
	newCust.Name = newCustNm;
	newCust.PIN = newPIN;
	newCust.ID = 0;
	newCust.dateCreated = dt;
	newCust.num_acc = 0;
	newCust.num_trans = 0;

	return newCust;

}
// </op 0>

// <op 1 View all customers>
// From UDPcontroller JSON populate st_customer arr
// always get all from DB so you dont have to manage local array of customers
vector<BankManagementSystem_Model::st_customer> BankManagementSystem_Model::getAllCustomers(string buf)
{
	vector<st_customer> new_pt_customers;
	pt_customers = new_pt_customers;
	// test json parser
	// buffer is a json text, so need to serialize it.
	try {
		json j_complete = json::parse(buf);


	// print to see if ok
	//cout << endl << j_complete << endl;

	// now see if can get values // for default value if none found it must be the same type or it will end process
	//cout << endl << j_complete[0].value("id_cust", -1) << endl;
	//cout << endl << j_complete[1].value("nm_cust", "no name") << endl;

	//int count = 0;
	// populate st_customers
	for ( auto iter = j_complete.begin(); iter != j_complete.end(); ++iter ) {

		json j = *iter;
		//cout << j.value("nm_cust", "-1") << endl;

		// This section will terminate program if value types in the JSON and the expected type in j.value don't match
		st_customer newCust;
		newCust.Name = j.value("nm_cust", "-1");
		newCust.PIN = j.value("pin_cust", -1);
		newCust.ID = j.value("id_cust", -1);
		newCust.dateCreated = j.value("datecreated_cust", "0");
		newCust.num_acc = j.value("num_accounts_cust", 0);
		newCust.num_trans = j.value("num_trans_cust", 0);

		pt_customers.push_back(newCust);

		//count ++;

	}
	} catch (json::parse_error& e)
	{
		// output exception information
		std::cout << "message: " << e.what() << '\n'
				  << "exception id: " << e.id << '\n'
				  << "byte position of error: " << e.byte << std::endl;
	 }

	//BankManagementSystem_Model::setNumCustomers(count);
	// just peeking at an attribute
	/*
	for (std::vector<BankManagementSystem_Model::st_customer>::iterator iter = pt_customers.begin(); iter != pt_customers.end(); ++iter) {

		cout << iter.base()->Name << endl;
	}
	 */

	return pt_customers;
}

void BankManagementSystem_Model::setNumCustomers(int newSize)
{
	BankManagementSystem_Model::pt_customers_Size = newSize;
}

int BankManagementSystem_Model::getNumCustomers()
{
	return BankManagementSystem_Model::pt_customers_Size;
}

// </op1>

// <op 2 Select customer>


void BankManagementSystem_Model::modName(BankManagementSystem_Model::st_customer customer, string newNm)
{
	if(customer.ID != -1)
	{
		customer.Name = newNm;
	}
}

void BankManagementSystem_Model::modPIN(BankManagementSystem_Model::st_customer customer, int newPIN)
{
	if(customer.ID != -1)
	{
		customer.PIN = newPIN;
	}
}

BankManagementSystem_Model::st_account BankManagementSystem_Model::createAccount(double accAmt){
	//<Get the current time>
	// current date/time based on current system
	time_t now = time(0);

	// convert now to string form
	char* dt = ctime(&now);

	BankManagementSystem_Model::st_account new_acc;
	new_acc.dateCreated = dt;
	new_acc.tot_amount = accAmt;

	return new_acc;
}

// </op 2>

// <op 3>
BankManagementSystem_Model::st_customer BankManagementSystem_Model::removeCustomer_m(int selID)
{
	// check current struct array if has the desired customer
	/*
	if (pt_customers != NULL)
	{
		for(int i = 0; i < getNumCustomers(); i++)
		{
			if(selID == pt_customers[i].ID)
			{
				// delete from struct arr

				// delete from DB

				// return the struct to display removed info to user
				return *(pt_customers+i);
			}
		}
	}
	*/
	// if not found, return a struct with ID = -1
	st_customer notFoundCust;
	notFoundCust.ID = -1;
	return notFoundCust;

}
// </op 3>

// <op 4>
vector<BankManagementSystem_Model::st_account> BankManagementSystem_Model::getAllAccounts(string buf)
{
	vector<st_account> new_pt_accounts;
	pt_accounts = new_pt_accounts;
	// test json parser
	// buffer is a json text, so need to serialize it.
	json j_complete = json::parse(buf);

	// print to see if ok
	//cout << endl << j_complete << endl;

	//int count = 0;
	// populate st_accounts
	for ( auto iter = j_complete.begin(); iter != j_complete.end(); ++iter ) {

		json j = *iter;
		//cout << j.value("nm_cust", "-1") << endl;

		// This section will terminate program if value types in the JSON and the expected type in j.value don't match
		st_account newAcc;
		newAcc.ownerID = j.value("id_owner", -1);
		newAcc.accountID = j.value("id_account", -1);
		newAcc.dateCreated = j.value("datecreated_acc", "-1");
		newAcc.tot_amount = j.value("amount_acc", -1);


		pt_accounts.push_back(newAcc);

		//count ++;

	}


	return pt_accounts;
}

int BankManagementSystem_Model::getNumAccounts()
{
	return BankManagementSystem_Model::pt_accounts_Size;
}
// </op 4>
