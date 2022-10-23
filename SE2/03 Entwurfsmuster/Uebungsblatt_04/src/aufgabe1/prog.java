package aufgabe1;

import aufgabe1.objects.Tuer;

public class prog {
    public static void main(String[] args) {
        Tuer tuer = new Tuer();

        tuer.oeffnen();
        tuer.abschliessen();
        tuer.aufschliessen();
        tuer.schliessen();

        tuer.schliessen();
        tuer.aufschliessen();
        tuer.oeffnen();

        tuer.schliessen();

        tuer.abschliessen();

        tuer.oeffnen();
        tuer.schliessen();
        tuer.abschliessen();
        tuer.aufschliessen();
    }
}
