package nr3;

public class prog {
    public static void main(String[] args) {
        aType at1 = new aType("Thinkpad Docking Station");

        nType nt1 = new nType("ThnikPad");
        nType nt2 = new nType("Apple MacBook");

        Austattung as1 = new Austattung("aus1", at1);

        Notebook nb1 = new Notebook("nb1" , nt1);
        Notebook nb2 = new Notebook("nb2", nt2);

        nt1.addAllowed(at1);

        System.out.println(nb1.addPart(as1));
        System.out.println(nb2.addPart(as1));
    }
}
