const mysql = require('C:\\Users\\Jonathan\\Desktop\\NodeJs Tutorial\\node_modules\\mysql'); // sql lib

'use strict';

const cust_columns = ["id_cust", "nm_cust", "pin_cust", "datecreated_cust", "num_accounts_cust", "num_trans_cust"];
const acc_columns = ["id_owner", "id_account", "datecreated_acc", "amount_acc"];
// create a connection to the database
// <Create connection>
function connectDBSQL(dbName){
  var con = mysql.createConnection({
    host: "localhost",
    user: "foo",
    password: "bar",
    database: "bankdb"
  });
  return con;
}
// </Create connection>

function closeSQL(con){
  con.end();
}

exports.createCustomer = function(name, pin, date, num_acc, num_trans, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: createCustomer, could not connect to database.");
      throw err;
    }
    console.log("log: createCustomer: connected to DB.");
    con.query("SELECT id_cust FROM customers", function (err, rows, result, fields) {
      if (err){
        console.log("Error: createCustomer select ID query");
        throw err;
      }
      let max = 0;
      rows.forEach(function(row) {
        console.log(row.id_cust);
        if(row.id_cust > max){
          max = row.id_cust;
        }
      });

      let assignedID = max + 1;
      console.log("assigned ID: " + assignedID);


      let sql_ins = "INSERT INTO customers (??,??,??,??,??,??) VALUES (?,?,?,?,?,?)";
      const vals = [assignedID, name, pin, date, num_acc, num_trans];
      const params = [];
      //params.push(cust_columns[0], cust_columns[1], cust_columns[2], cust_columns[3],cust_columns[4], cust_columns[5], assignedID, name, pin, date, 0, 0);
      params.push(...cust_columns, ...vals);

      con.query(sql_ins, params, function (err, result, fields) {
        if (err){
          console.log("Error: createCustomer insert query");
          throw err;
        }

        exports.getCustomers(assignedID, function(result){

          // handle falsey values
          result[0].id_cust = result[0].id_cust || 0;
          result[0].nm_cust = result[0].nm_cust || 0;
          if(result[0].nm_cust == 0){
            result[0].nm_cust = "0";
          }
          result[0].pin_cust = result[0].pin_cust || 0;
          result[0].datecreated_cust = result[0].datecreated_cust || 0;
          if(result[0].datecreated_cust == 0){
            result[0].datecreated_cust = "0";
          }
          result[0].num_accounts_cust = result[0].num_accounts_cust || 0;
          result[0].num_trans_cust = result[0].num_trans_cust || 0;

          //let jsonStr = JSON.stringify(result);
          //console.log("JJJJ " + jsonStr);
          return callback(result);
        });

      });

      console.log("log: createCustomer: Complete.");
      closeSQL(con);
    });
  });
}

exports.getCustomers = function(id, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: getCustomers, could not connect to database.");
      throw err;
    }
    var sql_q = "SELECT * FROM customers";
    var sql_where = " WHERE ?? = ?";
    var params = [];
    let i_id = parseInt(id, 10);
    if(i_id != -2){
      sql_q = sql_q + sql_where;
      params.push(cust_columns[0], i_id);
    }

    console.log("id = " + i_id + ". sql for select: " + sql_q);
    console.log(params);

    console.log("log: getCustomers: connected to DB.");
    con.query(sql_q, params, function (err, result, fields) {
      if (err){
        console.log("Error: getCustomers select query.");
        throw err;
      }

      let i;
      var res_len = result.length;
      for (i = 0; i < res_len; i++)
      {
        // handle falsey values
        result[i].id_cust = result[i].id_cust || 0;
        result[i].nm_cust = result[i].nm_cust || 0;
        if(result[i].nm_cust == 0){
          result[i].nm_cust = "0";
        }
        result[i].pin_cust = result[i].pin_cust || 0;
        result[i].datecreated_cust = result[i].datecreated_cust || 0;
        if(result[i].datecreated_cust == 0){
          result[i].datecreated_cust = "0";
        }
        result[i].num_accounts_cust = result[i].num_accounts_cust || 0;
        result[i].num_trans_cust = result[i].num_trans_cust || 0;
      }

      return callback(result);
      //console.log(result);
    });
    console.log("log: getCustomers: Complete.");
    closeSQL(con);
  });
}

// to be remove
/*
exports.selectCustomer = function(id, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: selectCustomer, could not connect to database.");
      throw err;
    }
    console.log("log: selectCustomer: connected to DB.");
    let sql_sel = "SELECT * FROM customers WHERE ?? = ?"
    const params = [cust_columns[0], id];

    con.query(sql_sel, params, function (err, result, fields) {
      if (err){
        console.log("Error: selectCustomer select query.");
        throw err;
      }
      return callback(result);
      //console.log(result);
    });
    console.log("log: selectCustomer: Complete.");
    closeSQL(con);
  });
}
*/
// "id_cust", "nm_cust", "pin_cust", "datecreated_cust", "num_accounts_cust", "num_trans_cust"
// "id_cust", "nm_cust", "pin_cust", "datecreated_cust", "num_accounts_cust", "num_trans_cust"
exports.updateCustomer = function(id, name, pin, num_acc, num_trans, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: updateCustomer, could not connect to database.");
      throw err;
    }

    // query update

    var name_s = name.toString();
    var id_int = parseInt(id, 10);

    let sql_q = "UPDATE customers SET ?? = ?, ?? = ?, ?? = ?, ?? = ? WHERE ?? = ?";
    //let sql_q = "UPDATE customers SET "+cust_columns[1]+" = "+name+", "+cust_columns[2]+" = "+pin+", "+cust_columns[4]+" = "+num_acc+", "+cust_columns[5]+" = "+num_trans+" WHERE "+cust_columns[0]+" = "+id;
    // ref  cust_columns = ["id_cust", "nm_cust", "pin_cust", "datecreated_cust", "num_accounts_cust", "num_trans_cust"];
    params = [];
    params.push(cust_columns[1], name_s, cust_columns[2], pin, cust_columns[4], num_acc, cust_columns[5], num_trans, cust_columns[0], id_int);
    let sql_whole = mysql.format(sql_q, params);
    console.log("params: " + sql_whole);
    con.query(sql_q, params, function (err, result, fields) {
      if (err){
        console.log("Error: updateCustomer query.");
        throw err;
      }
      return callback(result.affectedRows);
      //console.log(result);
    });

    console.log("log: updateCustomer: Complete.");
    closeSQL(con);
  });

}

