var mysql = require('mysql');
var db = mysql.createConnection({
    host: 'localhost',
    user: 'nodejs',
    password: '55374586',
    database: 'opentutorials',
    port: '3306'
  });
  db.connect();
  module.exports = db;