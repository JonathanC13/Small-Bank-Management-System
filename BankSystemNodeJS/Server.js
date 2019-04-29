// require function file
const impl_API = require('./bank_API_impl.js');

'use strict';

const TOTAL_PACKET_SIZE = 1024;
const HEADER_SIZE       = 24;
const CMD_SIZE          = 2;
const ENC_SIZE          = 22;

const BODY_SIZE	        = 1000;
// <customer info>
const NAME_SIZE	        = 50;
const ID_SIZE           = 10;
const PIN_SIZE	        = 4;
const DATE_SIZE	        = 30;
const NUM_ACC_SIZE      = 4;
const	NUM_TRANS_SIZE    = 4;
// </customer info>
// <account info>
const OWNER_SIZE        =	10;
const ACC_ID_SIZE       = 10;
//const DATE_SIZE	= 30
const TOT_AMT_SIZE      =	30;
// </account info>
// <account transfer info>
const TRANS_ACC_SRC_ID_SIZE  = 10;
const TRANS_ACC_DEST_ID_SIZE = 10;
const TRANS_AMT_SIZE			   = 10;
// </account transfer info>

const dgram = require('dgram');
const server = dgram.createSocket('udp4');
const client = dgram.createSocket('udp4');

server.on('error', (err) => {
  console.log(`server error:\n${err.stack}`);
  server.close();
});

