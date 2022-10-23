package aufgabe2.template;

import aufgabe2.Benutzer;

public class prog {
    public static void main(String[] args) {
        Benutzer b = new Benutzer(true);

        Buch_Normal b_normal = new Buch_Normal();
        b_normal.leiheAus(b);

        Buch_Vorgemerkt b_vorgemerkt = new Buch_Vorgemerkt();
        b_vorgemerkt.leiheAus(b);

        Buch_Präsenz b_praesenz = new Buch_Präsenz();
        b_praesenz.leiheAus(b);
    }
}
