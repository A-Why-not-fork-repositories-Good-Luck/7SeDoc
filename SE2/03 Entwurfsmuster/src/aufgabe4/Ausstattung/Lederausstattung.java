package aufgabe4.Ausstattung;

import aufgabe4.Modelle.Grundmodell;

public class Lederausstattung extends Sonderausstattungen{

    public Lederausstattung(Grundmodell extra) {
        super(extra);
    }

    @Override
    public double getPreis() {
        return this.extra.getPreis() + 7;
    }

    @Override
    public String getBeschreibung() {
        return this.extra.getBeschreibung() + " und eine Lederausstattung";
    }
}
