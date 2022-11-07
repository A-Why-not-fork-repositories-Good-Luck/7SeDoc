// Next.js API route support: https://nextjs.org/docs/api-routes/introduction
import type { NextApiRequest, NextApiResponse } from "next";
import { PrismaClient } from "@prisma/client";

const prisma = new PrismaClient();

type Data = {
    name: string;
};

async function main() {
    const allUsers = await prisma.user.findFirst();
    console.log(allUsers);
}

main()
    .then(async () => {
        await prisma.$disconnect();
    })
    .catch(async (e) => {
        console.error(e);
        await prisma.$disconnect();
        process.exit(1);
    });

export default function handler(
    req: NextApiRequest,
    res: NextApiResponse<Data>
) {
    console.log('TEST1');
    //main();
    console.log('TEST3');
    res.status(200).json({ name: "John Doe" });
}
