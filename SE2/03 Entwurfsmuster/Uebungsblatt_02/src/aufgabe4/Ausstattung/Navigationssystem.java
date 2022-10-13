package aufgabe4.Ausstattung;

import aufgabe4.Modelle.*;

public class Navigationssystem extends Sonderausstattungen {

    public Navigationssystem(Grundmodell extra) {
        super(extra);
    }

    @Override
    public double getPreis() {
        return this.extra.getPreis() + 9;
    }

    @Override
    public String getBeschreibung() {
        return this.extra.getBeschreibung() + " und ein Navigationssystem";
    }
}
