package aufgabe2.strategy;

public class prog {
    public static void main(String[] args) {
        Normal normal = new Normal();
        Vorgemerkt vorgemerkt = new Vorgemerkt();
        Praesenz praesenz = new Praesenz();

        Buch_2 buch_1 = new Buch_2(normal);
        Buch_2 buch_2 = new Buch_2(vorgemerkt);
        Buch_2 buch_3 = new Buch_2(praesenz);

        buch_1.rueckgabe();
        buch_2.rueckgabe();
        buch_3.rueckgabe();
    }
}
