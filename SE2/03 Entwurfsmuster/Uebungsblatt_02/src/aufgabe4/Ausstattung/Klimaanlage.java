package aufgabe4.Ausstattung;

import aufgabe4.Modelle.Grundmodell;

public class Klimaanlage extends Sonderausstattungen{

    public Klimaanlage(Grundmodell extra) {
        super(extra);
    }

    @Override
    public double getPreis() {
        return this.extra.getPreis() + 5;
    }

    @Override
    public String getBeschreibung() {
        return this.extra.getBeschreibung() + " und eine Klimaanlage";
    }
}
