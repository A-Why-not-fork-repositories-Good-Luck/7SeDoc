package aufgabe4.Ausstattung;

import aufgabe4.Modelle.Grundmodell;

public abstract class Sonderausstattungen implements Grundmodell {
    protected Grundmodell extra;

    public Sonderausstattungen(Grundmodell extra) {
        this.extra = extra;
    }
}
