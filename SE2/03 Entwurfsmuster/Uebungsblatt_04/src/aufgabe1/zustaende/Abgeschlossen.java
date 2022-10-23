package aufgabe1.zustaende;

public class Abgeschlossen implements Zustand {
    @Override
    public Zustand oeffnen() {
        System.out.println("Tür ist noch abgeschlossen!");
        return this;
    }

    @Override
    public Zustand schliessen() {
        System.out.println("Tür bereits geschlossen und abgeschlossen!");
        return this;
    }

    @Override
    public Zustand aufschliessen() {
        System.out.println("Tür aufgeschlossen! Zustand gewechselt!");
        return new Geschlossen();
    }

    @Override
    public Zustand abschliessen() {
        System.out.println("Tür bereits abgeschlossen!");
        return this;
    }
}
