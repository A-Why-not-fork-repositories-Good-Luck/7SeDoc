package aufgabe2.strategy;

public class Buch_2 {
    private int frist;
    private Ausleihe ausleihe;

    public Buch_2(Ausleihe ausleihe) {
        this.ausleihe = ausleihe;
    }

    public void rueckgabe() {
        this.frist = this.ausleihe.rueckgabe();
        System.out.println("Frist liegt bei " + frist + " Tagen!");
    }

    public void setAusleihe(Ausleihe ausleihe) {
        this.ausleihe = ausleihe;
    }
}