server.on('message', (msg, rinfo) => {

  // Parse the message here and respond
  console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);


  let i, msglen;
  msglen = msg.length;
  // start(inclusive), end(exclude)

  // <Extract cmd request>
  let cmd = msg.slice(0,CMD_SIZE);
  console.log("CMD " + cmd);
  let i_cmd = parseInt(cmd, 10);
  // </Extract cmd request>

  // <Extract name>
  let s_name_seg = msg.slice(HEADER_SIZE, HEADER_SIZE + NAME_SIZE);
  let s_name_ind = s_name_seg.indexOf("/");
  var s_name;
  if(s_name_ind != -1)
  {
    s_name = s_name_seg.slice(0,s_name_ind);
  } else {
    s_name = s_name_seg;
  }
  console.log("NAME: " + s_name);
  // </Extract name>

  // <Extract ID>
  let s_ID_seg = msg.slice(HEADER_SIZE + NAME_SIZE , HEADER_SIZE + NAME_SIZE + ID_SIZE);
  let s_ID_ind = s_ID_seg.indexOf("/");
  var s_ID;
  if(s_ID_ind != -1)
  {
    s_ID = s_ID_seg.slice(0, s_ID_ind);
  } else {
    s_ID = s_ID_seg;
  }
  var i_ID = parseInt(s_ID, 10);
  console.log("ID: " + i_ID);
  // </Extract ID>

  // <Extract pin>
  // PIN always 4 digits
  let s_PIN = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE);
  console.log("PIN: " + s_PIN);
  var i_PIN = parseInt(s_PIN, 10);
  // </Extract pin>

  // <Extract date>
  let s_date_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE);
  let s_date_ind = s_date_seg.indexOf("/");
  var s_date;
  if(s_date_ind != -1)
  {
    s_date = s_date_seg.slice(0, s_date_ind-1);
  } else {
    s_date = s_date_seg;
  }
  console.log("DATE: " + s_date);
  // </Extract date>

  // < todo extract account info stuff>
  // <Extract number accounts>
  let s_num_acc_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE);
  let s_num_acc_ind = s_num_acc_seg.indexOf("/");
  var i_num_acc;
  if(s_num_acc_ind != -1){
    let s = s_num_acc_seg.slice(0, s_num_acc_ind - 1);
    i_num_acc = parseInt(s, 10);
  } else {
    i_num_acc = parseInt(s_num_acc_seg, 10);
  }
  // </Extract number accounts>
  // <Extract number transactions>
  let i_num_trans_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE);
  let i_num_trans_ind = i_num_trans_seg.indexOf("/");
  var i_num_trans;
  if(i_num_trans_ind != -1){
    let s = i_num_trans_seg.slice(0, i_num_trans_ind - 1);
    i_num_trans = parseInt(s, 10);
  } else {
    i_num_trans = parseInt(i_num_trans_seg, 10);
  }
  // </Extract number transactions>

  // account info
  // <Extract owner ID>
  let s_ownerID_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE);
  let s_ownerID_ind = s_ownerID_seg.indexOf("/");
  var s_ownerID;
  if(s_ownerID_ind != -1)
  {
    s_ownerID = s_ownerID_seg.slice(0, s_ownerID_ind);
  } else {
    s_ownerID = s_ownerID_seg;
  }
  var i_ownerID = parseInt(s_ownerID, 10);
  // <Extract owner ID>
  // <Extract acc ID>
  let s_accID_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE);
  let s_accID_ind = s_accID_seg.indexOf("/");
  var s_accID;
  if(s_accID_ind != -1)
  {
    s_accID = s_accID_seg.slice(0, s_accID_ind);
  } else {
    s_accID = s_accID_seg;
  }
  var i_accID = parseInt(s_accID, 10);
  // </Extract acc ID>
  // <Extract account date>
  let s_accdate_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE);
  let s_accdate_ind = s_accdate_seg.indexOf("/");
  var s_accdate;
  if(s_accdate_ind != -1)
  {
    s_accdate = s_accdate_seg.slice(0, s_accdate_ind-1);
  } else {
    s_accdate = s_accdate_seg;
  }
  // </Extract account date>
  // <Extract account total amount>
  let s_accAMT_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE + TOT_AMT_SIZE);
  let s_accAMT_ind = s_accAMT_seg.indexOf("/");
  var s_accAMT;
  if(s_accAMT_ind != -1)
  {
    s_accAMT = s_accAMT_seg.slice(0, s_accAMT_ind);
  } else {
    s_accAMT = s_accAMT_seg;
  }
  var i_accAMT = parseInt(s_accAMT, 10);
  // </Extract account total amount>

  // account table
  // <transfer source>
  let i_SRC_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE + TOT_AMT_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + ACC_ID_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE);
  let i_SRC_ind = i_SRC_seg.indexOf("/");
  var i_SRC;
  if(i_SRC_ind != -1){
    let s = i_SRC_seg.slice(0, i_SRC_ind);
    i_SRC = parseInt(s, 10);
  } else {
    i_SRC = parseInt(i_SRC_seg, 10);
  }
  console.log("SRC: " + i_SRC);
  // </transfer source>
  // <transfer destination>
  let i_DEST_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE + TRANS_ACC_DEST_ID_SIZE);
  let i_DEST_ind = i_DEST_seg.indexOf("/");
  var i_DEST;
  if(i_DEST_ind != -1){
    let s = i_DEST_seg.slice(0, i_DEST_ind);
    i_DEST = parseInt(s, 10);
  } else {
    i_DEST = parseInt(i_DEST_seg, 10);
  }
  // </transfer destination>
  // <Extract amount>
  let i_amount_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE + TRANS_ACC_DEST_ID_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE + TRANS_ACC_DEST_ID_SIZE + TRANS_AMT_SIZE);
  let i_amount_ind = i_amount_seg.indexOf("/");
  var i_amount;
  if(i_amount_ind != -1){
    let s = i_amount_seg.slice(0, i_amount_ind - 1);
    i_amount = parseInt(s, 10);
  } else {
    i_amount = parseInt(i_amount_seg, 10);
  }
  // </Extract amount>


  // <Handle request command>
  if(i_cmd == 1){
    // request command: Create customer
    // todo, determine unique ID and return full created customer
    impl_API.createCustomer(s_name, i_PIN, s_date, i_num_acc, i_num_trans, function(result){
      var stuff_i_want = result;  // json obj

      // conver JSON to string
      let jsonStr = JSON.stringify(stuff_i_want);
      console.log("SocketLog: create customer JSON = " + jsonStr);
      // send response
      //const message = Buffer.from('Some bytes');
      const message = Buffer.from(jsonStr);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    });
  }
  else if(i_cmd == 2){
    // request command: select all customers
    impl_API.getCustomers(-2, function(result){
      var stuff_i_want = result;  // json obj
      console.log(stuff_i_want);

      // conver JSON to string
      let jsonStr = JSON.stringify(stuff_i_want);
      console.log("SocketLog: view all customer JSON = " + jsonStr);
      // send response
      //const message = Buffer.from('Some bytes');
      const message = Buffer.from(jsonStr);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        //console.log(rinfo.address);
        //console.log(rinfo.port);
        //console.log(message);

        //server.close();
      });

    });
  }
  else if(i_cmd == 3){
    // request command: select customer
    impl_API.getCustomers(i_ID, function(result){
      var stuff_i_want = result;  // json obj
      console.log(stuff_i_want);

/*
      if (result.length === 0){
        console.log("json is empty");
      }
*/
      // conver JSON to string
      let jsonStr = JSON.stringify(stuff_i_want);
      console.log("SocketLog: view selected customer JSON = " + jsonStr);
      // send response
      //const message = Buffer.from('Some bytes');
      const message = Buffer.from(jsonStr);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        //console.log(rinfo.address);
        //console.log(rinfo.port);
        //console.log(message);

        //server.close();
      });
    });
  }
  else if(i_cmd == 4){
    // update a customer row
    impl_API.updateCustomer(i_ID, s_name, i_PIN, i_num_acc, i_num_trans, function(result){
      var stuff_i_want = result.toString();  // json obj
      console.log(stuff_i_want);

      // conver JSON to string

      const message = Buffer.from(stuff_i_want);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        //console.log(rinfo.address);
        //console.log(rinfo.port);
        //console.log(message);

        //server.close();
      });
    });
  } else if(i_cmd == 5){
    // request command: remove customer
    impl_API.removeCustomer(i_ID, function(result){
      var stuff_i_want = result.toString();  // json obj
      console.log(stuff_i_want);

      // conver JSON to string

      const message = Buffer.from(stuff_i_want);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        //console.log(rinfo.address);
        //console.log(rinfo.port);
        //console.log(message);

        //server.close();
      });
    });
  } else if(i_cmd == 6){
    // request command: get all accounts (-2) or to specific account ID (sent through i_SRC segment)
    impl_API.getAllAccounts(i_accID, function(result){
      var stuff_i_want = result;  // json obj
      console.log(stuff_i_want);

      // conver JSON to string
      let jsonStr = JSON.stringify(stuff_i_want);
      console.log("SocketLog: view all customer JSON = " + jsonStr);
      // send response
      //const message = Buffer.from('Some bytes');
      const message = Buffer.from(jsonStr);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        //console.log(rinfo.address);
        //console.log(rinfo.port);
        //console.log(message);

        //server.close();
      });

    });
  } else if(i_cmd == 7){
    // request command: get all accounts associated to owner ID
    impl_API.getCustAccounts(i_ID, function(result){
      var stuff_i_want = result;  // json obj
      console.log(stuff_i_want);

      // conver JSON to string
      let jsonStr = JSON.stringify(stuff_i_want);
      console.log("SocketLog: view all customer JSON = " + jsonStr);
      // send response
      //const message = Buffer.from('Some bytes');
      const message = Buffer.from(jsonStr);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        //console.log(rinfo.address);
        //console.log(rinfo.port);
        //console.log(message);

        //server.close();
      });

    });
  } else if(i_cmd == 8){
    // request command: update account
    impl_API.updateAccount(i_ownerID, i_accID, i_accAMT, function(result){
      var stuff_i_want = result.toString();  // json obj
      console.log(stuff_i_want);

      // conver JSON to string

      const message = Buffer.from(stuff_i_want);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        //console.log(rinfo.address);
        //console.log(rinfo.port);
        //console.log(message);

        //server.close();
      });
    });

  } else if(i_cmd == 9){
    // request command: remove specific account
    impl_API.removeAccount(i_accID, function(result){
      var stuff_i_want = result.toString();  // json obj
      console.log(stuff_i_want);

      // conver JSON to string

      const message = Buffer.from(stuff_i_want);
      client.send(message, rinfo.port, rinfo.address, (err) => {
        //console.log(rinfo.address);
        //console.log(rinfo.port);
        //console.log(message);

        //server.close();
      });
    });
  } else if (i_cmd == 50){
    let close_msg = "Server closing ...";
    client.send(close_msg, rinfo.port, rinfo.address, (err) => {
      //console.log(rinfo.address);
      //console.log(rinfo.port);
      console.log(close_msg);

      //server.close();
    });
    server.close();
    process.exit()
  }
  // </Handle request command>

});

server.on('listening', () => {
  const address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
});

server.bind(8080);
