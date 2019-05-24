/*
 * BankManagementSystemUDPcontroller.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Jonathan
 */

#include "BankManagementSystemModel.h"

#ifndef BANKMANAGEMENTSYSTEMUDPCONTROLLER_H_
#define BANKMANAGEMENTSYSTEMUDPCONTROLLER_H_

class BankManagementSystemUDP_controller {

public:
	BankManagementSystemUDP_controller();
	virtual ~BankManagementSystemUDP_controller();

	void printtest(BankManagementSystem_Model::st_customer* aa);
	void checkConn();
	string testConn();

	string send_rec_Msg(string msgToSend);	// routine to send the desired msg and return the response string buffer

	string createUDPPacket(int reqCmd, BankManagementSystem_Model::st_customer cust_st, BankManagementSystem_Model::st_account acc_st, int acct_ID_SRC, int acct_ID_DEST, int transfer_amt);
	void createUDPHeader(char* b_header, int req);
	void createUDPBody(char* b_body, BankManagementSystem_Model::st_customer cust_st, BankManagementSystem_Model::st_account acc_st, int acct_ID_SRC, int acct_ID_DEST, int transfer_amt);
	void transfer(int type, int custID);
};

#endif /* BANKMANAGEMENTSYSTEMUDPCONTROLLER_H_ */
