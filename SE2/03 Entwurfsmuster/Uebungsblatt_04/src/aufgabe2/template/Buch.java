package aufgabe2.template;

import aufgabe2.Benutzer;

public abstract class Buch {
    private int frist;
    abstract int rueckgabe();

    public void leiheAus(Benutzer b) {
        if (b.isBerechtigt()) {
            this.frist = this.rueckgabe();
            System.out.println("Frist liegt bei " + frist + " Tagen!");
        }
    }

    public int getFrist() {
        return frist;
    }

    public void setFrist(int frist) {
        this.frist = frist;
    }
}
