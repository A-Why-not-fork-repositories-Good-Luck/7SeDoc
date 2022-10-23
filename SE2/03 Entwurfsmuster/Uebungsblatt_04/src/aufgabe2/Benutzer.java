package aufgabe2;

public class Benutzer {
    private boolean berechtigt;

    public Benutzer(boolean berechtigt) {
        this.berechtigt = berechtigt;
    }

    public boolean isBerechtigt() {
        return berechtigt;
    }
}
