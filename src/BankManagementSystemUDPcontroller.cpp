/*
 * BankManagementSystemUDPcontroller.cpp
 *
 *  Created on: Apr 18, 2019
 *      Author: Jonathan
 */

#include "BankManagementSystemUDPcontroller.h"
#include "BankManagementSystemModel.h"

#include "json.hpp"
// for convenience
using json = nlohmann::json;

using namespace std;
#include <string>
#include <iostream>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT     8081
#define MAXLINE 1024000
#define nodePORT	8080

#define TOTAL_PACKET_SIZE 1024
#define HEADER_SIZE 24
#define CMD_SIZE  	2
#define ENC_SIZE 	22

#define BODY_SIZE	1000
// <customer info>
#define NAME_SIZE	50
#define ID_SIZE		10	// customer ID / owner ID
#define PIN_SIZE	4
#define DATE_SIZE	30
#define NUM_ACC_SIZE	4
#define	NUM_TRANS_SIZE	4
// </customer info>
// <account info>
#define OWNER_SIZE	10
#define ACC_ID_SIZE	10
//#define DATE_SIZE	30
#define TOT_AMT_SIZE	30
// </account info>
// <acount transfer info>
#define TRANS_ACC_SRC_ID_SIZE	10
#define TRANS_ACC_DEST_ID_SIZE	10
#define TRANS_AMT_SIZE			10
// </acount transfer info>

BankManagementSystemUDP_controller::BankManagementSystemUDP_controller() {
	// TODO Auto-generated constructor stub

}

BankManagementSystemUDP_controller::~BankManagementSystemUDP_controller() {
	// TODO Auto-generated destructor stub
}

void BankManagementSystemUDP_controller::printtest(BankManagementSystem_Model::st_customer* aa)
{
	cout << aa->Name << endl;
}

