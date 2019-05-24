// require function file
const impl_API = require('./bank_API_impl.js');

"use strict"; // catch bad syntax

// <UDP packet segments>
// <Packet header>
const TOTAL_PACKET_SIZE = 1024;   // total udp packet size
const HEADER_SIZE       = 24;     // total udp packet header size
const CMD_SIZE          = 2;      // command request size
const ENC_SIZE          = 22;     // Encryption key size
// </Packet header>
// <Packet body>
const BODY_SIZE	        = 1000;   // total udp packet body size
// <customer table row info>
const NAME_SIZE	        = 50;     // name of the customer size
const ID_SIZE           = 10;     // ID of the customer size
const PIN_SIZE	        = 4;      // PIN of the customer size
const DATE_SIZE	        = 30;     // Date of creation of the customer size
const NUM_ACC_SIZE      = 4;      // number of accounts the customer owns size
const	NUM_TRANS_SIZE    = 4;      // number of transactions the customer has completed (to and from)
// </customer table row info>
// <account table row info>
const OWNER_SIZE        =	10;     // ID of the owner of the account size
const ACC_ID_SIZE       = 10;     // ID of the account size
//const DATE_SIZE	= 30            // Date of creation of the account size (re-use customer const size)
const TOT_AMT_SIZE      =	30;     // total amount in the account size
// </account table row info>
// <account transfer info> // Not in use at the moment.
const TRANS_ACC_SRC_ID_SIZE  = 10;// transfer account ID source size
const TRANS_ACC_DEST_ID_SIZE = 10;// transfer account ID destination size
const TRANS_AMT_SIZE			   = 10;// total amount to transfer size
// </account transfer info>
// </Packet body>
// </UDP packet segments>

function isEmpty(obj) {
    for(var key in obj) {
        if(obj.hasOwnProperty(key))
            return false;
    }
    return true;
}

const dgram = require('dgram');
const server = dgram.createSocket('udp4');  // Open socket for udp transfers
//const client = dgram.createSocket('udp4');

// <Handle server error by closing the socket.>
server.on('error', (err) => {
  console.log(`server error:\n${err.stack}`);
  server.close();
});
// </Handle server error by closing the socket.>

