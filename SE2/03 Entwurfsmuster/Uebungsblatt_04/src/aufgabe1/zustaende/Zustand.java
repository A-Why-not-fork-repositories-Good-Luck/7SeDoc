package aufgabe1.zustaende;

public interface Zustand {
    Zustand oeffnen();
    Zustand schliessen();
    Zustand aufschliessen();
    Zustand abschliessen();
}
