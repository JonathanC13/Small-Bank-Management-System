const mysql = require('C:\\Users\\Jonathan\\Desktop\\NodeJs Tutorial\\node_modules\\mysql'); // sql lib

'use strict';

// Database table columns
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

// <Close connection>
function closeSQL(con){
  con.end();
}
// </Close connection>

exports.createCustomer = function(name, pin, date, num_acc, num_trans, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: createCustomer, could not connect to database.");
      console.log(err);
      throw err;
    }
    console.log("log: createCustomer: connected to DB.");
    con.query("SELECT id_cust FROM customers", function (err, rows, result, fields) {
      if (err){
        console.log("Error: createCustomer select ID query");
        console.log(err);
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
          console.log("Error: createCustomer insert query:");
          console.log(err);
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
      console.log(err);
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
        console.log(err);
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

// "id_cust", "nm_cust", "pin_cust", "datecreated_cust", "num_accounts_cust", "num_trans_cust"
exports.updateCustomer = function(id, name, pin, num_acc, num_trans, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: updateCustomer, could not connect to database.");
      console.log(err);
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
    var q = con.query(sql_q, params, function (err, result, fields) {
      if (err){
        console.log(q.sql);
        console.log("Error: updateCustomer query.");
        console.log(err);
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
      console.log(err);
      throw err;
    }
    console.log("log: removeCustomer: connected to DB.");

    let sql_del = "DELETE FROM customers WHERE ?? = ?";
    const params = [cust_columns[0], id];
    con.query(sql_del, params, function (err, result, fields) {
      if (err){
        console.log("Error: removeCustomer delete query.");
        console.log(err);
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
      console.log(err);
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
        console.log(err);
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
      console.log(err);
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
        console.log(err);
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
      console.log(err);
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
        console.log(err);
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
      console.log(err);
      throw err;
    }
    console.log("log: removeAccount: connected to DB.");

    let sql_del = "DELETE FROM accounts WHERE ?? = ?";
    let params = [];
    params.push(acc_columns[1], id);
    con.query(sql_del, params, function (err, result, fields) {
      if (err){
        console.log("Error: removeAccount delete query.");
        console.log(err);
        throw err;
      }
      return callback(result.affectedRows);
      //console.log(result);
    });
    console.log("log: removeAccount: Complete.");
    closeSQL(con);
  });
}

exports.createAccount = function(owner_id, datecr_acc, accountAmt, callback){
  const con = connectDBSQL();

  con.connect(function(err) {
    if (err){
      console.log("Error: createAccount, could not connect to database.");
      console.log(err);
      throw err;
    }
    console.log("log: createAccount: connected to DB.");
    //
    // to determine a unique account ID, get the highest current ID first
    con.query("SELECT id_account FROM accounts", function (err, rows, result, fields) {
      if (err){
        console.log("Error: createAccount select account ID query");
        console.log(err);
        throw err;
      }
      let max = 0;
      rows.forEach(function(row) {
        console.log(row.id_account);
        if(row.id_account > max){
          max = row.id_account;
        }
      });

      let assignedID = max + 1;
      console.log("assigned ID: " + assignedID);


      let sql_ins = "INSERT INTO accounts (??,??,??,??) VALUES (?,?,?,?)";
      const vals = [owner_id, assignedID, datecr_acc, accountAmt];
      const params = [];
      //["id_owner", "id_account", "datecreated_acc", "amount_acc"];
      params.push(... acc_columns, ... vals);

      var q = con.query(sql_ins, params, function (err, result, fields) {
        if (err){
          console.log(q.sql);
          console.log("Error: createAccount insert query");
          console.log(err);
          throw err;
        }

        exports.getAllAccounts(assignedID, function(result){

          // handle falsey values
          result[0].id_owner = result[0].id_owner || 0;
          result[0].id_account = result[0].id_account || 0;
          if(result[0].datecreated_cust == 0){
            result[0].datecreated_cust = "0";
          }
          result[0].amount_acc = result[0].amount_acc || 0;

          //let jsonStr = JSON.stringify(result);
          //console.log("JJJJ " + jsonStr);
          return callback(result);
        });

      });

      console.log("log: createAccount: Complete.");
      closeSQL(con);
    });
  });
}