// <Handle receiving messages>
server.on('message', (msg, rinfo) => {

  // Parse the message here and respond
  console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);

  let i, msglen;
  msglen = msg.length;

  // <Extract information from udp packet>
  // <Extract cmd request>
  let cmd = msg.slice(0,CMD_SIZE);  // start(inclusive), end(exclude)
  //console.log("CMD " + cmd);
  var i_cmd = parseInt(cmd, 10);
  // </Extract cmd request>

  // <Extract customer name>
  let s_name_seg = msg.slice(HEADER_SIZE, HEADER_SIZE + NAME_SIZE); // copy the segment to a local array
  let s_name_ind = s_name_seg.indexOf("/"); // find end of information in segment
  var s_name;
  if(s_name_ind != -1)
  {
    // if end of information char found, then copy that valid segment out.
    s_name = s_name_seg.slice(0,s_name_ind);
  } else {
    // if no end of information char found, then the information fills the whole segment
    s_name = s_name_seg;
  }
  //console.log("NAME: " + s_name);
  // </Extract customer name>

  // <Extract customer ID>
  let s_ID_seg = msg.slice(HEADER_SIZE + NAME_SIZE , HEADER_SIZE + NAME_SIZE + ID_SIZE);
  let s_ID_ind = s_ID_seg.indexOf("/"); // find end of information in segment
  var s_ID;
  if(s_ID_ind != -1)
  {
    s_ID = s_ID_seg.slice(0, s_ID_ind);
  } else {
    s_ID = s_ID_seg;
  }
  var i_ID = parseInt(s_ID, 10);  // convert extracted string to integer
  //console.log("ID: " + i_ID);
  // </Extract customer ID>

  // <Extract customer pin>
  // PIN always 4 digits
  let s_PIN = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE);
  //console.log("PIN: " + s_PIN);
  var i_PIN = parseInt(s_PIN, 10);  // convert extracted string to integer
  // </Extract customer pin>

  // <Extract customer creation date>
  let s_date_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE);
  let s_date_ind = s_date_seg.indexOf("/");
  var s_date;
  if(s_date_ind != -1)
  {
    s_date = s_date_seg.slice(0, s_date_ind-1);
  } else {
    s_date = s_date_seg;
  }
  //console.log("DATE: " + s_date);
  // </Extract customer creation date>

  // < todo extract account info stuff>
  // <Extract number accounts>
  let s_num_acc_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE);
  let s_num_acc_ind = s_num_acc_seg.indexOf("/");
  var s_num_acc;
  if(s_num_acc_ind != -1){
    s_num_acc = s_num_acc_seg.slice(0, s_num_acc_ind);
  } else {
    s_num_acc = s_num_acc_seg;
  }
  var i_num_acc = parseInt(s_num_acc, 10);
  console.log("num_acc: " + i_num_acc);
  // </Extract number accounts>
  // <Extract number transactions>
  let s_num_trans_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE);
  let s_num_trans_ind = s_num_trans_seg.indexOf("/");
  var s_num_trans;
  if(s_num_trans_ind != -1){
    s_num_trans = s_num_trans_seg.slice(0, s_num_trans_ind);
  } else {
    s_num_trans = s_num_trans_seg;
  }
  var i_num_trans = parseInt(s_num_trans, 10);
  console.log("num_trans: " + i_num_trans);
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
  var s_SRC;
  if(i_SRC_ind != -1){
    s_SRC = i_SRC_seg.slice(0, i_SRC_ind);
  } else {
    s_SRC = i_SRC_seg;
  }
  var i_SRC = parseInt(i_SRC_seg, 10);
  //console.log("SRC: " + i_SRC);
  // </transfer source>
  // <transfer destination>
  let i_DEST_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE + TRANS_ACC_DEST_ID_SIZE);
  let i_DEST_ind = i_DEST_seg.indexOf("/");
  var s_DEST;
  if(i_DEST_ind != -1){
    s_DEST = i_DEST_seg.slice(0, i_DEST_ind);
  } else {
    s_DEST = i_DEST_seg;
  }
  var i_DEST = parseInt(s_DEST, 10);
  // </transfer destination>
  // <Extract amount>
  let i_amount_seg = msg.slice(HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE + TRANS_ACC_DEST_ID_SIZE, HEADER_SIZE + NAME_SIZE + ID_SIZE + PIN_SIZE + DATE_SIZE + NUM_ACC_SIZE + NUM_TRANS_SIZE + OWNER_SIZE + DATE_SIZE + TOT_AMT_SIZE + TRANS_ACC_SRC_ID_SIZE + TRANS_ACC_DEST_ID_SIZE + TRANS_AMT_SIZE);
  let i_amount_ind = i_amount_seg.indexOf("/");
  var s_amount;
  if(i_amount_ind != -1){
    s_amount = i_amount_seg.slice(0, i_amount_ind - 1);
  } else {
    s_amount = i_amount_seg;
  }
  var i_amount = parseInt(s_amount, 10);
  // </Extract amount>
  // </Extract information from udp packet>
  // <Handle request commands>
  if(i_cmd == 1){
    // request command: Create customer
    try {
      impl_API.createCustomer(s_name, i_PIN, s_date, i_num_acc, i_num_trans, function(result){

        var stuff_i_want = result;  // json obj

        if(isEmpty(stuff_i_want)){
          // empty JSON, so create JSON with success code = 2
          let resp_empty = '{"success":2}';
          const message = Buffer.from(resp_empty);
          server.send(message, rinfo.port, rinfo.address, (err) => {
            // sending to client
          });
        } else {
          // not empty

          // add success status
          let resp_success = '{\"success\": 1}';
          var json_resp = JSON.parse(resp_success);
          stuff_i_want.unshift(json_resp);
          console.log(stuff_i_want);

          // convert JSON to string
          let jsonStr = JSON.stringify(stuff_i_want);
          console.log("SocketLog: create customer JSON = " + jsonStr);
          // send response
          //const message = Buffer.from('Some bytes');
          const message = Buffer.from(jsonStr);
          server.send(message, rinfo.port, rinfo.address, (err) => {
            // sending to client
          });
        }

      });

    } catch (e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  }
  else if(i_cmd == 2){
    // request command: select all customers
    try {
      impl_API.getCustomers(-2, function(result){
        var stuff_i_want = result;  // json obj

        if(isEmpty(stuff_i_want)){
          // empty JSON, so create JSON with success code = 2
          let resp_empty = '{"success":2}';
          const message = Buffer.from(resp_empty);
          server.send(message, rinfo.port, rinfo.address, (err) => {
            // sending to client
          });
        } else {
          let resp_success = '{\"success\": 1}';
          var json_resp = JSON.parse(resp_success);
          stuff_i_want.unshift(json_resp);
          console.log(stuff_i_want);

          // convert JSON to string
          let jsonStr = JSON.stringify(stuff_i_want);

          console.log("SocketLog: view all customer JSON = " + jsonStr);
          // send response
          //const message = Buffer.from('Some bytes');
          const message = Buffer.from(jsonStr);
          server.send(message, rinfo.port, rinfo.address, (err) => {
            //console.log(rinfo.address);
            //console.log(rinfo.port);
            //console.log(message);

            //server.close();
          });
        }
      });
    } catch (e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }

  }
  else if(i_cmd == 3){
    // request command: select customer
    try {
      impl_API.getCustomers(i_ID, function(result){
        var stuff_i_want = result;  // json obj

        if(isEmpty(stuff_i_want)){
          // empty JSON, so create JSON with success code = 2
          let resp_empty = '[{"success":2}]';
          const message = Buffer.from(resp_empty);
          server.send(message, rinfo.port, rinfo.address, (err) => {
            // sending to client
          });
        } else {

          let resp_success = '{\"success\": 1}';
          var json_resp = JSON.parse(resp_success);
          stuff_i_want.unshift(json_resp);
          console.log(stuff_i_want);

          // convert JSON to string
          let jsonStr = JSON.stringify(stuff_i_want);
          console.log("SocketLog: view selected customer JSON = " + jsonStr);
          // send response
          //const message = Buffer.from('Some bytes');
          const message = Buffer.from(jsonStr);
          server.send(message, rinfo.port, rinfo.address, (err) => {
            //console.log(rinfo.address);
            //console.log(rinfo.port);
            //console.log(message);

            //server.close();
          });
        }
      });

    } catch(e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  }
  else if(i_cmd == 4){
    // update a customer row
    try {
      impl_API.updateCustomer(i_ID, s_name, i_PIN, i_num_acc, i_num_trans, function(result){
        var stuff_i_want = parseInt(result, 10);  // json obj
        let resp_success = '[{\"success\": 3}, {\"affectedRows\":' + stuff_i_want + '}]';

        console.log(resp_success);

        const message = Buffer.from(resp_success);
        server.send(message, rinfo.port, rinfo.address, (err) => {
          //console.log(rinfo.address);
          //console.log(rinfo.port);
          //console.log(message);

          //server.close();
        });
      });
    } catch (e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  } else if(i_cmd == 5){
    // request command: remove customer
    try {
      impl_API.removeCustomer(i_ID, function(result){
        var stuff_i_want = parseInt(result, 10);;  // int
        let resp_success = '[{\"success\": 3}, {\"affectedRows\":' + stuff_i_want + '}]';

        console.log(resp_success);

        const message = Buffer.from(resp_success);
        server.send(message, rinfo.port, rinfo.address, (err) => {
          //console.log(rinfo.address);
          //console.log(rinfo.port);
          //console.log(message);

          //server.close();
        });
      });
    } catch (e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  } else if(i_cmd == 6){
    // request command: get all accounts (-2) or to specific account ID (sent through i_SRC segment)
    try {
      impl_API.getAllAccounts(i_accID, function(result){
        var stuff_i_want = result;  // json obj
        let resp_success = '{\"success\": 1}';
        var json_resp = JSON.parse(resp_success);
        stuff_i_want.unshift(json_resp);
        console.log(stuff_i_want);

        // convert JSON to string
        let jsonStr = JSON.stringify(stuff_i_want);
        console.log("SocketLog: view all customer JSON = " + jsonStr);
        // send response
        //const message = Buffer.from('Some bytes');
        const message = Buffer.from(jsonStr);
        server.send(message, rinfo.port, rinfo.address, (err) => {
          //console.log(rinfo.address);
          //console.log(rinfo.port);
          //console.log(message);

          //server.close();
        });

      });
    } catch (e) {
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  } else if(i_cmd == 7){
    // request command: get all accounts associated to owner ID
    try {
      impl_API.getCustAccounts(i_ID, function(result){
        var stuff_i_want = result;  // json obj
        let resp_success = '{\"success\": 1}';
        var json_resp = JSON.parse(resp_success);
        stuff_i_want.unshift(json_resp);
        console.log(stuff_i_want);

        // convert JSON to string
        let jsonStr = JSON.stringify(stuff_i_want);
        console.log("SocketLog: view all customer JSON = " + jsonStr);
        // send response
        //const message = Buffer.from('Some bytes');
        const message = Buffer.from(jsonStr);
        server.send(message, rinfo.port, rinfo.address, (err) => {
          //console.log(rinfo.address);
          //console.log(rinfo.port);
          //console.log(message);

          //server.close();
        });

      });
    } catch (e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  } else if(i_cmd == 8){
    // request command: update account
    try {
      impl_API.updateAccount(i_ownerID, i_accID, i_accAMT, function(result){
        var stuff_i_want = parseInt(result, 10);;  // json obj
        let resp_success = '[{\"success\": 3}, {\"affectedRows\":' + stuff_i_want + '}]';

        console.log(resp_success);

        const message = Buffer.from(resp_success);
        server.send(message, rinfo.port, rinfo.address, (err) => {
          //console.log(rinfo.address);
          //console.log(rinfo.port);
          //console.log(message);

          //server.close();
        });
      });
    } catch (e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  } else if(i_cmd == 9){

    // request command: remove specific account
    try {
      impl_API.removeAccount(i_accID, function(result){
        var stuff_i_want = parseInt(result, 10);;  // json obj
        let resp_success = '[{\"success\": 3}, {\"affectedRows\":' + stuff_i_want + '}]';

        console.log(resp_success);

        const message = Buffer.from(resp_success);
        server.send(message, rinfo.port, rinfo.address, (err) => {
          //console.log(rinfo.address);
          //console.log(rinfo.port);
          //console.log(message);

          //server.close();
        });
      });
    } catch (e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  } else if(i_cmd == 10){
    // request command: Create account
    try {
      impl_API.createAccount(i_ID, s_accdate, i_accAMT, function(result){
        var stuff_i_want = result;  // json obj
        let resp_success = '{\"success\": 1}';
        var json_resp = JSON.parse(resp_success);
        stuff_i_want.unshift(json_resp);
        console.log(stuff_i_want);

        // convert JSON to string
        let jsonStr = JSON.stringify(stuff_i_want);
        console.log("SocketLog: create customer JSON = " + jsonStr);
        // send response
        //const message = Buffer.from('Some bytes');
        const message = Buffer.from(jsonStr);
        server.send(message, rinfo.port, rinfo.address, (err) => {
          // sending to client
        });
      });
    } catch (e){
      // send error code indicating API error
      let s_e = e.toString();
      let resp_error = '[{"success":0, "err_message":' + s_e + '}]';
      const message = Buffer.from(resp_error);
      server.send(message, rinfo.port, rinfo.address, (err) => {
        // sending to client
      });
    }
  }
   else if (i_cmd == 50){
    var close_msg = "Server closing ...";
    server.send(close_msg, rinfo.port, rinfo.address, (err) => {
      //console.log(rinfo.address);
      //console.log(rinfo.port);
      console.log(close_msg);
      server.close();
      process.exit()
    });
  }
  // </Handle request commands>

});
// </Handle receiving messages>

// <Put server in listening mode>
server.on('listening', () => {
  const address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
});
// </Put server in listening mode>

server.bind(8080);  // bind to a port to receive on
