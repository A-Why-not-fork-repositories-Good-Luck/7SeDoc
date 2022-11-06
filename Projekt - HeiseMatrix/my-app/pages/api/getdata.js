export default function handler(req, res) {
    const mysql = require("mysql2");

    const connection = mysql.createConnection({
        host: "localhost",
        database: "heisematrixdb",
        //port: "3306",
        user: "root",
        password: "password",
    });

    const query = "SELECT * FROM user WHERE user_id = ?";
    const values = [1];

    try {
        connection.execute(query, values, function (err, results, fields) {
            console.log(results);
            res.status(200).json({ data: results });
        });
    } catch (error) {
        res.status(500).json({ error: error.message });
    }

    //res.status(200).json({ name: "John Doe" });
}
