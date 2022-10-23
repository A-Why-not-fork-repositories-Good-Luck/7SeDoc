package aufgabe3_3.commands;

import aufgabe3_3.reciever.Beleuchtungssteuerung;

public class LichtAnschalten implements Command {
    private Beleuchtungssteuerung beleuchtung;

    public LichtAnschalten(Beleuchtungssteuerung beleuchtung) {
        this.beleuchtung = beleuchtung;
    }

    @Override
    public void execute() {
        this.beleuchtung.anschalten();
    }
}
