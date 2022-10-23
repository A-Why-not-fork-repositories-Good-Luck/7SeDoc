package aufgabe1.zustaende;

public class Offen implements Zustand {
    @Override
    public Zustand oeffnen() {
        System.out.println("Tür bereits offen!");
        return this;
    }

    @Override
    public Zustand schliessen() {
        System.out.println("Tuer geschlossen! Zustand gewechselt!");
        return new Geschlossen();
    }

    @Override
    public Zustand aufschliessen() {
        System.out.println("Tür ist nicht abgeschlossen!");
        return this;
    }

    @Override
    public Zustand abschliessen() {
        System.out.println("Tür noch offen!");
        return this;
    }
}
