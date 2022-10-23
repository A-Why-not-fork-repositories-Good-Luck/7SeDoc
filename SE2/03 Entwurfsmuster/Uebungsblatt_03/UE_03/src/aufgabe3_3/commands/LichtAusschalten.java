package aufgabe3_3.commands;

import aufgabe3_3.reciever.Beleuchtungssteuerung;

public class LichtAusschalten implements Command {
    private Beleuchtungssteuerung beleuchtung;

    public LichtAusschalten(Beleuchtungssteuerung beleuchtung) {
        this.beleuchtung = beleuchtung;
    }

    @Override
    public void execute() {
        this.beleuchtung.ausschalten();
    }
}
