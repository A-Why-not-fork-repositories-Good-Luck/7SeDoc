package aufgabe1.objects;

import aufgabe1.zustaende.*;

public class Tuer {
    private Zustand zustand = new Offen();

    public void oeffnen() {
        zustand = this.zustand.oeffnen();
    }

    public void schliessen() {
        zustand = this.zustand.schliessen();
    }

    public void aufschliessen() {
        zustand = this.zustand.aufschliessen();
    }

    public void abschliessen() {
        zustand = this.zustand.abschliessen();
    }

    public void setZustand(Zustand zustand) {
        this.zustand = zustand;
    }
}
