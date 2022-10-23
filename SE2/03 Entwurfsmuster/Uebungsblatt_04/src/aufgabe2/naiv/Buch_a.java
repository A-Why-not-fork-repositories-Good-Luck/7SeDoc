package aufgabe2.naiv;

import aufgabe2.Benutzer;

public class Buch_a {
    private String type;
    private int frist;

    public Buch_a(String type) {
        this.type = type;
    }

    public void leiheAus(Benutzer b) {
        if (b.isBerechtigt()) {
            switch (type) {
                case "Normal":
                    this.frist = 4 * 7;
                    System.out.println(frist);
                    break;
                case "Vorgemerkt":
                    this.frist = 2 * 7;
                    System.out.println(frist);
                    break;
                case "Präsenz":
                    this.frist = 1;
                    System.out.println(frist);
                    break;
            }
        }
    }
}