exports.removeCustomer = function(id, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: removeCustomer, could not connect to database.");
      throw err;
    }
    console.log("log: removeCustomer: connected to DB.");

    let sql_del = "DELETE FROM customers WHERE ?? = ?";
    const params = [cust_columns[0], id];
    con.query(sql_del, params, function (err, result, fields) {
      if (err){
        console.log("Error: removeCustomer delete query.");
        throw err;
      }
      return callback(result.affectedRows);
      //console.log(result);
    });
    console.log("log: removeCustomer: Complete.");
    closeSQL(con);
  });
}

exports.getAllAccounts = function(id, callback){
  const con = connectDBSQL();

  //console.log("id = " + i_id + ". sql for select: " + sql_q);
  //console.log(params);

  con.connect(function(err) {
    if (err){
      console.log("Error: getAllAccounts, could not connect to database.");
      throw err;
    }

    var sql_q = "SELECT * FROM accounts";
    var sql_where = " WHERE ?? = ?";
    var params = [];
    let i_id = parseInt(id, 10);
    if(i_id != -2){
      sql_q = sql_q + sql_where;
      params.push(acc_columns[1], i_id);
    }

    console.log("log: getAllAccounts: connected to DB.");
    con.query(sql_q, params, function (err, result, fields) {
      if (err){
        console.log("Error: getAllAccounts select query.");
        throw err;
      }
      return callback(result);
      //console.log(result);
    });
    console.log("log: getAllAccounts: Complete.");
    closeSQL(con);
  });
}

exports.getCustAccounts = function(ownerID, callback){
  const con = connectDBSQL();

  //console.log("id = " + i_id + ". sql for select: " + sql_q);
  //console.log(params);

  con.connect(function(err) {
    if (err){
      console.log("Error: getCustAccounts, could not connect to database.");
      throw err;
    }

    var sql_q = "SELECT * FROM accounts";
    var sql_where = " WHERE ?? = ?";
    var params = [];
    let i_id = parseInt(ownerID, 10);
    if(i_id != -2){
      sql_q = sql_q + sql_where;
      params.push(acc_columns[0], ownerID);
    }

    console.log("log: getCustAccounts: connected to DB.");
    con.query(sql_q, params, function (err, result, fields) {
      if (err){
        console.log("Error: getCustAccounts select query.");
        throw err;
      }
      return callback(result);
      //console.log(result);
    });
    console.log("log: getCustAccounts: Complete.");
    closeSQL(con);
  });
}

exports.updateAccount = function(ownerID, accID, tot_AMT, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: updateAccount, could not connect to database.");
      throw err;
    }

    // query update

    var i_ownerID = parseInt(ownerID, 10);
    var i_tot_AMT = parseInt(tot_AMT, 10);

    let sql_q = "UPDATE accounts SET ?? = ?, ?? = ? WHERE ?? = ?";
    params = [];
    params.push(acc_columns[0], i_ownerID, acc_columns[3], i_tot_AMT, acc_columns[1], accID);
    let sql_whole = mysql.format(sql_q, params);
    console.log("params: " + sql_whole);
    con.query(sql_q, params, function (err, result, fields) {
      if (err){
        console.log("Error: updateAccount query.");
        throw err;
      }
      return callback(result.affectedRows);
      //console.log(result);
    });

    console.log("log: updateAccount: Complete.");
    closeSQL(con);
  });

}

exports.removeAccount = function(id, callback){
  const con = connectDBSQL();

  //console.log("acc id passed: " + id);
  con.connect(function(err) {
    if (err){
      console.log("Error: removeAccount, could not connect to database.");
      throw err;
    }
    console.log("log: removeAccount: connected to DB.");

    let sql_del = "DELETE FROM accounts WHERE ?? = ?";
    let params = [];
    params.push(acc_columns[1], id);
    con.query(sql_del, params, function (err, result, fields) {
      if (err){
        console.log("Error: removeAccount delete query.");
        throw err;
      }
      return callback(result.affectedRows);
      //console.log(result);
    });
    console.log("log: removeAccount: Complete.");
    closeSQL(con);
  });
}

//<transfers>
// in c++
//  if local, it gets both accounts exist and checks if same ownerID, if yes send the transfer request
//  if global, just check if both accounts exist
exports.amountTransfer = function(toAcc, fromAcc, ammount, callback){
  //todo
  // with the source acc ID and dest acc ID get the rows.
}
//</transfers>
