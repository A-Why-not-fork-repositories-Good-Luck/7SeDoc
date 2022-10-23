package aufgabe1.zustaende;

public class Geschlossen implements Zustand {
    @Override
    public Zustand oeffnen() {
        System.out.println("Tür geöffnet! Zustand gewechselt!");
        return new Offen();
    }

    @Override
    public Zustand schliessen() {
        System.out.println("Tür bereits geschlossen!");
        return this;
    }

    @Override
    public Zustand aufschliessen() {
        System.out.println("Tür ist nicht abgeschlossen!");
        return this;
    }

    @Override
    public Zustand abschliessen() {
        System.out.println("Tür abgeschlossen! Zustand gewechselt!");
        return new Abgeschlossen();
    }
}