// open node.js command prompt and manually start node js server
string BankManagementSystemUDP_controller::testConn()
{
	int sockfd;
	char buffer[MAXLINE];
	const char *hello = "02Hello from client";
	struct sockaddr_in     servaddr;
	struct sockaddr_in		clientaddr;



	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	/*
	// get this clients socket info
	struct sockaddr_in sin;
	socklen_t len_sin = sizeof(sin);
	if (getsockname(sockfd, (struct sockaddr *)&sin, &len_sin) == -1)
	    perror("getsockname");
	else
	    printf("port number %d\n", ntohs(sin.sin_port));
*/

	memset(&clientaddr, 0, sizeof(clientaddr));

	// Filling server information
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(PORT);
	clientaddr.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&clientaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// time out for socket
	struct timeval read_timeout;
	read_timeout.tv_sec = 10;
	read_timeout.tv_usec = 0;
	int sockoptRes = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
	if (sockoptRes != 0){
		printf("Setting timeout.");
		fprintf(stderr, "Value of errno: %d\n", errno);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(nodePORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	int n;

	sendto(sockfd, (const char *)hello, strlen(hello),
		0, (const struct sockaddr *) &servaddr,
			sizeof(servaddr));
	printf("Hello message sent.\n");

	// Since receive from node js it causes 'bad address' error.
	/*
	int len;
	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				0, (struct sockaddr *) &servaddr,
				&len);
	*/
	n = recv(sockfd, (char *)buffer, MAXLINE, 0);

	if (n == -1){
		fprintf(stderr, "Value of errno: %d\n", errno);
		perror("Error printed by perror");
		if(errno == 1)
		{
			printf("Socket timeout, server may be offline.");
		}
	}

	buffer[n] = '\0';
	printf("Server : %s\n", buffer);
	printf("size: %d", n);
	//printf("Server sending port: %s", servaddr.sin_port);

	close(sockfd);

	// test json parser
	// buffer is a json text, so need to serialize it.
	//json j_complete = json::parse(buffer);

	// print to see if ok
	//cout << endl << j_complete << endl;

	// now see if can get values // for default value if none found it must be the same type or it will end process
	//cout << endl << j_complete[0].value("id_cust", -1) << endl;
	//cout << endl << j_complete[1].value("nm_cust", "no name") << endl;

	return buffer;


}

string BankManagementSystemUDP_controller::send_rec_Msg(string msgToSend)
{

	int sockfd;
	char rec_Buffer[MAXLINE];
	//std::string s = b_Packet;
	const char *hello = msgToSend.c_str();	// 1 byte each index

	cout << "Sending... " << hello <<endl;

	struct sockaddr_in     servaddr;
	struct sockaddr_in		clientaddr;

	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&clientaddr, 0, sizeof(clientaddr));

	// Filling server information
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(PORT);
	clientaddr.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&clientaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// time out for socket
	struct timeval read_timeout;
	read_timeout.tv_sec = 10;
	read_timeout.tv_usec = 0;
	int sockoptRes = setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
	if (sockoptRes != 0){
		printf("Setting timeout.");
		fprintf(stderr, "Value of errno: %d\n", errno);
	}

	memset(&servaddr, 0, sizeof(servaddr));

	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(nodePORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;

	int n;

	sendto(sockfd, (const char *)hello, strlen(hello),
		0, (const struct sockaddr *) &servaddr,
			sizeof(servaddr));
	//printf("Hello message sent.\n");

	n = recv(sockfd, (char *)rec_Buffer, MAXLINE, 0);

	if (n == -1){
		fprintf(stderr, "Value of errno: %d\n", errno);
		perror("Error printed by perror");
		if(errno == 1)
		{
			printf("Socket timeout, server may be offline.");
		}
	}

	rec_Buffer[n] = '\0';
	//cout << "Server: " << rec_Buffer << endl;
	//cout << "size: " << n << endl;
	//printf("Server sending port: %s", servaddr.sin_port);

	close(sockfd);

	cout << "Returned JSON: " << rec_Buffer << endl;

	return rec_Buffer;
}

// int
/*	request commands
 *	01 = create customer (inserting a customer to the DB)
 *	02 = get all customers (selecting all customers)
 *	03 = select customer
 *	04 = update customer
 *	05 = remove a customer (deleting a row)
 *	06 = get all accounts (either all (-2) or based on an acc ID passed)
 *	07 = select account based on owner
 *	08 = update account
 *	09 = remove an account (deleting a row)
 *	10 = add an account
 *	// 20 = account amount transfer (need account IDs (to/from) and valid amount.)
 *	50 = client closing, tell server it can as well.
 *
 *	// todo send message to node JS to commit to DB
		// a class to handle UDP
		// msg format [job][encyption key][body]
		// [01][ads3331a][[name][ID][PIN][date created]]
 */
string BankManagementSystemUDP_controller::createUDPPacket(int reqCmd, BankManagementSystem_Model::st_customer cust_st, BankManagementSystem_Model::st_account acc_st, int acct_ID_SRC, int acct_ID_DEST, int transfer_amt)
{
	// <create packet>
	char b_header[HEADER_SIZE+1];
	std::fill_n(b_header, HEADER_SIZE, '0');
	//string z = to_string(0);
	//char const *pchar_z = z.c_str();

	createUDPHeader(b_header, reqCmd);
	b_header[HEADER_SIZE] = '\0';

	/*
	for(int i = 0 ; i < HEADER_SIZE ; i ++ ){
		cout << i << ".";
		  cout << (int)b_header[i] << " ";
	}
	cout << endl;
	cout.flush();
	*/
	// </create packet>

	// <create body>
	char b_body[BODY_SIZE+1];
	std::fill_n(b_body, BODY_SIZE, '0');
	createUDPBody(b_body, cust_st, acc_st, acct_ID_SRC, acct_ID_DEST, transfer_amt);
	b_body[BODY_SIZE] = '\0';

	/*
	for(int i = 0 ; i < BODY_SIZE ; i ++ ){
	cout << i << ".";
		  cout << (int)b_body[i] << " ";
	}
	cout << endl;
	cout.flush();
	 */
	// </create body>

	// <Complete packet>
	char b_Packet[TOTAL_PACKET_SIZE + 1];

	// total packet.
	std::fill_n(b_Packet, TOTAL_PACKET_SIZE, '0');

	b_Packet[TOTAL_PACKET_SIZE] = '\0';

	//strncpy(b_Packet, b_header, HEADER_SIZE); // not working
	for(int i = 0 ; i < HEADER_SIZE ; i ++ ){
		b_Packet[i] = b_header[i];
	}


	//strncpy(b_Packet + HEADER_SIZE, b_body, BODY_SIZE); //not working
	for(int i = 0 ; i < BODY_SIZE ; i ++ ){
		b_Packet[i + HEADER_SIZE] = b_body[i];
	}
	/*
	for(int i = 0 ; i < TOTAL_PACKET_SIZE ; i ++ ){
		cout << i << ".";
	      cout << (int)b_Packet[i];
	      cout  << " ";
	}
	cout << endl;
	cout.flush();
	 */
	string s_fullPack(b_Packet);
	//cout << "FULL PACKET STRING: " << s_fullPack << endl;
	// </Complete packet>

	string returnedJSON = send_rec_Msg(s_fullPack);
	//cout << "rec string: " << returnedJSON << endl;

	// convert to JSON so the "success" value can be checked whether it is:
	//  3 to indicate affectedRows json
	// 	2 to  an empty JSON was returned from the DB query
	//  1 for normal OK
	// 	0 an API error occurred (either in the API itself or DB error)
	// buffer is a json text, so need to serialize it.
	try {
		json j_complete = json::parse(returnedJSON);

		// check empty json returned, so check the string
		/*
		if(returnedJSON.compare(0,1,"[") == 0 && returnedJSON.compare(1,1,"]") == 0)
		{
			returnedJSON = "[{ \"id_cust\" : -1, \"id_account\" : -1}]";
			//cout << "here !! " << returnedJSON << endl;
		}*/

		int suc_tag = j_complete.at(0)["success"];
		cout << "success tag: " << suc_tag << endl;

		if (suc_tag == 0){
			// error
			// print error for developer
			string err_msg = j_complete.at(0)["err_message"];
			cout << "Error message from server: " << err_msg << endl;

			returnedJSON = "[{ \"id_cust\" : -1, \"id_account\" : -1}]";
		} else if(suc_tag == 1){
			// normal OK
			// remove success JSON at front
			j_complete.erase(0);
			returnedJSON = j_complete.dump();	// serialize to string


		} else if(suc_tag == 2){
			// empty returned from db query
			returnedJSON = "[{ \"id_cust\" : -1, \"id_account\" : -1}]";
		} else if(suc_tag == 3){
			int aff = j_complete.at(1)["affectedRows"];

			returnedJSON = to_string(aff);

		} else {
			// something else wrong.
			returnedJSON = "[{ \"id_cust\" : -1, \"id_account\" : -1}]";
		}
	} catch (json::parse_error& e) {
		// output exception information
		std::cout << "message: " << e.what() << '\n'
				  << "exception id: " << e.id << '\n'
				  << "byte position of error: " << e.byte << std::endl;
	}

	cout << "returned: " << returnedJSON <<endl;

	return returnedJSON;
}

//void BankManagementSystemUDP_controller::createUDPHeader(char* b_header, int reqCmd)
void BankManagementSystemUDP_controller::createUDPHeader(char* b_header, int reqCmd)
{
	std::string s = std::to_string(reqCmd);
	if(s.length() == 1){
		char req = '0' + reqCmd;
		//char const *pchar = s.c_str();
		//strncpy(b_header+1, pchar, 1);
		b_header[1] = req;
	} else {
		int d10 = reqCmd / 10;
		d10 = floor(d10);
		int d1 = reqCmd % 10;

		char req10 = '0' + d10;
		char req1 = '0' + d1;

		b_header[0] = req10;
		b_header[1] = req1;
	}

	//char b_encryp[ENC_SIZE];
	// encrypt place holder
	//std::fill_n(b_encryp, ENC_SIZE, 9);
	//strncpy(b_header+HEADER_SIZE, b_encryp, ENC_SIZE); // not working

	for(int i =  0; i < HEADER_SIZE - CMD_SIZE ; i ++ ){
		b_header[i + CMD_SIZE] = '0' + 9;
	}


	// Just use string...
	/*
	std::string s_req = std::to_string(reqCmd);
	b_header.append("0");
	b_header.append(s_req);

	for(int i =  0; i < HEADER_SIZE ; i ++ ){
		b_header[i + CMD_SIZE] = '0' + 9;
	}
*/
}

void BankManagementSystemUDP_controller::createUDPBody(char* b_body, BankManagementSystem_Model::st_customer cust_st, BankManagementSystem_Model::st_account acc_st, int acct_ID_SRC, int acct_ID_DEST, int transfer_amt)
{
	// <customer section>
	string name = cust_st.Name + "/";
	char const *pchar_name = name.c_str();

	//strncpy(b_body, pchar_name, NAME_SIZE); null terminating because destination is large enough
	for(unsigned int i = 0; i < strlen(pchar_name); i ++)
	{
		b_body[i] = pchar_name[i];
	}

	string id = to_string(cust_st.ID) + "/";
	char const *pchar_id = id.c_str();

	//cout << "ID HERE: " << id << endl;

	for(unsigned int i = 0; i < strlen(pchar_id); i ++)
	{
		b_body[i+NAME_SIZE] = pchar_id[i];
	}

	string pin = to_string(cust_st.PIN);
	char const *pchar_pin = pin.c_str();

	//strncpy(b_body + NAME_SIZE, pchar_pin, PIN_SIZE);
	for(unsigned int i = 0; i < strlen(pchar_pin); i ++)
	{
		b_body[i+NAME_SIZE+ID_SIZE] = pchar_pin[i];
	}

	string date = cust_st.dateCreated + "/";
	char const *pchar_date = date.c_str();

	//strncpy(b_body + NAME_SIZE + PIN_SIZE, pchar_date, DATE_SIZE);
	for(unsigned int i = 0; i < strlen(pchar_date); i ++)
	{
		b_body[i+NAME_SIZE + ID_SIZE + PIN_SIZE] = pchar_date[i];
	}

	string numOfAcc = to_string(cust_st.num_acc) + "/";
	char const *pchar_AccNum = numOfAcc.c_str();

	//strncpy(b_body + NAME_SIZE, pchar_pin, PIN_SIZE);
	for(unsigned int i = 0; i < strlen(pchar_AccNum); i ++)
	{
		b_body[i+NAME_SIZE+ID_SIZE+ PIN_SIZE + DATE_SIZE] = pchar_AccNum[i];
	}

	string numOfTrans = to_string(cust_st.num_trans) + "/";
	char const *pchar_TransNum = numOfTrans.c_str();

	//strncpy(b_body + NAME_SIZE, pchar_pin, PIN_SIZE);
	for(unsigned int i = 0; i < strlen(pchar_TransNum); i ++)
	{
		b_body[i+NAME_SIZE+ID_SIZE+ PIN_SIZE + DATE_SIZE+ NUM_ACC_SIZE] = pchar_TransNum[i];
	}

	// </customer section>

	// account section
	string acct_owner = to_string(acc_st.ownerID) + "/";
	char const *pchar_acctOwner = acct_owner.c_str();

	for(unsigned int i = 0; i < strlen(pchar_acctOwner); i ++)
	{
		b_body[i+NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE] = pchar_acctOwner[i];
	}

	string acct_ID = to_string(acc_st.accountID) + "/";
	char const *pchar_acctID = acct_ID.c_str();

	for(unsigned int i = 0; i < strlen(pchar_acctID); i ++)
	{
		b_body[i+NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE] = pchar_acctID[i];
	}

	string acc_date = acc_st.dateCreated + "/";
	char const *pchar_date_acc = acc_date.c_str();

	//strncpy(b_body + NAME_SIZE + PIN_SIZE, pchar_date, DATE_SIZE);
	for(unsigned int i = 0; i < strlen(pchar_date_acc); i ++)
	{
		b_body[i+NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE] = pchar_date_acc[i];
	}

	string acc_amt = to_string(acc_st.tot_amount) + "/";
	char const *pchar_amt_acc = acc_amt.c_str();

	//strncpy(b_body + NAME_SIZE + PIN_SIZE, pchar_date, DATE_SIZE);
	for(unsigned int i = 0; i < strlen(pchar_amt_acc); i ++)
	{
		b_body[i+NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE] = pchar_amt_acc[i];
	}

	// account transfer section
	string acct_src = to_string(acct_ID_SRC) + "/";
	char const *pchar_acctSRC = acct_src.c_str();

	for(unsigned int i = 0; i < strlen(pchar_acctSRC); i ++)
	{
		b_body[i+NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE + TOT_AMT_SIZE] = pchar_acctSRC[i];
	}

	string acct_dest = to_string(acct_ID_DEST) + "/";
	char const *pchar_acctDEST = acct_dest.c_str();

	for(unsigned int i = 0; i < strlen(pchar_acctDEST); i ++)
	{
		b_body[i+NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_DEST_ID_SIZE] = pchar_acctDEST[i];
	}

	string tras_amt = to_string(transfer_amt) + "/";
	char const *pchar_acctTrans = tras_amt.c_str();

	for(unsigned int i = 0; i < strlen(pchar_acctTrans); i ++)
	{
		b_body[i+NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_DEST_ID_SIZE + TRANS_ACC_DEST_ID_SIZE] = pchar_acctTrans[i];
	}
}



// int
void communicateUDPmsg()
{
	/*
	 * switch depending on reqCmd for correct handling of packet
	 * switch(reqCmd):
	 * case(newCust):
	 * 	call func
	 * 	break
	 * case(view all)
	 * 	call func
	 * 	break;
	 */
}


