import type { NextApiRequest, NextApiResponse } from "next";
import mysql from "mysql2/promise";

export default async function handler(req: NextApiRequest, res: NextApiResponse) 
{
    //const mysql = require('mysql2');

    const dbconnection = await mysql.createConnection({
        host: "localhost",
        database: "heisematrixdb",
        //port: "3306",
        user: "root",
        password: "password"
    });

    try {
        const query = "SELECT * FROM user WHERE user_id = ?";
        const values = [1];
        const [data] = await dbconnection.execute(query, values);
        dbconnection.end();

        res.status(200).json({ result: data });
        
    } catch (error) {
        res.status(500).json({ error: error.message});
    }
}
